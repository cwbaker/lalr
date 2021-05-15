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
    struct EventSink : public ErrorPolicy
    {   
        int expected_error_; 
        int errors_;
        
        EventSink( int expected_error )
        : expected_error_( expected_error )
        , errors_( 0 )
        {
        }

        void lalr_error( int /*line*/, int /*column*/, int error, const char* /*format*/, va_list /*args*/ )
        {
            ++errors_;
            // char message [1024];
            // vsnprintf( message, sizeof(message), format, args );
            // printf( "%s\n", message );
            CHECK( error == expected_error_ );
        }
    };

    TEST( ExpressionsThatRequireShiftReduceConflictResolution )
    {
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
        
        EventSink event_sink( PARSER_ERROR_PARSE_TABLE_CONFLICT );
        GrammarCompiler compiler;
        compiler.compile( precedence_grammar, precedence_grammar + strlen(precedence_grammar), &event_sink );
        CHECK( event_sink.errors_ == 0 );
    }

    TEST( MissingGrammarHeader )
    {
        const char* grammar =
            "   %left 'int' float' 'void'; \n"
            "} \n"
        ;
        EventSink event_sink( LALR_ERROR_SYNTAX );
        GrammarCompiler compiler;
        compiler.compile( grammar, grammar + strlen(grammar), &event_sink );
    }

    TEST( MissingGrammarFooter )
    {
        const char* grammar =
            "missing_footer { \n"
            "   %left 'int' 'float' 'void'; \n"
        ;
        EventSink event_sink( LALR_ERROR_SYNTAX );
        GrammarCompiler compiler;
        compiler.compile( grammar, grammar + strlen(grammar), &event_sink );
    }

    TEST( UnterminatedDirectiveLiterals )
    {
        const char* grammar =
            "unterminated_directive_literals { \n"
                "%left 'int' float' 'void'; \n"
                "%left 'return' 'break' 'continue' 'if' 'while' 'for' identifier '{'; \n"
            "} \n"
        ;
        EventSink event_sink( LALR_ERROR_UNTERMINATED_LITERAL );
        GrammarCompiler compiler;
        compiler.compile( grammar, grammar + strlen(grammar), &event_sink );
    }
}
