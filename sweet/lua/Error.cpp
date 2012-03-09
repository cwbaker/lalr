//
// Error.cpp
// Copyright (c) 2007  - 2010 Charles Baker.  All rights reserved.
//

#include "Error.hpp"

using namespace sweet::lua;

/**
// Constructor.
//
// @param error
//  A number that uniquely identifies the error that has occured.
*/
Error::Error( int error )
: sweet::error::Error( error )
{
}
