
#include <stdio.h>
#include <stdarg.h>
#include <lalr/GrammarCompiler.hpp>
#include <lalr/Parser.ipp>
#include <string.h>

using namespace std;
using namespace lalr;

void lalr_error_handling_calculator_example()
{
    bool error = false;
    extern const lalr::ParserStateMachine* error_handling_calculator_parser_state_machine;
    Parser<const char*, int> parser( error_handling_calculator_parser_state_machine );
    parser.parser_action_handlers()
        ( "add", [] ( const int* data, const ParserNode<>* nodes, size_t length )
            {
                return data[0] + data[2];
            } 
        )
        ( "subtract", [] ( const int* data, const ParserNode<>* nodes, size_t length )
            {
                return data[0] - data[2];
            }
        )
        ( "multiply", [] ( const int* data, const ParserNode<>* nodes, size_t length )
            {
                return data[0] * data[2];
            } 
        )
        ( "divide", [] ( const int* data, const ParserNode<>* nodes, size_t length )
            {
                return data[0] / data[2];
            } 
        )
        ( "compound", [] ( const int* data, const ParserNode<>* nodes, size_t length )
            {
                return data[1];
            }
        )
        ( "integer", [] ( const int* data, const ParserNode<>* nodes, size_t length )
            {
                return ::atoi( nodes[0].lexeme().c_str() );
            } 
        )
        ( "result", [&error] ( const int* data, const ParserNode<>* nodes, size_t length )
            {
                if ( !error )
                {
                    printf( "%d\n", data[0] );
                }
                error = false;
                return data[0];
            }
        )
        ( "unexpected_error", [&error] ( const int* data, const ParserNode<>* nodes, size_t length )
            {
                error = true;
                printf( "Unexpected error!\n" );
                return 0;
            }
        )
        ( "unknown_operator_error", [&error] ( const int* data, const ParserNode<>* nodes, size_t length )
            {
                error = true;
                printf( "Unknown operator error!\n" );
                return 0;
            }
        )
    ;

    const char* input = "1 + 2 * (3 + 4) + 5; 2 ^ 3; 3 * 4; foo!;";
    parser.parse( input, input + strlen(input) );
    LALR_ASSERT( parser.accepted() );
    LALR_ASSERT( parser.full() );
}
