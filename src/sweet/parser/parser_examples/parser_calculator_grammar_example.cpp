
#include <stdio.h>
#include <stdarg.h>
#include <sweet/parser/ParserStateMachine.hpp>
#include <sweet/parser/Parser.ipp>
#include <sweet/parser/Grammar.hpp>
#include <string.h>

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
    Grammar grammar;
    grammar
        .left() ('+') ('-')
        .left() ('*') ('/')
        .none() ("integer")
        .whitespace() (regex("[ \t\r\n]*"))
        .production( "expr" )
            ("expr") ('+') ("expr") ["add"]
            ("expr") ('-') ("expr") ["subtract"]
            ("expr") ('*') ("expr") ["multiply"]
            ("expr") ('/') ("expr") ["divide"]
            ('(') ("expr") (')') ["compound"]
            ("integer") ["integer"]
        .end_production()
        .production( "integer" )
            (regex("[0-9]+"))
        .end_production()
    ;

    ParserStateMachine parser_state_machine( grammar.parser_grammar() );
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
