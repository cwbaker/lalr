// From: https://github.com/zyp001a/ansi-c-grammar.git

cparser {

%whitespace "[ \t\r\n]*";
%whitespace "//[^\n]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

//%token IDENTIFIER
//%token CONSTANT
//%token STRING_LITERAL
//%token SIZEOF
//%token PTR_OP
//%token INC_OP
//%token DEC_OP
//%token LEFT_OP
//%token RIGHT_OP
//%token LE_OP
//%token GE_OP
//%token EQ_OP
//%token NE_OP
//%token AND_OP
//%token OR_OP
//%token MUL_ASSIGN
//%token DIV_ASSIGN
//%token MOD_ASSIGN
//%token ADD_ASSIGN
//%token SUB_ASSIGN
//%token LEFT_ASSIGN
//%token RIGHT_ASSIGN
//%token AND_ASSIGN
//%token XOR_ASSIGN
//%token OR_ASSIGN
//%token TYPE_NAME
//%token TYPEDEF
//%token EXTERN
//%token STATIC
//%token AUTO
//%token REGISTER
//%token CHAR
//%token SHORT
//%token INT
//%token LONG
//%token SIGNED
//%token UNSIGNED
//%token FLOAT
//%token DOUBLE
//%token CONST
//%token VOLATILE
//%token VOID
//%token STRUCT
//%token UNION
//%token ENUM
//%token ELLIPSIS
//%token CASE
//%token DEFAULT
//%token IF
//%token ELSE
//%token SWITCH
//%token WHILE
//%token DO
//%token FOR
//%token GOTO
//%token CONTINUE
//%token BREAK
//%token RETURN
//%token '('
//%token ')'
//%token '['
//%token ']'
//%token '.'
//%token ','
//%token '&'
//%token '*'
//%token '+'
//%token '-'
//%token '~'
//%token '!'
//%token '/'
//%token '%'
//%token '<'
//%token '>'
//%token '^'
//%token '|'
//%token '?'
//%token ':'
//%token '='
//%token ';'
//%token '{'
//%token '}'


//%start translation_unit

//%%

translation_unit :
	 external_declaration
	| translation_unit external_declaration
	;

primary_expression :
	 IDENTIFIER
	| CONSTANT
	| STRING_LITERAL
	| '(' expression ')'
	;
postfix_expression :
	 primary_expression
	| postfix_expression '[' expression ']'
	| postfix_expression '(' ')'
	| postfix_expression '(' argument_expression_list ')'
	| postfix_expression '.' IDENTIFIER
	| postfix_expression PTR_OP IDENTIFIER
	| postfix_expression INC_OP
	| postfix_expression DEC_OP
	;
argument_expression_list :
	 assignment_expression
	| argument_expression_list ',' assignment_expression
	;
unary_expression :
	 postfix_expression
	| INC_OP unary_expression
	| DEC_OP unary_expression
	| unary_operator cast_expression
	| SIZEOF unary_expression
	| SIZEOF '(' type_name ')'
	;
unary_operator :
	 '&'
	| '*'
	| '+'
	| '-'
	| '~'
	| '!'
	;
cast_expression :
	 unary_expression
	| '(' type_name ')' cast_expression
	;
multiplicative_expression :
	 cast_expression
	| multiplicative_expression '*' cast_expression
	| multiplicative_expression '/' cast_expression
	| multiplicative_expression '%' cast_expression
	;
additive_expression :
	 multiplicative_expression
	| additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
	;
shift_expression :
	 additive_expression
	| shift_expression LEFT_OP additive_expression
	| shift_expression RIGHT_OP additive_expression
	;
relational_expression :
	 shift_expression
	| relational_expression '<' shift_expression
	| relational_expression '>' shift_expression
	| relational_expression LE_OP shift_expression
	| relational_expression GE_OP shift_expression
	;
equality_expression :
	 relational_expression
	| equality_expression EQ_OP relational_expression
	| equality_expression NE_OP relational_expression
	;
and_expression :
	 equality_expression
	| and_expression '&' equality_expression
	;
exclusive_or_expression :
	 and_expression
	| exclusive_or_expression '^' and_expression
	;
inclusive_or_expression :
	 exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression
	;
logical_and_expression :
	 inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression
	;
logical_or_expression :
	 logical_and_expression
	| logical_or_expression OR_OP logical_and_expression
	;
conditional_expression :
	 logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression
	;
assignment_expression :
	 conditional_expression
	| unary_expression assignment_operator assignment_expression
	;
assignment_operator :
	 '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;
expression :
	 assignment_expression
	| expression ',' assignment_expression
	;
constant_expression :
	 conditional_expression
	;
declaration :
	 declaration_specifiers ';'
	| declaration_specifiers init_declarator_list ';'
	;
declaration_specifiers :
	 storage_class_specifier
	| storage_class_specifier declaration_specifiers
	| type_specifier
	| type_specifier declaration_specifiers
	| type_qualifier
	| type_qualifier declaration_specifiers
	;
init_declarator_list :
	 init_declarator
	| init_declarator_list ',' init_declarator
	;
init_declarator :
	 declarator
	| declarator '=' initializer
	;
storage_class_specifier :
	 TYPEDEF
	| EXTERN
	| STATIC
	| AUTO
	| REGISTER
	;
type_specifier :
	 VOID
	| CHAR
	| SHORT
	| INT
	| LONG
	| FLOAT
	| DOUBLE
	| SIGNED
	| UNSIGNED
	| struct_or_union_specifier
	| enum_specifier
	| TYPE_NAME
	;
struct_or_union_specifier :
	 struct_or_union IDENTIFIER '{' struct_declaration_list '}'
	| struct_or_union '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER
	;
struct_or_union :
	 STRUCT
	| UNION
	;
struct_declaration_list :
	 struct_declaration
	| struct_declaration_list struct_declaration
	;
struct_declaration :
	 specifier_qualifier_list struct_declarator_list ';'
	;
specifier_qualifier_list :
	 type_specifier specifier_qualifier_list
	| type_specifier
	| type_qualifier specifier_qualifier_list
	| type_qualifier
	;
struct_declarator_list :
	 struct_declarator
	| struct_declarator_list ',' struct_declarator
	;
struct_declarator :
	 declarator
	| ':' constant_expression
	| declarator ':' constant_expression
	;
enum_specifier :
	 ENUM '{' enumerator_list '}'
	| ENUM IDENTIFIER '{' enumerator_list '}'
	| ENUM IDENTIFIER
	;
enumerator_list :
	 enumerator
	| enumerator_list ',' enumerator
	;
enumerator :
	 IDENTIFIER
	| IDENTIFIER '=' constant_expression
	;
type_qualifier :
	 CONST
	| VOLATILE
	;
declarator :
	 pointer direct_declarator
	| direct_declarator
	;
direct_declarator :
	 IDENTIFIER
	| '(' declarator ')'
	| direct_declarator '[' constant_expression ']'
	| direct_declarator '[' ']'
	| direct_declarator '(' parameter_type_list ')'
	| direct_declarator '(' identifier_list ')'
	| direct_declarator '(' ')'
	;
pointer :
	 '*'
	| '*' type_qualifier_list
	| '*' pointer
	| '*' type_qualifier_list pointer
	;
type_qualifier_list :
	 type_qualifier
	| type_qualifier_list type_qualifier
	;
parameter_type_list :
	 parameter_list
	| parameter_list ',' ELLIPSIS
	;
parameter_list :
	 parameter_declaration
	| parameter_list ',' parameter_declaration
	;
parameter_declaration :
	 declaration_specifiers declarator
	| declaration_specifiers abstract_declarator
	| declaration_specifiers
	;
identifier_list :
	 IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;
type_name :
	 specifier_qualifier_list
	| specifier_qualifier_list abstract_declarator
	;
abstract_declarator :
	 pointer
	| direct_abstract_declarator
	| pointer direct_abstract_declarator
	;
direct_abstract_declarator :
	 '(' abstract_declarator ')'
	| '[' ']'
	| '[' constant_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' constant_expression ']'
	| '(' ')'
	| '(' parameter_type_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_type_list ')'
	;
