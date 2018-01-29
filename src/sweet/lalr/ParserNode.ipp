//
// ParserNode.ipp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_PARSER_PARSERNODE_IPP_INCLUDED
#define SWEET_PARSER_PARSERNODE_IPP_INCLUDED

#include "ParserNode.hpp"
#include "assert.hpp"

namespace sweet
{

namespace lalr
{

/**
// Constructor.
//
// @param state
//  The %ParserState at this node.
//
// @param started_productions
//  The productions that were started at this node.
//
// @param symbol
//  The Symbol at this node.
//
// @param user_data
//  The user data that stores application specific data at this node.
*/
template <class UserData, class Char, class Traits, class Allocator>
ParserNode<UserData, Char, Traits, Allocator>::ParserNode( const ParserState* state, const ParserSymbol* symbol, const UserData& user_data )
: state_( state ),
  symbol_( symbol ),
  lexeme_(),
  user_data_( user_data )
{
    SWEET_ASSERT( state );
}

/**
// Constructor.
//
// @param state
//  The state at this node.
//
// @param started_productions
//  The productions that were started at this node.
//
// @param symbol
//  The symbol at this node.
//
// @param lexeme
//  The lexeme at this node.
*/
template <class UserData, class Char, class Traits, class Allocator>
ParserNode<UserData, Char, Traits, Allocator>::ParserNode( const ParserState* state, const ParserSymbol* symbol, const std::basic_string<Char, Traits, Allocator>& lexeme )
: state_( state ),
  symbol_( symbol ),
  lexeme_( lexeme ),
  user_data_()
{
    SWEET_ASSERT( state );
}

/**
// Get the state at this node.
//
// @return
//  The state.
*/
template <class UserData, class Char, class Traits, class Allocator>
const ParserState* ParserNode<UserData, Char, Traits, Allocator>::get_state() const
{
    return state_;
}

/**
// Get the symbol at this state.
//
// @return
//  The symbol.
*/
template <class UserData, class Char, class Traits, class Allocator>
const ParserSymbol* ParserNode<UserData, Char, Traits, Allocator>::get_symbol() const
{
    return symbol_;
}

/**
// Get the lexeme at this state.
//
// @return
//  The lexeme.
*/
template <class UserData, class Char, class Traits, class Allocator>
const std::basic_string<Char, Traits, Allocator>& ParserNode<UserData, Char, Traits, Allocator>::get_lexeme() const
{
    return lexeme_;
}

/**
// Get the user data at this state.
//
// @return
//  The user data.
*/
template <class UserData, class Char, class Traits, class Allocator>
const UserData& ParserNode<UserData, Char, Traits, Allocator>::get_user_data() const
{
    return user_data_;
}

}

}

#endif
