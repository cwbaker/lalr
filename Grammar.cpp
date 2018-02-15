//
// Grammar.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "Grammar.hpp"
#include "GrammarDirective.hpp"
#include "GrammarSymbol.hpp"
#include "GrammarProduction.hpp"
#include "GrammarAction.hpp"
#include "LalrGenerator.hpp"
#include "LalrGrammar.hpp"
#include "LalrAction.hpp"
#include "LalrState.hpp"
#include "ParserState.hpp"
#include "ParserAction.hpp"
#include "ParserSymbol.hpp"
#include "ParserTransition.hpp"
#include "ParserStateMachine.hpp"
#include "LexerStateMachine.hpp"
#include "LexerTokenType.hpp"
#include "assert.hpp"

using std::string;
using std::vector;
using std::set;
using std::shared_ptr;
using std::unique_ptr;
using namespace sweet;
using namespace sweet::lalr;
using namespace sweet::lalr;

Grammar::Grammar()
: identifier_(),
  directives_(),
  symbols_(),
  productions_(),
  actions_(),
  whitespace_tokens_(),
  active_directive_( nullptr ),
  active_production_( nullptr ),
  active_symbol_( nullptr )
{
}

const std::vector<std::shared_ptr<GrammarDirective>>& Grammar::directives() const
{
    return directives_;
}

const std::vector<std::shared_ptr<GrammarSymbol>>& Grammar::symbols() const
{
    return symbols_;
}

const std::vector<std::shared_ptr<GrammarProduction>>& Grammar::productions() const
{
    return productions_;
}

const std::vector<LexerToken>& Grammar::whitespace_tokens() const
{
    return whitespace_tokens_;
}

Grammar& Grammar::begin()
{
    return *this;
}

Grammar& Grammar::left()
{
    active_whitespace_directive_ = false;
    active_directive_ = directive( ASSOCIATE_LEFT );
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::right()
{
    active_whitespace_directive_ = false;
    active_directive_ = directive( ASSOCIATE_RIGHT );
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::none()
{
    active_whitespace_directive_ = false;
    active_directive_ = directive( ASSOCIATE_NONE );
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::whitespace()
{
    active_whitespace_directive_ = true;
    active_directive_ = nullptr;
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::precedence( char literal )
{
    SWEET_ASSERT( active_production_ );
    if ( active_production_ )
    {
        active_production_->set_precedence_symbol( symbol(literal) );
    }
    return *this;
}

Grammar& Grammar::precedence( const char* regex )
{
    SWEET_ASSERT( active_production_ );
    if ( active_production_ )
    {
        active_production_->set_precedence_symbol( symbol(regex) );
    }
    return *this;
}

Grammar& Grammar::production( const char* identifier )
{
    SWEET_ASSERT( identifier );
    active_whitespace_directive_ = false;
    active_directive_ = nullptr;
    active_production_ = nullptr;
    active_symbol_ = symbol( identifier );
    return *this;
}

Grammar& Grammar::end_production()
{
    SWEET_ASSERT( active_symbol_ );
    active_whitespace_directive_ = false;
    active_directive_ = nullptr;
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::operator()( char literal )
{
    SWEET_ASSERT( active_whitespace_directive_ || active_directive_ || active_symbol_ );
    if ( active_whitespace_directive_ )
    {
        char lexeme [2] = { literal, 0 };
        whitespace_tokens_.push_back( LexerToken(TOKEN_LITERAL, 0, nullptr, lexeme) );
    }
    else if ( active_directive_ )
    {
        active_directive_->append_symbol( symbol(literal) );
    }
    else if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = production( active_symbol_ );
        }
        active_production_->append_symbol( symbol(literal) );
    }
    return *this;
}

Grammar& Grammar::operator()( const char* regex )
{
    SWEET_ASSERT( active_whitespace_directive_ || active_directive_ || active_symbol_ );
    if ( active_whitespace_directive_ )
    {
        whitespace_tokens_.push_back( LexerToken(TOKEN_REGULAR_EXPRESSION, 0, nullptr, regex) );
    }
    else if ( active_directive_ )
    {
        active_directive_->append_symbol( symbol(regex) );
    }
    else if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = production( active_symbol_ );
        }
        active_production_->append_symbol( symbol(regex) );
    }
    return *this;
}

Grammar& Grammar::operator()( const GrammarNil& /*nil*/ )
{
    SWEET_ASSERT( active_symbol_ );
    if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = production( active_symbol_ );
        }
    }
    return *this;
}

Grammar& Grammar::error()
{
    if ( active_directive_ )
    {
        active_directive_->append_symbol( error_symbol() );
    }
    else if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = production( active_symbol_ );
        }
        active_production_->append_symbol( error_symbol() );
    }
    return *this;
}

Grammar& Grammar::operator[]( const char* identifier )
{
    SWEET_ASSERT( active_production_ );
    if ( active_production_ )
    {
        active_production_->set_action( action(identifier) );
        active_production_ = nullptr;
    }
    return *this;
}

Grammar& Grammar::operator[]( const GrammarNil& /*nil*/ )
{
    active_production_ = nullptr;
    return *this;
}

