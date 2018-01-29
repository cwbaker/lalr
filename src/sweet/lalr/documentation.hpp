
#error "This file is for doxygen documentation only - don't include it!"

namespace sweet
{

/**
 %Lexical analyzer library.
 
 Sweet %Lexer is a library that generates a lexical analyzer from one or more 
 tokens specified as literals or regular expressions.  It provides classes to
 represent tokens (LexerToken), lexical analyzer state machines (LexerStateMachine, 
 LexerState, LexerItem, LexerTransition, and LexerAction) and a class template 
 (Lexer) to scan sequences of input.
 
 The LexerStateMachine class represents a lexical analyzer's state machine.  A
 %LexerStateMachine is constructed by passing it a vector of LexerTokens to represent 
 the tokens to match and a vector of tokens to treat as whitespace.  The 
 %LexerStateMachine is then used by passing it to a %Lexer that will be used to 
 scan sequences for tokens.
 
 The LexerState, LexerItem, and LexerTransition classes represent the states
 and transitions in a lexical analyzer's state machine.  A %LexerState 
 represents a set of positions in a regular expression that a lexical analyzer 
 may be recognizing.  If the state is a matching state then the symbol that is 
 matched is also stored in the %LexerState.  A %LexerItem is a single position 
 in a regular expression that a lexical analyzer may be recognizing.  A 
 %LexerTransition specifies the next state to move to for an interval of 
 characters.
 
 The LexerAction class represents an action specified as part of a regular 
 expression.

 The Lexer class template uses the data defined in a %LexerStateMachine to
 scan input.

 The LexerToken class represents a token that can be matched by the lexical
 analyzer.  LexerTokens are expressed as either literal strings or regular 
 expressions.  Literals result in the literal text being matched in the 
 scanned input while regular expressions result in the pattern specified 
 by the regular expression being matched in the scanned input.
 
 Literals support the C/C++ style escape sequences '\\b', '\\f', '\\n', 
 '\\r', '\\t', '\\x####' (hex), and '\\###' (octal).  Any other escaped 
 character evaluates as itself.
 
 Regular expressions can be made up of character classes ('[...]' and 
 '[^...]'), star operators ('*'), plus operators ('+'), and optional 
 operators ('?').  The C/C++ style escape sequences '\\b', '\\f', '\\n', 
 '\\r', '\\t', '\\x####' (hex), '\\###' (octal) are recognized.  Any 
 other escaped character evaluates to itself to allow escaping of double 
 quotes and the characters that have special meaning in the regular 
 expression ('|*+?[]()-').

 Lexical analyzer actions can be attached to regular expressions.  A lexical
 analyzer action allows users of the library to attach an arbitrary function
 to be executed on certain lexical analyzer states.  This can be used to
 deal with situations in which the behaviour of the lexical analyzer is 
 changed such as when scanning strings or comments, perform escape sequence 
 conversion when parsing strings, and perform disambiguation using symbol
 tables.  Lexical analyzer actions are attached in regular expressions using 
 ':'.  Any identifier specified between ':' characters in a regular expression 
 is added as an action that is called when the lexical analyzer reaches a 
 state that has the action as its next position.  The action functionss are 
 attached to a %Lexer by associating them with their identifier - see
 Lexer::set_action_handler().
   
@section regular_expression_grammar Regular Expression ParserGrammar

@code
regular_expression {

    regluar_expression: or_expression
                      ;

    or_expression: cat_expression ('|' or_expression)?
                 ;

    cat_expression: postfix_expression cat_expression?
                  ;

    postfix_expression: base_expression postfix?
                      ;

    base_expression: negative_bracket_expression
                   | bracket_expression
                   | action_expression
                   | '(' or_expression ')'
                   | character
                   ;
                    
    negative_bracket_expression: '[^' '-'? negative_item* '-'? ']'
                               ;

    bracket_expression: '[' '-'? item* '-'? ']'
                      ;

    action_expression: ':' identifier ':'

    postfix: '*' | '+' | '?'
           ;

    character: '.' | ch
             ;

    item: '[:alnum:]'
        | '[:word:]'
        | '[:alpha:]'
        | '[:blank:]'
        | '[:cntrl:]'
        | '[:digit:]'
        | '[:graph:]'
        | '[:lower:]'
        | '[:print:]'
        | '[:punct:]'
        | '[:space:]'
        | '[:upper:]'
        | '[:xdigit:]'
        | ch '-' ch
        | ch
        ;

    negative_item: '[:alnum:]'
                 | '[:word:]'
                 | '[:alpha:]'
                 | '[:blank:]'
                 | '[:cntrl:]'
                 | '[:digit:]'
                 | '[:graph:]'
                 | '[:lower:]'
                 | '[:print:]'
                 | '[:punct:]'
                 | '[:space:]'
                 | '[:upper:]'
                 | '[:xdigit:]'
                 | ch '-' ch
                 | ch
                 ;

    ch: '\\' octal_digit
      | '\\x' hex_digit
      | '\\' "."
      | "[^\|\*\+\?\[\]\(\)\-]"
      ;
      
    octal_digit: "[0-7]"
               ;
               
    hex_digit: "[0-9ABCDEFabcdef]"
             ;
      
}
      
@endcode

*/
namespace parser
{
}

}

#endif
