//
// ParserItem.cpp
// Copyright (c) Charles Baker. All rights reserved.
//    

#include "ParserItem.hpp"
#include "ParserSymbol.hpp"
#include "ParserProduction.hpp"
#include <sweet/assert/assert.hpp>

using std::vector;
using namespace sweet;
using namespace sweet::parser;

/**
// Constructor.
*/
ParserItem::ParserItem()
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
ParserItem::ParserItem( std::shared_ptr<ParserProduction> production, int position )
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
std::shared_ptr<ParserProduction> ParserItem::get_production() const
{
    return production_;
}

/**
// Get the position of the dot in this item.
//
// @return
//  The position of the dot in this item.
*/
int ParserItem::get_position() const
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
bool ParserItem::is_dot_at_beginning() const
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
bool ParserItem::is_dot_at_end() const
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
bool ParserItem::is_next_node( const ParserSymbol& symbol ) const
{
    return production_->get_symbol_by_position(position_) == &symbol;
}

/**
// Get a string that describes the nodes that make up this item.
//
// @param description
//  A variable to receive the description of this item (assumed not null).
*/
void ParserItem::describe( std::string* description ) const
{
    SWEET_ASSERT( description );
    SWEET_ASSERT( production_ );
        
    int position = 0;
    const vector<ParserSymbol*>& symbols = production_->get_symbols();
    vector<ParserSymbol*>::const_iterator i = symbols.begin(); 
    while ( i != symbols.end() && position < position_ )
    {
        const ParserSymbol* symbol = *i;
        SWEET_ASSERT( symbol );
        symbol->describe( description );
        description->append( " " );
        ++i;
        ++position;
    }
    description->append( ". " );
    
    while ( i != symbols.end() )
    {
        const ParserSymbol* symbol = *i;
        SWEET_ASSERT( symbol );
        symbol->describe( description );
        description->append( " " );
        ++i;
    }

    std::set<const ParserSymbol*>::const_iterator j = lookahead_symbols_.begin();
    if ( j != lookahead_symbols_.end() )
    {
        const ParserSymbol* symbol = *j;
        SWEET_ASSERT( symbol );
        description->append( "; " );
        symbol->describe( description );
        ++j;
    }

    while ( j != lookahead_symbols_.end() )
    {
        const ParserSymbol* symbol = *j;
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
int ParserItem::add_lookahead_symbols( const std::set<const ParserSymbol*>& lookahead_symbols ) const
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
const std::set<const ParserSymbol*>& ParserItem::get_lookahead_symbols() const
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
bool ParserItem::operator<( const ParserItem& item ) const
{
    return 
        production_->get_index() < item.production_->get_index() || 
        (production_->get_index() == item.production_->get_index() && position_ < item.position_)
    ;
}
