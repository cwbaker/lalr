//
// Error.hpp
// Copyright (c) 2007 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_ERROR_HPP_INCLUDED
#define SWEET_LUA_ERROR_HPP_INCLUDED

#include "declspec.hpp"
#include <sweet/error/Error.hpp>
#include <sweet/error/ErrorTemplate.hpp>
#include <sweet/error/macros.hpp>

namespace sweet
{

namespace lua
{

/**
// Unique identifiers for the errors thrown from the %lua library.
//
// @relates Error
*/
enum ErrorCode
{
    LUA_ERROR_NONE, ///< No %error occured.
    LUA_ERROR_OPENING_FILE_FAILED, /// Opening a script file failed.
    LUA_ERROR_SYNTAX, ///< A syntax %error occured while compiling a script.
    LUA_ERROR_MEMORY_ALLOCATION, ///< An attempt to allocate memory failed.
    LUA_ERROR_RUNTIME, ///< An %error occured while executing a script.
    LUA_ERROR_PANIC ///< The %Lua virtual machine called the panic handler.
};

/**
// Errors thrown from the %lua library.
*/
class SWEET_LUA_DECLSPEC Error : public error::Error
{
    public:
        Error( int error );
};

/**
// Opening a script file failed.
//
// @relates Error
*/
typedef error::ErrorTemplate<LUA_ERROR_OPENING_FILE_FAILED, Error> OpeningFileFailedError;

/**
// A syntax %error occured while compiling a script.
//
// @relates Error
*/
typedef error::ErrorTemplate<LUA_ERROR_SYNTAX, Error> SyntaxError;

/**
// An attempt to allocate %memory failed.
//
// @relates Error
*/
typedef error::ErrorTemplate<LUA_ERROR_MEMORY_ALLOCATION, Error> MemoryAllocationError;

/**
// An %error occured while executing a script.
//
// @relates Error
*/
typedef error::ErrorTemplate<LUA_ERROR_RUNTIME, Error> RuntimeError;

/**
// The %Lua virtual machine called the panic handler.
//
// @relates Error
*/
typedef error::ErrorTemplate<LUA_ERROR_PANIC, Error> PanicError;

}

}

#endif
