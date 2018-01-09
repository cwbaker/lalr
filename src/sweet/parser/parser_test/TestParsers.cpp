//
// TestParsers.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "stdafx.hpp"
#include <sweet/parser/Parser.ipp>
#include <sweet/parser/ParserErrorPolicy.hpp>
#include <sweet/parser/ParserStateMachine.hpp>
#include <sweet/parser/ParserSymbol.hpp>
#include <sweet/parser/ParserProduction.hpp>
#include <sweet/parser/Error.hpp>
#include <sweet/lexer/Error.hpp>
#include <sweet/lexer/LexerErrorPolicy.hpp>
#include <functional>
#include <unit/UnitTest.h>
#include <string.h>

using std::bind;
using namespace std::placeholders;
using namespace sweet;
using namespace sweet::lexer;
using namespace sweet::parser;

SUITE( Parsers )
{
    class IgnoreParserErrorPolicy : public ParserErrorPolicy
    {
        public:
            void parser_error( int /*line*/, const error::Error& /*error*/ )
            {
            }
    };

    class PrintParserErrorPolicy : public ParserErrorPolicy     
    {
        public:
            void parser_vprintf( const char* format, va_list args )
            {
                vprintf( format, args );
            }
        
            void parser_error( int line, const error::Error& error )
            {
                printf( "(%d): error: %s.\n", line, error.what() );
            }
    };

    struct CheckParserErrorPolicy : public ParserErrorPolicy
    {    
        int expected_error_;
        int errors_;            
        
        CheckParserErrorPolicy( int expected_error )
        : expected_error_( expected_error ),
          errors_( 0 )
        {
        }

        void parser_error( int /*line*/, const error::Error& error )
        {
            (void) error;
            ++errors_;
            CHECK( error.error() == expected_error_ );
        }
    };
    
    struct CheckLexerErrorPolicy : public lexer::LexerErrorPolicy
    {    
        int expected_error_;
        int errors_;
        
        CheckLexerErrorPolicy( int expected_error )
        : expected_error_( expected_error ),
          errors_( 0 )
        {
        }

        void lexer_error( int /*line*/, const error::Error& error )
        {
            (void) error;
            ++errors_;
            CHECK( error.error() == expected_error_ );
        }
    };
        
    TEST( OrOperator )
    {
        const char* grammar = 
            "OrOperator {\n"
            "   unit: one | two | three;\n"    
            "   one: '1';\n"
            "   two: '2';\n"
            "   three: '3';\n"
            "}"
        ;
        
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        Parser<const char*> parser( &parser_state_machine );
    
        const char* input = "1";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );

        input = "2";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );

        input = "3";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );

        input = "12";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );
        CHECK( !parser.full() );

        input = "4";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );
    }

    TEST( Alternate )
    {
        const char* grammar = 
            "Alternate {\n"
            "   unit: one two_three;\n"
            "   two_three: two | three;\n"
            "   one: '1';\n"
            "   two: '2';\n"
            "   three: '3';\n"
            "}"
        ;
        
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        Parser<const char*> parser( &parser_state_machine );

        const char* input = "12";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "13";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "1";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "2";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "14";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );
    }

    TEST( ZeroToManyRepeats )
    {
        const char* grammar = 
            "ZeroToManyRepeats {\n"
            "   %left two; \n"
            "   unit: one twos three;\n"
            "   twos: twos two | two | %precedence two;\n"
            "   one: '1';\n"
            "   two: '2';\n"
            "   three: '3';\n"
            "}"
        ;
        
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        Parser<const char*> parser( &parser_state_machine );

        const char* input = "13";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "123";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "1222223";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "2";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "12";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "12222";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "1224";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );
    }

    TEST( OneToManyRepeats )
    {
        const char* grammar =
            "OneToManyRepeats {\n"
            "   unit: one twos three;\n"
            "   twos: twos two | two;\n"
            "   one: '1';\n"
            "   two: '2';\n"
            "   three: '3';\n"
            "}"
        ;
        
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        Parser<const char*> parser( &parser_state_machine );

        const char* input = "123";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "1222223";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "2";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "12";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "12222";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "1224";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );
    }

    TEST( Optional )
    {
        const char* grammar =         
            "Optional {\n"            
            "    unit: one two_opt three;\n"
            "    two_opt: two | ;\n"
            "    one: '1';\n"
            "    two: '2';\n"
            "    three: '3';\n"
            "}"
        ;

        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        Parser<const char*> parser( &parser_state_machine );

        const char* input = "123";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "13";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "2";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "12";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "1223";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "12222";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "1224";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );
    }

    TEST( Compound )
    {
        const char* grammar =
            "Compound {\n"
            "    compound: one one_two three;\n"
            "    one_two: one two | two one;\n"
            "    one: '1';\n"
            "    two: '2';\n"
            "    three: '3';\n"
            "}"
        ;
    
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        Parser<const char*> parser( &parser_state_machine );

        const char* input = "1123";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "1213";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "2";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "12";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "1223";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "12222";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "1224";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );
    }

    TEST( BinaryOperator )
    {
        const char* grammar =
            "BinaryOperator {\n"
            "    E: E '+' T | T;\n"
            "    T: T '*' F | F;\n"
            "    F: '(' E ')' | i;\n"
            "    i: \"[0-9]+\";\n"
            "}"
        ;
        
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        Parser<const char*> parser( &parser_state_machine );

        const char* input = "1";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "1+2";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "1*2";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "1+2*3";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "(1+2)*3";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );               
        
        input = "1+2+3*(2+3)";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        
        input = "((1+2)*3";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );
        
        input = "1+";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "4)";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );
        CHECK( !parser.full() );
    }

    TEST( NestedProductions )
    {
        const char* grammar =
            "NestedProductions {\n"
            "    %left 'b' 'c';\n"
            "    A: 'a' bcs 'd';\n"
            "    bcs: bcs bc | bc | %precedence 'b';\n"
            "    bc: 'b' 'c';\n"
            "}"
        ;
        
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        Parser<const char*> parser( &parser_state_machine );

        const char* input = "ad";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "abcbcbcd";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "abcbcbd";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "abcbcbcbbd";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "abcbcbc";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );
    }

    TEST( FollowGeneration )
    {
        const char* grammar =
            "FollowGeneration {\n"
            "    unit: one two four | one three four;\n"
            "    one: '1';\n"
            "    two: '2';\n"
            "    three: '3';\n"
            "    four: '4';\n"
            "}"
        ;
        
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        Parser<const char*> parser( &parser_state_machine );

        const char* input = "124";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "134";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "14";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "12";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "13";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "33";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );
    }
 
    TEST( Canonical )
    {
        const char* grammar =
            "Canonical {\n"
            "    S: C C;\n"
            "    C: 'c' C | 'd';\n"
            "}"
        ;
        
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        Parser<const char*> parser( &parser_state_machine );

        const char* input = "cdd";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "cccccdd";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "c";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "d";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );
    }

    TEST( MultipleDotNodesParser )
    {
        const char* grammar =
            "MultipleDotNodesParser {\n"
            "    unit: lt | lt question;\n"
            "    lt: '<';\n"
            "    question: '?';\n"
            "}"
        ;
        
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        Parser<const char*> parser( &parser_state_machine );

        const char* input = "<";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "<?";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );

        input = "?";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );

        input = "d";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );
    }
       
    TEST( ProductionsOnCollapsedSymbols )
    {
        const char* grammar =
            "ProductionsOnCollapsedSymbols {\n"
            "    unit: 'a' [unit];\n"
            "}"
        ;
        
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        Parser<const char*> parser( &parser_state_machine );
        
        const char* input = "a";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
    }
    
    TEST( ReduceStarNode )
    {
        const char* grammar =
            "ReduceStarNode {\n"
            "   %left one;\n"
            "   unit: ones;\n"
            "   ones: ones one | one | %precedence one;\n"
            "   one: '1';\n"
            "}"
        ;
        
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        Parser<const char*> parser( &parser_state_machine );
        
        const char* input = "1";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        
        input = "111";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
    }

    TEST( ReduceParenthesis )
    {
        const char* grammar =
            "ReduceParenthesis {\n"
            "    expr: '(' expr ')' | '1';\n"
            "}"
        ;
        
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        Parser<const char*> parser( &parser_state_machine );
        
        const char* input = "1";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );
        
        input = "(1)";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );

        input = "((1))";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );

        input = "((1)";
        parser.parse( input, input + strlen(input) );
        CHECK( !parser.accepted() );
        CHECK( parser.full() );
    }

    TEST( ReduceStarAndParenthesis )
    {
        const char* grammar =
            "ReduceStarAndParenthesis {\n"
            "   %left '(' '1';\n"
            "   expr: '(' exprs ')' | '1';\n"
            "   exprs: exprs expr | expr | %precedence '(';\n"
            "}"
        ;
        
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        Parser<const char*> parser( &parser_state_machine );
        
        const char* input = "(111)";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );
    }
    
    TEST( Whitespace )
    {
        const char* grammar =
            "Whitespace {\n"
            "   %whitespace \"[ \\t\\r\\n]*\";\n"
            "   unit: identifiers;\n"
            "   identifiers: identifiers identifier\n"
            "              | identifier\n"
            "              ;\n"
            "   identifier: \"[A-Za-z_][A-Za-z_0-9]*\";\n"
            "}"
        ;

        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        Parser<const char*> parser( &parser_state_machine );
        
        const char* input = "a b";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );

        input = "abc def";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );

        input = "abc \t\r def";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );

        input = "abc \n\t def";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );

        input = "abc";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );
    }    
    
    TEST( String )
    {
        struct StringLexer
        {
            static void string_lexer( const char** begin, const char* end, std::string* lexeme, const void** symbol )
            {
                SWEET_ASSERT( begin && *begin );
                SWEET_ASSERT( end );
                SWEET_ASSERT( lexeme );
                SWEET_ASSERT( symbol );

                lexeme->clear();

                const char* position = *begin;
                while ( position != end && *position != '\'' )
                {
                    *lexeme += *position;
                    ++position;
                }
                
                if ( position != end )
                {
                    SWEET_ASSERT( *position == '\'' );
                    ++position;
                }
                
                *begin = position;
            }
        };
    
        const char* grammar = 
            "String { \n"
            "   %whitespace \"[ \\t\\r\\n]*\";\n"
            "   unit: strings;\n"
            "   strings: strings string | string;\n"
            "   string: \"':string:\";\n"
            "}"
        ;

        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        Parser<const char*> parser( &parser_state_machine );
        parser.lexer_action_handlers()
            ( "string", &StringLexer::string_lexer )
        ;            

        const char* input = "'first' 'second' 'third'";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );
    }
        
    TEST( LineComment )
    {
        struct LineComment
        {
            static void line_comment( const char** begin, const char* end, std::string* lexeme, const void** symbol )
            {
                SWEET_ASSERT( begin && *begin );
                SWEET_ASSERT( end );
                SWEET_ASSERT( lexeme );
                SWEET_ASSERT( symbol );
                
                lexeme->clear();
                
                const char* position = *begin;
                while ( position != end && *position != '\n' && *position != '\r' )
                {
                    ++position;
                }
                
                *begin = position;
            }
        };
    
        const char* grammar = 
            "LineComment {\n"
            "   %whitespace \"([ \\t\\r\\n]|\\/\\/:line_comment:)*\";\n"
            "   unit: digits;\n"
            "   digits: digits digit | digit;\n"
            "   digit: \"[0-9]\";\n"
            "}"
        ;

        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        Parser<const char*> parser( &parser_state_machine );
        parser.lexer_action_handlers()
            ( "line_comment", &LineComment::line_comment )
        ;

        const char* input = 
            "1 2 // This is a comment\n"
            "3 4 // This is another comment\r"
            "// 5 6 \n\r"
        ;
        
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );
    }

    TEST( BlockComment )
    {
        struct BlockComment
        {
            static void block_comment( const char** begin, const char* end, std::string* lexeme, const void** symbol )
            {
                SWEET_ASSERT( begin );
                SWEET_ASSERT( lexeme );
                SWEET_ASSERT( symbol );

                lexeme->clear();
                                
                bool done = false;
                const char* position = *begin;
                while ( position != end && !done )
                {
                    while ( position != end && *position != '*' )
                    {
                        ++position;
                    }

                    if ( position != end )
                    {
                        SWEET_ASSERT( *position == '*' );

                        ++position;
                        if ( *position == '/' )
                        {
                            ++position;
                            done = true;
                        }
                    }        
                }
                
                *begin = position;
            }
        };
    
        const char* grammar = 
            "BlockComment {\n"
            "   %whitespace \"([ \\t\\r\\n]|\\/\\*:block_comment:)*\";\n"
            "   unit: digits;\n"
            "   digits: digits digit | digit;\n"
            "   digit: \"[0-9]\";\n"
            "}"
        ;

        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        Parser<const char*> parser( &parser_state_machine );
        parser.lexer_action_handlers()
            ( "block_comment", &BlockComment::block_comment )
        ;

        const char* input = 
            "1 2 /* This is a comment\n"
            "3 */ 4 /* This is another comment\r"
            "*/ 5 6 /* This is a third comment */ \n\r"
        ;
        
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );
    }

    TEST( SyntaxErrorsInGrammar )
    {
        {    
            const char* missing_open_brace = 
                "MissingOpenBrace \n"
                "   %whitespace \"[ \\t\\r\\n]*\";\n"
                "   unit: string;\n"
                "   string: \"':string:'\";\n"
                "}"
            ;

            CheckParserErrorPolicy error_policy( PARSER_ERROR_SYNTAX );
            ParserStateMachine parser_state_machine( missing_open_brace, missing_open_brace + strlen(missing_open_brace), &error_policy );
            CHECK( error_policy.errors_ == 1 );
            CHECK( parser_state_machine.start_state() == NULL );
            CHECK( parser_state_machine.states().empty() );
        }

        {
            const char* missing_close_quotes = 
                "MissingCloseQuotes { \n"
                "   %whitespace \"[ \\t\\r\\n]*;\n"
                "   unit: strings;\n"
                "   strings: strings string | string;\n"
                "   string: \"':string:';\n"
                "}"
            ;

            CheckParserErrorPolicy error_policy( PARSER_ERROR_SYNTAX );
            ParserStateMachine parser_state_machine( missing_close_quotes, missing_close_quotes + strlen(missing_close_quotes), &error_policy );
            CHECK( error_policy.errors_ == 2 );
            CHECK( parser_state_machine.start_state() == NULL );
            CHECK( parser_state_machine.states().empty() );
        }
    }    
    
    TEST( SyntaxErrorsInRegularExpressions )
    {
        const char* grammar = 
            "SyntaxErrorsInRegularExpressions {\n"
            "   %whitespace \"[ \\t\\r\\n*\";\n"
            "   one: \"[A-z*\";\n"
            "}"
        ;

        CheckLexerErrorPolicy error_policy( LEXER_ERROR_SYNTAX );
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar), NULL, &error_policy );
        CHECK( error_policy.errors_ == 2 );
    }
        
    TEST( UndefinedSymbolError )
    {
        const char* grammar = 
            "UndefinedSymbolError {\n"
            "   %whitespace \"[ \\t\\r\\n*\";\n"
            "   one: undefined_symbol;\n"
            "}"
        ;

        CheckParserErrorPolicy error_policy( PARSER_ERROR_UNDEFINED_SYMBOL );
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar), &error_policy );
        CHECK( error_policy.errors_ == 1 );        
    }
        
    TEST( UnreferencedSymbolError )
    {
        const char* grammar = 
            "UnreferencedSymbolError {\n"
            "   one: 'one';\n"
            "   unreferenced_symbol: 'two';\n"
            "}"
        ;

        CheckParserErrorPolicy error_policy( PARSER_ERROR_UNREFERENCED_SYMBOL );
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar), &error_policy );
        CHECK( error_policy.errors_ == 1 );
    }
    
    TEST( SymbolsOnlyAppearingInPrecedenceDirectivesAreCountedAsReferenced )
    {
        const char* grammar = 
            "SymbolsOnlyAppearingInPrecedenceDirectivesAreCountedAsReferenced { \n"
            "   %left unary_minus; \n"
            "   %left '+' '-'; \n"
            "   %left '/' '*'; \n"
            " \n"
            "   expression: expression '/' expression \n"
            "             | expression '*' expression \n"
            "             | expression '+' expression \n"
            "             | expression '-' expression \n"
            "             | '-' expression %precedence unary_minus \n"
            "             | integer \n"
            "             ; \n"
            " \n"
            " integer: \"[0-9]+\"; \n"
            "}"
        ;

        CheckParserErrorPolicy error_policy( PARSER_ERROR_NONE );
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar), &error_policy );
        CHECK( error_policy.errors_ == 0 );
    }
        
    TEST( LexerConflict )
    {
        const char* grammar = 
            "LexerConflictError {\n"
            "   %whitespace \"[ \\t\\r\\n]*\";\n"
            "   %left prototype value;\n"
            "   target: prototype '{' targets '}';\n"
            "   targets: targets target | targets value | target | value | %precedence prototype;\n"
            "   prototype: \"[A-Za-z_][A-Za-z_0-9]*\"; value: \"[A-Za-z_0-9\\./@:-]+\";\n"
            "}"
        ;
        
        CheckLexerErrorPolicy error_policy( LEXER_ERROR_SYMBOL_CONFLICT );
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar), NULL, &error_policy );
        CHECK( error_policy.errors_ == 1 );                
    }

    struct LexerConflictResolutionActionHandler
    {
        void* prototype_symbol_;
        void* value_symbol_;

        LexerConflictResolutionActionHandler( const ParserStateMachine& parser_state_machine )
        : prototype_symbol_( (void*) parser_state_machine.find_symbol_by_identifier("prototype") ),
          value_symbol_( (void*) parser_state_machine.find_symbol_by_identifier("value") )
        {
            SWEET_ASSERT( prototype_symbol_ );
            SWEET_ASSERT( value_symbol_ );
        }

        void prototype( const char** begin, const char* end, std::string* lexeme, const void** symbol ) const
        {
            SWEET_ASSERT( begin && *begin );
            SWEET_ASSERT( end );
            SWEET_ASSERT( lexeme );
            SWEET_ASSERT( symbol );
            *symbol = *lexeme == "prototype" ? prototype_symbol_ : value_symbol_;
        }
    };
    
    TEST( LexerConflictResolution )
    {
        const char* grammar = 
            "LexerSymbolConflictResolvedByAction {\n"
            "   %whitespace \"[ \\t\\r\\n]*\";\n"
            "   %left prototype value;\n"
            "   target: prototype '{' targets '}';\n"
            "   targets: targets target | targets value | target | value | %precedence prototype;\n"
            "   prototype: \"[A-Za-z_][A-Za-z0-9_]*:prototype:\";\n"
            "   value: \"[A-Za-z0-9_\\./@:-]+\";\n"
            "}"
        ;
        
        CheckParserErrorPolicy error_policy( PARSER_ERROR_NONE );
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar), &error_policy );
        CHECK( error_policy.errors_ == 0 );

        if ( error_policy.errors_ == 0 )
        {
            Parser<const char*> parser( &parser_state_machine );
            LexerConflictResolutionActionHandler action_handler( parser_state_machine );
            parser.lexer_action_handlers()
                ( "prototype", bind(&LexerConflictResolutionActionHandler::prototype, &action_handler, _1, _2, _3, _4) )
            ;

            const char* input = "prototype { prototype { value value } value value }";
            parser.parse( input, input + strlen(input) );
            CHECK( parser.accepted() );
            CHECK( parser.full() );
        }
    }
        
    TEST( AssociativityDirectives )
    {
        const char* grammar = 
            "AssociativityDirectives {\n"
            "   %left '+' '-';\n"
            "   %left '*' '/';\n"
            "   %none integer;\n"
            "   unit: expr;\n"
            "   expr: expr '+' expr | expr '-' expr | expr '*' expr | expr '/' expr | integer;\n"
            "   integer: \"[0-9]+\";\n"
            "}"
        ;
        
        CheckParserErrorPolicy error_policy( PARSER_ERROR_PARSE_TABLE_CONFLICT );
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar), &error_policy );
        CHECK( error_policy.errors_ == 0 );

        if ( error_policy.errors_ == 0 )
        {
            const char* input = "1+2";
            Parser<const char*> parser( &parser_state_machine );
            parser.parse( input, input + strlen(input) );
            CHECK( parser.accepted() );
            CHECK( parser.full() );
            
            input = "1+2*3";
            parser.parse( input, input + strlen(input) );
            CHECK( parser.accepted() );
            CHECK( parser.full() );
            
            input = "1*2+3";
            parser.parse( input, input + strlen(input) );
            CHECK( parser.accepted() );
            CHECK( parser.full() );
        }
    }

    TEST( PrecedenceDirectives )
    {
        const char* grammar = 
            "PrecedenceDirectives {\n"
            "   %left '+';\n"
            "   %left '-';\n"
            "   %none integer;\n"
            "   unit: expr;\n"
            "   expr: expr '+' expr %precedence '-' [first]\n"
            "       | expr '-' expr %precedence '+' [second]\n"
            "       | integer\n"
            "       ;\n"
            "   integer: \"[0-9]+\";\n"
            "}"
        ;
        
        CheckParserErrorPolicy error_policy( PARSER_ERROR_PARSE_TABLE_CONFLICT );
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar), &error_policy );
        CHECK( error_policy.errors_ == 0 );

        const ParserSymbol* expr = parser_state_machine.find_symbol_by_identifier( "expr" );
        CHECK( expr );
        if ( expr )
        {
            CHECK( expr->get_productions()[0]->get_precedence() == 2 );
            CHECK( expr->get_productions()[1]->get_precedence() == 1 );
            CHECK( expr->get_productions()[2]->get_precedence() == 3 );
        }
    }    
    
    TEST( ErrorSymbolOnLeftHandSideError )
    {
        const char* grammar = 
            "ErrorSymbolOnLeftHandSideError {\n"
            "   %whitespace \"[ \\t\\r\\n]*\";\n"
            "   unit: prototype '{' error '}';\n"
            "   prototype: \"[A-Za-z_][A-Za-z_0-9]*\";\n"
            "   error: ;\n"
            "}"
        ;
        
        CheckParserErrorPolicy error_policy( PARSER_ERROR_ERROR_SYMBOL_ON_LEFT_HAND_SIDE );
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar), &error_policy );
        CHECK( error_policy.errors_ == 1 );             
    }
        
    TEST( ErrorProcessing )
    {
        const char* grammar =
            "ErrorProcessing {"
            "   %left error;\n"
            "   %left '+';\n"
            "   expr: expr '+' expr [add] | expr error expr [error] | integer;\n"
            "   integer: \"[0-9]+\";\n"
            "}"
        ;
        
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
        const char* input = "1+2";
        Parser<const char*> parser( &parser_state_machine );
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );        

        input = "1-2";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );       
    }
        
    TEST( EmptyProduction )
    {
        const char* grammar =
            "EmptyProduction { \n"
            "   %left integer; \n"
            "   unit: statements; \n"
            "   statements: statements statement | statement %precedence integer | %precedence integer; \n"
            "   statement: integer ';'; \n"
            "   integer: \"[0-9]+\"; \n"
            "}"
        ;
        
        PrintParserErrorPolicy error_policy;
        ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar), &error_policy );
        const char* input = "1;2;3;";
        Parser<const char*> parser( &parser_state_machine );
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );        

        input = "1;";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );        

        input = "";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );
    }
}
