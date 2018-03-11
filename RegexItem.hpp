#ifndef SWEET_LALR_REGEXITEM_HPP_INCLUDED
#define SWEET_LALR_REGEXITEM_HPP_INCLUDED

#include "RegexNodeLess.hpp"
#include <string>
#include <set>

namespace sweet
{

namespace lalr
{

class RegexNode;
class RegexAction;

/**
// An item that defines the positions in a regular expression that a state 
// represents.
*/
class RegexItem
{
    std::set<RegexNode*, RegexNodeLess> next_nodes_; ///< The nodes that appear after the dot in this item.

public:
    RegexItem();
    RegexItem( const std::set<RegexNode*, RegexNodeLess>& next_nodes );
    const std::set<RegexNode*, RegexNodeLess>& next_nodes() const;
    std::set<RegexNode*, RegexNodeLess> next_nodes( int begin, int end ) const;
    const RegexAction* find_action_by_interval( int begin, int end ) const;
    bool operator<( const RegexItem& item ) const;
};

}

}

#endif
