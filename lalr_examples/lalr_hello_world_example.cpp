
#include <sweet/lalr/GrammarCompiler.hpp>
#include <sweet/lalr/Parser.ipp>
#include <stdio.h>
#include <string.h>

using namespace std;
using namespace sweet;
using namespace sweet::lalr;

static shared_ptr<ParserUserData<char> > hello_world( const ParserSymbol* /*symbol*/, const ParserNode<>* /*start*/, const ParserNode<>* /*finish*/ )
{
    printf( "Hello World!\n" );
    return shared_ptr<ParserUserData<char> >();
}

void lalr_hello_world_example()
{
    const char* hello_world_grammar = 
        "hello_world {\n"
        "   hello_world: 'Hello World!' [hello_world];\n"
        "}\n"
    ;
    GrammarCompiler compiler;
    compiler.compile( hello_world_grammar, hello_world_grammar + strlen(hello_world_grammar) );
    Parser<const char*> parser( compiler.parser_state_machine() );
    parser.parser_action_handlers()
        ( "hello_world", &hello_world )
    ;
    
    const char* input = "Hello World!";
    parser.parse( input, input + strlen(input) );
    SWEET_ASSERT( parser.accepted() );
    SWEET_ASSERT( parser.full() );
}
