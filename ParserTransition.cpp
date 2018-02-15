//
// ParserTransition.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "ParserTransition.hpp"

using namespace sweet::lalr;

ParserTransition::ParserTransition()
: symbol( nullptr ),
  state( nullptr ),
  reduced_symbol( nullptr ),
  reduced_length( 0 ),
  precedence( 0 ),
  action( 0 ),
  type( TRANSITION_SHIFT ),
  index( 0 )
{
}
