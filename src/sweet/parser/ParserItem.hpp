//
// ParserItem.hpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//    

#ifndef SWEET_PARSER_PARSERITEM_HPP_INCLUDED
#define SWEET_PARSER_PARSERITEM_HPP_INCLUDED

#include "declspec.hpp"
#include <string>
#include <set>

namespace sweet
{

namespace parser
{

class ParserSymbol;
class ParserProduction;

/**
// An item that defines the positions in the grammar that a state 
// represents.
*/
class SWEET_PARSER_DECLSPEC ParserItem
{
    std::shared_ptr<ParserProduction> production_; ///< The production that this item is for.
    int position_; ///< The position of the dot in this item.
    mutable std::set<const ParserSymbol*> lookahead_symbols_; ///< The lookahead Symbols for this item.

    public:
        ParserItem();
        ParserItem( std::shared_ptr<ParserProduction> production, int position );

        std::shared_ptr<ParserProduction> get_production() const;
        int get_position() const;
        bool is_dot_at_beginning() const;
        bool is_dot_at_end() const;
        bool is_next_node( const ParserSymbol& symbol ) const;
        void describe( std::string* description ) const;

        int add_lookahead_symbols( const std::set<const ParserSymbol*>& lookahead_symbols ) const;
        const std::set<const ParserSymbol*>& get_lookahead_symbols() const;

        bool operator<( const ParserItem& item ) const;
};

}

}

#endif
