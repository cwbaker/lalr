//
// RegexNode.cpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "RegexNode.hpp"
#include "LexerAction.hpp"
#include <algorithm>
#include <stdio.h>
#include <limits.h>

using std::find;
using std::vector;
using std::set;
using namespace sweet;
using namespace sweet::lexer;

const int sweet::lexer::BEGIN_CHARACTER = 0;
const int sweet::lexer::END_CHARACTER = INT_MAX;
const int sweet::lexer::INVALID_BEGIN_CHARACTER = -2;
const int sweet::lexer::INVALID_END_CHARACTER = -1;

/**
// Constructor.
//
// @param index
//  The index of this node.
//
// @param type
//  The type of this node.
*/
RegexNode::RegexNode( int index, RegexNodeType type )
: index_( index ),
  type_( type ),
  begin_character_( INVALID_BEGIN_CHARACTER ),
  end_character_( INVALID_END_CHARACTER ),
  token_( NULL ),
  action_( NULL ),
  nodes_(),
  nullable_( false ),
  first_positions_(),
  last_positions_(),
  follow_positions_()
{
    SWEET_ASSERT( type_ != LEXER_NODE_SYMBOL );
}

/**
// Constructor.
//
// @param index
//  The index of this node.
//
// @param begin_character
//  The first character in the interval for this node (assumed less than
//  \e end_character).
//
// @param end_character
//  One past the last character in the interval for this node.
*/
RegexNode::RegexNode( int index, int begin_character, int end_character )
: index_( index ),
  type_( LEXER_NODE_SYMBOL ),
  begin_character_( begin_character ),
  end_character_( end_character ),
  token_( NULL ),
  action_( NULL ),
  nodes_(),
  nullable_( false ),
  first_positions_(),
  last_positions_(),
  follow_positions_()
{
    SWEET_ASSERT( begin_character_ < end_character_ );
}

/**
// Constructor.
//
// @param index
//  The index of this node.
//
// @param begin_character
//  The first character in the interval for this node (assumed less than
//  \e end_character).
//
// @param end_character
//  One past the last character in the interval for this node.
//
// @param token
//  The LexerToken recognized at this node (assumed not null).
*/
RegexNode::RegexNode( int index, int begin_character, int end_character, const LexerToken* token )
: index_( index ),
  type_( LEXER_NODE_SYMBOL ),
  begin_character_( begin_character ),
  end_character_( end_character ),
  token_( token ),
  action_( NULL ),
  nodes_(),
  nullable_( false ),
  first_positions_(),
  last_positions_(),
  follow_positions_()
{
    SWEET_ASSERT( begin_character_ < end_character_ );
    SWEET_ASSERT( token_ );
}

/**
// Constructor.
//
// @param index
//  The index of this node.
//
// @param action
//  The action taken at this node (assumed not null).
*/
RegexNode::RegexNode( int index, const LexerAction* action )
: index_( index ),
  type_( LEXER_NODE_ACTION ),
  begin_character_( BEGIN_CHARACTER ),
  end_character_( END_CHARACTER ),
  token_( NULL ),
  action_( action ),
  nodes_(),
  nullable_( false ),
  first_positions_(),
  last_positions_(),
  follow_positions_()
{
    SWEET_ASSERT( action_ );
}

/**
// Get the index of this node.
*/
int RegexNode::get_index() const
{
    return index_;
}

/**
// Get the type of this node.
*/
RegexNodeType RegexNode::get_type() const
{
    return type_;
}

/**
// Get the lexeme of this node.
*/
const char* RegexNode::get_lexeme() const
{
    const char* lexeme = "??";

    switch ( type_ )
    {

        case LEXER_NODE_NULL:
            lexeme = "null";
            break;

        case LEXER_NODE_CAT:
            lexeme = "cat";
            break;

        case LEXER_NODE_OR:
            lexeme = "or";
            break;

        case LEXER_NODE_STAR:
            lexeme = "*";
            break;

        case LEXER_NODE_PLUS:
            lexeme = "+";
            break;

        case LEXER_NODE_OPTIONAL:
            lexeme = "?";
            break;

        case LEXER_NODE_SYMBOL:
            lexeme = ".";
            break;

        case LEXER_NODE_ACTION:
            lexeme = "action";
            break;

        default:
            SWEET_ASSERT( false );
            break;
    }

    return lexeme;
}

/**
// Get the first character in the interval of this node.
//
// @return
//  The begin character.
*/
int RegexNode::get_begin_character() const
{
    return begin_character_;
}

