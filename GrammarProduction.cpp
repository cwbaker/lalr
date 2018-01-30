//
// GrammarProduction.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "GrammarProduction.hpp"
#include "assert.hpp"

using namespace sweet::lalr;

GrammarProduction::GrammarProduction( GrammarSymbol* symbol )
: symbol_( symbol ),
  symbols_(),
  precedence_symbol_( nullptr ),
  action_( nullptr )
{
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

GrammarAction* GrammarProduction::action() const
{
    return action_;
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

void GrammarProduction::set_action( GrammarAction* action )
{
    action_ = action;
}
