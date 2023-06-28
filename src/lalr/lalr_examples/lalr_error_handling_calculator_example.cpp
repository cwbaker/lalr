
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
        ( "add", [] ( int& result, const int* data, const ParserNode<>* nodes, size_t length )
            {
                result = data[0] + data[2];
		return true;
            }
        )
        ( "subtract", [] ( int& result, const int* data, const ParserNode<>* nodes, size_t length )
            {
                result = data[0] - data[2];
		return true;
            }
        )
        ( "multiply", [] ( int& result, const int* data, const ParserNode<>* nodes, size_t length )
            {
                result = data[0] * data[2];
		return true;
            }
        )
        ( "divide", [] ( int& result, const int* data, const ParserNode<>* nodes, size_t length )
            {
                result = data[0] / data[2];
		return true;
            }
        )
        ( "compound", [] ( int& result, const int* data, const ParserNode<>* nodes, size_t length )
            {
                result = data[1];
		return true;
            }
        )
        ( "integer", [] ( int& result, const int* data, const ParserNode<>* nodes, size_t length )
            {
                result = ::atoi( nodes[0].lexeme().c_str() );
		return true;
            }
        )
        ( "result", [&error] ( int& result, const int* data, const ParserNode<>* nodes, size_t length )
            {
                if ( !error )
                {
                    printf( "%d\n", data[0] );
                }
                error = false;
                result = data[0];
		return true;
            }
        )
        ( "unexpected_error", [&error] ( int& result, const int* data, const ParserNode<>* nodes, size_t length )
            {
                error = true;
                printf( "Unexpected error!\n" );
                result = 0;
		return true;
            }
        )
        ( "unknown_operator_error", [&error] ( int& result, const int* data, const ParserNode<>* nodes, size_t length )
            {
                error = true;
                printf( "Unknown operator error!\n" );
                result = 0;
		return true;
            }
        )
    ;

    const char* input = "1 + 2 * (3 + 4) + 5; 2 ^ 3; 3 * 4; foo!;";
    parser.parse( input, input + strlen(input) );
    LALR_ASSERT( parser.accepted() );
    LALR_ASSERT( parser.full() );
}
