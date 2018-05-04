#ifndef LALR_ERRORPOLICY_HPP_INCLUDED
#define LALR_ERRORPOLICY_HPP_INCLUDED

#include <stdarg.h>

namespace lalr
{

/**
// An interface to be implemented by classes that wish to be notified of
// errors and %debug information from the *lalr* library.
*/
class ErrorPolicy
{
public:
    virtual ~ErrorPolicy();
    virtual void lalr_error( int line, int error, const char* format, va_list args );
    virtual void lalr_vprintf( const char* format, va_list args );
};

}

#endif