/**
// Get the character that is one past the last character in the interval of
// this node.
//
// @return
//  The end character.
*/
int RegexNode::get_end_character() const
{
    return end_character_;
}

/**
// Get the LexerToken that is recognized at this node.
//
// @return
//  The token or null if this node doesn't recognize a token.
*/
const LexerToken* RegexNode::get_token() const
{
    return token_;
}

/**
// Get the action at this node.
//
// @return
//  The action.
*/
const LexerAction* RegexNode::get_action() const
{
    return action_;
}

/**
// Does this node match [\e begin, \e end)?
//
// The range [\e begin, \e end) is assumed to fall entirely inside or entirely
// outside the range of this node.  If [\e begin, \e end) spans the boundary 
// of the range of any nodes then the transition generated from it will be 
// invalid - it would generate a transition to that node for some characters 
// that weren't in the range of that node.
//
// @param begin
//  The character to test if this node matches.
//
// @return
//  True if this node matches [\e begin, \e end) otherwise false.
*/
bool RegexNode::is_match( int begin, int end ) const
{
    SWEET_ASSERT( begin < end );
    SWEET_ASSERT( end <= begin_character_ || begin >= end_character_ || begin >= begin_character_ && end <= end_character_ );
    return begin >= begin_character_ && end <= end_character_;
}

/**
// Is this node the end character?
//
// @return
//  True if this node matches the end character otherwise false.
*/
bool RegexNode::is_end() const
{
    return type_ == LEXER_NODE_SYMBOL && begin_character_ == INVALID_BEGIN_CHARACTER && end_character_ == INVALID_END_CHARACTER;
}

/**
// Is this node an action node?
//
// @return
//  True if this node is an action otherwise false.
*/
bool RegexNode::is_action() const
{
    return type_ == LEXER_NODE_ACTION;
}

/**
// Add \e node as a child of this node.
//
// @param node
//  The node to add to this node (assumed not null and not already a child
//  of this node).
*/
void RegexNode::add_node( const ptr<RegexNode>& node )
{
    SWEET_ASSERT( node.get() );
    SWEET_ASSERT( find(nodes_.begin(), nodes_.end(), node) == nodes_.end() );
    nodes_.push_back( node );
}

/**
// Get the \e nth child of this node.
//
// @param n
//  The index in the children of this node of the node to get (assumed t be
//  valid).
//
// @return
//  The \e nth child node.
*/
RegexNode* RegexNode::get_node( int n ) const
{
    SWEET_ASSERT( n >= 0 && n < (int) nodes_.size() );
    return nodes_[n].get();
}

/**
// Get the nodes that are children of this node.
//
// @return
//  The child nodes.
*/
const std::vector<ptr<RegexNode> >& RegexNode::get_nodes() const
{
    return nodes_;
}

/**
// Is this node nullable?
//
// @return
//  True if this node is nullable otherwise false.
*/
bool RegexNode::is_nullable() const
{
    return nullable_;
}

/**
// Get the first positions at this node.
//
// @return
//  The first positions.
*/
const std::set<RegexNode*, RegexNodeLess>& RegexNode::get_first_positions() const
{
    return first_positions_;
}

/**
// Get the last positions at this node.
//
// @return
//  The last positions.
*/
const std::set<RegexNode*, RegexNodeLess>& RegexNode::get_last_positions() const
{
    return last_positions_;
}

/**
// Get the follow positions at this node.
//
// @return
//  The follow positions.
*/
const std::set<RegexNode*, RegexNodeLess>& RegexNode::get_follow_positions() const
{
    return follow_positions_;
}

/**
// Get the nodes that can appear after this node.
//
// This returns either the first positions or the follow positions of this
// node based on its type.  For a node that matches one or more characters 
// (LEXER_NODE_SYMBOL) it returns the follow positions for all other types
// it returns the first positions.
//
// @return
//  The first or follow positions of this node depending on its type.
*/
const std::set<RegexNode*, RegexNodeLess>& RegexNode::get_next_positions() const
{
    return type_ == LEXER_NODE_SYMBOL ? follow_positions_ : first_positions_;
}

