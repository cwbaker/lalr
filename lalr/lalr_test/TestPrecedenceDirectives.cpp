//
// TestPrecedenceDirectives.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include <lalr/ParserStateMachine.hpp>
#include <lalr/GrammarCompiler.hpp>
#include <lalr/ErrorPolicy.hpp>
#include <lalr/ErrorCode.hpp>
#include <UnitTest++/UnitTest++.h>
#include <stdio.h>
#include <string.h>

using namespace lalr;

SUITE( PrecedenceDirectives )
{
    TEST( ExpressionsThatRequireShiftReduceConflictResolution )
    {
        struct EventSink : public ErrorPolicy
        {    
            int errors_;
            
            EventSink ()
            : errors_( 0 )
            {
            }

            void lalr_error( int /*line*/, int /*column*/, int error, const char* format, va_list args )
            {
                ++errors_;
                char message [1024];
                vsnprintf( message, sizeof(message), format, args );
                printf( "%s\n", message );
                CHECK( error == PARSER_ERROR_PARSE_TABLE_CONFLICT );
            }
        };

        const char* precedence_grammar = 
            "precedence_grammar { \n"
            "   %whitespace \"[ \\t\\r\\n]*\"; \n"
            "   %left '+' '-'; \n"
            "   %left '*' '/'; \n"
            "   %none integer; \n"
            "   unit: expr; \n"
            "   expr: \n"
            "       expr '+' expr | \n"
            "       expr '-' expr | \n"
            "       expr '*' expr | \n"
            "       expr '/' expr | \n"
            "       integer \n"
            "   ; \n"
            "   integer: \"[0-9]+\"; \n"
            "} \n"
        ;
        
        EventSink event_sink;
        GrammarCompiler compiler;
        compiler.compile( precedence_grammar, precedence_grammar + strlen(precedence_grammar), &event_sink );
        CHECK( event_sink.errors_ == 0 );
    }
}
