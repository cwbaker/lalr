#pragma once

#include <stddef.h>
#include <vector>
#include <set>

namespace lalr
{

class GrammarSymbol;

class GrammarSymbolSet
{
    std::vector<size_t> set_;
    size_t minimum_;
    size_t maximum_;

public:
    GrammarSymbolSet( size_t symbols );
    GrammarSymbolSet( GrammarSymbolSet&& set );
    GrammarSymbolSet( const GrammarSymbolSet& set );
    GrammarSymbolSet& operator=( GrammarSymbolSet&& set );
    GrammarSymbolSet& operator=( const GrammarSymbolSet& set );

    int minimum_index() const;
    int maximum_index() const;
    bool contains( int symbol_index ) const;
    bool insert( const GrammarSymbol* symbol );
    int insert( const GrammarSymbolSet& set );
};

}
