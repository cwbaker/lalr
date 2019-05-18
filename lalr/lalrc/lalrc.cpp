//
// lalrc.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS

#include <lalr/GrammarCompiler.hpp>
#include <lalr/ParserStateMachine.hpp>
#include <lalr/ParserState.hpp>
#include <lalr/ParserTransition.hpp>
#include <lalr/ParserSymbol.hpp>
#include <lalr/ParserAction.hpp>
#include <lalr/RegexCompiler.hpp>
#include <lalr/LexerStateMachine.hpp>
#include <lalr/LexerState.hpp>
#include <lalr/LexerTransition.hpp>
#include <lalr/LexerAction.hpp>
#include <lalr/ErrorPolicy.hpp>
#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

using std::string;
using std::vector;
using namespace lalr;

static void print_cxx_parser_state_machine( const ParserStateMachine* state_machine, FILE* file );
static void generate_cxx_parser_state_machine( const ParserStateMachine* state_machine, FILE* file );
static void generate_cxx_lexer_state_machine( FILE* file, const LexerStateMachine* lexer_state_machine, const char* prefix );

int main( int argc, char** argv )
{
    string input;
    string output;
    bool print = false;
    bool help = false;
    bool version = false;

    int argi = 1;
    while ( argi < argc )
    {
        if ( argv[argi][0] != '-' )
        {
            input = argv[argi];
            argi += 1;
        }
        else if ( strcmp(argv[argi], "-o") == 0 || strcmp(argv[argi], "--output") == 0 )
        {
            output = argv[argi + 1];
            argi += 2;
        }
        else if ( strcmp(argv[argi], "-p") == 0 || strcmp(argv[argi], "--print") == 0 )
        {
            print = true;
            argi += 1;
        }
        else if ( strcmp(argv[argi], "-h") == 0 || strcmp(argv[argi], "--help") == 0 )
        {
            help = true;
            argi += 1;
        }
        else if ( strcmp(argv[argi], "-v") == 0 || strcmp(argv[argi], "--version") == 0 )
        {
            version = true;
            argi += 1;
        }
    }

    if ( version )
    {
        printf( "lalrc - LALR parser compiler\n" );
        printf( "Copyright (c) Charles Baker. All rights reserved.\n" );
    }

    if ( help || input.empty() )
    {
        printf( "lalrc [options] [-o|--output OUTPUT] INPUT\n" );
        printf( "-h|--help     Display this help message\n" );
        printf( "-v|--version  Display version\n" );
        printf( "-p|--print    Print parser state machine\n" );
        printf( "-o|--output   Output file\n" );
        printf( "\n" );
        return help ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    if ( !input.empty() )
    {
        struct stat stat;
        int result = ::stat( input.c_str(), &stat );
        if ( result != 0 )
        {
            fprintf( stderr, "Stat failed on '%s' - result=%d\n", input.c_str(), result );
            return EXIT_FAILURE;
        }

        FILE* file = fopen( input.c_str(), "rb" );
        if ( !file )
        {
            fprintf( stderr, "Opening '%s' to read failed - errno=%d\n", input.c_str(), errno );
            return EXIT_FAILURE;
        }

        int size = stat.st_size;
        vector<char> grammar_source;
        grammar_source.resize( size );
        int read = int( fread(&grammar_source[0], sizeof(uint8_t), size, file) );
        fclose( file );
        file = nullptr;
        if ( read != size )
        {
            fprintf( stderr, "Reading grammar from '%s' failed - read=%d\n", input.c_str(), int(read) );
            return EXIT_FAILURE;
        }

        GrammarCompiler compiler;
        ErrorPolicy error_policy;
        int errors = compiler.compile( &grammar_source[0], &grammar_source[0] + grammar_source.size(), &error_policy );
        if ( errors != 0 )            
        {
            return EXIT_FAILURE;
        }

        const ParserStateMachine* state_machine = compiler.parser_state_machine();
        if ( !output.empty() )
        {
            file = fopen( output.c_str(), "wb" );
            if ( !file )
            {
                fprintf( stderr, "Opening '%s' to write parser state machine failed - errno=%d\n", output.c_str(), errno );
                return EXIT_FAILURE;
            }
        }
        else
        {
            file = stdout;
        }

        if ( print )
        {
            print_cxx_parser_state_machine( state_machine, file );
        }
        else
        {
            generate_cxx_parser_state_machine( state_machine, file );
        }

        if ( file != stdout )
        {
            fclose( file );            
        }
        file = nullptr;
    }

    return EXIT_SUCCESS;
}

void print_cxx_parser_state_machine( const ParserStateMachine* state_machine, FILE* file )
{
    const ParserState* states = state_machine->states;
    const ParserState* states_end = states + state_machine->states_size;
    for ( const ParserState* state = states; state != states_end; ++state )
    {
        fprintf( file, "state %d:\n", state->index );
        const ParserTransition* transitions = state->transitions;
        const ParserTransition* transitions_end = transitions + state->length;
        for ( const ParserTransition* transition = transitions; transition != transitions_end; ++transition )
        {
            if ( transition->state )
            {
                fprintf( file, "  %s -> state %d\n", transition->symbol->lexeme, transition->state->index );                
            }
            else
            {
                fprintf( file, "  %s <- %s\n", transition->symbol->lexeme, transition->reduced_symbol->lexeme );
            }
        }
        fprintf( file, "\n" );
    }
}

void generate_cxx_parser_state_machine( const ParserStateMachine* state_machine, FILE* file )
{
    fprintf( file, "\n" );
    fprintf( file, "#include <lalr/ParserStateMachine.hpp>\n" );
    fprintf( file, "#include <lalr/ParserState.hpp>\n" );
    fprintf( file, "#include <lalr/ParserTransition.hpp>\n" );
    fprintf( file, "#include <lalr/ParserSymbol.hpp>\n" );
    fprintf( file, "#include <lalr/ParserAction.hpp>\n" );
    fprintf( file, "#include <lalr/LexerStateMachine.hpp>\n" );
    fprintf( file, "#include <lalr/LexerState.hpp>\n" );
    fprintf( file, "#include <lalr/LexerTransition.hpp>\n" );
    fprintf( file, "#include <lalr/LexerAction.hpp>\n" );
    fprintf( file, "\n" );
    fprintf( file, "using namespace lalr;\n" );
    fprintf( file, "\n" );
    fprintf( file, "namespace\n" );
    fprintf( file, "{\n" );
    fprintf( file, "\n" );
    fprintf( file, "extern const LexerAction lexer_actions [];\n" );
    fprintf( file, "extern const LexerTransition lexer_transitions [];\n" );
    fprintf( file, "extern const LexerState lexer_states [];\n" );
    fprintf( file, "extern const LexerAction whitespace_lexer_actions [];\n" );
    fprintf( file, "extern const LexerTransition whitespace_lexer_transitions [];\n" );
    fprintf( file, "extern const LexerState whitespace_lexer_states [];\n" );
    fprintf( file, "extern const ParserAction actions [];\n" );
    fprintf( file, "extern const ParserSymbol symbols [];\n" );
    fprintf( file, "extern const ParserTransition transitions [];\n" );
    fprintf( file, "extern const ParserState states [];\n" );
    fprintf( file, "\n" );

    fprintf( file, "const ParserAction actions [] = \n" );
    fprintf( file, "{\n" );
    const ParserAction* actions = state_machine->actions;
    const ParserAction* actions_end = actions + state_machine->actions_size;
    for ( const ParserAction* action = actions; action != actions_end; ++action )
    {
        fprintf( file, "    {%d, \"%s\"},\n", 
            action->index, 
            action->identifier
        );
    }
    fprintf( file, "    {-1, nullptr}\n" );
    fprintf( file, "};\n" );
    fprintf( file, "\n" );

    fprintf( file, "const ParserSymbol symbols [] = \n" );
    fprintf( file, "{\n" );
    const ParserSymbol* symbols = state_machine->symbols;
    const ParserSymbol* symbols_end = symbols + state_machine->symbols_size;
    for ( const ParserSymbol* symbol = symbols; symbol != symbols_end; ++symbol )
    {
        fprintf( file, "    {%d, \"%s\", \"%s\", (SymbolType) %d},\n", 
            symbol->index, 
            symbol->identifier, 
            symbol->lexeme,
            symbol->type
        );
    }
    fprintf( file, "    {-1, nullptr, nullptr, (SymbolType) 0}\n" );
    fprintf( file, "};\n" );
    fprintf( file, "\n" );

    fprintf( file, "const ParserTransition transitions [] = \n" );
    fprintf( file, "{\n" );
    const ParserTransition* transitions = state_machine->transitions;
    const ParserTransition* transitions_end = transitions + state_machine->transitions_size;
    for ( const ParserTransition* transition = transitions; transition != transitions_end; ++transition )
    {
        if ( transition->reduced_symbol )
        {
            fprintf( file, "    {&symbols[%d], nullptr, &symbols[%d], %d, %d, %d, (TransitionType) %d, %d},\n",
                transition->symbol ? transition->symbol->index : -1,
                transition->reduced_symbol->index,
                transition->reduced_length,
                transition->precedence,
                transition->action,
                transition->type,
                transition->index
            );
        }
        else
        {
            fprintf( file, "    {&symbols[%d], &states[%d], nullptr, %d, %d, %d, (TransitionType) %d, %d},\n",
                transition->symbol ? transition->symbol->index : -1,
                transition->state ? transition->state->index : -1,
                transition->reduced_length,
                transition->precedence,
                transition->action,
                transition->type,
                transition->index
            );
        }
    }
    fprintf( file, "    {nullptr, nullptr, nullptr, 0, 0, 0, (TransitionType) 0, -1}\n" );
    fprintf( file, "};\n" );
    fprintf( file, "\n" );

    fprintf( file, "const ParserState states [] = \n" );
    fprintf( file, "{\n" );
    const ParserState* states = state_machine->states;
    const ParserState* states_end = states + state_machine->states_size;
    for ( const ParserState* state = states; state != states_end; ++state )
    {
        fprintf( file, "    {%d, %d, &transitions[%d]},\n",
            state->index,
            state->length,
            state->transitions->index 
        );
    }
    fprintf( file, "    {-1, 0, nullptr}\n" );
    fprintf( file, "};\n" );
    fprintf( file, "\n" );

    generate_cxx_lexer_state_machine( file, state_machine->lexer_state_machine, "lexer" );
    generate_cxx_lexer_state_machine( file, state_machine->whitespace_lexer_state_machine, "whitespace_lexer" );

    fprintf( file, "const ParserStateMachine parser_state_machine = \n" );
    fprintf( file, "{\n" );
    fprintf( file, "    \"%s\",\n", state_machine->identifier );
    fprintf( file, "    %d, // #actions\n", state_machine->actions_size );
    fprintf( file, "    %d, // #symbols\n", state_machine->symbols_size );
    fprintf( file, "    %d, // #transitions\n", state_machine->transitions_size );
    fprintf( file, "    %d, // #states\n", state_machine->states_size );
    fprintf( file, "    actions,\n" );
    fprintf( file, "    symbols,\n" );
    fprintf( file, "    transitions,\n" );
    fprintf( file, "    states,\n" );
    fprintf( file, "    &symbols[%d], // start symbol\n", state_machine->start_symbol->index );
    fprintf( file, "    &symbols[%d], // end symbol\n", state_machine->end_symbol->index );
    fprintf( file, "    &symbols[%d], // error symbol\n", state_machine->error_symbol->index );
    fprintf( file, "    &states[%d], // start state\n", state_machine->start_state->index );
    fprintf( file, "    %s, // lexer state machine\n", state_machine->lexer_state_machine ? "&lexer_state_machine" : "null" );
    fprintf( file, "    %s // whitespace lexer state machine\n", state_machine->whitespace_lexer_state_machine ? "&whitespace_lexer_state_machine" : "null" );
    fprintf( file, "};\n" );

    fprintf( file, "\n" );
    fprintf( file, "}\n" );
    fprintf( file, "\n" );

    fprintf( file, "const ParserStateMachine* %s_parser_state_machine = &parser_state_machine;\n", state_machine->identifier );
    fprintf( file, "\n" );
}

void generate_cxx_lexer_state_machine( FILE* file, const LexerStateMachine* state_machine, const char* prefix )
{
    if ( state_machine )
    {
        fprintf( file, "const LexerAction %s_actions [] = \n", prefix );
        fprintf( file, "{\n" );
        const LexerAction* actions = state_machine->actions;
        const LexerAction* actions_end = actions + state_machine->actions_size;
        for ( const LexerAction* action = actions; action != actions_end; ++action )
        {
            fprintf( file, "    {%d, \"%s\"},\n", 
                action->index, 
                action->identifier
            );
        }
        fprintf( file, "    {-1, nullptr}\n" );
        fprintf( file, "};\n" );
        fprintf( file, "\n" );

        fprintf( file, "const LexerTransition %s_transitions [] = \n", prefix );
        fprintf( file, "{\n" );
        const LexerTransition* transitions = state_machine->transitions;
        const LexerTransition* transitions_end = transitions + state_machine->transitions_size;
        for ( const LexerTransition* transition = transitions; transition != transitions_end; ++transition )
        {
            fprintf( file, "    {%d, %d, &%s_states[%d], ",
                transition->begin,
                transition->end,
                prefix,
                transition->state->index
            );
            if ( transition->action )
            {
                fprintf( file, "&%s_actions[%d]},\n", prefix, transition->action->index );
            }
            else
            {
                fprintf( file, "nullptr},\n" );
            }
        }
        fprintf( file, "    {-1, -1, nullptr, nullptr}\n" );
        fprintf( file, "};\n" );
        fprintf( file, "\n" );

        fprintf( file, "const LexerState %s_states [] = \n", prefix );
        fprintf( file, "{\n" );
        const LexerState* states = state_machine->states;
        const LexerState* states_end = states + state_machine->states_size;
        for ( const LexerState* state = states; state != states_end; ++state )
        {
            fprintf( file, "    {%d, %d, &%s_transitions[%d], ",
                state->index,
                state->length,
                prefix,
                int(state->transitions - state_machine->transitions)
            );
            const ParserSymbol* symbol = reinterpret_cast<const ParserSymbol*>( state->symbol );
            if ( symbol )
            {
                fprintf( file, "&symbols[%d]},\n", symbol->index );
            }
            else
            {
                fprintf( file, "nullptr},\n" );
            }
        }
        fprintf( file, "    {-1, 0, nullptr, nullptr}\n" );
        fprintf( file, "};\n" );
        fprintf( file, "\n" );

        fprintf( file, "const LexerStateMachine %s_state_machine = \n", prefix );
        fprintf( file, "{\n" );
        fprintf( file, "    %d, // #actions\n", state_machine->actions_size );
        fprintf( file, "    %d, // #transitions\n", state_machine->transitions_size );
        fprintf( file, "    %d, // #states\n", state_machine->states_size );
        fprintf( file, "    %s_actions, // actions\n", prefix );
        fprintf( file, "    %s_transitions, // transitions\n", prefix );
        fprintf( file, "    %s_states, // states\n", prefix );
        fprintf( file, "    &%s_states[%d] // start state\n", prefix, state_machine->start_state->index );
        fprintf( file, "};\n" );
        fprintf( file, "\n" );
    }
}
