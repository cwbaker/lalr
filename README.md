# lalr

Modern [LALR(1)](https://en.wikipedia.org/wiki/LALR_parser) parser generator and parser for C++.

## Features

- Generate LALR(1) parsers in C++ from BNF grammars.
- Compile grammars at build-time or run-time.
- Bind lexer actions for escaping characters and symbol table lookup.
- Bind actions to `std::tr1::function` objects.
- Specify terminals inline in the grammar.
- Re-entrant and thread-safe.

## Example

~~~c++

#include <stdio.h>
#include <stdarg.h>
#include <lalr/GrammarCompiler.hpp>
#include <lalr/Parser.ipp>
#include <string.h>

using namespace std;
using namespace lalr;

static int add( const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[0].user_data() + start[2].user_data();
}

static int subtract( const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[0].user_data() - start[2].user_data();
}

static int multiply( const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[0].user_data() * start[2].user_data();
}

static int divide( const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[0].user_data() / start[2].user_data();
}

static int compound( const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[1].user_data();
}

static int integer( const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return ::atoi( start[0].lexeme().c_str() );
}

void lalr_calculator_example()
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
    LALR_ASSERT( parser.accepted() );
    LALR_ASSERT( parser.full() );
    LALR_ASSERT( parser.user_data() == 20 );
}
~~~

## Installation

