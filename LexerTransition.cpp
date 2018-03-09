//
// LexerTransition.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "LexerTransition.hpp"

using namespace sweet::lalr;

LexerTransition::LexerTransition()
: begin( 0 ),
  end( 0 ),
  state( nullptr ),
  action( nullptr )
{
}

bool LexerTransition::is_on_character( int character ) const
{
    return character >= begin && character < end;
}
