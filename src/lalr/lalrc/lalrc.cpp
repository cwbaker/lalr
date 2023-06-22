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

        // Enable state labels only when generating Graphviz DOT graphs
        // because generating so many strings takes lots of time and memory.
        GrammarCompiler compiler;
        if ( dot )
        {
            compiler.labels_enabled( true );
        }

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

void writeToOutput( const char* format, ... )
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
        writeToOutput( "state %d:\n", state->index );
        const ParserTransition* transitions = state->transitions;
        const ParserTransition* transitions_end = transitions + state->length;
        for ( const ParserTransition* transition = transitions; transition != transitions_end; ++transition )
        {
            if ( transition->state )
            {
                writeToOutput( "  %s -> state %d\n", transition->symbol->lexeme, transition->state->index );
            }
            else
            {
                writeToOutput( "  %s <- %s\n", transition->symbol->lexeme, transition->reduced_symbol->lexeme );
            }
        }
        writeToOutput( "\n" );
    }
}

void generate_cxx_parser_state_machine( const ParserStateMachine* state_machine )
{
    writeToOutput( "\n" );
    writeToOutput( "#include <lalr/ParserStateMachine.hpp>\n" );
    writeToOutput( "#include <lalr/ParserState.hpp>\n" );
    writeToOutput( "#include <lalr/ParserTransition.hpp>\n" );
    writeToOutput( "#include <lalr/ParserSymbol.hpp>\n" );
    writeToOutput( "#include <lalr/ParserAction.hpp>\n" );
    writeToOutput( "#include <lalr/LexerStateMachine.hpp>\n" );
    writeToOutput( "#include <lalr/LexerState.hpp>\n" );
    writeToOutput( "#include <lalr/LexerTransition.hpp>\n" );
    writeToOutput( "#include <lalr/LexerAction.hpp>\n" );
    writeToOutput( "\n" );
    writeToOutput( "namespace\n" );
    writeToOutput( "{\n" );
    writeToOutput( "\n" );
    writeToOutput( "using namespace lalr;\n" );
    writeToOutput( "\n" );
    writeToOutput( "extern const LexerAction lexer_actions [];\n" );
    writeToOutput( "extern const LexerTransition lexer_transitions [];\n" );
    writeToOutput( "extern const LexerState lexer_states [];\n" );
    writeToOutput( "extern const LexerAction whitespace_lexer_actions [];\n" );
    writeToOutput( "extern const LexerTransition whitespace_lexer_transitions [];\n" );
    writeToOutput( "extern const LexerState whitespace_lexer_states [];\n" );
    writeToOutput( "extern const ParserAction actions [];\n" );
    writeToOutput( "extern const ParserSymbol symbols [];\n" );
    writeToOutput( "extern const ParserTransition transitions [];\n" );
    writeToOutput( "extern const ParserState states [];\n" );
    writeToOutput( "\n" );

    writeToOutput( "const ParserAction actions [] = \n" );
    writeToOutput( "{\n" );
    const ParserAction* actions = state_machine->actions;
    const ParserAction* actions_end = actions + state_machine->actions_size;
    for ( const ParserAction* action = actions; action != actions_end; ++action )
    {
        writeToOutput( "    {%d, \"%s\"},\n",
            action->index,
            action->identifier
        );
    }
    writeToOutput( "    {-1, nullptr}\n" );
    writeToOutput( "};\n" );
    writeToOutput( "\n" );

    writeToOutput( "const ParserSymbol symbols [] = \n" );
    writeToOutput( "{\n" );
    const ParserSymbol* symbols = state_machine->symbols;
    const ParserSymbol* symbols_end = symbols + state_machine->symbols_size;
    for ( const ParserSymbol* symbol = symbols; symbol != symbols_end; ++symbol )
    {
        writeToOutput( "    {%d, (SymbolType) %d, \"%s\", \"%s\", \"%s\"},\n",
            symbol->index,
            symbol->type,
            symbol->identifier,
            sanitize(symbol->lexeme).c_str(),
            sanitize(symbol->label).c_str()
        );
    }
    writeToOutput( "    {-1, (SymbolType) 0, nullptr, nullptr, nullptr}\n" );
    writeToOutput( "};\n" );
    writeToOutput( "\n" );

    writeToOutput( "const ParserTransition transitions [] = \n" );
    writeToOutput( "{\n" );
    const ParserTransition* transitions = state_machine->transitions;
    const ParserTransition* transitions_end = transitions + state_machine->transitions_size;
    for ( const ParserTransition* transition = transitions; transition != transitions_end; ++transition )
    {
        if ( transition->reduced_symbol )
        {
            writeToOutput( "    {&symbols[%d], nullptr, &symbols[%d], %d, %d, %d, %d},\n",
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
            writeToOutput( "    {&symbols[%d], &states[%d], nullptr, %d, %d, %d, %d},\n",
                transition->symbol ? transition->symbol->index : -1,
                transition->state ? transition->state->index : -1,
                transition->reduced_length,
                transition->precedence,
                transition->action,
                transition->index
            );
        }
    }
    writeToOutput( "    {nullptr, nullptr, nullptr, 0, 0, 0, -1}\n" );
    writeToOutput( "};\n" );
    writeToOutput( "\n" );

    writeToOutput( "const ParserState states [] = \n" );
    writeToOutput( "{\n" );
    const ParserState* states = state_machine->states;
    const ParserState* states_end = states + state_machine->states_size;
    for ( const ParserState* state = states; state != states_end; ++state )
    {
        writeToOutput( "    {%d, %d, &transitions[%d], \"%s\"},\n",
            state->index,
            state->length,
            state->transitions->index,
            state->label ? sanitize( state->label ).c_str() : nullptr
        );
    }
    writeToOutput( "    {-1, 0, nullptr}\n" );
    writeToOutput( "};\n" );
    writeToOutput( "\n" );

    generate_cxx_lexer_state_machine( state_machine->lexer_state_machine, "lexer" );
    generate_cxx_lexer_state_machine( state_machine->whitespace_lexer_state_machine, "whitespace_lexer" );

    writeToOutput( "const ParserStateMachine parser_state_machine = \n" );
    writeToOutput( "{\n" );
    writeToOutput( "    \"%s\",\n", state_machine->identifier );
    writeToOutput( "    %d, // #actions\n", state_machine->actions_size );
    writeToOutput( "    %d, // #symbols\n", state_machine->symbols_size );
    writeToOutput( "    %d, // #transitions\n", state_machine->transitions_size );
    writeToOutput( "    %d, // #states\n", state_machine->states_size );
    writeToOutput( "    %d, // #shift_reduce_count\n", state_machine->shift_reduce_count_ );
    writeToOutput( "    %d, // #reduce_reduce_count\n", state_machine->reduce_reduce_count_ );
    writeToOutput( "    actions,\n" );
    writeToOutput( "    symbols,\n" );
    writeToOutput( "    transitions,\n" );
    writeToOutput( "    states,\n" );
    writeToOutput( "    &symbols[%d], // start symbol\n", state_machine->start_symbol->index );
    writeToOutput( "    &symbols[%d], // end symbol\n", state_machine->end_symbol->index );
    writeToOutput( "    &symbols[%d], // error symbol\n", state_machine->error_symbol->index );
    writeToOutput( "    &symbols[%d], // whitespace symbol\n", state_machine->whitespace_symbol->index );
    writeToOutput( "    &states[%d], // start state\n", state_machine->start_state->index );
    writeToOutput( "    %s, // lexer state machine\n", state_machine->lexer_state_machine ? "&lexer_state_machine" : "null" );
    writeToOutput( "    %s // whitespace lexer state machine\n", state_machine->whitespace_lexer_state_machine ? "&whitespace_lexer_state_machine" : "null" );
    writeToOutput( "};\n" );

    writeToOutput( "\n" );
    writeToOutput( "}\n" );
    writeToOutput( "\n" );

    writeToOutput( "const lalr::ParserStateMachine* %s_parser_state_machine = &parser_state_machine;\n", state_machine->identifier );
}

void generate_cxx_lexer_state_machine( const LexerStateMachine* state_machine, const char* prefix )
{
    if ( state_machine )
    {
        writeToOutput( "const LexerAction %s_actions [] = \n", prefix );
        writeToOutput( "{\n" );
        const LexerAction* actions = state_machine->actions;
        const LexerAction* actions_end = actions + state_machine->actions_size;
        for ( const LexerAction* action = actions; action != actions_end; ++action )
        {
            writeToOutput( "    {%d, \"%s\"},\n",
                action->index,
                action->identifier
            );
        }
        writeToOutput( "    {-1, nullptr}\n" );
        writeToOutput( "};\n" );
        writeToOutput( "\n" );

        writeToOutput( "const LexerTransition %s_transitions [] = \n", prefix );
        writeToOutput( "{\n" );
        const LexerTransition* transitions = state_machine->transitions;
        const LexerTransition* transitions_end = transitions + state_machine->transitions_size;
        for ( const LexerTransition* transition = transitions; transition != transitions_end; ++transition )
        {
            writeToOutput( "    {%d, %d, &%s_states[%d], ",
                transition->begin,
                transition->end,
                prefix,
                transition->state->index
            );
            if ( transition->action )
            {
                writeToOutput( "&%s_actions[%d]},\n", prefix, transition->action->index );
            }
            else
            {
                writeToOutput( "nullptr},\n" );
            }
        }
        writeToOutput( "    {-1, -1, nullptr, nullptr}\n" );
        writeToOutput( "};\n" );
        writeToOutput( "\n" );

        writeToOutput( "const LexerState %s_states [] = \n", prefix );
        writeToOutput( "{\n" );
        const LexerState* states = state_machine->states;
        const LexerState* states_end = states + state_machine->states_size;
        for ( const LexerState* state = states; state != states_end; ++state )
        {
            writeToOutput( "    {%d, %d, &%s_transitions[%d], ",
                state->index,
                state->length,
                prefix,
                int(state->transitions - state_machine->transitions)
            );
            const ParserSymbol* symbol = reinterpret_cast<const ParserSymbol*>( state->symbol );
            if ( symbol )
            {
                writeToOutput( "&symbols[%d]},\n", symbol->index );
            }
            else
            {
                writeToOutput( "nullptr},\n" );
            }
        }
        writeToOutput( "    {-1, 0, nullptr, nullptr}\n" );
        writeToOutput( "};\n" );
        writeToOutput( "\n" );

        writeToOutput( "const LexerStateMachine %s_state_machine = \n", prefix );
        writeToOutput( "{\n" );
        writeToOutput( "    %d, // #actions\n", state_machine->actions_size );
        writeToOutput( "    %d, // #transitions\n", state_machine->transitions_size );
        writeToOutput( "    %d, // #states\n", state_machine->states_size );
        writeToOutput( "    %s_actions, // actions\n", prefix );
        writeToOutput( "    %s_transitions, // transitions\n", prefix );
        writeToOutput( "    %s_states, // states\n", prefix );
        writeToOutput( "    &%s_states[%d] // start state\n", prefix, state_machine->start_state->index );
        writeToOutput( "};\n" );
        writeToOutput( "\n" );
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
