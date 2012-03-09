
#include <sweet/parser/ParserStateMachine.hpp>
#include <sweet/parser/Parser.ipp>
#include <stdio.h>

using namespace std;
using namespace sweet;
using namespace sweet::parser;

static ptr<ParserUserData<char>> hello_world( const ParserSymbol* symbol, const ParserNode<>* start, const ParserNode<>* finish )
{
    printf( "Hello World!\n" );
    return ptr<ParserUserData<char>>();
}

void parser_hello_world_example()
{
    const char* grammar = 
        "hello_world {\n"
        "   hello_world: 'Hello World!' [hello_world];\n"
        "}"
    ;

    ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
    Parser<const char*> parser( &parser_state_machine );
    parser.parser_action_handlers()
        ( "hello_world", &hello_world )
    ;
    
    const char* input = "Hello World!";
    parser.parse( input, input + strlen(input) );
    SWEET_ASSERT( parser.accepted() );
    SWEET_ASSERT( parser.full() );
}
