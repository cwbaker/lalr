//
// GrammarSymbolSet.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "GrammarSymbolSet.hpp"
#include "GrammarSymbol.hpp"
#include "assert.hpp"

using std::vector;
using namespace lalr;

static const size_t ONE = 1;
static const size_t BITS_PER_ELEMENT = sizeof(size_t) * 8;

GrammarSymbolSet::GrammarSymbolSet( size_t symbols )
: set_()
{
    set_.resize( symbols / BITS_PER_ELEMENT );
}

GrammarSymbolSet::GrammarSymbolSet( GrammarSymbolSet&& set )
: set_( std::move(set.set_) )
{
}

GrammarSymbolSet::GrammarSymbolSet( const GrammarSymbolSet& set )
: set_( set.set_ )
{
}

GrammarSymbolSet& GrammarSymbolSet::operator=( GrammarSymbolSet&& set )
{
    if ( this != &set )
    {
        std::swap( set_, set.set_ );
    }
    return *this;
}

GrammarSymbolSet& GrammarSymbolSet::operator=( const GrammarSymbolSet& set )
{
    if ( this != &set )
    {
        set_ = set.set_;
    }
    return *this;    
}

bool GrammarSymbolSet::contains( int symbol_index ) const
{
    size_t index = symbol_index / BITS_PER_ELEMENT;
    if ( index < set_.size() )
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
    for ( size_t i = 0; i < set.set_.size(); ++i )
    {
        size_t mask = set_[i];
        size_t new_mask = mask | set.set_[i];
        if ( mask != new_mask )
        {
            set_[i] = new_mask;
            ++added;
        }
    }
    return added;
}

void GrammarSymbolSet::swap( GrammarSymbolSet&& set )
{
    std::swap( set_, set.set_ );
}
