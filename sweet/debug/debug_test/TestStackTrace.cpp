//
// TestStackTrace.cpp
// Copyright (c) 2008 - 2010 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include <sweet/unit/UnitTest.h>
#include <sweet/debug/debug.hpp>

using namespace sweet::debug;

SUITE( TestStackTrace )
{
    struct Fixture
    {
        static void function_one()
        {
            void* addresses[32];
            stack_trace( addresses, 32 );
            Fixture::function_two();
        }

        static void function_two()
        {
            void* addresses[6];
            stack_trace( addresses, 6 );
            Fixture::function_three( 1, 2 );
        }

        static void function_three( int a, int b )
        {
            void* addresses[6];
            stack_trace( addresses, 6 );
        }
    };

    TEST_FIXTURE( Fixture, TestFunctionOne )
    {
        Fixture::function_one();
    }

    TEST_FIXTURE( Fixture, TestFunctionTwo )
    {
        Fixture::function_two();
    }

    TEST_FIXTURE( Fixture, TestFunctionThree )
    {
        Fixture::function_three( 1, 2 );
    }
}