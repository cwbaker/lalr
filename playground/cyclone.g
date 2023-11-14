cyclone {

%whitespace "[ \t\r\n]*";
%whitespace "//[^\n]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

/*Tokens*/
//%token YYEOF
//%token YYerror
//%token YYUNDEF
//%token AUTO
//%token REGISTER
//%token STATIC
//%token EXTERN
//%token TYPEDEF
//%token VOID
//%token CHAR
//%token SHORT
//%token INT
//%token LONG
//%token ISIZE_T
//%token USIZE_T
//%token FLOAT
//%token FLOAT128
//%token DOUBLE
//%token SIGNED
//%token UNSIGNED
//%token CONST
//%token VOLATILE
//%token RESTRICT
//%token STRUCT
//%token UNION
//%token CASE
//%token DEFAULT
//%token INLINE
//%token SIZEOF
//%token OFFSETOF
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
//%token ENUM
//%token TYPEOF
//%token BUILTIN_VA_LIST
//%token EXTENSION
//%token COMPLEX
//%token NULL_kw
//%token LET
//%token THROW
//%token TRY
//%token CATCH
//%token EXPORT
//%token OVERRIDE
//%token HIDE
//%token NEW
//%token QNEW
//%token ABSTRACT
//%token FALLTHRU
//%token USING
//%token NAMESPACE
//%token NOINFERENCE
//%token DATATYPE
//%token MALLOC
//%token RMALLOC
//%token RVMALLOC
//%token RMALLOC_INLINE
//%token QMALLOC
//%token CALLOC
//%token QCALLOC
//%token RCALLOC
//%token SWAP
//%token ASSERT
//%token REGION_T
//%token TAG_T
//%token REGION
//%token RNEW
//%token REGIONS
//%token PORTON
//%token PORTOFF
//%token PRAGMA
//%token TEMPESTON
//%token TEMPESTOFF
//%token AQ_ALIASABLE
//%token AQ_REFCNT
//%token AQ_RESTRICTED
//%token AQ_UNIQUE
//%token AQUAL_T
//%token NUMELTS
//%token TAGOF
//%token VALUEOF
//%token VALUEOF_T
//%token TAGCHECK
//%token NUMELTS_QUAL
//%token THIN_QUAL
//%token FAT_QUAL
//%token NOTNULL_QUAL
//%token NULLABLE_QUAL
//%token REQUIRES_QUAL
//%token ENSURES_QUAL
//%token EFFECT_QUAL
//%token THROWS_QUAL
//%token SUBSET_QUAL
//%token REGION_QUAL
//%token NOZEROTERM_QUAL
//%token ZEROTERM_QUAL
//%token TAGGED_QUAL
//%token ASSERT_QUAL
//%token ASSERT_FALSE_QUAL
//%token ALIAS_QUAL
//%token AQUALS
//%token CHECKS_QUAL
//%token EXTENSIBLE_QUAL
//%token AUTORELEASED_QUAL
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
//%token ELLIPSIS
//%token LEFT_RIGHT
//%token COLON_COLON
//%token IDENTIFIER
//%token INTEGER_CONSTANT
//%token STRING
//%token WSTRING
//%token CHARACTER_CONSTANT
//%token WCHARACTER_CONSTANT
//%token FLOATING_CONSTANT
//%token TYPE_VAR
//%token TYPEDEF_NAME
//%token QUAL_IDENTIFIER
//%token QUAL_TYPEDEF_NAME
//%token AQUAL_SHORT_CONST
//%token ATTRIBUTE
//%token ASM_TOK
//%token ';'
//%token '{'
//%token '}'
//%token ','
//%token '*'
//%token '='
//%token '<'
//%token '>'
//%token '('
//%token ')'
//%token '_'
//%token '$'
//%token '|'
//%token ':'
//%token '.'
//%token '['
//%token ']'
//%token '@'
//%token '?'
//%token '+'
//%token '^'
//%token '&'
//%token '-'
//%token '/'
//%token '%'
//%token '~'
//%token '!'
//%token 'A'
//%token 'V'


//%start prog_or_constraints

//%%

prog_or_constraints :
	/*YYEOF
	|*/ prog
	| all_constraints
	;

prog :
	translation_unit
	;

translation_unit :
	external_declaration translation_unit
	| using_action ';' translation_unit
	| using_action '{' translation_unit unusing_action translation_unit
	| namespace_action ';' translation_unit
	| namespace_action '{' translation_unit unnamespace_action translation_unit
	| noinference_action '{' translation_unit unnoinference_action translation_unit
	| extern_c_action '{' translation_unit end_extern_c override_opt export_list_opt hide_list_opt translation_unit
	| PORTON ';' translation_unit
	| PORTOFF ';' translation_unit
	| tempest_on_action ';' translation_unit
	| tempest_off_action ';' translation_unit
	| /*empty*/
	;

tempest_on_action :
	TEMPESTON
	;

tempest_off_action :
	TEMPESTOFF
	;

extern_c_action :
	EXTERN STRING
	;

end_extern_c :
	'}'
	;

hide_list_opt :
	/*empty*/
	| HIDE '{' hide_list_values '}'
	;

hide_list_values :
	struct_union_name
	| struct_union_name ';'
	| struct_union_name ',' hide_list_values
	;

export_list_opt :
	/*empty*/
	| export_list
	;

export_list :
	EXPORT '{' export_list_values '}'
	| EXPORT '{' '}'
	| EXPORT '{' '*' '}'
	;

export_list_values :
	struct_union_name optional_semi
	| struct_union_name ',' export_list_values
	;

override_opt :
	/*empty*/
	| OVERRIDE '{' translation_unit '}'
	;

external_declaration :
	function_definition
	| declaration
	| error
	;

optional_semi :
	';'
	| /*empty*/
	;

function_definition :
	declarator fndef_compound_statement optional_semi
	| declaration_specifiers declarator fndef_compound_statement optional_semi
	| declarator declaration_list fndef_compound_statement optional_semi
	| declaration_specifiers declarator declaration_list fndef_compound_statement optional_semi
	;

function_definition2 :
	declaration_specifiers declarator compound_statement
	| declaration_specifiers declarator declaration_list compound_statement
	;

using_action :
	USING qual_opt_identifier
	;

unusing_action :
	'}'
	;

namespace_action :
	NAMESPACE IDENTIFIER
	;

unnamespace_action :
	'}'
	;

noinference_action :
	NOINFERENCE
	;

unnoinference_action :
	'}'
	;

declaration :
	declaration_specifiers ';'
	| declaration_specifiers init_declarator_list_rev ';'
	| LET pattern '=' expression ';'
	| LET identifier_list ';'
	| REGION '<' TYPE_VAR '>' IDENTIFIER ';'
	| REGION IDENTIFIER open_opt ';'
	;

open_opt :
	/*empty*/
	| '=' IDENTIFIER '(' expression ')'
	;

declaration_list :
	declaration
	| declaration_list declaration
	;

declaration_specifiers :
	storage_class_specifier
	| storage_class_specifier declaration_specifiers
	| EXTENSION declaration_specifiers
	| type_specifier
	| type_specifier declaration_specifiers
	| type_qualifier
	| type_qualifier declaration_specifiers
	| INLINE
	| INLINE declaration_specifiers
	| attributes
	| attributes declaration_specifiers
	;

storage_class_specifier :
	AUTO
	| REGISTER
	| STATIC
	| EXTERN
	| EXTERN STRING
	| TYPEDEF
	| ABSTRACT
	;

attributes_opt :
	/*empty*/
	| attributes
	;

attributes :
	ATTRIBUTE '(' '(' attribute_list ')' ')'
	;

attribute_list :
	attribute
	| attribute ',' attribute_list
	;

attribute :
	IDENTIFIER
	| CONST
	| IDENTIFIER '(' assignment_expression ')'
	| IDENTIFIER '(' IDENTIFIER ',' INTEGER_CONSTANT ',' INTEGER_CONSTANT ')'
	;

type_specifier :
	type_specifier_notypedef
	| qual_opt_typedef type_params_opt
	;

type_specifier_notypedef :
	VOID
	| CHAR
	| SHORT
	| INT
	| ISIZE_T
	| USIZE_T
	| LONG
	| FLOAT
	| FLOAT128
	| DOUBLE
	| SIGNED
	| UNSIGNED
	| COMPLEX
	| enum_specifier
	| struct_or_union_specifier
	| TYPEOF '(' expression ')'
	| BUILTIN_VA_LIST
	| datatype_specifier
	| type_var
	| '_'
	| '_' COLON_COLON kind
	| '$' '(' parameter_list ')'
	| REGION_T '<' any_type_name right_angle
	| REGION_T
	| AQUAL_T '<' aqual_specifier right_angle
	| TAG_T '<' any_type_name right_angle
	| TAG_T
	| VALUEOF_T '(' expression ')'
	| SUBSET_QUAL '(' specifier_qualifier_list declarator_withtypedef '|' constant_expression ')'
	;

kind :
	field_name
	;

type_qualifier :
	CONST
	| VOLATILE
	| RESTRICT
	;

enum_specifier :
	ENUM qual_opt_identifier '{' enum_declaration_list '}'
	| ENUM qual_opt_identifier
	| ENUM '{' enum_declaration_list '}'
	;

enum_field :
	qual_opt_identifier
	| qual_opt_identifier '=' constant_expression
	;

enum_declaration_list :
	enum_field
	| enum_field ','
	| enum_field ',' enum_declaration_list
	;

struct_or_union_specifier :
	struct_or_union '{' struct_declaration_list '}'
	| maybe_tagged_struct_union struct_union_name type_params_opt '{' type_params_opt optional_effconstr_qualbnd struct_declaration_list '}'
	| maybe_tagged_struct_union struct_union_name type_params_opt
	;

maybe_tagged_struct_union :
	TAGGED_QUAL struct_or_union
	| struct_or_union
	;

struct_or_union :
	STRUCT
	| UNION
	;

type_params_opt :
	/*empty*/
	| '<' type_name_list right_angle
	;

struct_declaration_list :
	/*empty*/
	| struct_declaration_list0
	;

struct_declaration_list0 :
	struct_declaration
	| struct_declaration_list0 struct_declaration
	;

init_declarator_list_rev :
	init_declarator
	| init_declarator_list_rev ',' init_declarator
	;

init_declarator :
	declarator
	| declarator ASM_TOK asm_expr
	| declarator '=' initializer
	| declarator ASM_TOK asm_expr '=' initializer
	;

struct_declaration :
	specifier_qualifier_list struct_declarator_list_rev ';'
	;

specifier_qualifier_list :
	type_specifier
	| type_specifier notypedef_specifier_qualifier_list
	| type_qualifier
	| type_qualifier specifier_qualifier_list
	| attributes
	| attributes specifier_qualifier_list
	;

notypedef_specifier_qualifier_list :
	type_specifier_notypedef
	| type_specifier_notypedef notypedef_specifier_qualifier_list
	| type_qualifier
	| type_qualifier notypedef_specifier_qualifier_list
	| attributes
	| attributes notypedef_specifier_qualifier_list
	;

struct_declarator_list_rev :
	struct_declarator
	| struct_declarator_list_rev ',' struct_declarator
	;

struct_declarator :
	declarator_withtypedef requires_clause_opt
	| ':' constant_expression
	| /*empty*/
	| declarator_withtypedef ':' constant_expression
	;

requires_clause_opt :
	REQUIRES_QUAL '(' constant_expression ')'
	| /*empty*/
	;

datatype_specifier :
	qual_datatype qual_opt_identifier type_params_opt '{' datatypefield_list '}'
	| qual_datatype qual_opt_identifier type_params_opt
	| qual_datatype qual_opt_identifier '.' qual_opt_identifier type_params_opt
	;

qual_datatype :
	DATATYPE
	| EXTENSIBLE_QUAL DATATYPE
	;

datatypefield_list :
	datatypefield
	| datatypefield ';'
	| datatypefield ',' datatypefield_list
	| datatypefield ';' datatypefield_list
	;

datatypefield_scope :
	/*empty*/
	| EXTERN
	| STATIC
	;

datatypefield :
	datatypefield_scope qual_opt_identifier
	| datatypefield_scope qual_opt_identifier '(' parameter_list ')'
	;

declarator :
	direct_declarator
	| pointer direct_declarator
	;

declarator_withtypedef :
	direct_declarator_withtypedef
	| pointer direct_declarator_withtypedef
	;

direct_declarator :
	qual_opt_identifier
	| '(' declarator ')'
	| '(' attributes declarator ')'
	| direct_declarator '[' ']' zeroterm_qual_opt
	| direct_declarator '[' assignment_expression ']' zeroterm_qual_opt
	| direct_declarator '(' parameter_type_list ')' chk_req_ens_thr_opt
	| direct_declarator '(' identifier_list ')'
	| direct_declarator '<' type_name_list right_angle
	| direct_declarator attributes
	;

direct_declarator_withtypedef :
	qual_opt_identifier
	| qual_opt_typedef
	| '(' declarator_withtypedef ')'
	| '(' attributes declarator_withtypedef ')'
	| direct_declarator_withtypedef '[' ']' zeroterm_qual_opt
	| direct_declarator_withtypedef '[' assignment_expression ']' zeroterm_qual_opt
	| direct_declarator_withtypedef '(' parameter_type_list ')' chk_req_ens_thr_opt
	| direct_declarator_withtypedef '(' identifier_list ')'
	| direct_declarator_withtypedef '<' type_name_list right_angle
	| direct_declarator_withtypedef attributes
	;

pointer :
	one_pointer
	| one_pointer pointer
	;

one_pointer :
	pointer_null_and_bound pointer_quals eff_opt attributes_opt tqual_list
	;

pointer_quals :
	/*empty*/
	| pointer_qual pointer_quals
	;

pointer_qual :
	NUMELTS_QUAL '(' assignment_expression ')'
	| REGION_QUAL '(' any_type_name ')'
	| EFFECT_QUAL '(' eff_set ')'
	| THIN_QUAL
	| FAT_QUAL
	| AUTORELEASED_QUAL
	| ZEROTERM_QUAL
	| NOZEROTERM_QUAL
	| NOTNULL_QUAL
	| NULLABLE_QUAL
	| ALIAS_QUAL '(' aqual_specifier ')'
	| AQUAL_SHORT_CONST
	;

aqual_specifier :
	aqual_const
	| type_var
	| AQUALS '(' any_type_name ')'
	;

//aqual_opt :
//	/*empty*/
//	| AQUAL_SHORT_CONST
//	;

pointer_null_and_bound :
	'*' pointer_bound
	| '@' pointer_bound
	| '?'
	;

pointer_bound :
	/*empty*/
	| '{' assignment_expression '}'
	| '{' TYPE_VAR '}'
	;

zeroterm_qual_opt :
	/*empty*/
	| ZEROTERM_QUAL
	| NOZEROTERM_QUAL
	;

eff_set :
	type_var
	| type_var '+' eff_set
	;

eff_opt :
	/*empty*/
	| eff_set
	| '_'
	;

tqual_list :
	/*empty*/
	| type_qualifier tqual_list
	;

parameter_type_list :
	optional_effect optional_effconstr_qualbnd
	| parameter_list optional_effect optional_effconstr_qualbnd
	| parameter_list ',' ELLIPSIS optional_effect optional_effconstr_qualbnd
	| ELLIPSIS optional_inject parameter_declaration optional_effect optional_effconstr_qualbnd
	| parameter_list ',' ELLIPSIS optional_inject parameter_declaration optional_effect optional_effconstr_qualbnd
	;

opt_aqual_bnd :
	/*empty*/
	| AQUAL_SHORT_CONST
	;

type_var :
	TYPE_VAR opt_aqual_bnd
	| TYPE_VAR COLON_COLON kind opt_aqual_bnd
	;

optional_effect :
	/*empty*/
	| ';' effect_set
	;

optional_effconstr_qualbnd :
	/*empty*/
	| ':' effconstr_qualbnd
	;

effconstr_qualbnd :
	effconstr_elt
	| qual_bnd_elt
	| effconstr_elt ',' effconstr_qualbnd
	| qual_bnd_elt ',' effconstr_qualbnd
	;

effconstr_elt :
	atomic_effect LE_OP TYPE_VAR
	| atomic_effect '|' atomic_effect
	| IDENTIFIER '(' atomic_effect ')'
	;

qual_bnd_elt :
	qual_bnd_const GE_OP qual_bnd_term
	;

aqual_const :
	AQ_ALIASABLE
	| AQ_UNIQUE
	| AQ_REFCNT
	| AQ_RESTRICTED
	| AQUAL_SHORT_CONST
	;

qual_bnd_const :
	aqual_const
	| AQUALS '(' any_type_name ')'
	;

qual_bnd_term :
	TYPE_VAR
	| AQUALS '(' any_type_name ')'
	;

optional_inject :
	/*empty*/
	| IDENTIFIER
	;

effect_set :
	atomic_effect
	| atomic_effect '+' effect_set
	;

atomic_effect :
	'{' '}'
	| '{' region_set '}'
	| REGIONS '(' any_type_name ')'
	| type_var
	;

region_set :
	type_name
	| type_name ',' region_set
	;

parameter_list :
	parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration :
	specifier_qualifier_list declarator_withtypedef
	| specifier_qualifier_list
	| specifier_qualifier_list abstract_declarator
	;

identifier_list :
	identifier_list0
	;

identifier_list0 :
	IDENTIFIER
	| identifier_list0 ',' IDENTIFIER
	;

initializer :
	assignment_expression
	| array_initializer
	;

array_initializer :
	'{' '}'
	| '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	| '{' FOR IDENTIFIER '<' expression ':' expression '}'
	| '{' FOR IDENTIFIER '<' expression ':' type_name '}'
	;

initializer_list :
	initializer
	| designation initializer
	| initializer_list ',' initializer
	| initializer_list ',' designation initializer
	;

designation :
	designator_list '='
	| field_name ':'
	;

designator_list :
	designator
	| designator_list designator
	;

designator :
	'[' constant_expression ']'
	| '.' field_name
	;

type_name :
	specifier_qualifier_list
	| specifier_qualifier_list abstract_declarator
	;

any_type_name :
	type_name
	| '{' '}'
	| '{' region_set '}'
	| REGIONS '(' any_type_name ')'
	| any_type_name '+' atomic_effect
	| qual_bnd_const
	;

type_name_list :
	any_type_name
	| type_name_list ',' any_type_name
	;

abstract_declarator :
	pointer
	| direct_abstract_declarator
	| pointer direct_abstract_declarator
	;

direct_abstract_declarator :
	'(' abstract_declarator ')'
	| '[' ']' zeroterm_qual_opt
	| direct_abstract_declarator '[' ']' zeroterm_qual_opt
	| '[' assignment_expression ']' zeroterm_qual_opt
	| direct_abstract_declarator '[' assignment_expression ']' zeroterm_qual_opt
	| '(' parameter_type_list ')' chk_req_ens_thr_opt
	| direct_abstract_declarator '(' parameter_type_list ')' chk_req_ens_thr_opt
	| direct_abstract_declarator '<' type_name_list right_angle
	| direct_abstract_declarator attributes
	;

chk_req_ens_thr :
	CHECKS_QUAL '(' constant_expression ')'
	| REQUIRES_QUAL '(' constant_expression ')'
	| ENSURES_QUAL '(' constant_expression ')'
	| THROWS_QUAL '(' constant_expression ')'
	| CHECKS_QUAL '(' constant_expression ')' chk_req_ens_thr
	| REQUIRES_QUAL '(' constant_expression ')' chk_req_ens_thr
	| ENSURES_QUAL '(' constant_expression ')' chk_req_ens_thr
	| THROWS_QUAL '(' constant_expression ')' chk_req_ens_thr
	;

chk_req_ens_thr_opt :
	/*empty*/
	| chk_req_ens_thr
	;

statement :
	labeled_statement
	| expression_statement
	| compound_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

labeled_statement :
	IDENTIFIER ':' statement
	;

expression_statement :
	';'
	| expression ';'
	;

start_fndef_block :
	'{'
	;

end_fndef_block :
	'}'
	;

fndef_compound_statement :
	start_fndef_block end_fndef_block
	| start_fndef_block block_item_list end_fndef_block
	;

compound_statement :
	'{' '}'
	| '{' block_item_list '}'
	;

block_item_list :
	declaration
	| declaration block_item_list
	| IDENTIFIER ':' declaration
	| IDENTIFIER ':' declaration block_item_list
	| statement
	| statement block_item_list
	| function_definition2
	| function_definition2 block_item_list
	;

selection_statement :
	IF '(' expression ')' statement
	| IF '(' expression ')' statement ELSE statement
	| SWITCH '(' expression ')' '{' switch_clauses '}'
	| SWITCH qual_opt_identifier '{' switch_clauses '}'
	| SWITCH '$' '(' argument_expression_list ')' '{' switch_clauses '}'
	| TRY statement CATCH '{' switch_clauses '}'
	;

switch_clauses :
	/*empty*/
	| DEFAULT ':' block_item_list switch_clauses
	| CASE exp_pattern ':' switch_clauses
	| CASE exp_pattern ':' block_item_list switch_clauses
	| CASE pattern AND_OP expression ':' switch_clauses
	| CASE pattern AND_OP expression ':' block_item_list switch_clauses
	;

iteration_statement :
	WHILE '(' expression ')' statement
	| DO statement WHILE '(' expression ')' ';'
	| FOR '(' for_exp_opt ';' for_exp_opt ';' for_exp_opt ')' statement
	| FOR '(' declaration for_exp_opt ';' for_exp_opt ')' statement
	;

for_exp_opt :
	/*empty*/
	| expression
	;

jump_statement :
	GOTO IDENTIFIER ';'
	| CONTINUE ';'
	| BREAK ';'
	| RETURN ';'
	| RETURN expression ';'
	| FALLTHRU ';'
	| FALLTHRU '(' ')' ';'
	| FALLTHRU '(' argument_expression_list ')' ';'
	;

exp_pattern :
	conditional_pattern
	;

conditional_pattern :
	logical_or_pattern
	| logical_or_pattern '?' expression ':' conditional_expression
	;

logical_or_pattern :
	logical_and_pattern
	| logical_or_pattern OR_OP logical_and_expression
	;

logical_and_pattern :
	inclusive_or_pattern
	| inclusive_or_pattern AND_OP inclusive_or_expression
	;

inclusive_or_pattern :
	exclusive_or_pattern
	| exclusive_or_pattern '|' exclusive_or_expression
	;

exclusive_or_pattern :
	and_pattern
	| and_pattern '^' exclusive_or_expression
	;

and_pattern :
	equality_pattern
	| and_pattern '&' equality_expression
	;

equality_pattern :
	relational_pattern
	| equality_pattern equality_op relational_expression
	;

relational_pattern :
	shift_pattern
	| relational_pattern relational_op shift_expression
	;

shift_pattern :
	additive_pattern
	| shift_pattern LEFT_OP additive_expression
	| shift_pattern RIGHT_OP additive_expression
	;

additive_pattern :
	multiplicative_pattern
	| additive_pattern additive_op multiplicative_expression
	;

multiplicative_pattern :
	cast_pattern
	| multiplicative_pattern multiplicative_op cast_expression
	;

cast_pattern :
	unary_pattern
	| '(' type_name ')' cast_expression
	;

unary_pattern :
	postfix_pattern
	| unary_operator cast_expression
	| SIZEOF '(' type_name ')'
	| SIZEOF unary_expression
	| OFFSETOF '(' type_name ',' field_expression ')'
	;

postfix_pattern :
	primary_pattern
	;

primary_pattern :
	pattern
	;

pattern :
	'_'
	| '(' expression ')'
	| qual_opt_identifier
	| '&' pattern
	| constant
	| IDENTIFIER IDENTIFIER pattern
	| IDENTIFIER '<' TYPE_VAR '>' type_name IDENTIFIER
	| TYPEDEF_NAME '<' TYPE_VAR '>' type_name IDENTIFIER
	| '$' '(' field_pattern_list ')'
	| qual_opt_identifier '(' tuple_pattern_list ')'
	| qual_opt_identifier '{' type_params_opt field_pattern_list '}'
	| '{' type_params_opt field_pattern_list '}'
	| AND_OP pattern
	| '*' IDENTIFIER
	| '*' IDENTIFIER IDENTIFIER pattern
	| IDENTIFIER '<' TYPE_VAR '>'
	| IDENTIFIER '<' '_' '>'
	;

tuple_pattern_list :
	tuple_pattern_list0
	| tuple_pattern_list0 ',' ELLIPSIS
	| ELLIPSIS
	;

tuple_pattern_list0 :
	pattern
	| tuple_pattern_list0 ',' pattern
	;

field_pattern :
	pattern
	| designation pattern
	;

field_pattern_list :
	field_pattern_list0
	| field_pattern_list0 ',' ELLIPSIS
	| ELLIPSIS
	;

field_pattern_list0 :
	field_pattern
	| field_pattern_list0 ',' field_pattern
	;

expression :
	assignment_expression
	| expression ',' assignment_expression
	;

assignment_expression :
	conditional_expression
	| unary_expression assignment_operator assignment_expression
	| unary_expression SWAP assignment_expression
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

conditional_expression :
	logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression
	| THROW conditional_expression
	| NEW array_initializer
	| NEW logical_or_expression
	| QNEW '(' expression ')' array_initializer
	| QNEW '(' expression ')' logical_or_expression
	| RNEW '(' expression ')' array_initializer
	| RNEW '(' expression ')' logical_or_expression
	;

constant_expression :
	conditional_expression
	;

logical_or_expression :
	logical_and_expression
	| logical_or_expression OR_OP logical_and_expression
	;

logical_and_expression :
	inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression
	;

inclusive_or_expression :
	exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression
	;

exclusive_or_expression :
	and_expression
	| exclusive_or_expression '^' and_expression
	;

and_expression :
	equality_expression
	| and_expression '&' equality_expression
	;

equality_expression :
	relational_expression
	| equality_expression equality_op relational_expression
	;

relational_expression :
	shift_expression
	| relational_expression relational_op shift_expression
	;

shift_expression :
	additive_expression
	| shift_expression LEFT_OP additive_expression
	| shift_expression RIGHT_OP additive_expression
	;

additive_expression :
	multiplicative_expression
	| additive_expression additive_op multiplicative_expression
	;

multiplicative_expression :
	cast_expression
	| multiplicative_expression multiplicative_op cast_expression
	;

equality_op :
	EQ_OP
	| NE_OP
	;

relational_op :
	'<'
	| '>'
	| LE_OP
	| GE_OP
	;

additive_op :
	'+'
	| '-'
	;

multiplicative_op :
	'*'
	| '/'
	| '%'
	;

cast_expression :
	unary_expression
	| '(' type_name ')' cast_expression
	;

unary_expression :
	postfix_expression
	| INC_OP unary_expression
	| DEC_OP unary_expression
	| '&' cast_expression
	| '*' cast_expression
	| unary_operator cast_expression
	| SIZEOF '(' type_name ')'
	| SIZEOF unary_expression
	| OFFSETOF '(' type_name ',' field_expression ')'
	| MALLOC '(' assignment_expression ')'
	| QMALLOC '(' assignment_expression ',' assignment_expression ',' assignment_expression ')'
	| RMALLOC '(' expression ',' assignment_expression ')'
	| RVMALLOC '(' expression ',' assignment_expression ')'
	| RMALLOC_INLINE '(' expression ',' assignment_expression ')'
	| CALLOC '(' assignment_expression ',' SIZEOF '(' type_name ')' ')'
	| QCALLOC '(' assignment_expression ',' assignment_expression ',' assignment_expression ',' SIZEOF '(' type_name ')' ')'
	| RCALLOC '(' assignment_expression ',' assignment_expression ',' SIZEOF '(' type_name ')' ')'
	| NUMELTS '(' assignment_expression ')'
	| TAGOF '(' assignment_expression ')'
	| TAGCHECK '(' postfix_expression '.' field_name ')'
	| TAGCHECK '(' postfix_expression PTR_OP field_name ')'
	| VALUEOF '(' type_name ')'
	| ASM_TOK asm_expr
	| EXTENSION unary_expression
	| ASSERT '(' assignment_expression ')'
	| ASSERT_QUAL '(' assignment_expression ')'
	| ASSERT_FALSE_QUAL '(' assignment_expression ')'
	;

unary_operator :
	'~'
	| '!'
	| '-'
	| '+'
	;

asm_expr :
	volatile_opt '(' STRING asm_out_opt ')'
	;

volatile_opt :
	/*empty*/
	| VOLATILE
	;

asm_out_opt :
	/*empty*/
	| ':' asm_in_opt
	| ':' asm_outlist asm_in_opt
	;

asm_outlist :
	asm_io_elt
	| asm_outlist ',' asm_io_elt
	;

asm_in_opt :
	/*empty*/
	| ':' asm_clobber_opt
	| ':' asm_inlist asm_clobber_opt
	;

asm_inlist :
	asm_io_elt
	| asm_inlist ',' asm_io_elt
	;

asm_io_elt :
	STRING '(' expression ')'
	;

asm_clobber_opt :
	/*empty*/
	| ':'
	| ':' asm_clobber_list
	;

asm_clobber_list :
	STRING
	| asm_clobber_list ',' STRING
	;

postfix_expression :
	primary_expression
	| postfix_expression '[' expression ']'
	| postfix_expression '(' ')'
	| postfix_expression '(' argument_expression_list ')'
	| postfix_expression '.' field_name
	| postfix_expression PTR_OP field_name
	| postfix_expression INC_OP
	| postfix_expression DEC_OP
	| '(' type_name ')' '{' '}'
	| '(' type_name ')' '{' initializer_list '}'
	| '(' type_name ')' '{' initializer_list ',' '}'
	;

field_expression :
	field_name
	| INTEGER_CONSTANT
	| field_expression '.' field_name
	| field_expression '.' INTEGER_CONSTANT
	;

primary_expression :
	qual_opt_identifier
	| PRAGMA '(' IDENTIFIER ')'
	| constant
	| STRING
	| WSTRING
	| '(' expression ')'
	| primary_expression LEFT_RIGHT
	| primary_expression '@' '<' type_name_list right_angle
	| qual_opt_identifier '{' type_params_opt initializer_list '}'
	| '$' '(' argument_expression_list ')'
	| '(' '{' block_item_list '}' ')'
	;

argument_expression_list :
	argument_expression_list0
	;

argument_expression_list0 :
	assignment_expression
	| argument_expression_list0 ',' assignment_expression
	;

constant :
	INTEGER_CONSTANT
	| CHARACTER_CONSTANT
	| WCHARACTER_CONSTANT
	| NULL_kw
	| FLOATING_CONSTANT
	;

qual_opt_identifier :
	IDENTIFIER
	| QUAL_IDENTIFIER
	;

qual_opt_typedef :
	TYPEDEF_NAME
	| QUAL_TYPEDEF_NAME
	;

struct_union_name :
	qual_opt_identifier
	| qual_opt_typedef
	;

field_name :
	IDENTIFIER
	| TYPEDEF_NAME
	;

right_angle :
	'>'
	| RIGHT_OP
	;

all_constraints :
	/*empty*/
	| TYPE_VAR STRING ',' STRING '(' constraint_list_opt ')' all_constraints
	;

constraint_list_opt :
	/*empty*/
	| constraint_list
	;

constraint_list :
	constraint
	| constraint ';' constraint_list
	;

tvar_or_string :
	TYPE_VAR
	| STRING
	;

constraint :
	STRING '!' '(' constraint ')'
	| STRING '^' '(' c_op ',' tvar_or_string ',' tvar_or_string ')'
	| STRING '?' '(' tvar_or_string ',' tvar_or_string ')'
	| STRING '=' '(' tvar_or_string ',' tvar_or_string ')'
	| STRING '<' '(' tvar_or_string ',' tvar_or_string ')'
	| STRING '>' '(' constraint ',' constraint ')'
	| STRING '+' '(' c_op ',' constraint ',' constraint ')'
	;

c_op :
	'A'
	| 'V'
	| '!'
	| '='
	| '<'
	;

//Lexer
//lalr (\d+:\d+): ERROR: undefined symbol '\([^']+\)'
PORTON : 'PORTON' ;
PORTOFF : 'PORTOFF' ;
TEMPESTON : 'TEMPESTON' ;
TEMPESTOFF : 'TEMPESTOFF' ;
EXTERN : 'extern' ;
STRING : 'STRING' ;
HIDE : 'hide' ;
EXPORT : 'export' ;
OVERRIDE : 'override' ;
USING : 'using' ;
NAMESPACE : 'namespace' ;
IDENTIFIER : 'IDENTIFIER' ;
NOINFERENCE : 'NOINFERENCE' ;
LET : 'let' ;
REGION : 'region' ;
TYPE_VAR : 'TYPE_VAR' ;
EXTENSION : 'extension' ;
INLINE : 'inline' ;
AUTO : 'auto' ;
REGISTER : 'register' ;
STATIC : 'static' ;
TYPEDEF : 'typedef' ;
ABSTRACT : 'abstract' ;
ATTRIBUTE : 'ATTRIBUTE' ;
CONST : 'const' ;
INTEGER_CONSTANT : 'INTEGER_CONSTANT' ;
VOID : 'void' ;
CHAR : 'char' ;
SHORT : 'short' ;
INT : 'int' ;
ISIZE_T : 'isize_t' ;
USIZE_T : 'usize_t' ;
LONG : 'long' ;
FLOAT : 'float' ;
FLOAT128 : 'float128' ;
DOUBLE : 'double' ;
SIGNED : 'signed' ;
UNSIGNED : 'unsigned' ;
COMPLEX : 'COMPLEX' ;
TYPEOF : 'typeof' ;
BUILTIN_VA_LIST : 'BUILTIN_VA_LIST' ;
COLON_COLON : '::' ;
REGION_T : 'REGION_T' ;
AQUAL_T : 'AQUAL_T' ;
TAG_T : 'TAG_T' ;
VALUEOF_T : 'VALUEOF_T' ;
SUBSET_QUAL : 'SUBSET_QUAL' ;
VOLATILE : 'volatile' ;
RESTRICT : 'restrict' ;
ENUM : 'enum' ;
TAGGED_QUAL : 'TAGGED_QUAL' ;
STRUCT : 'struct' ;
UNION : 'union' ;
ASM_TOK : 'ASM_TOK' ;
REQUIRES_QUAL : 'REQUIRES_QUAL' ;
DATATYPE : 'DATATYPE' ;
EXTENSIBLE_QUAL : 'EXTENSIBLE_QUAL' ;
NUMELTS_QUAL : 'NUMELTS_QUAL' ;
REGION_QUAL : 'REGION_QUAL' ;
EFFECT_QUAL : 'EFFECT_QUAL' ;
THIN_QUAL : 'THIN_QUAL' ;
FAT_QUAL : 'FAT_QUAL' ;
AUTORELEASED_QUAL : 'AUTORELEASED_QUAL' ;
ZEROTERM_QUAL : 'ZEROTERM_QUAL' ;
NOZEROTERM_QUAL : 'NOZEROTERM_QUAL' ;
NOTNULL_QUAL : 'NOTNULL_QUAL' ;
NULLABLE_QUAL : 'NULLABLE_QUAL' ;
ALIAS_QUAL : 'ALIAS_QUAL' ;
AQUAL_SHORT_CONST : 'AQUAL_SHORT_CONST' ;
AQUALS : 'AQUALS' ;
ELLIPSIS : '...' ;
LE_OP : '<=' ;
GE_OP : '>=' ;
AQ_ALIASABLE : 'AQ_ALIASABLE' ;
AQ_UNIQUE : 'AQ_UNIQUE' ;
AQ_REFCNT : 'AQ_REFCNT' ;
AQ_RESTRICTED : 'AQ_RESTRICTED' ;
REGIONS : 'REGIONS' ;
FOR : 'for' ;
CHECKS_QUAL : 'CHECKS_QUAL' ;
ENSURES_QUAL : 'ENSURES_QUAL' ;
THROWS_QUAL : 'THROWS_QUAL' ;
IF : 'if' ;
ELSE : 'else' ;
SWITCH : 'switch' ;
TRY : 'try' ;
CATCH : 'catch' ;
DEFAULT : 'default' ;
CASE : 'case' ;
AND_OP : '&&' ;
WHILE : 'while' ;
DO : 'do' ;
GOTO : 'goto' ;
CONTINUE : 'continue' ;
BREAK : 'break' ;
RETURN : 'return' ;
FALLTHRU : 'FALLTHRU' ;
OR_OP : '||' ;
LEFT_OP : '<<' ;
RIGHT_OP : '>>' ;
SIZEOF : 'sizeof' ;
OFFSETOF : 'offsetof' ;
TYPEDEF_NAME : 'TYPEDEF_NAME' ;
SWAP : 'SWAP' ;
MUL_ASSIGN : '*=' ;
DIV_ASSIGN : '/=' ;
MOD_ASSIGN : '%=' ;
ADD_ASSIGN : '+=' ;
SUB_ASSIGN : '-=' ;
LEFT_ASSIGN : '<<=' ;
RIGHT_ASSIGN : '>>=' ;
AND_ASSIGN : '&=' ;
XOR_ASSIGN : '~=' ;
OR_ASSIGN : '|=' ;
THROW : 'throw' ;
NEW : 'new' ;
QNEW : 'QNEW' ;
RNEW : 'RNEW' ;
EQ_OP : '==' ;
NE_OP : '!=' ;
INC_OP : '++' ;
DEC_OP : '--' ;
MALLOC : 'MALLOC' ;
QMALLOC : 'QMALLOC' ;
RMALLOC : 'RMALLOC' ;
RVMALLOC : 'RVMALLOC' ;
RMALLOC_INLINE : 'RMALLOC_INLINE' ;
CALLOC : 'CALLOC' ;
QCALLOC : 'QCALLOC' ;
RCALLOC : 'RCALLOC' ;
NUMELTS : 'NUMELTS' ;
TAGOF : 'TAGOF' ;
TAGCHECK : 'TAGCHECK' ;
PTR_OP : 'PTR_OP' ;
VALUEOF : 'VALUEOF' ;
ASSERT : 'assert' ;
ASSERT_QUAL : 'ASSERT_QUAL' ;
ASSERT_FALSE_QUAL : 'ASSERT_FALSE_QUAL' ;
PRAGMA : 'PRAGMA' ;
WSTRING : 'WSTRING' ;
LEFT_RIGHT : 'LEFT_RIGHT' ;
CHARACTER_CONSTANT : 'CHARACTER_CONSTANT' ;
WCHARACTER_CONSTANT : 'WCHARACTER_CONSTANT' ;
NULL_kw : 'NULL' ;
FLOATING_CONSTANT : 'FLOATING_CONSTANT' ;
QUAL_IDENTIFIER : 'QUAL_IDENTIFIER' ;
QUAL_TYPEDEF_NAME : 'QUAL_TYPEDEF_NAME' ;

}