initializer :
	 assignment_expression
	| '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	;
initializer_list :
	 initializer
	| initializer_list ',' initializer
	;
statement :
	 open_statement
	| closed_statement
	;
open_statement :
	 IF '(' expression ')' statement
	| IF '(' expression ')' closed_statement ELSE open_statement
	| loop_header open_statement
	;
closed_statement :
	 simple_statement
	| IF '(' expression ')' closed_statement ELSE closed_statement
	| loop_header closed_statement
	;
simple_statement :
	 compound_statement
	| expression_statement
	| jump_statement
	| DO statement WHILE '(' expression ')' ';'
	;
loop_header :
	 labeled_header
	| iteration_header
	| selection_header
	;
labeled_header :
	 IDENTIFIER ':'
	| CASE constant_expression ':'
	| DEFAULT ':'
	;
iteration_header :
	 WHILE '(' expression ')'
	| FOR '(' expression_statement expression_statement ')'
	| FOR '(' expression_statement expression_statement expression ')'
	;
selection_header :
	 SWITCH '(' expression ')'
	;
compound_statement :
	 '{' '}'
	| '{' statement_list '}'
	| '{' declaration_list '}'
	| '{' declaration_list statement_list '}'
	;
declaration_list :
	 declaration
	| declaration_list declaration
	;
