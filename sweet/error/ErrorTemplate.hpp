//
// ErrorTemplate.hpp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_ERROR_ERRORTEMPLATE_HPP_INCLUDED
#define SWEET_ERROR_ERRORTEMPLATE_HPP_INCLUDED

#include <stdarg.h>

namespace sweet
{

namespace error
{

/**
// A class template for implementing Error derived classes.
*/
template <int ERRNO, class Base>
class ErrorTemplate : public Base
{
    public:
        ErrorTemplate( const char* format, ... );
        ErrorTemplate( const char* format, va_list args );
};

}

}

#include "ErrorTemplate.ipp"

#endif
