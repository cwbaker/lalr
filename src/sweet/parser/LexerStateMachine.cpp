//
// LexerStateMachine.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "LexerStateMachine.hpp"
#include "RegexNodeLess.hpp"
#include "LexerItem.hpp"
#include "LexerTransition.hpp"
#include "LexerState.hpp"
#include "LexerToken.hpp"
#include "LexerGenerator.hpp"
#include "ErrorCode.hpp"
#include "assert.hpp"

using namespace sweet;
using namespace sweet::parser;

/**
// Constructor.
//
// @param regular_expression
//  The regular expression to create a lexical analyzer to recognize.
//
// @param symbol
//  The symbol to return when the regular expression is recognized.
//
// @param event_sink
//  The object to report errors and debug information to or null to ignore 
//  errors and debug information.
*/
LexerStateMachine::LexerStateMachine( const std::string& regular_expression, void* symbol, LexerErrorPolicy* event_sink )
: identifier_(),
  actions_(),
  states_(),
  whitespace_states_(),
  start_state_( NULL ),
  whitespace_start_state_( NULL )
{
    LexerGenerator lexer_generator( LexerToken(TOKEN_REGULAR_EXPRESSION, 0, symbol, regular_expression), event_sink );
    actions_.swap( lexer_generator.actions() );
    states_.reserve( lexer_generator.states().size() );
    copy( lexer_generator.states().begin(), lexer_generator.states().end(), back_inserter(states_) );
    whitespace_states_.reserve( lexer_generator.states().size() );
    copy( lexer_generator.whitespace_states().begin(), lexer_generator.whitespace_states().end(), back_inserter(whitespace_states_) );
    start_state_ = lexer_generator.start_state();
    whitespace_start_state_ = lexer_generator.whitespace_start_state();
}

/**
// Constructor.
//
// @param index
//  The index of this LexerStateMachine.
//
// @param identifier
//  The identifier of this LexerStateMachine.
//
// @param tokens
//  The tokens to generate a lexical analyzer to recognize.
//
// @param whitespace_tokens
//  The tokens to ignore as whitespace.
//
// @param event_sink
//  The object to report errors and debug information to or null to ignore 
//  errors and debug information.
*/
LexerStateMachine::LexerStateMachine( const std::string& identifier, const std::vector<LexerToken>& tokens, const std::vector<LexerToken>& whitespace_tokens, LexerErrorPolicy* event_sink )
: identifier_( identifier ),
  actions_(),
  states_(),
  whitespace_states_(),
  start_state_( NULL ),
  whitespace_start_state_( NULL )
{
    LexerGenerator lexer_generator( tokens, whitespace_tokens, event_sink );
    actions_.swap( lexer_generator.actions() );
    states_.reserve( lexer_generator.states().size() );
    copy( lexer_generator.states().begin(), lexer_generator.states().end(), back_inserter(states_) );
    whitespace_states_.reserve( lexer_generator.states().size() );
    copy( lexer_generator.whitespace_states().begin(), lexer_generator.whitespace_states().end(), back_inserter(whitespace_states_) );
    start_state_ = lexer_generator.start_state();
    whitespace_start_state_ = lexer_generator.whitespace_start_state();
}

/**
// Get the identifier of this LexerStateMachine.
//
// @return
//  The identifier.
*/
const std::string& LexerStateMachine::identifier() const
{
    return identifier_;
}

/**
// Get the actions in this LexerStateMachine.
//
// @return
//  The actions.
*/
const std::vector<std::shared_ptr<LexerAction> >& LexerStateMachine::actions() const
{
    return actions_;
}

/**
// Get the states in this LexerStateMachine.
//
// @return
//  The states.
*/
const std::vector<std::shared_ptr<LexerState> >& LexerStateMachine::states() const
{
    return states_;
}

/**
// Get the whitespace states in this LexerStateMachine.
//
// @return
//  The whitespace states.
*/
const std::vector<std::shared_ptr<LexerState> >& LexerStateMachine::whitespace_states() const
{
    return whitespace_states_;
}

/**
// Get the starting state for the lexical analyzer.
//
// @return
//  The starting state.
*/
const LexerState* LexerStateMachine::start_state() const
{
    return start_state_;
}

/**
// Get the whitespace start state in this LexerStateMachine.
//
// @return
//  The whitespace start state in this LexerStateMachine.
*/
const LexerState* LexerStateMachine::whitespace_start_state() const
{
    return whitespace_start_state_;
}

/**
// Describe the lexical analyzer.
//
// @param description
//  A variable to receive the description (assumed not null).
*/
void LexerStateMachine::describe( std::string* description ) const
{
    SWEET_ASSERT( description );
    std::vector<std::shared_ptr<LexerState> >::const_iterator i = states_.begin(); 
    while ( i != states_.end() )
    {
        const LexerState* state = i->get();
        SWEET_ASSERT( state );
        state->describe( description );
        description->append( "\n" );
        ++i;
    }
}

/**
// Get a description of this lexical analyzer.
//
// @return
//  The description.
*/
std::string LexerStateMachine::description() const
{
    std::string description;
    description.reserve( 1024 );
    describe( &description );
    return description;
}