statement_list :
	 statement
	| statement_list statement
	;
expression_statement :
	 ';'
	| expression ';'
	;
jump_statement :
	 GOTO IDENTIFIER ';'
	| CONTINUE ';'
	| BREAK ';'
	| RETURN ';'
	| RETURN expression ';'
	;

external_declaration :
	 function_definition
	| declaration
	;
function_definition :
	 declaration_specifiers declarator declaration_list compound_statement
	| declaration_specifiers declarator compound_statement
	| declarator declaration_list compound_statement
	| declarator compound_statement
	;

//Lexer

IDENTIFIER : "[a-zA-Z_][a-zA-Z_0-9]*" ;
CONSTANT : "'([^'\n]|\\[^\n])'|0[xX][a-fA-F0-9]+|[0-9]+([Ee][+-]?[0-9]+)?|[0-9]*\\.[0-9]+([Ee][+-]?[0-9]+)?" ;
STRING_LITERAL : "\"([^\"\n]|\\[^\n])*\"" ;
PTR_OP : 'PTR_OP' ;
INC_OP : '++' ;
DEC_OP : '--' ;
SIZEOF : 'sizeof' ;
LEFT_OP : '<<' ;
RIGHT_OP : '>>' ;
LE_OP : '<=' ;
GE_OP : '>=' ;
EQ_OP : '==' ;
NE_OP : '!=' ;
AND_OP : '&&' ;
OR_OP : '||' ;
MUL_ASSIGN : '*=' ;
DIV_ASSIGN : '/=' ;
MOD_ASSIGN : '%=' ;
ADD_ASSIGN : '+=' ;
SUB_ASSIGN : '-=' ;
LEFT_ASSIGN : '<<=' ;
RIGHT_ASSIGN : '>>=' ;
AND_ASSIGN : '&=' ;
XOR_ASSIGN : '^=' ;
OR_ASSIGN : '|=' ;
TYPEDEF : 'typedef' ;
EXTERN : 'extern' ;
STATIC : 'static' ;
AUTO : 'auto' ;
REGISTER : 'register' ;
VOID : 'void' ;
CHAR : 'char' ;
SHORT : 'short' ;
INT : 'int' ;
LONG : 'long' ;
FLOAT : 'float' ;
DOUBLE : 'double' ;
SIGNED : 'signed' ;
UNSIGNED : 'unsigned' ;
TYPE_NAME : 'TYPE_NAME' ;
STRUCT : 'struct' ;
UNION : 'union' ;
ENUM : 'enum' ;
CONST : 'const' ;
VOLATILE : 'volatile' ;
ELLIPSIS : '...' ;
IF : 'if' ;
ELSE : 'else' ;
DO : 'do' ;
WHILE : 'while' ;
CASE : 'case' ;
DEFAULT : 'default' ;
FOR : 'for' ;
SWITCH : 'switch' ;
GOTO : 'goto' ;
CONTINUE : 'continue' ;
BREAK : 'break' ;
RETURN : 'return' ;

}
