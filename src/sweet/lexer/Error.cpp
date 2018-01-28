//
// Error.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "Error.hpp"

using namespace sweet::lexer;

/**
// Constructor.
//
// @param error
//  The number that uniquely identifies this %error.
*/
Error::Error( int error )
: sweet::error::Error( error )
{
}
