//
// ParserAction.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#define __STDC_WANT_LIB_EXT2__ 1
#include "ParserAction.hpp"
#include <string.h>

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
ParserAction::ParserAction( int iindex, const char* iidentifier )
: index( iindex ),
  identifier( nullptr )
{
    identifier = strdup( iidentifier );
}

ParserAction::~ParserAction()
{
    if ( identifier )
    {
        free( (void*) identifier );
        identifier = nullptr;
    }
}
