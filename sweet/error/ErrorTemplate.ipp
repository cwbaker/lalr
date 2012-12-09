//
// ErrorTemplate.ipp
// Copyright (c) 2008 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_ERROR_ERRORTEMPLATE_IPP_INCLUDED
#define SWEET_ERROR_ERRORTEMPLATE_IPP_INCLUDED

#include <stdarg.h>
#include "ErrorTemplate.hpp"

namespace sweet
{

namespace error
{

/**
// Constructor.
//
// @param format
//  A printf style format string that describes the error.
//
// @param ...
//  Parameters as described by \e format.
*/
template <int ERRNO, class Base>
ErrorTemplate<ERRNO, Base>::ErrorTemplate( const char* format, ... )
: Base( ERRNO )
{
    va_list args;
    va_start( args, format );
    Error::append( format, args );
    va_end( args );
}

/**
// Constructor.
//
// @param format
//  A printf style format string that describes the error.
//
// @param args
//  Parameters as described by \e format.
*/
template <int ERRNO, class Base> 
ErrorTemplate<ERRNO, Base>::ErrorTemplate( const char* format, va_list args )
: Base( ERRNO )
{
    Error::append( format, args );
}

}

}

#endif
