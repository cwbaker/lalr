//
// ErrorPolicy.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "ErrorPolicy.hpp"
#include <stdio.h>

using namespace lalr;

/**
// Destructor.
*/
ErrorPolicy::~ErrorPolicy()
{
}

/**
// An %error has occured in the *lalr* library.
//
// @param line
//  The line number that the %error occured on.
//
// @param column
//  The column number that the %error occured on.
//
// @param error
//  The error code.
//
// @param format
//  A printf-style format string that describes the error.
//
// @param ...
//  Arguments as described by *format*.
*/
void ErrorPolicy::lalr_error( int line, int /*column*/, int /*error*/, const char* format, va_list args )
{
    fprintf( stderr, "lalr (%d): ERROR: ", line );
    vfprintf( stderr, format, args );
    fprintf( stderr, "\n" );
}

/**
// Debug output has been sent from the *lalr* library.
//
// @param format
//  The printf-style format string that describes the text to print.
//
// @param args
//  Arguments as described by \e format.
*/
void ErrorPolicy::lalr_vprintf( const char* format, va_list args )
{
    printf( "lalr: " );
    vprintf( format, args );
}
