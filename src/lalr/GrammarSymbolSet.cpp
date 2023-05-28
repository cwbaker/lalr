//
// GrammarSymbolSet.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "GrammarSymbolSet.hpp"
#include "GrammarSymbol.hpp"
#include "assert.hpp"

using std::vector;
using namespace lalr;

GrammarSymbolSet::GrammarSymbolSet( size_t symbols )
: set_()
{
    set_.resize( symbols / 8 );
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
    size_t index = symbol_index / 8;
    if ( index < set_.size() )
    {
        unsigned char mask = 1 << (symbol_index % 8);
        return (set_[index] & mask) != 0;
    }
    return false;
}

bool GrammarSymbolSet::insert( const GrammarSymbol* symbol )
{
    if ( symbol )
    {
        int index = symbol->index() / 8;
        if ( index >= int(set_.size()) )
        {
            set_.insert( set_.end(), index - set_.size() + 1, 0 );
        }
        unsigned char mask = 1 << (symbol->index() % 8);
        if ( !(set_[index] & mask) )
        {
            set_[index] |= mask;
            // symbols_.push_back( symbol );
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
        unsigned char mask = set_[i];
        unsigned char new_mask = mask | set.set_[i];
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
