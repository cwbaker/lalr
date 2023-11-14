//From: https://mirrors.ocf.berkeley.edu/gnu/bc/bc-1.07.1.tar.gz
bc {

%whitespace "[ \t]*";
%whitespace "#[^\r\n]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

/*
%token ENDOFLINE
%token AND
%token OR
%token NOT
%token STRING
%token NAME
%token NUMBER
%token ASSIGN_OP
%token REL_OP
%token INCR_DECR
%token Define
%token Break
%token Quit
%token Length
%token Return
%token For
%token If
%token While
%token Sqrt
%token Else
%token Scale
%token Ibase
%token Obase
%token Auto
%token Read
%token Random
%token Warranty
%token Halt
%token Last
%token Continue
%token Print
%token Limits
%token UNARY_MINUS
%token HistoryVar
%token Void
%token '+'
%token '-'
%token '*'
%token '/'
%token '%'
%token '^'
%token ';'
%token '('
%token ')'
%token '{'
%token '}'
%token ','
%token '['
%token ']'
%token '&'
*/

%nonassoc IF_WITHOUT_ELSE ;
%nonassoc Else ;
%left /*1*/ OR ;
%left /*2*/ AND ;
%nonassoc /*3*/ NOT ;
%left /*4*/ REL_OP ;
%right /*5*/ ASSIGN_OP ;
%left /*6*/ '+' '-' ;
%left /*7*/ '*' '/' '%' ;
%right /*8*/ '^' ;
%nonassoc /*9*/ UNARY_MINUS ;
%nonassoc /*10*/ INCR_DECR ;

//%start program

//%%

program :
	 /*empty*/
	| program input_item
	;
input_item :
	 semicolon_list ENDOFLINE
	| function
	| error ENDOFLINE
	;
opt_newline :
	 /*empty*/
	| ENDOFLINE
	;
semicolon_list :
	 /*empty*/
	| statement_or_error
	| semicolon_list ';' statement_or_error
	| semicolon_list ';'
	;
statement_list :
	 /*empty*/
	| statement_or_error
	| statement_list ENDOFLINE
	| statement_list ENDOFLINE statement_or_error
	| statement_list ';'
	| statement_list ';' statement
	;
statement_or_error :
	 statement
	| error statement
	;
statement :
	 Warranty
	| Limits
	| expression
	| STRING
	| Break
	| Continue
	| Quit
	| Halt
	| Return return_expression
	;
statement :
	 For '(' opt_expression ';' opt_expression ';' opt_expression ')' opt_newline statement
	;
statement :
	 If '(' expression ')' opt_newline statement opt_else
	;
statement :
	 While '(' expression ')' opt_newline statement
	| '{' statement_list '}'
	;
statement :
	 Print print_list
	;
print_list :
	 print_element
	| print_element ',' print_list
	;
print_element :
	 STRING
	| expression
	;
opt_else :
	 /*empty*/  %prec IF_WITHOUT_ELSE
	;
opt_else :
	 Else opt_newline statement
	;
function :
	 Define opt_void NAME '(' opt_parameter_list ')' opt_newline '{' required_eol opt_auto_define_list statement_list '}'
	;
opt_void :
	 /*empty*/
	| Void
	;
opt_parameter_list :
	 /*empty*/
	| define_list
	;
opt_auto_define_list :
	 /*empty*/
	| Auto define_list ENDOFLINE
	| Auto define_list ';'
	;
define_list :
	 NAME
	| NAME '[' ']'
	| '*' /*7L*/ NAME '[' ']'
	| '&' NAME '[' ']'
	| define_list ',' NAME
	| define_list ',' NAME '[' ']'
	| define_list ',' '*' /*7L*/ NAME '[' ']'
	| define_list ',' '&' NAME '[' ']'
	;
opt_argument_list :
	 /*empty*/
	| argument_list
	;
argument_list :
	 expression
	| NAME '[' ']'
	| argument_list ',' expression
	| argument_list ',' NAME '[' ']'
	;
opt_expression :
	 /*empty*/
	| expression
	;
return_expression :
	 /*empty*/
	| expression
	;
expression :
	 named_expression ASSIGN_OP /*5R*/ expression
	;
expression :
	 expression AND /*2L*/ expression
	;
expression :
	 expression OR /*1L*/ expression
	| NOT /*3N*/ expression
	| expression REL_OP /*4L*/ expression
	| expression '+' /*6L*/ expression
	| expression '-' /*6L*/ expression
	| expression '*' /*7L*/ expression
	| expression '/' /*7L*/ expression
	| expression '%' /*7L*/ expression
	| expression '^' /*8R*/ expression
	| '-' /*6L*/ expression %prec UNARY_MINUS /*9N*/
	| named_expression
	| NUMBER
	| '(' expression ')'
	| NAME '(' opt_argument_list ')'
	| INCR_DECR /*10N*/ named_expression
	| named_expression INCR_DECR /*10N*/
	| Length '(' expression ')'
	| Sqrt '(' expression ')'
	| Scale '(' expression ')'
	| Read '(' ')'
	| Random '(' ')'
	;
named_expression :
	 NAME
	| NAME '[' expression ']'
	| Ibase
	| Obase
	| Scale
	| HistoryVar
	| Last
	;
required_eol :
	 /*empty*/
	| ENDOFLINE
	| required_eol ENDOFLINE
	;

//Lexer

//\(\S+\)\s+return(\([^)]+\));
Define : 'define' ;
Break : 'break' ;
Quit : 'quit' ;
Length : 'length' ;
Return : 'return' ;
For : 'for' ;
If : 'if' ;
While : 'while' ;
Sqrt : 'sqrt' ;
Scale : 'scale' ;
Ibase : 'ibase' ;
Obase : 'obase' ;
Auto : 'auto' ;
Else : 'else' ;
Read : 'read' ;
Random : 'random' ;
Halt : 'halt' ;
Last : 'last' ;
Void : 'void' ;

OR : '||' ;
AND : '&&' ;
NOT : '!' ;
REL_OP : "==|<=|>=|!=|<|>" ;
ASSIGN_OP : "=|\+=|\-=|\*=|/=|%=|\^=" ;
INCR_DECR : "\+\+|\-\-" ;
ENDOFLINE : '\n';
Warranty : 'Warranty' ;
Limits : 'Limits' ;
Continue : 'Continue' ;
Print : 'Print' ;
HistoryVar : 'HistoryVar' ;

NAME : "[a-z][a-z0-9_]*" ;
STRING :  "\"(\\.|[^\"\n])*\"" ;
NUMBER : "[0-9]+(\.[0-9]+)?";

}