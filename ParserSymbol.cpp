//
// ParserSymbol.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#define __STDC_WANT_LIB_EXT2__ 1 // To get `strdup()`
#include "ParserSymbol.hpp"
#include "assert.hpp"
#include <string.h>
#include <stdlib.h>

using namespace sweet::lalr;

ParserSymbol::ParserSymbol()
: index( 0 ),
  identifier( nullptr ),
  lexeme( nullptr )
{
}

ParserSymbol::~ParserSymbol()
{
    if ( identifier )
    {
        free( (void*) identifier );
        identifier = nullptr;
    }

    if ( lexeme )
    {
        free( (void*) lexeme );
        lexeme = nullptr;
    }
}

void ParserSymbol::reset( int iindex, const char* iidentifier, const char* llexeme )
{
    SWEET_ASSERT( !identifier );
    SWEET_ASSERT( !lexeme );
    SWEET_ASSERT( iindex >= 0 );
    SWEET_ASSERT( iidentifier );
    SWEET_ASSERT( llexeme );
    index = iindex;
    identifier = strdup( iidentifier );
    lexeme = strdup( llexeme );
}
