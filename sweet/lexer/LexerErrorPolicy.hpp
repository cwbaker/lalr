//
// LexerErrorPolicy.hpp
// Copyright (c) 2010 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LEXER_LEXERERRORPOLICY_HPP_INCLUDED
#define SWEET_LEXER_LEXERERRORPOLICY_HPP_INCLUDED

#include "declspec.hpp"
#include <stdarg.h>

namespace sweet
{

namespace error
{

class Error;

}

namespace lexer
{

/**
// An interface to be implemented by classes that wish to handle errors and 
// %debug information from the %lexer library.
*/
class SWEET_LEXER_DECLSPEC LexerErrorPolicy
{
    public:
        virtual ~LexerErrorPolicy();
        virtual void lexer_error( int line, const error::Error& error );
        virtual void lexer_vprintf( const char* format, va_list args );
};

}

}

#endif
