//
// ErrorPolicy.hpp
// Copyright (c) 2001 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_ERROR_ERRORPOLICY_HPP_INCLUDED
#define SWEET_ERROR_ERRORPOLICY_HPP_INCLUDED

#include "declspec.hpp"

namespace std
{
    class exception;
}

namespace sweet
{

namespace error
{

class Error;

/**
// An interface for providing runtime error handling policies.
*/
class SWEET_ERROR_DECLSPEC ErrorPolicy
{
    public:
        ErrorPolicy();
        virtual ~ErrorPolicy();
        virtual void error( const Error& error );
        virtual void error( const std::exception& exception );
};

}

}

#endif
