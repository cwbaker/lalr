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
// Get the state that this transition is to.
//
// @return
//  The state or null if this transition is a reduction.
*/
GrammarState* GrammarTransition::state() const
{
    return state_;
}

/**
// Get the production that this transition reduces.
//
// @return
//  The reduced production or null if this transition is a shift.
*/
const GrammarProduction* GrammarTransition::production() const
{
    return production_;
}

bool GrammarTransition::is_shift() const
{
    return state_ != nullptr;
}

bool GrammarTransition::is_reduce() const
{
    return production_ != nullptr;
}

const GrammarSymbol* GrammarTransition::reduced_symbol() const
{
    return production_ ? production_->symbol() : nullptr;
}

int GrammarTransition::reduced_length() const
{
    return production_ ? production_->length() : 0;
}

int GrammarTransition::precedence() const
{
    return production_ ? production_->precedence() : 0;
}

int GrammarTransition::action() const
{
    return production_ ? production_->action_index() : 0;
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
    return symbol_;
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