void Grammar::end()
{
    int precedence = 1;
    for ( auto i = directives_.begin(); i != directives_.end(); ++i )
    {
        const GrammarDirective* directive = i->get();
        SWEET_ASSERT( directive );
        const vector<GrammarSymbol*>& symbols = directive->symbols();
        for ( auto j = symbols.begin(); j != symbols.end(); ++j )
        {
            GrammarSymbol* symbol = *j;
            SWEET_ASSERT( symbol );
            symbol->set_associativity( directive->associativity() );
            symbol->set_precedence( precedence );
        }
        ++precedence;
    }

    for ( auto i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        GrammarSymbol* symbol = i->get();
        SWEET_ASSERT( symbol );
        if ( !symbol->productions().empty() )
        {
            symbol->set_lexeme_type( LEXEME_NULL );
            symbol->set_symbol_type( SYMBOL_NON_TERMINAL );
        }
    }
}

void Grammar::generate( ParserStateMachine* state_machine, ParserErrorPolicy* parser_error_policy, LexerErrorPolicy* lexer_error_policy )
{
    SWEET_ASSERT( state_machine );

    vector<LexerToken> tokens;
    LalrGrammar parser_grammar;

    const vector<shared_ptr<GrammarProduction>>& productions = productions_;
    for ( auto i = productions.begin(); i != productions.end(); ++i )
    {
        const GrammarProduction* production = i->get();
        SWEET_ASSERT( production );

        const GrammarSymbol* symbol = production->symbol();
        SWEET_ASSERT( symbol );

        parser_grammar.begin_production( parser_grammar.symbol(symbol), 0 );
        const vector<GrammarSymbol*>& symbols = production->symbols();
        for ( auto k = symbols.begin(); k != symbols.end(); ++k )
        {
            parser_grammar.symbol( parser_grammar.symbol(*k) );
        }
        GrammarAction* action = production->action();
        if ( action )
        {
           parser_grammar.action( parser_grammar.action(action->identifier()) );
        }
        GrammarSymbol* precedence_symbol = production->precedence_symbol();
        if ( precedence_symbol )
        {
            parser_grammar.precedence_symbol( parser_grammar.symbol(precedence_symbol) );
        }
        parser_grammar.end_production();
    }

    const vector<shared_ptr<GrammarSymbol>>& symbols = symbols_;
    for ( auto i = symbols.begin(); i != symbols.end(); ++i )
    {
        const GrammarSymbol* symbol = i->get();
        SWEET_ASSERT( symbol );
        if ( symbol->lexeme_type() != LEXEME_NULL )
        {
            LalrSymbol* parser_symbol = parser_grammar.symbol( symbol );
            parser_symbol->set_associativity( symbol->associativity() );
            parser_symbol->set_precedence( symbol->precedence() );
        }
    }

    LalrGenerator generator( parser_grammar, parser_error_policy );
    if ( generator.errors() == 0 )
    {
        const vector<unique_ptr<LalrAction>>& source_actions = generator.actions();
        int actions_size = source_actions.size();
        unique_ptr<ParserAction[]> actions( new ParserAction [actions_size] );
        for ( int i = 0; i < actions_size; ++i )
        {
            const LalrAction* source_action = source_actions[i].get();
            SWEET_ASSERT( source_action );
            ParserAction* action = &actions[i];
            SWEET_ASSERT( action );
            action->reset( source_action->index, source_action->identifier );
        }

        const vector<unique_ptr<LalrSymbol>>& source_symbols = generator.symbols();
        int symbols_size = source_symbols.size();
        unique_ptr<ParserSymbol[]> symbols( new ParserSymbol [symbols_size] );
        for ( int i = 0; i < symbols_size; ++i )
        {
            const LalrSymbol* source_symbol = source_symbols[i].get();
            SWEET_ASSERT( source_symbol );
            ParserSymbol* symbol = &symbols[i];
            SWEET_ASSERT( symbol );
            symbol->reset( source_symbol->get_index(), source_symbol->get_identifier().c_str(), source_symbol->get_lexeme().c_str() );
        }
    
        // Generate tokens for generating the lexical analyzer from each of 
        // the terminal symbols in the grammar.
        for ( int i = 0; i < symbols_size; ++i )
        {
            const LalrSymbol* source_symbol = source_symbols[i].get();
            SWEET_ASSERT( source_symbol );
            if ( source_symbol->get_type() == SYMBOL_TERMINAL )
            {
                const ParserSymbol* symbol = &symbols[i];
                SWEET_ASSERT( symbol );
                LexerTokenType token_type = source_symbol->lexeme_type() == LEXEME_REGULAR_EXPRESSION ? TOKEN_REGULAR_EXPRESSION : TOKEN_LITERAL;
                tokens.push_back( LexerToken(token_type, int(tokens.size()) + 1, symbol, symbol->lexeme) );                
            }
        }

        const set<shared_ptr<LalrState>, shared_ptr_less<LalrState>>& source_states = generator.states();
        int states_size = source_states.size();
        unique_ptr<ParserState[]> states( new ParserState [states_size] );

        int transitions_size = 0;
        for ( auto i = source_states.begin(); i != source_states.end(); ++i )
        {
            const LalrState* source_state = i->get();
            SWEET_ASSERT( source_state );
            transitions_size += source_state->get_transitions().size();
        }
        unique_ptr<ParserTransition[]> transitions( new ParserTransition [transitions_size] );

        const ParserState* start_state = nullptr;
        int state_index = 0;
        int transition_index = 0;
        for ( auto i = source_states.begin(); i != source_states.end(); ++i )
        {
            const LalrState* source_state = i->get();
            SWEET_ASSERT( source_state );
            ParserState* state = &states[state_index];
            SWEET_ASSERT( state );
            const set<LalrTransition>& source_transitions = source_state->get_transitions();
            state->index = state_index;
            state->length = source_transitions.size();
            state->transitions = &transitions[transition_index];
            if ( source_state == generator.start_state() )
            {
                start_state = state;
            }
            for ( auto j = source_transitions.begin(); j != source_transitions.end(); ++j )
            {
                const LalrTransition* source_transition = &(*j);
                SWEET_ASSERT( source_transition );
                const LalrSymbol* source_symbol = source_transition->get_symbol();
                SWEET_ASSERT( source_symbol );
                const LalrState* state_transitioned_to = source_transition->get_state();
                const LalrSymbol* reduced_symbol = source_transition->reduced_symbol();
                ParserTransition* transition = &transitions[transition_index];
                transition->symbol = &symbols[source_symbol->get_index()];
                transition->state = state_transitioned_to ? &states[state_transitioned_to->get_index()] : nullptr;
                transition->reduced_symbol = reduced_symbol ? &symbols[reduced_symbol->get_index()] : nullptr;
                transition->reduced_length = source_transition->reduced_length();
                transition->precedence = source_transition->precedence();
                transition->action = source_transition->action();
                transition->type = source_transition->get_type();
                transition->index = transition_index;
                ++transition_index;
            }
            ++state_index;
        }

        shared_ptr<LexerStateMachine> lexer_state_machine( new LexerStateMachine(identifier_, tokens, whitespace_tokens(), lexer_error_policy) );

        state_machine->set_actions( actions, actions_size );
        state_machine->set_symbols( symbols, symbols_size );
        state_machine->set_transitions( transitions, transitions_size );
        state_machine->set_states( states, states_size, start_state );
        state_machine->set_lexer_state_machine( lexer_state_machine );
    }
}

