# Sweet Parser

*Sweet Parser* is a modern LALR(1) parser and parser generator.

The parser can be generated at runtime without the need for an offline generation step.  If offline generation is required there is a stand alone executable that provides Lua scriptable code generation from the parser state machine.  Parsers generated this way have no dependencies on anything other than the standard libary.

Features:

- LALR(1) parser generation from a BNF grammar.
- No separate generation step required.
- Lua bindings allow generation of parsers for C++ and other languages.
- Lexer actions for escape character conversion.
- Lexer actions for feedback from the parser to the lexer.
- Bind parser and lexer actions to `std::tr1::function` objects.
- Lexical tokens can be specified inline in the grammar.
- Re-entrant and thread-safe.

## Installation

## Usage

*Sweet Parser* is a C++ library that generates LALR(1) parsers from BNF grammars.  It provides classes to represent a grammar (`ParserGrammar`), the parser state machine (`ParserStateMachine`, `ParserState`, `ParserItem`, `ParserTransition`, `ParserProduction`, `ParserSymbol`, and `ParserAction`), receive notification of errors during parser generation and parsing (`ParserErrorPolicy`), and class templates to parse input (`Parser`, `ParserNode`, and `ParserUserData`).

The `ParserStateMachine` class represents a parser's state machine.  Its constructor takes as a grammar as input from which it generates a state machine that can be used to parse source text that conforms to that grammar.  The input can be a programmatically constructed ParserGrammar object or a sequence or file containing a grammar specified using the format defined in the grammar section below.

The `ParserState` class represents the set of positions in the grammar that a parser may be recognizing.

The `ParserItem` class is a single position in the grammar that a parser may be recognizing.  

The `ParserTransition` class specifies the next state to move to for a particular symbol, whether to shift or reduce on that transition, and for reduction transitions the action that should be taken (if any).

The `ParserProduction` class represents a production in the grammar that can be reduced from a list of symbols on the right-hand side to a single symbol on the left-hand side.

The `ParserSymbol` class represents a symbol in the grammar.  The symbols may be terminal or non-terminal.  Non-terminal symbols contain all of the productions that can be reduced to that symbol.

The `ParserAction` class represents an action that is attached to a production in the grammar.   

The `ParserErrorPolicy` class is an interfaces that can be implemented to be notified of errors and provide debug output during parser generation and parsing.

The `Parser` class template uses the data defined in a ParserStateMachine to parse input.  Construct a Parser object using a ParserStateMachine and then pass it sequences of input to parse.

The `ParserNode` class template is an element on the parser's stack.  It stores the right-hand side of the parsed input that hasn't been matched.  When the elements on the top of the stack match the right-hand side of a production then they are popped from the stack and replaced by a single element for the symbol on the left-hand side of the production that has been reduced to.  Each element on the stack stores the state that the parser is in when that element is at the top of the stack, the productions that were potentially started when the parser entered that state, the symbol at that element, the lexeme at that element, and any user data supplied by actions that were taken when that element was reduced.

The `ParserUserData` class template is a default implementation for user data stored in an element on the parser's stack.  It is expected that users of the library will provide their own user data implementation and pass it as a template parameter to a Parser.

### Initialization
 
The library needs no special initialization.  Constructing a `ParserStateMachine` object with a valid grammar and using that to construct a `Parser` is all that needs to be done to start parsing input.

~~~c++
#include <sweet/parser/ParserStateMachine.hpp>
#include <sweet/parser/Parser.ipp>
#include <stdio.h>
#include <string.h>

using namespace std;
using namespace sweet;
using namespace sweet::parser;

