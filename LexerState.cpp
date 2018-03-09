//
// LexerState.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "LexerState.hpp"
#include "LexerTransition.hpp"

using namespace sweet::lalr;

LexerState::LexerState()
: index( 0 ),
  length( 0 ),
  transitions( nullptr ),
  symbol( nullptr )
{
}

const LexerTransition* LexerState::find_transition_by_character( int character ) const
{
    const LexerTransition* transition = transitions;
    const LexerTransition* transitions_end = transitions + length;
    while ( transition != transitions_end && !transition->is_on_character(character) )
    {
        ++transition;
    }
    return transition != transitions_end ? transition : nullptr;
}
