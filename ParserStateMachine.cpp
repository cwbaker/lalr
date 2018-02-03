//
// ParserStateMachine.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "ParserStateMachine.hpp"
#include "ParserGenerator.hpp"
#include "ParserGrammar.hpp"
#include "ParserProduction.hpp"
#include "ParserSymbol.hpp"
#include "ParserState.hpp"
#include "ParserAction.hpp"
#include "GrammarSymbol.hpp"
#include "GrammarProduction.hpp"
#include "GrammarDirective.hpp"
#include "GrammarAction.hpp"
#include "Grammar.hpp"
#include "LexerAction.hpp"
#include "LexerStateMachine.hpp"
#include "assert.hpp"

using std::vector;
using std::shared_ptr;
using std::copy;
using std::back_inserter;
using namespace sweet;
using namespace sweet::lalr;
using namespace sweet::lalr;

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
ParserStateMachine::ParserStateMachine( Grammar& grammar, ParserErrorPolicy* error_policy, LexerErrorPolicy* lexer_error_policy )
: identifier_(),
  actions_(),
  productions_(),
  symbols_(),
  states_(),
  start_symbol_( nullptr ),
  end_symbol_( nullptr ),
  error_symbol_( nullptr ),
  start_state_( nullptr ),
  lexer_state_machine_()
{
    vector<LexerToken> tokens;
    ParserGrammar parser_grammar;

    const vector<shared_ptr<GrammarProduction>>& productions = grammar.productions();
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

    const vector<shared_ptr<GrammarSymbol>>& symbols = grammar.symbols();
    for ( auto i = symbols.begin(); i != symbols.end(); ++i )
    {
        const GrammarSymbol* symbol = i->get();
        SWEET_ASSERT( symbol );
        if ( symbol->type() == GRAMMAR_LITERAL || symbol->type() == GRAMMAR_REGULAR_EXPRESSION )
        {
            ParserSymbol* parser_symbol = parser_grammar.symbol( symbol );
            parser_symbol->set_associativity( symbol->associativity() );
            parser_symbol->set_precedence( symbol->precedence() );
            LexerTokenType token_type = symbol->type() == GRAMMAR_REGULAR_EXPRESSION ? TOKEN_REGULAR_EXPRESSION : TOKEN_LITERAL;
            tokens.push_back( LexerToken(token_type, int(tokens.size()) + 1, parser_symbol, symbol->lexeme()) );                
        }
    }

    ParserGenerator parser_generator( parser_grammar, error_policy ); 
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
        lexer_state_machine_.reset( new LexerStateMachine(identifier_, tokens, grammar.whitespace_tokens(), lexer_error_policy) );
    }
}

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
: identifier_(),
  actions_(),
  productions_(),
  symbols_(),
  states_(),
  start_symbol_( nullptr ),
  end_symbol_( nullptr ),
  error_symbol_( nullptr ),
  start_state_( nullptr ),
  lexer_state_machine_()
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

ParserStateMachine::~ParserStateMachine()
{
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
const std::vector<std::unique_ptr<ParserAction> >& ParserStateMachine::actions() const
{
    return actions_;
}

/**
// Get the productions in the parser grammar.
//
// @return
//  The productions.
*/
const std::vector<std::shared_ptr<ParserProduction> >& ParserStateMachine::productions() const
{
    return productions_;
}

/**
// Get the symbols in the parser grammar.
//
// @return
//  The symbols.
*/
const std::vector<std::shared_ptr<ParserSymbol> >& ParserStateMachine::symbols() const
{
    return symbols_;
}

/**
// Get the states.
//
// @return
//  The states.
*/
const std::vector<std::shared_ptr<ParserState> >& ParserStateMachine::states() const
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
const LexerStateMachine* ParserStateMachine::lexer_state_machine() const
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

    std::vector<std::shared_ptr<ParserSymbol> >::const_iterator i = symbols_.begin();
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
    
    for ( std::vector<std::shared_ptr<ParserProduction> >::const_iterator i = productions_.begin(); i != productions_.end(); ++i )
    {
        const ParserProduction* production = i->get();
        SWEET_ASSERT( production );
        production->describe( &description );
        description.append( "\n" );
    }
    
    for ( std::vector<std::shared_ptr<ParserState> >::const_iterator i = states_.begin(); i != states_.end(); ++i )
    {
        const ParserState* state = i->get();
        SWEET_ASSERT( state );
        state->describe( &description );
        description.append( "\n" );
    }

    return description;
}
