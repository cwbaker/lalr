#ifndef LALR_REGEXITEM_HPP_INCLUDED
#define LALR_REGEXITEM_HPP_INCLUDED

#include "RegexNode.hpp"
#include <string>
#include <set>

namespace lalr
{

class RegexAction;

/**
// An item that defines the positions in a regular expression that a state 
// represents.
*/
class RegexItem
{
    std::set<RegexNode*, RegexNodeLess> next_nodes_; ///< The nodes that appear after the dot in this item.

public:
    RegexItem(): next_nodes_(){}
/**
// Constructor.
//
// @param next_nodes
//  The nodes that appear after the dot in this item.
*/
    RegexItem( const std::set<RegexNode*, RegexNodeLess>& next_nodes ): next_nodes_( next_nodes ){}
/**
// Get the nodes that appear after the dot in this item.
//
// @return
//  The next nodes.
*/
    const std::set<RegexNode*, RegexNodeLess>& next_nodes() const {return next_nodes_;}
    std::set<RegexNode*, RegexNodeLess> next_nodes( int begin, int end ) const;
    const RegexAction* find_action_by_interval( int begin, int end ) const;
    bool operator<( const RegexItem& item ) const;
};

}

#endif
