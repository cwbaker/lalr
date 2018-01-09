//
// Error.hpp
// Copyright (c) 2001 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_ERROR_ERROR_HPP_INCLUDED
#define SWEET_ERROR_ERROR_HPP_INCLUDED

#include "declspec.hpp"
#include "macros.hpp"
#include <exception>
#include <stdarg.h>

namespace sweet
{

namespace error
{

/**
// Base class for errors.
*/
class SWEET_ERROR_DECLSPEC Error : virtual public std::exception
{
    int error_;
    char text_ [1024];

    public:
        explicit Error( int error );
        Error( int error, const char* format, ... );
        virtual ~Error() throw ();
        int error() const;
        const char* what() const throw ();
        static const char* format( int oserror, char* buffer, unsigned int length );

    protected:
        void append( const char* format, va_list args );
        void append( const char* text );
};

}

}

#endif