GrammarDirective* Grammar::directive( Associativity associativity )
{
    shared_ptr<GrammarDirective> directive( new GrammarDirective(associativity) );
    directives_.push_back( directive );
    return directive.get();
}

GrammarSymbol* Grammar::symbol( char literal )
{
    char lexeme [2] = { literal, 0 };
    return Grammar::symbol( lexeme, LEXEME_LITERAL, SYMBOL_NULL );
}

GrammarSymbol* Grammar::symbol( const char* regex )
{
    SWEET_ASSERT( regex );
    return symbol( regex, LEXEME_REGULAR_EXPRESSION, SYMBOL_NULL );
}

GrammarSymbol* Grammar::error_symbol()
{
    return symbol( ".error", LEXEME_LITERAL, SYMBOL_NULL );
}

GrammarSymbol* Grammar::symbol( const char* lexeme, LexemeType lexeme_type, SymbolType symbol_type )
{
    SWEET_ASSERT( lexeme );
    vector<shared_ptr<GrammarSymbol>>::const_iterator i = symbols_.begin();
    while ( i != symbols_.end() && (*i)->lexeme() != lexeme )
    {
        ++i;
    }
    if ( i == symbols_.end() )
    {
        shared_ptr<GrammarSymbol> symbol( new GrammarSymbol(lexeme) );
        symbol->set_lexeme_type( lexeme_type );
        symbol->set_symbol_type( symbol_type );
        symbols_.push_back( symbol );
        return symbol.get();
    }
    GrammarSymbol* symbol = i->get();
    SWEET_ASSERT( symbol );
    SWEET_ASSERT( symbol->lexeme_type() == lexeme_type );
    SWEET_ASSERT( symbol->symbol_type() == symbol_type );
    return symbol;
}

GrammarProduction* Grammar::production( GrammarSymbol* symbol )
{
    SWEET_ASSERT( symbol );
    int index = int(productions_.size());
    shared_ptr<GrammarProduction> production( new GrammarProduction(index, symbol) );
    productions_.push_back( production );
    symbol->append_production( production.get() );
    return production.get();
}

GrammarAction* Grammar::action( const char* identifier )
{
    SWEET_ASSERT( identifier );
    vector<shared_ptr<GrammarAction>>::const_iterator i = actions_.begin();
    while ( i != actions_.end() && (*i)->identifier() != identifier )
    {
        ++i;
    }
    if ( i == actions_.end() )
    {
        shared_ptr<GrammarAction> action( new GrammarAction(identifier) );
        actions_.push_back( action );
        return action.get();
    }
    return i->get();
}
