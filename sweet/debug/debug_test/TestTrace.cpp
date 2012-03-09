//
// TestTrace.cpp
// Copyright (c) 2008 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include <sweet/unit/UnitTest.h>
#include <sweet/debug/debug.hpp>

using namespace sweet::debug;

SUITE( TestTrace )
{
    TEST( TestTrace )
    {
        Trace trace;
        trace.open( "test.txt" );
        trace.set_stack_trace_size( 8 );
        trace.get_stack_trace_size();
        trace.printf( "abc, %08x, %d, ", 0xbaadf00d, 128 );
        trace.stack_trace();
        trace.printf( "\n" );
        trace.flush();
        trace.close();
    }
}