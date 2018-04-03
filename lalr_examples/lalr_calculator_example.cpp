
#include <stdio.h>
#include <stdarg.h>
#include <lalr/GrammarCompiler.hpp>
#include <lalr/Parser.ipp>
#include <string.h>

using namespace std;
using namespace lalr;

static int add( const ParserSymbol* symbol, const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[0].user_data() + start[2].user_data();
}

static int subtract( const ParserSymbol* symbol, const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[0].user_data() - start[2].user_data();
}

static int multiply( const ParserSymbol* symbol, const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[0].user_data() * start[2].user_data();
}

static int divide( const ParserSymbol* symbol, const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[0].user_data() / start[2].user_data();
}

static int compound( const ParserSymbol* symbol, const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[1].user_data();
}

static int integer( const ParserSymbol* symbol, const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return ::atoi( start[0].lexeme().c_str() );
}

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
        ( "add", &add )
        ( "subtract", &subtract )
        ( "multiply", &multiply )
        ( "divide", &divide )
        ( "compound", &compound )
        ( "integer", &integer )
    ;
    
    const char* input = "1 + 2 * (3 + 4) + 5";
    parser.parse( input, input + strlen(input) );
    SWEET_ASSERT( parser.accepted() );
    SWEET_ASSERT( parser.full() );
    SWEET_ASSERT( parser.user_data() == 20 );
}
