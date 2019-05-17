//
// TestParsers.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include <lalr/Parser.ipp>
#include <lalr/ParserStateMachine.hpp>
#include <lalr/ErrorCode.hpp>
#include <lalr/GrammarCompiler.hpp>
#include <lalr/ErrorPolicy.hpp>
#include <functional>
#include <UnitTest++/UnitTest++.h>
#include <string.h>

using std::bind;
using namespace std::placeholders;
using namespace lalr;

SUITE( Parsers )
{
    static const ParserSymbol* find_symbol_by_identifier( const ParserStateMachine* parser_state_machine, const char* identifier )
    {
        LALR_ASSERT( parser_state_machine );
        LALR_ASSERT( identifier );
        const ParserSymbol* symbol = parser_state_machine->symbols;
        const ParserSymbol* symbols_end = parser_state_machine->symbols + parser_state_machine->symbols_size;
        while ( symbol != symbols_end && strcmp(symbol->identifier, identifier) != 0 )
        {
            ++symbol;
        }    
        return symbol != symbols_end ? symbol : nullptr ;
    }

    class IgnoreParserErrorPolicy : public ErrorPolicy
    {
        public:
            void parser_error( int /*line*/, int /*error*/, const char* /*format*/, va_list /*args*/ )
            {
            }
    };

    struct PrintParserErrorPolicy : public ErrorPolicy     
    {
        int errors;

        PrintParserErrorPolicy()
        : errors( 0 )
        {
        }

        void lalr_vprintf( const char* format, va_list args )
        {
            vprintf( format, args );
        }
    
        void lalr_error( int line, int /*error*/, const char* format, va_list args )
        {
            char message [1024];
            vsnprintf( message, sizeof(message), format, args );
            printf( "(%d): error: %s.\n", line, message );
            ++errors;
        }
    };

    struct CheckParserErrorPolicy : public ErrorPolicy
    {    
        int expected_error;
        int errors;            
        
        CheckParserErrorPolicy( int expected_error )
        : expected_error( expected_error ),
          errors( 0 )
        {
        }

        void lalr_error( int /*line*/, int error, const char* /*format*/, va_list /*args*/ )
        {
            (void) error;
            ++errors;
            CHECK( error == expected_error );
        }
    };
    
    struct CheckLexerErrorPolicy : public ErrorPolicy
    {    
        int expected_error;
        int errors;
        
        CheckLexerErrorPolicy( int expected_error )
        : expected_error( expected_error ),
          errors( 0 )
        {
        }

        void lalr_error( int /*line*/, int error, const char* /*format*/, va_list /*args*/ )
        {
            (void) error;
            ++errors;
            CHECK( error == expected_error );
        }
    };
        
    TEST( OrOperator )
    {
        const char* or_grammar = 
            "OrOperator {\n"
            "   unit: one | two | three;\n"
            "   one: '1';\n"
            "   two: '2';\n"
            "   three: '3';\n"
            "}\n"
        ;

        GrammarCompiler compiler;
        compiler.compile( or_grammar, or_grammar + strlen(or_grammar) );        
        Parser<const char*> parser( compiler.parser_state_machine() );
    
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
        const char* alternate_grammar =
            "Alternate {\n"
            "   unit: one two_three;\n"
            "   two_three: two | three;\n"
            "   one: '1';\n"
            "   two: '2';\n"
            "   three: '3';\n"
            "}\n"
        ;

        GrammarCompiler compiler;
        compiler.compile( alternate_grammar, alternate_grammar + strlen(alternate_grammar) );
        Parser<const char*> parser( compiler.parser_state_machine() );

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
        const char* zero_to_many_repeats_grammar = 
            "ZeroToManyRepeats {\n"
            "   %left two; \n"
            "   unit: one twos three;\n"
            "   twos: twos two | two | %precedence two;\n"
            "   one: '1';\n"
            "   two: '2';\n"
            "   three: '3';\n"
            "}"
        ;

        GrammarCompiler compiler;
        compiler.compile( zero_to_many_repeats_grammar, zero_to_many_repeats_grammar + strlen(zero_to_many_repeats_grammar) );
        Parser<const char*> parser( compiler.parser_state_machine() );

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
        const char* one_to_many_repeats_grammar =
            "OneToManyRepeats {\n"
            "   unit: one twos three;\n"
            "   twos: twos two | two;\n"
            "   one: '1';\n"
            "   two: '2';\n"
            "   three: '3';\n"
            "}"
        ;

        GrammarCompiler compiler;
        compiler.compile( one_to_many_repeats_grammar, one_to_many_repeats_grammar + strlen(one_to_many_repeats_grammar) );
        Parser<const char*> parser( compiler.parser_state_machine() );

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
        const char* optional_grammar =         
            "Optional {\n"            
            "    unit: one two_opt three;\n"
            "    two_opt: two | ;\n"
            "    one: '1';\n"
            "    two: '2';\n"
            "    three: '3';\n"
            "}"
        ;

        GrammarCompiler compiler;
        compiler.compile( optional_grammar, optional_grammar + strlen(optional_grammar) );
        Parser<const char*> parser( compiler.parser_state_machine() );

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
        const char* compound_grammar =
            "Compound {\n"
            "    compound: one one_two three;\n"
            "    one_two: one two | two one;\n"
            "    one: '1';\n"
            "    two: '2';\n"
            "    three: '3';\n"
            "}"
        ;

        GrammarCompiler compiler;
        compiler.compile( compound_grammar, compound_grammar + strlen(compound_grammar) );
        Parser<const char*> parser( compiler.parser_state_machine() );

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
        const char* binary_operator_grammar =
            "BinaryOperator {\n"
            "    E: E '+' T | T;\n"
            "    T: T '*' F | F;\n"
            "    F: '(' E ')' | i;\n"
            "    i: \"[0-9]+\";\n"
            "}"
        ;

        ErrorPolicy error_policy;
        GrammarCompiler compiler;
        compiler.compile( binary_operator_grammar, binary_operator_grammar + strlen(binary_operator_grammar), &error_policy );
        Parser<const char*> parser( compiler.parser_state_machine() );

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
        const char* nested_productions_grammar =
            "NestedProductions {\n"
            "    %left 'b' 'c';\n"
            "    A: 'a' bcs 'd';\n"
            "    bcs: bcs bc | bc | %precedence 'b';\n"
            "    bc: 'b' 'c';\n"
            "}"
        ;

        GrammarCompiler compiler;
        compiler.compile( nested_productions_grammar, nested_productions_grammar + strlen(nested_productions_grammar) );
        Parser<const char*> parser( compiler.parser_state_machine() );

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
        const char* follow_generator_grammar =
            "FollowGeneration {\n"
            "    unit: one two four | one three four;\n"
            "    one: '1';\n"
            "    two: '2';\n"
            "    three: '3';\n"
            "    four: '4';\n"
            "}"
        ;

        GrammarCompiler compiler;
        compiler.compile( follow_generator_grammar, follow_generator_grammar + strlen(follow_generator_grammar) );
        Parser<const char*> parser( compiler.parser_state_machine() );

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
        const char* canonical_grammar =
            "Canonical {\n"
            "    S: C C;\n"
            "    C: 'c' C | 'd';\n"
            "}"
        ;

        ErrorPolicy error_policy;
        GrammarCompiler compiler;
        compiler.compile( canonical_grammar, canonical_grammar + strlen(canonical_grammar), &error_policy );
        Parser<const char*> parser( compiler.parser_state_machine() );

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
        const char* multiple_dot_nodes_grammar =
            "MultipleDotNodesParser {\n"
            "    unit: lt | lt question;\n"
            "    lt: '<';\n"
            "    question: '?';\n"
            "}"
        ;

        GrammarCompiler compiler;
        compiler.compile( multiple_dot_nodes_grammar, multiple_dot_nodes_grammar + strlen(multiple_dot_nodes_grammar) );
        Parser<const char*> parser( compiler.parser_state_machine() );

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
        const char* productions_on_collapsed_symbols_grammar =
            "ProductionsOnCollapsedSymbols {\n"
            "    unit: 'a' [unit];\n"
            "}"
        ;
        GrammarCompiler compiler;
        compiler.compile( productions_on_collapsed_symbols_grammar, productions_on_collapsed_symbols_grammar + strlen(productions_on_collapsed_symbols_grammar) );
        Parser<const char*> parser( compiler.parser_state_machine() );
        
        const char* input = "a";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
    }
    
    TEST( ReduceStarNode )
    {   
        const char* reduce_star_node_grammar =
            "ReduceStarNode {\n"
            "   %left one;\n"
            "   unit: ones;\n"
            "   ones: ones one | one | %precedence one;\n"
            "   one: '1';\n"
            "}"
        ;

        GrammarCompiler compiler;
        compiler.compile( reduce_star_node_grammar, reduce_star_node_grammar + strlen(reduce_star_node_grammar) );
        Parser<const char*> parser( compiler.parser_state_machine() );
        
        const char* input = "1";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        
        input = "111";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
    }

    TEST( ReduceParenthesis )
    {
        const char* reduce_parenthesis_grammar =
            "ReduceParenthesis {\n"
            "    expr: '(' expr ')' | '1';\n"
            "}"
        ;

        GrammarCompiler compiler;
        compiler.compile( reduce_parenthesis_grammar, reduce_parenthesis_grammar + strlen(reduce_parenthesis_grammar) );
        Parser<const char*> parser( compiler.parser_state_machine() );
        
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
        const char* reduce_star_and_parenthesis_grammar =
            "ReduceStarAndParenthesis {\n"
            "   %left '(' '1';\n"
            "   expr: '(' exprs ')' | '1';\n"
            "   exprs: exprs expr | expr | %precedence '(';\n"
            "}"
        ;

        GrammarCompiler compiler;
        compiler.compile( reduce_star_and_parenthesis_grammar, reduce_star_and_parenthesis_grammar + strlen(reduce_star_and_parenthesis_grammar) );
        Parser<const char*> parser( compiler.parser_state_machine() );
        
        const char* input = "(1)";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );

        input = "(111)";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );
    }
    
    TEST( Whitespace )
    {
        const char* whitespace_grammar =
            "Whitespace {\n"
            "   %whitespace \"[ \\t\\r\\n]*\";\n"
            "   unit: identifiers;\n"
            "   identifiers: identifiers identifier\n"
            "              | identifier\n"
            "              ;\n"
            "   identifier: \"[A-Za-z_][A-Za-z_0-9]*\";\n"
            "}"
        ;

        GrammarCompiler compiler;
        compiler.compile( whitespace_grammar, whitespace_grammar + strlen(whitespace_grammar) );
        Parser<const char*> parser( compiler.parser_state_machine() );
        
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
            static void string_lexer( const char* begin, const char* end, std::string* lexeme, const void** /*symbol*/, const char** position, int* lines )
            {
                LALR_ASSERT( begin );
                LALR_ASSERT( end );
                LALR_ASSERT( begin <= end );
                LALR_ASSERT( lexeme );
                LALR_ASSERT( position );
                LALR_ASSERT( lines );

                lexeme->clear();

                const char* i = begin;
                while ( i != end && *i != '\'' )
                {
                    *lexeme += *i;
                    ++i;
                }
                
                if ( i != end )
                {
                    LALR_ASSERT( *i == '\'' );
                    ++i;
                }
                
                *position = i;
                *lines = 0;
            }
        };
    
        const char* string_grammar = 
            "String { \n"
            "   %whitespace \"[ \\t\\r\\n]*\";\n"
            "   unit: strings;\n"
            "   strings: strings string | string;\n"
            "   string: \"':string:\";\n"
            "}"
        ;

        GrammarCompiler compiler;
        compiler.compile( string_grammar, string_grammar + strlen(string_grammar) );
        Parser<const char*> parser( compiler.parser_state_machine() );
        parser.lexer_action_handlers()
            ( "string", &StringLexer::string_lexer )
        ;            

        const char* input = "'first' 'second' 'third'";
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );

        parser.set_lexer_action_handler( "string", nullptr );
        parser.set_lexer_action_handler( "string", &StringLexer::string_lexer );
        parser.parse( input, input + strlen(input) );
        CHECK( parser.accepted() );
        CHECK( parser.full() );
    }
        
    TEST( LineCommentInWhitespaceDirective )
    {
        struct LineComment
        {
            static void line_comment( const char* begin, const char* end, std::string* lexeme, const void** /*symbol*/, const char** position, int* lines )
            {
                LALR_ASSERT( begin );
                LALR_ASSERT( end );
                LALR_ASSERT( begin <= end );
                LALR_ASSERT( lexeme );
                LALR_ASSERT( position );
                LALR_ASSERT( lines );
                
                lexeme->clear();
                
                const char* i = begin;
                while ( i != end && *i != '\n' && *i != '\r' )
                {
                    ++i;
                }
                
                *position = i;
                *lines = 1;
            }
        };
    
        const char* line_comment_grammar = 
            "LineComment {\n"
            "   %whitespace \"([ \\t\\r\\n]|\\/\\/:line_comment:)*\";\n"
            "   unit: digits;\n"
            "   digits: digits digit | digit;\n"
            "   digit: \"[0-9]\";\n"
            "}"
        ;

        GrammarCompiler compiler;
        compiler.compile( line_comment_grammar, line_comment_grammar + strlen(line_comment_grammar) );
        Parser<const char*> parser( compiler.parser_state_machine() );
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

    TEST( BlockCommentInWhitespaceDirective )
    {
        struct BlockComment
        {
            static void block_comment( const char* begin, const char* end, std::string* lexeme, const void** /*symbol*/, const char** position, int* lines )
            {
                LALR_ASSERT( begin );
                LALR_ASSERT( end );
                LALR_ASSERT( begin <= end );
                LALR_ASSERT( lexeme );
                LALR_ASSERT( position );
                LALR_ASSERT( lines );

                lexeme->clear();
                                
                bool done = false;
                const char* i = begin;
                while ( i != end && !done )
                {
                    while ( i != end && *i != '*' )
                    {
                        ++i;
                    }

                    if ( i != end )
                    {
                        LALR_ASSERT( *i == '*' );

                        ++i;
                        if ( *i == '/' )
                        {
                            ++i;
                            done = true;
                        }
                    }        
                }
                
                *position = i;
                *lines = 0;
            }
        };
    
        const char* block_comment_grammar = 
            "BlockComment {\n"
            "   %whitespace \"([ \\t\\r\\n]|\\/\\*:block_comment:)*\";\n"
            "   unit: digits;\n"
            "   digits: digits digit | digit;\n"
            "   digit: \"[0-9]\";\n"
            "}"
        ;

        GrammarCompiler compiler;
        compiler.compile( block_comment_grammar, block_comment_grammar + strlen(block_comment_grammar) );
        Parser<const char*> parser( compiler.parser_state_machine() );
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

    /*
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
            CHECK( error_policy.errors == 1 );
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
            CHECK( error_policy.errors == 2 );
            CHECK( parser_state_machine.start_state() == NULL );
            CHECK( parser_state_machine.states().empty() );
        }
    }    
    */

    TEST( SyntaxErrorsInRegularExpressions )
    {
        const char* syntax_errors_in_regular_expressions_grammar = 
            "SyntaxErrorsInRegularExpressions {\n"
            "   %whitespace \"[ \\t\\r\\n*\";\n"
            "   one: \"[A-z*\";\n"
            "}"
        ;

        CheckLexerErrorPolicy error_policy( LEXER_ERROR_SYNTAX );
        GrammarCompiler compiler;
        compiler.compile( 
            syntax_errors_in_regular_expressions_grammar, 
            syntax_errors_in_regular_expressions_grammar + strlen(syntax_errors_in_regular_expressions_grammar),
            &error_policy
        );
        CHECK( error_policy.errors == 2 );
    }

    TEST( UndefinedSymbolError )
    {
        const char* undefined_symbol_grammar = 
            "UndefinedSymbolError {\n"
            "   %whitespace \"[ \\t\\r\\n]*\";\n"
            "   one: undefined_symbol;\n"
            "}"
        ;

        CheckParserErrorPolicy error_policy( PARSER_ERROR_UNDEFINED_SYMBOL );
        GrammarCompiler compiler;
        compiler.compile( undefined_symbol_grammar, undefined_symbol_grammar + strlen(undefined_symbol_grammar), &error_policy );
        CHECK( error_policy.errors == 1 );        
    }

    TEST( UnreferencedSymbolError )
    {
        const char* unreferenced_symbol_error_grammar = 
            "UnreferencedSymbolError {\n"
            "   one: 'one';\n"
            "   unreferenced_symbol: 'two';\n"
            "}"
        ;

        CheckParserErrorPolicy error_policy( PARSER_ERROR_UNREFERENCED_SYMBOL );
        GrammarCompiler compiler;
        compiler.compile( 
            unreferenced_symbol_error_grammar, 
            unreferenced_symbol_error_grammar + strlen(unreferenced_symbol_error_grammar),
            &error_policy
        );
        CHECK( error_policy.errors == 1 );
    }
    
    TEST( SymbolsOnlyAppearingInPrecedenceDirectivesAreCountedAsReferenced )
    {
        const char* precedence_directive_symbols_grammar = 
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

        GrammarCompiler compiler;
        CheckParserErrorPolicy error_policy( PARSER_ERROR_NONE );
        compiler.compile( precedence_directive_symbols_grammar, precedence_directive_symbols_grammar + strlen(precedence_directive_symbols_grammar), &error_policy );
        CHECK( error_policy.errors == 0 );
    }
        
    TEST( LexerConflict )
    {
        const char* lexer_conflict_grammar = 
            "LexerConflictError {\n"
            "   %whitespace \"[ \\t\\r\\n]*\";\n"
            "   %left prototype value;\n"
            "   target: prototype '{' targets '}';\n"
            "   targets: targets target | targets value | target | value | %precedence prototype;\n"
            "   prototype: \"[A-Za-z_][A-Za-z_0-9]*\"; value: \"[A-Za-z_0-9\\./@:-]+\";\n"
            "}"
        ;
        CheckLexerErrorPolicy error_policy( LEXER_ERROR_SYMBOL_CONFLICT );
        GrammarCompiler compiler;
        compiler.compile( lexer_conflict_grammar, lexer_conflict_grammar + strlen(lexer_conflict_grammar), &error_policy );
        CHECK( error_policy.errors == 1 );
    }

    struct LexerConflictResolutionActionHandler
    {
        const void* prototype_symbol_;
        const void* value_symbol_;

        LexerConflictResolutionActionHandler( const ParserStateMachine* parser_state_machine )
        : prototype_symbol_( find_symbol_by_identifier(parser_state_machine, "prototype") ),
          value_symbol_( find_symbol_by_identifier(parser_state_machine, "value") )
        {
            LALR_ASSERT( prototype_symbol_ );
            LALR_ASSERT( value_symbol_ );
        }

        void prototype( const char* /*begin*/, const char* /*end*/, std::string* lexeme, const void** symbol, const char** /*position*/, int* /*lines*/ ) const
        {
            LALR_ASSERT( lexeme );
            LALR_ASSERT( symbol );
            *symbol = *lexeme == "prototype" ? prototype_symbol_ : value_symbol_;
        }
    };
    
    TEST( LexerConflictResolution )
    {
        const char* lexer_conflict_grammar = 
            "LexerSymbolConflictResolvedByAction {\n"
            "   %whitespace \"[ \\t\\r\\n]*\";\n"
            "   %left prototype value;\n"
            "   target: prototype '{' targets '}';\n"
            "   targets: targets target | targets value | target | value | %precedence prototype;\n"
            "   prototype: \"[A-Za-z_][A-Za-z0-9_]*:prototype:\";\n"
            "   value: \"[A-Za-z0-9_\\./@:-]+\";\n"
            "}"
        ;

        GrammarCompiler compiler;
        CheckParserErrorPolicy error_policy( PARSER_ERROR_NONE );
        compiler.compile( lexer_conflict_grammar, lexer_conflict_grammar + strlen(lexer_conflict_grammar), &error_policy );
        CHECK( error_policy.errors == 0 );

        if ( error_policy.errors == 0 )
        {
            Parser<const char*> parser( compiler.parser_state_machine() );
            LexerConflictResolutionActionHandler action_handler( compiler.parser_state_machine() );
            parser.lexer_action_handlers()
                ( "prototype", bind(&LexerConflictResolutionActionHandler::prototype, &action_handler, _1, _2, _3, _4, _5, _6) )
            ;

            const char* input = "prototype { prototype { value value } value value }";
            parser.parse( input, input + strlen(input) );
            CHECK( parser.accepted() );
            CHECK( parser.full() );
        }
    }
        
    TEST( AssociativityDirectives )
    {
        const char* associativity_grammar = 
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
        GrammarCompiler compiler;
        compiler.compile( associativity_grammar, associativity_grammar + strlen(associativity_grammar), &error_policy );
        CHECK( error_policy.errors == 0 );

        if ( error_policy.errors == 0 )
        {
            const char* input = "1+2";
            Parser<const char*> parser( compiler.parser_state_machine() );
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
        const char* precedence_grammar = 
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

        GrammarCompiler compiler;
        CheckParserErrorPolicy error_policy( PARSER_ERROR_PARSE_TABLE_CONFLICT );
        compiler.compile( precedence_grammar, precedence_grammar + strlen(precedence_grammar), &error_policy );
        CHECK( error_policy.errors == 0 );

        const ParserSymbol* expr = find_symbol_by_identifier( compiler.parser_state_machine(), "expr" );
        CHECK( expr );
        if ( expr )
        {
            // CHECK( expr->get_productions()[0]->get_precedence() == 2 );
            // CHECK( expr->get_productions()[1]->get_precedence() == 1 );
            // CHECK( expr->get_productions()[2]->get_precedence() == 3 );
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
        
        GrammarCompiler compiler;
        CheckParserErrorPolicy error_policy( PARSER_ERROR_ERROR_SYMBOL_ON_LEFT_HAND_SIDE );
        compiler.compile( grammar, grammar + strlen(grammar), &error_policy );
        CHECK( error_policy.errors == 1 );             
    }
        
    TEST( ErrorProcessing )
    {
        const char* error_processing_grammar =
            "ErrorProcessing {"
            "   %left error;\n"
            "   %left '+';\n"
            "   expr: expr '+' expr [add] | expr error expr [error] | integer;\n"
            "   integer: \"[0-9]+\";\n"
            "}"
        ;

        GrammarCompiler compiler;
        compiler.compile( error_processing_grammar, error_processing_grammar + strlen(error_processing_grammar) );
        const char* input = "1+2";
        Parser<const char*> parser( compiler.parser_state_machine() );
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
        const char* empty_production_grammar =
            "EmptyProduction { \n"
            "   %left integer; \n"
            "   unit: statements; \n"
            "   statements: statements statement | statement %precedence integer | %precedence integer; \n"
            "   statement: integer ';'; \n"
            "   integer: \"[0-9]+\"; \n"
            "}"
        ;
        
        PrintParserErrorPolicy error_policy;
        GrammarCompiler compiler;
        compiler.compile( empty_production_grammar, empty_production_grammar + strlen(empty_production_grammar), &error_policy );

        const char* input = "1;2;3;";
        Parser<const char*> parser( compiler.parser_state_machine() );
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

    TEST( LineComment )
    {
        const char* line_comment_grammar =
            "// Line comment LF \n"
            "// Line comment ending with CR \r"
            "// Line comment LF CR \n\r"
            "// Line comment CR LF \r\n"
            "LineComment { \n"
            "   unit: line_comment_example; \n"
            "   line_comment_example: 'LineCommentExample'; // Line comment at the end of a valid line \n"
            "} \n"
            "// Unterminated line comment"
        ;
        
        PrintParserErrorPolicy error_policy;
        GrammarCompiler compiler;
        compiler.compile( line_comment_grammar, line_comment_grammar + strlen(line_comment_grammar), &error_policy );
        CHECK(error_policy.errors == 0);
    }

    TEST( BlockComment )
    {
        const char* block_comment_grammar =
            "/* Block comment before grammar... \n"
            "...that spans several lines... \n"
            "*/ \n"
            "BlockComment /* Block comment between tokens */ { \n"
            "   unit: block_comment_example; /* Block comment at the end of a line */ \n"
            "   block_comment_example: /* Another block comment between tokens */ 'BlockCommentExample'; // Line comment at the end of a valid line \n"
            "} \n"
            "/* Block comment at the end of input */"
        ;
        
        PrintParserErrorPolicy error_policy;
        GrammarCompiler compiler;
        compiler.compile( block_comment_grammar, block_comment_grammar + strlen(block_comment_grammar), &error_policy );
        CHECK( error_policy.errors == 0 );

        const char* unterminated_block_comment_grammar = 
            "BlockComment { \n"
            "   unit: 'BlockCommentExample'; \n"
            "} \n"
            "/* Unterminated block comment... \n"
        ;
        compiler.compile( unterminated_block_comment_grammar, unterminated_block_comment_grammar + strlen(unterminated_block_comment_grammar), &error_policy );
        CHECK( error_policy.errors == 0 );
    }
}
