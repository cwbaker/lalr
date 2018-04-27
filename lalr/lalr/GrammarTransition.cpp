//
// GrammarTransition.cpp
// Copyright (c) Charles Baker. All rights reserved.
//    

#include "GrammarTransition.hpp"
#include "GrammarSymbol.hpp"
#include "GrammarAction.hpp"
#include "assert.hpp"
#include <stdio.h>

using namespace lalr;

/**
// Constructor.
//
// @param symbol
//  The symbol that this transition is made on (assumed not null).
//
// @param reduced_symbol
//  The GrammarSymbol that this transition reduces to.
//
// @param length
//  The number of symbols on the right-hand side of the production that this 
//  transition reduces.
//
// @param action
//  The index of the action taken on this reduction or 
//  `GrammarAction::INVALID_INDEX` if no action is taken.
*/
GrammarTransition::GrammarTransition( const GrammarSymbol* symbol, const GrammarSymbol* reduced_symbol, int reduced_length, int precedence, int action )
: symbol_( symbol ),
  state_( NULL ),
  reduced_symbol_( reduced_symbol ),
  reduced_length_( reduced_length ),
  precedence_( precedence ),
  action_( action ),
  type_( TRANSITION_REDUCE ),
  index_( INVALID_INDEX )
{
    LALR_ASSERT( reduced_symbol_ );
    LALR_ASSERT( reduced_length_ >= 0 );
    LALR_ASSERT( precedence_ >= 0 );
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
GrammarTransition::GrammarTransition( const GrammarSymbol* symbol, GrammarState* state )
: symbol_( symbol ),
  state_( state ),
  reduced_symbol_( nullptr ),
  reduced_length_( 0 ),
  precedence_( 0 ),
  action_( GrammarAction::INVALID_INDEX ),
  type_( TRANSITION_SHIFT ),
  index_( INVALID_INDEX )
{
    LALR_ASSERT( symbol_ );
    LALR_ASSERT( state_ );
}

/**
// Get the state that this transition is to.
//
// @return
//  The state or null if this transition is a reduce transition.
*/
GrammarState* GrammarTransition::state() const
{
    return state_;
}

const GrammarSymbol* GrammarTransition::reduced_symbol() const
{
    return reduced_symbol_;
}

int GrammarTransition::reduced_length() const
{
    return reduced_length_;
}

int GrammarTransition::precedence() const
{
    return precedence_;
}

int GrammarTransition::action() const
{
    return action_;
}

/**
// Get the type of this transition.
//
// @return
//  The type of this transition.
*/
TransitionType GrammarTransition::type() const
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
bool GrammarTransition::taken_on_symbol( const GrammarSymbol* symbol ) const
{
    return symbol_ == symbol;
}

/**
// Get the symbol that this transition is on.
//
// @return
//  The symbol.
*/
const GrammarSymbol* GrammarTransition::symbol() const
{
    LALR_ASSERT( symbol_ );
    return symbol_;
}

/**
// Get the index of this transition.
//
// @return
//  The index.
*/
int GrammarTransition::index() const
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
bool GrammarTransition::operator<( const GrammarTransition& transition ) const
{
    LALR_ASSERT( symbol_->index() >= 0 );
    LALR_ASSERT( transition.symbol()->index() >= 0 );
    return symbol_->index() < transition.symbol()->index();
}

/**
// Set the index of this transition.
//
// @param index
//  The value to set the index of this production to.
*/
void GrammarTransition::set_index( int index ) const
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
void GrammarTransition::override_shift_to_reduce( const GrammarSymbol* symbol, int length, int precedence, int action ) const
{
    LALR_ASSERT( type_ == TRANSITION_SHIFT );
    LALR_ASSERT( state_ );
    LALR_ASSERT( !reduced_symbol_ );
    LALR_ASSERT( length >= 0 );
    LALR_ASSERT( precedence >= 0 );
    LALR_ASSERT( symbol );    
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
void GrammarTransition::override_reduce_to_reduce( const GrammarSymbol* symbol, int length, int precedence, int action ) const
{
    LALR_ASSERT( type_ == TRANSITION_REDUCE );
    LALR_ASSERT( reduced_symbol_ );
    LALR_ASSERT( symbol );
    LALR_ASSERT( length >= 0 );
    LALR_ASSERT( precedence >= 0 );
    LALR_ASSERT( reduced_symbol_ != symbol );
    reduced_symbol_ = symbol;
    reduced_length_ = length;
    precedence_ = precedence;
    action_ = action;
}
