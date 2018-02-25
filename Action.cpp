//
// Action.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "Action.hpp"
#include "assert.hpp"

using namespace sweet::lalr;

/**
// Constructor.
//
// @param index
//  The index of this action (assumed >= 0).
//
// @param identifier
//  The identifier of this action (assumed not empty).
*/
Action::Action( int index, const std::string& identifier )
: index_( index ),
  identifier_( identifier )
{
    SWEET_ASSERT( index_ >= 0 );
    SWEET_ASSERT( !identifier_.empty() );
}

int Action::index() const
{
    return index_;
}

const std::string& Action::identifier() const
{
    return identifier_;
}
