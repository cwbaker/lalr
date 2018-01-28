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
#include <sweet/parser/ErrorCode.hpp>
#include <sweet/parser/Grammar.hpp>
#include <sweet/lexer/ErrorCode.hpp>
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
            void parser_error( int /*line*/, int /*error*/, const char* /*format*/, va_list /*args*/ )
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
        
            void parser_error( int line, int /*error*/, const char* format, va_list args )
            {
                char message [1024];
                vsnprintf( message, sizeof(message), format, args );
                printf( "(%d): error: %s.\n", line, message );
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

        void parser_error( int /*line*/, int error, const char* /*format*/, va_list /*args*/ )
        {
            (void) error;
            ++errors_;
            CHECK( error == expected_error_ );
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

        void lexer_error( int /*line*/, int error, const char* /*format*/, va_list /*args*/ )
        {
            (void) error;
            ++errors_;
            CHECK( error == expected_error_ );
        }
    };
        
    TEST( OrOperator )
    {
        Grammar grammar;
        grammar.begin()
            .production( "unit" )
                ("one") [nil]
                ("two") [nil]
                ("three") [nil]
            .end_production()
            .production("one")
                ('1')
            .end_production()
            .production("two")
                ('2')
            .end_production()
            .production("three")
                ('3')
            .end_production()
        .end();
        
        ParserStateMachine parser_state_machine( grammar );
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
        Grammar grammar;
        grammar.begin()
            .production( "unit" )
                ("one") ("two_or_three")
            .end_production()
            .production( "two_or_three" )
                ("two") [nil]
                ("three") ["three"]
            .end_production()
            .production("one")
                ('1')
            .end_production()
            .production("two")
                ('2')
            .end_production()
            .production("three")
                ('3')
            .end_production()
        .end();
        
        ParserStateMachine parser_state_machine( grammar );
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
        Grammar grammar;
        grammar.begin()
            .left() ("two")
            .production( "unit" )
                ("one") ("twos") ("three")
            .end_production()
            .production( "twos" )
                ("twos") ("two") [nil]
                ("two") [nil]
                (nil) .precedence( "two")
            .end_production()
            .production("one")
                ('1')
            .end_production()
            .production("two")
                ('2')
            .end_production()
            .production("three")
                ('3')
            .end_production()            
        .end();
        
        ParserStateMachine parser_state_machine( grammar );
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
        Grammar grammar;
        grammar.begin()
            .production( "unit" )
                ("one") ("twos") ("three")
            .end_production()
            .production( "twos" )
                ("twos") ("two") [nil]
                ("two")
            .end_production()
            .production("one")
                ('1')
            .end_production()
            .production("two")
                ('2')
            .end_production()
            .production("three")
                ('3')
            .end_production()
        .end();
        
        ParserStateMachine parser_state_machine( grammar );
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
        Grammar grammar;
        grammar.begin()
            .production( "unit" )
                ("one") ("two_opt") ("three")
            .end_production()
            .production( "two_opt" )
                ("two") [nil]
                (nil) 
            .end_production()
            .production("one")
                ('1')
            .end_production()
            .production("two")
                ('2')
            .end_production()
            .production("three")
                ('3')
            .end_production()            
        .end();

        ParserStateMachine parser_state_machine( grammar );
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
        Grammar grammar;
        grammar.begin()
            .production( "compound" )
                ("one") ("one_two") ("three")
            .end_production()
            .production( "one_two" )
                ("one") ("two") [nil]
                ("two") ("one")
            .end_production()
            .production("one")
                ('1')
            .end_production()
            .production("two")
                ('2')
            .end_production()
            .production("three")
                ('3')
            .end_production()
        .end();
    
        ParserStateMachine parser_state_machine( grammar );
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
        Grammar grammar;
        grammar.begin()
            .production( "E" )
                ("E") ('+') ("T") [nil]
                ("T") [nil]
            .end_production()
            .production( "T" )
                ("T") ('*') ("F") [nil]
                ("F") [nil]
            .end_production()
            .production( "F" )
                ('(') ("E") (')') [nil]
                ("i") [nil]
            .end_production()
            .production( "i" )
                ("[0-9]+") [nil]
            .end_production()
        .end();
        
        ParserStateMachine parser_state_machine( grammar );
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
        Grammar grammar;
        grammar.begin()
            .left() ('b') ('c')
            .production( "A" )
                ('a') ("bcs") ('d')
            .end_production()
            .production( "bcs" )
                ("bcs") ("bc") [nil]
                ("bc") [nil]
                (nil) .precedence('b') [nil]
            .end_production()
            .production( "bc" )
                ('b') ('c')
            .end_production()
        .end();
        
        ParserStateMachine parser_state_machine( grammar );
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
        Grammar grammar;
        grammar.begin()
            .production( "unit" )
                ("one") ("two") ("four") [nil]
                ("one") ("three") ("four") [nil]
            .end_production()
            .production( "one" )
                ('1')
            .end_production()
            .production( "two" )
                ('2')
            .end_production()
            .production( "three" )
                ('3')
            .end_production()
            .production( "four" )
                ('4')
            .end_production()
        .end();
        
        ParserStateMachine parser_state_machine( grammar );
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
        Grammar grammar;
        grammar.begin()
            .production( "S" )
                ("C") ("C") [nil]
            .end_production()
            .production( "C" )
                ('c') ("C") [nil]
                ('d') [nil]
            .end_production()
        .end();
        
        ParserStateMachine parser_state_machine( grammar );
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
        Grammar grammar;
        grammar.begin()
            .production( "unit" )
                ("lt") [nil]
                ("lt") ("question") [nil]
            .end_production()
            .production( "lt" )
                ('<')
            .end_production()
            .production( "question" )
                ('?')
            .end_production()
        .end();
        
        ParserStateMachine parser_state_machine( grammar );
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
        Grammar grammar;
        grammar.begin()
            .production( "unit" )
                ('a') ["unit"]
            .end_production()
        .end();
        
        ParserStateMachine parser_state_machine( grammar );
        Parser<const char*> parser( &parser_state_machine );
        
        const char* input = "a";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
    }
    
    TEST( ReduceStarNode )
    {
        Grammar grammar;
        grammar.begin()
            .left() ("one")
            .production( "unit" )
                ("ones") [nil]
            .end_production()
            .production( "ones" )
                ("ones") ("one") [nil]
                ("one") [nil]
                (nil) .precedence("one") [nil]
            .end_production()
            .production( "one" )
                ('1')
            .end_production()
        .end();
        
        ParserStateMachine parser_state_machine( grammar );
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
        Grammar grammar;
        grammar.begin()
            .production( "expr" )
                ('(') ("expr") (')') [nil]
                ('1') [nil]
            .end_production()
        .end();
        
        ParserStateMachine parser_state_machine( grammar );
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
        Grammar grammar;
        grammar.begin()
            .left() ('(') ('1')
            .production( "expr" )
                ('(') ("exprs") (')') [nil]
                ('1') [nil]
            .end_production()
            .production( "exprs" )
                ("exprs") ("expr") [nil]
                ("expr") [nil]
                (nil) .precedence('(') [nil]
            .end_production()
        .end();
        
        ParserStateMachine parser_state_machine( grammar );
        Parser<const char*> parser( &parser_state_machine );
        
        const char* input = "(111)";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );
    }
    
    TEST( Whitespace )
    {
        Grammar grammar;
        grammar.begin()
            .whitespace() ("[ \\t\\r\\n]*")
            .production( "unit" )
                ("identifiers") [nil]
            .end_production()
            .production( "identifiers" )
                ("identifiers") ("identifier") [nil]
                ("identifier") [nil]
            .end_production()
            .production( "identifier" )
                ("[A-Za-z_][A-Za-z_0-9]*")
            .end_production()
        .end();

        ParserStateMachine parser_state_machine( grammar );
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
            static void string_lexer( const char** begin, const char* end, std::string* lexeme, const void** /*symbol*/ )
            {
                SWEET_ASSERT( begin && *begin );
                SWEET_ASSERT( end );
                SWEET_ASSERT( lexeme );

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
    
        Grammar grammar;
        grammar.begin()
            .whitespace() ("[ \\t\\r\\n]*")
            .production( "unit" )
                ("strings")
            .end_production()
            .production( "strings" )
                ("strings") ("string") [nil]
                ("string") [nil]
            .end_production()
            .production( "string" )
                ("':string:")
            .end_production()
        .end();

        ParserStateMachine parser_state_machine( grammar );
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
            static void line_comment( const char** begin, const char* end, std::string* lexeme, const void** /*symbol*/ )
            {
                SWEET_ASSERT( begin && *begin );
                SWEET_ASSERT( end );
                SWEET_ASSERT( lexeme );
                
                lexeme->clear();
                
                const char* position = *begin;
                while ( position != end && *position != '\n' && *position != '\r' )
                {
                    ++position;
                }
                
                *begin = position;
            }
        };
    
        Grammar grammar;
        grammar.begin()
            .whitespace() ("([ \\t\\r\\n]|\\/\\/:line_comment:)*")
            .production( "unit" )
                ("digits") [nil]
            .end_production()
            .production( "digits" )
                ("digits") ("digit") [nil]
                ("digit") [nil]
            .end_production()
            .production( "digit" )
                ("[0-9]")
            .end_production()
        .end();

        ParserStateMachine parser_state_machine( grammar );
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
            static void block_comment( const char** begin, const char* end, std::string* lexeme, const void** /*symbol*/ )
            {
                SWEET_ASSERT( begin );
                SWEET_ASSERT( lexeme );

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
    
        Grammar grammar;
        grammar.begin()
            .whitespace() ("([ \\t\\r\\n]|\\/\\*:block_comment:)*")
            .production( "unit" )
                ("digits")
            .end_production()
            .production( "digits" )
                ("digits") ("digit") [nil]
                ("digit") [nil]
            .end_production()
            .production( "digit" )
                ("[0-9]")
            .end_production()
        .end();

        ParserStateMachine parser_state_machine( grammar );
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

    TEST( SyntaxErrorsInRegularExpressions )
    {
        Grammar grammar;
        grammar.begin()
            .whitespace() ("[ \\t\\r\\n*")
            .production( "one" )
                ("[A-z*")
            .end_production()
        .end();

        CheckLexerErrorPolicy error_policy( LEXER_ERROR_SYNTAX );
        ParserStateMachine parser_state_machine( grammar, nullptr, &error_policy );
        CHECK( error_policy.errors_ == 2 );
    }

    TEST( UnreferencedSymbolError )
    {
        Grammar grammar;
        grammar.begin()
            .production( "one" )
                ('1')
            .end_production()
            .production( "unreferenced_symbol" )
                ('2')
            .end_production()
        .end();

        CheckParserErrorPolicy error_policy( PARSER_ERROR_UNREFERENCED_SYMBOL );
        ParserStateMachine parser_state_machine( grammar, &error_policy );
        CHECK( error_policy.errors_ == 1 );
    }
    
    TEST( SymbolsOnlyAppearingInPrecedenceDirectivesAreCountedAsReferenced )
    {
        Grammar grammar;
        grammar.begin()
            .left() ("unary_minus")
            .left() ('+') ('-')
            .left() ('*') ('/')
            .production( "expr" )
                ("expr") ('/') ("expr") [nil]
                ("expr") ('*') ("expr") [nil]
                ("expr") ('+') ("expr") [nil]
                ("expr") ('-') ("expr") [nil]
                ('-') ("expr") .precedence("unary_minus") [nil]
                ("integer") [nil]
            .end_production()
            .production( "integer" )
                ("[0-9]+")
            .end_production()
        .end();

        CheckParserErrorPolicy error_policy( PARSER_ERROR_NONE );
        ParserStateMachine parser_state_machine( grammar, &error_policy );
        CHECK( error_policy.errors_ == 0 );
    }
        
    TEST( LexerConflict )
    {
        Grammar grammar;
        grammar.begin()
            .whitespace() ("[ \\t\\r\\n]*")
            .left() ("prototype") ("value") 
            .production( "target" )
                ("prototype") ('{') ("targets") ('}') [nil]
            .end_production()
            .production( "targets" )
                ("targets") ("target") [nil]
                ("targets") ("value") [nil]
                ("target") [nil]
                ("value") [nil]
                (nil) .precedence("prototype")
            .end_production()
            .production( "prototype" )
                ("[A-Za-z_][A-Za-z_0-9]*")
            .end_production()
            .production( "value" )
                ("[A-Za-z_0-9\\./@:-]+")
            .end_production()
        .end();
        
        CheckLexerErrorPolicy error_policy( LEXER_ERROR_SYMBOL_CONFLICT );
        ParserStateMachine parser_state_machine( grammar, nullptr, &error_policy );
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

        void prototype( const char** /*begin*/, const char* /*end*/, std::string* lexeme, const void** symbol ) const
        {
            SWEET_ASSERT( lexeme );
            SWEET_ASSERT( symbol );
            *symbol = *lexeme == "prototype" ? prototype_symbol_ : value_symbol_;
        }
    };
    
    TEST( LexerConflictResolution )
    {
        Grammar grammar;
        grammar.begin()
            .whitespace() ("[ \\t\\r\\n]*")
            .left() ("prototype") ("value") 
            .production( "target" )
                ("prototype") ('{') ("targets") ('}') [nil]
            .end_production()
            .production( "targets" )
                ("targets") ("target") [nil]
                ("targets") ("value") [nil]
                ("target") [nil]
                ("value") [nil]
                (nil) .precedence("prototype")
            .end_production()
            .production( "prototype" )
                ("[A-Za-z_][A-Za-z_0-9]*:prototype:")
            .end_production()
            .production( "value" )
                ("[A-Za-z_0-9\\./@:-]+")
            .end_production()
        .end();
        
        CheckParserErrorPolicy error_policy( PARSER_ERROR_NONE );
        ParserStateMachine parser_state_machine( grammar, &error_policy );
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
        Grammar grammar;
        grammar.begin()
            .left() ('+') ('-')
            .left() ('*') ('/')
            .none() ("integer")
            .production( "unit" )
                ("expr")
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
        
        CheckParserErrorPolicy error_policy( PARSER_ERROR_PARSE_TABLE_CONFLICT );
        ParserStateMachine parser_state_machine( grammar, &error_policy );
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
        Grammar grammar;
        grammar.begin()
            .left() ('+') 
            .left() ('-')
            .none() ("integer")
            .production( "unit" )
                ("expr")
            .end_production()
            .production( "expr" )
                ("expr") ('+') ("expr") .precedence('-') ["first"]
                ("expr") ('-') ("expr") .precedence('+') ["second"]
                ("integer") [nil]
            .end_production()
            .production( "integer" )
                ("[0-9]+") [nil]
            .end_production()
        .end();
        
        CheckParserErrorPolicy error_policy( PARSER_ERROR_PARSE_TABLE_CONFLICT );
        ParserStateMachine parser_state_machine( grammar, &error_policy );
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
    
    TEST( ErrorProcessing )
    {
        Grammar grammar;
        grammar.begin()
            .left() .error()
            .left() ('+')
            .production( "expr" )
                ("expr") ('+') ("expr") ["add"]
                ("expr") .error() ("expr") ["error"]
                ("integer") [nil]
            .end_production()
            .production( "integer" )
                ("[0-9]+")
            .end_production()
        .end();
        
        ParserStateMachine parser_state_machine( grammar );
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
        Grammar grammar;
        grammar.begin()
            .left() ("integer")
            .production( "unit" )
                ("statements")
            .end_production()
            .production( "statements" )
                ("statements") ("statement") [nil]
                ("statement") .precedence("integer") [nil]
                (nil) .precedence("integer") [nil]
            .end_production()
            .production( "statement" )
                ("integer") (';')
            .end_production()
            .production( "integer" )
                ("[0-9]+")
            .end_production()
        .end();

        PrintParserErrorPolicy error_policy;
        ParserStateMachine parser_state_machine( grammar, &error_policy );
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
