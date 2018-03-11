//
// RegexAction.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "RegexAction.hpp"

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
RegexAction::RegexAction( int index, const std::string& identifier )
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
int RegexAction::index() const
{
    return index_;
}

/**
// Get the identifier of this action.
//
// @return
//  The identifier of this action.
*/
const std::string& RegexAction::identifier() const
{
    return identifier_;
}
