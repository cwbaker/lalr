//
// ParserErrorPolicy.hpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_PARSER_PARSERERRORPOLICY_HPP_INCLUDED
#define SWEET_PARSER_PARSERERRORPOLICY_HPP_INCLUDED

#include "declspec.hpp"
#include <stdarg.h>

namespace sweet
{

namespace error
{

class Error;

}

namespace parser
{

/**
// An interface to be implemented by classes that wish to be notified of
// errors and %debug information from the %parser library.
*/
class SWEET_PARSER_DECLSPEC ParserErrorPolicy
{
    public:
        virtual ~ParserErrorPolicy();
        virtual void parser_error( int line, const error::Error& error );
        virtual void parser_vprintf( const char* format, va_list args );
};

}

}

#endif
