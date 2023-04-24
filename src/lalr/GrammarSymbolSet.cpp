//
// GrammarSymbolSet.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "GrammarSymbolSet.hpp"
#include "GrammarSymbol.hpp"
#include "assert.hpp"

using std::vector;
using namespace lalr;

GrammarSymbolSet::GrammarSymbolSet()
: set_()
, symbols_()
{
}

GrammarSymbolSet::GrammarSymbolSet( GrammarSymbolSet&& set )
: set_()
, symbols_()
{
    std::swap( set_, set.set_ );
    std::swap( symbols_, set.symbols_ );
}

GrammarSymbolSet::GrammarSymbolSet( const GrammarSymbolSet& set )
: set_( set.set_ )
, symbols_( set.symbols_ )
{
}

GrammarSymbolSet& GrammarSymbolSet::operator=( GrammarSymbolSet&& set )
{
    if ( this != &set )
    {
        std::swap( set_, set.set_ );
        std::swap( symbols_, set.symbols_ );
    }
    return *this;
}

GrammarSymbolSet& GrammarSymbolSet::operator=( const GrammarSymbolSet& set )
{
    if ( this != &set )
    {
        set_ = set.set_;
        symbols_ = set.symbols_;
    }
    return *this;    
}

size_t GrammarSymbolSet::empty() const
{
    return symbols_.empty();
}

size_t GrammarSymbolSet::size() const
{
    return symbols_.size();
}

std::vector<const GrammarSymbol*>::const_iterator GrammarSymbolSet::begin() const
{
    return symbols_.begin();
}

std::vector<const GrammarSymbol*>::const_iterator GrammarSymbolSet::end() const
{
    return symbols_.end();
}

const std::vector<const GrammarSymbol*>& GrammarSymbolSet::symbols() const
{
    return symbols_;
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
            symbols_.push_back( symbol );
            return true;
        }
    }
    return false;
}

int GrammarSymbolSet::insert( std::set<const GrammarSymbol*, GrammarSymbolLess>::const_iterator begin, std::set<const GrammarSymbol*, GrammarSymbolLess>::const_iterator end )
{
    int added = 0;
    for ( auto i = begin; i != end; ++i )
    {
        added += insert( *i ) ? 1 : 0;
    }
    return added;
}

int GrammarSymbolSet::insert( const GrammarSymbolSet& set )
{
    int added = 0;
    const vector<const GrammarSymbol*>& symbols = set.symbols_;
    for ( const GrammarSymbol* symbol : symbols )
    {
        LALR_ASSERT( symbol );
        added += insert( symbol ) ? 1 : 0;
    }
    return added;
}

void GrammarSymbolSet::swap( GrammarSymbolSet&& set )
{
    std::swap( set_, set.set_ );
    std::swap( symbols_, set.symbols_ );
}
