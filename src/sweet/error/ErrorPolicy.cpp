//
// ErrorPolicy.cpp
// Copyright (c) 2001 - 2012 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "ErrorPolicy.hpp"

using namespace sweet::error;

/**
// Constructor.
*/
ErrorPolicy::ErrorPolicy()
{
}

/**
// Destructor.
*/
ErrorPolicy::~ErrorPolicy()
{
}

/**
// Handle an error.
//
// @param error
//  The Error object that describes the error that has occured.
*/
void ErrorPolicy::error( const Error& error )
{
}

/**
// Handle a std::exception.
//
// @param exception
//  The std::exception object that describes the error that has occured.
*/
void ErrorPolicy::error( const std::exception& exception )
{
}
