//From: https://github.com/rmosolgo/graphql-ruby/blob/master/graphql-c_parser/ext/graphql_c_parser_ext/parser.y

graphql_ruby {

%whitespace "[ \t\r\n]*";
%whitespace "//[^\n\r]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

/*Tokens*/
//%token YYEOF
//%token YYerror
//%token YYUNDEF
//%token AMP
//%token BANG
//%token COLON
//%token DIRECTIVE
//%token DIR_SIGN
//%token ENUM
//%token ELLIPSIS
//%token EQUALS
//%token EXTEND
//%token FALSE_LITERAL
//%token FLOAT
//%token FRAGMENT
//%token IDENTIFIER
//%token INPUT
//%token IMPLEMENTS
//%token INT
//%token INTERFACE
//%token LBRACKET
//%token LCURLY
//%token LPAREN
//%token MUTATION
//%token NULL_LITERAL
//%token ON
//%token PIPE
//%token QUERY
//%token RBRACKET
//%token RCURLY
//%token REPEATABLE
//%token RPAREN
//%token SCALAR
//%token SCHEMA
//%token STRING
//%token SUBSCRIPTION
//%token TRUE_LITERAL
//%token TYPE_LITERAL
//%token UNION
//%token VAR_SIGN


//%start start

//%%

start :
	/*YYEOF
	|*/ document
	;

document :
	definitions_list
	;

definitions_list :
	definition
	| definitions_list definition
	;

definition :
	executable_definition
	| type_system_definition
	| type_system_extension
	;

executable_definition :
	operation_definition
	| fragment_definition
	;

operation_definition :
	operation_type operation_name_opt variable_definitions_opt directives_list_opt selection_set
	| LCURLY selection_list RCURLY
	| LCURLY RCURLY
	;

operation_type :
	QUERY
	| MUTATION
	| SUBSCRIPTION
	;

operation_name_opt :
	/*empty*/
	| name
	;

variable_definitions_opt :
	/*empty*/
	| LPAREN variable_definitions_list RPAREN
	;

variable_definitions_list :
	variable_definition
	| variable_definitions_list variable_definition
	;

variable_definition :
	VAR_SIGN name COLON type default_value_opt
	;

default_value_opt :
	/*empty*/
	| EQUALS literal_value
	;

selection_list :
	selection
	| selection_list selection
	;

selection :
	field
	| fragment_spread
	| inline_fragment
	;

selection_set :
	LCURLY selection_list RCURLY
	;

selection_set_opt :
	/*empty*/
	| selection_set
	;

field :
	name COLON name arguments_opt directives_list_opt selection_set_opt
	| name arguments_opt directives_list_opt selection_set_opt
	;

arguments_opt :
	/*empty*/
	| LPAREN arguments_list RPAREN
	;

arguments_list :
	argument
	| arguments_list argument
	;

argument :
	name COLON input_value
	;

literal_value :
	FLOAT
	| INT
	| STRING
	| TRUE_LITERAL
	| FALSE_LITERAL
	| null_value
	| enum_value
	| list_value
	| object_literal_value
	;

input_value :
	literal_value
	| variable
	| object_value
	;

null_value :
	NULL_LITERAL
	;

variable :
	VAR_SIGN name
	;

list_value :
	LBRACKET RBRACKET
	| LBRACKET list_value_list RBRACKET
	;

list_value_list :
	input_value
	| list_value_list input_value
	;

enum_name :
	IDENTIFIER
	| FRAGMENT
	| REPEATABLE
	| ON
	| operation_type
	| schema_keyword
	;

enum_value :
	enum_name
	;

object_value :
	LCURLY object_value_list_opt RCURLY
	;

object_value_list_opt :
	/*empty*/
	| object_value_list
	;

object_value_list :
	object_value_field
	| object_value_list object_value_field
	;

object_value_field :
	name COLON input_value
	;

object_literal_value :
	LCURLY object_literal_value_list_opt RCURLY
	;

object_literal_value_list_opt :
	/*empty*/
	| object_literal_value_list
	;

object_literal_value_list :
	object_literal_value_field
	| object_literal_value_list object_literal_value_field
	;

object_literal_value_field :
	name COLON literal_value
	;

directives_list_opt :
	/*empty*/
	| directives_list
	;

directives_list :
	directive
	| directives_list directive
	;

directive :
	DIR_SIGN name arguments_opt
	;

name :
	name_without_on
	| ON
	;

schema_keyword :
	SCHEMA
	| SCALAR
	| TYPE_LITERAL
	| IMPLEMENTS
	| INTERFACE
	| UNION
	| ENUM
	| INPUT
	| DIRECTIVE
	;

name_without_on :
	IDENTIFIER
	| FRAGMENT
	| REPEATABLE
	| TRUE_LITERAL
	| FALSE_LITERAL
	| operation_type
	| schema_keyword
	;

fragment_spread :
	ELLIPSIS name_without_on directives_list_opt
	;

inline_fragment :
	ELLIPSIS ON type directives_list_opt selection_set
	| ELLIPSIS directives_list_opt selection_set
	;

fragment_definition :
	FRAGMENT fragment_name_opt ON type directives_list_opt selection_set
	;

fragment_name_opt :
	/*empty*/
	| name_without_on
	;

type :
	nullable_type
	| nullable_type BANG
	;

nullable_type :
	name
	| LBRACKET type RBRACKET
	;

type_system_definition :
	schema_definition
	| type_definition
	| directive_definition
	;

schema_definition :
	SCHEMA directives_list_opt operation_type_definition_list_opt
	;

operation_type_definition_list_opt :
	/*empty*/
	| LCURLY operation_type_definition_list RCURLY
	;

operation_type_definition_list :
	operation_type_definition
	| operation_type_definition_list operation_type_definition
	;

operation_type_definition :
	operation_type COLON name
	;

type_definition :
	scalar_type_definition
	| object_type_definition
	| interface_type_definition
	| union_type_definition
	| enum_type_definition
	| input_object_type_definition
	;

description :
	STRING
	;

description_opt :
	/*empty*/
	| description
	;

scalar_type_definition :
	description_opt SCALAR name directives_list_opt
	;

object_type_definition :
	description_opt TYPE_LITERAL name implements_opt directives_list_opt field_definition_list_opt
	;

implements_opt :
	/*empty*/
	| IMPLEMENTS AMP interfaces_list
	| IMPLEMENTS interfaces_list
	| IMPLEMENTS legacy_interfaces_list
	;

interfaces_list :
	name
	| interfaces_list AMP name
	;

legacy_interfaces_list :
	name
	| legacy_interfaces_list name
	;

input_value_definition :
	description_opt name COLON type default_value_opt directives_list_opt
	;

input_value_definition_list :
	input_value_definition
	| input_value_definition_list input_value_definition
	;

arguments_definitions_opt :
	/*empty*/
	| LPAREN input_value_definition_list RPAREN
	;

field_definition :
	description_opt name arguments_definitions_opt COLON type directives_list_opt
	;

field_definition_list_opt :
	/*empty*/
	| LCURLY field_definition_list RCURLY
	;

field_definition_list :
	/*empty*/
	| field_definition
	| field_definition_list field_definition
	;

interface_type_definition :
	description_opt INTERFACE name implements_opt directives_list_opt field_definition_list_opt
	;

union_members :
	name
	| union_members PIPE name
	;

union_type_definition :
	description_opt UNION name directives_list_opt EQUALS union_members
	;

enum_type_definition :
	description_opt ENUM name directives_list_opt LCURLY enum_value_definitions RCURLY
	;

enum_value_definition :
	description_opt enum_name directives_list_opt
	;

enum_value_definitions :
	enum_value_definition
	| enum_value_definitions enum_value_definition
	;

input_object_type_definition :
	description_opt INPUT name directives_list_opt LCURLY input_value_definition_list RCURLY
	;

directive_definition :
	description_opt DIRECTIVE DIR_SIGN name arguments_definitions_opt directive_repeatable_opt ON directive_locations
	;

directive_repeatable_opt :
	/*empty*/
	| REPEATABLE
	;

directive_locations :
	name
	| directive_locations PIPE name
	;

type_system_extension :
	schema_extension
	| type_extension
	;

schema_extension :
	EXTEND SCHEMA directives_list_opt LCURLY operation_type_definition_list RCURLY
	| EXTEND SCHEMA directives_list
	;

type_extension :
	scalar_type_extension
	| object_type_extension
	| interface_type_extension
	| union_type_extension
	| enum_type_extension
	| input_object_type_extension
	;

scalar_type_extension :
	EXTEND SCALAR name directives_list
	;

object_type_extension :
	EXTEND TYPE_LITERAL name implements_opt directives_list_opt field_definition_list_opt
	;

interface_type_extension :
	EXTEND INTERFACE name implements_opt directives_list_opt field_definition_list_opt
	;

union_type_extension :
	EXTEND UNION name directives_list_opt EQUALS union_members
	| EXTEND UNION name directives_list
	;

enum_type_extension :
	EXTEND ENUM name directives_list_opt LCURLY enum_value_definitions RCURLY
	| EXTEND ENUM name directives_list
	;

input_object_type_extension :
	EXTEND INPUT name directives_list_opt LCURLY input_value_definition_list RCURLY
	| EXTEND INPUT name directives_list
	;

//Lexer

LCURLY : '{' ;
RCURLY : '}' ;
QUERY : 'QUERY' ;
MUTATION : 'MUTATION' ;
SUBSCRIPTION : 'SUBSCRIPTION' ;
LPAREN : '(' ;
RPAREN : ')' ;
VAR_SIGN : 'VAR_SIGN' ;
COLON : ':' ;
EQUALS : 'EQUALS' ;
FLOAT : 'FLOAT' ;
INT : 'INT' ;
STRING : 'STRING' ;
TRUE_LITERAL : 'TRUE' ;
FALSE_LITERAL : 'FALSE' ;
NULL_LITERAL : 'NULL' ;
LBRACKET : '[' ;
RBRACKET : ']' ;
IDENTIFIER : 'IDENTIFIER' ;
FRAGMENT : 'FRAGMENT' ;
REPEATABLE : 'REPEATABLE' ;
ON : 'ON' ;
DIR_SIGN : 'DIR_SIGN' ;
SCHEMA : 'SCHEMA' ;
SCALAR : 'SCALAR' ;
TYPE_LITERAL : 'TYPE_LITERAL' ;
IMPLEMENTS : 'IMPLEMENTS' ;
INTERFACE : 'INTERFACE' ;
UNION : 'UNION' ;
ENUM : 'ENUM' ;
INPUT : 'INPUT' ;
DIRECTIVE : 'DIRECTIVE' ;
ELLIPSIS : '...' ;
BANG : 'BANG' ;
AMP : '&' ;
PIPE : '|' ;
EXTEND : 'EXTEND' ;

}
