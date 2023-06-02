//
// GrammarSymbolSet.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "GrammarSymbolSet.hpp"
#include "GrammarSymbol.hpp"
#include "assert.hpp"
#include <algorithm>
#include <limits>

using std::min;
using std::max;
using std::vector;
using std::numeric_limits;
using namespace lalr;

static const size_t ONE = 1;
static const size_t BITS_PER_ELEMENT = sizeof(size_t) * 8;

GrammarSymbolSet::GrammarSymbolSet( size_t symbols )
: set_()
, minimum_(numeric_limits<size_t>::max())
, maximum_(numeric_limits<size_t>::min())
{
    set_.resize( symbols / BITS_PER_ELEMENT );
}

GrammarSymbolSet::GrammarSymbolSet( GrammarSymbolSet&& set )
: set_( std::move(set.set_) )
, minimum_(numeric_limits<size_t>::max())
, maximum_(numeric_limits<size_t>::min())
{
}

GrammarSymbolSet::GrammarSymbolSet( const GrammarSymbolSet& set )
: set_( set.set_ )
, minimum_(numeric_limits<size_t>::max())
, maximum_(numeric_limits<size_t>::min())
{
}

GrammarSymbolSet& GrammarSymbolSet::operator=( GrammarSymbolSet&& set )
{
    if ( this != &set )
    {
        std::swap( set_, set.set_ );
        std::swap( minimum_, set.minimum_ );
        std::swap( maximum_, set.maximum_ );
    }
    return *this;
}

GrammarSymbolSet& GrammarSymbolSet::operator=( const GrammarSymbolSet& set )
{
    if ( this != &set )
    {
        set_ = set.set_;
        minimum_ = set.minimum_;
        maximum_ = set.maximum_;
    }
    return *this;    
}

int GrammarSymbolSet::minimum_index() const
{
    return int(minimum_ * BITS_PER_ELEMENT);
}

int GrammarSymbolSet::maximum_index() const
{
    return int((maximum_ + 1) * BITS_PER_ELEMENT);
}

bool GrammarSymbolSet::contains( int symbol_index ) const
{
    size_t index = symbol_index / BITS_PER_ELEMENT;
    if ( index >= minimum_ && index <= maximum_ )
    {
        size_t mask = ONE << (symbol_index % BITS_PER_ELEMENT);
        return (set_[index] & mask) != 0;
    }
    return false;
}

bool GrammarSymbolSet::insert( const GrammarSymbol* symbol )
{
    if ( symbol )
    {
        size_t index = symbol->index() / BITS_PER_ELEMENT;
        if ( index >= set_.size() )
        {
            set_.insert( set_.end(), index - set_.size() + 1, 0 );
        }
        size_t mask = ONE << (symbol->index() % BITS_PER_ELEMENT);
        if ( !(set_[index] & mask) )
        {
            set_[index] |= mask;
            minimum_ = min( minimum_, index );
            maximum_ = max( maximum_, index );
            return true;
        }
    }
    return false;
}

int GrammarSymbolSet::insert( const GrammarSymbolSet& set )
{
    if ( set_.size() < set.set_.size() )
    {
        set_.insert( set_.end(), set.set_.size() - set_.size(), 0 );
    }

    int added = 0;
    for ( size_t i = set.minimum_; i < set.maximum_ + 1; ++i )
    {
        size_t mask = set_[i];
        size_t new_mask = mask | set.set_[i];
        if ( mask != new_mask )
        {
            set_[i] = new_mask;
            ++added;
        }
    }
    minimum_ = min( minimum_, set.minimum_ );
    maximum_ = min( maximum_, set.maximum_ );
    return added;
}
