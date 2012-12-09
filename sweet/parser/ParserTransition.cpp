//
// ParserTransition.cpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//    

#include "stdafx.hpp"
#include "ParserTransition.hpp"
#include "ParserProduction.hpp"
#include "ParserSymbol.hpp"
#include "ParserItem.hpp"
#include "ParserState.hpp"
#include <stdio.h>

using namespace sweet::parser;

#if defined(BUILD_PLATFORM_MSVC)
#define snprintf _snprintf
#endif

/**
// Constructor.
//
// @param symbol
//  The symbol that this transition is made on (assumed not null).
//
// @param state
//  The state that this transition is to (assumed not null).
*/
ParserTransition::ParserTransition( const ParserSymbol* symbol, ParserState* state )
: symbol_( symbol ),
  state_( state ),
  reduced_production_( NULL ),
  type_( TRANSITION_SHIFT ),
  index_( INVALID_INDEX )
{
    SWEET_ASSERT( symbol_ );
    SWEET_ASSERT( state_ );
}

/**
// Constructor.
//
// @param symbol
//  The symbol that this transition is made on (assumed not null).
//
// @param reduced_production
//  The production that is reduced on this transition (assumed not null).
*/
ParserTransition::ParserTransition( const ParserSymbol* symbol, const ParserProduction* reduced_production )
: symbol_( symbol ),
  state_( NULL ),
  reduced_production_( reduced_production ),
  type_( TRANSITION_REDUCE ),
  index_( INVALID_INDEX )
{
    SWEET_ASSERT( reduced_production );
}

/**
// Get the state that this transition is to.
//
// @return
//  The state or null if this transition is a reduce transition.
*/
ParserState* ParserTransition::get_state() const
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
const ParserProduction* ParserTransition::get_reduced_production() const
{
    return reduced_production_;
}

/**
// Get the type of this transition.
//
// @return
//  The type of this transition.
*/
int ParserTransition::get_type() const
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
bool ParserTransition::is_symbol( const ParserSymbol* symbol ) const
{
    return symbol_ == symbol;
}

/**
// Get the symbol that this transition is on.
//
// @return
//  The symbol.
*/
const ParserSymbol* ParserTransition::get_symbol() const
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
void ParserTransition::describe( std::string* description ) const
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
            SWEET_ASSERT( reduced_production_ );
            SWEET_ASSERT( reduced_production_->get_symbol() );
            char buffer [512];
            snprintf( buffer, sizeof(buffer), "reduce to %s on ", reduced_production_->get_symbol()->get_identifier().c_str() );
            buffer [sizeof(buffer) - 1] = '\0';
            description->append( buffer );
            break;
            
        default:
            SWEET_ASSERT( false );
            break;
    }
    
    symbol_->describe( description );
    description->append( " ; " );
}

/**
// Set the index of this transition.
//
// @param index
//  The value to set the index of this production to.
*/
void ParserTransition::set_index( int index ) const
{
    index_ = index;
}

/**
// Get the index of this transition.
//
// @return
//  The index.
*/
int ParserTransition::get_index() const
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
void ParserTransition::override_shift_to_reduce( const ParserProduction* reduced_production ) const
{
    SWEET_ASSERT( type_ == TRANSITION_SHIFT );
    SWEET_ASSERT( state_ );
    SWEET_ASSERT( !reduced_production_ );
    SWEET_ASSERT( reduced_production );
    
    type_ = TRANSITION_REDUCE;
    state_ = NULL;
    reduced_production_ = reduced_production;
}

/**
// Change this transition from being a reduce transition for one production 
// into a reduce transition for a different production.
//
// @param reduced_production
//  The production to reduce by when this transition is taken.
*/
void ParserTransition::override_reduce_to_reduce( const ParserProduction* reduced_production ) const
{
    SWEET_ASSERT( type_ == TRANSITION_REDUCE );
    SWEET_ASSERT( reduced_production_ );
    SWEET_ASSERT( reduced_production );
    SWEET_ASSERT( reduced_production_ != reduced_production );
    reduced_production_ = reduced_production;
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
bool ParserTransition::operator<( const ParserTransition& transition ) const
{
    return symbol_ < transition.symbol_;
}
