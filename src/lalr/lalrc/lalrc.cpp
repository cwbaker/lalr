//
// lalrc.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS

#include "dot.hpp"
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
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

using std::string;
using std::vector;
using std::count;
using namespace lalr;

static FILE* file_ = nullptr;
static int errors_ = 0;

static void error( const char* format, ... );
static void open( const char* filename );
static void close();
void write( const char* format, ... );
static void print_cxx_parser_state_machine( const ParserStateMachine* state_machine );
static void generate_cxx_parser_state_machine( const ParserStateMachine* state_machine );
static void generate_cxx_lexer_state_machine( const LexerStateMachine* lexer_state_machine, const char* prefix );
static string sanitize( const char* input );

int main( int argc, char** argv )
{
    string input;
    string output;
    bool print = false;
    bool help = false;
    bool dot = false;
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
        else if ( strcmp(argv[argi], "-d") == 0 || strcmp(argv[argi], "--dot") == 0 )
        {
            dot = true;
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
        printf( "-p|--print    Print state machine\n" );
        printf( "-d|--dot      Print state machine as GraphViz DOT graph\n" );
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
            error( "Stat failed on '%s' - result=%d\n", input.c_str(), result );
            return EXIT_FAILURE;
        }

        FILE* file = fopen( input.c_str(), "rb" );
        if ( !file )
        {
            error( "Opening '%s' to read failed - errno=%d\n", input.c_str(), errno );
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
            error( "Reading grammar from '%s' failed - read=%d\n", input.c_str(), int(read) );
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
        open( !output.empty() ? output.c_str() : nullptr );
        if ( print )
        {
            print_cxx_parser_state_machine( state_machine );
        }
        else if ( dot )
        {
            write_graphviz_dot( state_machine );
        }
        else
        {
            generate_cxx_parser_state_machine( state_machine );
        }
        close();
    }

    return errors_ == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void error( const char* format, ... )
{
    ++errors_;
    va_list args;
    va_start( args, format );
    vfprintf( stderr, format, args );
    va_end( args );
}

static void open( const char* filename )
{
    if ( filename )
    {
        close();
        file_ = fopen( filename, "wb" );
        if ( !file_ )
        {
            error( "Opening '%s' to write parser state machine failed - errno=%d\n", filename, errno );
        }
    }
    else
    {
        file_ = stdout;
    }
}

static void close()
{
    if ( file_ && file_ != stdout )
    {
        fclose( file_ );
    }
    file_ = nullptr;
}

void write( const char* format, ... )
{
    if ( file_ )
    {
        va_list args;
        va_start( args, format );
        int result = vfprintf( file_, format, args );
        va_end( args );
        if ( result < 0 )
        {
            error( "Writing output failed" );
            close();
        }
    }
}

void print_cxx_parser_state_machine( const ParserStateMachine* state_machine )
{
    const ParserState* states = state_machine->states;
    const ParserState* states_end = states + state_machine->states_size;
    for ( const ParserState* state = states; state != states_end; ++state )
    {
        write( "state %d:\n", state->index );
        const ParserTransition* transitions = state->transitions;
        const ParserTransition* transitions_end = transitions + state->length;
        for ( const ParserTransition* transition = transitions; transition != transitions_end; ++transition )
        {
            if ( transition->state )
            {
                write( "  %s -> state %d\n", transition->symbol->lexeme, transition->state->index );                
            }
            else
            {
                write( "  %s <- %s\n", transition->symbol->lexeme, transition->reduced_symbol->lexeme );
            }
        }
        write( "\n" );
    }
}

void generate_cxx_parser_state_machine( const ParserStateMachine* state_machine )
{
    write( "\n" );
    write( "#include <lalr/ParserStateMachine.hpp>\n" );
    write( "#include <lalr/ParserState.hpp>\n" );
    write( "#include <lalr/ParserTransition.hpp>\n" );
    write( "#include <lalr/ParserSymbol.hpp>\n" );
    write( "#include <lalr/ParserAction.hpp>\n" );
    write( "#include <lalr/LexerStateMachine.hpp>\n" );
    write( "#include <lalr/LexerState.hpp>\n" );
    write( "#include <lalr/LexerTransition.hpp>\n" );
    write( "#include <lalr/LexerAction.hpp>\n" );
    write( "\n" );
    write( "namespace\n" );
    write( "{\n" );
    write( "\n" );
    write( "using namespace lalr;\n" );
    write( "\n" );
    write( "extern const LexerAction lexer_actions [];\n" );
    write( "extern const LexerTransition lexer_transitions [];\n" );
    write( "extern const LexerState lexer_states [];\n" );
    write( "extern const LexerAction whitespace_lexer_actions [];\n" );
    write( "extern const LexerTransition whitespace_lexer_transitions [];\n" );
    write( "extern const LexerState whitespace_lexer_states [];\n" );
    write( "extern const ParserAction actions [];\n" );
    write( "extern const ParserSymbol symbols [];\n" );
    write( "extern const ParserTransition transitions [];\n" );
    write( "extern const ParserState states [];\n" );
    write( "\n" );

    write( "const ParserAction actions [] = \n" );
    write( "{\n" );
    const ParserAction* actions = state_machine->actions;
    const ParserAction* actions_end = actions + state_machine->actions_size;
    for ( const ParserAction* action = actions; action != actions_end; ++action )
    {
        write( "    {%d, \"%s\"},\n", 
            action->index, 
            action->identifier
        );
    }
    write( "    {-1, nullptr}\n" );
    write( "};\n" );
    write( "\n" );

    write( "const ParserSymbol symbols [] = \n" );
    write( "{\n" );
    const ParserSymbol* symbols = state_machine->symbols;
    const ParserSymbol* symbols_end = symbols + state_machine->symbols_size;
    for ( const ParserSymbol* symbol = symbols; symbol != symbols_end; ++symbol )
    {
        write( "    {%d, \"%s\", \"%s\", \"%s\", (SymbolType) %d},\n", 
            symbol->index, 
            symbol->identifier, 
            sanitize(symbol->lexeme).c_str(),
            sanitize(symbol->label).c_str(),
            symbol->type
        );
    }
    write( "    {-1, nullptr, nullptr, nullptr, (SymbolType) 0}\n" );
    write( "};\n" );
    write( "\n" );

    write( "const ParserTransition transitions [] = \n" );
    write( "{\n" );
    const ParserTransition* transitions = state_machine->transitions;
    const ParserTransition* transitions_end = transitions + state_machine->transitions_size;
    for ( const ParserTransition* transition = transitions; transition != transitions_end; ++transition )
    {
        if ( transition->reduced_symbol )
        {
            write( "    {&symbols[%d], nullptr, &symbols[%d], %d, %d, %d, %d},\n",
                transition->symbol ? transition->symbol->index : -1,
                transition->reduced_symbol->index,
                transition->reduced_length,
                transition->precedence,
                transition->action,
                transition->index
            );
        }
        else
        {
            write( "    {&symbols[%d], &states[%d], nullptr, %d, %d, %d, %d},\n",
                transition->symbol ? transition->symbol->index : -1,
                transition->state ? transition->state->index : -1,
                transition->reduced_length,
                transition->precedence,
                transition->action,
                transition->index
            );
        }
    }
    write( "    {nullptr, nullptr, nullptr, 0, 0, 0, -1}\n" );
    write( "};\n" );
    write( "\n" );

    write( "const ParserState states [] = \n" );
    write( "{\n" );
    const ParserState* states = state_machine->states;
    const ParserState* states_end = states + state_machine->states_size;
    for ( const ParserState* state = states; state != states_end; ++state )
    {
        write( "    {%d, %d, &transitions[%d], \"%s\"},\n",
            state->index,
            state->length,
            state->transitions->index,
            state->label
        );
    }
    write( "    {-1, 0, nullptr}\n" );
    write( "};\n" );
    write( "\n" );

    generate_cxx_lexer_state_machine( state_machine->lexer_state_machine, "lexer" );
    generate_cxx_lexer_state_machine( state_machine->whitespace_lexer_state_machine, "whitespace_lexer" );

    write( "const ParserStateMachine parser_state_machine = \n" );
    write( "{\n" );
    write( "    \"%s\",\n", state_machine->identifier );
    write( "    %d, // #actions\n", state_machine->actions_size );
    write( "    %d, // #symbols\n", state_machine->symbols_size );
    write( "    %d, // #transitions\n", state_machine->transitions_size );
    write( "    %d, // #states\n", state_machine->states_size );
    write( "    actions,\n" );
    write( "    symbols,\n" );
    write( "    transitions,\n" );
    write( "    states,\n" );
    write( "    &symbols[%d], // start symbol\n", state_machine->start_symbol->index );
    write( "    &symbols[%d], // end symbol\n", state_machine->end_symbol->index );
    write( "    &symbols[%d], // error symbol\n", state_machine->error_symbol->index );
    write( "    &symbols[%d], // whitespace symbol\n", state_machine->whitespace_symbol->index );
    write( "    &states[%d], // start state\n", state_machine->start_state->index );
    write( "    %s, // lexer state machine\n", state_machine->lexer_state_machine ? "&lexer_state_machine" : "null" );
    write( "    %s // whitespace lexer state machine\n", state_machine->whitespace_lexer_state_machine ? "&whitespace_lexer_state_machine" : "null" );
    write( "};\n" );

    write( "\n" );
    write( "}\n" );
    write( "\n" );

    write( "const lalr::ParserStateMachine* %s_parser_state_machine = &parser_state_machine;\n", state_machine->identifier );
}

void generate_cxx_lexer_state_machine( const LexerStateMachine* state_machine, const char* prefix )
{
    if ( state_machine )
    {
        write( "const LexerAction %s_actions [] = \n", prefix );
        write( "{\n" );
        const LexerAction* actions = state_machine->actions;
        const LexerAction* actions_end = actions + state_machine->actions_size;
        for ( const LexerAction* action = actions; action != actions_end; ++action )
        {
            write( "    {%d, \"%s\"},\n", 
                action->index, 
                action->identifier
            );
        }
        write( "    {-1, nullptr}\n" );
        write( "};\n" );
        write( "\n" );

        write( "const LexerTransition %s_transitions [] = \n", prefix );
        write( "{\n" );
        const LexerTransition* transitions = state_machine->transitions;
        const LexerTransition* transitions_end = transitions + state_machine->transitions_size;
        for ( const LexerTransition* transition = transitions; transition != transitions_end; ++transition )
        {
            write( "    {%d, %d, &%s_states[%d], ",
                transition->begin,
                transition->end,
                prefix,
                transition->state->index
            );
            if ( transition->action )
            {
                write( "&%s_actions[%d]},\n", prefix, transition->action->index );
            }
            else
            {
                write( "nullptr},\n" );
            }
        }
        write( "    {-1, -1, nullptr, nullptr}\n" );
        write( "};\n" );
        write( "\n" );

        write( "const LexerState %s_states [] = \n", prefix );
        write( "{\n" );
        const LexerState* states = state_machine->states;
        const LexerState* states_end = states + state_machine->states_size;
        for ( const LexerState* state = states; state != states_end; ++state )
        {
            write( "    {%d, %d, &%s_transitions[%d], ",
                state->index,
                state->length,
                prefix,
                int(state->transitions - state_machine->transitions)
            );
            const ParserSymbol* symbol = reinterpret_cast<const ParserSymbol*>( state->symbol );
            if ( symbol )
            {
                write( "&symbols[%d]},\n", symbol->index );
            }
            else
            {
                write( "nullptr},\n" );
            }
        }
        write( "    {-1, 0, nullptr, nullptr}\n" );
        write( "};\n" );
        write( "\n" );

        write( "const LexerStateMachine %s_state_machine = \n", prefix );
        write( "{\n" );
        write( "    %d, // #actions\n", state_machine->actions_size );
        write( "    %d, // #transitions\n", state_machine->transitions_size );
        write( "    %d, // #states\n", state_machine->states_size );
        write( "    %s_actions, // actions\n", prefix );
        write( "    %s_transitions, // transitions\n", prefix );
        write( "    %s_states, // states\n", prefix );
        write( "    &%s_states[%d] // start state\n", prefix, state_machine->start_state->index );
        write( "};\n" );
        write( "\n" );
    }
}

string sanitize( const char* input )
{
    size_t length = strlen( input );
    size_t backslashes = count( input, input + length, '\\' );

    string output;
    output.reserve( length + backslashes );
    for ( const char* i = input; i != input + length; ++i )
    {
        char character = *i;
        if (character == '\\' || character == '"')
        {
            output.push_back('\\');
        }
        output.push_back(character);
    }
    return output;
}
