//
// assert.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "assert.hpp"
#include <sweet/build.hpp>
#include <stdlib.h>
#include <stdio.h>

#if defined(BUILD_OS_WINDOWS)
#include <windows.h>
#elif defined(BUILD_OS_ANDROID)
#include <android/log.h>
#include <signal.h>
#endif

/**
// Break in the debugger.
*/
void sweet_break()
{
#if defined(BUILD_OS_WINDOWS) && defined(_MSC_VER)
    DebugBreak();
#elif defined(BUILD_OS_MACOSX)
    asm( "int $3" );
#elif defined(BUILD_OS_ANDROID)
    raise( SIGABRT );
#endif
}

/**
// If \e expression isn't true then print \e file, \e line, and 
// \e description to the debug console and stderr.
//
// @param expression
//  The expression to test in the assertion.
//
// @param file
//  The source file that the assertion is in.
//
// @param line
//  The line number that the assertion is on.
*/
void sweet_assert( int expression, const char* description, const char* file, int line )
{
#if defined(BUILD_OS_WINDOWS)
    int error = GetLastError();
    if ( !expression )
    {
        char message [1024];
        _snprintf( message, sizeof(message), "%s(%i) : %s\n", file, line, description );
        message[sizeof(message) - 1] = 0;
        fputs( message, stderr );
    }
    SetLastError( error );
#elif defined(BUILD_OS_ANDROID)
    if ( !expression )
    {
        char message [1024];
        snprintf( message, sizeof(message), "%s(%i) : %s\n", file, line, description );
        message[sizeof(message) - 1] = 0;
        __android_log_print( ANDROID_LOG_ERROR, "SWEET_ASSERT", message );
    }    
#else
    if ( !expression )
    {
        char message [1024];
        snprintf( message, sizeof(message), "%s(%i) : %s\n", file, line, description );
        message[sizeof(message) - 1] = 0;
        fputs( message, stderr );
    }
#endif
}

/**
// Assert with embedded break (for use with Lua).
//
// @param expression
//  The expression to test in the assertion.
//
// @param file
//  The source file that the assertion is in.
//
// @param line
//  The line number that the assertion is on.
*/
void sweet_assert_with_break( int expression, const char* description, const char* file, int line )
{
    sweet_assert( expression, description, file, line );
    if ( !expression )
    {
        SWEET_BREAK();
    }
}
