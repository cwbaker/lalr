//
// error_functions.hpp
// Copyright (c) 2001 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_ERROR_FUNCTIONS_HPP_INCLUDED
#define SWEET_ERROR_FUNCTIONS_HPP_INCLUDED

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
class ErrorPolicy;

SWEET_ERROR_DECLSPEC void set_error_policy( ErrorPolicy* error_policy );
SWEET_ERROR_DECLSPEC ErrorPolicy* get_error_policy();
SWEET_ERROR_DECLSPEC void error( const Error& error );
SWEET_ERROR_DECLSPEC void error( const std::exception& exception );

}

}

#endif
