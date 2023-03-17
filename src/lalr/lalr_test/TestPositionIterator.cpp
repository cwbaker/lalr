//
// TestRegularExpressions.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include <UnitTest++/UnitTest++.h>
#include <lalr/RegexCompiler.hpp>
#include <lalr/Lexer.ipp>
#include <lalr/PositionIterator.hpp>
#include <string.h>

using std::string;
using namespace lalr;

SUITE( PositionIterator )
{
    TEST( LineFeeds )
    {
        const char* text = "1\n2\n3\n4\n5";
        PositionIterator<const char*> i( text, text + strlen(text) );

        CHECK( !i.ended() );
        CHECK_EQUAL( '1', *i );
        CHECK_EQUAL( 1, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '\n', *i );
        CHECK_EQUAL( 1, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '2', *i );
        CHECK_EQUAL( 2, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '\n', *i );
        CHECK_EQUAL( 2, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '3', *i );
        CHECK_EQUAL( 3, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '\n', *i );
        CHECK_EQUAL( 3, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '4', *i );
        CHECK_EQUAL( 4, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '\n', *i );
        CHECK_EQUAL( 4, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '5', *i );
        CHECK_EQUAL( 5, i.line() );

        ++i;
        CHECK( i.ended() );
    }

    TEST( CarriageReturns )
    {
        const char* text = "1\r2\r3\r4\r5";
        PositionIterator<const char*> i( text, text + strlen(text) );

        CHECK( !i.ended() );
        CHECK_EQUAL( '1', *i );
        CHECK_EQUAL( 1, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '\r', *i );
        CHECK_EQUAL( 1, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '2', *i );
        CHECK_EQUAL( 2, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '\r', *i );
        CHECK_EQUAL( 2, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '3', *i );
        CHECK_EQUAL( 3, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '\r', *i );
        CHECK_EQUAL( 3, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '4', *i );
        CHECK_EQUAL( 4, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '\r', *i );
        CHECK_EQUAL( 4, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '5', *i );
        CHECK_EQUAL( 5, i.line() );

        ++i;
        CHECK( i.ended() );
    }

    TEST( NewLineCarriageReturns )
    {
        const char* text = "1\n2\r3\n4\r5";
        PositionIterator<const char*> i( text, text + strlen(text) );

        CHECK( !i.ended() );
        CHECK_EQUAL( '1', *i );
        CHECK_EQUAL( 1, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '\n', *i );
        CHECK_EQUAL( 1, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '2', *i );
        CHECK_EQUAL( 2, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '\r', *i );
        CHECK_EQUAL( 2, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '3', *i );
        CHECK_EQUAL( 3, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '\n', *i );
        CHECK_EQUAL( 3, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '4', *i );
        CHECK_EQUAL( 4, i.line() );
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '\r', *i );
        CHECK_EQUAL( 4, i.line() );
        ++i;

        CHECK( !i.ended() );
        ++i;
        CHECK( i.ended() );
    }

    TEST( CarriageReturnLineFeeds )
    {
        const char* text = "1\r\n2\r\n3\r\n4\r\n5";
        PositionIterator<const char*> i( text, text + strlen(text) );

        CHECK( !i.ended() );
        CHECK_EQUAL( '1', *i );
        CHECK_EQUAL( 1, i.line() );
        ++i;
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '2', *i );
        CHECK_EQUAL( 2, i.line() );
        ++i;
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '3', *i );
        CHECK_EQUAL( 3, i.line() );
        ++i;
        ++i;

        CHECK( !i.ended() );
        CHECK_EQUAL( '4', *i );
        CHECK_EQUAL( 4, i.line() );
        ++i;
        ++i;

        CHECK( !i.ended() );
        ++i;
        CHECK( i.ended() );
    }
}
