//
// Transition.cpp
// Copyright (c) Charles Baker. All rights reserved.
//    

#include "Transition.hpp"
#include "Symbol.hpp"
#include "GrammarAction.hpp"
#include "assert.hpp"
#include <stdio.h>

using namespace sweet::lalr;

#if defined(BUILD_PLATFORM_MSVC)
#define snprintf _snprintf
#endif

/**
// Constructor.
//
// @param symbol
//  The symbol that this transition is made on (assumed not null).
//
// @param reduced_symbol
//  The Symbol that this transition reduces to.
//
// @param length
//  The number of symbols on the right-hand side of the production that this 
//  transition reduces.
//
// @param action
//  The index of the action taken on this reduction or 
//  `GrammarAction::INVALID_INDEX` if no action is taken.
*/
Transition::Transition( const Symbol* symbol, const Symbol* reduced_symbol, int reduced_length, int precedence, int action )
: symbol_( symbol ),
  state_( NULL ),
  reduced_symbol_( reduced_symbol ),
  reduced_length_( reduced_length ),
  precedence_( precedence ),
  action_( action ),
  type_( TRANSITION_REDUCE ),
  index_( INVALID_INDEX )
{
    SWEET_ASSERT( reduced_symbol_ );
    SWEET_ASSERT( reduced_length_ >= 0 );
    SWEET_ASSERT( precedence_ >= 0 );
}

/**
// Constructor.
//
// @param symbol
//  The symbol that this transition is made on (assumed not null).
//
// @param state
//  The state that this transition is to (assumed not null).
*/
Transition::Transition( const Symbol* symbol, GrammarState* state )
: symbol_( symbol ),
  state_( state ),
  reduced_symbol_( nullptr ),
  reduced_length_( 0 ),
  precedence_( 0 ),
  action_( GrammarAction::INVALID_INDEX ),
  type_( TRANSITION_SHIFT ),
  index_( INVALID_INDEX )
{
    SWEET_ASSERT( symbol_ );
    SWEET_ASSERT( state_ );
}

/**
// Get the state that this transition is to.
//
// @return
//  The state or null if this transition is a reduce transition.
*/
GrammarState* Transition::state() const
{
    return state_;
}

const Symbol* Transition::reduced_symbol() const
{
    return reduced_symbol_;
}

int Transition::reduced_length() const
{
    return reduced_length_;
}

int Transition::precedence() const
{
    return precedence_;
}

int Transition::action() const
{
    return action_;
}

/**
// Get the type of this transition.
//
// @return
//  The type of this transition.
*/
TransitionType Transition::type() const
{
    return type_;
}

/**
// Is this transition made on \e symbol?
//
// @param symbol
//  The symbol to check this transition being made on.
//
// @return
//  True if this transition is taken on \e symbol otherwise false.
*/
bool Transition::taken_on_symbol( const Symbol* symbol ) const
{
    return symbol_ == symbol;
}

/**
// Get the symbol that this transition is on.
//
// @return
//  The symbol.
*/
const Symbol* Transition::symbol() const
{
    SWEET_ASSERT( symbol_ );
    return symbol_;
}

/**
// Get the index of this transition.
//
// @return
//  The index.
*/
int Transition::index() const
{
    return index_;
}

/**
// Less than operator.
//
// @param transition
//  The transition to compare this transition with.
//
// @return
//  True if the address of this transition's symbol is less than the address 
//  of \e transition's symbol.
*/
bool Transition::operator<( const Transition& transition ) const
{
    return symbol_ < transition.symbol_;
}

/**
// Set the index of this transition.
//
// @param index
//  The value to set the index of this production to.
*/
void Transition::set_index( int index ) const
{
    index_ = index;
}

/**
// Change this transition from being a shift transition into being a reduce
// transition.
//
// @param reduced_production
//  The production to reduce by when this transition is taken.
*/
void Transition::override_shift_to_reduce( const Symbol* symbol, int length, int precedence, int action ) const
{
    SWEET_ASSERT( type_ == TRANSITION_SHIFT );
    SWEET_ASSERT( state_ );
    SWEET_ASSERT( !reduced_symbol_ );
    SWEET_ASSERT( length > 0 );
    SWEET_ASSERT( precedence >= 0 );
    SWEET_ASSERT( symbol );    
    type_ = TRANSITION_REDUCE;
    state_ = nullptr;
    reduced_symbol_ = symbol;
    reduced_length_ = length;
    precedence_ = precedence;
    action_ = action;
}

/**
// Change this transition from being a reduce transition for one production 
// into a reduce transition for a different production.
//
// @param reduced_production
//  The production to reduce by when this transition is taken.
*/
void Transition::override_reduce_to_reduce( const Symbol* symbol, int length, int precedence, int action ) const
{
    SWEET_ASSERT( type_ == TRANSITION_REDUCE );
    SWEET_ASSERT( reduced_symbol_ );
    SWEET_ASSERT( symbol );
    SWEET_ASSERT( length > 0 );
    SWEET_ASSERT( precedence >= 0 );
    SWEET_ASSERT( reduced_symbol_ != symbol );
    reduced_symbol_ = symbol;
    reduced_length_ = length;
    precedence_ = precedence;
    action_ = action;
}