/**
// Recursively calculate nullable for this node.
*/
void RegexNode::calculate_nullable()
{
    for ( std::vector<ptr<RegexNode> >::const_iterator i = nodes_.begin(); i != nodes_.end(); ++i )
    {
        RegexNode* node = i->get();
        SWEET_ASSERT( node );
        node->calculate_nullable();
    }

    switch ( type_ )
    {
        case LEXER_NODE_NULL:
            nullable_ = true;
            break;
 
        case LEXER_NODE_CAT:
            SWEET_ASSERT( nodes_.size() == 2 );
            nullable_ = nodes_[0]->is_nullable() && nodes_[1]->is_nullable();
            break;

        case LEXER_NODE_OR:
            SWEET_ASSERT( nodes_.size() == 2 );
            nullable_ = nodes_[0]->is_nullable() || nodes_[1]->is_nullable();
            break;

        case LEXER_NODE_STAR:
        case LEXER_NODE_OPTIONAL:
            nullable_ = true;
            break;

        case LEXER_NODE_PLUS:
            SWEET_ASSERT( nodes_.size() == 1 );
            nullable_ = nodes_[0]->is_nullable();
            break;

        case LEXER_NODE_SYMBOL:
            nullable_ = begin_character_ == INVALID_BEGIN_CHARACTER && end_character_ == INVALID_END_CHARACTER;
            break;

        case LEXER_NODE_ACTION:
            nullable_ = false;
            break;

        default:
            SWEET_ASSERT( false );
            nullable_ = false;
            break;
    }
}

/**
// Recursively calculate the first positions at this node.
*/
void RegexNode::calculate_first_positions()
{
    for ( std::vector<ptr<RegexNode> >::const_iterator i = nodes_.begin(); i != nodes_.end(); ++i )
    {
        RegexNode* node = i->get();
        SWEET_ASSERT( node );
        node->calculate_first_positions();
    }

    switch ( type_ )
    {
        case LEXER_NODE_NULL:
            break;
          
        case LEXER_NODE_CAT:
        {
            SWEET_ASSERT( nodes_.size() == 2 );
            first_positions_.insert( nodes_[0]->first_positions_.begin(), nodes_[0]->first_positions_.end() );
            if ( nodes_[0]->is_nullable() )
            {
                first_positions_.insert( nodes_[1]->first_positions_.begin(), nodes_[1]->first_positions_.end() );
            }
            break;
        }

        case LEXER_NODE_OR:
            SWEET_ASSERT( nodes_.size() == 2 );
            first_positions_.insert( nodes_[0]->first_positions_.begin(), nodes_[0]->first_positions_.end() );
            first_positions_.insert( nodes_[1]->first_positions_.begin(), nodes_[1]->first_positions_.end() );
            break;

        case LEXER_NODE_STAR:
        case LEXER_NODE_PLUS:
        case LEXER_NODE_OPTIONAL:
            SWEET_ASSERT( nodes_.size() == 1 );
            first_positions_.insert( nodes_[0]->first_positions_.begin(), nodes_[0]->first_positions_.end() );
            break;

        case LEXER_NODE_SYMBOL:
        case LEXER_NODE_ACTION:
            first_positions_.insert( this );
            break;

        default:
            SWEET_ASSERT( false );
            break;
    }
}

/**
// Recursively calculate the last positions at this node.
*/
void RegexNode::calculate_last_positions()
{
    for ( std::vector<ptr<RegexNode> >::const_iterator i = nodes_.begin(); i != nodes_.end(); ++i )
    {
        RegexNode* node = i->get();
        SWEET_ASSERT( node );
        node->calculate_last_positions();
    }

    switch ( type_ )
    {
        case LEXER_NODE_NULL: 
            break;
             
        case LEXER_NODE_CAT:
        {
            SWEET_ASSERT( nodes_.size() == 2 );
            last_positions_.insert( nodes_[1]->last_positions_.begin(), nodes_[1]->last_positions_.end() );
            if ( nodes_[1]->is_nullable() )
            {
                last_positions_.insert( nodes_[0]->last_positions_.begin(), nodes_[0]->last_positions_.end() );
            }
            break;
        }

        case LEXER_NODE_OR:
            SWEET_ASSERT( nodes_.size() == 2 );
            last_positions_.insert( nodes_[0]->last_positions_.begin(), nodes_[0]->last_positions_.end() );
            last_positions_.insert( nodes_[1]->last_positions_.begin(), nodes_[1]->last_positions_.end() );
            break;

        case LEXER_NODE_STAR:
        case LEXER_NODE_PLUS:
        case LEXER_NODE_OPTIONAL:
            SWEET_ASSERT( nodes_.size() == 1 );
            last_positions_.insert( nodes_[0]->last_positions_.begin(), nodes_[0]->last_positions_.end() );
            break;

        case LEXER_NODE_SYMBOL:
        case LEXER_NODE_ACTION:
            last_positions_.insert( this );
            break;

        default:
            SWEET_ASSERT( false );
            break;
    }
}

