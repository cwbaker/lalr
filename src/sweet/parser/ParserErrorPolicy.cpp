//
// ParserErrorPolicy.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "ParserErrorPolicy.hpp"

using namespace sweet;
using namespace sweet::parser;

/**
// Destructor.
*/
ParserErrorPolicy::~ParserErrorPolicy()
{
}

/**
// An %error has occured in a %ParserStateMachine.
//
// @param line
//  The line number that the %error occured on.
//
// @param error
//  The %Error that describes the %error that has occured.
*/
void 
ParserErrorPolicy::parser_error( int /*line*/, const error::Error& /*error*/ )
{
}

/**
// Debug output has been sent from a %ParserStateMachine.
//
// @param format
//  The printf-style format string that describes the text to print.
//
// @param args
//  Arguments as described by \e format.
*/
void 
ParserErrorPolicy::parser_vprintf( const char* /*format*/, va_list /*args*/ )
{
}
