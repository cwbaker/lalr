//
// ParserAction.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "ParserAction.hpp"

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
ParserAction::ParserAction( int iindex, const std::string& iidentifier )
: index( iindex ),
  identifier( iidentifier )
{
}
