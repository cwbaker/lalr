//
// GrammarLookahead.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "GrammarLookahead.hpp"
#include "GrammarItem.hpp"
#include "assert.hpp"

using std::vector;
using namespace lalr;

GrammarLookahead::GrammarLookahead( GrammarItem* item )
: item_( item )
, propagate_to_()
, lookaheads_{}
{
    LALR_ASSERT( item_ );
}

GrammarLookahead::GrammarLookahead( GrammarLookahead&& item )
: item_( nullptr )
, propagate_to_()
, lookaheads_()
{
    std::swap( item_, item.item_ );
    std::swap( propagate_to_, item.propagate_to_ );
    std::swap( lookaheads_, item.lookaheads_ );
}

GrammarItem* GrammarLookahead::item() const
{
    return item_;
}

const std::vector<GrammarLookahead*>& GrammarLookahead::propagate_to() const
{
    return propagate_to_;
}

const GrammarSymbolSet& GrammarLookahead::lookaheads() const
{
    return lookaheads_;
}

void GrammarLookahead::set_item( GrammarItem* item )
{
    LALR_ASSERT( item );
    item_ = item;
}

void GrammarLookahead::add_propagate_to( GrammarLookahead* propagate_to )
{
    LALR_ASSERT( propagate_to );
    LALR_ASSERT( propagate_to != this );
    propagate_to_.push_back( propagate_to );
}

size_t GrammarLookahead::add_lookaheads( const GrammarSymbolSet& lookaheads )
{
    return lookaheads_.insert( lookaheads );
}
