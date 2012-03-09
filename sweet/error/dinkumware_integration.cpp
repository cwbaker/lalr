//
// dinkumware_integration.cpp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include <sweet/build.hpp>
#include <sweet/error/error.hpp>

#ifndef SWEET_EXCEPTIONS_ENABLED

namespace std
{

class exception;

/**
// @internal
//
// Override the std::_Throw() function that is called instead of throwing 
// exceptions by the Dinkumware STL when _HAS_EXCEPTIONS is defined to 0.
//
// @param exception
//  The std::exception that describes the error that has occured.
*/
void _Throw( const std::exception& exception )
{
    sweet::error::error( exception );
}

}

#endif
