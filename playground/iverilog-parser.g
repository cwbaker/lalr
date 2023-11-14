//From: https://github.com/steveicarus/iverilog/blob/master/parse.y
iverilog {

//%error_recovery_show;
%error_recovery_off;
%whitespace "[ \t\r\n]*";
%whitespace "//[^\n\r]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

/*Tokens*/
//%token YYEOF
//%token YYerror
//%token YYUNDEF
//%token IDENTIFIER
//%token SYSTEM_IDENTIFIER
//%token STRING
//%token TIME_LITERAL
//%token TYPE_IDENTIFIER
//%token PACKAGE_IDENTIFIER
//%token DISCIPLINE_IDENTIFIER
//%token PATHPULSE_IDENTIFIER
//%token BASED_NUMBER
//%token DEC_NUMBER
//%token UNBASED_NUMBER
//%token REALTIME
//%token K_PLUS_EQ
//%token K_MINUS_EQ
//%token K_INCR
//%token K_DECR
//%token K_LE
//%token K_GE
//%token K_EG
//%token K_EQ
//%token K_NE
//%token K_CEQ
//%token K_CNE
//%token K_WEQ
//%token K_WNE
//%token K_LP
//%token K_LS
//%token K_RS
//%token K_RSS
//%token K_SG
//%token K_CONTRIBUTE
//%token K_PO_POS
//%token K_PO_NEG
//%token K_POW
//%token K_PSTAR
//%token K_STARP
//%token K_DOTSTAR
//%token K_LOR
//%token K_LAND
//%token K_NAND
//%token K_NOR
//%token K_NXOR
//%token K_TRIGGER
//%token K_NB_TRIGGER
//%token K_LEQUIV
//%token K_SCOPE_RES
//%token K_edge_descriptor
//%token K_CONSTRAINT_IMPL
//%token K_always
//%token K_and
//%token K_assign
//%token K_begin
//%token K_buf
//%token K_bufif0
//%token K_bufif1
//%token K_case
//%token K_casex
//%token K_casez
//%token K_cmos
//%token K_deassign
//%token K_default
//%token K_defparam
//%token K_disable
//%token K_edge
//%token K_else
//%token K_end
//%token K_endcase
//%token K_endfunction
//%token K_endmodule
//%token K_endprimitive
//%token K_endspecify
//%token K_endtable
//%token K_endtask
//%token K_event
//%token K_for
//%token K_force
//%token K_forever
//%token K_fork
//%token K_function
//%token K_highz0
//%token K_highz1
//%token K_if
//%token K_ifnone
//%token K_initial
//%token K_inout
//%token K_input
//%token K_integer
//%token K_join
//%token K_large
//%token K_macromodule
//%token K_medium
//%token K_module
//%token K_nand
//%token K_negedge
//%token K_nmos
//%token K_nor
//%token K_not
//%token K_notif0
//%token K_notif1
//%token K_or
//%token K_output
//%token K_parameter
//%token K_pmos
//%token K_posedge
//%token K_primitive
//%token K_pull0
//%token K_pull1
//%token K_pulldown
//%token K_pullup
//%token K_rcmos
//%token K_real
//%token K_realtime
//%token K_reg
//%token K_release
//%token K_repeat
//%token K_rnmos
//%token K_rpmos
//%token K_rtran
//%token K_rtranif0
//%token K_rtranif1
//%token K_scalared
//%token K_small
//%token K_specify
//%token K_specparam
//%token K_strong0
//%token K_strong1
//%token K_supply0
//%token K_supply1
//%token K_table
//%token K_task
//%token K_time
//%token K_tran
//%token K_tranif0
//%token K_tranif1
//%token K_tri
//%token K_tri0
//%token K_tri1
//%token K_triand
//%token K_trior
//%token K_trireg
//%token K_vectored
//%token K_wait
//%token K_wand
//%token K_weak0
//%token K_weak1
//%token K_while
//%token K_wire
//%token K_wor
//%token K_xnor
//%token K_xor
//%token K_Shold
//%token K_Snochange
//%token K_Speriod
//%token K_Srecovery
//%token K_Ssetup
//%token K_Ssetuphold
//%token K_Sskew
//%token K_Swidth
//%token KK_attribute
//%token K_bool
//%token K_logic
//%token K_automatic
//%token K_endgenerate
//%token K_generate
//%token K_genvar
//%token K_localparam
//%token K_noshowcancelled
//%token K_pulsestyle_onevent
//%token K_pulsestyle_ondetect
//%token K_showcancelled
//%token K_signed
//%token K_unsigned
//%token K_Sfullskew
//%token K_Srecrem
//%token K_Sremoval
//%token K_Stimeskew
//%token K_cell
//%token K_config
//%token K_design
//%token K_endconfig
//%token K_incdir
//%token K_include
//%token K_instance
//%token K_liblist
//%token K_library
//%token K_use
//%token K_wone
//%token K_uwire
//%token K_alias
//%token K_always_comb
//%token K_always_ff
//%token K_always_latch
//%token K_assert
//%token K_assume
//%token K_before
//%token K_bind
//%token K_bins
//%token K_binsof
//%token K_bit
//%token K_break
//%token K_byte
//%token K_chandle
//%token K_class
//%token K_clocking
//%token K_const
//%token K_constraint
//%token K_context
//%token K_continue
//%token K_cover
//%token K_covergroup
//%token K_coverpoint
//%token K_cross
//%token K_dist
//%token K_do
//%token K_endclass
//%token K_endclocking
//%token K_endgroup
//%token K_endinterface
//%token K_endpackage
//%token K_endprogram
//%token K_endproperty
//%token K_endsequence
//%token K_enum
//%token K_expect
//%token K_export
//%token K_extends
//%token K_extern
//%token K_final
//%token K_first_match
//%token K_foreach
//%token K_forkjoin
//%token K_iff
//%token K_ignore_bins
//%token K_illegal_bins
//%token K_import
//%token K_inside
//%token K_int
//%token K_interface
//%token K_intersect
//%token K_join_any
//%token K_join_none
//%token K_local
//%token K_longint
//%token K_matches
//%token K_modport
//%token K_new
//%token K_null
//%token K_package
//%token K_packed
//%token K_priority
//%token K_program
//%token K_property
//%token K_protected
//%token K_pure
//%token K_rand
//%token K_randc
//%token K_randcase
//%token K_randsequence
//%token K_ref
//%token K_return
//%token K_sequence
//%token K_shortint
//%token K_shortreal
//%token K_solve
//%token K_static
//%token K_string
//%token K_struct
//%token K_super
//%token K_tagged
//%token K_this
//%token K_throughout
//%token K_timeprecision
//%token K_timeunit
//%token K_type
//%token K_typedef
//%token K_union
//%token K_unique
//%token K_var
//%token K_virtual
//%token K_void
//%token K_wait_order
//%token K_wildcard
//%token K_with
//%token K_within
//%token K_accept_on
//%token K_checker
//%token K_endchecker
//%token K_eventually
//%token K_global
//%token K_implies
//%token K_let
//%token K_nexttime
//%token K_reject_on
//%token K_restrict
//%token K_s_always
//%token K_s_eventually
//%token K_s_nexttime
//%token K_s_until
//%token K_s_until_with
//%token K_strong
//%token K_sync_accept_on
//%token K_sync_reject_on
//%token K_unique0
//%token K_until
//%token K_until_with
//%token K_untyped
//%token K_weak
//%token K_implements
//%token K_interconnect
//%token K_nettype
//%token K_soft
//%token K_above
//%token K_abs
//%token K_absdelay
//%token K_abstol
//%token K_access
//%token K_acos
//%token K_acosh
//%token K_ac_stim
//%token K_aliasparam
//%token K_analog
//%token K_analysis
//%token K_asin
//%token K_asinh
//%token K_atan
//%token K_atan2
//%token K_atanh
//%token K_branch
//%token K_ceil
//%token K_connect
//%token K_connectmodule
//%token K_connectrules
//%token K_continuous
//%token K_cos
//%token K_cosh
//%token K_ddt
//%token K_ddt_nature
//%token K_ddx
//%token K_discipline
//%token K_discrete
//%token K_domain
//%token K_driver_update
//%token K_endconnectrules
//%token K_enddiscipline
//%token K_endnature
//%token K_endparamset
//%token K_exclude
//%token K_exp
//%token K_final_step
//%token K_flicker_noise
//%token K_floor
//%token K_flow
//%token K_from
//%token K_ground
//%token K_hypot
//%token K_idt
//%token K_idtmod
//%token K_idt_nature
//%token K_inf
//%token K_initial_step
//%token K_laplace_nd
//%token K_laplace_np
//%token K_laplace_zd
//%token K_laplace_zp
//%token K_last_crossing
//%token K_limexp
//%token K_ln
//%token K_log
//%token K_max
//%token K_merged
//%token K_min
//%token K_nature
//%token K_net_resolution
//%token K_noise_table
//%token K_paramset
//%token K_potential
//%token K_pow
//%token K_resolveto
//%token K_sin
//%token K_sinh
//%token K_slew
//%token K_split
//%token K_sqrt
//%token K_tan
//%token K_tanh
//%token K_timer
//%token K_transition
//%token K_units
//%token K_white_noise
//%token K_wreal
//%token K_zi_nd
//%token K_zi_np
//%token K_zi_zd
//%token K_zi_zp
//%token K_TAND
//%token K_MUL_EQ
//%token K_DIV_EQ
//%token K_MOD_EQ
//%token K_AND_EQ
//%token K_OR_EQ
//%token K_XOR_EQ
//%token K_LS_EQ
//%token K_RS_EQ
//%token K_RSS_EQ
//%token '?'
//%token ':'
//%token '|'
//%token '^'
//%token '&'
//%token '<'
//%token '>'
//%token '+'
//%token '-'
//%token '*'
//%token '/'
//%token '%'
//%token UNARY_PREC
//%token less_than_K_else
//%token '('
//%token no_timeunits_declaration
//%token one_timeunits_declaration
//%token '}'
//%token ';'
//%token ')'
//%token '{'
//%token '['
//%token ']'
//%token '#'
//%token ','
//%token '='
//%token '.'
//%token '$'
//%token '@'
//%token '~'
//%token '!'
//%token '\''
//%token '0'
//%token '1'
//%token 'x'
//%token 'b'
//%token 'f'
//%token 'F'
//%token 'l'
//%token 'h'
//%token 'B'
//%token 'r'
//%token 'R'
//%token 'M'
//%token 'n'
//%token 'N'
//%token 'p'
//%token 'P'
//%token 'Q'
//%token 'q'
//%token '_'

%nonassoc /*1*/ K_PLUS_EQ K_MINUS_EQ K_MUL_EQ K_DIV_EQ K_MOD_EQ K_AND_EQ K_OR_EQ ;
%nonassoc /*2*/ K_NB_TRIGGER K_XOR_EQ K_LS_EQ K_RS_EQ K_RSS_EQ ;
%right /*3*/ K_TRIGGER K_LEQUIV ;
%right /*4*/ K_inside '?' ':' ;
%left /*5*/ K_LOR ;
%left /*6*/ K_LAND ;
%left /*7*/ '|' ;
%left /*8*/ K_NOR K_NXOR '^' ;
%left /*9*/ K_NAND '&' ;
%left /*10*/ K_EQ K_NE K_CEQ K_CNE K_WEQ K_WNE ;
%left /*11*/ K_LE K_GE '<' '>' ;
%left /*12*/ K_LS K_RS K_RSS ;
%left /*13*/ '+' '-' ;
%left /*14*/ '*' '/' '%' ;
%left /*15*/ K_POW ;
%left /*16*/ UNARY_PREC ;
%nonassoc /*17*/ less_than_K_else ;
%nonassoc /*18*/ K_else ;
%nonassoc /*19*/ '(' ;
%nonassoc /*20*/ K_exclude ;
%nonassoc /*21*/ no_timeunits_declaration ;
%nonassoc /*22*/ one_timeunits_declaration ;
%nonassoc /*23*/ K_timeprecision K_timeunit ;

//%start source_text

//%%

source_text :
	/*YYEOF
	|*/ timeunits_declaration_opt description_list
	| /*empty*/
	;

assert_or_assume :
	K_assert
	| K_assume
	;

assertion_item :
	concurrent_assertion_item
	| deferred_immediate_assertion_item
	;

assignment_pattern :
	K_LP expression_list_proper '}'
	| K_LP '}'
	;

block_identifier_opt :
	IDENTIFIER ':' /*4R*/
	| /*empty*/
	;

class_declaration :
	K_virtual_opt K_class lifetime_opt identifier_name class_declaration_extends_opt ';' class_items_opt K_endclass class_declaration_endlabel_opt
	;

class_constraint :
	constraint_prototype
	| constraint_declaration
	;

identifier_name :
	IDENTIFIER
	| TYPE_IDENTIFIER
	;

class_declaration_endlabel_opt :
	':' /*4R*/ identifier_name
	| /*empty*/
	;

class_declaration_extends_opt :
	K_extends ps_type_identifier argument_list_parens_opt
	| /*empty*/
	;

class_items_opt :
	class_items
	| /*empty*/
	;

class_items :
	class_items class_item
	| class_item
	;

class_item :
	method_qualifier_opt K_function K_new tf_port_list_parens_opt ';' block_item_decls_opt statement_or_null_list_opt K_endfunction endnew_opt
	| property_qualifier_opt data_type list_of_variable_decl_assignments ';'
	| K_const class_item_qualifier_opt data_type list_of_variable_decl_assignments ';'
	| property_qualifier_opt type_declaration
	| method_qualifier_opt task_declaration
	| method_qualifier_opt function_declaration
	| K_extern method_qualifier_opt K_function K_new tf_port_list_parens_opt ';'
	| K_extern method_qualifier_opt K_function data_type_or_implicit_or_void IDENTIFIER tf_port_list_parens_opt ';'
	| K_extern method_qualifier_opt K_task IDENTIFIER tf_port_list_parens_opt ';'
	| class_constraint
	| property_qualifier_opt data_type error ';'
	| property_qualifier_opt IDENTIFIER error ';'
	| method_qualifier_opt K_function K_new error K_endfunction endnew_opt
	| parameter_declaration
	| ';'
	| error ';'
	;

class_item_qualifier :
	K_static
	| K_protected
	| K_local
	;

class_item_qualifier_list :
	class_item_qualifier_list class_item_qualifier
	| class_item_qualifier
	;

class_item_qualifier_opt :
	class_item_qualifier_list
	| /*empty*/
	;

class_scope :
	ps_type_identifier K_SCOPE_RES
	;

class_new :
	K_new argument_list_parens_opt
	| class_scope K_new argument_list_parens_opt
	| K_new hierarchy_identifier
	;

concurrent_assertion_item :
	block_identifier_opt concurrent_assertion_statement
	;

concurrent_assertion_statement :
	assert_or_assume K_property '(' /*19N*/ property_spec ')' statement_or_null %prec less_than_K_else /*17N*/
	| assert_or_assume K_property '(' /*19N*/ property_spec ')' K_else /*18N*/ statement_or_null
	| assert_or_assume K_property '(' /*19N*/ property_spec ')' statement_or_null K_else /*18N*/ statement_or_null
	| K_cover K_property '(' /*19N*/ property_spec ')' statement_or_null
	| K_cover K_sequence '(' /*19N*/ property_spec ')' statement_or_null
	| K_restrict K_property '(' /*19N*/ property_spec ')' ';'
	| assert_or_assume K_property '(' /*19N*/ error ')' statement_or_null %prec less_than_K_else /*17N*/
	| assert_or_assume K_property '(' /*19N*/ error ')' K_else /*18N*/ statement_or_null
	| assert_or_assume K_property '(' /*19N*/ error ')' statement_or_null K_else /*18N*/ statement_or_null
	| K_cover K_property '(' /*19N*/ error ')' statement_or_null
	| K_cover K_sequence '(' /*19N*/ error ')' statement_or_null
	| K_restrict K_property '(' /*19N*/ error ')' ';'
	;

constraint_block_item :
	constraint_expression
	;

constraint_block_item_list :
	constraint_block_item_list constraint_block_item
	| constraint_block_item
	;

constraint_block_item_list_opt :
	/*empty*/
	| constraint_block_item_list
	;

constraint_declaration :
	K_static_opt K_constraint IDENTIFIER '{' constraint_block_item_list_opt '}'
	| K_static_opt K_constraint IDENTIFIER '{' error '}'
	;

constraint_expression :
	expression ';'
	| expression K_dist '{' '}' ';'
	| expression constraint_trigger
	| K_if '(' /*19N*/ expression ')' constraint_set %prec less_than_K_else /*17N*/
	| K_if '(' /*19N*/ expression ')' constraint_set K_else /*18N*/ constraint_set
	| K_foreach '(' /*19N*/ IDENTIFIER '[' loop_variables ']' ')' constraint_set
	;

constraint_trigger :
	K_CONSTRAINT_IMPL '{' constraint_expression_list '}'
	;

constraint_expression_list :
	constraint_expression_list constraint_expression
	| constraint_expression
	;

constraint_prototype :
	K_static_opt K_constraint IDENTIFIER ';'
	;

constraint_set :
	constraint_expression
	| '{' constraint_expression_list '}'
	;

data_declaration :
	attribute_list_opt data_type list_of_variable_decl_assignments ';'
	| attribute_list_opt K_var data_type_or_implicit list_of_variable_decl_assignments ';'
	| attribute_list_opt K_event event_variable_list ';'
	| attribute_list_opt package_import_declaration
	;

package_scope :
	PACKAGE_IDENTIFIER K_SCOPE_RES
	;

ps_type_identifier :
	TYPE_IDENTIFIER
	| package_scope TYPE_IDENTIFIER
	;

packed_array_data_type :
	enum_data_type
	| struct_data_type
	| ps_type_identifier
	;

simple_packed_type :
	integer_vector_type unsigned_signed_opt dimensions_opt
	| atom_type signed_unsigned_opt
	| K_time unsigned_signed_opt
	;

data_type :
	simple_packed_type
	| non_integer_type
	| packed_array_data_type dimensions_opt
	| K_string
	;

data_type_opt :
	data_type
	| /*empty*/
	;

scalar_vector_opt :
	K_vectored
	| K_scalared
	| /*empty*/
	;

data_type_or_implicit :
	data_type_or_implicit_no_opt
	| /*empty*/
	;

data_type_or_implicit_no_opt :
	data_type
	| signing dimensions_opt
	| scalar_vector_opt dimensions
	;

data_type_or_implicit_or_void :
	data_type_or_implicit
	| K_void
	;

deferred_immediate_assertion_item :
	block_identifier_opt deferred_immediate_assertion_statement
	;

deferred_immediate_assertion_statement :
	assert_or_assume deferred_mode '(' /*19N*/ expression ')' statement_or_null %prec less_than_K_else /*17N*/
	| assert_or_assume deferred_mode '(' /*19N*/ expression ')' K_else /*18N*/ statement_or_null
	| assert_or_assume deferred_mode '(' /*19N*/ expression ')' statement_or_null K_else /*18N*/ statement_or_null
	| K_cover deferred_mode '(' /*19N*/ expression ')' statement_or_null
	| assert_or_assume deferred_mode '(' /*19N*/ error ')' statement_or_null %prec less_than_K_else /*17N*/
	| assert_or_assume deferred_mode '(' /*19N*/ error ')' K_else /*18N*/ statement_or_null
	| assert_or_assume deferred_mode '(' /*19N*/ error ')' statement_or_null K_else /*18N*/ statement_or_null
	| K_cover deferred_mode '(' /*19N*/ error ')' statement_or_null
	;

deferred_mode :
	'#' DEC_NUMBER
	| K_final
	;

description :
	module
	| udp_primitive
	| config_declaration
	| nature_declaration
	| package_declaration
	| discipline_declaration
	| package_item
	| KK_attribute '(' /*19N*/ IDENTIFIER ',' STRING ',' STRING ')'
	| ';'
	;

description_list :
	description
	| description_list description
	;

endnew_opt :
	':' /*4R*/ K_new
	| /*empty*/
	;

dynamic_array_new :
	K_new '[' expression ']'
	| K_new '[' expression ']' '(' /*19N*/ expression ')'
	;

for_step :
	lpvalue '=' expression
	| inc_or_dec_expression
	| compressed_statement
	;

for_step_opt :
	for_step
	| /*empty*/
	;

function_declaration :
	K_function lifetime_opt data_type_or_implicit_or_void IDENTIFIER ';' tf_item_list_opt statement_or_null_list_opt K_endfunction label_opt
	| K_function lifetime_opt data_type_or_implicit_or_void IDENTIFIER '(' /*19N*/ tf_port_list_opt ')' ';' block_item_decls_opt statement_or_null_list_opt K_endfunction label_opt
	| K_function lifetime_opt data_type_or_implicit_or_void IDENTIFIER error K_endfunction label_opt
	;

genvar_iteration :
	IDENTIFIER '=' expression
	| IDENTIFIER compressed_operator expression
	| IDENTIFIER K_INCR
	| IDENTIFIER K_DECR
	| K_INCR IDENTIFIER
	| K_DECR IDENTIFIER
	;

import_export :
	K_import
	| K_export
	;

implicit_class_handle :
	K_this '.'
	| K_super '.'
	| K_this '.' K_super '.'
	;

class_hierarchy_identifier :
	implicit_class_handle hierarchy_identifier
	;

inc_or_dec_expression :
	K_INCR lpvalue %prec UNARY_PREC /*16L*/
	| lpvalue K_INCR %prec UNARY_PREC /*16L*/
	| K_DECR lpvalue %prec UNARY_PREC /*16L*/
	| lpvalue K_DECR %prec UNARY_PREC /*16L*/
	;

inside_expression :
	expression K_inside /*4R*/ '{' open_range_list '}'
	;

integer_vector_type :
	K_reg
	| K_bit
	| K_logic
	| K_bool
	;

join_keyword :
	K_join
	| K_join_none
	| K_join_any
	;

jump_statement :
	K_break ';'
	| K_continue ';'
	| K_return ';'
	| K_return expression ';'
	;

lifetime :
	K_automatic
	| K_static
	;

lifetime_opt :
	lifetime
	| /*empty*/
	;

loop_statement :
	K_for '(' /*19N*/ lpvalue '=' expression ';' expression ';' for_step_opt ')' statement_or_null
	| K_for '(' /*19N*/ ';' expression ';' for_step_opt ')' statement_or_null
	| K_for '(' /*19N*/ K_var_opt data_type IDENTIFIER '=' expression ';' expression ';' for_step_opt ')' statement_or_null
	| K_forever statement_or_null
	| K_repeat '(' /*19N*/ expression ')' statement_or_null
	| K_while '(' /*19N*/ expression ')' statement_or_null
	| K_do statement_or_null K_while '(' /*19N*/ expression ')' ';'
	| K_foreach '(' /*19N*/ IDENTIFIER '[' loop_variables ']' ')' statement_or_null
	| K_for '(' /*19N*/ lpvalue '=' expression ';' expression ';' error ')' statement_or_null
	| K_for '(' /*19N*/ lpvalue '=' expression ';' error ';' for_step_opt ')' statement_or_null
	| K_for '(' /*19N*/ error ')' statement_or_null
	| K_while '(' /*19N*/ error ')' statement_or_null
	| K_do statement_or_null K_while '(' /*19N*/ error ')' ';'
	| K_foreach '(' /*19N*/ IDENTIFIER '[' error ']' ')' statement_or_null
	;

list_of_variable_decl_assignments :
	variable_decl_assignment
	| list_of_variable_decl_assignments ',' variable_decl_assignment
	;

initializer_opt :
	'=' expression
	| /*empty*/
	;

var_decl_initializer_opt :
	initializer_opt
	| '=' class_new
	| '=' dynamic_array_new
	;

variable_decl_assignment :
	IDENTIFIER dimensions_opt var_decl_initializer_opt
	;

loop_variables :
	loop_variables ',' IDENTIFIER
	| loop_variables ','
	| IDENTIFIER
	| /*empty*/
	;

method_qualifier :
	K_virtual
	| class_item_qualifier
	;

method_qualifier_opt :
	method_qualifier
	| /*empty*/
	;

modport_declaration :
	K_modport modport_item_list ';'
	;

modport_item_list :
	modport_item
	| modport_item_list ',' modport_item
	;

modport_item :
	IDENTIFIER '(' /*19N*/ modport_ports_list ')'
	;

modport_ports_list :
	modport_ports_declaration
	| modport_ports_list ',' modport_ports_declaration
	| modport_ports_list ',' modport_simple_port
	| modport_ports_list ',' modport_tf_port
	| modport_ports_list ',' IDENTIFIER
	| modport_ports_list ','
	;

modport_ports_declaration :
	attribute_list_opt port_direction IDENTIFIER
	| attribute_list_opt port_direction modport_simple_port
	| attribute_list_opt import_export IDENTIFIER
	| attribute_list_opt import_export modport_tf_port
	| attribute_list_opt K_clocking IDENTIFIER
	;

modport_simple_port :
	'.' IDENTIFIER '(' /*19N*/ expression ')'
	;

modport_tf_port :
	K_task IDENTIFIER tf_port_list_parens_opt
	| K_function data_type_or_implicit_or_void IDENTIFIER tf_port_list_parens_opt
	;

non_integer_type :
	K_real
	| K_realtime
	| K_shortreal
	;

number :
	BASED_NUMBER
	| DEC_NUMBER
	| DEC_NUMBER BASED_NUMBER
	| UNBASED_NUMBER
	| DEC_NUMBER UNBASED_NUMBER
	;

open_range_list :
	open_range_list ',' value_range
	| value_range
	;

package_declaration :
	K_package lifetime_opt IDENTIFIER ';' timeunits_declaration_opt package_item_list_opt K_endpackage label_opt
	;

module_package_import_list_opt :
	/*empty*/
	| package_import_list
	;

package_import_list :
	package_import_declaration
	| package_import_list package_import_declaration
	;

package_import_declaration :
	K_import package_import_item_list ';'
	;

package_import_item :
	package_scope IDENTIFIER
	| package_scope TYPE_IDENTIFIER
	| package_scope '*' /*14L*/
	;

package_import_item_list :
	package_import_item_list ',' package_import_item
	| package_import_item
	;

package_export_declaration :
	K_export package_export_item_list ';'
	| K_export '*' /*14L*/ K_SCOPE_RES '*' /*14L*/ ';'
	;

package_export_item :
	PACKAGE_IDENTIFIER K_SCOPE_RES IDENTIFIER
	| PACKAGE_IDENTIFIER K_SCOPE_RES TYPE_IDENTIFIER
	| PACKAGE_IDENTIFIER K_SCOPE_RES '*' /*14L*/
	;

package_export_item_list :
	package_export_item_list ',' package_export_item
	| package_export_item
	;

package_item :
	timeunits_declaration
	| parameter_declaration
	| type_declaration
	| function_declaration
	| task_declaration
	| data_declaration
	| class_declaration
	| package_export_declaration
	;

package_item_list :
	package_item_list package_item
	| package_item
	;

package_item_list_opt :
	package_item_list
	| /*empty*/
	;

port_direction :
	K_input
	| K_output
	| K_inout
	| K_ref
	;

port_direction_opt :
	port_direction
	| /*empty*/
	;

procedural_assertion_statement :
	block_identifier_opt concurrent_assertion_statement
	| block_identifier_opt simple_immediate_assertion_statement
	| block_identifier_opt deferred_immediate_assertion_statement
	;

property_expr :
	expression
	;

property_qualifier :
	class_item_qualifier
	| random_qualifier
	;

property_qualifier_opt :
	property_qualifier_list
	| /*empty*/
	;

property_qualifier_list :
	property_qualifier_list property_qualifier
	| property_qualifier
	;

property_spec :
	clocking_event_opt property_spec_disable_iff_opt property_expr
	;

property_spec_disable_iff_opt :
	K_disable K_iff '(' /*19N*/ expression ')'
	| /*empty*/
	;

random_qualifier :
	K_rand
	| K_randc
	;

signing :
	K_signed
	| K_unsigned
	;

simple_immediate_assertion_statement :
	assert_or_assume '(' /*19N*/ expression ')' statement_or_null %prec less_than_K_else /*17N*/
	| assert_or_assume '(' /*19N*/ expression ')' K_else /*18N*/ statement_or_null
	| assert_or_assume '(' /*19N*/ expression ')' statement_or_null K_else /*18N*/ statement_or_null
	| K_cover '(' /*19N*/ expression ')' statement_or_null
	| assert_or_assume '(' /*19N*/ error ')' statement_or_null %prec less_than_K_else /*17N*/
	| assert_or_assume '(' /*19N*/ error ')' K_else /*18N*/ statement_or_null
	| assert_or_assume '(' /*19N*/ error ')' statement_or_null K_else /*18N*/ statement_or_null
	| K_cover '(' /*19N*/ error ')' statement_or_null
	;

simple_type_or_string :
	integer_vector_type
	| non_integer_type
	| atom_type
	| K_time
	| K_string
	| ps_type_identifier
	;

statement :
	attribute_list_opt statement_item
	;

statement_or_null :
	statement
	| attribute_list_opt ';'
	;

stream_expression :
	expression
	;

stream_expression_list :
	stream_expression_list ',' stream_expression
	| stream_expression
	;

stream_operator :
	K_LS /*12L*/
	| K_RS /*12L*/
	;

streaming_concatenation :
	'{' stream_operator '{' stream_expression_list '}' '}'
	;

task_declaration :
	K_task lifetime_opt IDENTIFIER ';' tf_item_list_opt statement_or_null_list_opt K_endtask label_opt
	| K_task lifetime_opt IDENTIFIER '(' /*19N*/ tf_port_list_opt ')' ';' block_item_decls_opt statement_or_null_list_opt K_endtask label_opt
	| K_task lifetime_opt IDENTIFIER error K_endtask label_opt
	;

tf_port_declaration :
	port_direction K_var_opt data_type_or_implicit list_of_port_identifiers ';'
	;

tf_port_item :
	port_direction_opt K_var_opt data_type_or_implicit IDENTIFIER dimensions_opt initializer_opt
	| port_direction_opt K_var_opt data_type_or_implicit IDENTIFIER error
	;

tf_port_list :
	tf_port_item_list
	;

tf_port_item_list :
	tf_port_item_list ',' tf_port_item
	| tf_port_item
	| error ',' tf_port_item
	| tf_port_item_list ','
	| tf_port_item_list ';'
	;

timeunits_declaration :
	K_timeunit /*23N*/ TIME_LITERAL ';'
	| K_timeunit /*23N*/ TIME_LITERAL '/' /*14L*/ TIME_LITERAL ';'
	| K_timeprecision /*23N*/ TIME_LITERAL ';'
	;

timeunits_declaration_opt :
	%prec no_timeunits_declaration /*21N*/ /*empty*/
	| timeunits_declaration %prec one_timeunits_declaration /*22N*/
	| timeunits_declaration timeunits_declaration
	;

value_range :
	expression
	| '[' expression ':' /*4R*/ expression ']'
	;

variable_dimension :
	'[' expression ':' /*4R*/ expression ']'
	| '[' expression ']'
	| '[' ']'
	| '[' '$' ']'
	| '[' '$' ':' /*4R*/ expression ']'
	;

variable_lifetime_opt :
	lifetime
	| /*empty*/
	;

attribute_list_opt :
	attribute_instance_list
	| /*empty*/
	;

attribute_instance_list :
	K_PSTAR K_STARP
	| K_PSTAR attribute_list K_STARP
	| attribute_instance_list K_PSTAR K_STARP
	| attribute_instance_list K_PSTAR attribute_list K_STARP
	;

attribute_list :
	attribute_list ',' attribute
	| attribute
	;

attribute :
	IDENTIFIER initializer_opt
	;

block_item_decl :
	K_var variable_lifetime_opt data_type_or_implicit list_of_variable_decl_assignments ';'
	| variable_lifetime_opt data_type list_of_variable_decl_assignments ';'
	| variable_lifetime_opt K_reg data_type list_of_variable_decl_assignments ';'
	| K_event event_variable_list ';'
	| parameter_declaration
	| type_declaration
	| package_import_declaration
	| K_var variable_lifetime_opt data_type_or_implicit error ';'
	| variable_lifetime_opt data_type error ';'
	| K_event error ';'
	| parameter error ';'
	| localparam error ';'
	;

block_item_decls :
	block_item_decl
	| block_item_decls block_item_decl
	;

block_item_decls_opt :
	block_item_decls
	| /*empty*/
	;

typedef_basic_type :
	K_struct
	| K_union
	| K_class
	;

type_declaration :
	K_typedef data_type identifier_name dimensions_opt ';'
	| K_typedef identifier_name ';'
	| K_typedef typedef_basic_type identifier_name ';'
	| K_typedef K_enum identifier_name ';'
	| K_typedef error ';'
	;

enum_base_type :
	simple_packed_type
	| ps_type_identifier dimensions_opt
	| /*empty*/
	;

enum_data_type :
	K_enum enum_base_type '{' enum_name_list '}'
	;

enum_name_list :
	enum_name
	| enum_name_list ',' enum_name
	;

pos_neg_number :
	number
	| '-' /*13L*/ number
	;

enum_name :
	IDENTIFIER initializer_opt
	| IDENTIFIER '[' pos_neg_number ']' initializer_opt
	| IDENTIFIER '[' pos_neg_number ':' /*4R*/ pos_neg_number ']' initializer_opt
	;

packed_signing :
	K_packed unsigned_signed_opt
	| /*empty*/
	;

struct_data_type :
	K_struct packed_signing '{' struct_union_member_list '}'
	| K_union packed_signing '{' struct_union_member_list '}'
	| K_struct packed_signing '{' error '}'
	| K_union packed_signing '{' error '}'
	;

struct_union_member_list :
	struct_union_member_list struct_union_member
	| struct_union_member
	;

struct_union_member :
	attribute_list_opt data_type list_of_variable_decl_assignments ';'
	| error ';'
	;

case_item :
	expression_list_proper ':' /*4R*/ statement_or_null
	| K_default ':' /*4R*/ statement_or_null
	| K_default statement_or_null
	| error ':' /*4R*/ statement_or_null
	;

case_items :
	case_items case_item
	| case_item
	;

charge_strength :
	'(' /*19N*/ K_small ')'
	| '(' /*19N*/ K_medium ')'
	| '(' /*19N*/ K_large ')'
	;

charge_strength_opt :
	charge_strength
	| /*empty*/
	;

defparam_assign :
	hierarchy_identifier '=' expression
	;

defparam_assign_list :
	defparam_assign
	| dimensions defparam_assign
	| defparam_assign_list ',' defparam_assign
	;

delay1 :
	'#' delay_value_simple
	| '#' '(' /*19N*/ delay_value ')'
	;

delay3 :
	'#' delay_value_simple
	| '#' '(' /*19N*/ delay_value ')'
	| '#' '(' /*19N*/ delay_value ',' delay_value ')'
	| '#' '(' /*19N*/ delay_value ',' delay_value ',' delay_value ')'
	;

delay3_opt :
	delay3
	| /*empty*/
	;

delay_value_list :
	delay_value
	| delay_value_list ',' delay_value
	;

delay_value :
	expression
	| expression ':' /*4R*/ expression ':' /*4R*/ expression
	;

delay_value_simple :
	DEC_NUMBER
	| REALTIME
	| IDENTIFIER
	| TIME_LITERAL
	;

optional_semicolon :
	';'
	| /*empty*/
	;

discipline_declaration :
	K_discipline IDENTIFIER optional_semicolon discipline_items K_enddiscipline
	;

discipline_items :
	discipline_items discipline_item
	| discipline_item
	;

discipline_item :
	K_domain K_discrete ';'
	| K_domain K_continuous ';'
	| K_potential IDENTIFIER ';'
	| K_flow IDENTIFIER ';'
	;

nature_declaration :
	K_nature IDENTIFIER optional_semicolon nature_items K_endnature
	;

nature_items :
	nature_items nature_item
	| nature_item
	;

nature_item :
	K_units '=' STRING ';'
	| K_abstol '=' expression ';'
	| K_access '=' IDENTIFIER ';'
	| K_idt_nature '=' IDENTIFIER ';'
	| K_ddt_nature '=' IDENTIFIER ';'
	;

config_declaration :
	K_config IDENTIFIER ';' K_design lib_cell_identifiers ';' list_of_config_rule_statements K_endconfig
	;

lib_cell_identifiers :
	/*empty*/
	| lib_cell_identifiers lib_cell_id
	;

list_of_config_rule_statements :
	/*empty*/
	| list_of_config_rule_statements config_rule_statement
	;

config_rule_statement :
	K_default K_liblist list_of_libraries ';'
	| K_instance hierarchy_identifier K_liblist list_of_libraries ';'
	| K_instance hierarchy_identifier K_use lib_cell_id opt_config ';'
	| K_cell lib_cell_id K_liblist list_of_libraries ';'
	| K_cell lib_cell_id K_use lib_cell_id opt_config ';'
	;

opt_config :
	/*empty*/
	| ':' /*4R*/ K_config
	;

lib_cell_id :
	IDENTIFIER
	| IDENTIFIER '.' IDENTIFIER
	;

list_of_libraries :
	/*empty*/
	| list_of_libraries IDENTIFIER
	;

drive_strength :
	'(' /*19N*/ dr_strength0 ',' dr_strength1 ')'
	| '(' /*19N*/ dr_strength1 ',' dr_strength0 ')'
	| '(' /*19N*/ dr_strength0 ',' K_highz1 ')'
	| '(' /*19N*/ dr_strength1 ',' K_highz0 ')'
	| '(' /*19N*/ K_highz1 ',' dr_strength0 ')'
	| '(' /*19N*/ K_highz0 ',' dr_strength1 ')'
	;

drive_strength_opt :
	drive_strength
	| /*empty*/
	;

dr_strength0 :
	K_supply0
	| K_strong0
	| K_pull0
	| K_weak0
	;

dr_strength1 :
	K_supply1
	| K_strong1
	| K_pull1
	| K_weak1
	;

clocking_event_opt :
	event_control
	| /*empty*/
	;

event_control :
	'@' hierarchy_identifier
	| '@' '(' /*19N*/ event_expression_list ')'
	| '@' '(' /*19N*/ error ')'
	;

event_expression_list :
	event_expression
	| event_expression_list K_or event_expression
	| event_expression_list ',' event_expression
	;

event_expression :
	K_posedge expression
	| K_negedge expression
	| K_edge expression
	| expression
	;

branch_probe_expression :
	IDENTIFIER '(' /*19N*/ IDENTIFIER ',' IDENTIFIER ')'
	| IDENTIFIER '(' /*19N*/ IDENTIFIER ')'
	;

expression :
	expr_primary_or_typename
	| inc_or_dec_expression
	| inside_expression
	| '+' /*13L*/ attribute_list_opt expr_primary %prec UNARY_PREC /*16L*/
	| '-' /*13L*/ attribute_list_opt expr_primary %prec UNARY_PREC /*16L*/
	| '~' attribute_list_opt expr_primary %prec UNARY_PREC /*16L*/
	| '&' /*9L*/ attribute_list_opt expr_primary %prec UNARY_PREC /*16L*/
	| '!' attribute_list_opt expr_primary %prec UNARY_PREC /*16L*/
	| '|' /*7L*/ attribute_list_opt expr_primary %prec UNARY_PREC /*16L*/
	| '^' /*8L*/ attribute_list_opt expr_primary %prec UNARY_PREC /*16L*/
	| '~' '&' /*9L*/ attribute_list_opt expr_primary %prec UNARY_PREC /*16L*/
	| '~' '|' /*7L*/ attribute_list_opt expr_primary %prec UNARY_PREC /*16L*/
	| '~' '^' /*8L*/ attribute_list_opt expr_primary %prec UNARY_PREC /*16L*/
	| K_NAND /*9L*/ attribute_list_opt expr_primary %prec UNARY_PREC /*16L*/
	| K_NOR /*8L*/ attribute_list_opt expr_primary %prec UNARY_PREC /*16L*/
	| K_NXOR /*8L*/ attribute_list_opt expr_primary %prec UNARY_PREC /*16L*/
	| '!' error %prec UNARY_PREC /*16L*/
	| '^' /*8L*/ error %prec UNARY_PREC /*16L*/
	| expression '^' /*8L*/ attribute_list_opt expression
	| expression K_POW /*15L*/ attribute_list_opt expression
	| expression '*' /*14L*/ attribute_list_opt expression
	| expression '/' /*14L*/ attribute_list_opt expression
	| expression '%' /*14L*/ attribute_list_opt expression
	| expression '+' /*13L*/ attribute_list_opt expression
	| expression '-' /*13L*/ attribute_list_opt expression
	| expression '&' /*9L*/ attribute_list_opt expression
	| expression '|' /*7L*/ attribute_list_opt expression
	| expression K_NAND /*9L*/ attribute_list_opt expression
	| expression K_NOR /*8L*/ attribute_list_opt expression
	| expression K_NXOR /*8L*/ attribute_list_opt expression
	| expression '<' /*11L*/ attribute_list_opt expression
	| expression '>' /*11L*/ attribute_list_opt expression
	| expression K_LS /*12L*/ attribute_list_opt expression
	| expression K_RS /*12L*/ attribute_list_opt expression
	| expression K_RSS /*12L*/ attribute_list_opt expression
	| expression K_EQ /*10L*/ attribute_list_opt expression
	| expression K_CEQ /*10L*/ attribute_list_opt expression
	| expression K_WEQ /*10L*/ attribute_list_opt expression
	| expression K_LE /*11L*/ attribute_list_opt expression
	| expression K_GE /*11L*/ attribute_list_opt expression
	| expression K_NE /*10L*/ attribute_list_opt expression
	| expression K_CNE /*10L*/ attribute_list_opt expression
	| expression K_WNE /*10L*/ attribute_list_opt expression
	| expression K_LOR /*5L*/ attribute_list_opt expression
	| expression K_LAND /*6L*/ attribute_list_opt expression
	| expression K_TRIGGER /*3R*/ attribute_list_opt expression
	| expression K_LEQUIV /*3R*/ attribute_list_opt expression
	| expression '?' /*4R*/ attribute_list_opt expression ':' /*4R*/ expression
	;

expr_mintypmax :
	expression
	| expression ':' /*4R*/ expression ':' /*4R*/ expression
	;

expression_list_with_nuls :
	expression_list_with_nuls ',' expression
	| expression
	| /*empty*/
	| expression_list_with_nuls ','
	;

argument_list_parens :
	'(' /*19N*/ expression_list_with_nuls ')'
	;

argument_list_parens_opt :
	argument_list_parens
	| /*empty*/
	;

expression_list_proper :
	expression_list_proper ',' expression
	| expression
	;

expr_primary_or_typename :
	expr_primary
	| data_type
	;

expr_primary :
	number
	| REALTIME
	| STRING
	| TIME_LITERAL
	| SYSTEM_IDENTIFIER
	| hierarchy_identifier
	| hierarchy_identifier '.' K_and
	| hierarchy_identifier '.' K_or
	| hierarchy_identifier '.' K_unique
	| hierarchy_identifier '.' K_xor
	| package_scope hierarchy_identifier
	| hierarchy_identifier attribute_list_opt argument_list_parens
	| class_hierarchy_identifier argument_list_parens
	| SYSTEM_IDENTIFIER '(' /*19N*/ expression_list_proper ')'
	| package_scope hierarchy_identifier argument_list_parens
	| SYSTEM_IDENTIFIER '(' /*19N*/ ')'
	| K_this
	| class_hierarchy_identifier
	| K_acos '(' /*19N*/ expression ')'
	| K_acosh '(' /*19N*/ expression ')'
	| K_asin '(' /*19N*/ expression ')'
	| K_asinh '(' /*19N*/ expression ')'
	| K_atan '(' /*19N*/ expression ')'
	| K_atanh '(' /*19N*/ expression ')'
	| K_atan2 '(' /*19N*/ expression ',' expression ')'
	| K_ceil '(' /*19N*/ expression ')'
	| K_cos '(' /*19N*/ expression ')'
	| K_cosh '(' /*19N*/ expression ')'
	| K_exp '(' /*19N*/ expression ')'
	| K_floor '(' /*19N*/ expression ')'
	| K_hypot '(' /*19N*/ expression ',' expression ')'
	| K_ln '(' /*19N*/ expression ')'
	| K_log '(' /*19N*/ expression ')'
	| K_pow '(' /*19N*/ expression ',' expression ')'
	| K_sin '(' /*19N*/ expression ')'
	| K_sinh '(' /*19N*/ expression ')'
	| K_sqrt '(' /*19N*/ expression ')'
	| K_tan '(' /*19N*/ expression ')'
	| K_tanh '(' /*19N*/ expression ')'
	| K_abs '(' /*19N*/ expression ')'
	| K_max '(' /*19N*/ expression ',' expression ')'
	| K_min '(' /*19N*/ expression ',' expression ')'
	| '(' /*19N*/ expr_mintypmax ')'
	| '{' expression_list_proper '}'
	| '{' expression '{' expression_list_proper '}' '}'
	| '{' expression '{' expression_list_proper '}' error '}'
	| '{' '}'
	| expr_primary '\'' '(' /*19N*/ expression ')'
	| simple_type_or_string '\'' '(' /*19N*/ expression ')'
	| signing '\'' '(' /*19N*/ expression ')'
	| assignment_pattern
	| streaming_concatenation
	| K_null
	;

tf_item_list_opt :
	tf_item_list
	| /*empty*/
	;

tf_item_list :
	tf_item_declaration
	| tf_item_list tf_item_declaration
	;

tf_item_declaration :
	tf_port_declaration
	| block_item_decl
	;

gate_instance :
	IDENTIFIER '(' /*19N*/ port_conn_expression_list_with_nuls ')'
	| IDENTIFIER dimensions '(' /*19N*/ port_conn_expression_list_with_nuls ')'
	| '(' /*19N*/ port_conn_expression_list_with_nuls ')'
	| IDENTIFIER dimensions
	| IDENTIFIER '(' /*19N*/ port_name_list ')'
	| IDENTIFIER dimensions '(' /*19N*/ port_name_list ')'
	| IDENTIFIER '(' /*19N*/ error ')'
	| IDENTIFIER dimensions '(' /*19N*/ error ')'
	;

gate_instance_list :
	gate_instance_list ',' gate_instance
	| gate_instance
	;

gatetype :
	K_and
	| K_nand
	| K_or
	| K_nor
	| K_xor
	| K_xnor
	| K_buf
	| K_bufif0
	| K_bufif1
	| K_not
	| K_notif0
	| K_notif1
	;

switchtype :
	K_nmos
	| K_rnmos
	| K_pmos
	| K_rpmos
	| K_cmos
	| K_rcmos
	| K_tran
	| K_rtran
	| K_tranif0
	| K_tranif1
	| K_rtranif0
	| K_rtranif1
	;

hierarchy_identifier :
	IDENTIFIER
	| hierarchy_identifier '.' IDENTIFIER
	| hierarchy_identifier '[' expression ']'
	| hierarchy_identifier '[' '$' ']'
	| hierarchy_identifier '[' expression ':' /*4R*/ expression ']'
	| hierarchy_identifier '[' expression K_PO_POS expression ']'
	| hierarchy_identifier '[' expression K_PO_NEG expression ']'
	;

list_of_identifiers :
	IDENTIFIER
	| list_of_identifiers ',' IDENTIFIER
	;

list_of_port_identifiers :
	IDENTIFIER dimensions_opt
	| list_of_port_identifiers ',' IDENTIFIER dimensions_opt
	;

list_of_variable_port_identifiers :
	IDENTIFIER dimensions_opt initializer_opt
	| list_of_variable_port_identifiers ',' IDENTIFIER dimensions_opt initializer_opt
	;

list_of_ports :
	port_opt
	| list_of_ports ',' port_opt
	;

list_of_port_declarations :
	port_declaration
	| list_of_port_declarations ',' port_declaration
	| list_of_port_declarations ',' attribute_list_opt IDENTIFIER dimensions_opt initializer_opt
	| list_of_port_declarations ','
	| list_of_port_declarations ';'
	;

port_declaration :
	attribute_list_opt port_direction net_type_or_var_opt data_type_or_implicit IDENTIFIER dimensions_opt initializer_opt
	| attribute_list_opt net_type_or_var data_type_or_implicit IDENTIFIER dimensions_opt initializer_opt
	| attribute_list_opt data_type_or_implicit_no_opt IDENTIFIER dimensions_opt initializer_opt
	| attribute_list_opt port_direction K_wreal IDENTIFIER
	;

unsigned_signed_opt :
	K_signed
	| K_unsigned
	| /*empty*/
	;

signed_unsigned_opt :
	K_signed
	| K_unsigned
	| /*empty*/
	;

atom_type :
	K_byte
	| K_shortint
	| K_int
	| K_longint
	| K_integer
	;

lpvalue :
	hierarchy_identifier
	| class_hierarchy_identifier
	| '{' expression_list_proper '}'
	| streaming_concatenation
	;

cont_assign :
	lpvalue '=' expression
	;

cont_assign_list :
	cont_assign_list ',' cont_assign
	| cont_assign
	;

module :
	attribute_list_opt module_start lifetime_opt IDENTIFIER module_package_import_list_opt module_parameter_port_list_opt module_port_list_opt module_attribute_foreign ';' timeunits_declaration_opt module_item_list_opt module_end label_opt
	;

module_start :
	K_module
	| K_macromodule
	| K_program
	| K_interface
	;

module_end :
	K_endmodule
	| K_endprogram
	| K_endinterface
	;

label_opt :
	':' /*4R*/ IDENTIFIER
	| /*empty*/
	;

module_attribute_foreign :
	K_PSTAR IDENTIFIER K_integer IDENTIFIER '=' STRING ';' K_STARP
	| /*empty*/
	;

module_port_list_opt :
	'(' /*19N*/ list_of_ports ')'
	| '(' /*19N*/ list_of_port_declarations ')'
	| /*empty*/
	| '(' /*19N*/ error ')'
	;

module_parameter_port_list_opt :
	/*empty*/
	| '#' '(' /*19N*/ module_parameter_port_list ')'
	;

type_param :
	K_type
	;

module_parameter :
	parameter param_type parameter_assign
	| localparam param_type parameter_assign
	;

module_parameter_port_list :
	module_parameter
	| data_type_opt parameter_assign
	| type_param parameter_assign
	| module_parameter_port_list ',' module_parameter
	| module_parameter_port_list ',' data_type_opt parameter_assign
	| module_parameter_port_list ',' type_param parameter_assign
	;

module_item :
	module
	| attribute_list_opt net_type data_type_or_implicit delay3_opt net_variable_list ';'
	| attribute_list_opt K_wreal delay3 net_variable_list ';'
	| attribute_list_opt K_wreal net_variable_list ';'
	| attribute_list_opt net_type data_type_or_implicit delay3_opt net_decl_assigns ';'
	| attribute_list_opt net_type data_type_or_implicit drive_strength net_decl_assigns ';'
	| attribute_list_opt K_wreal net_decl_assigns ';'
	| K_trireg charge_strength_opt dimensions_opt delay3_opt list_of_identifiers ';'
	| attribute_list_opt port_direction net_type_or_var data_type_or_implicit list_of_port_identifiers ';'
	| attribute_list_opt port_direction K_wreal list_of_port_identifiers ';'
	| attribute_list_opt K_inout data_type_or_implicit list_of_port_identifiers ';'
	| attribute_list_opt K_input data_type_or_implicit list_of_port_identifiers ';'
	| attribute_list_opt K_output data_type_or_implicit list_of_variable_port_identifiers ';'
	| attribute_list_opt port_direction net_type_or_var data_type_or_implicit error ';'
	| attribute_list_opt K_inout data_type_or_implicit error ';'
	| attribute_list_opt K_input data_type_or_implicit error ';'
	| attribute_list_opt K_output data_type_or_implicit error ';'
	| K_let IDENTIFIER let_port_list_opt '=' expression ';'
	| DISCIPLINE_IDENTIFIER list_of_identifiers ';'
	| attribute_list_opt block_item_decl
	| K_defparam defparam_assign_list ';'
	| attribute_list_opt gatetype gate_instance_list ';'
	| attribute_list_opt gatetype delay3 gate_instance_list ';'
	| attribute_list_opt gatetype drive_strength gate_instance_list ';'
	| attribute_list_opt gatetype drive_strength delay3 gate_instance_list ';'
	| attribute_list_opt switchtype gate_instance_list ';'
	| attribute_list_opt switchtype delay3 gate_instance_list ';'
	| K_pullup gate_instance_list ';'
	| K_pulldown gate_instance_list ';'
	| K_pullup '(' /*19N*/ dr_strength1 ')' gate_instance_list ';'
	| K_pullup '(' /*19N*/ dr_strength1 ',' dr_strength0 ')' gate_instance_list ';'
	| K_pullup '(' /*19N*/ dr_strength0 ',' dr_strength1 ')' gate_instance_list ';'
	| K_pulldown '(' /*19N*/ dr_strength0 ')' gate_instance_list ';'
	| K_pulldown '(' /*19N*/ dr_strength1 ',' dr_strength0 ')' gate_instance_list ';'
	| K_pulldown '(' /*19N*/ dr_strength0 ',' dr_strength1 ')' gate_instance_list ';'
	| attribute_list_opt IDENTIFIER parameter_value_opt gate_instance_list ';'
	| attribute_list_opt IDENTIFIER parameter_value_opt error ';'
	| K_assign drive_strength_opt delay3_opt cont_assign_list ';'
	| attribute_list_opt K_always statement_item
	| attribute_list_opt K_always_comb statement_item
	| attribute_list_opt K_always_ff statement_item
	| attribute_list_opt K_always_latch statement_item
	| attribute_list_opt K_initial statement_item
	| attribute_list_opt K_final statement_item
	| attribute_list_opt K_analog analog_statement
	| attribute_list_opt assertion_item
	| timeunits_declaration
	| class_declaration
	| task_declaration
	| function_declaration
	| K_generate generate_item_list_opt K_endgenerate
	| K_genvar list_of_identifiers ';'
	| K_for '(' /*19N*/ K_genvar_opt IDENTIFIER '=' expression ';' expression ';' genvar_iteration ')' generate_block
	| generate_if generate_block K_else /*18N*/ generate_block
	| generate_if generate_block %prec less_than_K_else /*17N*/
	| K_case '(' /*19N*/ expression ')' generate_case_items K_endcase
	| SYSTEM_IDENTIFIER argument_list_parens_opt ';'
	| modport_declaration
	| attribute_list_opt K_specparam specparam_decl ';'
	| K_specify specify_item_list_opt K_endspecify
	| K_specify error K_endspecify
	| error ';'
	| K_assign error '=' expression ';'
	| K_assign error ';'
	| K_function error K_endfunction label_opt
	| KK_attribute '(' /*19N*/ IDENTIFIER ',' STRING ',' STRING ')' ';'
	| KK_attribute '(' /*19N*/ error ')' ';'
	| ';'
	;

let_port_list_opt :
	'(' /*19N*/ let_port_list ')'
	| '(' /*19N*/ ')'
	| /*empty*/
	;

let_port_list :
	let_port_item
	| let_port_list ',' let_port_item
	;

let_port_item :
	attribute_list_opt let_formal_type IDENTIFIER dimensions_opt initializer_opt
	;

let_formal_type :
	data_type_or_implicit
	| K_untyped
	;

module_item_list :
	module_item_list module_item
	| module_item
	;

module_item_list_opt :
	module_item_list
	| /*empty*/
	;

generate_if :
	K_if '(' /*19N*/ expression ')'
	;

generate_case_items :
	generate_case_items generate_case_item
	| generate_case_item
	;

generate_case_item :
	expression_list_proper ':' /*4R*/ generate_block
	| K_default ':' /*4R*/ generate_block
	;

generate_item :
	module_item
	| K_begin generate_item_list_opt K_end
	| K_begin ':' /*4R*/ IDENTIFIER generate_item_list_opt K_end
	;

generate_item_list :
	generate_item_list generate_item
	| generate_item
	;

generate_item_list_opt :
	generate_item_list
	| /*empty*/
	;

generate_block :
	module_item
	| K_begin label_opt generate_item_list_opt K_end label_opt
	;

net_decl_assign :
	IDENTIFIER '=' expression
	;

net_decl_assigns :
	net_decl_assigns ',' net_decl_assign
	| net_decl_assign
	;

net_type :
	K_wire
	| K_tri
	| K_tri1
	| K_supply0
	| K_wand
	| K_triand
	| K_tri0
	| K_supply1
	| K_wor
	| K_trior
	| K_wone
	| K_uwire
	;

net_type_opt :
	net_type
	| /*empty*/
	;

net_type_or_var :
	net_type
	| K_var
	;

net_type_or_var_opt :
	net_type_opt
	| K_var
	;

param_type :
	data_type_or_implicit
	| type_param
	;

parameter :
	K_parameter
	;

localparam :
	K_localparam
	;

parameter_declaration :
	parameter_or_localparam param_type parameter_assign_list ';'
	;

parameter_or_localparam :
	parameter
	| localparam
	;

parameter_assign_list :
	parameter_assign
	| parameter_assign_list ',' parameter_assign
	;

parameter_assign :
	IDENTIFIER initializer_opt parameter_value_ranges_opt
	;

parameter_value_ranges_opt :
	parameter_value_ranges
	| /*empty*/
	;

parameter_value_ranges :
	parameter_value_ranges parameter_value_range
	| parameter_value_range
	;

parameter_value_range :
	from_exclude '[' value_range_expression ':' /*4R*/ value_range_expression ']'
	| from_exclude '[' value_range_expression ':' /*4R*/ value_range_expression ')'
	| from_exclude '(' /*19N*/ value_range_expression ':' /*4R*/ value_range_expression ']'
	| from_exclude '(' /*19N*/ value_range_expression ':' /*4R*/ value_range_expression ')'
	| K_exclude /*20N*/ expression
	;

value_range_expression :
	expression
	| K_inf
	| '+' /*13L*/ K_inf
	| '-' /*13L*/ K_inf
	;

from_exclude :
	K_from
	| K_exclude /*20N*/
	;

parameter_value_opt :
	'#' '(' /*19N*/ expression_list_with_nuls ')'
	| '#' '(' /*19N*/ parameter_value_byname_list ')'
	| '#' DEC_NUMBER
	| '#' REALTIME
	| '#' error
	| /*empty*/
	;

parameter_value_byname :
	'.' IDENTIFIER '(' /*19N*/ expression ')'
	| '.' IDENTIFIER '(' /*19N*/ ')'
	;

parameter_value_byname_list :
	parameter_value_byname
	| parameter_value_byname_list ',' parameter_value_byname
	;

port :
	port_reference
	| '.' IDENTIFIER '(' /*19N*/ port_reference ')'
	| '{' port_reference_list '}'
	| '.' IDENTIFIER '(' /*19N*/ '{' port_reference_list '}' ')'
	;

port_opt :
	port
	| /*empty*/
	;

port_name :
	attribute_list_opt '.' IDENTIFIER '(' /*19N*/ expression ')'
	| attribute_list_opt '.' IDENTIFIER '(' /*19N*/ error ')'
	| attribute_list_opt '.' IDENTIFIER '(' /*19N*/ ')'
	| attribute_list_opt '.' IDENTIFIER
	| K_DOTSTAR
	;

port_name_list :
	port_name_list ',' port_name
	| port_name
	;

port_conn_expression_list_with_nuls :
	port_conn_expression_list_with_nuls ',' attribute_list_opt expression
	| attribute_list_opt expression
	| /*empty*/
	| port_conn_expression_list_with_nuls ','
	;

port_reference :
	IDENTIFIER
	| IDENTIFIER '[' expression ':' /*4R*/ expression ']'
	| IDENTIFIER '[' expression ']'
	| IDENTIFIER '[' error ']'
	;

port_reference_list :
	port_reference
	| port_reference_list ',' port_reference
	;

dimensions_opt :
	/*empty*/
	| dimensions
	;

dimensions :
	variable_dimension
	| dimensions variable_dimension
	;

net_variable :
	IDENTIFIER dimensions_opt
	;

net_variable_list :
	net_variable
	| net_variable_list ',' net_variable
	;

event_variable :
	IDENTIFIER dimensions_opt
	;

event_variable_list :
	event_variable
	| event_variable_list ',' event_variable
	;

specify_item :
	K_specparam specparam_decl ';'
	| specify_simple_path_decl ';'
	| specify_edge_path_decl ';'
	| K_if '(' /*19N*/ expression ')' specify_simple_path_decl ';'
	| K_if '(' /*19N*/ expression ')' specify_edge_path_decl ';'
	| K_ifnone specify_simple_path_decl ';'
	| K_ifnone specify_edge_path_decl ';'
	| K_Sfullskew '(' /*19N*/ spec_reference_event ',' spec_reference_event ',' delay_value ',' delay_value fullskew_opt_args ')' ';'
	| K_Shold '(' /*19N*/ spec_reference_event ',' spec_reference_event ',' delay_value spec_notifier_opt ')' ';'
	| K_Snochange '(' /*19N*/ spec_reference_event ',' spec_reference_event ',' delay_value ',' delay_value spec_notifier_opt ')' ';'
	| K_Speriod '(' /*19N*/ spec_reference_event ',' delay_value spec_notifier_opt ')' ';'
	| K_Srecovery '(' /*19N*/ spec_reference_event ',' spec_reference_event ',' delay_value spec_notifier_opt ')' ';'
	| K_Srecrem '(' /*19N*/ spec_reference_event ',' spec_reference_event ',' expr_mintypmax ',' expr_mintypmax recrem_opt_args ')' ';'
	| K_Sremoval '(' /*19N*/ spec_reference_event ',' spec_reference_event ',' delay_value spec_notifier_opt ')' ';'
	| K_Ssetup '(' /*19N*/ spec_reference_event ',' spec_reference_event ',' delay_value spec_notifier_opt ')' ';'
	| K_Ssetuphold '(' /*19N*/ spec_reference_event ',' spec_reference_event ',' expr_mintypmax ',' expr_mintypmax setuphold_opt_args ')' ';'
	| K_Sskew '(' /*19N*/ spec_reference_event ',' spec_reference_event ',' delay_value spec_notifier_opt ')' ';'
	| K_Stimeskew '(' /*19N*/ spec_reference_event ',' spec_reference_event ',' delay_value timeskew_opt_args ')' ';'
	| K_Swidth '(' /*19N*/ spec_reference_event ',' delay_value ',' expression spec_notifier_opt ')' ';'
	| K_Swidth '(' /*19N*/ spec_reference_event ',' delay_value ')' ';'
	| K_pulsestyle_onevent specify_path_identifiers ';'
	| K_pulsestyle_ondetect specify_path_identifiers ';'
	| K_showcancelled specify_path_identifiers ';'
	| K_noshowcancelled specify_path_identifiers ';'
	;

specify_item_list :
	specify_item
	| specify_item_list specify_item
	;

specify_item_list_opt :
	/*empty*/
	| specify_item_list
	;

specify_edge_path_decl :
	specify_edge_path '=' '(' /*19N*/ delay_value_list ')'
	| specify_edge_path '=' delay_value_simple
	;

edge_operator :
	K_posedge
	| K_negedge
	;

specify_edge_path :
	'(' /*19N*/ specify_path_identifiers spec_polarity K_EG '(' /*19N*/ specify_path_identifiers polarity_operator expression ')' ')'
	| '(' /*19N*/ edge_operator specify_path_identifiers spec_polarity K_EG '(' /*19N*/ specify_path_identifiers polarity_operator expression ')' ')'
	| '(' /*19N*/ specify_path_identifiers spec_polarity K_SG '(' /*19N*/ specify_path_identifiers polarity_operator expression ')' ')'
	| '(' /*19N*/ edge_operator specify_path_identifiers spec_polarity K_SG '(' /*19N*/ specify_path_identifiers polarity_operator expression ')' ')'
	;

polarity_operator :
	K_PO_POS
	| K_PO_NEG
	| ':' /*4R*/
	;

specify_simple_path_decl :
	specify_simple_path '=' '(' /*19N*/ delay_value_list ')'
	| specify_simple_path '=' delay_value_simple
	| specify_simple_path '=' '(' /*19N*/ error ')'
	;

specify_simple_path :
	'(' /*19N*/ specify_path_identifiers spec_polarity K_EG specify_path_identifiers ')'
	| '(' /*19N*/ specify_path_identifiers spec_polarity K_SG specify_path_identifiers ')'
	| '(' /*19N*/ error ')'
	;

specify_path_identifiers :
	IDENTIFIER
	| IDENTIFIER '[' expr_primary ']'
	| IDENTIFIER '[' expr_primary polarity_operator expr_primary ']'
	| specify_path_identifiers ',' IDENTIFIER
	| specify_path_identifiers ',' IDENTIFIER '[' expr_primary ']'
	| specify_path_identifiers ',' IDENTIFIER '[' expr_primary polarity_operator expr_primary ']'
	;

specparam :
	IDENTIFIER '=' expr_mintypmax
	| PATHPULSE_IDENTIFIER '=' expression
	| PATHPULSE_IDENTIFIER '=' '(' /*19N*/ expression ',' expression ')'
	;

specparam_list :
	specparam
	| specparam_list ',' specparam
	;

specparam_decl :
	specparam_list
	| dimensions specparam_list
	;

spec_polarity :
	'+' /*13L*/
	| '-' /*13L*/
	| /*empty*/
	;

spec_reference_event :
	hierarchy_identifier
	| hierarchy_identifier K_TAND expression
	| K_posedge hierarchy_identifier
	| K_negedge hierarchy_identifier
	| K_posedge hierarchy_identifier K_TAND expression
	| K_negedge hierarchy_identifier K_TAND expression
	| K_edge '[' edge_descriptor_list ']' hierarchy_identifier
	| K_edge '[' edge_descriptor_list ']' hierarchy_identifier K_TAND expression
	;

edge_descriptor_list :
	edge_descriptor_list ',' K_edge_descriptor
	| K_edge_descriptor
	;

setuphold_opt_args :
	setuphold_recrem_opt_notifier
	| /*empty*/
	;

recrem_opt_args :
	setuphold_recrem_opt_notifier
	| /*empty*/
	;

setuphold_recrem_opt_notifier :
	','
	| ',' hierarchy_identifier
	| ',' setuphold_recrem_opt_timestamp_cond
	| ',' hierarchy_identifier setuphold_recrem_opt_timestamp_cond
	;

setuphold_recrem_opt_timestamp_cond :
	','
	| ',' expression
	| ',' setuphold_recrem_opt_timecheck_cond
	| ',' expression setuphold_recrem_opt_timecheck_cond
	;

setuphold_recrem_opt_timecheck_cond :
	','
	| ',' expression
	| ',' setuphold_recrem_opt_delayed_reference
	| ',' expression setuphold_recrem_opt_delayed_reference
	;

setuphold_recrem_opt_delayed_reference :
	','
	| ',' hierarchy_identifier
	| ',' setuphold_recrem_opt_delayed_data
	| ',' hierarchy_identifier setuphold_recrem_opt_delayed_data
	;

setuphold_recrem_opt_delayed_data :
	','
	| ',' hierarchy_identifier
	;

timeskew_opt_args :
	timeskew_fullskew_opt_notifier
	| /*empty*/
	;

fullskew_opt_args :
	timeskew_fullskew_opt_notifier
	| /*empty*/
	;

timeskew_fullskew_opt_notifier :
	','
	| ',' hierarchy_identifier
	| ',' timeskew_fullskew_opt_event_based_flag
	| ',' hierarchy_identifier timeskew_fullskew_opt_event_based_flag
	;

timeskew_fullskew_opt_event_based_flag :
	','
	| ',' expression
	| ',' timeskew_fullskew_opt_remain_active_flag
	| ',' expression timeskew_fullskew_opt_remain_active_flag
	;

timeskew_fullskew_opt_remain_active_flag :
	','
	| ',' expression
	;

spec_notifier_opt :
	/*empty*/
	| spec_notifier
	;

spec_notifier :
	','
	| ',' hierarchy_identifier
	;

subroutine_call :
	hierarchy_identifier argument_list_parens_opt
	| class_hierarchy_identifier argument_list_parens_opt
	| SYSTEM_IDENTIFIER argument_list_parens_opt
	| hierarchy_identifier '(' /*19N*/ error ')'
	;

statement_item :
	K_assign lpvalue '=' expression ';'
	| K_deassign lpvalue ';'
	| K_force lpvalue '=' expression ';'
	| K_release lpvalue ';'
	| K_begin label_opt block_item_decls_opt statement_or_null_list_opt K_end label_opt
	| K_fork label_opt block_item_decls_opt statement_or_null_list_opt join_keyword label_opt
	| K_disable hierarchy_identifier ';'
	| K_disable K_fork ';'
	| K_TRIGGER /*3R*/ hierarchy_identifier ';'
	| K_TRIGGER /*3R*/ package_scope hierarchy_identifier
	| K_NB_TRIGGER /*2N*/ hierarchy_identifier ';'
	| K_NB_TRIGGER /*2N*/ delay1 hierarchy_identifier ';'
	| K_NB_TRIGGER /*2N*/ event_control hierarchy_identifier ';'
	| K_NB_TRIGGER /*2N*/ K_repeat '(' /*19N*/ expression ')' event_control hierarchy_identifier ';'
	| procedural_assertion_statement
	| loop_statement
	| jump_statement
	| unique_priority K_case '(' /*19N*/ expression ')' case_items K_endcase
	| unique_priority K_casex '(' /*19N*/ expression ')' case_items K_endcase
	| unique_priority K_casez '(' /*19N*/ expression ')' case_items K_endcase
	| unique_priority K_case '(' /*19N*/ expression ')' error K_endcase
	| unique_priority K_casex '(' /*19N*/ expression ')' error K_endcase
	| unique_priority K_casez '(' /*19N*/ expression ')' error K_endcase
	| K_if '(' /*19N*/ expression ')' statement_or_null %prec less_than_K_else /*17N*/
	| K_if '(' /*19N*/ expression ')' statement_or_null K_else /*18N*/ statement_or_null
	| K_if '(' /*19N*/ error ')' statement_or_null %prec less_than_K_else /*17N*/
	| K_if '(' /*19N*/ error ')' statement_or_null K_else /*18N*/ statement_or_null
	| compressed_statement ';'
	| inc_or_dec_expression ';'
	| delay1 statement_or_null
	| event_control statement_or_null
	| '@' '*' /*14L*/ statement_or_null
	| '@' '(' /*19N*/ '*' /*14L*/ ')' statement_or_null
	| lpvalue '=' expression ';'
	| error '=' expression ';'
	| lpvalue K_LE /*11L*/ expression ';'
	| error K_LE /*11L*/ expression ';'
	| lpvalue '=' delay1 expression ';'
	| lpvalue K_LE /*11L*/ delay1 expression ';'
	| lpvalue '=' event_control expression ';'
	| lpvalue '=' K_repeat '(' /*19N*/ expression ')' event_control expression ';'
	| lpvalue K_LE /*11L*/ event_control expression ';'
	| lpvalue K_LE /*11L*/ K_repeat '(' /*19N*/ expression ')' event_control expression ';'
	| lpvalue '=' dynamic_array_new ';'
	| lpvalue '=' class_new ';'
	| K_wait '(' /*19N*/ expression ')' statement_or_null
	| K_wait K_fork ';'
	| K_void '\'' '(' /*19N*/ subroutine_call ')' ';'
	| subroutine_call ';'
	| hierarchy_identifier K_with '{' constraint_block_item_list_opt '}' ';'
	| implicit_class_handle K_new argument_list_parens_opt ';'
	| error ';'
	;

compressed_operator :
	K_PLUS_EQ /*1N*/
	| K_MINUS_EQ /*1N*/
	| K_MUL_EQ /*1N*/
	| K_DIV_EQ /*1N*/
	| K_MOD_EQ /*1N*/
	| K_AND_EQ /*1N*/
	| K_OR_EQ /*1N*/
	| K_XOR_EQ /*2N*/
	| K_LS_EQ /*2N*/
	| K_RS_EQ /*2N*/
	| K_RSS_EQ /*2N*/
	;

compressed_statement :
	lpvalue compressed_operator expression
	;

statement_or_null_list_opt :
	statement_or_null_list
	| /*empty*/
	;

statement_or_null_list :
	statement_or_null_list statement_or_null
	| statement_or_null
	;

analog_statement :
	branch_probe_expression K_CONTRIBUTE expression ';'
	;

tf_port_list_opt :
	tf_port_list
	| /*empty*/
	;

tf_port_list_parens_opt :
	'(' /*19N*/ tf_port_list_opt ')'
	| /*empty*/
	;

udp_body :
	K_table udp_entry_list K_endtable
	| K_table K_endtable
	| K_table error K_endtable
	;

udp_entry_list :
	udp_comb_entry_list
	| udp_sequ_entry_list
	;

udp_comb_entry :
	udp_input_list ':' /*4R*/ udp_output_sym ';'
	;

udp_comb_entry_list :
	udp_comb_entry
	| udp_comb_entry_list udp_comb_entry
	;

udp_sequ_entry_list :
	udp_sequ_entry
	| udp_sequ_entry_list udp_sequ_entry
	;

udp_sequ_entry :
	udp_input_list ':' /*4R*/ udp_input_sym ':' /*4R*/ udp_output_sym ';'
	;

udp_initial :
	K_initial IDENTIFIER '=' number ';'
	;

udp_init_opt :
	udp_initial
	| /*empty*/
	;

udp_input_list :
	udp_input_sym
	| udp_input_list udp_input_sym
	;

udp_input_sym :
	'0'
	| '1'
	| 'x'
	| '?' /*4R*/
	| 'b'
	| '*' /*14L*/
	| '%' /*14L*/
	| 'f'
	| 'F'
	| 'l'
	| 'h'
	| 'B'
	| 'r'
	| 'R'
	| 'M'
	| 'n'
	| 'N'
	| 'p'
	| 'P'
	| 'Q'
	| 'q'
	| '_'
	| '+' /*13L*/
	| DEC_NUMBER
	;

udp_output_sym :
	'0'
	| '1'
	| 'x'
	| '-' /*13L*/
	| DEC_NUMBER
	;

udp_port_decl :
	K_input list_of_identifiers ';'
	| K_output IDENTIFIER ';'
	| K_reg IDENTIFIER ';'
	| K_output K_reg IDENTIFIER ';'
	;

udp_port_decls :
	udp_port_decl
	| udp_port_decls udp_port_decl
	;

udp_port_list :
	IDENTIFIER
	| udp_port_list ',' IDENTIFIER
	;

udp_reg_opt :
	K_reg
	| /*empty*/
	;

udp_input_declaration_list :
	K_input IDENTIFIER
	| udp_input_declaration_list ',' K_input IDENTIFIER
	;

udp_primitive :
	K_primitive IDENTIFIER '(' /*19N*/ udp_port_list ')' ';' udp_port_decls udp_init_opt udp_body K_endprimitive label_opt
	| K_primitive IDENTIFIER '(' /*19N*/ K_output udp_reg_opt IDENTIFIER initializer_opt ',' udp_input_declaration_list ')' ';' udp_body K_endprimitive label_opt
	;

unique_priority :
	/*empty*/
	| K_unique
	| K_unique0
	| K_priority
	;

K_genvar_opt :
	K_genvar
	| /*empty*/
	;

K_static_opt :
	K_static
	| /*empty*/
	;

K_virtual_opt :
	K_virtual
	| /*empty*/
	;

K_var_opt :
	K_var
	| /*empty*/
	;

//Lexer

K_PLUS_EQ : '+=' ;
K_MINUS_EQ : '-=' ;
K_MUL_EQ : '*=' ;
K_DIV_EQ : '/=' ;
K_MOD_EQ : '%=' ;
K_AND_EQ : '&=' ;
K_OR_EQ : '|=' ;
K_NB_TRIGGER : 'K_NB_TRIGGER' ;
K_XOR_EQ : 'K_XOR_EQ' ;
K_LS_EQ : '<<=' ;
K_RS_EQ : '>>=' ;
K_RSS_EQ : 'K_RSS_EQ' ;
K_TRIGGER : 'K_TRIGGER' ;
K_LEQUIV : 'K_LEQUIV' ;
K_inside : 'inside' ;
K_LOR : '||' ;
K_LAND : '&&' ;
K_NOR : 'K_NOR' ;
K_NXOR : 'K_NXOR' ;
K_NAND : 'K_NAND' ;
K_EQ : '==' ;
K_NE : '!=' ;
K_CEQ : 'K_CEQ' ;
K_CNE : 'K_CNE' ;
K_WEQ : 'K_WEQ' ;
K_WNE : 'K_WNE' ;
K_LE : '<=' ;
K_GE : '>=' ;
K_LS : 'K_LS' ;
K_RS : 'K_RS' ;
K_RSS : 'K_RSS' ;
K_POW : 'K_POW' ;
K_else : 'else' ;
K_exclude : 'exclude' ;
K_timeprecision : 'timeprecision' ;
K_timeunit : 'timeunit' ;
K_assert : 'assert' ;
K_assume : 'assume' ;
K_LP : 'K_LP' ;
IDENTIFIER : "[a-zA-Z_][a-zA-Z_0-9]*" ;
K_class : 'class' ;
K_endclass : 'endclass' ;
TYPE_IDENTIFIER : 'TYPE_IDENTIFIER' ;
K_extends : 'extends' ;
K_function : 'function' ;
K_new : 'new' ;
K_endfunction : 'endfunction' ;
K_const : 'const' ;
K_extern : 'extern' ;
K_task : 'task' ;
K_static : 'static' ;
K_protected : 'protected' ;
K_local : 'local' ;
K_SCOPE_RES : 'K_SCOPE_RES' ;
K_property : 'property' ;
K_cover : 'cover' ;
K_sequence : 'sequence' ;
K_restrict : 'restrict' ;
K_constraint : 'constraint' ;
K_dist : 'dist' ;
K_if : 'if' ;
K_foreach : 'foreach' ;
K_CONSTRAINT_IMPL : 'K_CONSTRAINT_IMPL' ;
K_var : 'var' ;
K_event : 'event' ;
PACKAGE_IDENTIFIER : 'PACKAGE_IDENTIFIER' ;
K_time : 'time' ;
K_string : 'string' ;
K_vectored : 'vectored' ;
K_scalared : 'scalared' ;
K_void : 'void' ;
DEC_NUMBER : "[0-9]+" ;
K_final : 'final' ;
KK_attribute : 'KK_attribute' ;
STRING : "\"(\\.|[^\\\"])+\"" ;
K_INCR : '++' ;
K_DECR : '--' ;
K_import : 'import' ;
K_export : 'export' ;
K_this : 'this' ;
K_super : 'super' ;
K_reg : 'reg' ;
K_bit : 'bit' ;
K_logic : 'logic' ;
K_bool : 'bool' ;
K_join : 'join' ;
K_join_none : 'join_none' ;
K_join_any : 'join_any' ;
K_break : 'break' ;
K_continue : 'continue' ;
K_return : 'return' ;
K_automatic : 'automatic' ;
K_for : 'for' ;
K_forever : 'forever' ;
K_repeat : 'repeat' ;
K_while : 'while' ;
K_do : 'do' ;
K_virtual : 'virtual' ;
K_modport : 'modport' ;
K_clocking : 'clocking' ;
K_real : 'real' ;
K_realtime : 'realtime' ;
K_shortreal : 'shortreal' ;
BASED_NUMBER : 'BASED_NUMBER' ;
UNBASED_NUMBER : 'UNBASED_NUMBER' ;
K_package : 'package' ;
K_endpackage : 'endpackage' ;
K_input : 'input' ;
K_output : 'output' ;
K_inout : 'inout' ;
K_ref : 'ref' ;
K_disable : 'disable' ;
K_iff : 'iff' ;
K_rand : 'rand' ;
K_randc : 'randc' ;
K_signed : 'signed' ;
K_unsigned : 'unsigned' ;
K_endtask : 'endtask' ;
TIME_LITERAL : 'TIME_LITERAL' ;
K_PSTAR : 'K_PSTAR' ;
K_STARP : 'K_STARP' ;
K_struct : 'struct' ;
K_union : 'union' ;
K_typedef : 'typedef' ;
K_enum : 'enum' ;
K_packed : 'packed' ;
K_default : 'default' ;
K_small : 'small' ;
K_medium : 'medium' ;
K_large : 'large' ;
REALTIME : 'REALTIME' ;
K_discipline : 'discipline' ;
K_enddiscipline : 'enddiscipline' ;
K_domain : 'domain' ;
K_discrete : 'discrete' ;
K_continuous : 'continuous' ;
K_potential : 'potential' ;
K_flow : 'flow' ;
K_nature : 'nature' ;
K_endnature : 'endnature' ;
K_units : 'units' ;
K_abstol : 'abstol' ;
K_access : 'access' ;
K_idt_nature : 'idt_nature' ;
K_ddt_nature : 'ddt_nature' ;
K_config : 'config' ;
K_design : 'design' ;
K_endconfig : 'endconfig' ;
K_liblist : 'liblist' ;
K_instance : 'instance' ;
K_use : 'use' ;
K_cell : 'cell' ;
K_highz1 : 'highz1' ;
K_highz0 : 'highz0' ;
K_supply0 : 'supply0' ;
K_strong0 : 'strong0' ;
K_pull0 : 'pull0' ;
K_weak0 : 'weak0' ;
K_supply1 : 'supply1' ;
K_strong1 : 'strong1' ;
K_pull1 : 'pull1' ;
K_weak1 : 'weak1' ;
K_or : 'or' ;
K_posedge : 'posedge' ;
K_negedge : 'negedge' ;
K_edge : 'edge' ;
SYSTEM_IDENTIFIER : 'SYSTEM_IDENTIFIER' ;
K_and : 'and' ;
K_unique : 'unique' ;
K_xor : 'xor' ;
K_acos : 'acos' ;
K_acosh : 'acosh' ;
K_asin : 'asin' ;
K_asinh : 'asinh' ;
K_atan : 'atan' ;
K_atanh : 'atanh' ;
K_atan2 : 'atan2' ;
K_ceil : 'ceil' ;
K_cos : 'cos' ;
K_cosh : 'cosh' ;
K_exp : 'exp' ;
K_floor : 'floor' ;
K_hypot : 'hypot' ;
K_ln : 'ln' ;
K_log : 'log' ;
K_pow : 'pow' ;
K_sin : 'sin' ;
K_sinh : 'sinh' ;
K_sqrt : 'sqrt' ;
K_tan : 'tan' ;
K_tanh : 'tanh' ;
K_abs : 'abs' ;
K_max : 'max' ;
K_min : 'min' ;
K_null : 'null' ;
K_nand : 'nand' ;
K_nor : 'nor' ;
K_xnor : 'xnor' ;
K_buf : 'buf' ;
K_bufif0 : 'bufif0' ;
K_bufif1 : 'bufif1' ;
K_not : 'not' ;
K_notif0 : 'notif0' ;
K_notif1 : 'notif1' ;
K_nmos : 'nmos' ;
K_rnmos : 'rnmos' ;
K_pmos : 'pmos' ;
K_rpmos : 'rpmos' ;
K_cmos : 'cmos' ;
K_rcmos : 'rcmos' ;
K_tran : 'tran' ;
K_rtran : 'rtran' ;
K_tranif0 : 'tranif0' ;
K_tranif1 : 'tranif1' ;
K_rtranif0 : 'rtranif0' ;
K_rtranif1 : 'rtranif1' ;
K_PO_POS : 'K_PO_POS' ;
K_PO_NEG : 'K_PO_NEG' ;
K_wreal : 'wreal' ;
K_byte : 'byte' ;
K_shortint : 'shortint' ;
K_int : 'int' ;
K_longint : 'longint' ;
K_integer : 'integer' ;
K_module : 'module' ;
K_macromodule : 'macromodule' ;
K_program : 'program' ;
K_interface : 'interface' ;
K_endmodule : 'endmodule' ;
K_endprogram : 'endprogram' ;
K_endinterface : 'endinterface' ;
K_type : 'type' ;
K_trireg : 'trireg' ;
K_let : 'let' ;
DISCIPLINE_IDENTIFIER : 'DISCIPLINE_IDENTIFIER' ;
K_defparam : 'defparam' ;
K_pullup : 'pullup' ;
K_pulldown : 'pulldown' ;
K_assign : 'assign' ;
K_always : 'always' ;
K_always_comb : 'always_comb' ;
K_always_ff : 'always_ff' ;
K_always_latch : 'always_latch' ;
K_initial : 'initial' ;
K_analog : 'analog' ;
K_generate : 'generate' ;
K_endgenerate : 'endgenerate' ;
K_genvar : 'genvar' ;
K_case : 'case' ;
K_endcase : 'endcase' ;
K_specparam : 'specparam' ;
K_specify : 'specify' ;
K_endspecify : 'endspecify' ;
K_untyped : 'untyped' ;
K_begin : 'begin' ;
K_end : 'end' ;
K_wire : 'wire' ;
K_tri : 'tri' ;
K_tri1 : 'tri1' ;
K_wand : 'wand' ;
K_triand : 'triand' ;
K_tri0 : 'tri0' ;
K_wor : 'wor' ;
K_trior : 'trior' ;
K_wone : 'wone' ;
K_uwire : 'uwire' ;
K_parameter : 'parameter' ;
K_localparam : 'localparam' ;
K_inf : 'inf' ;
K_from : 'from' ;
K_DOTSTAR : 'K_DOTSTAR' ;
K_ifnone : 'ifnone' ;
K_Sfullskew : 'K_Sfullskew' ;
K_Shold : 'K_Shold' ;
K_Snochange : 'K_Snochange' ;
K_Speriod : 'K_Speriod' ;
K_Srecovery : 'K_Srecovery' ;
K_Srecrem : 'K_Srecrem' ;
K_Sremoval : 'K_Sremoval' ;
K_Ssetup : 'K_Ssetup' ;
K_Ssetuphold : 'K_Ssetuphold' ;
K_Sskew : 'K_Sskew' ;
K_Stimeskew : 'K_Stimeskew' ;
K_Swidth : 'K_Swidth' ;
K_pulsestyle_onevent : 'pulsestyle_onevent' ;
K_pulsestyle_ondetect : 'pulsestyle_ondetect' ;
K_showcancelled : 'showcancelled' ;
K_noshowcancelled : 'noshowcancelled' ;
K_EG : 'K_EG' ;
K_SG : 'K_SG' ;
PATHPULSE_IDENTIFIER : 'PATHPULSE_IDENTIFIER' ;
K_TAND : 'K_TAND' ;
K_edge_descriptor : 'edge_descriptor' ;
K_deassign : 'deassign' ;
K_force : 'force' ;
K_release : 'release' ;
K_fork : 'fork' ;
K_casex : 'casex' ;
K_casez : 'casez' ;
K_wait : 'wait' ;
K_with : 'with' ;
K_CONTRIBUTE : 'K_CONTRIBUTE' ;
K_table : 'table' ;
K_endtable : 'endtable' ;
K_primitive : 'primitive' ;
K_endprimitive : 'endprimitive' ;
K_unique0 : 'unique0' ;
K_priority : 'priority' ;

}
