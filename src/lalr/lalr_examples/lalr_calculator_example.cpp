
#include <stdio.h>
#include <stdarg.h>
#include <lalr/GrammarCompiler.hpp>
#include <lalr/Parser.ipp>
#include <string.h>

using namespace std;
using namespace lalr;

void lalr_calculator_example()
{
    const char* calculator_grammar =
        "calculator { \n"
        "   %left '+' '-'; \n"
        "   %left '*' '/'; \n"
        "   %none integer; \n"
        "   %whitespace \"[ \t\r\n]*\"; \n"
        "   expr: \n"
        "      expr '+' expr [add] | \n"
        "      expr '-' expr [subtract] | \n"
        "      expr '*' expr [multiply] | \n"
        "      expr '/' expr [divide] | \n"
        "      '(' expr ')' [compound] | \n"
        "      integer [integer] \n"
        "   ; \n"
        "   integer: \"[0-9]+\"; \n"
        "} \n"
    ;

    GrammarCompiler compiler;
    compiler.compile( calculator_grammar, calculator_grammar + strlen(calculator_grammar) );
    Parser<const char*, int> parser( compiler.parser_state_machine() );
    parser.parser_action_handlers()
        ( "add", [] ( int &result, const int* data, const ParserNode<>* nodes, size_t length, const ParserTransition */*transition*/ )
            {
                result = data[0] + data[2];
		return true;
            }
        )
        ( "subtract", [] (  int &result, const int* data, const ParserNode<>* nodes, size_t length, const ParserTransition */*transition*/ )
            {
                result = data[0] - data[2];
		return true;
            }
        )
        ( "multiply", [] (  int &result, const int* data, const ParserNode<>* nodes, size_t length, const ParserTransition */*transition*/ )
            {
                result = data[0] * data[2];
		return true;
            }
        )
        ( "divide", [] (  int &result, const int* data, const ParserNode<>* nodes, size_t length, const ParserTransition */*transition*/ )
            {
                result = data[0] / data[2];
		return true;
            }
        )
        ( "compound", [] (  int &result, const int* data, const ParserNode<>* nodes, size_t length, const ParserTransition */*transition*/ )
            {
                result = data[1];
		return true;
            }
        )
        ( "integer", [] (  int &result, const int* data, const ParserNode<>* nodes, size_t length, const ParserTransition */*transition*/ )
            {
                result = ::atoi( nodes[0].lexeme().c_str() );
		return true;
            }
        )
    ;

    const char* input = "1 + 2 * (3 + 4) + 5";
    parser.parse( input, input + strlen(input) );
    printf( "1 + 2 * (3 + 4) + 5 = %d\n", parser.user_data() );
    LALR_ASSERT( parser.accepted() );
    LALR_ASSERT( parser.full() );
    LALR_ASSERT( parser.user_data() == 20 );
}
