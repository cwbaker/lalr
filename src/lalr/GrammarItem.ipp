//
// GrammarItem.ipp
// Copyright (c) Charles Baker. All rights reserved.
//    
#pragma once

#include "GrammarItem.hpp"
#include "GrammarSymbol.hpp"
#include "GrammarProduction.hpp"
#include "assert.hpp"
#include <algorithm>
#include <limits>

namespace lalr
{

/**
// Constructor.
*/
GrammarItem::GrammarItem()
: production_( 0 )
, position_( 0 )
, index_( 0 )
{
}

/**
// Constructor.
//
// @param production
//  The production that this item is for.
//
// @param position
//  The position of the dot in this item.
*/
GrammarItem::GrammarItem( GrammarProduction* production, int position )
: production_( production->index() )
, position_( position )
, index_( 0 )
{
    using std::numeric_limits;
    LALR_ASSERT( production_ >= 0 && production_ < numeric_limits<unsigned short>::max() );
    LALR_ASSERT( position_ >= 0 && position_ <  numeric_limits<unsigned short>::max() );
    LALR_ASSERT( position_ >= 0 && position_ < production->length() + 1 );
}

/**
// Get the index of this item.
//
// @return
//  The index of this item.
*/
int GrammarItem::index() const
{
    return index_;
}

/**
// Get the index of the production for this item.
*/
int GrammarItem::production() const
{
    return production_;
}

/**
// Get the position of the dot in this item.
//
// @return
//  The position of the dot in this item.
*/
int GrammarItem::position() const
{
    return position_;
}

/**
// Is the dot that marks the position of this item at the beginning of the
// production?
//
// @return
//  True if the dot is at the beginning of the production otherwise false.
*/
bool GrammarItem::dot_at_beginning() const
{
    return position_ == 0;
}

/**
// Less than operator.
//
// @param item
//  The item to compare this item with.
//
// @return 
//  True if this item's production is less than \e item's or if this item's
//  production is the same as \e item's then if this item's next nodes
//  are lexically less than item's otherwise false.
*/
bool GrammarItem::operator<( const GrammarItem& item ) const
{
    return 
        production_ < item.production_ || 
        (production_ == item.production_ && position_ < item.position_)
    ;
}

/**
// Set the index of this item.
//
// @param index
//  The index to set this item (assumed >= 0).
*/
void GrammarItem::set_index( int index ) const
{
    index_ = index;
}

}
