//
// GrammarCompiler.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "GrammarCompiler.hpp"
#include "Grammar.hpp"
#include "GrammarParser.hpp"
#include "GrammarGenerator.hpp"
#include "GrammarSymbol.hpp"
#include "GrammarAction.hpp"
#include "GrammarState.hpp"
#include "GrammarTransition.hpp"
#include "RegexGenerator.hpp"
#include "RegexToken.hpp"
#include "ParserStateMachine.hpp"
#include "ParserSymbol.hpp"
#include "ParserState.hpp"
#include "ParserAction.hpp"
#include "ParserTransition.hpp"
#include "RegexCompiler.hpp"
#include "ErrorPolicy.hpp"
#include "assert.hpp"
#include <iterator>
#include <string.h>

using std::set;
using std::vector;
using std::copy;
using std::back_inserter;
using std::unique_ptr;
using std::shared_ptr;
using namespace lalr;

GrammarCompiler::GrammarCompiler()
: strings_()
, actions_()
, symbols_()
, transitions_()
, states_()
, lexer_()
, whitespace_lexer_()
, parser_state_machine_()
{
    lexer_.reset( new RegexCompiler );
    whitespace_lexer_.reset( new RegexCompiler );
    parser_state_machine_.reset( new ParserStateMachine );
    memset( parser_state_machine_.get(), 0, sizeof(*parser_state_machine_) );
}

GrammarCompiler::~GrammarCompiler()
{
}

const RegexCompiler* GrammarCompiler::lexer() const
{
    return lexer_.get();
}

const RegexCompiler* GrammarCompiler::whitespace_lexer() const
{
    return whitespace_lexer_.get();
}

const ParserStateMachine* GrammarCompiler::parser_state_machine() const
{
    return parser_state_machine_.get();
}

int GrammarCompiler::compile( const char* begin, const char* end, ErrorPolicy* error_policy )
{
    Grammar grammar;

    GrammarParser parser;
    int errors = parser.parse( begin, end, error_policy, &grammar );
    if ( errors == 0 )
    {
        GrammarGenerator generator;
        errors = generator.generate( grammar, error_policy );

        if ( errors == 0 )
        {
            populate_parser_state_machine( grammar, generator );

            // Generate tokens for generating the lexical analyzer from each of 
            // the terminal symbols in the grammar.
            vector<RegexToken> tokens;
            int column = 1;
            const vector<unique_ptr<GrammarSymbol>>& grammar_symbols = generator.symbols();
            for ( size_t i = 0; i < grammar_symbols.size(); ++i, ++column )
            {
                const GrammarSymbol* grammar_symbol = grammar_symbols[i].get();
                LALR_ASSERT( grammar_symbol );
                if ( grammar_symbol->symbol_type() == SYMBOL_TERMINAL )
                {
                    const ParserSymbol* symbol = &symbols_[i];
                    LALR_ASSERT( symbol );
                    int line = grammar_symbol->line();
                    RegexTokenType token_type = grammar_symbol->lexeme_type() == LEXEME_REGULAR_EXPRESSION ? TOKEN_REGULAR_EXPRESSION : TOKEN_LITERAL;
                    tokens.push_back( RegexToken(token_type, line, column, symbol, symbol->lexeme) );
                }
            }

            errors += lexer_->compile( tokens, error_policy );
            if ( errors == 0 )
            {
                parser_state_machine_->lexer_state_machine = lexer_->state_machine();
            }

            const vector<RegexToken>& whitespace_tokens = grammar.whitespace_tokens();
            if ( !whitespace_tokens.empty() )
            {
                errors += whitespace_lexer_->compile( whitespace_tokens, error_policy );
                if ( errors == 0 )
                {
                    parser_state_machine_->whitespace_lexer_state_machine = whitespace_lexer_->state_machine();
                }
            }
        }
    }
    return errors;
}

const char* GrammarCompiler::add_string( const std::string& string )
{
    strings_.push_back( string );
    return strings_.back().c_str();
}

void GrammarCompiler::set_actions( std::unique_ptr<ParserAction[]>& actions, int actions_size )
{
    LALR_ASSERT( parser_state_machine_ );
    LALR_ASSERT( actions || actions_size == 0 );
    LALR_ASSERT( actions_size >= 0 );
    actions_ = move( actions );
    parser_state_machine_->actions_size = actions_size;
    parser_state_machine_->actions = actions_.get();
}

