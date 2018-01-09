//
// ErrorPolicy.cpp
// Copyright (c) 2001 - 2013 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "ErrorPolicy.hpp"
#include <sweet/assert/assert.hpp>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#if defined(_MSC_VER)
#define vsnprintf _vsnprintf
#endif

using namespace sweet::error;

/**
// Constructor.
*/
ErrorPolicy::ErrorPolicy()
: stack_index_( 0 )
  //errors_( 0 )
{
    memset( errors_, 0, sizeof(errors_) );
}

/**
// Destructor.
*/
ErrorPolicy::~ErrorPolicy()
{
}

/**
// Reset the error count at the current stack level to 0.
*/
void ErrorPolicy::clear()
{
    SWEET_ASSERT( stack_index_ >= 0 && stack_index_ < ERROR_STACK_SIZE - 1 );
    errors_[stack_index_] = 0;
}

/**
// Push an error count onto the stack of error counts.
//
// Assumes that the no more than \e ERROR_STACK_SIZE (32) levels are ever
// pushed.
*/
void ErrorPolicy::push_errors()
{
    SWEET_ASSERT( stack_index_ >= 0 && stack_index_ < ERROR_STACK_SIZE - 1 );
    if ( stack_index_ < ERROR_STACK_SIZE - 1 )
    {
        ++stack_index_;
        errors_[stack_index_] = 0;
    }
    else
    {
        report_error( "Error stack overflow!" );
    }
}

/**
// Pop an error count from the stack of error counts.
//
// The number of errors at the popped level is added to the previous level 
// and returned so that application code can branch based on errors happening
// or not in different sections of code.
//
// @return
//  The number of errors that have been counted at the popped error count 
//  level.
*/
int ErrorPolicy::pop_errors()
{
    SWEET_ASSERT( stack_index_ > 0  );
    int errors = 0;
    if ( stack_index_ > 0 )
    {
        errors = errors_[stack_index_];
        --stack_index_;
        errors_[stack_index_] += errors;
    }
    else
    {
        report_error( "Error stack underflow!" );
    }
    return errors;
}

/**
// Get the number of errors that have occured at the current level of the
// stack of error counts.
//
// @return
//  The number of erros counted at the current error count level.
*/
int ErrorPolicy::errors() const
{
    return errors_[stack_index_];
}

/**
// Count and report an error if \e condition is true; otherwise ignore it.
//
// If \e condition is true then the error count at the current stack level is
// incremented and the printf-style error message is formatted and reported 
// through a call to ErrorPolicy::report_error().
//
// @param condition
//  True to indicate that an error has occured false otherwise.
//
// @param format
//  A printf style format string that describes the error that has occured.
//
// @param ...
//  Variable arguments matching the format specified in \e format.
*/
void ErrorPolicy::error( bool condition, const char* format, ... )
{
    SWEET_ASSERT( format );

    if ( condition )
    {
        va_list args;
        va_start( args, format );
        error_varargs( condition, format, args );
        va_end( args );
    }
}

/**
// Print using printf-style format string.
//
// @param format
//  A printf-style format string that describes the text to print.
//
// @param ...
//  Variable arguments matching the format specified in \e format.
*/
void ErrorPolicy::print( const char* format, ... )
{
    SWEET_ASSERT( format );

    va_list args;
    va_start( args, format );
    print_varargs( format, args );
    va_end( args );
}

/**
// Count and report an error if \e condition is true; otherwise ignore it.
//
// If \e condition is true then the error count at the current stack level is
// incremented and the printf-style error message is formatted and reported 
// through a call to ErrorPolicy::report_error().
//
// @param condition
//  True to indicate that an error has occured false otherwise.
//
// @param format
//  A printf-style format string that describes the error that has occured.
//
// @param args
//  Variable arguments matching the format specified in \e format.
*/
void ErrorPolicy::error_varargs( bool condition, const char* format, va_list args )
{
    if ( condition )
    {
        ++errors_[stack_index_];

        char message [1024];
        vsnprintf( message, sizeof(message), format, args );
        message[sizeof(message) - 1] = 0;

        report_error( message );
    }
}

/**
// Print using printf-style format string.
//
// @param format
//  A printf-style format string that describes the text to print.
//
// @param args
//  Variable arguments matching the format specified in \e format.
*/
void ErrorPolicy::print_varargs( const char* format, va_list args )
{
    char message [1024];
    vsnprintf( message, sizeof(message), format, args );
    message[sizeof(message) - 1] = 0;

    report_print( message );
}

/**
// Report an error.
//
// @param message
//  A message that describes the error.
*/
void ErrorPolicy::report_error( const char* message )
{
    SWEET_ASSERT( message );
    fputs( message, stderr );
    fputs( "\n", stderr );
}

/**
// Report a printed message.
//
// @param message
//  The printed message.
*/
void ErrorPolicy::report_print( const char* message )
{
    SWEET_ASSERT( message );
    fputs( message, stdout );
    fputs( "\n", stdout );
}
