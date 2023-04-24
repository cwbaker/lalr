//
// GrammarItem.cpp
// Copyright (c) Charles Baker. All rights reserved.
//    

#include "GrammarItem.hpp"
#include "GrammarSymbol.hpp"
#include "GrammarProduction.hpp"
#include "assert.hpp"
#include <algorithm>
#include <limits>

using std::find;
using std::string;
using std::vector;
using std::numeric_limits;
using namespace lalr;

/**
// Constructor.
*/
GrammarItem::GrammarItem()
: production_index_( 0 )
, position_( 0 )
, production_( nullptr )
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
: production_index_( production->index() )
, position_( position )
, production_( production )
, index_( 0 )
{
    LALR_ASSERT( production_ );
    LALR_ASSERT( production_->index() >= 0 && production_->index() < numeric_limits<unsigned short>::max() );
    LALR_ASSERT( position_ >= 0 && position_ <  numeric_limits<unsigned short>::max() );
    LALR_ASSERT( position_ >= 0 && position_ < production_->length() + 1 );
}

int GrammarItem::index() const
{
    return index_;
}

/**
// Get the production for this item.
*/
GrammarProduction* GrammarItem::production() const
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
// Is the dot that marks the position of this item at the end of the 
// production?
//
// @return
//  True if the dot is at the end of the production otherwise false.
*/
bool GrammarItem::dot_at_end() const
{
    return position_ == production_->length();
}

/**
// Is \e symbol one of the symbols that could be visited next from this item?
//
// @param symbol
//  The symbol to check for being a next symbol of this item.
//
// @return
//  True if \e symbol could be one of the next symbols to be visited from
//  this item otherwise false.
*/
bool GrammarItem::next_node( const GrammarSymbol& symbol ) const
{
    return production_->symbol_by_position( position_ ) == &symbol;
}

/**
// Get the symbol after the dot in this item.
//
// @return
//  The next symbol, i.e. following the dot, in this item or null if there
//  is no such symbol.
*/
const GrammarSymbol* GrammarItem::next_symbol() const
{
    return production_->symbol_by_position( position_ );
}

bool GrammarItem::nullable_after_next() const
{
    return production_->nullable_after( position_ + 1 );
}

/**
// Less than operator.
//
// @param item
//  The item to compare this item with.
//
// @return 
//  True if this items production is less than \e item's or if this item's
//  production is the same as \e item's then if this item's next nodes
//  are lexically less than item's otherwise false.
*/
bool GrammarItem::operator<( const GrammarItem& item ) const
{
    return 
        production_index_ < item.production_index_ || 
        (production_index_ == item.production_index_ && position_ < item.position_)
    ;
}

void GrammarItem::set_index( int index ) const
{
    index_ = index;
}
