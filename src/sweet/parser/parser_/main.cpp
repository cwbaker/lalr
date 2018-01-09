//
// main.cpp
// Copyright (c) 2007 - 2012 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "Application.hpp"
#include <stdio.h>
#include <stdlib.h>

using namespace sweet::parser;

int main( int argc, char** argv )
{
    int result = EXIT_FAILURE;

    try
    {
        Application application( argc, argv );
        result = application.get_result();
    }

    catch ( const std::exception& exception )
    {
        fprintf( stderr, "parser: %s.\n", exception.what() );
        result = EXIT_FAILURE;
    }

    return result;
}
