//
// TestPrecedenceDirectives.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include <lalr/ParserStateMachine.hpp>
#include <lalr/GrammarCompiler.hpp>
#include <lalr/ErrorPolicy.hpp>
#include <lalr/ErrorCode.hpp>
#include <UnitTest++/UnitTest++.h>
#include <vector>
#include <initializer_list>
#include <stdio.h>
#include <string.h>

using namespace lalr;

SUITE( PrecedenceDirectives )
{
    struct EventSink : public ErrorPolicy
    {   
        std::vector<int> expected_errors_;
        int errors_;
        
        EventSink( int expected_error )
        : expected_errors_()
        , errors_( 0 )
        {
            expected_errors_.push_back(  expected_error );
        }

        EventSink( std::initializer_list<int> expected_errors )
        : expected_errors_()
        , errors_( 0 )
        {
            expected_errors_.insert( expected_errors_.end(), expected_errors.begin(), expected_errors.end() );
        }

        void lalr_error( int /*line*/, int /*column*/, int error, const char* /*format*/, va_list /*args*/ )
        {
            CHECK( errors_ < int(expected_errors_.size()) );
            if ( errors_ < int(expected_errors_.size()) )
            {
                CHECK_EQUAL( expected_errors_[errors_], error );
            }
            ++errors_;
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
            "   %whitespace \"[ \\t\\r\\n]*\"; \n"
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
            "   %whitespace \"[ \\t\\r\\n]*\"; \n"
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
            "   %whitespace \"[ \\t\\r\\n]*\"; \n"
                "%left 'int' float' 'void'; \n"
                "%left 'return' 'break' 'continue' 'if' 'while' 'for' identifier '{'; \n"
            "} \n"
        ;
        EventSink event_sink{ LALR_ERROR_UNTERMINATED_LITERAL, LALR_ERROR_SYNTAX, LALR_ERROR_SYNTAX };
        GrammarCompiler compiler;
        int errors = compiler.compile( grammar, grammar + strlen(grammar), &event_sink );
        CHECK( errors > 0 );
    }
}
