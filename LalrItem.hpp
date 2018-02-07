#ifndef SWEET_PARSER_PARSERITEM_HPP_INCLUDED
#define SWEET_PARSER_PARSERITEM_HPP_INCLUDED

#include <string>
#include <set>

namespace sweet
{

namespace lalr
{

class LalrSymbol;
class LalrProduction;

/**
// An item that defines the positions in the grammar that a state 
// represents.
*/
class LalrItem
{
    LalrProduction* production_; ///< The production that this item is for.
    int position_; ///< The position of the dot in this item.
    mutable std::set<const LalrSymbol*> lookahead_symbols_; ///< The lookahead Symbols for this item.

    public:
        LalrItem();
        LalrItem( LalrProduction* production, int position );

        LalrProduction* get_production() const;
        int get_position() const;
        bool is_dot_at_beginning() const;
        bool is_dot_at_end() const;
        bool is_next_node( const LalrSymbol& symbol ) const;
        void describe( std::string* description ) const;

        int add_lookahead_symbols( const std::set<const LalrSymbol*>& lookahead_symbols ) const;
        const std::set<const LalrSymbol*>& get_lookahead_symbols() const;

        bool operator<( const LalrItem& item ) const;
};

}

}

#endif
