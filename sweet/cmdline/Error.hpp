//
// Error.hpp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_CMDLINE_ERROR_HPP_INCLUDED
#define SWEET_CMDLINE_ERROR_HPP_INCLUDED

#include "declspec.hpp"
#include <sweet/error/Error.hpp>
#include <sweet/error/ErrorTemplate.hpp>

namespace sweet
{

namespace cmdline
{

/**
// Error codes for exceptions thrown from the %cmdline library.
//
// @relates Error
*/
enum ErrorCodes
{
    CMDLINE_ERROR_NONE,            ///< No error has occured.
    CMDLINE_ERROR_INVALID_OPTION,  ///< An option on the command line is not recognized or expects an argument and is grouped with other options.
    CMDLINE_ERROR_INVALID_ARGUMENT ///< An option on the command line that requires an argument doesn't have one.
};

/**
// Errors thrown from the %cmdline library.
*/
class SWEET_CMDLINE_DECLSPEC Error : public error::Error 
{
    public:
        Error( int error );
};

/**
// An option on the command line is not recognized or expects an argument and 
// is grouped with other options.
//
// @relates Error
*/
typedef error::ErrorTemplate<CMDLINE_ERROR_INVALID_OPTION, Error> InvalidOptionError;

/**
// An option on the command line that requires an argument doesn't have 
// one.
//
// @relates Error
*/
typedef error::ErrorTemplate<CMDLINE_ERROR_INVALID_ARGUMENT, Error> InvalidArgumentError;

}

}

#endif
