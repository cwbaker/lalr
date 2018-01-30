//
// GrammarAction.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "GrammarAction.hpp"

using namespace sweet::lalr;

GrammarAction::GrammarAction( std::string identifier )
: identifier_( identifier )
{
}

const std::string& GrammarAction::identifier() const
{
    return identifier_;
}