void GrammarCompiler::set_symbols( std::unique_ptr<ParserSymbol[]>& symbols, int symbols_size )
{
    LALR_ASSERT( parser_state_machine_ );
    LALR_ASSERT( symbols );
    LALR_ASSERT( symbols_size >= 3 );
    symbols_ = move( symbols );
    parser_state_machine_->symbols_size = symbols_size;
    parser_state_machine_->symbols = symbols_.get();
    parser_state_machine_->start_symbol = &symbols_[0];
    parser_state_machine_->end_symbol = &symbols_[1];
    parser_state_machine_->error_symbol = &symbols_[2];
    parser_state_machine_->whitespace_symbol = &symbols_[3];
} 

void GrammarCompiler::set_transitions( std::unique_ptr<ParserTransition[]>& transitions, int transitions_size )
{
    LALR_ASSERT( transitions );
    LALR_ASSERT( transitions_size >= 0 );
    transitions_ = move( transitions );
    parser_state_machine_->transitions_size = transitions_size;
    parser_state_machine_->transitions = transitions_.get();
}

void GrammarCompiler::set_states( std::unique_ptr<ParserState[]>& states, int states_size, const ParserState* start_state )
{
    LALR_ASSERT( states );
    LALR_ASSERT( states_size >= 0 );
    LALR_ASSERT( start_state );
    states_ = move( states );
    parser_state_machine_->states_size = states_size;
    parser_state_machine_->states = states_.get();
    parser_state_machine_->start_state = start_state;
}

void GrammarCompiler::populate_parser_state_machine( const Grammar& grammar, const GrammarGenerator& generator )
{
    const vector<unique_ptr<GrammarAction>>& grammar_actions = generator.actions();
    int actions_size = int(grammar_actions.size());
    unique_ptr<ParserAction[]> actions( new ParserAction [actions_size] );
    for ( int i = 0; i < actions_size; ++i )
    {
        const GrammarAction* grammar_action = grammar_actions[i].get();
        LALR_ASSERT( grammar_action );
        ParserAction* action = &actions[i];
        LALR_ASSERT( action );
        action->index = grammar_action->index();
        action->identifier = add_string( grammar_action->identifier() );
    }

    const vector<unique_ptr<GrammarSymbol>>& grammar_symbols = generator.symbols();
    int symbols_size = int(grammar_symbols.size());
    unique_ptr<ParserSymbol[]> symbols( new ParserSymbol [symbols_size] );
    for ( int i = 0; i < symbols_size; ++i )
    {
        const GrammarSymbol* source_symbol = grammar_symbols[i].get();
        LALR_ASSERT( source_symbol );
        ParserSymbol* symbol = &symbols[i];
        LALR_ASSERT( symbol );
        symbol->index = source_symbol->index();
        symbol->identifier = add_string( source_symbol->identifier() );
        symbol->lexeme = add_string( source_symbol->lexeme() );
        symbol->type = source_symbol->symbol_type();
    }

    const set<shared_ptr<GrammarState>, GrammarStateLess>& grammar_states = generator.states();
    int states_size = int(grammar_states.size());
    unique_ptr<ParserState[]> states( new ParserState [states_size] );

    int transitions_size = 0;
    for ( auto i = grammar_states.begin(); i != grammar_states.end(); ++i )
    {
        const GrammarState* grammar_state = i->get();
        LALR_ASSERT( grammar_state );
        transitions_size += int(grammar_state->transitions().size());
    }
    unique_ptr<ParserTransition[]> transitions( new ParserTransition [transitions_size] );

    const ParserState* start_state = nullptr;
    int state_index = 0;
    int transition_index = 0;
    for ( auto i = grammar_states.begin(); i != grammar_states.end(); ++i )
    {
        const GrammarState* grammar_state = i->get();
        LALR_ASSERT( grammar_state );
        ParserState* state = &states[state_index];
        LALR_ASSERT( state );
        const set<GrammarTransition>& source_transitions = grammar_state->transitions();
        state->index = state_index;
        state->length = int(source_transitions.size());
        state->transitions = &transitions[transition_index];
        if ( grammar_state == generator.start_state() )
        {
            start_state = state;
        }
        for ( auto j = source_transitions.begin(); j != source_transitions.end(); ++j )
        {
            const GrammarTransition* source_transition = &(*j);
            LALR_ASSERT( source_transition );
            const GrammarSymbol* source_symbol = source_transition->symbol();
            LALR_ASSERT( source_symbol );
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
