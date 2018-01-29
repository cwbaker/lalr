//
// TestPrecedenceDirectives.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include <sweet/lalr/ParserStateMachine.hpp>
#include <sweet/lalr/ParserErrorPolicy.hpp>
#include <sweet/lalr/Grammar.hpp>
#include <sweet/lalr/ErrorCode.hpp>
#include <unit/UnitTest.h>
#include <stdio.h>
#include <string.h>

using namespace sweet;
using namespace sweet::lalr;

SUITE( PrecedenceDirectives )
{
    TEST( ExpressionsThatRequireShiftReduceConflictResolution )
    {
        struct EventSink : public ParserErrorPolicy
        {    
            int errors_;
            
            EventSink ()
            : errors_( 0 )
            {
            }

            void parser_error( int /*line*/, int error, const char* format, va_list args )
            {
                ++errors_;
                char message [1024];
                vsnprintf( message, sizeof(message), format, args );
                printf( "%s\n", message );
                CHECK( error == PARSER_ERROR_PARSE_TABLE_CONFLICT );
            }
        };

        Grammar grammar;
        grammar.begin()
            .whitespace() ("[ \\t\\r\\n]*")
            .left() ('+') ('-')
            .left() ('*') ('/')
            .none() ("integer")
            .production( "unit" )
                ("expr") [nil]
            .end_production()
            .production( "expr" )
                ("expr") ('+') ("expr") [nil]
                ("expr") ('-') ("expr") [nil]
                ("expr") ('*') ("expr") [nil]
                ("expr") ('/') ("expr") [nil]
                ("integer") [nil]
            .end_production()
            .production( "integer" )
                ("[0-9]+")
            .end_production()
        .end();
        
        EventSink event_sink;
        ParserStateMachine parser_state_machine( grammar, &event_sink );
        CHECK( event_sink.errors_ == 0 );
    }
}
