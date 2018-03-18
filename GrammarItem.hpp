#ifndef SWEET_LALR_GRAMMARITEM_HPP_INCLUDED
#define SWEET_LALR_GRAMMARITEM_HPP_INCLUDED

#include <string>
#include <set>

namespace sweet
{

namespace lalr
{

class Symbol;
class Production;

/**
// An item that defines the positions in the grammar that a state 
// represents.
*/
class GrammarItem
{
    Production* production_; ///< The production that this item is for.
    int position_; ///< The position of the dot in this item.
    mutable std::set<const Symbol*> lookahead_symbols_; ///< The lookahead Symbols for this item.

    public:
        GrammarItem();
        GrammarItem( Production* production, int position );
        Production* production() const;
        int position() const;
        bool dot_at_beginning() const;
        bool dot_at_end() const;
        bool next_node( const Symbol& symbol ) const;
        const std::set<const Symbol*>& lookahead_symbols() const;
        bool operator<( const GrammarItem& item ) const;
        int add_lookahead_symbols( const std::set<const Symbol*>& lookahead_symbols ) const;
};

}

}

#endif
