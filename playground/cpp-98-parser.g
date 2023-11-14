//From: https://github.com/IvanAli/CPPSemanticAnalysis/blob/master/cpp.y
cpp_98 {

%whitespace "[ \t\r\n]*";
%whitespace "//[^\n\r]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

/*Tokens*/
//%token YYEOF
//%token YYerror
//%token YYUNDEF
//%token INT
//%token FLOAT
//%token STRING
//%token BOOL
//%token H_CHAR
//%token Q_CHAR
//%token C_CHAR
//%token IDENTIFIER
//%token ZERO
//%token CLASS
//%token PUBLIC
//%token PRIVATE
//%token NEW
//%token DOUBLEKW
//%token CHARKW
//%token VOIDKW
//%token BOOLKW
//%token IF
//%token FOR
//%token CONST
//%token USING
//%token NAMESPACE
//%token AND
//%token OR
//%token BITWISE_AND
//%token BITWISE_OR
//%token BITWISE_XOR
//%token LESS_OR_EQUAL_THAN
//%token GREATER_OR_EQUAL_THAN
//%token LESS_THAN
//%token GREATER_THAN
//%token EQUAL
//%token NOT_EQUAL
//%token ASSIGNMENT
//%token ADDITION
//%token SUBTRACTION
//%token MULTIPLICATION
//%token DIVISION
//%token MODULO
//%token INCREMENT
//%token DECREMENT
//%token ADDITION_EQUAL
//%token SUBTRACTION_EQUAL
//%token MULTIPLICATION_EQUAL
//%token DIVISION_EQUAL
//%token MODULO_EQUAL
//%token AND_EQUAL
//%token OR_EQUAL
//%token XOR_EQUAL
//%token LEFT_SHIFT_EQUAL
//%token RIGHT_SHIFT_EQUAL
//%token OBJECT_ACCESS
//%token POINTER_ACCESS
//%token COLON
//%token TERNARY
//%token DOUBLE_COLON
//%token SEMI_COLON
//%token LEFT_SHIFT
//%token RIGHT_SHIFT
//%token CHARACTER
//%token OPEN_PARENTHESES
//%token CLOSE_PARENTHESES
//%token OPEN_BRACE
//%token CLOSE_BRACE
//%token OPEN_BRACKET
//%token CLOSE_BRACKET
//%token DOUBLEP
//%token CHARP
//%token VOIDP
//%token POINTERID
//%token DEPOINTERID
//%token RETURN
//%token COMMA
//%token ELSE
//%token TRUE
//%token FALSE
//%token THIS
//%token NOT
//%token DIRECTIVE
//%token INTKW
//%token INTP
//%token BOOLP
//%token SINGLE_QUOTE
//%token DOUBLE_QUOTE
//%token ESCAPE_SEQUENCE
//%token "."
//%token "->"
//%token "0"


//%start translation_unit

//%%

translation_unit :
	declaration_seq
	;

preprocessing_token :
	header_name
	;

tok :
	keyword
	| literal
	| operator_token
	;

header_name :
	LESS_THAN h_char_sequence GREATER_THAN
	| DOUBLE_QUOTE q_char_sequence DOUBLE_QUOTE
	;

h_char_sequence :
	H_CHAR
	| h_char_sequence H_CHAR
	;

q_char_sequence :
	Q_CHAR
	| q_char_sequence Q_CHAR
	;

identifier :
	IDENTIFIER
	;

keyword :
	BOOLKW
	| RETURN
	| DOUBLEKW
	| IF
	| NEW
	| PRIVATE
	| FALSE
	| VOIDKW
	| ELSE
	| INTKW
	| PUBLIC
	| THIS
	| CHARKW
	| FOR
	| CLASS
	| TRUE
	;

open_brace :
	OPEN_BRACE
	;

close_brace :
	CLOSE_BRACE
	;

open_parentheses :
	OPEN_PARENTHESES
	;

close_parentheses :
	CLOSE_PARENTHESES
	;

literal :
	integer_literal
	| character_literal
	| floating_literal
	| string_literal
	| boolean_literal
	;

integer_literal :
	INT
	;

character_literal :
	SINGLE_QUOTE c_char_sequence SINGLE_QUOTE
	;

c_char_sequence :
	C_CHAR
	| c_char_sequence C_CHAR
	;

escape_sequence :
	ESCAPE_SEQUENCE
	;

floating_literal :
	FLOAT
	;

string_literal :
	STRING
	;

boolean_literal :
	BOOL
	;

operator_token :
	OPEN_BRACE
	| CLOSE_BRACE
	| OPEN_BRACKET
	| CLOSE_BRACKET
	| OPEN_PARENTHESES
	| CLOSE_PARENTHESES
	| SEMI_COLON
	| COLON
	| NEW
	| OBJECT_ACCESS
	| ADDITION
	| SUBTRACTION
	| MULTIPLICATION
	| DIVISION
	| MODULO
	| BITWISE_XOR
	| BITWISE_AND
	| BITWISE_OR
	| NOT
	| ASSIGNMENT
	| LESS_THAN
	| GREATER_THAN
	| ADDITION_EQUAL
	| SUBTRACTION_EQUAL
	| MULTIPLICATION_EQUAL
	| DIVISION_EQUAL
	| MODULO_EQUAL
	| XOR_EQUAL
	| AND_EQUAL
	| OR_EQUAL
	| LEFT_SHIFT
	| RIGHT_SHIFT
	| LEFT_SHIFT_EQUAL
	| RIGHT_SHIFT_EQUAL
	| EQUAL
	| NOT_EQUAL
	| LESS_OR_EQUAL_THAN
	| GREATER_OR_EQUAL_THAN
	| AND
	| OR
	| INCREMENT
	| DECREMENT
	| COMMA
	| POINTER_ACCESS
	;

primary_expression :
	literal
	| THIS
	| OPEN_PARENTHESES expression CLOSE_PARENTHESES
	| id_expression
	;

id_expression :
	unqualified_id
	;

unqualified_id :
	identifier
	;

qualified_id :
	nested_name_specifier unqualified_id
	;

nested_name_specifier :
	class_name DOUBLE_COLON nested_name_specifier
	;

postfix_expression :
	primary_expression
	| postfix_expression OPEN_BRACKET expression CLOSE_BRACKET
	| simple_type_specifier OPEN_PARENTHESES CLOSE_PARENTHESES
	| simple_type_specifier OPEN_PARENTHESES expression_list CLOSE_PARENTHESES
	| postfix_expression OPEN_PARENTHESES expression_list CLOSE_PARENTHESES
	| postfix_expression OPEN_PARENTHESES CLOSE_PARENTHESES
	| postfix_expression INCREMENT
	| postfix_expression DECREMENT
	| postfix_expression "." id_expression
	| postfix_expression "->" id_expression
	;

expression_list :
	assignment_expression
	| expression_list COMMA assignment_expression
	;

unary_expression :
	postfix_expression
	| INCREMENT unary_expression
	| DECREMENT unary_expression
	| new_expression
	;

new_expression :
	NEW new_type_id
	;

new_type_id :
	type_specifier_seq
	;

cast_expression :
	unary_expression
	;

pm_expression :
	cast_expression
	;

multiplicative_expression :
	unary_expression
	| multiplicative_expression MULTIPLICATION unary_expression
	| multiplicative_expression DIVISION unary_expression
	| multiplicative_expression MODULO unary_expression
	;

additive_expression :
	multiplicative_expression
	| additive_expression ADDITION multiplicative_expression
	| additive_expression SUBTRACTION multiplicative_expression
	;

shift_expression :
	additive_expression
	| shift_expression LEFT_SHIFT additive_expression
	| shift_expression RIGHT_SHIFT additive_expression
	;

relational_expression :
	shift_expression
	| relational_expression LESS_THAN shift_expression
	| relational_expression GREATER_THAN shift_expression
	| relational_expression LESS_OR_EQUAL_THAN shift_expression
	| relational_expression GREATER_OR_EQUAL_THAN shift_expression
	;

equality_expression :
	relational_expression
	| equality_expression EQUAL relational_expression
	| equality_expression NOT_EQUAL relational_expression
	;

and_expression :
	equality_expression
	| and_expression BITWISE_AND equality_expression
	;

exclusive_or_expression :
	and_expression
	| exclusive_or_expression BITWISE_XOR and_expression
	;

inclusive_or_expression :
	exclusive_or_expression
	| inclusive_or_expression BITWISE_OR exclusive_or_expression
	;

logical_and_expression :
	inclusive_or_expression
	| logical_and_expression AND inclusive_or_expression
	;

logical_or_expression :
	logical_and_expression
	| logical_or_expression OR logical_and_expression
	;

conditional_expression :
	logical_or_expression
	;

assignment_expression :
	conditional_expression
	| logical_or_expression assignment_operator assignment_expression
	;

assignment_operator :
	ASSIGNMENT
	| MULTIPLICATION_EQUAL
	| DIVISION_EQUAL
	| MODULO_EQUAL
	| ADDITION_EQUAL
	| SUBTRACTION_EQUAL
	| RIGHT_SHIFT_EQUAL
	| LEFT_SHIFT_EQUAL
	| AND_EQUAL
	| XOR_EQUAL
	| OR_EQUAL
	;

expression :
	assignment_expression
	| expression COMMA assignment_expression
	;

constant_expression :
	conditional_expression
	;

statement :
	expression_statement
	| compound_statement
	| selection_statement
	| iteration_statement
	| declaration_statement
	| jump_statement
	;

expression_statement :
	expression SEMI_COLON
	;

compound_statement :
	open_brace close_brace
	| open_brace statement_seq_opt close_brace
	;

statement_seq_opt :
	statement
	| statement_seq_opt statement
	;

selection_statement :
	IF OPEN_PARENTHESES condition CLOSE_PARENTHESES statement
	| IF OPEN_PARENTHESES condition CLOSE_PARENTHESES statement ELSE statement
	;

condition :
	expression
	| simple_type_specifier identifier ASSIGNMENT assignment_expression
	;

iteration_statement :
	FOR open_parentheses condition SEMI_COLON expression SEMI_COLON assignment_expression close_parentheses statement
	;

for_init_statement :
	expression_statement
	| simple_declaration
	;

jump_statement :
	RETURN expression SEMI_COLON
	;

declaration_statement :
	simple_declaration
	;

declaration_seq :
	declaration
	| declaration_seq declaration
	;

declaration :
	simple_declaration
	| function_definition
	;

block_declaration :
	simple_declaration
	;

simple_declaration :
	cv_qualifier simple_type_specifier init_declarator_list SEMI_COLON
	| simple_type_specifier init_declarator_list SEMI_COLON
	| elaborated_type_specifier init_declarator_list SEMI_COLON
	| class_specifier init_declarator_list SEMI_COLON
	| class_specifier SEMI_COLON
	;

init_declarator_list :
	init_declarator
	| init_declarator_list COMMA init_declarator
	;

init_declarator :
	declarator
	| declarator initializer_opt
	;

initializer_opt :
	ASSIGNMENT initializer_clause
	| OPEN_PARENTHESES expression_list CLOSE_PARENTHESES
	;

decl_specifier_seq :
	decl_specifier
	| decl_specifier decl_specifier_seq
	;

decl_specifier :
	type_specifier
	;

type_specifier_seq :
	type_specifier
	| type_specifier type_specifier_seq
	;

type_specifier :
	simple_type_specifier
	| elaborated_type_specifier
	| cv_qualifier
	| class_specifier
	;

simple_type_specifier :
	CHARKW
	| BOOLKW
	| INTKW
	| DOUBLEKW
	| VOIDKW
	;

elaborated_type_specifier :
	class_key identifier
	;

function_definition :
	simple_type_specifier declarator function_body
	;

function_body :
	OPEN_BRACE CLOSE_BRACE
	| OPEN_BRACE statement_seq_opt CLOSE_BRACE
	;

type_name :
	class_name
	;

declarator :
	direct_declarator
	| ptr_operator declarator
	;

direct_declarator :
	declarator_id
	| direct_declarator parameters_and_qualifiers
	| direct_declarator OPEN_BRACKET constant_expression CLOSE_BRACKET
	| direct_declarator OPEN_BRACKET CLOSE_BRACKET
	;

noptr_declarator :
	declarator_id
	;

parameters_and_qualifiers :
	open_parentheses parameter_declaration_clause_opt close_parentheses
	;

parameter_declaration_clause_opt :
	/*empty*/
	| parameter_declaration_list
	;

parameter_declaration_list :
	parameter_declaration
	| parameter_declaration_list COMMA parameter_declaration
	;

parameter_declaration :
	simple_type_specifier identifier
	;

ptr_operator :
	MULTIPLICATION cv_qualifier_seq
	| BITWISE_AND
	;

cv_qualifier_seq :
	cv_qualifier cv_qualifier_seq
	;

cv_qualifier :
	CONST
	;

declarator_id :
	id_expression
	;

initializer_clause :
	assignment_expression
	| open_brace initializer_list COMMA close_brace
	| open_brace close_brace
	;

initializer_list :
	initializer_clause
	| initializer_list COMMA initializer_clause
	;

class_name :
	identifier
	;

class_specifier :
	class_head open_brace member_specification_opt close_brace
	;

class_head :
	class_key identifier
	;

class_key :
	CLASS
	;

member_specification_opt :
	/*empty*/
	| member_declaration member_specification_opt
	| access_specifier COLON member_specification_opt
	;

access_specifier :
	PUBLIC
	| PRIVATE
	;

member_declaration :
	simple_type_specifier member_declarator_list SEMI_COLON
	| function_definition SEMI_COLON
	| function_definition
	;

member_declarator_list :
	member_declarator
	| member_declarator_list COMMA member_declarator
	;

member_declarator :
	declarator
	| declarator pure_specifier
	;

pure_specifier :
	ASSIGNMENT "0"
	;

constant_initializer :
	ASSIGNMENT constant_expression
	;

ctor_initializer :
	COLON mem_initializer_list
	;

mem_initializer_list :
	mem_initializer
	| mem_initializer COMMA mem_initializer_list
	;

mem_initializer :
	mem_initializer_id OPEN_PARENTHESES expression_list CLOSE_PARENTHESES
	;

mem_initializer_id :
	identifier
	| DOUBLE_COLON nested_name_specifier class_name
	;

//Lexer

LESS_THAN : 'LESS_THAN' ;
GREATER_THAN : 'GREATER_THAN' ;
DOUBLE_QUOTE : 'DOUBLE_QUOTE' ;
H_CHAR : 'H_CHAR' ;
Q_CHAR : 'Q_CHAR' ;
IDENTIFIER : 'IDENTIFIER' ;
BOOLKW : 'BOOLKW' ;
RETURN : 'RETURN' ;
DOUBLEKW : 'DOUBLEKW' ;
IF : 'IF' ;
NEW : 'NEW' ;
PRIVATE : 'PRIVATE' ;
FALSE : 'FALSE' ;
VOIDKW : 'VOIDKW' ;
ELSE : 'ELSE' ;
INTKW : 'INTKW' ;
PUBLIC : 'PUBLIC' ;
THIS : 'THIS' ;
CHARKW : 'CHARKW' ;
FOR : 'FOR' ;
CLASS : 'CLASS' ;
TRUE : 'TRUE' ;
OPEN_BRACE : 'OPEN_BRACE' ;
CLOSE_BRACE : 'CLOSE_BRACE' ;
OPEN_PARENTHESES : 'OPEN_PARENTHESES' ;
CLOSE_PARENTHESES : 'CLOSE_PARENTHESES' ;
INT : 'INT' ;
SINGLE_QUOTE : 'SINGLE_QUOTE' ;
C_CHAR : 'C_CHAR' ;
ESCAPE_SEQUENCE : 'ESCAPE_SEQUENCE' ;
FLOAT : 'FLOAT' ;
STRING : 'STRING' ;
BOOL : 'BOOL' ;
OPEN_BRACKET : 'OPEN_BRACKET' ;
CLOSE_BRACKET : 'CLOSE_BRACKET' ;
SEMI_COLON : 'SEMI_COLON' ;
COLON : 'COLON' ;
OBJECT_ACCESS : 'OBJECT_ACCESS' ;
ADDITION : 'ADDITION' ;
SUBTRACTION : 'SUBTRACTION' ;
MULTIPLICATION : 'MULTIPLICATION' ;
DIVISION : 'DIVISION' ;
MODULO : 'MODULO' ;
BITWISE_XOR : 'BITWISE_XOR' ;
BITWISE_AND : 'BITWISE_AND' ;
BITWISE_OR : 'BITWISE_OR' ;
NOT : 'NOT' ;
ASSIGNMENT : 'ASSIGNMENT' ;
ADDITION_EQUAL : 'ADDITION_EQUAL' ;
SUBTRACTION_EQUAL : 'SUBTRACTION_EQUAL' ;
MULTIPLICATION_EQUAL : 'MULTIPLICATION_EQUAL' ;
DIVISION_EQUAL : 'DIVISION_EQUAL' ;
MODULO_EQUAL : 'MODULO_EQUAL' ;
XOR_EQUAL : 'XOR_EQUAL' ;
AND_EQUAL : 'AND_EQUAL' ;
OR_EQUAL : 'OR_EQUAL' ;
LEFT_SHIFT : 'LEFT_SHIFT' ;
RIGHT_SHIFT : 'RIGHT_SHIFT' ;
LEFT_SHIFT_EQUAL : 'LEFT_SHIFT_EQUAL' ;
RIGHT_SHIFT_EQUAL : 'RIGHT_SHIFT_EQUAL' ;
EQUAL : 'EQUAL' ;
NOT_EQUAL : 'NOT_EQUAL' ;
LESS_OR_EQUAL_THAN : 'LESS_OR_EQUAL_THAN' ;
GREATER_OR_EQUAL_THAN : 'GREATER_OR_EQUAL_THAN' ;
AND : 'AND' ;
OR : 'OR' ;
INCREMENT : 'INCREMENT' ;
DECREMENT : 'DECREMENT' ;
COMMA : 'COMMA' ;
POINTER_ACCESS : 'POINTER_ACCESS' ;
DOUBLE_COLON : 'DOUBLE_COLON' ;
CONST : 'CONST' ;

}
