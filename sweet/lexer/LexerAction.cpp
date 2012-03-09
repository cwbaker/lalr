//
// LexerAction.cpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "LexerAction.hpp"

using namespace sweet::lexer;

/**
// Constructor.
//
// @param index
//  The index of this action.
//
// @param identifier
//  The identifier of this action.
*/
LexerAction::LexerAction( int index, const std::string& identifier )
: index_( index ),
  identifier_( identifier )
{
}
  
/**
// Get the index of this action.
//
// @return
//  The index.
*/
int LexerAction::get_index() const
{
    return index_;
}

/**
// Get the identifier of this action.
//
// @return
//  The identifier of this action.
*/
const std::string& LexerAction::get_identifier() const
{
    return identifier_;
}
