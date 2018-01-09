//
// LexerItem.cpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "LexerItem.hpp"
#include "RegexNode.hpp"
#include <sweet/assert/assert.hpp>
#include <string>
#include <stdio.h>

using namespace sweet;
using namespace sweet::lexer;

#if defined(BUILD_PLATFORM_MSVC)
#define snprintf _snprintf
#endif

/**
// Constructor.
*/
LexerItem::LexerItem()
: next_nodes_()
{
}

/**
// Constructor.
//
// @param next_nodes
//  The nodes that appear after the dot in this item.
*/
LexerItem::LexerItem( const std::set<RegexNode*, RegexNodeLess>& next_nodes )
: next_nodes_( next_nodes )
{
}

/**
// Get the nodes that appear after the dot in this item.
//
// @return
//  The next nodes.
*/
const std::set<RegexNode*, RegexNodeLess>& LexerItem::get_next_nodes() const
{
    return next_nodes_;
}

/**
// Calculate the nodes that can be visited after [\e begin, \e end) is 
// transitioned on from this item.
//
// These nodes then become the core of a new item for the state that 
// the lexical analyzer is in after accepted [\e begin, \e end) from the
// state that this item is part of.
//
// Action nodes are special in that they are only able to be transitioned on
// if no other nodes can be visited after [\e begin, \e end) is transitioned
// on.  Nodes are ordered by type and then index and the action type 
// (LEXER_NODE_ACTION) is the highest so that action nodes sort to the end
// of the nodes in an item.
//
// @param begin
//  The begin character of the interval to calculate the next nodes for.
//
// @param end
//  The end character of the interval to calculate the next nodes for.
//
// @return
//  The next nodes after [\e begin, \e end) is transitioned on from this item.
*/
std::set<RegexNode*, RegexNodeLess> LexerItem::next_nodes( int begin, int end ) const
{
    std::set<RegexNode*, RegexNodeLess> next_nodes;
    
//
// Add transitions from non-action nodes to the nodes that are visitable
// next.
//    
    std::set<RegexNode*, RegexNodeLess>::const_iterator i = next_nodes_.begin(); 
    while ( i != next_nodes_.end() && (*i)->get_type() != LEXER_NODE_ACTION )
    {   
        const RegexNode* node = *i;
        SWEET_ASSERT( node );            
        SWEET_ASSERT( node->get_type() < LEXER_NODE_ACTION );

        if ( node->is_match(begin, end) )
        {
            next_nodes.insert( node->get_follow_positions().begin(), node->get_follow_positions().end() );
        }
        
        ++i;
    }

//
// If there were no transitions from any other nodes then add transitions
// from action nodes to the nodes that are visitable next.
//    
    if ( next_nodes.empty() )
    {
        while ( i != next_nodes_.end() )
        {
            const RegexNode* node = *i;
            SWEET_ASSERT( node );            
            SWEET_ASSERT( node->get_type() == LEXER_NODE_ACTION );

            if ( node->is_match(begin, end) )
            {
                next_nodes.insert( node->get_follow_positions().begin(), node->get_follow_positions().end() );
            }
            
            ++i;
        }
    }
    
    return next_nodes;
}

/**
// Find the action that should be taken after [\e begin, \e end) is 
// transitioned on from this item.
//
// @param begin
//  The begin character of the interval to calculate the next nodes for. 
//
// @param end
//  The end character of the interval to calculate the next nodes for.
//
// @return
//  The action that should be taken after [\e begin, \e end) is 
//  transitioned on or null if there is no such action.
*/
const LexerAction* LexerItem::find_action_by_interval( int begin, int end ) const
{
    std::set<RegexNode*, RegexNodeLess>::const_iterator i = next_nodes_.begin();
    while ( i != next_nodes_.end() && !(*i)->is_action() && !(*i)->is_match(begin, end) )
    {
        ++i;
    }
    
    if ( i != next_nodes_.end() && (*i)->is_action() )
    {
        while ( i != next_nodes_.end() && !(*i)->is_match(begin, end) )
        {
            SWEET_ASSERT( (*i)->is_action() );
            ++i;
        }
    }
    
    return i != next_nodes_.end() ? (*i)->get_action() : NULL;
}

/**
// Describe this item
//
// @param description
//  A variable to append the description to (assumed not null).
*/
void LexerItem::describe( std::string* description ) const
{
    SWEET_ASSERT( description );
    
    for ( std::set<RegexNode*, RegexNodeLess>::const_iterator i = next_nodes_.begin(); i != next_nodes_.end(); ++i )
    {
        const RegexNode* node = *i;
        SWEET_ASSERT( node );
        char buffer [32];
        snprintf( buffer, sizeof(buffer), "%d ", node->get_index() );
        buffer [sizeof(buffer) - 1] = '\0';
        description->append( buffer );
    }
}

/**
// Less than operator.
//
// @return
//  True if the next nodes of this item are less than the next nodes of 
//  \e item.
*/
bool LexerItem::operator<( const LexerItem& item ) const
{
    return next_nodes_ < item.next_nodes_;
}
