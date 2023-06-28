#pragma once

#include <stddef.h>
#include <vector>
#include <set>

namespace lalr
{

class GrammarSymbol;

class GrammarSymbolSet
{
    typedef size_t vector_set_t;
    std::vector<vector_set_t> set_;
    size_t minimum_;
    size_t maximum_;

public:
    GrammarSymbolSet( size_t symbols );
    GrammarSymbolSet( GrammarSymbolSet&& set );
    GrammarSymbolSet( const GrammarSymbolSet& set );
    GrammarSymbolSet& operator=( GrammarSymbolSet&& set );
    GrammarSymbolSet& operator=( const GrammarSymbolSet& set );

    size_t minimum_index() const;
    size_t maximum_index() const;
    bool contains( size_t symbol_index ) const;
    bool insert( const GrammarSymbol* symbol );
    int insert( const GrammarSymbolSet& set );
};

}
