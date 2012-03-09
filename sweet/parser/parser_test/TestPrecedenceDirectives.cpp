//
// TestParsers.cpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include <sweet/parser/ParserStateMachine.hpp>
#include <sweet/parser/ParserErrorPolicy.hpp>
#include <sweet/parser/Error.hpp>
#include <sweet/unit/UnitTest.h>

using namespace sweet;
using namespace sweet::parser;

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

            void parser_error( int line, const error::Error& error )
            {
                ++errors_;
                printf( "%s\n", error.what() );
                CHECK( error.error() == PARSER_ERROR_PARSE_TABLE_CONFLICT );
            }
        };

        const char* grammar = 
            "ExpressionsThatRequireShiftReduceConflictResolution {\n"
            " \n"
            "   %whitespace \"[ \t\r\n]*\";\n"
            "   %left '+' '-'; \n"
            "   %left '*' '/'; \n"
            "   %none integer; \n"
            " \n"
            "   unit: expr;\n"
            "   expr: expr '+' expr \n"
            "       | expr '-' expr \n"
            "       | expr '*' expr \n"
            "       | expr '/' expr \n"
            "       | integer \n"
            "       ; \n"
            "   integer: \"[0-9]+\"; \n"
            "}"
        ;
        
        EventSink event_sink;
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar), &event_sink );
        CHECK( event_sink.errors_ == 0 );
    }
}