/**
// Recursively calculate the follow positions at this node.
*/
void RegexNode::calculate_follow_positions()
{
    for ( std::vector<ptr<RegexNode> >::const_iterator i = nodes_.begin(); i != nodes_.end(); ++i )
    {
        RegexNode* node = i->get();
        SWEET_ASSERT( node );
        node->calculate_follow_positions();
    }

    switch ( type_ )
    {
        case LEXER_NODE_NULL:  
            break;
            
        case LEXER_NODE_CAT:
        {
            SWEET_ASSERT( nodes_.size() == 2 );

            const std::set<RegexNode*, RegexNodeLess>& last_positions = nodes_[0]->last_positions_;
            const std::set<RegexNode*, RegexNodeLess>& first_positions = nodes_[1]->first_positions_;

            for ( std::set<RegexNode*, RegexNodeLess>::const_iterator i = last_positions.begin(); i != last_positions.end(); ++i )
            {
                RegexNode* node = *i;
                SWEET_ASSERT( node );
                if ( node->get_type() == LEXER_NODE_SYMBOL || node->get_type() == LEXER_NODE_ACTION )
                {
                    node->follow_positions_.insert( first_positions.begin(), first_positions.end() );
                }
            }

            break;
        }

        case LEXER_NODE_STAR:
        case LEXER_NODE_PLUS:
        {
            SWEET_ASSERT( nodes_.size() == 1 );
            const std::set<RegexNode*, RegexNodeLess>& last_positions = last_positions_;
            const std::set<RegexNode*, RegexNodeLess>& first_positions = first_positions_;

            for ( std::set<RegexNode*, RegexNodeLess>::const_iterator i = last_positions.begin(); i != last_positions.end(); ++i )
            {
                RegexNode* node = *i;
                SWEET_ASSERT( node );
                if ( node->get_type() == LEXER_NODE_SYMBOL || node->get_type() == LEXER_NODE_ACTION )
                {
                    node->follow_positions_.insert( first_positions.begin(), first_positions.end() );
                }
            }

            break;
        }

        case LEXER_NODE_OR:
        case LEXER_NODE_OPTIONAL:
        case LEXER_NODE_SYMBOL:
        case LEXER_NODE_ACTION:
            break;

        default:
            SWEET_ASSERT( false );
            break;
    }
}

/**
// Compare this RegexNode with \e node by type and then index.
//
// The type is part of the comparison so that action nodes (LEXER_NODE_ACTION)
// sort at the end of the set of nodes for an item.  The value of 
// LEXER_NODE_ACTION is assumed to be the highest value.
//
// @param node
//  The node to compare this node with.
//
// @return
//  True if the type of this RegexNode is less than that of \e node or if the 
//  type of this RegexNode is equal to that of \e node then true if the index 
//  of this RegexNode is less than that of \e node otherwise false.
*/
bool RegexNode::operator<( const RegexNode& node ) const
{
    return type_ < node.type_
        || (type_ == node.type_ && index_ < node.index_)
         ;
}

/**
// Recursively print this RegexNode as part of a LexerItem.
//
// @param dot_nodes
//  The Nodes that appear after the dot in the LexerItem (if \e dot_nodes 
//  contains this RegexNode then a leading dot is printed).
*/
void RegexNode::print( const std::set<RegexNode*>& dot_nodes ) const
{
    if ( dot_nodes.find(const_cast<RegexNode*>(this)) != dot_nodes.end() )
    {
        printf( ". " );
    }

    if ( type_ == LEXER_NODE_SYMBOL && begin_character_ != INVALID_BEGIN_CHARACTER && end_character_ != INVALID_END_CHARACTER )
    {
        for ( int character = begin_character_; character < end_character_; ++character )
        {
            printf( character >= ' ' ? "'%c' " : "0x%02x ", character );
        }
    }
    else if ( type_ == LEXER_NODE_ACTION )
    {
        printf( "%s", action_ != NULL ? action_->get_identifier().c_str() : "null" );
    }    

    for ( std::vector<ptr<RegexNode> >::const_iterator i = nodes_.begin(); i != nodes_.end(); ++i )
    {
        const ptr<RegexNode>& node = *i;        
        node->print( dot_nodes );
    }
}
