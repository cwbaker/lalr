//
// LalrAction.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "LalrAction.hpp"
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
LalrAction::LalrAction( int index, const std::string& identifier )
: index_( index ),
  identifier_( identifier )
{
    SWEET_ASSERT( index_ >= 0 );
    SWEET_ASSERT( !identifier_.empty() );
}

int LalrAction::index() const
{
    return index_;
}

const std::string& LalrAction::identifier() const
{
    return identifier_;
}
