//
// LexerErrorPolicy.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "LexerErrorPolicy.hpp"

using namespace sweet;
using namespace sweet::lalr;

/**
// Destructor.
*/
LexerErrorPolicy::~LexerErrorPolicy()
{
}

void LexerErrorPolicy::lexer_error( int /*line*/, int /*error*/, const char* /*format*/, va_list /*args*/ )
{
}

void LexerErrorPolicy::lexer_vprintf( const char* /*format*/, va_list /*args*/ )
{
}
