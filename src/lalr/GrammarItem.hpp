#ifndef LALR_GRAMMARITEM_HPP_INCLUDED
#define LALR_GRAMMARITEM_HPP_INCLUDED

#include "GrammarSymbolLess.hpp"
#include "GrammarSymbolSet.hpp"
#include <string>
#include <vector>
#include <set>

namespace lalr
{

class GrammarSymbol;
class GrammarProduction;

/**
// An item that defines the positions in the grammar that a state 
// represents.
*/
class GrammarItem
{
    unsigned short production_index_; ///< The index of the production of this item.
    unsigned short position_; ///< The position of the dot in this item.
    GrammarProduction* production_; ///< The production that this item is for.
    mutable int index_;

public:
    GrammarItem();
    GrammarItem( GrammarProduction* production, int position );
    int index() const;
    GrammarProduction* production() const;
    int position() const;
    bool dot_at_beginning() const;
    bool dot_at_end() const;
    bool next_node( const GrammarSymbol& symbol ) const;
    const GrammarSymbol* next_symbol() const;
    bool nullable_after_next() const;
    std::string label() const;
    bool operator<( const GrammarItem& item ) const;
    void set_index( int index ) const;
};

}

#endif
