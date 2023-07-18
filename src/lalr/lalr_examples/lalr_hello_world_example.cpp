
#include <lalr/GrammarCompiler.hpp>
#include <lalr/Parser.ipp>
#include <stdio.h>
#include <string.h>

using namespace std;
using namespace lalr;

void lalr_hello_world_example()
{
    const char* hello_world_grammar =
        "hello_world {\n"
        "   %whitespace \"[ \\t\\r\\n]\" ;\n"
        "   hello_world: 'Hello World!' [hello_world];\n"
        "}\n"
    ;

    GrammarCompiler compiler;
    compiler.compile( hello_world_grammar, hello_world_grammar + strlen(hello_world_grammar) );
    Parser<const char*> parser( compiler.parser_state_machine() );
    parser.parser_action_handlers()
        ( "hello_world", [] (shared_ptr<ParserUserData<char>>& result,
					const shared_ptr<ParserUserData<char>>* data,
					const ParserNode<>* nodes, size_t length, const ParserTransition */*transition*/)
            {
                printf( "Hello World!\n" );
                return true;
            }
        );
    ;

    const char* input = "Hello World!";
    parser.parse( input, input + strlen(input) );
    LALR_ASSERT( parser.accepted() );
    LALR_ASSERT( parser.full() );
}
