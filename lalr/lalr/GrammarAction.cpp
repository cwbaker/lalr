//
// GrammarAction.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "GrammarAction.hpp"
#include "assert.hpp"

using namespace lalr;

/**
// Constructor.
//
// @param index
//  The index of this action (assumed >= 0).
//
// @param identifier
//  The identifier of this action (assumed not empty).
*/
GrammarAction::GrammarAction( int index, const std::string& identifier )
: index_( index ),
  identifier_( identifier )
{
    LALR_ASSERT( index_ >= 0 );
    LALR_ASSERT( !identifier_.empty() );
}

int GrammarAction::index() const
{
    return index_;
}

const std::string& GrammarAction::identifier() const
{
    return identifier_;
}
