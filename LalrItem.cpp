//
// LalrItem.cpp
// Copyright (c) Charles Baker. All rights reserved.
//    

#include "LalrItem.hpp"
#include "LalrSymbol.hpp"
#include "LalrProduction.hpp"
#include "assert.hpp"

using std::vector;
using namespace sweet;
using namespace sweet::lalr;

/**
// Constructor.
*/
LalrItem::LalrItem()
: production_(),
  position_( 0 ),
  lookahead_symbols_()
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
LalrItem::LalrItem( LalrProduction* production, int position )
: production_( production ),
  position_( position ),
  lookahead_symbols_()
{
    SWEET_ASSERT( production_ );
    SWEET_ASSERT( position_ >= 0 && position_ < production_->get_length() + 1 );
}

/**
// Get the production for this item.
*/
LalrProduction* LalrItem::get_production() const
{
    return production_;
}

/**
// Get the position of the dot in this item.
//
// @return
//  The position of the dot in this item.
*/
int LalrItem::get_position() const
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
bool LalrItem::is_dot_at_beginning() const
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
bool LalrItem::is_dot_at_end() const
{
    return position_ == production_->get_length();
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
bool LalrItem::is_next_node( const LalrSymbol& symbol ) const
{
    return production_->get_symbol_by_position(position_) == &symbol;
}

/**
// Get a string that describes the nodes that make up this item.
//
// @param description
//  A variable to receive the description of this item (assumed not null).
*/
void LalrItem::describe( std::string* description ) const
{
    SWEET_ASSERT( description );
    SWEET_ASSERT( production_ );
        
    int position = 0;
    const vector<LalrSymbol*>& symbols = production_->get_symbols();
    vector<LalrSymbol*>::const_iterator i = symbols.begin(); 
    while ( i != symbols.end() && position < position_ )
    {
        const LalrSymbol* symbol = *i;
        SWEET_ASSERT( symbol );
        symbol->describe( description );
        description->append( " " );
        ++i;
        ++position;
    }
    description->append( ". " );
    
    while ( i != symbols.end() )
    {
        const LalrSymbol* symbol = *i;
        SWEET_ASSERT( symbol );
        symbol->describe( description );
        description->append( " " );
        ++i;
    }

    std::set<const LalrSymbol*>::const_iterator j = lookahead_symbols_.begin();
    if ( j != lookahead_symbols_.end() )
    {
        const LalrSymbol* symbol = *j;
        SWEET_ASSERT( symbol );
        description->append( "; " );
        symbol->describe( description );
        ++j;
    }

    while ( j != lookahead_symbols_.end() )
    {
        const LalrSymbol* symbol = *j;
        SWEET_ASSERT( symbol );
        description->append( ", " );
        symbol->describe( description );
        ++j;
    }
}

/**
// Add \e lookahead_symbols to the lookahead symbols for this item.
//
// @param lookahead_symbols
//  The lookahead symbols to add to this item.
//
// @return
//  The number of symbols added to the lookahead set of this item.
*/
int LalrItem::add_lookahead_symbols( const std::set<const LalrSymbol*>& lookahead_symbols ) const
{
    size_t original_size = lookahead_symbols_.size();
    lookahead_symbols_.insert( lookahead_symbols.begin(), lookahead_symbols.end() );
    return lookahead_symbols_.size() - original_size;
}

/**
// Get the lookahead set for this item.
//
// @return
//  The lookahead set.
*/
const std::set<const LalrSymbol*>& LalrItem::get_lookahead_symbols() const
{
    return lookahead_symbols_;
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
bool LalrItem::operator<( const LalrItem& item ) const
{
    return 
        production_->get_index() < item.production_->get_index() || 
        (production_->get_index() == item.production_->get_index() && position_ < item.position_)
    ;
}
