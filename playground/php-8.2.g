php {

%whitespace "[ \t\r\n]*";
%whitespace "//[^\n]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

/*Tokens*/
//%token END
//%token YYerror
//%token YYUNDEF
//%token PREC_ARROW_FUNCTION
//%token '='
//%token '?'
//%token ':'
//%token '|'
//%token '^'
//%token '<'
//%token '>'
//%token '.'
//%token '+'
//%token '-'
//%token '*'
//%token '/'
//%token '%'
//%token '!'
//%token '~'
//%token '@'
//%token T_NOELSE
//%token T_LNUMBER
//%token T_DNUMBER
//%token T_STRING
//%token T_NAME_FULLY_QUALIFIED
//%token T_NAME_RELATIVE
//%token T_NAME_QUALIFIED
//%token T_VARIABLE
//%token T_INLINE_HTML
//%token T_ENCAPSED_AND_WHITESPACE
//%token T_CONSTANT_ENCAPSED_STRING
//%token T_STRING_VARNAME
//%token T_NUM_STRING
//%token T_INCLUDE
//%token T_INCLUDE_ONCE
//%token T_EVAL
//%token T_REQUIRE
//%token T_REQUIRE_ONCE
//%token T_LOGICAL_OR
//%token T_LOGICAL_XOR
//%token T_LOGICAL_AND
//%token T_PRINT
//%token T_YIELD
//%token T_YIELD_FROM
//%token T_INSTANCEOF
//%token T_NEW
//%token T_CLONE
//%token T_EXIT
//%token T_IF
//%token T_ELSEIF
//%token T_ELSE
//%token T_ENDIF
//%token T_ECHO
//%token T_DO
//%token T_WHILE
//%token T_ENDWHILE
//%token T_FOR
//%token T_ENDFOR
//%token T_FOREACH
//%token T_ENDFOREACH
//%token T_DECLARE
//%token T_ENDDECLARE
//%token T_AS
//%token T_SWITCH
//%token T_ENDSWITCH
//%token T_CASE
//%token T_DEFAULT
//%token T_MATCH
//%token T_BREAK
//%token T_CONTINUE
//%token T_GOTO
//%token T_FUNCTION
//%token T_FN
//%token T_CONST
//%token T_RETURN
//%token T_TRY
//%token T_CATCH
//%token T_FINALLY
//%token T_THROW
//%token T_USE
//%token T_INSTEADOF
//%token T_GLOBAL
//%token T_STATIC
//%token T_ABSTRACT
//%token T_FINAL
//%token T_PRIVATE
//%token T_PROTECTED
//%token T_PUBLIC
//%token T_READONLY
//%token T_VAR
//%token T_UNSET
//%token T_ISSET
//%token T_EMPTY
//%token T_HALT_COMPILER
//%token T_CLASS
//%token T_TRAIT
//%token T_INTERFACE
//%token T_ENUM
//%token T_EXTENDS
//%token T_IMPLEMENTS
//%token T_NAMESPACE
//%token T_LIST
//%token T_ARRAY
//%token T_CALLABLE
//%token T_LINE
//%token T_FILE
//%token T_DIR
//%token T_CLASS_C
//%token T_TRAIT_C
//%token T_METHOD_C
//%token T_FUNC_C
//%token T_NS_C
//%token T_ATTRIBUTE
//%token T_PLUS_EQUAL
//%token T_MINUS_EQUAL
//%token T_MUL_EQUAL
//%token T_DIV_EQUAL
//%token T_CONCAT_EQUAL
//%token T_MOD_EQUAL
//%token T_AND_EQUAL
//%token T_OR_EQUAL
//%token T_XOR_EQUAL
//%token T_SL_EQUAL
//%token T_SR_EQUAL
//%token T_COALESCE_EQUAL
//%token T_BOOLEAN_OR
//%token T_BOOLEAN_AND
//%token T_IS_EQUAL
//%token T_IS_NOT_EQUAL
//%token T_IS_IDENTICAL
//%token T_IS_NOT_IDENTICAL
//%token T_IS_SMALLER_OR_EQUAL
//%token T_IS_GREATER_OR_EQUAL
//%token T_SPACESHIP
//%token T_SL
//%token T_SR
//%token T_INC
//%token T_DEC
//%token T_INT_CAST
//%token T_DOUBLE_CAST
//%token T_STRING_CAST
//%token T_ARRAY_CAST
//%token T_OBJECT_CAST
//%token T_BOOL_CAST
//%token T_UNSET_CAST
//%token T_OBJECT_OPERATOR
//%token T_NULLSAFE_OBJECT_OPERATOR
//%token T_DOUBLE_ARROW
//%token T_COMMENT
//%token T_DOC_COMMENT
//%token T_OPEN_TAG
//%token T_OPEN_TAG_WITH_ECHO
//%token T_CLOSE_TAG
//%token T_WHITESPACE
//%token T_START_HEREDOC
//%token T_END_HEREDOC
//%token T_DOLLAR_OPEN_CURLY_BRACES
//%token T_CURLY_OPEN
//%token T_PAAMAYIM_NEKUDOTAYIM
//%token T_NS_SEPARATOR
//%token T_ELLIPSIS
//%token T_COALESCE
//%token T_POW
//%token T_POW_EQUAL
//%token T_AMPERSAND_FOLLOWED_BY_VAR_OR_VARARG
//%token T_AMPERSAND_NOT_FOLLOWED_BY_VAR_OR_VARARG
//%token T_BAD_CHARACTER
//%token T_ERROR
//%token ','
//%token ']'
//%token '('
//%token ')'
//%token ';'
//%token '{'
//%token '}'
//%token '['
//%token '`'
//%token '"'
//%token '$'

%precedence /*1*/ T_THROW ;
%precedence /*2*/ PREC_ARROW_FUNCTION ;
%precedence /*3*/ T_INCLUDE T_INCLUDE_ONCE T_REQUIRE T_REQUIRE_ONCE ;
%left /*4*/ T_LOGICAL_OR ;
%left /*5*/ T_LOGICAL_XOR ;
%left /*6*/ T_LOGICAL_AND ;
%precedence /*7*/ T_PRINT ;
%precedence /*8*/ T_YIELD ;
%precedence /*9*/ T_DOUBLE_ARROW ;
%precedence /*10*/ T_YIELD_FROM ;
%precedence /*11*/ '=' T_PLUS_EQUAL T_MINUS_EQUAL T_MUL_EQUAL T_DIV_EQUAL T_CONCAT_EQUAL T_MOD_EQUAL T_AND_EQUAL T_OR_EQUAL T_XOR_EQUAL T_SL_EQUAL T_SR_EQUAL T_COALESCE_EQUAL T_POW_EQUAL ;
%left /*12*/ '?' ':' ;
%right /*13*/ T_COALESCE ;
%left /*14*/ T_BOOLEAN_OR ;
%left /*15*/ T_BOOLEAN_AND ;
%left /*16*/ '|' ;
%left /*17*/ '^' ;
%left /*18*/ T_AMPERSAND_FOLLOWED_BY_VAR_OR_VARARG T_AMPERSAND_NOT_FOLLOWED_BY_VAR_OR_VARARG ;
%nonassoc /*19*/ T_IS_EQUAL T_IS_NOT_EQUAL T_IS_IDENTICAL T_IS_NOT_IDENTICAL T_SPACESHIP ;
%nonassoc /*20*/ '<' '>' T_IS_SMALLER_OR_EQUAL T_IS_GREATER_OR_EQUAL ;
%left /*21*/ '.' ;
%left /*22*/ T_SL T_SR ;
%left /*23*/ '+' '-' ;
%left /*24*/ '*' '/' '%' ;
%precedence /*25*/ '!' ;
%precedence /*26*/ T_INSTANCEOF ;
%precedence /*27*/ '~' '@' T_INT_CAST T_DOUBLE_CAST T_STRING_CAST T_ARRAY_CAST T_OBJECT_CAST T_BOOL_CAST T_UNSET_CAST ;
%right /*28*/ T_POW ;
%precedence /*29*/ T_CLONE ;
%precedence /*30*/ T_NOELSE ;
%precedence /*31*/ T_ELSEIF ;
%precedence /*32*/ T_ELSE ;

//%start start

//%%

start :
	/*END
	|*/ top_statement_list
	;

reserved_non_modifiers :
	T_INCLUDE /*3P*/
	| T_INCLUDE_ONCE /*3P*/
	| T_EVAL
	| T_REQUIRE /*3P*/
	| T_REQUIRE_ONCE /*3P*/
	| T_LOGICAL_OR /*4L*/
	| T_LOGICAL_XOR /*5L*/
	| T_LOGICAL_AND /*6L*/
	| T_INSTANCEOF /*26P*/
	| T_NEW
	| T_CLONE /*29P*/
	| T_EXIT
	| T_IF
	| T_ELSEIF /*31P*/
	| T_ELSE /*32P*/
	| T_ENDIF
	| T_ECHO
	| T_DO
	| T_WHILE
	| T_ENDWHILE
	| T_FOR
	| T_ENDFOR
	| T_FOREACH
	| T_ENDFOREACH
	| T_DECLARE
	| T_ENDDECLARE
	| T_AS
	| T_TRY
	| T_CATCH
	| T_FINALLY
	| T_THROW /*1P*/
	| T_USE
	| T_INSTEADOF
	| T_GLOBAL
	| T_VAR
	| T_UNSET
	| T_ISSET
	| T_EMPTY
	| T_CONTINUE
	| T_GOTO
	| T_FUNCTION
	| T_CONST
	| T_RETURN
	| T_PRINT /*7P*/
	| T_YIELD /*8P*/
	| T_LIST
	| T_SWITCH
	| T_ENDSWITCH
	| T_CASE
	| T_DEFAULT
	| T_BREAK
	| T_ARRAY
	| T_CALLABLE
	| T_EXTENDS
	| T_IMPLEMENTS
	| T_NAMESPACE
	| T_TRAIT
	| T_INTERFACE
	| T_CLASS
	| T_CLASS_C
	| T_TRAIT_C
	| T_FUNC_C
	| T_METHOD_C
	| T_LINE
	| T_FILE
	| T_DIR
	| T_NS_C
	| T_FN
	| T_MATCH
	| T_ENUM
	;

semi_reserved :
	reserved_non_modifiers
	| T_STATIC
	| T_ABSTRACT
	| T_FINAL
	| T_PRIVATE
	| T_PROTECTED
	| T_PUBLIC
	| T_READONLY
	;

ampersand :
	T_AMPERSAND_FOLLOWED_BY_VAR_OR_VARARG /*18L*/
	| T_AMPERSAND_NOT_FOLLOWED_BY_VAR_OR_VARARG /*18L*/
	;

identifier :
	T_STRING
	| semi_reserved
	;

top_statement_list :
	top_statement_list top_statement
	| /*empty*/
	;

namespace_declaration_name :
	identifier
	| T_NAME_QUALIFIED
	;

namespace_name :
	T_STRING
	| T_NAME_QUALIFIED
	;

legacy_namespace_name :
	namespace_name
	| T_NAME_FULLY_QUALIFIED
	;

name :
	T_STRING
	| T_NAME_QUALIFIED
	| T_NAME_FULLY_QUALIFIED
	| T_NAME_RELATIVE
	;

attribute_decl :
	class_name
	| class_name argument_list
	;

attribute_group :
	attribute_decl
	| attribute_group ',' attribute_decl
	;

attribute :
	T_ATTRIBUTE attribute_group possible_comma ']'
	;

attributes :
	attribute
	| attributes attribute
	;

attributed_statement :
	function_declaration_statement
	| class_declaration_statement
	| trait_declaration_statement
	| interface_declaration_statement
	| enum_declaration_statement
	;

top_statement :
	statement
	| attributed_statement
	| attributes attributed_statement
	| T_HALT_COMPILER '(' ')' ';'
	| T_NAMESPACE namespace_declaration_name ';'
	| T_NAMESPACE namespace_declaration_name '{' top_statement_list '}'
	| T_NAMESPACE '{' top_statement_list '}'
	| T_USE mixed_group_use_declaration ';'
	| T_USE use_type group_use_declaration ';'
	| T_USE use_declarations ';'
	| T_USE use_type use_declarations ';'
	| T_CONST const_list ';'
	;

use_type :
	T_FUNCTION
	| T_CONST
	;

group_use_declaration :
	legacy_namespace_name T_NS_SEPARATOR '{' unprefixed_use_declarations possible_comma '}'
	;

mixed_group_use_declaration :
	legacy_namespace_name T_NS_SEPARATOR '{' inline_use_declarations possible_comma '}'
	;

possible_comma :
	/*empty*/
	| ','
	;

inline_use_declarations :
	inline_use_declarations ',' inline_use_declaration
	| inline_use_declaration
	;

unprefixed_use_declarations :
	unprefixed_use_declarations ',' unprefixed_use_declaration
	| unprefixed_use_declaration
	;

use_declarations :
	use_declarations ',' use_declaration
	| use_declaration
	;

inline_use_declaration :
	unprefixed_use_declaration
	| use_type unprefixed_use_declaration
	;

unprefixed_use_declaration :
	namespace_name
	| namespace_name T_AS T_STRING
	;

use_declaration :
	legacy_namespace_name
	| legacy_namespace_name T_AS T_STRING
	;

const_list :
	const_list ',' const_decl
	| const_decl
	;

inner_statement_list :
	inner_statement_list inner_statement
	| /*empty*/
	;

inner_statement :
	statement
	| attributed_statement
	| attributes attributed_statement
	| T_HALT_COMPILER '(' ')' ';'
	;

statement :
	'{' inner_statement_list '}'
	| if_stmt
	| alt_if_stmt
	| T_WHILE '(' expr ')' while_statement
	| T_DO statement T_WHILE '(' expr ')' ';'
	| T_FOR '(' for_exprs ';' for_exprs ';' for_exprs ')' for_statement
	| T_SWITCH '(' expr ')' switch_case_list
	| T_BREAK optional_expr ';'
	| T_CONTINUE optional_expr ';'
	| T_RETURN optional_expr ';'
	| T_GLOBAL global_var_list ';'
	| T_STATIC static_var_list ';'
	| T_ECHO echo_expr_list ';'
	| T_INLINE_HTML
	| expr ';'
	| T_UNSET '(' unset_variables possible_comma ')' ';'
	| T_FOREACH '(' expr T_AS foreach_variable ')' foreach_statement
	| T_FOREACH '(' expr T_AS foreach_variable T_DOUBLE_ARROW /*9P*/ foreach_variable ')' foreach_statement
	| T_DECLARE '(' const_list ')' declare_statement
	| ';'
	| T_TRY '{' inner_statement_list '}' catch_list finally_statement
	| T_GOTO T_STRING ';'
	| T_STRING ':' /*12L*/
	;

catch_list :
	/*empty*/
	| catch_list T_CATCH '(' catch_name_list optional_variable ')' '{' inner_statement_list '}'
	;

catch_name_list :
	class_name
	| catch_name_list '|' /*16L*/ class_name
	;

optional_variable :
	/*empty*/
	| T_VARIABLE
	;

finally_statement :
	/*empty*/
	| T_FINALLY '{' inner_statement_list '}'
	;

unset_variables :
	unset_variable
	| unset_variables ',' unset_variable
	;

unset_variable :
	variable
	;

function_name :
	T_STRING
	| T_READONLY
	;

function_declaration_statement :
	function returns_ref function_name backup_doc_comment '(' parameter_list ')' return_type backup_fn_flags '{' inner_statement_list '}' backup_fn_flags
	;

is_reference :
	/*empty*/
	| T_AMPERSAND_FOLLOWED_BY_VAR_OR_VARARG /*18L*/
	;

is_variadic :
	/*empty*/
	| T_ELLIPSIS
	;

class_declaration_statement :
	class_modifiers T_CLASS T_STRING extends_from implements_list backup_doc_comment '{' class_statement_list '}'
	| T_CLASS T_STRING extends_from implements_list backup_doc_comment '{' class_statement_list '}'
	;

class_modifiers :
	class_modifier
	| class_modifiers class_modifier
	;

class_modifier :
	T_ABSTRACT
	| T_FINAL
	| T_READONLY
	;

trait_declaration_statement :
	T_TRAIT T_STRING backup_doc_comment '{' class_statement_list '}'
	;

interface_declaration_statement :
	T_INTERFACE T_STRING interface_extends_list backup_doc_comment '{' class_statement_list '}'
	;

enum_declaration_statement :
	T_ENUM T_STRING enum_backing_type implements_list backup_doc_comment '{' class_statement_list '}'
	;

enum_backing_type :
	/*empty*/
	| ':' /*12L*/ type_expr
	;

enum_case :
	T_CASE backup_doc_comment identifier enum_case_expr ';'
	;

enum_case_expr :
	/*empty*/
	| '=' /*11P*/ expr
	;

extends_from :
	/*empty*/
	| T_EXTENDS class_name
	;

interface_extends_list :
	/*empty*/
	| T_EXTENDS class_name_list
	;

implements_list :
	/*empty*/
	| T_IMPLEMENTS class_name_list
	;

foreach_variable :
	variable
	| ampersand variable
	| T_LIST '(' array_pair_list ')'
	| '[' array_pair_list ']'
	;

for_statement :
	statement
	| ':' /*12L*/ inner_statement_list T_ENDFOR ';'
	;

foreach_statement :
	statement
	| ':' /*12L*/ inner_statement_list T_ENDFOREACH ';'
	;

declare_statement :
	statement
	| ':' /*12L*/ inner_statement_list T_ENDDECLARE ';'
	;

switch_case_list :
	'{' case_list '}'
	| '{' ';' case_list '}'
	| ':' /*12L*/ case_list T_ENDSWITCH ';'
	| ':' /*12L*/ ';' case_list T_ENDSWITCH ';'
	;

case_list :
	/*empty*/
	| case_list T_CASE expr case_separator inner_statement_list
	| case_list T_DEFAULT case_separator inner_statement_list
	;

case_separator :
	':' /*12L*/
	| ';'
	;

match :
	T_MATCH '(' expr ')' '{' match_arm_list '}'
	;

match_arm_list :
	/*empty*/
	| non_empty_match_arm_list possible_comma
	;

non_empty_match_arm_list :
	match_arm
	| non_empty_match_arm_list ',' match_arm
	;

match_arm :
	match_arm_cond_list possible_comma T_DOUBLE_ARROW /*9P*/ expr
	| T_DEFAULT possible_comma T_DOUBLE_ARROW /*9P*/ expr
	;

match_arm_cond_list :
	expr
	| match_arm_cond_list ',' expr
	;

while_statement :
	statement
	| ':' /*12L*/ inner_statement_list T_ENDWHILE ';'
	;

if_stmt_without_else :
	T_IF '(' expr ')' statement
	| if_stmt_without_else T_ELSEIF /*31P*/ '(' expr ')' statement
	;

if_stmt :
	if_stmt_without_else %prec T_NOELSE /*30P*/
	| if_stmt_without_else T_ELSE /*32P*/ statement
	;

alt_if_stmt_without_else :
	T_IF '(' expr ')' ':' /*12L*/ inner_statement_list
	| alt_if_stmt_without_else T_ELSEIF /*31P*/ '(' expr ')' ':' /*12L*/ inner_statement_list
	;

alt_if_stmt :
	alt_if_stmt_without_else T_ENDIF ';'
	| alt_if_stmt_without_else T_ELSE /*32P*/ ':' /*12L*/ inner_statement_list T_ENDIF ';'
	;

parameter_list :
	non_empty_parameter_list possible_comma
	| /*empty*/
	;

non_empty_parameter_list :
	attributed_parameter
	| non_empty_parameter_list ',' attributed_parameter
	;

attributed_parameter :
	attributes parameter
	| parameter
	;

optional_property_modifiers :
	/*empty*/
	| optional_property_modifiers property_modifier
	;

property_modifier :
	T_PUBLIC
	| T_PROTECTED
	| T_PRIVATE
	| T_READONLY
	;

parameter :
	optional_property_modifiers optional_type_without_static is_reference is_variadic T_VARIABLE backup_doc_comment
	| optional_property_modifiers optional_type_without_static is_reference is_variadic T_VARIABLE backup_doc_comment '=' /*11P*/ expr
	;

optional_type_without_static :
	/*empty*/
	| type_expr_without_static
	;

type_expr :
	type
	| '?' /*12L*/ type
	| union_type
	| intersection_type
	;

type :
	type_without_static
	| T_STATIC
	;

union_type_element :
	type
	| '(' intersection_type ')'
	;

union_type :
	union_type_element '|' /*16L*/ union_type_element
	| union_type '|' /*16L*/ union_type_element
	;

intersection_type :
	type T_AMPERSAND_NOT_FOLLOWED_BY_VAR_OR_VARARG /*18L*/ type
	| intersection_type T_AMPERSAND_NOT_FOLLOWED_BY_VAR_OR_VARARG /*18L*/ type
	;

type_expr_without_static :
	type_without_static
	| '?' /*12L*/ type_without_static
	| union_type_without_static
	| intersection_type_without_static
	;

type_without_static :
	T_ARRAY
	| T_CALLABLE
	| name
	;

union_type_without_static_element :
	type_without_static
	| '(' intersection_type_without_static ')'
	;

union_type_without_static :
	union_type_without_static_element '|' /*16L*/ union_type_without_static_element
	| union_type_without_static '|' /*16L*/ union_type_without_static_element
	;

intersection_type_without_static :
	type_without_static T_AMPERSAND_NOT_FOLLOWED_BY_VAR_OR_VARARG /*18L*/ type_without_static
	| intersection_type_without_static T_AMPERSAND_NOT_FOLLOWED_BY_VAR_OR_VARARG /*18L*/ type_without_static
	;

return_type :
	/*empty*/
	| ':' /*12L*/ type_expr
	;

argument_list :
	'(' ')'
	| '(' non_empty_argument_list possible_comma ')'
	| '(' T_ELLIPSIS ')'
	;

non_empty_argument_list :
	argument
	| non_empty_argument_list ',' argument
	;

argument :
	expr
	| identifier ':' /*12L*/ expr
	| T_ELLIPSIS expr
	;

global_var_list :
	global_var_list ',' global_var
	| global_var
	;

global_var :
	simple_variable
	;

static_var_list :
	static_var_list ',' static_var
	| static_var
	;

static_var :
	T_VARIABLE
	| T_VARIABLE '=' /*11P*/ expr
	;

class_statement_list :
	class_statement_list class_statement
	| /*empty*/
	;

attributed_class_statement :
	variable_modifiers optional_type_without_static property_list ';'
	| method_modifiers T_CONST class_const_list ';'
	| method_modifiers function returns_ref identifier backup_doc_comment '(' parameter_list ')' return_type backup_fn_flags method_body backup_fn_flags
	| enum_case
	;

class_statement :
	attributed_class_statement
	| attributes attributed_class_statement
	| T_USE class_name_list trait_adaptations
	;

class_name_list :
	class_name
	| class_name_list ',' class_name
	;

trait_adaptations :
	';'
	| '{' '}'
	| '{' trait_adaptation_list '}'
	;

trait_adaptation_list :
	trait_adaptation
	| trait_adaptation_list trait_adaptation
	;

trait_adaptation :
	trait_precedence ';'
	| trait_alias ';'
	;

trait_precedence :
	absolute_trait_method_reference T_INSTEADOF class_name_list
	;

trait_alias :
	trait_method_reference T_AS T_STRING
	| trait_method_reference T_AS reserved_non_modifiers
	| trait_method_reference T_AS member_modifier identifier
	| trait_method_reference T_AS member_modifier
	;

trait_method_reference :
	identifier
	| absolute_trait_method_reference
	;

absolute_trait_method_reference :
	class_name T_PAAMAYIM_NEKUDOTAYIM identifier
	;

method_body :
	';'
	| '{' inner_statement_list '}'
	;

variable_modifiers :
	non_empty_member_modifiers
	| T_VAR
	;

method_modifiers :
	/*empty*/
	| non_empty_member_modifiers
	;

non_empty_member_modifiers :
	member_modifier
	| non_empty_member_modifiers member_modifier
	;

member_modifier :
	T_PUBLIC
	| T_PROTECTED
	| T_PRIVATE
	| T_STATIC
	| T_ABSTRACT
	| T_FINAL
	| T_READONLY
	;

property_list :
	property_list ',' property
	| property
	;

property :
	T_VARIABLE backup_doc_comment
	| T_VARIABLE '=' /*11P*/ expr backup_doc_comment
	;

class_const_list :
	class_const_list ',' class_const_decl
	| class_const_decl
	;

class_const_decl :
	identifier '=' /*11P*/ expr backup_doc_comment
	;

const_decl :
	T_STRING '=' /*11P*/ expr backup_doc_comment
	;

echo_expr_list :
	echo_expr_list ',' echo_expr
	| echo_expr
	;

echo_expr :
	expr
	;

for_exprs :
	/*empty*/
	| non_empty_for_exprs
	;

non_empty_for_exprs :
	non_empty_for_exprs ',' expr
	| expr
	;

anonymous_class :
	T_CLASS ctor_arguments extends_from implements_list backup_doc_comment '{' class_statement_list '}'
	;

new_expr :
	T_NEW class_name_reference ctor_arguments
	| T_NEW anonymous_class
	| T_NEW attributes anonymous_class
	;

expr :
	variable
	| T_LIST '(' array_pair_list ')' '=' /*11P*/ expr
	| '[' array_pair_list ']' '=' /*11P*/ expr
	| variable '=' /*11P*/ expr
	| variable '=' /*11P*/ ampersand variable
	| T_CLONE /*29P*/ expr
	| variable T_PLUS_EQUAL /*11P*/ expr
	| variable T_MINUS_EQUAL /*11P*/ expr
	| variable T_MUL_EQUAL /*11P*/ expr
	| variable T_POW_EQUAL /*11P*/ expr
	| variable T_DIV_EQUAL /*11P*/ expr
	| variable T_CONCAT_EQUAL /*11P*/ expr
	| variable T_MOD_EQUAL /*11P*/ expr
	| variable T_AND_EQUAL /*11P*/ expr
	| variable T_OR_EQUAL /*11P*/ expr
	| variable T_XOR_EQUAL /*11P*/ expr
	| variable T_SL_EQUAL /*11P*/ expr
	| variable T_SR_EQUAL /*11P*/ expr
	| variable T_COALESCE_EQUAL /*11P*/ expr
	| variable T_INC
	| T_INC variable
	| variable T_DEC
	| T_DEC variable
	| expr T_BOOLEAN_OR /*14L*/ expr
	| expr T_BOOLEAN_AND /*15L*/ expr
	| expr T_LOGICAL_OR /*4L*/ expr
	| expr T_LOGICAL_AND /*6L*/ expr
	| expr T_LOGICAL_XOR /*5L*/ expr
	| expr '|' /*16L*/ expr
	| expr T_AMPERSAND_NOT_FOLLOWED_BY_VAR_OR_VARARG /*18L*/ expr
	| expr T_AMPERSAND_FOLLOWED_BY_VAR_OR_VARARG /*18L*/ expr
	| expr '^' /*17L*/ expr
	| expr '.' /*21L*/ expr
	| expr '+' /*23L*/ expr
	| expr '-' /*23L*/ expr
	| expr '*' /*24L*/ expr
	| expr T_POW /*28R*/ expr
	| expr '/' /*24L*/ expr
	| expr '%' /*24L*/ expr
	| expr T_SL /*22L*/ expr
	| expr T_SR /*22L*/ expr
	| '+' /*23L*/ expr %prec '~' /*27P*/
	| '-' /*23L*/ expr %prec '~' /*27P*/
	| '!' /*25P*/ expr
	| '~' /*27P*/ expr
	| expr T_IS_IDENTICAL /*19N*/ expr
	| expr T_IS_NOT_IDENTICAL /*19N*/ expr
	| expr T_IS_EQUAL /*19N*/ expr
	| expr T_IS_NOT_EQUAL /*19N*/ expr
	| expr '<' /*20N*/ expr
	| expr T_IS_SMALLER_OR_EQUAL /*20N*/ expr
	| expr '>' /*20N*/ expr
	| expr T_IS_GREATER_OR_EQUAL /*20N*/ expr
	| expr T_SPACESHIP /*19N*/ expr
	| expr T_INSTANCEOF /*26P*/ class_name_reference
	| '(' expr ')'
	| new_expr
	| expr '?' /*12L*/ expr ':' /*12L*/ expr
	| expr '?' /*12L*/ ':' /*12L*/ expr
	| expr T_COALESCE /*13R*/ expr
	| internal_functions_in_yacc
	| T_INT_CAST /*27P*/ expr
	| T_DOUBLE_CAST /*27P*/ expr
	| T_STRING_CAST /*27P*/ expr
	| T_ARRAY_CAST /*27P*/ expr
	| T_OBJECT_CAST /*27P*/ expr
	| T_BOOL_CAST /*27P*/ expr
	| T_UNSET_CAST /*27P*/ expr
	| T_EXIT exit_expr
	| '@' /*27P*/ expr
	| scalar
	| '`' backticks_expr '`'
	| T_PRINT /*7P*/ expr
	| T_YIELD /*8P*/
	| T_YIELD /*8P*/ expr
	| T_YIELD /*8P*/ expr T_DOUBLE_ARROW /*9P*/ expr
	| T_YIELD_FROM /*10P*/ expr
	| T_THROW /*1P*/ expr
	| inline_function
	| attributes inline_function
	| T_STATIC inline_function
	| attributes T_STATIC inline_function
	| match
	;

inline_function :
	function returns_ref backup_doc_comment '(' parameter_list ')' lexical_vars return_type backup_fn_flags '{' inner_statement_list '}' backup_fn_flags
	| fn returns_ref backup_doc_comment '(' parameter_list ')' return_type T_DOUBLE_ARROW /*9P*/ backup_fn_flags backup_lex_pos expr backup_fn_flags
	;

fn :
	T_FN
	;

function :
	T_FUNCTION
	;

backup_doc_comment :
	/*empty*/
	;

backup_fn_flags :
	%prec PREC_ARROW_FUNCTION /*2P*/ /*empty*/
	;

backup_lex_pos :
	/*empty*/
	;

returns_ref :
	/*empty*/
	| ampersand
	;

lexical_vars :
	/*empty*/
	| T_USE '(' lexical_var_list possible_comma ')'
	;

lexical_var_list :
	lexical_var_list ',' lexical_var
	| lexical_var
	;

lexical_var :
	T_VARIABLE
	| ampersand T_VARIABLE
	;

function_call :
	name argument_list
	| T_READONLY argument_list
	| class_name T_PAAMAYIM_NEKUDOTAYIM member_name argument_list
	| variable_class_name T_PAAMAYIM_NEKUDOTAYIM member_name argument_list
	| callable_expr argument_list
	;

class_name :
	T_STATIC
	| name
	;

class_name_reference :
	class_name
	| new_variable
	| '(' expr ')'
	;

exit_expr :
	/*empty*/
	| '(' optional_expr ')'
	;

backticks_expr :
	/*empty*/
	| T_ENCAPSED_AND_WHITESPACE
	| encaps_list
	;

ctor_arguments :
	/*empty*/
	| argument_list
	;

dereferenceable_scalar :
	T_ARRAY '(' array_pair_list ')'
	| '[' array_pair_list ']'
	| T_CONSTANT_ENCAPSED_STRING
	| '"' encaps_list '"'
	;

scalar :
	T_LNUMBER
	| T_DNUMBER
	| T_START_HEREDOC T_ENCAPSED_AND_WHITESPACE T_END_HEREDOC
	| T_START_HEREDOC T_END_HEREDOC
	| T_START_HEREDOC encaps_list T_END_HEREDOC
	| dereferenceable_scalar
	| constant
	| class_constant
	;

constant :
	name
	| T_LINE
	| T_FILE
	| T_DIR
	| T_TRAIT_C
	| T_METHOD_C
	| T_FUNC_C
	| T_NS_C
	| T_CLASS_C
	;

class_constant :
	class_name T_PAAMAYIM_NEKUDOTAYIM identifier
	| variable_class_name T_PAAMAYIM_NEKUDOTAYIM identifier
	;

optional_expr :
	/*empty*/
	| expr
	;

variable_class_name :
	fully_dereferenceable
	;

fully_dereferenceable :
	variable
	| '(' expr ')'
	| dereferenceable_scalar
	| class_constant
	;

array_object_dereferenceable :
	fully_dereferenceable
	| constant
	;

callable_expr :
	callable_variable
	| '(' expr ')'
	| dereferenceable_scalar
	;

callable_variable :
	simple_variable
	| array_object_dereferenceable '[' optional_expr ']'
	| array_object_dereferenceable '{' expr '}'
	| array_object_dereferenceable T_OBJECT_OPERATOR property_name argument_list
	| array_object_dereferenceable T_NULLSAFE_OBJECT_OPERATOR property_name argument_list
	| function_call
	;

variable :
	callable_variable
	| static_member
	| array_object_dereferenceable T_OBJECT_OPERATOR property_name
	| array_object_dereferenceable T_NULLSAFE_OBJECT_OPERATOR property_name
	;

simple_variable :
	T_VARIABLE
	| '$' '{' expr '}'
	| '$' simple_variable
	;

static_member :
	class_name T_PAAMAYIM_NEKUDOTAYIM simple_variable
	| variable_class_name T_PAAMAYIM_NEKUDOTAYIM simple_variable
	;

new_variable :
	simple_variable
	| new_variable '[' optional_expr ']'
	| new_variable '{' expr '}'
	| new_variable T_OBJECT_OPERATOR property_name
	| new_variable T_NULLSAFE_OBJECT_OPERATOR property_name
	| class_name T_PAAMAYIM_NEKUDOTAYIM simple_variable
	| new_variable T_PAAMAYIM_NEKUDOTAYIM simple_variable
	;

member_name :
	identifier
	| '{' expr '}'
	| simple_variable
	;

property_name :
	T_STRING
	| '{' expr '}'
	| simple_variable
	;

array_pair_list :
	non_empty_array_pair_list
	;

possible_array_pair :
	/*empty*/
	| array_pair
	;

non_empty_array_pair_list :
	non_empty_array_pair_list ',' possible_array_pair
	| possible_array_pair
	;

array_pair :
	expr T_DOUBLE_ARROW /*9P*/ expr
	| expr
	| expr T_DOUBLE_ARROW /*9P*/ ampersand variable
	| ampersand variable
	| T_ELLIPSIS expr
	| expr T_DOUBLE_ARROW /*9P*/ T_LIST '(' array_pair_list ')'
	| T_LIST '(' array_pair_list ')'
	;

encaps_list :
	encaps_list encaps_var
	| encaps_list T_ENCAPSED_AND_WHITESPACE
	| encaps_var
	| T_ENCAPSED_AND_WHITESPACE encaps_var
	;

encaps_var :
	T_VARIABLE
	| T_VARIABLE '[' encaps_var_offset ']'
	| T_VARIABLE T_OBJECT_OPERATOR T_STRING
	| T_VARIABLE T_NULLSAFE_OBJECT_OPERATOR T_STRING
	| T_DOLLAR_OPEN_CURLY_BRACES expr '}'
	| T_DOLLAR_OPEN_CURLY_BRACES T_STRING_VARNAME '}'
	| T_DOLLAR_OPEN_CURLY_BRACES T_STRING_VARNAME '[' expr ']' '}'
	| T_CURLY_OPEN variable '}'
	;

encaps_var_offset :
	T_STRING
	| T_NUM_STRING
	| '-' /*23L*/ T_NUM_STRING
	| T_VARIABLE
	;

internal_functions_in_yacc :
	T_ISSET '(' isset_variables possible_comma ')'
	| T_EMPTY '(' expr ')'
	| T_INCLUDE /*3P*/ expr
	| T_INCLUDE_ONCE /*3P*/ expr
	| T_EVAL '(' expr ')'
	| T_REQUIRE /*3P*/ expr
	| T_REQUIRE_ONCE /*3P*/ expr
	;

isset_variables :
	isset_variable
	| isset_variables ',' isset_variable
	;

isset_variable :
	expr
	;

//Lexer

T_THROW : 'throw' ;
T_INCLUDE : 'include' ;
T_INCLUDE_ONCE : 'include_once' ;
T_REQUIRE : 'require' ;
T_REQUIRE_ONCE : 'require_once' ;
T_LOGICAL_OR : '||' ;
T_LOGICAL_XOR : 'LOGICAL_XOR' ;
T_LOGICAL_AND : '&&' ;
T_PRINT : 'print' ;
T_YIELD : 'yield' ;
T_DOUBLE_ARROW : '=>' ;
T_YIELD_FROM : 'YIELD_FROM' ;
T_PLUS_EQUAL : '+=' ;
T_MINUS_EQUAL : '-=' ;
T_MUL_EQUAL : '*=' ;
T_DIV_EQUAL : '/=' ;
T_CONCAT_EQUAL : 'CONCAT_EQUAL' ;
T_MOD_EQUAL : '%=' ;
T_AND_EQUAL : '&=' ;
T_OR_EQUAL : '|=' ;
T_XOR_EQUAL : 'XOR_EQUAL' ;
T_SL_EQUAL : '<<=' ;
T_SR_EQUAL : '>>=' ;
T_COALESCE_EQUAL : 'COALESCE_EQUAL' ;
T_POW_EQUAL : 'POW_EQUAL' ;
T_COALESCE : 'COALESCE' ;
T_BOOLEAN_OR : 'BOOLEAN_OR' ;
T_BOOLEAN_AND : 'BOOLEAN_AND' ;
T_AMPERSAND_FOLLOWED_BY_VAR_OR_VARARG : 'AMPERSAND_FOLLOWED_BY_VAR_OR_VARARG' ;
T_AMPERSAND_NOT_FOLLOWED_BY_VAR_OR_VARARG : 'AMPERSAND_NOT_FOLLOWED_BY_VAR_OR_VARARG' ;
T_IS_EQUAL : '==' ;
T_IS_NOT_EQUAL : '!=' ;
T_IS_IDENTICAL : '===' ;
T_IS_NOT_IDENTICAL : '!==' ;
T_SPACESHIP : '<=>' ;
T_IS_SMALLER_OR_EQUAL : '<=' ;
T_IS_GREATER_OR_EQUAL : '>=' ;
T_SL : '<<' ;
T_SR : '>>' ;
T_INSTANCEOF : 'instanceof' ;
T_INT_CAST : 'INT_CAST' ;
T_DOUBLE_CAST : 'DOUBLE_CAST' ;
T_STRING_CAST : 'STRING_CAST' ;
T_ARRAY_CAST : 'ARRAY_CAST' ;
T_OBJECT_CAST : 'OBJECT_CAST' ;
T_BOOL_CAST : 'BOOL_CAST' ;
T_UNSET_CAST : 'UNSET_CAST' ;
T_POW : 'POW' ;
T_CLONE : 'clone' ;
T_ELSEIF : 'elseif' ;
T_ELSE : 'else' ;
T_EVAL : 'eval' ;
T_NEW : 'new' ;
T_EXIT : 'exit' ;
T_IF : 'if' ;
T_ENDIF : 'endif' ;
T_ECHO : 'echo' ;
T_DO : 'do' ;
T_WHILE : 'while' ;
T_ENDWHILE : 'endwhile' ;
T_FOR : 'for' ;
T_ENDFOR : 'endfor' ;
T_FOREACH : 'foreach' ;
T_ENDFOREACH : 'endforeach' ;
T_DECLARE : 'declare' ;
T_ENDDECLARE : 'enddeclare' ;
T_AS : 'as' ;
T_TRY : 'try' ;
T_CATCH : 'catch' ;
T_FINALLY : 'finally' ;
T_USE : 'use' ;
T_INSTEADOF : 'insteadof' ;
T_GLOBAL : 'global' ;
T_VAR : 'var' ;
T_UNSET : 'unset' ;
T_ISSET : 'isset' ;
T_EMPTY : 'empty' ;
T_CONTINUE : 'continue' ;
T_GOTO : 'goto' ;
T_FUNCTION : 'function' ;
T_CONST : 'const' ;
T_RETURN : 'return' ;
T_LIST : 'list' ;
T_SWITCH : 'switch' ;
T_ENDSWITCH : 'endswitch' ;
T_CASE : 'case' ;
T_DEFAULT : 'default' ;
T_BREAK : 'break' ;
T_ARRAY : 'array' ;
T_CALLABLE : 'callable' ;
T_EXTENDS : 'extends' ;
T_IMPLEMENTS : 'implements' ;
T_NAMESPACE : 'namespace' ;
T_TRAIT : 'trait' ;
T_INTERFACE : 'interface' ;
T_CLASS : 'class' ;
T_CLASS_C : 'CLASS_C' ;
T_TRAIT_C : 'TRAIT_C' ;
T_FUNC_C : 'FUNC_C' ;
T_METHOD_C : 'METHOD_C' ;
T_LINE : 'LINE' ;
T_FILE : 'FILE' ;
T_DIR : 'DIR' ;
T_NS_C : 'NS_C' ;
T_FN : 'fn' ;
T_MATCH : 'match' ;
T_ENUM : 'enum' ;
T_STATIC : 'static' ;
T_ABSTRACT : 'abstract' ;
T_FINAL : 'final' ;
T_PRIVATE : 'private' ;
T_PROTECTED : 'protected' ;
T_PUBLIC : 'public' ;
T_READONLY : 'readonly' ;
T_STRING : "[a-zA-Z_][a-zA-Z_0-9]*" ;
T_NAME_QUALIFIED : 'NAME_QUALIFIED' ;
T_NAME_FULLY_QUALIFIED : 'NAME_FULLY_QUALIFIED' ;
T_NAME_RELATIVE : 'NAME_RELATIVE' ;
T_ATTRIBUTE : 'attribute' ;
T_HALT_COMPILER : 'HALT_COMPILER' ;
T_NS_SEPARATOR : 'NS_SEPARATOR' ;
T_INLINE_HTML : 'INLINE_HTML' ;
T_VARIABLE : 'VARIABLE' ;
T_ELLIPSIS : '...' ;
T_PAAMAYIM_NEKUDOTAYIM : 'PAAMAYIM_NEKUDOTAYIM' ;
T_INC : '++' ;
T_DEC : '--' ;
T_ENCAPSED_AND_WHITESPACE : 'ENCAPSED_AND_WHITESPACE' ;
T_CONSTANT_ENCAPSED_STRING : 'CONSTANT_ENCAPSED_STRING' ;
T_LNUMBER : 'LNUMBER' ;
T_DNUMBER : 'DNUMBER' ;
T_START_HEREDOC : 'START_HEREDOC' ;
T_END_HEREDOC : 'END_HEREDOC' ;
T_OBJECT_OPERATOR : 'OBJECT_OPERATOR' ;
T_NULLSAFE_OBJECT_OPERATOR : 'NULLSAFE_OBJECT_OPERATOR' ;
T_DOLLAR_OPEN_CURLY_BRACES : '${' ;
T_STRING_VARNAME : 'STRING_VARNAME' ;
T_CURLY_OPEN : '{' ;
T_NUM_STRING : 'NUM_STRING' ;

}