Build Lalr using [Forge](https://www.github.com/cwbaker/forge#forge) and a C++ compiler (XCode, Visual C++, or GCC depending on operating system).

**macOS**

- Install Xcode
- Install [Forge](https://github.com/cwbaker/forge#forge)
- `git clone git@github.com:cwbaker/lalr.git lalr`
- `cd lalr`
- `git submodule update --init`
- `forge variant=release`
- `./release/bin/lalr_examples`

**Windows**:

- Install Visual Studio 2017
- Install [Forge](https://github.com/cwbaker/forge#forge)
- `git clone git@github.com:cwbaker/lalr.git lalr`
- `cd lalr`
- `git submodule update --init`
- `forge variant=release`
- `.\release\bin\lalr_examples.exe`

## Usage

### Offline Compilation

Compile a grammar into a parse table offline using the *lalrc* tool.  The tool takes a grammar as input, compiles it to a parse table, then writes the parse table into a C++ source file that is compiled into your program.

Offline compilation completely avoids the time and memory used to generate the grammar at run-time at the cost of having to run the *lalrc* tool as a build step (or perhaps manually for grammars that change infrequently).

The *lalrc* tool's command line options:

~~~
$ lalrc --help
Usage: lalrc [options] [-o|--output OUTPUT] INPUT
-h|--help     Display this help message
-v|--version  Display version
-o|--output   Output file
~~~

The example JSON parser is generated offline using the following command line:

~~~
$ lalrc -o json.cpp json.g
~~~

### Run-time Compilation

Compile grammars into parser tables at run-time using a `GrammarCompiler` object.  The `GrammarCompiler` takes a string containing a grammar and compiles it directly into a parse table.  Take care with characters that have special meaning for both C++ strings and the regular expressions and literals in the Lalr grammar (e.g. `\`, `'`, and `"`, etc').

Run-time compilation takes time and memory but avoids any extra build steps.

### Parsing

Parsing with Lalr is best illustrated by example.  See [lalr_xml_example.cpp](#lalr_xml_example.cpp) for the following snippets in a fully working context:

**1. Include `<lalr/Parser.hpp>`**

~~~c++
#include <lalr/Parser.hpp>
~~~

Include `<lalr/Parser.hpp>`.  Lalr's parser and lexer source code are header only.  When used with offline generation of parse tables this provides a very straight forward way to integrate Lalr into your project for parsing.

**2. Create `Parser`**

~~~c++
extern lalr::ParserStateMachine* xml_parser_state_machine;
Parser<const char*, XmlUserData> parser( xml_parser_state_machine );
~~~

Reference a parse table as an `extern` variable for offline generated parse tables.  See [lalr_calculator_example.cpp](#lalr_calculator_example.cpp) for an example of compiling a grammar to parse tables at runtime.

Create a `Parser` object with the parse table as the sole argument to the constructor.  The `Parser` class template requires an iterator type template argument and optionally allows for user data, character type, character traits, and allocator to be overridden.

In the above example the iterator type is `const char*` and the user data is the custom `XmlUserData` type.

**3. Bind lexer action handlers**

~~~c++
parser.lexer_action_handlers()
    ( "string", &string_ )
;
~~~

Lexer actions allow user code to bind to any state in the lexical analyzer.  In this case the `string_()` function is skipping the leading and trailing quotes in the string.  

More complex uses include escaping sequences of characters in strings, looking up symbol tables, and even breaking out to parse different languages based on lexical tokens.

**4. Bind parser action handlers**

~~~c++
parser.parser_action_handlers()
    ( "document", &document )
    ( "add_element", &add_element )
    ( "create_element", &create_element )
    ( "short_element", &short_element )
    ( "long_element", &long_element )
    ( "add_attribute", &add_attribute )
    ( "create_attribute", &create_attribute )
    ( "attribute", &attribute )
;
~~~

Parser actions allow the user code to bind to reduce operations in the parser.  In this case matching the XML document, elements, and attributes and building up a tree of data in `XmlUserData` objects.

Each action handler function accepts the user data from the parser's stack that has been matched to the right-hand side of a production and expects the single user data for the left-hand side of the production to be returned.

In this way the input text is parsed into a syntax tree resulting in a single user data element representing the root of the syntax tree on the parser's stack at the end of a successful parse. 

**5. Parse input**

~~~c++
const char* input = 
    "<?xml version='1.0' encoding='UTF-8' standalone='yes'?>\n"
    "   <document name='example'>\n"
    "       <paragraph id='1.1'/>\n"
    "       <paragraph id='1.2'/>\n"
    "       <paragraph id='1.3'></paragraph>\n"
    "   </document>"
;

parser.parse( input, input + strlen(input) );
LALR_ASSERT( parser.accepted() );
LALR_ASSERT( parser.full() );
print( parser.user_data().element_.get(), 0 );}
~~~

Pass iterators to the beginning and end of the input text to `Parser::parse()`.

`Parser::accepted()` returns true if the parse was successful.  `Parser::full()` returns true if all of the input text (including trailing whitespace) was consumed.

## Grammars

### Structure

An *Lalr* grammar is a variation of [Backus-Naur form](https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form) consisting of an identifier followed by a curly brace delimited block containing directives and productions.  Extra whitespace and C/C++ style comments are ignored.

Ambiguous grammars fail to generate parsers.  Ambiguity can usually be resolved by rearranging the grammar or by specifying the precedence and associativity of the symbols and productions that are involved in the conflict.  See the [Shift/Reduce Conflicts](#shiftreduce-conflicts) and [Reduce/Reduce Conflicts](#reducereduce-conflicts) sections.

The following grammar, from the calculator example, has the identifier `calculator`, associativity/precedence directives `%left` and `%none`, a whitespace directive `%whitespace`, and productions to describe the language matched:

~~~
calculator {
   %left '+' '-';
   %left '*' '/';
   %none integer;
   %whitespace "[ \t\r\n]*";
   expr:
      expr '+' expr [add] |
      expr '-' expr [subtract] |
      expr '*' expr [multiply] |
      expr '/' expr [divide] |
      '(' expr ')' [compound] |
      integer [integer]
   ;
   integer: "[0-9]+";
}
~~~

### Productions

Each production consists of a symbol (the left-hand side), a colon ('`:`'), an expression made up of zero or more symbols (the right-hand side), an optional action in square brackets ('`[]`'), and an alternating pipe ('`|`') or terminating semi-colon '`;`'.

The first production in a grammar specifies the start symbol whose reduction indicates a successful parse.  After the first productions may appear in any order.

### Symbols

Terminal symbols appear as single or double quoted strings that match literal text or regular expressions respectively.  Non-terminal symbols appear as identifiers starting with a letter or underscore followed by zero or more letters, underscores, and digits.

Single quoted strings specify literal elements.  This directs the parser to literally parse the text provided.  The C/C++ style escape sequences `\b`, `\f`, `\n`, `\r`, `\t`, `\x####` (hex), `\###` (octal) are recognized.  Other escaped characters evaluate to themselves.

Double quote strings specify regular expressions.  Regular expressions can be made up of character classes (`[...]` and `[^...]`), star operators (`*`), plus operators (`+`), and optional operators (`?`).  The C/C++ style escape sequences `\b`, `\f`, `\n`, `\r`, `\t`, `\x####` (hex), `\###` (octal) are recognized.  Any other escaped character evaluates to itself to allow escaping of double quotes and the characters that have special meaning in the regular expression (`|*+?[]()-`).

Identifiers are non-terminal symbols recursively defined by productions in the grammar except for named terminals.  Named terminals use a special form of production to use an identifier to specify a terminal symbol.  See [Named Terminals](#named-terminals).

### Actions

Actions provide a binding point user code to be executed when a reduction occurs.  An action is attached to a production specifying the action's identifier between `[` and `]` at the end of the production.

Use code binds a function to an action by name at run-time.  The action function is accepts the data on the parser's stack matching the right-hand side of the production and returns the data that will be stored in the stack, in place of the right-hand side elements, with the left-hand side of the production.

For example `add`, `subtract`, etc appearing in square brackets are all actions.  Specifically the `add` in `expr: expr '+' expr [add]` is binds the action `add` to the production `expr: expr '+' expr`.

### Named Terminals

Productions with a right-hand side containing only a single literal or regular expression and no attached action introduce a named terminal.

Named terminals identify a terminal symbol by identifier rather than its literal or regular expression.  No reduction ever happens for a named terminal's production.

The naming provides a convenient way to give terminals, especially complex ones, more readable names.

For example the production `integer: "[0-9]+"` introduces the named terminal `integer` in place of the regular expression `[0-9]+`.
    
### Associativity and Precedence

Precedence/associativity directives consist of a directive (`%left`, `%right`, and `%none`), one or more terminals, and a terminating semi-colon `;`.  

The associativity of the listed terminals is set to left, right, or none as expected.  Precedence is assigned by the order of associativity directives with directives on later lines having higher precedence.

The precedence of a production defaults to that of its right-most terminal but can be explicitly set to the precedence of a different terminal using the `%precedence` directive.

The precedence directive appears after the right-hand side of a production before any attached action and is followed by the terminal whose precedence and associativity the production is to inherit.

### Shift/Reduce Conflicts

Shift/reduce conflicts are resolved by examining the associativity and precedence of the symbol that is to be shifted and reduced on and the production that is to be reduced. Shift/reduce conflicts are resolved in the following way:

- If either the symbol or the production lack precedence information then the conflict is not resolved, an error is reported, and the grammar fails to generate a parser.

- If the symbol and the production have the same precedence and the symbol explicitly has no associativity (i.e. it is listed in a `%none` directive) then the conflict is not resolved, an error is reported, and the grammar fails to generate a parser.

- If the production has higher precedence than the symbol or the production and the symbol have the same precedence and the symbol associates to the right then the conflict is resolved in favour of the reduction.

- Otherwise the symbol must have higher precedence than the production or the symbol has the same precedence as the production and associates to the left and the conflict is resolved in favour of the shift.

### Reduce/Reduce Conflicts

Reduce/reduce conflicts are resolved by examining the precedence of the two conflicting productions. Reduce/reduce conflicts are resolved in the following way:

- If either of the productions lack precedence information or both productions have the same precedence then the conflict is not resolved, an error is reported, and the grammar fails to generate a parser.

- Otherwise the precedence of one production is higher than the other and the conflict is resolved in favour of this production.

### Whitespace
 
The `%whitespace` directive specifies a regular expression that will be skipped as whitespace every time the lexical analyzer is called to advance by a token.

Whitespace at the end of a parse is also skipped to ignore whitespace that trails the input language.

### Lexer Actions

Lexical analyzer actions can be attached to regular expressions allowing clients of the library to attach an arbitrary function to be executed on certain lexical analyzer states.  

This can be used to deal with situations in which the behaviour of the lexical analyzer is changed such as when scanning strings or comments, perform escape sequence conversion when parsing strings, and perform disambiguation using symbol tables.

Lexical analyzer actions are attached in regular expressions using an identifier delimited by colons "`:`".  Any identifier specified between "`:`" characters in a regular expression is added as an action that is called when the lexical analyzer reaches a state that has the action as its next position.

### Errors and Debugging

Errors that occur during parser generation and parsing are reported through the `ErrorPolicy` class.  Clients of the library should implement these interfaces to handle and report errors and debug output.  The default implementations print errors and output to `stderr` and `stdout` respectively.

Syntax errors during parsing are handled by backtracking until the `error` symbol can be accepted, accepting that symbol, and then continuing the parse.  If there is no `error` symbol specified in the grammar or the parser backtracks all the way back to the start of the input then parsing fails.

### Thread Safety

The library has no static state and so creating and/or using multiple `ParserStateMachine` or `Parser` objects at the same time poses no problems.  `Parser` objects themselves aren't threadsafe and it is assumed that there is only one thread making a call into any one object at a time.  

Any number of `Parser` objects sharing the same `ParserStateMachine` can be used by any number of threads at once so long as multiple threads don't make calls into the same `Parser` object at the same time.

## License

*lalr* is licensed under the [MIT License](http://www.opensource.org/licenses/MIT).
