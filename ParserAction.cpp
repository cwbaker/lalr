//
// ParserAction.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#define __STDC_WANT_LIB_EXT2__ 1 // To get `strdup()`
#include "ParserAction.hpp"
#include "assert.hpp"
#include <string.h>

using namespace sweet::lalr;

ParserAction::ParserAction()
: index( 0 ),
  identifier( nullptr )
{
}

ParserAction::~ParserAction()
{
    if ( identifier )
    {
        free( (void*) identifier );
        identifier = nullptr;
    }
}

void ParserAction::reset( int iindex, const char* iidentifier )
{
    SWEET_ASSERT( !identifier );
    SWEET_ASSERT( iindex >= 0 );
    SWEET_ASSERT( iidentifier );
    index = iindex;
    identifier = strdup( iidentifier );
}
