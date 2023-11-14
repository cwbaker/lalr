// /usr/bin/time /home/mingo/dev/c/A_grammars/lalr/lalr-nb/dist/Release/GNU-Linux/lalr-nb cxx-parser.g
// /usr/bin/time /home/mingo/dev/c/A_grammars/lalr/test-grammar-nb/dist/Release/GNU-Linux/test-grammar-nb -g cxx-parser.g -i lemon.pp.c
// /usr/bin/time /home/mingo/dev/c/A_grammars/lalr/test-grammar-nb/dist/Release/GNU-Linux/test-grammar-nb -g cxx-parser.g -i test.c


cxxparser {

//%error_recovery_show;
%error_recovery_off;
%whitespace "[ \t\r\n]*";
%whitespace "//[^\n\r]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

//%token '+'
//%token '-'
//%token '*'
//%token '/'
//%token '%'
//%token '^'
//%token '&'
//%token '|'
//%token '~'
//%token '!'
//%token '<'
//%token '>'
//%token '='
//%token ':'
//%token '['
//%token ']'
//%token '{'
//%token '}'
//%token '('
//%token ')'
//%token '?'
//%token '.'
//%token '\''
//%token '"'
//%token '\\'
//%token '@'
//%token '$'
//%token ';'
//%token ','
//%token ARROW
//%token ARROW_STAR
//%token DEC
//%token EQ
//%token GE
//%token INC
//%token LE
//%token LOG_AND
//%token LOG_OR
//%token NE
//%token SHL
//%token SHR
//%token ASS_ADD
//%token ASS_AND
//%token ASS_DIV
//%token ASS_MOD
//%token ASS_MUL
//%token ASS_OR
//%token ASS_SHL
//%token ASS_SHR
//%token ASS_SUB
//%token ASS_XOR
//%token DOT_STAR
//%token ELLIPSIS
//%token SCOPE
//%token PRIVATE
//%token PROTECTED
//%token PUBLIC
//%token BOOL
//%token CHAR
//%token DOUBLE
//%token FLOAT
//%token INT
//%token LONG
//%token SHORT
//%token SIGNED
//%token UNSIGNED
//%token VOID
//%token WCHAR_T
//%token CLASS
//%token ENUM
//%token NAMESPACE
//%token STRUCT
//%token TYPENAME
//%token UNION
//%token CONST
//%token VOLATILE
//%token AUTO
//%token EXPLICIT
//%token EXPORT
//%token EXTERN
//%token FRIEND
//%token INLINE
//%token MUTABLE
//%token REGISTER
//%token STATIC
//%token TEMPLATE
//%token TYPEDEF
//%token USING
//%token VIRTUAL
//%token ASM
//%token ATTRIBUTE
//%token BREAK
//%token CASE
//%token CATCH
//%token CONST_CAST
//%token CONTINUE
//%token DEFAULT
//%token DELETE
//%token DO
//%token DYNAMIC_CAST
//%token ELSE
//%token FALSE
//%token FOR
//%token GOTO
//%token IF
//%token NEW
//%token OPERATOR
//%token REINTERPRET_CAST
//%token RETURN
//%token SIZEOF
//%token STATIC_CAST
//%token SWITCH
//%token THIS
//%token THROW
//%token TRUE
//%token TRY
//%token TYPEID
//%token WHILE
//%token CharacterLiteral
//%token FloatingLiteral
//%token Identifier
//%token IntegerLiteral
//%token NumberLiteral
//%token StringLiteral
//%token SHIFT_THERE
//%token REDUCE_HERE_MOSTLY
//%token '#'

%nonassoc /*1*/ SHIFT_THERE ;
%nonassoc /*2*/ '+' '-' '*' '&' '<' ':' '[' '{' DEC INC SCOPE ELSE StringLiteral ;
%nonassoc /*3*/ REDUCE_HERE_MOSTLY ;
%nonassoc /*4*/ '(' ;

//%start translation_unit

//%%

translation_unit :
	 declaration_seq_opt
	;

identifier_word :
	 Identifier
	;
identifier :
	 identifier_word %prec SHIFT_THERE /*1N*/
	;
id :
	 identifier %prec SHIFT_THERE /*1N*/
	| identifier template_test '+' /*2N*/ template_argument_list '>'
	| identifier template_test '+' /*2N*/ '>'
	| identifier template_test '-' /*2N*/
	| template_id
	;
template_test :
	 '<' /*2N*/
	;
global_scope :
	 SCOPE /*2N*/
	| TEMPLATE global_scope
	;
id_scope :
	 id SCOPE /*2N*/
	;
nested_id :
	 id %prec SHIFT_THERE /*1N*/
	| id_scope nested_id
	;
scoped_id :
	 nested_id
	| global_scope nested_id
	;
destructor_id :
	 '~' id
	| TEMPLATE destructor_id
	;
special_function_id :
	 conversion_function_id
	| operator_function_id
	| TEMPLATE special_function_id
	;
nested_special_function_id :
	 special_function_id
	| id_scope destructor_id
	| id_scope nested_special_function_id
	;
scoped_special_function_id :
	 nested_special_function_id
	| global_scope nested_special_function_id
	;
declarator_id :
	 scoped_id
	| scoped_special_function_id
	| destructor_id
	;
built_in_type_id :
	 built_in_type_specifier
	| built_in_type_id built_in_type_specifier
	;
pseudo_destructor_id :
	 built_in_type_id SCOPE /*2N*/ '~' built_in_type_id
	| '~' built_in_type_id
	| TEMPLATE pseudo_destructor_id
	;
nested_pseudo_destructor_id :
	 pseudo_destructor_id
	| id_scope nested_pseudo_destructor_id
	;
scoped_pseudo_destructor_id :
	 nested_pseudo_destructor_id
	| global_scope scoped_pseudo_destructor_id
	;
string :
	 StringLiteral /*2N*/
	;
literal :
	 IntegerLiteral
	| CharacterLiteral
	| FloatingLiteral
	| string
	| boolean_literal
	;
boolean_literal :
	 FALSE
	| TRUE
	;
primary_expression :
	 literal
	| THIS
	| suffix_decl_specified_ids
	| abstract_expression %prec REDUCE_HERE_MOSTLY /*3N*/
	;
abstract_expression :
	 parenthesis_clause
	| '[' /*2N*/ expression_opt ']'
	| TEMPLATE parenthesis_clause
	;
type1_parameters :
	 parameter_declaration_list ';'
	| type1_parameters parameter_declaration_list ';'
	;
mark_type1 :
	 /*empty*/
	;
postfix_expression :
	 primary_expression
	| postfix_expression parenthesis_clause mark_type1 '-' /*2N*/
	| postfix_expression parenthesis_clause mark_type1 '+' /*2N*/ type1_parameters mark '{' /*2N*/ error
	| postfix_expression parenthesis_clause mark_type1 '+' /*2N*/ type1_parameters mark error
	| postfix_expression parenthesis_clause mark_type1 '+' /*2N*/ error
	| postfix_expression '[' /*2N*/ expression_opt ']'
	| postfix_expression '.' declarator_id
	| postfix_expression '.' scoped_pseudo_destructor_id
	| postfix_expression ARROW declarator_id
	| postfix_expression ARROW scoped_pseudo_destructor_id
	| postfix_expression INC /*2N*/
	| postfix_expression DEC /*2N*/
	| DYNAMIC_CAST '<' /*2N*/ type_id '>' '(' /*4N*/ expression ')'
	| STATIC_CAST '<' /*2N*/ type_id '>' '(' /*4N*/ expression ')'
	| REINTERPRET_CAST '<' /*2N*/ type_id '>' '(' /*4N*/ expression ')'
	| CONST_CAST '<' /*2N*/ type_id '>' '(' /*4N*/ expression ')'
	| TYPEID parameters_clause
	;
expression_list_opt :
	 /*empty*/
	| expression_list
	;
expression_list :
	 assignment_expression
	| expression_list ',' assignment_expression
	;
unary_expression :
	 postfix_expression
	| INC /*2N*/ cast_expression
	| DEC /*2N*/ cast_expression
	| ptr_operator cast_expression
	| suffix_decl_specified_scope star_ptr_operator cast_expression
	| '+' /*2N*/ cast_expression
	| '-' /*2N*/ cast_expression
	| '!' cast_expression
	| '~' cast_expression
	| SIZEOF unary_expression
	| new_expression
	| global_scope new_expression
	| delete_expression
	| global_scope delete_expression
	;
delete_expression :
	 DELETE cast_expression
	;
new_expression :
	 NEW new_type_id new_initializer_opt
	| NEW parameters_clause new_type_id new_initializer_opt
	| NEW parameters_clause
	| NEW parameters_clause parameters_clause new_initializer_opt
	;
new_type_id :
	 type_specifier ptr_operator_seq_opt
	| type_specifier new_declarator
	| type_specifier new_type_id
	;
new_declarator :
	 ptr_operator new_declarator
	| direct_new_declarator
	;
direct_new_declarator :
	 '[' /*2N*/ expression ']'
	| direct_new_declarator '[' /*2N*/ constant_expression ']'
	;
new_initializer_opt :
	 /*empty*/
	| '(' /*4N*/ expression_list_opt ')'
	;
cast_expression :
	 unary_expression
	| abstract_expression cast_expression
	;
pm_expression :
	 cast_expression
	| pm_expression DOT_STAR cast_expression
	| pm_expression ARROW_STAR cast_expression
	;
multiplicative_expression :
	 pm_expression
	| multiplicative_expression star_ptr_operator pm_expression
	| multiplicative_expression '/' pm_expression
	| multiplicative_expression '%' pm_expression
	;
additive_expression :
	 multiplicative_expression
	| additive_expression '+' /*2N*/ multiplicative_expression
	| additive_expression '-' /*2N*/ multiplicative_expression
	;
shift_expression :
	 additive_expression
	| shift_expression SHL additive_expression
	| shift_expression SHR additive_expression
	;
relational_expression :
	 shift_expression
	| relational_expression '<' /*2N*/ shift_expression
	| relational_expression '>' shift_expression
	| relational_expression LE shift_expression
	| relational_expression GE shift_expression
	;
equality_expression :
	 relational_expression
	| equality_expression EQ relational_expression
	| equality_expression NE relational_expression
	;
and_expression :
	 equality_expression
	| and_expression '&' /*2N*/ equality_expression
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
	| logical_and_expression LOG_AND inclusive_or_expression
	;
logical_or_expression :
	 logical_and_expression
	| logical_or_expression LOG_OR logical_and_expression
	;
conditional_expression :
	 logical_or_expression
	| logical_or_expression '?' expression ':' /*2N*/ assignment_expression
	;
assignment_expression :
	 conditional_expression
	| logical_or_expression assignment_operator assignment_expression
	| logical_or_expression '=' braced_initializer
	| throw_expression
	;
assignment_operator :
	 '='
	| ASS_ADD
	| ASS_AND
	| ASS_DIV
	| ASS_MOD
	| ASS_MUL
	| ASS_OR
	| ASS_SHL
	| ASS_SHR
	| ASS_SUB
	| ASS_XOR
	;
expression_opt :
	 /*empty*/
	| expression
	;
expression :
	 assignment_expression
	| expression_list ',' assignment_expression
	;
constant_expression :
	 conditional_expression
	;
templated_relational_expression :
	 shift_expression
	| templated_relational_expression '<' /*2N*/ shift_expression
	| templated_relational_expression LE shift_expression
	| templated_relational_expression GE shift_expression
	;
templated_equality_expression :
	 templated_relational_expression
	| templated_equality_expression EQ templated_relational_expression
	| templated_equality_expression NE templated_relational_expression
	;
templated_and_expression :
	 templated_equality_expression
	| templated_and_expression '&' /*2N*/ templated_equality_expression
	;
templated_exclusive_or_expression :
	 templated_and_expression
	| templated_exclusive_or_expression '^' templated_and_expression
	;
templated_inclusive_or_expression :
	 templated_exclusive_or_expression
	| templated_inclusive_or_expression '|' templated_exclusive_or_expression
	;
templated_logical_and_expression :
	 templated_inclusive_or_expression
	| templated_logical_and_expression LOG_AND templated_inclusive_or_expression
	;
templated_logical_or_expression :
	 templated_logical_and_expression
	| templated_logical_or_expression LOG_OR templated_logical_and_expression
	;
templated_conditional_expression :
	 templated_logical_or_expression
	| templated_logical_or_expression '?' templated_expression ':' /*2N*/ templated_assignment_expression
	;
templated_assignment_expression :
	 templated_conditional_expression
	| templated_logical_or_expression assignment_operator templated_assignment_expression
	| templated_throw_expression
	;
templated_expression :
	 templated_assignment_expression
	| templated_expression_list ',' templated_assignment_expression
	;
templated_expression_list :
	 templated_assignment_expression
	| templated_expression_list ',' templated_assignment_expression
	;
looping_statement :
	 start_search looped_statement
	;
looped_statement :
	 statement
	| advance_search '+' /*2N*/ looped_statement
	| advance_search '-' /*2N*/
	;
statement :
	 control_statement
	| compound_statement
	| declaration_statement
	| try_block
	;
control_statement :
	 labeled_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;
labeled_statement :
	 identifier_word ':' /*2N*/ looping_statement
	| CASE constant_expression ':' /*2N*/ looping_statement
	| DEFAULT ':' /*2N*/ looping_statement
	;
compound_statement :
	 '{' /*2N*/ statement_seq_opt '}'
	| '{' /*2N*/ statement_seq_opt looping_statement '#' bang error '}'
	;
statement_seq_opt :
	 /*empty*/
	| statement_seq_opt looping_statement
	| statement_seq_opt looping_statement '#' bang error ';'
	;
selection_statement :
	 IF '(' /*4N*/ condition ')' looping_statement %prec SHIFT_THERE /*1N*/
	| IF '(' /*4N*/ condition ')' looping_statement ELSE /*2N*/ looping_statement
	| SWITCH '(' /*4N*/ condition ')' looping_statement
	;
condition_opt :
	 /*empty*/
	| condition
	;
condition :
	 parameter_declaration_list
	;
iteration_statement :
	 WHILE '(' /*4N*/ condition ')' looping_statement
	| DO looping_statement WHILE '(' /*4N*/ expression ')' ';'
	| FOR '(' /*4N*/ for_init_statement condition_opt ';' expression_opt ')' looping_statement
	;
for_init_statement :
	 simple_declaration
	;
jump_statement :
	 BREAK ';'
	| CONTINUE ';'
	| RETURN expression_opt ';'
	| GOTO identifier ';'
	;
declaration_statement :
	 block_declaration
	;
compound_declaration :
	 '{' /*2N*/ nest declaration_seq_opt '}'
	| '{' /*2N*/ nest declaration_seq_opt util looping_declaration '#' bang error '}'
	;
declaration_seq_opt :
	 /*empty*/
	| declaration_seq_opt util looping_declaration
	| declaration_seq_opt util looping_declaration '#' bang error ';'
	;
looping_declaration :
	 start_search1 looped_declaration
	;
looped_declaration :
	 declaration
	| advance_search '+' /*2N*/ looped_declaration
	| advance_search '-' /*2N*/
	;
declaration :
	 block_declaration
	| function_definition
	| template_declaration
	| explicit_specialization
	| specialised_declaration
	;
specialised_declaration :
	 linkage_specification
	| namespace_definition
	| TEMPLATE specialised_declaration
	;
block_declaration :
	 simple_declaration
	| specialised_block_declaration
	;
specialised_block_declaration :
	 asm_definition
	| namespace_alias_definition
	| using_declaration
	| using_directive
	| TEMPLATE specialised_block_declaration
	;
simple_declaration :
	 ';'
	| init_declaration ';'
	| init_declarations ';'
	| decl_specifier_prefix simple_declaration
	;
suffix_built_in_decl_specifier_raw :
	 built_in_type_specifier
	| suffix_built_in_decl_specifier_raw built_in_type_specifier
	| suffix_built_in_decl_specifier_raw decl_specifier_suffix
	;
suffix_built_in_decl_specifier :
	 suffix_built_in_decl_specifier_raw
	| TEMPLATE suffix_built_in_decl_specifier
	;
suffix_named_decl_specifier :
	 scoped_id
	| elaborate_type_specifier
	| suffix_named_decl_specifier decl_specifier_suffix
	;
suffix_named_decl_specifier_bi :
	 suffix_named_decl_specifier
	| suffix_named_decl_specifier suffix_built_in_decl_specifier_raw
	;
suffix_named_decl_specifiers :
	 suffix_named_decl_specifier_bi
	| suffix_named_decl_specifiers suffix_named_decl_specifier_bi
	;
suffix_named_decl_specifiers_sf :
	 scoped_special_function_id
	| suffix_named_decl_specifiers
	| suffix_named_decl_specifiers scoped_special_function_id
	;
suffix_decl_specified_ids :
	 suffix_built_in_decl_specifier
	| suffix_built_in_decl_specifier suffix_named_decl_specifiers_sf
	| suffix_named_decl_specifiers_sf
	;
suffix_decl_specified_scope :
	 suffix_named_decl_specifiers SCOPE /*2N*/
	| suffix_built_in_decl_specifier suffix_named_decl_specifiers SCOPE /*2N*/
	| suffix_built_in_decl_specifier SCOPE /*2N*/
	;
decl_specifier_affix :
	 storage_class_specifier
	| function_specifier
	| FRIEND
	| TYPEDEF
	| cv_qualifier
	;
decl_specifier_suffix :
	 decl_specifier_affix
	;
decl_specifier_prefix :
	 decl_specifier_affix
	| TEMPLATE decl_specifier_prefix
	;
storage_class_specifier :
	 REGISTER
	| STATIC
	| MUTABLE
	| EXTERN %prec SHIFT_THERE /*1N*/
	| AUTO
	;
function_specifier :
	 EXPLICIT
	| INLINE
	| VIRTUAL
	;
type_specifier :
	 simple_type_specifier
	| elaborate_type_specifier
	| cv_qualifier
	;
elaborate_type_specifier :
	 class_specifier
	| enum_specifier
	| elaborated_type_specifier
	| TEMPLATE elaborate_type_specifier
	;
simple_type_specifier :
	 scoped_id
	| built_in_type_specifier
	;
built_in_type_specifier :
	 CHAR
	| WCHAR_T
	| BOOL
	| SHORT
	| INT
	| LONG
	| SIGNED
	| UNSIGNED
	| FLOAT
	| DOUBLE
	| VOID
	;
elaborated_type_specifier :
	 elaborated_class_specifier
	| elaborated_enum_specifier
	| TYPENAME scoped_id
	;
elaborated_enum_specifier :
	 ENUM scoped_id %prec SHIFT_THERE /*1N*/
	;
enum_specifier :
	 ENUM scoped_id enumerator_clause
	| ENUM enumerator_clause
	;
enumerator_clause :
	 '{' /*2N*/ enumerator_list_ecarb
	| '{' /*2N*/ enumerator_list enumerator_list_ecarb
	| '{' /*2N*/ enumerator_list ',' enumerator_definition_ecarb
	;
enumerator_list_ecarb :
	 '}'
	| bang error '}'
	;
enumerator_definition_ecarb :
	 '}'
	| bang error '}'
	;
enumerator_definition_filler :
	 /*empty*/
	| bang error ','
	;
enumerator_list_head :
	 enumerator_definition_filler
	| enumerator_list ',' enumerator_definition_filler
	;
enumerator_list :
	 enumerator_list_head enumerator_definition
	;
enumerator_definition :
	 enumerator
	| enumerator '=' constant_expression
	;
enumerator :
	 identifier
	;
namespace_definition :
	 NAMESPACE scoped_id compound_declaration
	| NAMESPACE compound_declaration
	;
namespace_alias_definition :
	 NAMESPACE scoped_id '=' scoped_id ';'
	;
using_declaration :
	 USING declarator_id ';'
	| USING TYPENAME declarator_id ';'
	;
using_directive :
	 USING NAMESPACE scoped_id ';'
	;
asm_definition :
	 ASM '(' /*4N*/ string ')' ';'
	;
linkage_specification :
	 EXTERN string looping_declaration
	| EXTERN string compound_declaration
	;
init_declarations :
	 assignment_expression ',' init_declaration
	| init_declarations ',' init_declaration
	;
init_declaration :
	 assignment_expression
	;
star_ptr_operator :
	 '*' /*2N*/
	| star_ptr_operator cv_qualifier
	;
nested_ptr_operator :
	 star_ptr_operator
	| id_scope nested_ptr_operator
	;
ptr_operator :
	 '&' /*2N*/
	| nested_ptr_operator
	| global_scope nested_ptr_operator
	;
ptr_operator_seq :
	 ptr_operator
	| ptr_operator ptr_operator_seq
	;
ptr_operator_seq_opt :
	 /*empty*/ %prec SHIFT_THERE /*1N*/
	| ptr_operator ptr_operator_seq_opt
	;
cv_qualifier_seq_opt :
	 /*empty*/
	| cv_qualifier_seq_opt cv_qualifier
	;
cv_qualifier :
	 CONST
	| VOLATILE
	;
type_id :
	 type_specifier abstract_declarator_opt
	| type_specifier type_id
	;
abstract_declarator_opt :
	 /*empty*/
	| ptr_operator abstract_declarator_opt
	| direct_abstract_declarator
	;
direct_abstract_declarator_opt :
	 /*empty*/
	| direct_abstract_declarator
	;
direct_abstract_declarator :
	 direct_abstract_declarator_opt parenthesis_clause
	| direct_abstract_declarator_opt '[' /*2N*/ ']'
	| direct_abstract_declarator_opt '[' /*2N*/ constant_expression ']'
	;
parenthesis_clause :
	 parameters_clause cv_qualifier_seq_opt
	| parameters_clause cv_qualifier_seq_opt exception_specification
	;
parameters_clause :
	 '(' /*4N*/ parameter_declaration_clause ')'
	;
parameter_declaration_clause :
	 /*empty*/
	| parameter_declaration_list
	| parameter_declaration_list ELLIPSIS
	;
parameter_declaration_list :
	 parameter_declaration
	| parameter_declaration_list ',' parameter_declaration
	;
abstract_pointer_declaration :
	 ptr_operator_seq
	| multiplicative_expression star_ptr_operator ptr_operator_seq_opt
	;
abstract_parameter_declaration :
	 abstract_pointer_declaration
	| and_expression '&' /*2N*/
	| and_expression '&' /*2N*/ abstract_pointer_declaration
	;
special_parameter_declaration :
	 abstract_parameter_declaration
	| abstract_parameter_declaration '=' assignment_expression
	| ELLIPSIS
	;
parameter_declaration :
	 assignment_expression
	| special_parameter_declaration
	| decl_specifier_prefix parameter_declaration
	;
templated_parameter_declaration :
	 templated_assignment_expression
	| templated_abstract_declaration
	| templated_abstract_declaration '=' templated_assignment_expression
	| decl_specifier_prefix templated_parameter_declaration
	;
templated_abstract_declaration :
	 abstract_pointer_declaration
	| templated_and_expression '&' /*2N*/
	| templated_and_expression '&' /*2N*/ abstract_pointer_declaration
	;
function_definition :
	 ctor_definition
	| func_definition
	;
func_definition :
	 assignment_expression function_try_block
	| assignment_expression function_body
	| decl_specifier_prefix func_definition
	;
ctor_definition :
	 constructor_head function_try_block
	| constructor_head function_body
	| decl_specifier_prefix ctor_definition
	;
constructor_head :
	 bit_field_init_declaration
	| constructor_head ',' assignment_expression
	;
function_try_block :
	 TRY function_block handler_seq
	;
function_block :
	 ctor_initializer_opt function_body
	;
function_body :
	 compound_statement
	;
initializer_clause :
	 assignment_expression
	| braced_initializer
	;
braced_initializer :
	 '{' /*2N*/ initializer_list '}'
	| '{' /*2N*/ initializer_list ',' '}'
	| '{' /*2N*/ '}'
	| '{' /*2N*/ looping_initializer_clause '#' bang error '}'
	| '{' /*2N*/ initializer_list ',' looping_initializer_clause '#' bang error '}'
	;
initializer_list :
	 looping_initializer_clause
	| initializer_list ',' looping_initializer_clause
	;
looping_initializer_clause :
	 start_search looped_initializer_clause
	;
looped_initializer_clause :
	 initializer_clause
	| advance_search '+' /*2N*/ looped_initializer_clause
	| advance_search '-' /*2N*/
	;
colon_mark :
	 ':' /*2N*/
	;
elaborated_class_specifier :
	 class_key scoped_id %prec SHIFT_THERE /*1N*/
	| class_key scoped_id colon_mark error
	;
class_specifier_head :
	 class_key scoped_id colon_mark base_specifier_list '{' /*2N*/
	| class_key ':' /*2N*/ base_specifier_list '{' /*2N*/
	| class_key scoped_id '{' /*2N*/
	| class_key '{' /*2N*/
	;
class_key :
	 CLASS
	| STRUCT
	| UNION
	;
class_specifier :
	 class_specifier_head member_specification_opt '}'
	| class_specifier_head member_specification_opt util looping_member_declaration '#' bang error '}'
	;
member_specification_opt :
	 /*empty*/
	| member_specification_opt util looping_member_declaration
	| member_specification_opt util looping_member_declaration '#' bang error ';'
	;
looping_member_declaration :
	 start_search looped_member_declaration
	;
looped_member_declaration :
	 member_declaration
	| advance_search '+' /*2N*/ looped_member_declaration
	| advance_search '-' /*2N*/
	;
member_declaration :
	 accessibility_specifier
	| simple_member_declaration
	| function_definition
	| using_declaration
	| template_declaration
	;
simple_member_declaration :
	 ';'
	| assignment_expression ';'
	| constructor_head ';'
	| member_init_declarations ';'
	| decl_specifier_prefix simple_member_declaration
	;
member_init_declarations :
	 assignment_expression ',' member_init_declaration
	| constructor_head ',' bit_field_init_declaration
	| member_init_declarations ',' member_init_declaration
	;
member_init_declaration :
	 assignment_expression
	| bit_field_init_declaration
	;
accessibility_specifier :
	 access_specifier ':' /*2N*/
	;
bit_field_declaration :
	 assignment_expression ':' /*2N*/ bit_field_width
	| ':' /*2N*/ bit_field_width
	;
bit_field_width :
	 logical_or_expression
	| logical_or_expression '?' bit_field_width ':' /*2N*/ bit_field_width
	;
bit_field_init_declaration :
	 bit_field_declaration
	| bit_field_declaration '=' initializer_clause
	;
base_specifier_list :
	 base_specifier
	| base_specifier_list ',' base_specifier
	;
base_specifier :
	 scoped_id
	| access_specifier base_specifier
	| VIRTUAL base_specifier
	;
access_specifier :
	 PRIVATE
	| PROTECTED
	| PUBLIC
	;
conversion_function_id :
	 OPERATOR conversion_type_id
	;
conversion_type_id :
	 type_specifier ptr_operator_seq_opt
	| type_specifier conversion_type_id
	;
ctor_initializer_opt :
	 /*empty*/
	| ctor_initializer
	;
ctor_initializer :
	 ':' /*2N*/ mem_initializer_list
	| ':' /*2N*/ mem_initializer_list bang error
	;
mem_initializer_list :
	 mem_initializer
	| mem_initializer_list_head mem_initializer
	;
mem_initializer_list_head :
	 mem_initializer_list ','
	| mem_initializer_list bang error ','
	;
mem_initializer :
	 mem_initializer_id '(' /*4N*/ expression_list_opt ')'
	;
mem_initializer_id :
	 scoped_id
	;
operator_function_id :
	 OPERATOR operator
	;
operator :
	 NEW
	| DELETE
	| '+' /*2N*/
	| '-' /*2N*/
	| '*' /*2N*/
	| '/'
	| '%'
	| '^'
	| '&' /*2N*/
	| '|'
	| '~'
	| '!'
	| '='
	| '<' /*2N*/
	| '>'
	| ASS_ADD
	| ASS_SUB
	| ASS_MUL
	| ASS_DIV
	| ASS_MOD
	| ASS_XOR
	| ASS_AND
	| ASS_OR
	| SHL
	| SHR
	| ASS_SHR
	| ASS_SHL
	| EQ
	| NE
	| LE
	| GE
	| LOG_AND
	| LOG_OR
	| INC /*2N*/
	| DEC /*2N*/
	| ','
	| ARROW_STAR
	| ARROW
	| '(' /*4N*/ ')'
	| '[' /*2N*/ ']'
	;
template_declaration :
	 template_parameter_clause declaration
	| EXPORT template_declaration
	;
template_parameter_clause :
	 TEMPLATE '<' /*2N*/ template_parameter_list '>'
	;
template_parameter_list :
	 template_parameter
	| template_parameter_list ',' template_parameter
	;
template_parameter :
	 simple_type_parameter
	| simple_type_parameter '=' type_id
	| templated_type_parameter
	| templated_type_parameter '=' identifier
	| templated_parameter_declaration
	| bang error
	;
simple_type_parameter :
	 CLASS
	| TYPENAME
	;
templated_type_parameter :
	 template_parameter_clause CLASS
	| template_parameter_clause CLASS identifier
	;
template_id :
	 TEMPLATE identifier '<' /*2N*/ template_argument_list '>'
	| TEMPLATE template_id
	;
template_argument_list :
	 template_argument
	| template_argument_list ',' template_argument
	;
template_argument :
	 templated_parameter_declaration
	;
explicit_specialization :
	 TEMPLATE '<' /*2N*/ '>' declaration
	;
try_block :
	 TRY compound_statement handler_seq
	;
handler_seq :
	 handler
	| handler handler_seq
	;
handler :
	 CATCH '(' /*4N*/ exception_declaration ')' compound_statement
	;
exception_declaration :
	 parameter_declaration
	;
throw_expression :
	 THROW
	| THROW assignment_expression
	;
templated_throw_expression :
	 THROW
	| THROW templated_assignment_expression
	;
exception_specification :
	 THROW '(' /*4N*/ ')'
	| THROW '(' /*4N*/ type_id_list ')'
	;
type_id_list :
	 type_id
	| type_id_list ',' type_id
	;
advance_search :
	 error
	;
bang :
	 /*empty*/
	;
mark :
	 /*empty*/
	;
nest :
	 /*empty*/
	;
start_search :
	 /*empty*/
	;
start_search1 :
	 /*empty*/
	;
util :
	 /*empty*/
	;

// Tokens
//"\([^"]+\)"\s+{ LEX_STATIC_TOKEN(\([^)]+\)); }
//"\([^"]+\)"\s+{ LEX_C_STATIC_TOKEN(\([^)]+\)); }

//ATTRIBUTE : '__attribute__' ;
ASM : 'asm' ;
AUTO : 'auto' ;
BOOL : 'bool' ;
BREAK : 'break' ;
CASE : 'case' ;
CATCH : 'catch' ;
CHAR : 'char' ;
CLASS : 'class' ;
CONST : 'const' ;
CONST_CAST : 'const_cast' ;
CONTINUE : 'continue' ;
DEFAULT : 'default' ;
DELETE : 'delete' ;
DO : 'do' ;
DOUBLE : 'double' ;
DYNAMIC_CAST : 'dynamic_cast' ;
ELSE : 'else' ;
ENUM : 'enum' ;
EXPLICIT : 'explicit' ;
EXPORT : 'export' ;
EXTERN : 'extern' ;
FALSE : 'false' ;
FLOAT : 'float' ;
FOR : 'for' ;
FRIEND : 'friend' ;
GOTO : 'goto' ;
IF : 'if' ;
INLINE : 'inline' ;
INT : 'int' ;
LONG : 'long' ;
MUTABLE : 'mutable' ;
NAMESPACE : 'namespace' ;
NEW : 'new' ;
OPERATOR : 'operator' ;
PRIVATE : 'private' ;
PROTECTED : 'protected' ;
PUBLIC : 'public' ;
REGISTER : 'register' ;
REINTERPRET_CAST : 'reinterpret_cast' ;
RETURN : 'return' ;
SHORT : 'short' ;
SIGNED : 'signed' ;
SIZEOF : 'sizeof' ;
STATIC : 'static' ;
STATIC_CAST : 'static_cast' ;
STRUCT : 'struct' ;
SWITCH : 'switch' ;
TEMPLATE : 'template' ;
THIS : 'this' ;
THROW : 'throw' ;
TRUE : 'true' ;
TRY : 'try' ;
TYPEDEF : 'typedef' ;
TYPEID : 'typeid' ;
TYPENAME : 'typename' ;
UNION : 'union' ;
UNSIGNED : 'unsigned' ;
USING : 'using' ;
VIRTUAL : 'virtual' ;
VOID : 'void' ;
VOLATILE : 'volatile' ;
WCHAR_T : 'wchar_t' ;
WHILE : 'while' ;

SCOPE : '::' ;
ELLIPSIS : '...' ;
SHL : '<<' ;
SHR : '>>' ;
EQ : '==' ;
NE : '!=' ;
LE : '<=' ;
GE : '>=' ;
LOG_AND : '&&' ;
LOG_OR : '||' ;
INC : '++' ;
DEC : '--' ;
ARROW_STAR : '->*' ;
ARROW : '->' ;
DOT_STAR : '.*' ;
ASS_ADD : '+=' ;
ASS_SUB : '-=' ;
ASS_MUL : '*=' ;
ASS_DIV : '/=' ;
ASS_MOD : '%=' ;
ASS_XOR : '^=' ;
ASS_AND : '&=' ;
ASS_OR : '|=' ;
ASS_SHR : '>>=' ;
ASS_SHL : '<<=' ;

//digit : "[0-9]" ;
//hex : "[0-9A-Fa-f]" ;
//letter : "[A-Z_a-z]" ;
//simple_escape_sequence : "\\\'|\\\"|\\\?|\\\\|\\a|\\b|\\f|\\n|\\r|\\t|\\v" ;
//octal_escape_sequence : "\\[0-7]|\\[0-7][0-7]|\\[0-7][0-7][0-7]" ;
//hexadecimal_escape_sequence : "\\x[0-9A-Fa-f]+" ;
//escape_sequence : simple_escape_sequence | octal_escape_sequence | hexadecimal_escape_sequence ;
//universal_character_name	: '\\u' hex hex hex hex | '\\U' hex hex hex hex hex hex hex hex ;
//non_digit : letter | universal_character_name ;
//identifier : non_digit(non_digit|digit)* ;

//character_lit : "L?'([^'\\\n]|\\.)*" ;
//character_literal : character_lit '\'' ;

//string_lit : "L?\"([^\"\\\n]|\\.)*" ;
//string_literal : string_lit '"' ;

//pp_number : '.'? digit (digit | non_digit | "[eE][-+]" | '.')* ;
//pp_number : "\\.?[0-9]([0-9] | [eE][-+] | \\.)*" ;

Identifier : "[A-Z_a-z][A-Z_a-z0-9]*" ;
StringLiteral : "\"(\\.|[^\"\n])*\"" ;
IntegerLiteral : "[0-9]+" ;
CharacterLiteral : "'(\\.|[^'])[']" ;
FloatingLiteral : "([0-9]+\.[0-9]*|\.[0-9]+)([Ee](\+|\-)?[0-9]+)?" ;
}
