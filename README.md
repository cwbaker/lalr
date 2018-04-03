# lalr

*lalr* is a modern LALR(1) parser and parser generator for C++.

## Features

- LALR(1) parser generation from a BNF grammar.
- Separate grammar compilation step is optional.
- Lexer actions for escape character conversion.
- Lexer actions for feedback from the parser to the lexer.
- Bind parser and lexer actions to `std::tr1::function` objects.
- Lexical tokens can be specified inline in the grammar.
- Re-entrant and thread-safe.

## Example

~~~c++
#include <stdio.h>
#include <stdarg.h>
#include <sweet/lalr/ParserStateMachine.hpp>
#include <sweet/lalr/Parser.ipp>
#include <sweet/lalr/Grammar.hpp>
#include <string.h>

using namespace std;
using namespace sweet;
using namespace sweet::lalr;

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
~~~

## Installation

Use *lalr* in your own project by adding the *lalr* source to your build scripts or IDE project files and compiling as C++.

*lalr* is built for development using [Sweet Build](https://www.github.com/cwbaker/sweet_build#sweet-build) and a C++ compiler (XCode, Visual C++, or GCC depending on operating system).

**macOS**

- Install Xcode
- Install [Sweet Build](https://github.com/cwbaker/sweet_build#sweet-build)
- `git clone git@github.com:cwbaker/lalr.git lalr`
- `cd lalr/src`
- `build variant=release`
- `./release/bin/lalr_examples`

**Windows**:

- Install Visual Studio 2017
- Install [Sweet Build](https://github.com/cwbaker/sweet_build#sweet-build)
- `git clone git@github.com:cwbaker/lalr.git lalr`
- `cd lalr\src`
- `build variant=release`
- `.\release\bin\lalr_examples.exe`

## Usage

*lalr* is a C++ library that generates LALR(1) parsers from a BNF grammar expressed as a C++ string and built at run-time or stored in a file and built at build-time.  

Generate parsers at run-time as follows:

1. Create a grammar and embed it as a string in your C++ program taking care to escape backslashes and other C++ escape characters correctly.

2. Create a `GrammarCompiler` object and call `GrammarCompiler::compile()` to compile the grammar into a state machine.

3. Create one or more `Parser` objects from the state machine returned by `GrammarCompiler::parser_state_machine()`.

4. Pass input to the `Parser` objects as an iterator range to have the parser use its built-in lexer and process the input all at once.  Pass input as separate pairs of symbols and lexemes to drive the parser using an external lexer.

5. Implement the `ErrorPolicy` interface to be notified of errors and optional debug output during parser generation and parsing.

Generate parsers at build-time using the `lalrc` tool:

1. Create a grammar and save it to a text file.

2. Run `lalrc` to compile the grammar into a state machine in a generated C++ source file and compile and link/archive it into your executable/library.

3. Create one or more `Parser` objects from the state machine referenced by name as an external symbol.

4. Pass input to the `Parser` objects as an iterator range to have the parser use its built-in lexer and process the input all at once.  Pass input as separate pairs of symbols and lexemes to drive the parser using an external lexer.

5. Implement the `ErrorPolicy` interface to be notified of errors and optional debug output during parsing.

## License

*lalr* is licensed under the [MIT License](http://www.opensource.org/licenses/MIT).
