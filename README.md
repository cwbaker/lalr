# Sweet LALR

*Sweet LALR* is a modern LALR(1) parser and parser generator.

Features:

- LALR(1) parser generation from a BNF grammar DSL in C++.
- No separate generation step required.
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

Use *Sweet LALR* in your own project by adding the source from the [lalr](https:/www.github.com/cwbaker/lalr) repository to your build scripts or IDE project files and compiling as C++.

*Sweet LALR* is built for development using [Sweet Build](https:/www.github.com/cwbaker/sweet_build) and a C++ compiler (XCode, Visual C++, or GCC depending on operating system).

**macOS**

- Install Xcode
- Install [Sweet Build](https:/www.github.com/cwbaker/sweet_build)
- `git clone git@github.com:cwbaker/sweet-lalr.git sweet-lalr`
- `cd sweet-lalr/src`
- `build variant=release`
- `../release/bin/lalr_examples`

**Windows**:

- Install Visual Studio 2017
- Install [Sweet Build](https:/www.github.com/cwbaker/sweet_build)
- `git clone git@github.com:cwbaker/sweet-lalr.git sweet-lalr`
- `cd sweet-lalr\src`
- `build variant=release`
- `..\release\bin\lalr_examples.exe`

## Usage

*Sweet LALR* is a C++ library that generates LALR(1) parsers from a BNF grammar expressed as using a C++ DSL.  High level usage is as follows:

1. Create a `Grammar` object to represent the grammar of the language to be parser.  Use the DSL that it provides to define the symbols and productions that recursively define the language.

2. Create a `ParserStateMachine` object from the grammar created in the first step to generate a state machine that defines how to parse the defined language.

3. Create one or more `Parser` objects from the `ParserStateMachine` generated in the second step, attach custom actions to productions to transform the parser output into a syntax tree.  

4. Pass input to the `Parser` objects as an iterator range to have the parser use its built-in lexer and process the input all at once.  Pass input as separate pairs of symbols and lexemes to drive the parser using an external lexer.

5. Implement the `ParserErrorPolicy` interface to be notified of errors and optionally provide debug output during parser generation and parsing.

### Grammar

- Literals and regular expressions
- Precedence and associativity 
- Whitespace
- Productions
- Terminal and non-terminal symbols
- Errors

A grammar is built from one or more productions that recursively define how the symbols that make up a language can appear in relation to each other in valid input.  

Each production has a single symbol (the left-hand side) and one or more patterns (the right-hand sides).  Each pattern is an ordered list of one or more symbols and an action.

During parsing the parser shifts symbols from the input onto a stack until the symbols at the top of the stack match one of the patterns listed in a production in the grammar.  The matching symbols on the stack are removed from the stack and replaced by the single symbol on the left-hand side of the production.  This is the reduce step of the parser.

This process repeats until the right-hand side reduces to the start symbol specified on the left-hand side of the first production in the grammar.

Actions attached to productions allow user supplied code to be executed when a reduction occurs.  An action is attached to a production by specifying an identifier between `[` and `]` at the end of the production.   The identifier is used by user code to attach an arbitrary function to be executed when that production is reduced.  The action function is passed the symbol on the left-hand side and the elements on the parser's stack that make up the right-hand side.  The return value of the action function is stored in the element of the parser's stack that results from the reduction so that it can be accessed later by other actions.

The action also serves as the end marker for the expression on the right hand side of a production.  Productions that have no specific action must be terminated with an empty, nil action (e.g. `[sweet::lalr::nil]` or `[nil]`).

Ambiguous grammars fail to generate parsers.  Ambiguity can be resolved by rearranging the grammar or by specifying the precedence and associativity of the symbols and productions that are involved in the conflict.

Syntax errors that occur during parsing are handled by backtracking until a special `error` symbol can be accepted, accepting that symbol, and then continuing the parse.  If there is no `error` symbol specified in the grammar or the parser backtracks all the way back to the start of the input then parsing fails.

### Symbols

There is no explicit distinction made between terminal and non-terminal symbols.  Symbols that appear on the left-hand side of productions are non-terminals and all other symbols are terminal symbols.

Additionally productions that have a right-hand side containing only a single terminal symbol (expressed as a literal or a regular expression) and an empty action are assumed to be named terminals.  Using the symbol on the left-hand side of a production like this is the same as literally typing the expression on its right-hand side.  No reduction to the left-hand side of this production ever takes place.  This provides a convenient way to give important terminals more readable names.

