//
// LexerAction.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#define __STDC_WANT_LIB_EXT2__ 1 // To get `strdup()`
#include "LexerAction.hpp"
#include "assert.hpp"
#include <string.h>

using namespace sweet::lalr;

LexerAction::LexerAction()
: index( INVALID_INDEX ),
  identifier( nullptr )
{
}

LexerAction::~LexerAction()
{
    if ( identifier )
    {
        free( (void*) identifier );
        identifier = nullptr;
    }
}

void LexerAction::reset( int iindex, const char* iidentifier )
{
    SWEET_ASSERT( !identifier );
    SWEET_ASSERT( iindex >= 0 );
    SWEET_ASSERT( iidentifier );
    index = iindex;
    identifier = strdup( iidentifier );
}
