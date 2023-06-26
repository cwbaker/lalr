//
// GrammarTransition.cpp
// Copyright (c) Charles Baker. All rights reserved.
//    

#include "GrammarTransition.hpp"
#include "GrammarProduction.hpp"
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
// @param production
//  The GrammarProduction that this transition reduces (assumed not null).
*/
GrammarTransition::GrammarTransition( const GrammarSymbol* symbol, const GrammarProduction* production )
: symbol_( symbol )
, state_( nullptr )
, production_( production )
{
    LALR_ASSERT( symbol_ );
    LALR_ASSERT( production_ );
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
: symbol_( symbol )
, state_( state )
, production_( nullptr )
{
    LALR_ASSERT( symbol_ );
    LALR_ASSERT( state_ );
}

/**
// Change this transition from a shift into a reduction.
//
// @param reduced_production
//  The production to reduce by when this transition is taken.
*/
void GrammarTransition::override_shift_to_reduce( const GrammarProduction* production ) const
{
    LALR_ASSERT( state_ );
    LALR_ASSERT( !production_ );
    LALR_ASSERT( production );
    state_ = nullptr;
    production_ = production;
}

/**
// Change this transition from being a reduce transition for one production 
// into a reduce transition for a different production.
//
// @param reduced_production
//  The production to reduce by when this transition is taken.
*/
void GrammarTransition::override_reduce_to_reduce( const GrammarProduction* production ) const
{
    LALR_ASSERT( !state_ );
    LALR_ASSERT( production );
    production_ = production;
}
