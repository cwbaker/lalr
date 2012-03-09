//
// Error.cpp
// Copyright (c) 2001 - 2011 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include <sweet/error/Error.hpp>
#include <sweet/assert/assert.hpp>
#include <stdio.h>
#include <windows.h>

using namespace sweet::error;

/**
// Constructor.
//
// @param error
//  The number that uniquely identifies the %error.
*/
Error::Error( int error )
: error_( error )
{
   memset( text_, 0, sizeof(text_) );
}

/**
// Constructor.
//
// @param error
//  The number that uniquely identifies the error.
//
// @param format
//  A printf style format string that describes the error that has occured.
*/
Error::Error( int error, const char* format, ... )
: error_( error )
{
    memset( text_, 0, sizeof(text_) );

    va_list args;
    va_start( args, format );
    append( format, args );
    va_end( args );
}

/**
// Destructor.
*/
Error::~Error()
{
}

/**
// Get the error number of this Error.
//
// @return
//  The error number.
*/
int Error::error() const
{
   return error_;
}

/**
// Get the text of this Error.
//
// @return
//  The text.
*/
const char* Error::what() const
{
   return text_;
}

/**
// Append text to the contents of this Error.
//
// @param format 
//  A printf style format string to append to the text of this Error.
//
// @param args 
//  A variable length argument list that matches that arguments in @e 
//  format.
*/
void Error::append( const char* format, va_list& args )
{
    if ( format )
    {
        char* end = text_ + sizeof(text_);
        char* pos = text_;
        while ( *pos != '\0' && pos < end )
        {
            ++pos;
        }

        _vsnprintf( pos, end - pos, format, args );
        text_[sizeof(text_) - 1] = '\0';
    }
}

/**
// Append text to this Error.
//
// @param text
//  The null termainated string of text to append.
*/
void Error::append( const char* text )
{
    if ( text )
    {
        char* end = text_ + sizeof(text_);
        char* pos = text_;
        while ( *pos != '\0' && pos < end )
        {
            ++pos;
        }

        _snprintf( pos, end - pos, text );
        text_[sizeof(text_) - 1] = '\0';
    }
}

/**
// Format an operating system error message.
//
// @param oserror
//  The operating system error number.
// 
// @param buffer
//  A buffer to place the operating system error message into.
// 
// @param length
//  The length of the buffer.
// 
// @return
//  The buffer.
*/
const char* Error::format( int oserror, char* buffer, size_t length )
{
    SWEET_ASSERT( buffer );
    SWEET_ASSERT( length > 0 );
    int actual_length = ::FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, 0, oserror, 0, buffer, static_cast<int>(length), 0 );
    while ( actual_length > 0 && (buffer[actual_length] == '\n' || buffer[actual_length] == '\r' || buffer[actual_length] == '.' || buffer[actual_length] == 0) )
    {
        buffer[actual_length] = 0;
        --actual_length;
    }
    return buffer;
}
