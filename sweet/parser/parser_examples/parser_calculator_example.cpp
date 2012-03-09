
#include <stdio.h>
#include <stdarg.h>
#include <sweet/parser/ParserStateMachine.hpp>
#include <sweet/parser/Parser.ipp>

using namespace std;
using namespace sweet;
using namespace sweet::parser;

static int add( const ParserSymbol* symbol, const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[0].get_user_data() + start[2].get_user_data();
}

static int subtract( const ParserSymbol* symbol, const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[0].get_user_data() - start[2].get_user_data();
}

static int multiply( const ParserSymbol* symbol, const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[0].get_user_data() * start[2].get_user_data();
}

static int divide( const ParserSymbol* symbol, const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[0].get_user_data() / start[2].get_user_data();
}

static int compound( const ParserSymbol* symbol, const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[1].get_user_data();
}

static int integer( const ParserSymbol* symbol, const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return ::atoi( start[0].get_lexeme().c_str() );
}

void parser_calculator_example()
{
    const char* grammar = 
        "calculator {\n"
        "   %left '+' '-';\n"
        "   %left '*' '/';\n"
        "   %none integer;\n"
        "   %whitespace \"[ \t\r\n]*\";\n"
        "\n"
        "   expr: expr '+' expr [add]\n"
        "       | expr '-' expr [subtract]\n"
        "       | expr '*' expr [multiply]\n"
        "       | expr '/' expr [divide]\n"
        "       | '(' expr ')' [compound]\n"
        "       | integer [integer]\n"
        "       ;\n"
        "\n"
        "   integer: \"[0-9]+\";\n"
        "}"
    ;

    ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
    Parser<const char*, int> parser( &parser_state_machine );
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
