//From: https://github.com/Ada-France/ayacc/blob/master/examples/ada_parser/ada.y

ada_adayacc {

%whitespace "([ \t\r\n]*)|(\-\-[^\n]*)";

%case_insensitive ;

//%token '&'
//%token '\''
//%token '('
//%token ')'
//%token '*'
//%token '+'
//%token ','
//%token '-'
//%token '.'
//%token '/'
//%token ':'
//%token ';'
//%token '<'
//%token '='
//%token '>'
//%token '|'
//%token '!'
//%token ARROW
//%token DOUBLE_DOT
//%token DOUBLE_STAR
//%token ASSIGNMENT
//%token INEQUALITY
//%token GREATER_THAN_OR_EQUAL
//%token LESS_THAN_OR_EQUAL
//%token LEFT_LABEL_BRACKET
//%token RIGHT_LABEL_BRACKET
//%token BOX
//%token ABORT_TOKEN
//%token ABS_TOKEN
//%token ACCEPT_TOKEN
//%token ACCESS_TOKEN
//%token ALL_TOKEN
//%token AND_TOKEN
//%token ARRAY_TOKEN
//%token AT_TOKEN
//%token BEGIN_TOKEN
//%token BODY_TOKEN
//%token CASE_TOKEN
//%token CONSTANT_TOKEN
//%token DECLARE_TOKEN
//%token DELAY_TOKEN
//%token DELTA_TOKEN
//%token DIGITS_TOKEN
//%token DO_TOKEN
//%token ELSE_TOKEN
//%token ELSIF_TOKEN
//%token END_TOKEN
//%token ENTRY_TOKEN
//%token EXCEPTION_TOKEN
//%token EXIT_TOKEN
//%token FOR_TOKEN
//%token FUNCTION_TOKEN
//%token GENERIC_TOKEN
//%token GOTO_TOKEN
//%token IF_TOKEN
//%token IN_TOKEN
//%token IS_TOKEN
//%token LIMITED_TOKEN
//%token LOOP_TOKEN
//%token MOD_TOKEN
//%token NEW_TOKEN
//%token NOT_TOKEN
//%token NULL_TOKEN
//%token OF_TOKEN
//%token OR_TOKEN
//%token OTHERS_TOKEN
//%token OUT_TOKEN
//%token PACKAGE_TOKEN
//%token PRAGMA_TOKEN
//%token PRIVATE_TOKEN
//%token PROCEDURE_TOKEN
//%token RAISE_TOKEN
//%token RANGE_TOKEN
//%token RECORD_TOKEN
//%token REM_TOKEN
//%token RENAMES_TOKEN
//%token RETURN_TOKEN
//%token REVERSE_TOKEN
//%token SELECT_TOKEN
//%token SEPARATE_TOKEN
//%token SUBTYPE_TOKEN
//%token TASK_TOKEN
//%token TERMINATE_TOKEN
//%token THEN_TOKEN
//%token TYPE_TOKEN
//%token USE_TOKEN
//%token WHEN_TOKEN
//%token WHILE_TOKEN
//%token WITH_TOKEN
//%token XOR_TOKEN
//%token IDENTIFIER
//%token INTEGER_LITERAL
//%token REAL_LITERAL
//%token CHARACTER_LITERAL
//%token STRING_LITERAL
//%token ERROR1
//%token ERROR2
//%token ERROR3
//%token ERROR4
//%token ERROR5
//%token ERROR6
//%token ERROR7
//%token ERROR8
//%token ERROR9
//%token ERROR10
//%token ERROR11
//%token ERROR12
//%token ERROR13
//%token ERROR14
//%token ERROR15


//%start compilation

//%%

compilation :
	 __compilation_unit__
	;


prag :
	 PRAGMA_TOKEN IDENTIFIER _arg_ascs ';'
	;
arg_asc :
	 expr
	| IDENTIFIER ARROW expr
	;
numeric_literal :
	 REAL_LITERAL
	| INTEGER_LITERAL
	;
basic_d :
	 object_d
	| ty_d
	| subty_d
	| subprg_d
	| pkg_d
	| task_d
	| gen_d
	| excptn_d
	| gen_inst
	| renaming_d
	| number_d
	| error ';'
	;
object_d :
	 idents ':' subty_ind __ASN_expr_ ';'
	| idents ':' CONSTANT_TOKEN subty_ind __ASN_expr_ ';'
	| idents ':' c_arr_def __ASN_expr_ ';'
	| idents ':' CONSTANT_TOKEN c_arr_def __ASN_expr_ ';'
	;
number_d :
	 idents ':' CONSTANT_TOKEN ASSIGNMENT expr ';'
	;
idents :
	 IDENTIFIER ___ident__
	;
ty_d :
	 full_ty_d
	| incomplete_ty_d
	| priv_ty_d
	;
full_ty_d :
	 TYPE_TOKEN IDENTIFIER IS_TOKEN ty_def ';'
	| TYPE_TOKEN IDENTIFIER discr_part IS_TOKEN ty_def ';'
	;
ty_def :
	 enum_ty_def
	| integer_ty_def
	| real_ty_def
	| array_ty_def
	| rec_ty_def
	| access_ty_def
	| derived_ty_def
	;
subty_d :
	 SUBTYPE_TOKEN IDENTIFIER IS_TOKEN subty_ind ';'
	;
subty_ind :
	 ty_mk _constrt_
	;
ty_mk :
	 expanded_n
	;
constrt :
	 rng_c
	| fltg_point_c
	| fixed_point_c
	| aggr
	;
derived_ty_def :
	 NEW_TOKEN subty_ind
	;
rng_c :
	 RANGE_TOKEN rng
	;
rng :
	 name
	| sim_expr DOUBLE_DOT sim_expr
	;
enum_ty_def :
	 '(' enum_lit_spec ___enum_lit_spec__ ')'
	;
enum_lit_spec :
	 enum_lit
	;
enum_lit :
	 IDENTIFIER
	| CHARACTER_LITERAL
	;
integer_ty_def :
	 rng_c
	;
real_ty_def :
	 fltg_point_c
	| fixed_point_c
	;
fltg_point_c :
	 fltg_accuracy_def _rng_c_
	;
fltg_accuracy_def :
	 DIGITS_TOKEN sim_expr
	;
fixed_point_c :
	 fixed_accuracy_def _rng_c_
	;
fixed_accuracy_def :
	 DELTA_TOKEN sim_expr
	;
array_ty_def :
	 uncnstrnd_array_def
	| c_arr_def
	;
uncnstrnd_array_def :
	 ARRAY_TOKEN '(' idx_subty_def ___idx_subty_def__ ')' OF_TOKEN subty_ind
	;
c_arr_def :
	 ARRAY_TOKEN idx_c OF_TOKEN subty_ind
	;
idx_subty_def :
	 name RANGE_TOKEN BOX
	;
idx_c :
	 '(' dscr_rng ___dscr_rng__ ')'
	;
dscr_rng :
	 rng
	| name rng_c
	;
rec_ty_def :
	 RECORD_TOKEN cmpons END_TOKEN RECORD_TOKEN
	;
cmpons :
	 __prag__ __cmpon_d__ cmpon_d __prag__
	| __prag__ __cmpon_d__ variant_part __prag__
	| __prag__ NULL_TOKEN ';' __prag__
	;
cmpon_d :
	 idents ':' cmpon_subty_def __ASN_expr_ ';'
	;
cmpon_subty_def :
	 subty_ind
	;
discr_part :
	 '(' discr_spec ___discr_spec__ ')'
	;
discr_spec :
	 idents ':' ty_mk __ASN_expr_
	;
variant_part :
	 CASE_TOKEN sim_n IS_TOKEN __prag__ variant __variant__ END_TOKEN CASE_TOKEN ';'
	;
variant :
	 WHEN_TOKEN choice __or_choice__ ARROW cmpons
	;
paramchoice :
	 sim_expr
	| name rng_c
	| sim_expr DOUBLE_DOT sim_expr
	| OTHERS_TOKEN
	| error
	;
choice :
	 sim_expr
	| name rng_c
	| sim_expr DOUBLE_DOT sim_expr
	| OTHERS_TOKEN
	| error
	;
access_ty_def :
	 ACCESS_TOKEN subty_ind
	;
incomplete_ty_d :
	 TYPE_TOKEN IDENTIFIER ';'
	| TYPE_TOKEN IDENTIFIER discr_part ';'
	;
decl_part :
	 __basic_decl_item__
	| __basic_decl_item__ body __later_decl_item__
	;
basic_decl_item :
	 basic_d
	| rep_cl
	| use_cl
	;
later_decl_item :
	 body
	| subprg_d
	| pkg_d
	| task_d
	| gen_d
	| use_cl
	| gen_inst
	;
body :
	 proper_body
	| body_stub
	;
proper_body :
	 subprg_body
	| pkg_body
	| task_body
	;
name :
	 sim_n
	| CHARACTER_LITERAL
	| op_symbol
	| idxed_cmpon
	| selected_cmpon
	| attribute
	;
sim_n :
	 IDENTIFIER
	;
prefix :
	 name
	;
idxed_cmpon :
	 prefix aggr
	;
selected_cmpon :
	 prefix '.' selector
	;
selector :
	 sim_n
	| CHARACTER_LITERAL
	| op_symbol
	| ALL_TOKEN
	;
attribute :
	 prefix '\'' attribute_designator
	;
attribute_designator :
	 sim_n
	| DIGITS_TOKEN
	| DELTA_TOKEN
	| RANGE_TOKEN
	;
aggr :
	 '(' cmpon_asc ___cmpon_asc__ ')'
	;
cmpon_asc :
	 expr
	| paramchoice __or_paramchoice__ ARROW expr
	| sim_expr DOUBLE_DOT sim_expr
	| name rng_c
	;
expr :
	 rel__AND__rel__
	| rel__AND__THEN__rel__
	| rel__OR__rel__
	| rel__OR__ELSE__rel__
	| rel__XOR__rel__
	| xrel
	;
xrel :
	 sim_expr _relal_op__sim_expr_
	| sim_expr_NOT_IN__rng_or_sim_expr_NOT_IN__ty_mk
	;
rel :
	 sim_expr _relal_op__sim_expr_
	| sim_expr_NOT_IN__rng_or_sim_expr_NOT_IN__ty_mk
	;
sim_expr :
	 _unary_add_op_term__binary_add_op__term__
	;
term :
	 factor__mult_op__factor__
	;
factor :
	 pri __EXP___pri_
	| ABS_TOKEN pri
	| NOT_TOKEN '(' expr ')'
	| NOT_TOKEN qualified_expr
	| NOT_TOKEN name
	;
pri :
	 numeric_literal
	| NULL_TOKEN
	| allocator
	| qualified_expr
	| name
	| aggr
	;
relal_op :
	 '='
	| INEQUALITY
	| '<'
	| LESS_THAN_OR_EQUAL
	| '>'
	| GREATER_THAN_OR_EQUAL
	;
binary_add_op :
	 '+'
	| '-'
	| '&'
	;
unary_add_op :
	 '+'
	| '-'
	;
mult_op :
	 '*'
	| '/'
	| MOD_TOKEN
	| REM_TOKEN
	;
qualified_expr :
	 ty_mkaggr_or_ty_mkPexprP_
	;
allocator :
	 NEW_TOKEN ty_mk
	| NEW_TOKEN ty_mk aggr
	| NEW_TOKEN ty_mk '\'' aggr
	;
seq_of_stmts :
	 __prag__ stmt __stmt__
	;
stmt :
	 __label__ sim_stmt
	| __label__ compound_stmt
	| error ';'
	;
sim_stmt :
	 null_stmt
	| assignment_stmt
	| exit_stmt
	| return_stmt
	| goto_stmt
	| delay_stmt
	| abort_stmt
	| raise_stmt
	| code_stmt
	| name ';'
	;
compound_stmt :
	 if_stmt
	| case_stmt
	| loop_stmt
	| block_stmt
	| accept_stmt
	| select_stmt
	;
label :
	 LEFT_LABEL_BRACKET sim_n RIGHT_LABEL_BRACKET
	;
null_stmt :
	 NULL_TOKEN ';'
	;
assignment_stmt :
	 name ASSIGNMENT expr ';'
	;
if_stmt :
	 IF_TOKEN cond THEN_TOKEN seq_of_stmts __ELSIF__cond__THEN__seq_of_stmts__ _ELSE__seq_of_stmts_ END_TOKEN IF_TOKEN ';'
	;
cond :
	 expr
	;
case_stmt :
	 CASE_TOKEN expr IS_TOKEN case_stmt_alt__case_stmt_alt__ END_TOKEN CASE_TOKEN ';'
	;
case_stmt_alt :
	 WHEN_TOKEN choice __or_choice__ ARROW seq_of_stmts
	;
loop_stmt :
	 _sim_nC_ _iteration_scheme_ LOOP_TOKEN seq_of_stmts END_TOKEN LOOP_TOKEN _sim_n_ ';'
	;
type_of_while :
	 cond
	| error
	;
type_of_for :
	 loop_prm_spec
	| error
	;
iteration_scheme :
	 WHILE_TOKEN type_of_while
	| FOR_TOKEN type_of_for
	;
loop_prm_spec :
	 IDENTIFIER IN_TOKEN _REVERSE_ dscr_rng
	;
block_stmt :
	 _sim_nC_ _DECLARE__decl_part_ BEGIN_TOKEN seq_of_stmts _EXCEPTION__excptn_handler__excptn_handler___ END_TOKEN _sim_n_ ';'
	;
exit_stmt :
	 EXIT_TOKEN _expanded_n_ _WHEN__cond_ ';'
	;
return_stmt :
	 RETURN_TOKEN _expr_ ';'
	;
goto_stmt :
	 GOTO_TOKEN expanded_n ';'
	;
subprg_d :
	 subprg_spec ';'
	;
subprg_spec :
	 PROCEDURE_TOKEN IDENTIFIER _fml_part_
	| FUNCTION_TOKEN designator _fml_part_ RETURN_TOKEN ty_mk
	;
designator :
	 IDENTIFIER
	| op_symbol
	;
op_symbol :
	 STRING_LITERAL
	;
fml_part :
	 '(' prm_spec ____prm_spec__ ')'
	;
prm_spec :
	 idents ':' mode ty_mk __ASN_expr_
	;
mode :
	 _IN_
	| IN_TOKEN OUT_TOKEN
	| OUT_TOKEN
	;
subprg_body :
	 subprg_spec IS_TOKEN _decl_part_ BEGIN_TOKEN seq_of_stmts _EXCEPTION__excptn_handler__excptn_handler___ END_TOKEN _designator_ ';'
	;
pkg_d :
	 pkg_spec ';'
	;
pkg_spec :
	 PACKAGE_TOKEN IDENTIFIER IS_TOKEN __basic_decl_item__ _PRIVATE__basic_decl_item___ END_TOKEN _sim_n_
	;
pkg_body :
	 PACKAGE_TOKEN BODY_TOKEN sim_n IS_TOKEN _decl_part_ _BEGIN__seq_of_stmts_EXCEPTION__excptn_handler__excptn_handler___ END_TOKEN _sim_n_ ';'
	;
priv_ty_d :
	 TYPE_TOKEN IDENTIFIER IS_TOKEN _LIMITED_ PRIVATE_TOKEN ';'
	| TYPE_TOKEN IDENTIFIER discr_part IS_TOKEN _LIMITED_ PRIVATE_TOKEN ';'
	;
use_cl :
	 USE_TOKEN expanded_n ___expanded_n__ ';'
	;
renaming_d :
	 idents ':' ty_mk RENAMES_TOKEN name ';'
	| idents ':' EXCEPTION_TOKEN RENAMES_TOKEN expanded_n ';'
	| PACKAGE_TOKEN IDENTIFIER RENAMES_TOKEN expanded_n ';'
	| subprg_spec RENAMES_TOKEN name ';'
	;
task_d :
	 task_spec ';'
	;
task_spec :
	 TASK_TOKEN _TYPE_ IDENTIFIER _IS__ent_d___rep_cl_END_sim_n_
	;
task_body :
	 TASK_TOKEN BODY_TOKEN sim_n IS_TOKEN _decl_part_ BEGIN_TOKEN seq_of_stmts _EXCEPTION__excptn_handler__excptn_handler___ END_TOKEN _sim_n_ ';'
	;
ent_d :
	 ENTRY_TOKEN IDENTIFIER _fml_part_ ';'
	| ENTRY_TOKEN IDENTIFIER '(' dscr_rng ')' _fml_part_ ';'
	;
ent_call_stmt :
	 __prag__ name ';'
	;
accept_stmt :
	 ACCEPT_TOKEN sim_n _Pent_idx_P__fml_part_ _DO__seq_of_stmts__END_sim_n__ ';'
	;
ent_idx :
	 expr
	;
delay_stmt :
	 DELAY_TOKEN sim_expr ';'
	;
select_stmt :
	 selec_wait
	| condal_ent_call
	| timed_ent_call
	;
selec_wait :
	 SELECT_TOKEN select_alt __OR__select_alt__ _ELSE__seq_of_stmts_ END_TOKEN SELECT_TOKEN ';'
	;
select_alt :
	 _WHEN__condARROW_selec_wait_alt
	;
selec_wait_alt :
	 accept_alt
	| delay_alt
	| terminate_alt
	;
accept_alt :
	 accept_stmt_seq_of_stmts_
	;
delay_alt :
	 delay_stmt_seq_of_stmts_
	;
terminate_alt :
	 TERM_stmt
	;
condal_ent_call :
	 SELECT_TOKEN ent_call_stmt _seq_of_stmts_ ELSE_TOKEN seq_of_stmts END_TOKEN SELECT_TOKEN ';'
	;
timed_ent_call :
	 SELECT_TOKEN ent_call_stmt _seq_of_stmts_ OR_TOKEN delay_alt END_TOKEN SELECT_TOKEN ';'
	;
abort_stmt :
	 ABORT_TOKEN name ___name__ ';'
	;
compilation_unit :
	 context_cl library_unit
	| context_cl secondary_unit
	;
library_unit :
	 subprg_d
	| pkg_d
	| gen_d
	| gen_inst
	| subprg_body
	;
secondary_unit :
	 library_unit_body
	| subunit
	;
library_unit_body :
	 pkg_body_or_subprg_body
	;
context_cl :
	 __with_cl__use_cl____
	;
with_cl :
	 WITH_TOKEN sim_n ___sim_n__ ';'
	;
body_stub :
	 subprg_spec IS_TOKEN SEPARATE_TOKEN ';'
	| PACKAGE_TOKEN BODY_TOKEN sim_n IS_TOKEN SEPARATE_TOKEN ';'
	| TASK_TOKEN BODY_TOKEN sim_n IS_TOKEN SEPARATE_TOKEN ';'
	;
subunit :
	 SEPARATE_TOKEN '(' expanded_n ')' proper_body
	;
excptn_d :
	 idents ':' EXCEPTION_TOKEN ';'
	;
excptn_handler :
	 WHEN_TOKEN excptn_choice __or_excptn_choice__ ARROW seq_of_stmts
	;
excptn_choice :
	 expanded_n
	| OTHERS_TOKEN
	;
raise_stmt :
	 RAISE_TOKEN _expanded_n_ ';'
	;
gen_d :
	 gen_spec ';'
	;
gen_spec :
	 gen_fml_part subprg_spec
	| gen_fml_part pkg_spec
	;
gen_fml_part :
	 GENERIC_TOKEN __gen_prm_d__
	;
gen_prm_d :
	 idents ':' _IN_OUT__ ty_mk __ASN_expr_ ';'
	| TYPE_TOKEN IDENTIFIER IS_TOKEN gen_ty_def ';'
	| priv_ty_d
	| WITH_TOKEN subprg_spec _IS_BOX__ ';'
	;
gen_ty_def :
	 '(' BOX ')'
	| RANGE_TOKEN BOX
	| DIGITS_TOKEN BOX
	| DELTA_TOKEN BOX
	| array_ty_def
	| access_ty_def
	;
gen_inst :
	 PACKAGE_TOKEN IDENTIFIER IS_TOKEN NEW_TOKEN expanded_n _gen_act_part_ ';'
	| PROCEDURE__ident__IS_ NEW_TOKEN expanded_n _gen_act_part_ ';'
	| FUNCTION_TOKEN designator IS_TOKEN NEW_TOKEN expanded_n _gen_act_part_ ';'
	;
gen_act_part :
	 '(' gen_asc ___gen_asc__ ')'
	;
gen_asc :
	 _gen_fml_prmARROW_gen_act_prm
	;
gen_fml_prm :
	 sim_n
	| op_symbol
	;
gen_act_prm :
	 expr_or_name_or_subprg_n_or_ent_n_or_ty_mk
	;
rep_cl :
	 ty_rep_cl
	| address_cl
	;
ty_rep_cl :
	 length_cl
	| enum_rep_cl
	| rec_rep_cl
	;
length_cl :
	 FOR_TOKEN attribute USE_TOKEN sim_expr ';'
	;
enum_rep_cl :
	 FOR__ty_sim_n__USE_ aggr ';'
	;
rec_rep_cl :
	 FOR__ty_sim_n__USE_ RECORD_TOKEN _algt_cl_ __cmpon_cl__ END_TOKEN RECORD_TOKEN ';'
	;
algt_cl :
	 AT_TOKEN MOD_TOKEN sim_expr ';'
	;
cmpon_cl :
	 name AT_TOKEN sim_expr RANGE_TOKEN rng ';'
	;
address_cl :
	 FOR_TOKEN sim_n USE_TOKEN AT_TOKEN sim_expr ';'
	;
code_stmt :
	 ty_mk_rec_aggr ';'
	;
__prag__ :
	 /*empty*/
	| __prag__ prag
	;
_arg_ascs :
	 /*empty*/
	| '(' arg_ascs ')'
	;
arg_ascs :
	 arg_asc
	| arg_ascs ',' arg_asc
	;
__ASN_expr_ :
	 /*empty*/
	| ASSIGNMENT expr
	;
___ident__ :
	 /*empty*/
	| ___ident__ ',' IDENTIFIER
	;
_constrt_ :
	 /*empty*/
	| constrt
	;
expanded_n :
	 IDENTIFIER
	| expanded_n '.' IDENTIFIER
	;
___enum_lit_spec__ :
	 /*empty*/
	| ___enum_lit_spec__ ',' enum_lit_spec
	;
_rng_c_ :
	 /*empty*/
	| rng_c
	;
___idx_subty_def__ :
	 /*empty*/
	| ___idx_subty_def__ ',' idx_subty_def
	;
___dscr_rng__ :
	 /*empty*/
	| ___dscr_rng__ ',' dscr_rng
	;
__cmpon_d__ :
	 /*empty*/
	| __cmpon_d__ cmpon_d __prag__
	;
___discr_spec__ :
	 /*empty*/
	| ___discr_spec__ ';' discr_spec
	;
__variant__ :
	 /*empty*/
	| __variant__ variant
	;
__or_choice__ :
	 /*empty*/
	| __or_choice__ '|' choice
	| __or_choice__ '!' choice
	;
__or_paramchoice__ :
	 /*empty*/
	| __or_paramchoice__ '|' paramchoice
	| __or_paramchoice__ '!' paramchoice
	;
__basic_decl_item__ :
	 __prag__
	| __basic_decl_item__ basic_decl_item __prag__
	;
__later_decl_item__ :
	 __prag__
	| __later_decl_item__ later_decl_item __prag__
	;
___cmpon_asc__ :
	 /*empty*/
	| ___cmpon_asc__ ',' cmpon_asc
	;
rel__AND__rel__ :
	 rel AND_TOKEN rel
	| rel__AND__rel__ AND_TOKEN rel
	;
rel__OR__rel__ :
	 rel OR_TOKEN rel
	| rel__OR__rel__ OR_TOKEN rel
	;
rel__XOR__rel__ :
	 rel XOR_TOKEN rel
	| rel__XOR__rel__ XOR_TOKEN rel
	;
rel__AND__THEN__rel__ :
	 rel AND_TOKEN THEN_TOKEN rel
	| rel__AND__THEN__rel__ AND_TOKEN THEN_TOKEN rel
	;
rel__OR__ELSE__rel__ :
	 rel OR_TOKEN ELSE_TOKEN rel
	| rel__OR__ELSE__rel__ OR_TOKEN ELSE_TOKEN rel
	;
_relal_op__sim_expr_ :
	 /*empty*/
	| relal_op sim_expr
	;
sim_expr_NOT_IN__rng_or_sim_expr_NOT_IN__ty_mk :
	 sim_expr _NOT_ IN_TOKEN rng
	;
_NOT_ :
	 /*empty*/
	| NOT_TOKEN
	;
_unary_add_op_term__binary_add_op__term__ :
	 term
	| unary_add_op term
	| _unary_add_op_term__binary_add_op__term__ binary_add_op term
	;
factor__mult_op__factor__ :
	 factor
	| factor__mult_op__factor__ mult_op factor
	;
__EXP___pri_ :
	 /*empty*/
	| DOUBLE_STAR pri
	;
ty_mkaggr_or_ty_mkPexprP_ :
	 prefix '\'' aggr
	;
__stmt__ :
	 __prag__
	| __stmt__ stmt __prag__
	;
__label__ :
	 /*empty*/
	| __label__ label
	;
__ELSIF__cond__THEN__seq_of_stmts__ :
	 /*empty*/
	| __ELSIF__cond__THEN__seq_of_stmts__ ELSIF_TOKEN cond THEN_TOKEN seq_of_stmts
	;
_ELSE__seq_of_stmts_ :
	 /*empty*/
	| ELSE_TOKEN seq_of_stmts
	;
case_stmt_alt__case_stmt_alt__ :
	 __prag__ case_stmt_alt __case_stmt_alt__
	;
__case_stmt_alt__ :
	 /*empty*/
	| __case_stmt_alt__ case_stmt_alt
	;
_sim_nC_ :
	 /*empty*/
	| sim_n ':'
	;
_sim_n_ :
	 /*empty*/
	| sim_n
	;
_iteration_scheme_ :
	 /*empty*/
	| iteration_scheme
	;
_REVERSE_ :
	 /*empty*/
	| REVERSE_TOKEN
	;
_DECLARE__decl_part_ :
	 /*empty*/
	| DECLARE_TOKEN decl_part
	;
_EXCEPTION__excptn_handler__excptn_handler___ :
	 /*empty*/
	| EXCEPTION_TOKEN __prag__ excptn_handlers
	;
excptn_handlers :
	 excptn_handler
	| excptn_handlers excptn_handler
	;
_expanded_n_ :
	 /*empty*/
	| expanded_n
	;
_WHEN__cond_ :
	 /*empty*/
	| WHEN_TOKEN cond
	;
_expr_ :
	 /*empty*/
	| expr
	;
_fml_part_ :
	 /*empty*/
	| fml_part
	;
____prm_spec__ :
	 /*empty*/
	| ____prm_spec__ ';' prm_spec
	;
_IN_ :
	 /*empty*/
	| IN_TOKEN
	;
_decl_part_ :
	 decl_part
	;
_designator_ :
	 /*empty*/
	| designator
	;
_PRIVATE__basic_decl_item___ :
	 /*empty*/
	| PRIVATE_TOKEN __basic_decl_item__
	;
_BEGIN__seq_of_stmts_EXCEPTION__excptn_handler__excptn_handler___ :
	 /*empty*/
	| BEGIN_TOKEN seq_of_stmts _EXCEPTION__excptn_handler__excptn_handler___
	;
_LIMITED_ :
	 /*empty*/
	| LIMITED_TOKEN
	;
___expanded_n__ :
	 /*empty*/
	| ___expanded_n__ ',' expanded_n
	;
_TYPE_ :
	 /*empty*/
	| TYPE_TOKEN
	;
_IS__ent_d___rep_cl_END_sim_n_ :
	 /*empty*/
	| IS_TOKEN __ent_d__ __rep_cl__ END_TOKEN _sim_n_
	;
__ent_d__ :
	 __prag__
	| __ent_d__ ent_d __prag__
	;
__rep_cl__ :
	 /*empty*/
	| __rep_cl__ rep_cl __prag__
	;
_Pent_idx_P__fml_part_ :
	 _fml_part_
	| '(' ent_idx ')' _fml_part_
	;
_DO__seq_of_stmts__END_sim_n__ :
	 /*empty*/
	| DO_TOKEN seq_of_stmts END_TOKEN _sim_n_
	;
__OR__select_alt__ :
	 /*empty*/
	| __OR__select_alt__ OR_TOKEN select_alt
	;
_WHEN__condARROW_selec_wait_alt :
	 selec_wait_alt
	| WHEN_TOKEN cond ARROW selec_wait_alt
	;
accept_stmt_seq_of_stmts_ :
	 __prag__ accept_stmt _seq_of_stmts_
	;
delay_stmt_seq_of_stmts_ :
	 __prag__ delay_stmt _seq_of_stmts_
	;
TERM_stmt :
	 __prag__ TERMINATE_TOKEN ';' __prag__
	;
_seq_of_stmts_ :
	 __prag__
	| seq_of_stmts
	;
___name__ :
	 /*empty*/
	| ___name__ ',' name
	;
__compilation_unit__ :
	 __prag__
	| __compilation_unit__ compilation_unit __prag__
	;
pkg_body_or_subprg_body :
	 pkg_body
	;
__with_cl__use_cl____ :
	 /*empty*/
	| __with_cl__use_cl____ with_cl use_cls
	;
use_cls :
	 __prag__
	| use_cls use_cl __prag__
	;
___sim_n__ :
	 /*empty*/
	| ___sim_n__ ',' sim_n
	;
__or_excptn_choice__ :
	 /*empty*/
	| __or_excptn_choice__ '|' excptn_choice
	| __or_excptn_choice__ '!' excptn_choice
	;
__gen_prm_d__ :
	 /*empty*/
	| __gen_prm_d__ gen_prm_d
	;
_IN_OUT__ :
	 _IN_
	| IN_TOKEN OUT_TOKEN
	;
_IS_BOX__ :
	 /*empty*/
	| IS_TOKEN name
	| IS_TOKEN BOX
	;
PROCEDURE__ident__IS_ :
	 subprg_spec IS_TOKEN
	;
_gen_act_part_ :
	 /*empty*/
	| gen_act_part
	;
___gen_asc__ :
	 /*empty*/
	| ___gen_asc__ ',' gen_asc
	;
_gen_fml_prmARROW_gen_act_prm :
	 gen_act_prm
	| gen_fml_prm ARROW gen_act_prm
	;
expr_or_name_or_subprg_n_or_ent_n_or_ty_mk :
	 expr
	;
FOR__ty_sim_n__USE_ :
	 FOR_TOKEN sim_n USE_TOKEN
	;
_algt_cl_ :
	 __prag__
	| __prag__ algt_cl __prag__
	;
__cmpon_cl__ :
	 /*empty*/
	| __cmpon_cl__ cmpon_cl __prag__
	;
ty_mk_rec_aggr :
	 qualified_expr
	;

//Lexer

// \({\S+\)\s+{ECHO_L; ENTER(Z); return(\([^)]+\));}
ABORT_TOKEN : 'abort' ;
ABS_TOKEN : 'abs' ;
ACCEPT_TOKEN : 'accept' ;
ACCESS_TOKEN : 'access' ;
ALL_TOKEN : 'all' ;
AND_TOKEN : 'and' ;
ARRAY_TOKEN : 'array' ;
AT_TOKEN : 'at' ;
BEGIN_TOKEN : 'begin' ;
BODY_TOKEN : 'body' ;
CASE_TOKEN : 'case' ;
CONSTANT_TOKEN : 'constant' ;
DECLARE_TOKEN : 'declare' ;
DELAY_TOKEN : 'delay' ;
DELTA_TOKEN : 'delta' ;
DIGITS_TOKEN : 'digits' ;
DO_TOKEN : 'do' ;
ELSE_TOKEN : 'else' ;
ELSIF_TOKEN : 'elsif' ;
END_TOKEN : 'end' ;
ENTRY_TOKEN : 'entry' ;
EXCEPTION_TOKEN : 'exception' ;
EXIT_TOKEN : 'exit' ;
FOR_TOKEN : 'for' ;
FUNCTION_TOKEN : 'function' ;
GENERIC_TOKEN : 'generic' ;
GOTO_TOKEN : 'goto' ;
IF_TOKEN : 'if' ;
IN_TOKEN : 'in' ;
IS_TOKEN : 'is' ;
LIMITED_TOKEN : 'limited' ;
LOOP_TOKEN : 'loop' ;
MOD_TOKEN : 'mod' ;
NEW_TOKEN : 'new' ;
NOT_TOKEN : 'not' ;
NULL_TOKEN : 'null' ;
OF_TOKEN : 'of' ;
OR_TOKEN : 'or' ;
OTHERS_TOKEN : 'others' ;
OUT_TOKEN : 'out' ;
PACKAGE_TOKEN : 'package' ;
PRAGMA_TOKEN : 'pragma' ;
PRIVATE_TOKEN : 'private' ;
PROCEDURE_TOKEN : 'procedure' ;
RAISE_TOKEN : 'raise' ;
RANGE_TOKEN : 'range' ;
RECORD_TOKEN : 'record' ;
REM_TOKEN : 'rem' ;
RENAMES_TOKEN : 'renames' ;
RETURN_TOKEN : 'return' ;
REVERSE_TOKEN : 'reverse' ;
SELECT_TOKEN : 'select' ;
SEPARATE_TOKEN : 'separate' ;
SUBTYPE_TOKEN : 'subtype' ;
TASK_TOKEN : 'task' ;
TERMINATE_TOKEN : 'terminate' ;
THEN_TOKEN : 'then' ;
TYPE_TOKEN : 'type' ;
USE_TOKEN : 'use' ;
WHEN_TOKEN : 'when' ;
WHILE_TOKEN : 'while' ;
WITH_TOKEN : 'with' ;
XOR_TOKEN : 'xor' ;

  //-- Match all the compound Ada delimiters.  See Ada LRM 2.2.

ARROW : '=>' ;
DOUBLE_DOT : '..' ;
DOUBLE_STAR : '**' ;
ASSIGNMENT : ':=' ;
INEQUALITY : '/=' ;
GREATER_THAN_OR_EQUAL : '>=' ;
LESS_THAN_OR_EQUAL : '<=' ;
LEFT_LABEL_BRACKET : '<<' ;
RIGHT_LABEL_BRACKET : '>>' ;
BOX : '<>' ;

IDENTIFIER : "[a-zA-Z]([_]?[a-zA-Z0-9])*" ;
REAL_LITERAL : "([0-9]+\.[0-9]*|\.[0-9]+)([Ee](\+|\-)?[0-9]+)?" ;
INTEGER_LITERAL : "[0-9]+" ;
CHARACTER_LITERAL : "'(\\.|[^'])[']" ;
STRING_LITERAL : "\"(\\.|[^\"\n])*\"" ;

}