//
// assert.cpp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "assert.hpp"

namespace sweet
{

namespace assert
{

/**
// If \e expression isn't true then print \e file, \e line, and 
// \e description to the debug console and stderr.
//
// @param expression
//  The expression that has failed the assertion.
//
// @param file
//  The source file that the failed assertion is in.
//
// @param line
//  The line number that the failed assertion is on.
*/
void sweet_assert( bool expression, const char* description, const char* file, int line )
{
    int error = ::GetLastError();

    if ( !expression )
    {
        char message [1024];
        _snprintf( message, sizeof(message), "%s(%i) : %s\n", file, line, description );
        message[sizeof(message) - 1] = 0;
        ::OutputDebugStringA( message );
        ::fputs( message, stderr );
    }

    ::SetLastError( error );
}

}

}
