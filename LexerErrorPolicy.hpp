#ifndef SWEET_LEXER_LEXERERRORPOLICY_HPP_INCLUDED
#define SWEET_LEXER_LEXERERRORPOLICY_HPP_INCLUDED

#include <stdarg.h>

namespace sweet
{

namespace error
{

class Error;

}

namespace lalr
{

/**
// An interface to be implemented by classes that wish to handle errors and 
// %debug information from the %lexer library.
*/
class LexerErrorPolicy
{
    public:
        virtual ~LexerErrorPolicy();
        virtual void lexer_error( int line, int error, const char* format, va_list args );
        virtual void lexer_vprintf( const char* format, va_list args );
};

}

}

#endif
