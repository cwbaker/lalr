//
// LalrTransition.cpp
// Copyright (c) Charles Baker. All rights reserved.
//    

#include "LalrTransition.hpp"
#include "LalrSymbol.hpp"
#include "State.hpp"
#include "Action.hpp"
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
//  The LalrSymbol that this transition reduces to.
//
// @param length
//  The number of symbols on the right-hand side of the production that this 
//  transition reduces.
//
// @param action
//  The index of the action taken on this reduction or 
//  `Action::INVALID_INDEX` if no action is taken.
*/
LalrTransition::LalrTransition( const LalrSymbol* symbol, const LalrSymbol* reduced_symbol, int reduced_length, int precedence, int action )
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
LalrTransition::LalrTransition( const LalrSymbol* symbol, State* state )
: symbol_( symbol ),
  state_( state ),
  reduced_symbol_( nullptr ),
  reduced_length_( 0 ),
  precedence_( 0 ),
  action_( Action::INVALID_INDEX ),
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
State* LalrTransition::get_state() const
{
    return state_;
}

/**
// Get the production that is reduced on this transition.
//
// @return
//  The production that is reduced on this transition or null if this 
//  transition is a shift.
*/
// const LalrProduction* LalrTransition::get_reduced_production() const
// {
//     return reduced_production_;
// }

const LalrSymbol* LalrTransition::reduced_symbol() const
{
    return reduced_symbol_;
}

int LalrTransition::reduced_length() const
{
    return reduced_length_;
}

int LalrTransition::precedence() const
{
    return precedence_;
}

int LalrTransition::action() const
{
    return action_;
}

/**
// Get the type of this transition.
//
// @return
//  The type of this transition.
*/
TransitionType LalrTransition::get_type() const
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
bool LalrTransition::is_symbol( const LalrSymbol* symbol ) const
{
    return symbol_ == symbol;
}

/**
// Get the symbol that this transition is on.
//
// @return
//  The symbol.
*/
const LalrSymbol* LalrTransition::get_symbol() const
{
    SWEET_ASSERT( symbol_ );
    return symbol_;
}

/**
// Describe this transition.
//
// @param description
//  A variable to append the description of this transition to (assumed not 
//  null).
*/
void LalrTransition::describe( std::string* description ) const
{
    SWEET_ASSERT( description != NULL );

    switch ( type_ )
    {
        case TRANSITION_SHIFT:
        {
            SWEET_ASSERT( state_ );
            char buffer [512];
            snprintf( buffer, sizeof(buffer), "shift to %d on ", state_->get_index() );
            buffer [sizeof(buffer) - 1] = '\0';
            description->append( buffer );
            break;
        }
            
        case TRANSITION_REDUCE:
            // SWEET_ASSERT( reduced_production_ );
            SWEET_ASSERT( reduced_symbol_ );
            char buffer [512];
            snprintf( buffer, sizeof(buffer), "reduce to %s on ", reduced_symbol_->identifier().c_str() );
            buffer [sizeof(buffer) - 1] = '\0';
            description->append( buffer );
            break;
            
        default:
            SWEET_ASSERT( false );
            break;
    }
    
    // symbol_->describe( description );
    description->append( " ; " );
}

/**
// Set the index of this transition.
//
// @param index
//  The value to set the index of this production to.
*/
void LalrTransition::set_index( int index ) const
{
    index_ = index;
}

/**
// Get the index of this transition.
//
// @return
//  The index.
*/
int LalrTransition::get_index() const
{
    return index_;
}

/**
// Change this transition from being a shift transition into being a reduce
// transition.
//
// @param reduced_production
//  The production to reduce by when this transition is taken.
*/
void LalrTransition::override_shift_to_reduce( const LalrSymbol* symbol, int length, int precedence, int action ) const
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
void LalrTransition::override_reduce_to_reduce( const LalrSymbol* symbol, int length, int precedence, int action ) const
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
bool LalrTransition::operator<( const LalrTransition& transition ) const
{
    return symbol_ < transition.symbol_;
}