The special `error` symbol specifies states that the parser will search back to when syntax errors are found during parsing.

### Literals and Regular Expressions

Literal elements are specified as single quoted characters.  This directs the parser to literally parse the text provided.

Regular expressions are specified as double quoted strings.  Regular expressions can be made up of character classes (`[...]` and `[^...]`), star operators (`*`), plus operators (`+`), and optional operators (`?`).  The C/C++ style escape sequences `\b`, `\f`, `\n`, `\r`, `\t`, `\x####` (hex), `\###` (octal) are recognized.  Any other escaped character evaluates to itself to allow escaping of double quotes and the characters that have special meaning in the regular expression (`|*+?[]()-`).

Lexical analyzer actions can be attached to regular expressions.  A lexical analyzer action allows users of the library to attach an arbitrary function to be executed on certain lexical analyzer states.  This can be used to deal with situations in which the behaviour of the lexical analyzer is changed such as when scanning strings or comments, perform escape sequence conversion when parsing strings, and perform disambiguation using symbol tables.  Lexical analyzer actions are attached in regular expressions using "`:`".  Any identifier specified between "`:`" characters in a regular expression is added as an action that is called when the lexical analyzer reaches a state that has the action as its next position.

### Whitespace

The `whitespace` directive specifies the input that will be skipped as whitespace every time the lexical analyzer is called to advance by a token.

### Precedence and Associativity

Precedence and associativity of terminals is set using the associativity directives `left`, `right`, and `none`.  The precedence of a production defaults to that of its rightmost terminal symbol but can be explicitly set using the `precedence` directive and passing the symbol to inherit precedence from.

The `left`, `right`, and `none` directives set the associativity of any terminals listed after them to left, right, and none respectively.  Precedence is set implicitly based on the order that directives calls are made - directives that appear later give the terminals listed after them higher precedence than those that appear before.  Directives can be used as many times as necessary to generate the correct associativity and precedence.

Productions default to having the same associativity and precedence as their rightmost terminal symbol.  The associativity and precedence of a production can be set explicitly to be the same as a specific terminal using the `precedence` directive.  The precedence directive can be placed after the right-hand side of a production before any attached action and is followed by the terminal whose precedence and associativity the production is to inherit.

Shift/reduce conflicts are resolved by examining the associativity and precedence of the symbol that is to be shifted and reduced on and the production that is to be reduced. Shift/reduce conflicts are resolved in the following way:

- If either the symbol or the production lack precedence information then the conflict is not resolved, an error is reported, and the grammar fails to generate a parser.

- If the symbol and the production have the same precedence and the symbol explicitly has no associativity (i.e. it is listed in a 'none' directive) then the conflict is not resolved, an error is reported, and the grammar fails to generate a parser.

- If the production has higher precedence than the symbol or the production and the symbol have the same precedence and the symbol associates to the right then the conflict is resolved in favour of the reduction.

- Otherwise the symbol must have higher precedence than the production or the symbol has the same precedence as the production and associates to the left and the conflict is resolved in favour of the shift.

Reduce/reduce conflicts are resolved by examining the precedence of the two conflicting productions. Reduce/reduce conflicts are resolved in the following way:

- If either of the productions lack precedence information or both productions have the same precedence then the conflict is not resolved, an error is reported, and the grammar fails to generate a parser.

- Otherwise the precedence of one production is higher than the other and the conflict is resolved in favour of this production.

### Errors and Debugging

Errors that occur during parser generation and parsing are reported through the `ParserErrorPolicy` class.  Users of the library need to implement these interfaces to handle and report errors and debug output.  The default implementations silently ignore errors and debug output.  If no `ParserErrorPolicy` class is used then the library will throw exceptions to report errors and ignore debug output.

### Thread Safety

The library has no static state and so creating and/or using multiple `ParserStateMachines` or `Parser` objects at the same time poses no problems.  `ParserStateMachine` and `Parser` objects themselves aren't threadsafe and it is assumed that there is only one thread making a call into any one object at a time.

Any number of `Parser` objects sharing the same `ParserStateMachine` can be used by any number of threads so long as multiple threads don't make calls into the same `Parser` object at the same time.
