//
// ParserStateMachine.cpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "ParserStateMachine.hpp"
#include "ParserGenerator.hpp"
#include "GrammarParser.hpp"
#include "ParserProduction.hpp"
#include "ParserSymbol.hpp"
#include "ParserState.hpp"
#include <sweet/lexer/LexerAction.hpp>
#include <sweet/lexer/LexerStateMachine.hpp>

using std::vector;
using std::copy;
using std::back_inserter;
using namespace sweet;
using namespace sweet::parser;

/**
// Constructor.
//
// @param grammar
//  The ParserGrammar to generate this ParserStateMachine from.
//
// @param error_policy
//  The error policy to report errors during generation to or null to 
//  silently swallow errors.
*/
ParserStateMachine::ParserStateMachine( ParserGrammar& grammar, ParserErrorPolicy* error_policy )
{
    ParserGenerator parser_generator( grammar, error_policy );
    if ( parser_generator.errors() == 0 )
    {
        identifier_ = parser_generator.identifier();
        actions_.swap( parser_generator.actions() );
        productions_.swap( parser_generator.productions() );
        symbols_.swap( parser_generator.symbols() );
        states_.reserve( parser_generator.states().size() );
        copy( parser_generator.states().begin(), parser_generator.states().end(), back_inserter(states_) );
        start_symbol_ = parser_generator.start_symbol();
        end_symbol_ = parser_generator.end_symbol();
        error_symbol_ = parser_generator.error_symbol();
        start_state_ = parser_generator.start_state();
        lexer_state_machine_.reset();
    }
}

/**
// Constructor.
//
// @param filename
//  The name of the file to load the grammar to generate a parser for from.
//
// @param error_policy
//  The event sink to report errors to and print with or null to ignore errors
//  and prints.
*/
ParserStateMachine::ParserStateMachine( const char* filename, ParserErrorPolicy* error_policy, lexer::LexerErrorPolicy* lexer_error_policy )
: identifier_(),
  actions_(),
  productions_(),
  symbols_(),
  start_symbol_( NULL ),
  end_symbol_( NULL ),
  error_symbol_( NULL ),
  states_(),
  start_state_( NULL ),
  lexer_state_machine_()
{
    GrammarParser grammar_parser( filename, error_policy );
    if ( grammar_parser.errors() == 0 )
    {
        ParserGenerator parser_generator( grammar_parser.grammar(), error_policy );
        if ( parser_generator.errors() == 0 )
        {
            identifier_ = parser_generator.identifier();
            actions_.swap( parser_generator.actions() );
            productions_.swap( parser_generator.productions() );
            symbols_.swap( parser_generator.symbols() );
            states_.reserve( parser_generator.states().size() );
            copy( parser_generator.states().begin(), parser_generator.states().end(), back_inserter(states_) );
            start_symbol_ = parser_generator.start_symbol();
            end_symbol_ = parser_generator.end_symbol();
            error_symbol_ = parser_generator.error_symbol();
            start_state_ = parser_generator.start_state();
            lexer_state_machine_.reset( new lexer::LexerStateMachine(identifier_, grammar_parser.tokens(), grammar_parser.whitespace_tokens(), lexer_error_policy) );
        }
    }
}

/**
// Constructor.
//
// @param start
//  The first character in the sequence that contains the grammar to generate 
//  a parser from.
//
// @param finish
//  One past the last character in the sequence that contains the grammar to 
//  generate a parser from.
//
// @param error_policy
//  The event sink to report errors to and print with or null to ignore errors
//  and prints.
*/
ParserStateMachine::ParserStateMachine( const char* start, const char* finish, ParserErrorPolicy* error_policy, lexer::LexerErrorPolicy* lexer_error_policy )
: identifier_(),
  actions_(),
  productions_(),
  symbols_(),
  start_symbol_( NULL ),
  end_symbol_( NULL ),
  error_symbol_( NULL ),
  states_(),
  start_state_( NULL ),
  lexer_state_machine_()
{
    GrammarParser grammar_parser( start, finish, error_policy );
    if ( grammar_parser.errors() == 0 )
    {
        ParserGenerator parser_generator( grammar_parser.grammar(), error_policy );
        if ( parser_generator.errors() == 0 )
        {
            identifier_ = parser_generator.identifier();
            actions_.swap( parser_generator.actions() );
            productions_.swap( parser_generator.productions() );
            symbols_.swap( parser_generator.symbols() );
            states_.reserve( parser_generator.states().size() );
            copy( parser_generator.states().begin(), parser_generator.states().end(), back_inserter(states_) );
            start_symbol_ = parser_generator.start_symbol();
            end_symbol_ = parser_generator.end_symbol();
            error_symbol_ = parser_generator.error_symbol();
            start_state_ = parser_generator.start_state();
            lexer_state_machine_.reset( new lexer::LexerStateMachine(identifier_, grammar_parser.tokens(), grammar_parser.whitespace_tokens(), lexer_error_policy) );
        }
    }
}

