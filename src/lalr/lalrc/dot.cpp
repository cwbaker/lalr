
#include <lalr/ParserStateMachine.hpp>
#include <lalr/ParserState.hpp>
#include <lalr/ParserTransition.hpp>
#include <lalr/ParserSymbol.hpp>
#include <lalr/ParserAction.hpp>
#include <lalr/assert.hpp>

using namespace lalr;

extern void write( const char* format, ... );

void write_graphviz_dot( const ParserStateMachine* state_machine )
{
    LALR_ASSERT( state_machine );

    write( "digraph %s {\n", state_machine->identifier );
    write( "  rankdir = LR;\n" );
    write( "  size = \"8, 5\";\n" );
    write( "  bgcolor = transparent;\n" );
    write( "\n" );
    write( "  node [shape = circle];\n" );

    const ParserState* states = state_machine->states;
    const ParserState* states_end = states + state_machine->states_size;
    for ( const ParserState* state = states; state != states_end; ++state )
    {
        write( "  node [label =\"%s\"] i%d;\n",
            state->label,
            state->index
        );
    }
    write( "\n" );

    for ( const ParserState* state = states; state != states_end; ++state )
    {
        const ParserTransition* transitions = state->transitions;
        const ParserTransition* transitions_end = transitions + state->length;
        for ( const ParserTransition* transition = transitions; transition != transitions_end; ++transition )
        {
            if ( transition->state )
            {
                write( "  i%d -> i%d [label =\"%s\"];\n",
                    state->index,
                    transition->state->index,
                    transition->symbol->label
                );
            }
        }
    }

    write( "}\n" );
}
