//
// ParserStateMachine.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "ParserStateMachine.hpp"
#include "ParserSymbol.hpp"
#include "ParserState.hpp"
#include "ParserAction.hpp"
#include "ParserTransition.hpp"
#include "LexerAction.hpp"
#include "LexerStateMachine.hpp"
#include "assert.hpp"

using std::vector;
using std::copy;
using std::back_inserter;
using std::unique_ptr;
using std::shared_ptr;
using namespace sweet;
using namespace sweet::lalr;
using namespace sweet::lalr;

ParserStateMachine::ParserStateMachine()
: allocated_actions_(),
  allocated_symbols_(),
  allocated_states_(),
  actions_size_( 0 ),
  symbols_size_( 0 ),
  transitions_size_( 0 ),
  states_size_( 0 ),
  actions_( nullptr ),
  symbols_( nullptr ),
  states_( nullptr ),
  start_symbol_( nullptr ),
  end_symbol_( nullptr ),
  error_symbol_( nullptr ),
  start_state_( nullptr ),
  lexer_state_machine_()
{
}

ParserStateMachine::~ParserStateMachine()
{
    // for ( auto i = actions_.begin(); i != actions_.end(); ++i )
    // {
    //     ParserAction* action = i->get();
    //     SWEET_ASSERT( action );
    //     action->destroy();
    // }
}

int ParserStateMachine::actions_size() const
{
    return actions_size_;   
}

int ParserStateMachine::symbols_size() const
{
    return symbols_size_;   
}

int ParserStateMachine::transitions_size() const
{
    return transitions_size_;
}

int ParserStateMachine::states_size() const
{
    return states_size_;   
}

/**
// Get the actions in the parser grammar.
//
// @return
//  The actions.
*/
const ParserAction* ParserStateMachine::actions() const
{
    return actions_;
}

/**
// Get the symbols in the parser grammar.
//
// @return
//  The symbols.
*/
const ParserSymbol* ParserStateMachine::symbols() const
{
    return symbols_;
}

/**
// Get the states.
//
// @return
//  The states.
*/
const ParserState* ParserStateMachine::states() const
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
// Find the symbol whose identifier matches \e identifier.
//
// @param identifier
//  The identifier of the ParserSymbol to find (assumed not null).
//
// @return
//  The `ParserSymbol` with matching identifier or null if there was no 
//  matching `ParserSymbol`.
*/
const ParserSymbol* ParserStateMachine::find_symbol_by_identifier( const char* identifier ) const
{
    SWEET_ASSERT( identifier );
    const ParserSymbol* symbol = symbols_;
    const ParserSymbol* symbols_end = symbols_ + symbols_size_;
    while ( symbol != symbols_end && strcmp(symbol->identifier, identifier) != 0 )
    {
        ++symbol;
    }    
    return symbol != symbols_end ? symbol : nullptr;
}

void ParserStateMachine::set_actions( std::unique_ptr<ParserAction[]>& actions, int actions_size )
{
    SWEET_ASSERT( actions );
    SWEET_ASSERT( actions_size >= 0 );
    allocated_actions_ = move( actions );
    actions_size_ = actions_size;
    actions_ = allocated_actions_.get();
}

void ParserStateMachine::set_symbols( std::unique_ptr<ParserSymbol[]>& symbols, int symbols_size )
{
    SWEET_ASSERT( symbols );
    SWEET_ASSERT( symbols_size >= 3 );
    allocated_symbols_ = move( symbols );
    symbols_size_ = symbols_size;
    symbols_ = allocated_symbols_.get();
    start_symbol_ = &symbols_[0];
    end_symbol_ = &symbols_[1];
    error_symbol_ = &symbols_[2];
} 

void ParserStateMachine::set_transitions( std::unique_ptr<ParserTransition[]>& transitions, int transitions_size )
{
    SWEET_ASSERT( transitions );
    SWEET_ASSERT( transitions_size >= 0 );
    allocated_transitions_ = move( transitions );
    transitions_size_ = transitions_size;
    transitions_ = allocated_transitions_.get();
}

void ParserStateMachine::set_states( std::unique_ptr<ParserState[]>& states, int states_size, const ParserState* start_state )
{
    SWEET_ASSERT( states );
    SWEET_ASSERT( states_size >= 0 );
    SWEET_ASSERT( start_state );
    allocated_states_ = move( states );
    states_size_ = states_size;
    states_ = allocated_states_.get();
    start_state_ = start_state;
}

void ParserStateMachine::set_lexer_state_machine( const std::shared_ptr<LexerStateMachine>& lexer_state_machine )
{
    SWEET_ASSERT( lexer_state_machine.get() );
    lexer_state_machine_ = lexer_state_machine;
}