/**
// Get the identifier of the parser grammar.
//
// @return
//  The identifier.
*/
const std::string& ParserStateMachine::identifier() const
{
    return identifier_;
}

/**
// Get the actions in the parser grammar.
//
// @return
//  The actions.
*/
const std::vector<ptr<ParserAction>>& ParserStateMachine::actions() const
{
    return actions_;
}

/**
// Get the productions in the parser grammar.
//
// @return
//  The productions.
*/
const std::vector<ptr<ParserProduction>>& ParserStateMachine::productions() const
{
    return productions_;
}

/**
// Get the symbols in the parser grammar.
//
// @return
//  The symbols.
*/
const std::vector<ptr<ParserSymbol>>& ParserStateMachine::symbols() const
{
    return symbols_;
}

/**
// Get the states.
//
// @return
//  The states.
*/
const std::vector<ptr<ParserState>>& ParserStateMachine::states() const
{
    return states_;
}

/**
// Get the start symbol.
//
// @return
//  The start symbol or null if parsing the grammar failed.
*/
const ParserSymbol* ParserStateMachine::start_symbol() const
{
    return start_symbol_;
}

/**
// Get the end symbol.
//
// @return
//  The end symbol.
*/
const ParserSymbol* ParserStateMachine::end_symbol() const
{
    return end_symbol_;
}

/**
// Get the error symbol.
//
// @return
//  The error symbol.
*/
const ParserSymbol* ParserStateMachine::error_symbol() const
{
    return error_symbol_;
}

/**
// Get the start state.
//
// @return
//  The start state.
*/
const ParserState* ParserStateMachine::start_state() const
{
    return start_state_;
}

/**
// Get the lexical analyzer used in the parser.
//
// @return
//  The lexical analyzer or null if this ParserStateMachine doesn't have a lexical
//  analyzer.
*/
const lexer::LexerStateMachine* ParserStateMachine::lexer_state_machine() const
{
    return lexer_state_machine_.get();
}

/**
// Find the ParserSymbol whose identifier matches \e identifier.
//
// @param identifier
//  The identifier of the ParserSymbol to find (assumed not null).
//
// @return
//  The ParserSymbol or null if no ParserSymbol was found with a matching 
//  identifier.
*/
const ParserSymbol* ParserStateMachine::find_symbol_by_identifier( const char* identifier ) const
{
    SWEET_ASSERT( identifier );

    std::vector<ptr<ParserSymbol>>::const_iterator i = symbols_.begin();
    while ( i != symbols_.end() && (*i)->get_identifier() != identifier )
    {
        ++i;
    }    
    return i != symbols_.end() ? i->get() : NULL;
}

/**
// Describe this parser data.
//
// @return
//  The description of this parser data.
*/
std::string ParserStateMachine::description() const
{
    std::string description;
    description.reserve( 1024 );
    
    for ( std::vector<ptr<ParserProduction>>::const_iterator i = productions_.begin(); i != productions_.end(); ++i )
    {
        const ParserProduction* production = i->get();
        SWEET_ASSERT( production );
        production->describe( &description );
        description.append( "\n" );
    }
    
    for ( std::vector<ptr<ParserState>>::const_iterator i = states_.begin(); i != states_.end(); ++i )
    {
        const ParserState* state = i->get();
        SWEET_ASSERT( state );
        state->describe( &description );
        description.append( "\n" );
    }

    return description;
}
