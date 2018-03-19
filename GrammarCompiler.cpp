//
// GrammarCompiler.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "GrammarCompiler.hpp"
#include "Grammar.hpp"
#include "GrammarGenerator.hpp"
#include "GrammarSymbol.hpp"
#include "GrammarAction.hpp"
#include "GrammarState.hpp"
#include "GrammarTransition.hpp"
#include "LexerGenerator.hpp"
#include "LexerToken.hpp"
#include "ParserStateMachine.hpp"
#include "ParserSymbol.hpp"
#include "ParserState.hpp"
#include "ParserAction.hpp"
#include "ParserTransition.hpp"
#include "LexerAllocations.hpp"
#include "ErrorPolicy.hpp"
#include "assert.hpp"

using std::set;
using std::vector;
using std::copy;
using std::back_inserter;
using std::unique_ptr;
using std::shared_ptr;
using namespace sweet;
using namespace sweet::lalr;

GrammarCompiler::GrammarCompiler()
: strings_(),
  actions_(),
  symbols_(),
  transitions_(),
  states_(),
  lexer_allocations_(),
  whitespace_lexer_allocations_(),
  parser_state_machine_()
{
    lexer_allocations_.reset( new LexerAllocations );
    whitespace_lexer_allocations_.reset( new LexerAllocations );
    parser_state_machine_.reset( new ParserStateMachine );
    memset( parser_state_machine_.get(), 0, sizeof(*parser_state_machine_) );
}

GrammarCompiler::~GrammarCompiler()
{
}

const LexerAllocations* GrammarCompiler::lexer_allocations() const
{
    return lexer_allocations_.get();
}

const LexerAllocations* GrammarCompiler::whitespace_lexer_allocations() const
{
    return whitespace_lexer_allocations_.get();
}

const ParserStateMachine* GrammarCompiler::parser_state_machine() const
{
    return parser_state_machine_.get();
}

void GrammarCompiler::compile( const char* begin, const char* end, ErrorPolicy* error_policy )
{
    Grammar grammar;
    grammar.parse( begin, end );

    const vector<unique_ptr<GrammarSymbol>>& symbols = grammar.symbols();    
    for ( auto i = symbols.begin(); i != symbols.end(); ++i )
    {
        GrammarSymbol* symbol = i->get();
        SWEET_ASSERT( symbol );
        if ( !symbol->productions().empty() )
        {
            symbol->set_lexeme_type( LEXEME_NULL );
            symbol->set_symbol_type( SYMBOL_NON_TERMINAL );
        }
    }

    GrammarGenerator generator;
    int errors = generator.generate(grammar, this, error_policy );
    if ( errors == 0 )
    {
        populate_parser_state_machine( grammar, generator );
        populate_lexer_state_machine( generator, error_policy );
        populate_whitespace_lexer_state_machine( grammar, error_policy );
    }
}

const char* GrammarCompiler::add_string( const std::string& string )
{
    strings_.push_back( string );
    return strings_.back().c_str();
}

void GrammarCompiler::set_actions( std::unique_ptr<ParserAction[]>& actions, int actions_size )
{
    SWEET_ASSERT( parser_state_machine_ );
    SWEET_ASSERT( actions || actions_size == 0 );
    SWEET_ASSERT( actions_size >= 0 );
    actions_ = move( actions );
    parser_state_machine_->actions_size = actions_size;
    parser_state_machine_->actions = actions_.get();
}

void GrammarCompiler::set_symbols( std::unique_ptr<ParserSymbol[]>& symbols, int symbols_size )
{
    SWEET_ASSERT( parser_state_machine_ );
    SWEET_ASSERT( symbols );
    SWEET_ASSERT( symbols_size >= 3 );
    symbols_ = move( symbols );
    parser_state_machine_->symbols_size = symbols_size;
    parser_state_machine_->symbols = symbols_.get();
    parser_state_machine_->start_symbol = &symbols_[0];
    parser_state_machine_->end_symbol = &symbols_[1];
    parser_state_machine_->error_symbol = &symbols_[2];
} 

