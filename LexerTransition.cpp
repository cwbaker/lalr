//
// LexerTransition.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "LexerTransition.hpp"

using namespace sweet::lalr;

bool LexerTransition::is_on_character( int character ) const
{
    return character >= begin && character < end;
}
