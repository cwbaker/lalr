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
    inline GrammarLookahead( GrammarItem* item, size_t symbols );
    inline GrammarLookahead( GrammarLookahead&& item );

    inline GrammarItem* item() const;
    inline const std::vector<GrammarLookahead*>& propagate_to() const;
    inline const GrammarSymbolSet& lookaheads() const;
    inline void set_item( GrammarItem* item );
    inline void add_propagate_to( GrammarLookahead* propagate_to );
    inline size_t add_lookaheads( const GrammarSymbolSet& lookaheads );

    GrammarLookahead( const GrammarLookahead& item ) = delete;
    GrammarLookahead& operator=( const GrammarLookahead& item ) = delete;
};

}

#include "GrammarLookahead.ipp"