void GrammarCompiler::set_transitions( std::unique_ptr<ParserTransition[]>& transitions, int transitions_size )
{
    SWEET_ASSERT( transitions );
    SWEET_ASSERT( transitions_size >= 0 );
    transitions_ = move( transitions );
    parser_state_machine_->transitions_size = transitions_size;
    parser_state_machine_->transitions = transitions_.get();
}

void GrammarCompiler::set_states( std::unique_ptr<ParserState[]>& states, int states_size, const ParserState* start_state )
{
    SWEET_ASSERT( states );
    SWEET_ASSERT( states_size >= 0 );
    SWEET_ASSERT( start_state );
    states_ = move( states );
    parser_state_machine_->states_size = states_size;
    parser_state_machine_->states = states_.get();
    parser_state_machine_->start_state = start_state;
}

void GrammarCompiler::set_lexer_allocations( std::unique_ptr<LexerAllocations>& lexer_allocations )
{
    SWEET_ASSERT( lexer_allocations.get() );
    if ( lexer_allocations )
    {
        lexer_allocations_ = move( lexer_allocations );
        parser_state_machine_->lexer_state_machine = lexer_allocations_->state_machine();
    }
}

void GrammarCompiler::set_whitespace_lexer_allocations( std::unique_ptr<LexerAllocations>& whitespace_lexer_allocations )
{
    if ( whitespace_lexer_allocations )
    {
        whitespace_lexer_allocations_ = move( whitespace_lexer_allocations );
        parser_state_machine_->whitespace_lexer_state_machine = whitespace_lexer_allocations_->state_machine();
    }
}

void GrammarCompiler::populate_parser_state_machine( const Grammar& grammar, const GrammarGenerator& generator )
{
    const vector<unique_ptr<GrammarAction>>& grammar_actions = generator.actions();
    int actions_size = grammar_actions.size();
    unique_ptr<ParserAction[]> actions( new ParserAction [actions_size] );
    for ( int i = 0; i < actions_size; ++i )
    {
        const GrammarAction* grammar_action = grammar_actions[i].get();
        SWEET_ASSERT( grammar_action );
        ParserAction* action = &actions[i];
        SWEET_ASSERT( action );
        action->index = grammar_action->index();
        action->identifier = add_string( grammar_action->identifier() );
    }

    const vector<unique_ptr<GrammarSymbol>>& grammar_symbols = generator.symbols();
    int symbols_size = grammar_symbols.size();
    unique_ptr<ParserSymbol[]> symbols( new ParserSymbol [symbols_size] );
    for ( int i = 0; i < symbols_size; ++i )
    {
        const GrammarSymbol* source_symbol = grammar_symbols[i].get();
        SWEET_ASSERT( source_symbol );
        ParserSymbol* symbol = &symbols[i];
        SWEET_ASSERT( symbol );
        symbol->index = source_symbol->index();
        symbol->identifier = add_string( source_symbol->identifier() );
        symbol->lexeme = add_string( source_symbol->lexeme() );
        symbol->type = source_symbol->symbol_type();
    }

    const set<shared_ptr<GrammarState>, shared_ptr_less<GrammarState>>& grammar_states = generator.states();
    int states_size = grammar_states.size();
    unique_ptr<ParserState[]> states( new ParserState [states_size] );

    int transitions_size = 0;
    for ( auto i = grammar_states.begin(); i != grammar_states.end(); ++i )
    {
        const GrammarState* grammar_state = i->get();
        SWEET_ASSERT( grammar_state );
        transitions_size += grammar_state->transitions().size();
    }
    unique_ptr<ParserTransition[]> transitions( new ParserTransition [transitions_size] );

    const ParserState* start_state = nullptr;
    int state_index = 0;
    int transition_index = 0;
    for ( auto i = grammar_states.begin(); i != grammar_states.end(); ++i )
    {
        const GrammarState* grammar_state = i->get();
        SWEET_ASSERT( grammar_state );
        ParserState* state = &states[state_index];
        SWEET_ASSERT( state );
        const set<GrammarTransition>& source_transitions = grammar_state->transitions();
        state->index = state_index;
        state->length = source_transitions.size();
        state->transitions = &transitions[transition_index];
        if ( grammar_state == generator.start_state() )
        {
            start_state = state;
        }
        for ( auto j = source_transitions.begin(); j != source_transitions.end(); ++j )
        {
            const GrammarTransition* source_transition = &(*j);
            SWEET_ASSERT( source_transition );
            const GrammarSymbol* source_symbol = source_transition->symbol();
            SWEET_ASSERT( source_symbol );
            const GrammarState* state_transitioned_to = source_transition->state();
            const GrammarSymbol* reduced_symbol = source_transition->reduced_symbol();
            ParserTransition* transition = &transitions[transition_index];
            transition->symbol = &symbols[source_symbol->index()];
            transition->state = state_transitioned_to ? &states[state_transitioned_to->index()] : nullptr;
            transition->reduced_symbol = reduced_symbol ? &symbols[reduced_symbol->index()] : nullptr;
            transition->reduced_length = source_transition->reduced_length();
            transition->precedence = source_transition->precedence();
            transition->action = source_transition->action();
            transition->type = source_transition->type();
            transition->index = transition_index;
            ++transition_index;
        }
        ++state_index;
    }

    parser_state_machine_->identifier = add_string( grammar.identifier() );
    set_actions( actions, actions_size );
    set_symbols( symbols, symbols_size );
    set_transitions( transitions, transitions_size );
    set_states( states, states_size, start_state );
}

