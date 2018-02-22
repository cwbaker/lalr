//
// GrammarProduction.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "GrammarProduction.hpp"
#include "assert.hpp"

using std::vector;
using namespace sweet::lalr;

GrammarProduction::GrammarProduction( int index, GrammarSymbol* symbol )
: index_( index ),
  symbol_( symbol ),
  symbols_(),
  precedence_symbol_( nullptr ),
  action_( nullptr ),
  line_( 0 )
{
    SWEET_ASSERT( index_ >= 0 );
    SWEET_ASSERT( symbol );
}

int GrammarProduction::index() const
{
    return index_;
}

GrammarSymbol* GrammarProduction::symbol() const
{
    return symbol_;
}

const std::vector<GrammarSymbol*>& GrammarProduction::symbols() const
{
    return symbols_;
}

GrammarSymbol* GrammarProduction::precedence_symbol() const
{
    return precedence_symbol_;
}

LalrAction* GrammarProduction::action() const
{
    return action_;
}

int GrammarProduction::line() const
{
    return line_;
}

int GrammarProduction::length() const
{
    return int(symbols_.size());
}

void GrammarProduction::append_symbol( GrammarSymbol* symbol )
{
    SWEET_ASSERT( symbol );
    symbols_.push_back( symbol );
}

void GrammarProduction::set_precedence_symbol( GrammarSymbol* precedence_symbol )
{
    precedence_symbol_ = precedence_symbol;
}

void GrammarProduction::set_action( LalrAction* action )
{
    action_ = action;
}

/**
// Replace references to \e to_symbol in this production by \e with_symbol.
//
// @param to_symbol
//  The symbol to replace references to (assumed not null).
//
// @param with_symbol
//  The symbol to replace the references with (assumed not null).
*/
void GrammarProduction::replace_references_to_symbol( GrammarSymbol* to_symbol, GrammarSymbol* with_symbol )
{
    SWEET_ASSERT( to_symbol );
    SWEET_ASSERT( with_symbol );

    if ( symbol_ == to_symbol )
    {
        symbol_ = with_symbol;
    }
    
    if ( precedence_symbol_ == to_symbol )
    {
        precedence_symbol_ = with_symbol;
    }

    for ( vector<GrammarSymbol*>::iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        if ( *i == to_symbol )
        {
            *i = with_symbol;
        }
    }
}
