//
// LexerItem.hpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LEXER_LEXERITEM_HPP_INCLUDED
#define SWEET_LEXER_LEXERITEM_HPP_INCLUDED

#include "declspec.hpp"
#include "RegexNodeLess.hpp"
#include <sweet/pointer/ptr.hpp>
#include <set>

namespace sweet
{

namespace lexer
{

class RegexNode;
class LexerAction;

/**
// An item that defines the positions in a regular expression that a state 
// represents.
*/
class SWEET_LEXER_DECLSPEC LexerItem
{
    std::set<RegexNode*, RegexNodeLess> next_nodes_; ///< The nodes that appear after the dot in this item.

    public:
        LexerItem();
        LexerItem( const std::set<RegexNode*, RegexNodeLess>& next_nodes );
        
        const std::set<RegexNode*, RegexNodeLess>& get_next_nodes() const;
        std::set<RegexNode*, RegexNodeLess> next_nodes( int begin, int end ) const;
        const LexerAction* find_action_by_interval( int begin, int end ) const;
        void describe( std::string* description ) const;
        bool operator<( const LexerItem& item ) const;
};

}

}

#endif