void GrammarCompiler::populate_lexer_state_machine( const GrammarGenerator& generator, ErrorPolicy* error_policy )
{
    // Generate tokens for generating the lexical analyzer from each of 
    // the terminal symbols in the grammar.
    const vector<unique_ptr<GrammarSymbol>>& grammar_symbols = generator.symbols();
    vector<LexerToken> tokens;
    for ( size_t i = 0; i < grammar_symbols.size(); ++i )
    {
        const GrammarSymbol* grammar_symbol = grammar_symbols[i].get();
        SWEET_ASSERT( grammar_symbol );
        if ( grammar_symbol->symbol_type() == SYMBOL_TERMINAL )
        {
            const ParserSymbol* symbol = &symbols_[i];
            SWEET_ASSERT( symbol );
            int line = grammar_symbol->line();
            LexerTokenType token_type = grammar_symbol->lexeme_type() == LEXEME_REGULAR_EXPRESSION ? TOKEN_REGULAR_EXPRESSION : TOKEN_LITERAL;
            tokens.push_back( LexerToken(token_type, line, symbol, symbol->lexeme) );                
        }
    }

    unique_ptr<LexerAllocations> lexer_allocations( new LexerAllocations );
    LexerGenerator lexer_generator;
    lexer_generator.generate( tokens, lexer_allocations.get(), error_policy );
    lexer_allocations_ = move( lexer_allocations );
    parser_state_machine_->lexer_state_machine = lexer_allocations_->state_machine();
}

void GrammarCompiler::populate_whitespace_lexer_state_machine( const Grammar& grammar, ErrorPolicy* error_policy )
{
    unique_ptr<LexerAllocations> whitespace_lexer_allocations;
    const vector<LexerToken>& whitespace_tokens = grammar.whitespace_tokens();
    if ( !whitespace_tokens.empty() )
    {
        whitespace_lexer_allocations.reset( new LexerAllocations );
        LexerGenerator lexer_generator;
        lexer_generator.generate( whitespace_tokens, whitespace_lexer_allocations.get(), error_policy );        
        whitespace_lexer_allocations_ = move( whitespace_lexer_allocations );
        parser_state_machine_->whitespace_lexer_state_machine = whitespace_lexer_allocations_->state_machine();
    }
}
