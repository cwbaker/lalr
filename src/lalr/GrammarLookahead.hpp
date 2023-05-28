#pragma once

#include "GrammarSymbolSet.hpp"
#include <vector>

namespace lalr
{

class GrammarItem;

class GrammarLookahead
{
    GrammarItem* item_;
    std::vector<GrammarLookahead*> propagate_to_;
    GrammarSymbolSet lookaheads_;

public:
    GrammarLookahead( GrammarItem* item, size_t symbols );
    GrammarLookahead( GrammarLookahead&& item );

    GrammarItem* item() const;
    const std::vector<GrammarLookahead*>& propagate_to() const;
    const GrammarSymbolSet& lookaheads() const;
    void set_item( GrammarItem* item );
    void add_propagate_to( GrammarLookahead* propagate_to );
    size_t add_lookaheads( const GrammarSymbolSet& lookaheads );

    GrammarLookahead( const GrammarLookahead& item ) = delete;
    GrammarLookahead& operator=( const GrammarLookahead& item ) = delete;
};

}
