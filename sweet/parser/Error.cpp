//
// Error.cpp
// Copyright (c) 2007 - 2011 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "Error.hpp"

using namespace sweet::parser;

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
