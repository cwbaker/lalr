//
// ParserState.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "ParserState.hpp"
#include "ParserTransition.hpp"
#include "assert.hpp"

using namespace sweet::lalr;

ParserState::ParserState()
: index( 0 ),
  length( 0 ),
  transitions( nullptr )
{
    (void) index;
}

const ParserTransition* ParserState::find_transition_by_symbol( const ParserSymbol* symbol ) const
{
    const ParserTransition* transition = transitions;
    const ParserTransition* transitions_end = transition + length;
    while ( transition != transitions_end && transition->symbol != symbol )
    {
        ++transition;
    }
    return transition != transitions_end ? transition : nullptr;
}