static ptr<ParserUserData<char> > hello_world( const ParserSymbol* symbol, const ParserNode<>* start, const ParserNode<>* finish )
{
    printf( "Hello World!\n" );
    return ptr<ParserUserData<char> >();
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
~~~

### Grammar

When specified as a sequence or file a grammar consists of an identifier followed by one or more productions enclosed within curly braces.  The first production specifies the start symbol for the grammar after which productions can occur in any order.  Extra whitespace and C/C++ style comments are ignored.

Each production consists of a symbol (the left-hand side), a colon, an expression made up of symbols, operators, literals, and regular expressions (the right-hand side), an optional action, and a terminating semi-colon.

The right-hand side specifies a pattern in the language that is to be matched when parsing.  When the pattern is matched a reduction occurs and the symbols that have been matched on the right-hand side are reduced to the symbol on the left-hand side.  This process is repeated until a right-hand side is reduced to the start symbol.

The optional action allows user code to be executed when a reduction occurs.  An action is attached to a production by specifying the action's identifier between `[` and `]` at the end of the production.   The action's identifier is used by user code to attach an arbitrary function to be executed when that production is reduced.  The action function is passed the symbol on the left-hand side and the elements on the parser's stack that make up the right-hand side.  The return value of the action function is stored in the element of the parser's stack that results from the reduction.

Ambiguous grammars fail to generate parsers.  Ambiguity can be resolved by rearranging the grammar or by specifying the precedence and associativity of the symbols and productions that are involved in the conflict.

Syntax errors that occur during parsing are handled by backtracking until a special `error` symbol can be accepted, accepting that symbol, and then continuing the parse.  If there is no `error` symbol specified in the grammar or the parser backtracks all the way back to the start of the input then parsing fails.

~~~c++
#include <stdio.h>
#include <stdarg.h>
#include <sweet/parser/ParserStateMachine.hpp>
#include <sweet/parser/Parser.ipp>
#include <string.h>

using namespace std;
using namespace sweet;
using namespace sweet::parser;

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
    const char* grammar = 
        "calculator {\n"
        "   %left '+' '-';\n"
        "   %left '*' '/';\n"
        "   %none integer;\n"
        "   %whitespace \"[ \t\r\n]*\";\n"
        "\n"
        "   expr: expr '+' expr [add]\n"
        "       | expr '-' expr [subtract]\n"
        "       | expr '*' expr [multiply]\n"
        "       | expr '/' expr [divide]\n"
        "       | '(' expr ')' [compound]\n"
        "       | integer [integer]\n"
        "       ;\n"
        "\n"
        "   integer: \"[0-9]+\";\n"
        "}"
    ;

    ParserStateMachine parser_state_machine( grammar, grammar + strlen(grammar) );
    Parser<const char*, int> parser( &parser_state_machine );
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

Grammars can also be specified programmatically using a `ParserGrammar` object.  The interface to the `ParserGrammar` class is designed to be easy to use when driven from a parser rather than to be easy to use when programmed directly.

Productions are built up by calling `ParserGrammar::begin_production()` passing the symbol on the left hand side of the production.  This prepares the `ParserGrammar` for the right hand side of the production to be built up.  The right hand side of the production is built up repeatedly calling `ParserGrammar::symbol()` to append a symbols.  The action to be taken when the current production is reduced is set by calling `ParserGrammar::action()`.  The precedence of the current production is set to match that of a terminal symbol by calling `ParserGrammar::precedence_symbol()`.  To signal the end of the production `ParserGrammar::end_production()` is called.  `ParserGrammar::end_production()` must be called before the next production is started.

Symbols can be added at any time by calling `ParserGrammar::add_symbol()`, `ParserGrammar::add_terminal()`, or `ParserGrammar::add_non_terminal()` but they must obviously be added before they are used in either the left or right hand side of a production.

Actions, like symbols, can be added at any time by calling `ParserGrammar::add_action()`.  Again like symbols they must obviously be added before they can be used.
 
### Symbols

No explicit distinction needs to be made between terminal and non-terminal symbols.  The library is able to determine whether or not a symbol is terminal or non-terminal implicitly.  Productions that have a right-hand side that contains only a single terminal symbol (expressed as a literal or a regular expression) and no attached action are assumed to name terminals.  Using the symbol on the left-hand side of a production like this is the same as literally typing the expression on its right-hand side.  No reduction to the left-hand side of this production will ever take place.  This provides a convenient way to give important terminals more readable names.

The special `error` symbol specifies states that the parser will search back to when syntax errors are found during parsing.

### Literals and Regular Expressions

Literal elements are specified as single quoted strings.  This directs the parser to literally parse the text provided.  The C/C++ style escape sequences `\b`, `\f`, `\n`, `\r`, `\t`, `\x####` (hex), `\###` (octal) are recognized.  Any other escaped character evaluates as itself to allow escaping of single quotes in the literal.

Regular expressions are specified as double quoted strings.  Regular expressions can be made up of character classes (`[...]` and `[^...]`), star operators (`*`), plus operators (`+`), and optional operators (`?`).  The C/C++ style escape sequences `\b`, `\f`, `\n`, `\r`, `\t`, `\x####` (hex), `\###` (octal) are recognized.  Any other escaped character evaluates to itself to allow escaping of double quotes and the characters that have special meaning in the regular expression (`|*+?[]()-`).

Lexical analyzer actions can be attached to regular expressions.  A lexical analyzer action allows users of the library to attach an arbitrary function to be executed on certain lexical analyzer states.  This can be used to deal with situations in which the behaviour of the lexical analyzer is changed such as when scanning strings or comments, perform escape sequence conversion when parsing strings, and perform disambiguation using symbol tables.  Lexical analyzer actions are attached in regular expressions using "`:`".  Any identifier specified between "`:`" characters in a regular expression is added as an action that is called when the lexical analyzer reaches a state that has the action as its next position.

### Whitespace
 
The special `whitespace` directive specifies the input that will be skipped as whitespace every time the lexical analyzer is called to advance by a token.

### Precedence and Associativity

The library allows the precedence and associativity of terminals to be set using the associativity directives `left`, `right`, and `none`.  The precedence of a production defaults to that of its rightmost terminal symbol but can be explicitly set using the `precedence` directive.

The `left`, `right`, and `none` directives set the associativity of any terminals listed after them to left, right, and none respectively.  Precedence is set implicitly based on the order that directives appear in the file - directives that appear later give the terminals listed after them higher precedence than those that appear before.  Directives can be used as many times as necessary to generate the correct associativity and precedence.

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

Note that any number of `Parser` objects sharing the same `ParserStateMachine` can be used by any number of threads so long as multiple threads don't make calls into the same `Parser` object at the same time.
 
### Generating Parsers

Parsers can also be generated offline using the parser tool and some Lua script.  The tool provides Lua bindings to the library so that a grammar can be parsed from an input file to generate a state machine and then code can be generated from that state machine using Lua scripts.

Parsers generated this way only need to include 'parser.hpp' and not the entire parser library.  They have no external dependencies other than on the standard library.

~~~
Usage: parser [options] [attribute=value] file ...
Options:
  -h, --help         Print this message and exit.
  -v, --version      Print the version and exit.
  -r, --require      Set the filename of the script to require on startup.
  -s, --stack-trace  Enable stack traces in error messages.

> parser -r parser/cxx json.g
~~~

### Parser Grammar

~~~
grammar: identifier '{' statements '}'
       ;

statements: statements statement | statement | ;

statement: associativity_directive
         | whitespace_directive
         | production
         ;

whitespace_directive: '%whitespace' regex ';'
                    ;

associativity_directive: associativity symbols ';' 
                       ;
              
symbols: symbols symbol | symbol | ;

symbol: terminal | regex | identifier
      ;

associativity: '%left' | '%right' | '%none'
             ;    

production: identifier ':' expressions ';'
          ;

expressions: expressions '|' expression | expression | ;

expression: symbols precedence action
          ;

precedence: '%precedence' symbol
          |
          ;             

action: '[' identifier ']'
      |
      ;
         
terminal: "'[^']*'"
        ;

regex: "\"[^\"]*\""
     ;

identifier: "[A-Za-z_][A-Za-z0-9_]*"
          ;
~~~
