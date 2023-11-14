//From: https://github.com/riej/lsl_ext/blob/master/parser/parser.y

lsl_ext {

%whitespace "[ \t\r\n]*";
%whitespace "//[^\n]*";
//%whitespace "/\*[^*]+\*/";
//%whitespace "/\*.+\*/"; ////!!!!<<< this makes lalr enter an endless loop
%whitespace "/\*:C_MultilineComment:";

//%token COMMENT
//%token C_STYLE_COMMENT
//%token INTEGER
//%token FLOAT
//%token STRING
//%token KEY
//%token VECTOR
//%token ROTATION
//%token LIST
//%token INTEGER_CONSTANT
//%token FLOAT_CONSTANT
//%token STRING_CONSTANT
//%token IDENTIFIER
//%token DEFAULT
//%token STATE
//%token JUMP
//%token RETURN
//%token IF
//%token ELSE
//%token FOR
//%token DO
//%token WHILE
//%token INC_OP
//%token DEC_OP
//%token ADD_ASSIGN
//%token SUB_ASSIGN
//%token MUL_ASSIGN
//%token DIV_ASSIGN
//%token MOD_ASSIGN
//%token EQ
//%token NEQ
//%token LEQ
//%token GEQ
//%token BOOLEAN_AND
//%token BOOLEAN_OR
//%token SHIFT_LEFT
//%token SHIFT_RIGHT
//%token ARRAY_BRACES
//%token BOOLEAN
//%token TRUE
//%token FALSE
//%token INCLUDE
//%token PRAGMA
//%token STRUCT
//%token SWITCH
//%token CASE
//%token BREAK
//%token CONTINUE
//%token CONST
//%token DELETE
//%token '{'
//%token '@'
//%token '-'
//%token '!'
//%token '~'
//%token '('
//%token '<'
//%token '['
//%token '#'
//%token LOWER_THAN_ELSE
//%token '='
//%token '|'
//%token '^'
//%token '&'
//%token '>'
//%token '+'
//%token '*'
//%token '/'
//%token '%'
//%token LIST_ITEM_PREC
//%token INITIALIZER
//%token '}'
//%token ','
//%token ';'
//%token ')'
//%token ':'
//%token '.'
//%token ']'

%nonassoc /*1*/ LOWER_THAN_ELSE ;
%nonassoc /*2*/ ELSE ;
%nonassoc /*3*/ INTEGER_CONSTANT FLOAT_CONSTANT ;
%right /*4*/ ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN '=' ;
%left /*5*/ BOOLEAN_AND BOOLEAN_OR ;
%left /*6*/ '|' ;
%left /*7*/ '^' ;
%left /*8*/ '&' ;
%left /*9*/ EQ NEQ ;
%left /*10*/ LEQ GEQ '<' '>' ;
%left /*11*/ SHIFT_LEFT SHIFT_RIGHT ;
%left /*12*/ '-' '+' ;
%left /*13*/ '*' '/' '%' ;
%right /*14*/ INC_OP DEC_OP '!' '~' ;
%right /*15*/ '[' ; //LIST_ITEM_PREC ;
%nonassoc /*16*/ INITIALIZER ;

//%start lscript_program

//%%

lscript_program :
	 globals states
	| globals
	| states
	;
globals :
	 global
	| preproc
	| globals preproc
	| globals global
	;
preproc :
	 '#' INCLUDE STRING_CONSTANT
	| '#' PRAGMA IDENTIFIER
	;
global :
	variable_declaration
	| function
	| struct
	| empty_statement
	;
identifier :
	 IDENTIFIER
	;
typename :
	 INTEGER
	| FLOAT
	| STRING
	| KEY
	| VECTOR
	| ROTATION
	| LIST
	| BOOLEAN
	;
constant :
	 INTEGER_CONSTANT /*3N*/
	| FLOAT_CONSTANT /*3N*/
	| STRING_CONSTANT
	| TRUE
	| FALSE
	;
pre_variable :
	 identifier
	| identifier '=' /*4R*/ expression
	| identifier '{' '}'
	| identifier '{' struct_expression_variables '}'
	;
pre_variables :
	 pre_variable
	| pre_variables ',' pre_variable
	;
variable_declaration :
	 typename pre_variables ';'
	| identifier pre_variables ';'
	| identifier ARRAY_BRACES pre_variables ';'
	| CONST pre_variables ';'
	;
variable_declarations :
	 variable_declaration
	| variable_declarations variable_declaration
	;
struct :
	 STRUCT identifier '{' variable_declarations '}'
	;
function :
	 identifier '(' ')' block_statement
	| typename identifier '(' ')' block_statement
	| identifier '(' function_arguments ')' block_statement
	| typename identifier '(' function_arguments ')' block_statement
	;
function_arguments :
	 function_argument
	| function_argument ',' function_arguments
	;
function_argument :
	 typename identifier
	| identifier identifier
	;
event :
	 identifier '(' ')' block_statement
	| identifier '(' function_arguments ')' block_statement
	;
events :
	 event
	| event events
	;
states :
	 default_state
	| default_state other_states
	;
other_states :
	 state
	| state other_states
	;
default_state :
	 DEFAULT '{' events '}'
	| DEFAULT '{' '}'
	;
state :
	 STATE identifier '{' events '}'
	| STATE identifier '{' '}'
	;
block_statement :
	 '{' '}'
	| '{' statements '}'
	;
statements :
	 statement
	| variable_declaration
	| statements statement
	| statements variable_declaration
	;
empty_statement :
	 ';'
	;
statement :
	 empty_statement
	| STATE identifier ';'
	| STATE DEFAULT ';'
	| '@' identifier ';'
	| JUMP identifier ';'
	| RETURN expression ';'
	| RETURN ';'
	| expression ';'
	| block_statement
	| IF '(' expression ')' statement %prec LOWER_THAN_ELSE /*1N*/
	| IF '(' expression ')' statement ELSE /*2N*/ statement
	| FOR '(' for_expression_list ';' expression ';' for_expression_list ')' statement
	| FOR '(' for_expression_list ';' ';' for_expression_list ')' statement
	| DO statement WHILE '(' expression ')' ';'
	| WHILE '(' expression ')' statement
	| SWITCH '(' expression ')' block_statement
	| CASE next_for_expression_list ':'
	| DEFAULT ':'
	| BREAK ';'
	| CONTINUE ';'
	;
for_expression_list :
	 /*empty*/
	| next_for_expression_list
	;
next_for_expression_list :
	 expression
	| expression ',' next_for_expression_list
	;
lvalue_identifiers :
	 identifier
	| identifier '.' identifier
	;
list_item_expression :
	 lvalue_identifiers '[' /*15R*/ expression ']'
	| lvalue_identifiers '[' /*15R*/ expression ',' expression ']'
	;
lvalue :
	 lvalue_identifiers
	| list_item_expression
	| lvalue_identifiers '[' /*15R*/ expression ']' '.' identifier
	;
expression :
	 unary_expression
	| typecast_expression
	| lvalue '=' /*4R*/ expression
	| lvalue ADD_ASSIGN /*4R*/ expression
	| lvalue SUB_ASSIGN /*4R*/ expression
	| lvalue MUL_ASSIGN /*4R*/ expression
	| lvalue DIV_ASSIGN /*4R*/ expression
	| lvalue MOD_ASSIGN /*4R*/ expression
	| expression EQ /*9L*/ expression
	| expression NEQ /*9L*/ expression
	| expression LEQ /*10L*/ expression
	| expression GEQ /*10L*/ expression
	| expression '<' /*10L*/ expression
	| expression '>' /*10L*/ expression
	| expression '+' /*12L*/ expression
	| expression '-' /*12L*/ expression
	| expression '*' /*13L*/ expression
	| expression '/' /*13L*/ expression
	| expression '%' /*13L*/ expression
	| expression '&' /*8L*/ expression
	| expression '|' /*6L*/ expression
	| expression '^' /*7L*/ expression
	| expression BOOLEAN_AND /*5L*/ expression
	| expression BOOLEAN_OR /*5L*/ expression
	| expression SHIFT_LEFT /*11L*/ expression
	| expression SHIFT_RIGHT /*11L*/ expression
	;
unary_expression :
	 '-' /*12L*/ expression
	| '!' /*14R*/ expression
	| '~' /*14R*/ expression
	| INC_OP /*14R*/ lvalue
	| DEC_OP /*14R*/ lvalue
	| DELETE list_item_expression
	| unary_postfix_expression
	| '#' unary_expression
	| '(' expression ')'
	;
typecast_expression :
	 '(' typename ')' unary_expression
	;
unary_postfix_expression :
	 '<' /*10L*/ expression ',' expression ',' expression '>' /*10L*/ %prec INITIALIZER /*16N*/
	| '<' /*10L*/ expression ',' expression ',' expression ',' expression '>' /*10L*/ %prec INITIALIZER /*16N*/
	| ARRAY_BRACES %prec INITIALIZER /*16N*/
	| '[' /*15R*/ list_values ']' %prec INITIALIZER /*16N*/
	| struct_expression %prec INITIALIZER /*16N*/
	| lvalue
	| lvalue INC_OP /*14R*/
	| lvalue DEC_OP /*14R*/
	| identifier '(' ')'
	| identifier '(' list_values ')'
	| constant
	;
list_values :
	 expression
	| expression ',' list_values
	;
struct_expression :
	 identifier '{' '}'
	| identifier '{' struct_expression_variables '}'
	;
struct_expression_variables :
	 struct_expression_variable
	| struct_expression_variable ',' struct_expression_variables
	;
struct_expression_variable :
	 identifier ':' expression
	;

//Lexer

//"\([^"]+\)":\s*\(\w[^,]+\),
INTEGER : 'integer' ;
INTEGER : 'int' ;
FLOAT : 'float' ;
STRING : 'string' ;
KEY : 'key' ;
VECTOR : 'vector' ;
ROTATION : 'rotation' ;
ROTATION : 'quaternion' ;
LIST : 'list' ;

DEFAULT : 'default' ;
STATE : 'state' ;

JUMP : 'jump' ;
RETURN : 'return' ;
IF : 'if' ;
ELSE : 'else' ;
FOR : 'for' ;
DO : 'do' ;
WHILE : 'while' ;

INC_OP : '++' ;
DEC_OP : '--' ;

ADD_ASSIGN : '+=' ;
SUB_ASSIGN : '-=' ;
MUL_ASSIGN : '*=' ;
DIV_ASSIGN : '/=' ;
MOD_ASSIGN : '%=' ;

EQ : '==' ;
NEQ : '!=' ;
LEQ : '<=' ;
GEQ : '>=' ;

BOOLEAN_AND : '&&' ;
BOOLEAN_OR : '||' ;

SHIFT_LEFT : '<<' ;
SHIFT_RIGHT : '>>' ;

BOOLEAN : 'boolean' ;
TRUE : 'true' ;
FALSE : 'false' ;
TRUE : 'TRUE' ;
FALSE : 'FALSE' ;

INCLUDE : 'include' ;
PRAGMA : 'pragma' ;

STRUCT : 'struct' ;

ARRAY_BRACES : '[]' ;

SWITCH : 'switch' ;
CASE : 'case' ;
BREAK : 'break' ;
CONTINUE : 'continue' ;

CONST : 'const' ;

DELETE : 'delete' ;

IDENTIFIER : "[A-Z_a-z][A-Z_a-z0-9]*" ;
INTEGER_CONSTANT : "[0-9]+" ;
FLOAT_CONSTANT : "([0-9]+\.[0-9]*|\.[0-9]+)([Ee](\+|\-)?[0-9]+)?" ;
STRING_CONSTANT : "\"(\\.|[^\"\n])*\"" ;

//COMMENT : "//[^\n]*";
//C_STYLE_COMMENT : "/\*.+\*/" ;
//C_STYLE_COMMENT : "/\*[^*]+\*/" ;

}
