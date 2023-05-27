#pragma once

#include <stddef.h>
#include <vector>
#include <set>

namespace lalr
{

class GrammarSymbol;

class GrammarSymbolSet
{
    std::vector<unsigned char> set_;
    std::vector<const GrammarSymbol*> symbols_;

public:
    GrammarSymbolSet();
    GrammarSymbolSet( GrammarSymbolSet&& set );
    GrammarSymbolSet( const GrammarSymbolSet& set );
    GrammarSymbolSet& operator=( GrammarSymbolSet&& set );
    GrammarSymbolSet& operator=( const GrammarSymbolSet& set );

    size_t empty() const;
    size_t size() const;
    std::vector<const GrammarSymbol*>::const_iterator begin() const;
    std::vector<const GrammarSymbol*>::const_iterator end() const;
    const std::vector<const GrammarSymbol*>& symbols() const;
    bool insert( const GrammarSymbol* symbol );
    int insert( const GrammarSymbolSet& set );
    void swap( GrammarSymbolSet&& set );
};

}
