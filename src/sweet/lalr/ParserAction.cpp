//
// ParserAction.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "ParserAction.hpp"

using namespace sweet::lalr;

/**
// Constructor.
//
// @param index
//  The index of this action.
//
// @param identifier
//  The identifier of this action.
*/
ParserAction::ParserAction( int index, const std::string& identifier )
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
int ParserAction::get_index() const
{
    return index_;
}

/**
// Get the identifier of this action.
//
// @return
//  The identifier.
*/
const std::string& ParserAction::get_identifier() const
{
    return identifier_;
}