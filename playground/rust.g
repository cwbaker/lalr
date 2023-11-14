//From: https://github.com/bleibig/rust-grammar.git

rust {

%whitespace "[ \t\r\n]*";
%whitespace "//[^\n]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

/*Tokens*/
//%token YYEOF
//%token YYerror
//%token YYUNDEF
//%token SHL
//%token SHR
//%token LE
//%token EQEQ
//%token NE
//%token GE
//%token ANDAND
//%token OROR
//%token SHLEQ
//%token SHREQ
//%token MINUSEQ
//%token ANDEQ
//%token OREQ
//%token PLUSEQ
//%token STAREQ
//%token SLASHEQ
//%token CARETEQ
//%token PERCENTEQ
//%token DOTDOT
//%token DOTDOTDOT
//%token MOD_SEP
//%token RARROW
//%token LARROW
//%token FAT_ARROW
//%token LIT_BYTE
//%token LIT_CHAR
//%token LIT_INTEGER
//%token LIT_FLOAT
//%token LIT_STR
//%token LIT_STR_RAW
//%token LIT_BYTE_STR
//%token LIT_BYTE_STR_RAW
//%token IDENT
//%token UNDERSCORE
//%token LIFETIME
//%token SELF
//%token STATIC
//%token ABSTRACT
//%token ALIGNOF
//%token AS
//%token BECOME
//%token BREAK
//%token CATCH
//%token CRATE
//%token DO
//%token ELSE
//%token ENUM
//%token EXTERN
//%token FALSE
//%token FINAL
//%token FN
//%token FOR
//%token IF
//%token IMPL
//%token IN
//%token LET
//%token LOOP
//%token MACRO
//%token MATCH
//%token MOD
//%token MOVE
//%token MUT
//%token OFFSETOF
//%token OVERRIDE
//%token PRIV
//%token PUB
//%token PURE
//%token REF
//%token RETURN
//%token SIZEOF
//%token STRUCT
//%token SUPER
//%token UNION
//%token UNSIZED
//%token TRUE
//%token TRAIT
//%token TYPE
//%token UNSAFE
//%token VIRTUAL
//%token YIELD
//%token DEFAULT
//%token USE
//%token WHILE
//%token CONTINUE
//%token PROC
//%token BOX
//%token CONST
//%token WHERE
//%token TYPEOF
//%token INNER_DOC_COMMENT
//%token OUTER_DOC_COMMENT
//%token SHEBANG
//%token SHEBANG_LINE
//%token STATIC_LIFETIME
//%token PRECLOW
//%token LAMBDA
//%token SHIFTPLUS
//%token ':'
//%token FORTYPE
//%token '?'
//%token '='
//%token '<'
//%token '>'
//%token '|'
//%token '^'
//%token '&'
//%token '+'
//%token '-'
//%token '*'
//%token '/'
//%token '%'
//%token '!'
//%token '{'
//%token '['
//%token '('
//%token '.'
//%token RANGE
//%token ']'
//%token '#'
//%token ')'
//%token ','
//%token ';'
//%token '}'
//%token '@'
//%token '~'
//%token '$'

%precedence /*1*/ PRECLOW ;
%precedence /*2*/ LAMBDA ;
%precedence /*3*/ SELF ;
%precedence /*4*/ MUT ;
%precedence /*5*/ IDENT ;
%precedence /*6*/ CATCH ;
%precedence /*7*/ DEFAULT ;
%precedence /*8*/ UNION ;
%precedence /*9*/ SHIFTPLUS ;
%precedence /*10*/ MOD_SEP ;
%precedence /*11*/ RARROW ':' ;
%precedence /*12*/ FORTYPE ;
%precedence /*13*/ FOR ;
%precedence /*14*/ '?' ;
%precedence /*15*/ BOX ;
%nonassoc /*16*/ DOTDOT ;
%precedence /*17*/ RETURN YIELD ;
%right /*18*/ SHLEQ SHREQ MINUSEQ ANDEQ OREQ PLUSEQ STAREQ SLASHEQ CARETEQ PERCENTEQ '=' ;
%right /*19*/ LARROW ;
%left /*20*/ OROR ;
%left /*21*/ ANDAND ;
%left /*22*/ EQEQ NE ;
%left /*23*/ LE GE '<' '>' ;
%left /*24*/ '|' ;
%left /*25*/ '^' ;
%left /*26*/ '&' ;
%left /*27*/ SHL SHR ;
%left /*28*/ '+' '-' ;
%precedence /*29*/ AS ;
%left /*30*/ '*' '/' '%' ;
%precedence /*31*/ '!' ;
%precedence /*32*/ '{' '[' '(' '.' ;
%precedence /*33*/ RANGE ;

//%start crate

//%%

crate :
	/*YYEOF
	|*/ maybe_shebang inner_attrs maybe_mod_items
	| maybe_shebang maybe_mod_items
	;

maybe_shebang :
	SHEBANG_LINE
	| /*empty*/
	;

maybe_inner_attrs :
	inner_attrs
	| /*empty*/
	;

inner_attrs :
	inner_attr
	| inner_attrs inner_attr
	;

inner_attr :
	SHEBANG '[' /*32P*/ meta_item ']'
	| INNER_DOC_COMMENT
	;

maybe_outer_attrs :
	outer_attrs
	| /*empty*/
	;

outer_attrs :
	outer_attr
	| outer_attrs outer_attr
	;

outer_attr :
	'#' '[' /*32P*/ meta_item ']'
	| OUTER_DOC_COMMENT
	;

meta_item :
	ident
	| ident '=' /*18R*/ lit
	| ident '(' /*32P*/ meta_seq ')'
	| ident '(' /*32P*/ meta_seq ',' ')'
	;

meta_seq :
	/*empty*/
	| meta_item
	| meta_seq MOD_SEP /*10P*/ meta_item
	| meta_seq ',' meta_item
	;

maybe_mod_items :
	mod_items
	| /*empty*/
	;

mod_items :
	mod_item
	| mod_items mod_item
	;

attrs_and_vis :
	maybe_outer_attrs visibility
	;

mod_item :
	attrs_and_vis item
	;

item :
	stmt_item
	| item_macro
	;

stmt_item :
	item_static
	| item_const
	| item_type
	| block_item
	| view_item
	;

item_static :
	STATIC ident ':' /*11P*/ ty '=' /*18R*/ expr ';'
	| STATIC MUT /*4P*/ ident ':' /*11P*/ ty '=' /*18R*/ expr ';'
	;

item_const :
	CONST ident ':' /*11P*/ ty '=' /*18R*/ expr ';'
	;

item_macro :
	path_expr '!' /*31P*/ maybe_ident parens_delimited_token_trees ';'
	| path_expr '!' /*31P*/ maybe_ident braces_delimited_token_trees
	| path_expr '!' /*31P*/ maybe_ident brackets_delimited_token_trees ';'
	;

view_item :
	use_item
	| extern_fn_item
	| EXTERN CRATE ident ';'
	| EXTERN CRATE ident AS /*29P*/ ident ';'
	;

extern_fn_item :
	EXTERN maybe_abi item_fn
	;

use_item :
	USE view_path ';'
	;

view_path :
	path_no_types_allowed
	| path_no_types_allowed MOD_SEP /*10P*/ '{' /*32P*/ '}'
	| MOD_SEP /*10P*/ '{' /*32P*/ '}'
	| path_no_types_allowed MOD_SEP /*10P*/ '{' /*32P*/ idents_or_self '}'
	| MOD_SEP /*10P*/ '{' /*32P*/ idents_or_self '}'
	| path_no_types_allowed MOD_SEP /*10P*/ '{' /*32P*/ idents_or_self ',' '}'
	| MOD_SEP /*10P*/ '{' /*32P*/ idents_or_self ',' '}'
	| path_no_types_allowed MOD_SEP /*10P*/ '*' /*30L*/
	| MOD_SEP /*10P*/ '*' /*30L*/
	| '*' /*30L*/
	| '{' /*32P*/ '}'
	| '{' /*32P*/ idents_or_self '}'
	| '{' /*32P*/ idents_or_self ',' '}'
	| path_no_types_allowed AS /*29P*/ ident
	;

block_item :
	item_fn
	| item_unsafe_fn
	| item_mod
	| item_foreign_mod
	| item_struct
	| item_enum
	| item_union
	| item_trait
	| item_impl
	;

maybe_ty_ascription :
	':' /*11P*/ ty_sum
	| /*empty*/
	;

maybe_init_expr :
	'=' /*18R*/ expr
	| /*empty*/
	;

item_struct :
	STRUCT ident generic_params maybe_where_clause struct_decl_args
	| STRUCT ident generic_params struct_tuple_args maybe_where_clause ';'
	| STRUCT ident generic_params maybe_where_clause ';'
	;

struct_decl_args :
	'{' /*32P*/ struct_decl_fields '}'
	| '{' /*32P*/ struct_decl_fields ',' '}'
	;

struct_tuple_args :
	'(' /*32P*/ struct_tuple_fields ')'
	| '(' /*32P*/ struct_tuple_fields ',' ')'
	;

struct_decl_fields :
	struct_decl_field
	| struct_decl_fields ',' struct_decl_field
	| /*empty*/
	;

struct_decl_field :
	attrs_and_vis ident ':' /*11P*/ ty_sum
	;

struct_tuple_fields :
	struct_tuple_field
	| struct_tuple_fields ',' struct_tuple_field
	| /*empty*/
	;

struct_tuple_field :
	attrs_and_vis ty_sum
	;

item_enum :
	ENUM ident generic_params maybe_where_clause '{' /*32P*/ enum_defs '}'
	| ENUM ident generic_params maybe_where_clause '{' /*32P*/ enum_defs ',' '}'
	;

enum_defs :
	enum_def
	| enum_defs ',' enum_def
	| /*empty*/
	;

enum_def :
	attrs_and_vis ident enum_args
	;

enum_args :
	'{' /*32P*/ struct_decl_fields '}'
	| '{' /*32P*/ struct_decl_fields ',' '}'
	| '(' /*32P*/ maybe_ty_sums ')'
	| '=' /*18R*/ expr
	| /*empty*/
	;

item_union :
	UNION /*8P*/ ident generic_params maybe_where_clause '{' /*32P*/ struct_decl_fields '}'
	| UNION /*8P*/ ident generic_params maybe_where_clause '{' /*32P*/ struct_decl_fields ',' '}'
	;

item_mod :
	MOD ident ';'
	| MOD ident '{' /*32P*/ maybe_mod_items '}'
	| MOD ident '{' /*32P*/ inner_attrs maybe_mod_items '}'
	;

item_foreign_mod :
	EXTERN maybe_abi '{' /*32P*/ maybe_foreign_items '}'
	| EXTERN maybe_abi '{' /*32P*/ inner_attrs maybe_foreign_items '}'
	;

maybe_abi :
	str
	| /*empty*/
	;

maybe_foreign_items :
	foreign_items
	| /*empty*/
	;

foreign_items :
	foreign_item
	| foreign_items foreign_item
	;

foreign_item :
	attrs_and_vis STATIC item_foreign_static
	| attrs_and_vis item_foreign_fn
	| attrs_and_vis UNSAFE item_foreign_fn
	| attrs_and_vis TYPE item_foreign_type
	;

item_foreign_static :
	maybe_mut ident ':' /*11P*/ ty ';'
	;

item_foreign_fn :
	FN ident generic_params fn_decl_allow_variadic maybe_where_clause ';'
	;

item_foreign_type :
	ident generic_params ';'
	;

fn_decl_allow_variadic :
	fn_params_allow_variadic ret_ty
	;

fn_params_allow_variadic :
	'(' /*32P*/ ')'
	| '(' /*32P*/ params ')'
	| '(' /*32P*/ params ',' ')'
	| '(' /*32P*/ params ',' DOTDOTDOT ')'
	;

visibility :
	PUB %prec PRECLOW /*1P*/
	| PUB '(' /*32P*/ pub_param ')'
	| CRATE
	| /*empty*/
	;

pub_param :
	SELF /*3P*/
	| SUPER
	| CRATE
	;

idents_or_self :
	ident_or_self
	| idents_or_self AS /*29P*/ ident
	| idents_or_self MOD_SEP /*10P*/ ident
	| idents_or_self ',' ident_or_self
	;

ident_or_self :
	ident
	| SELF /*3P*/
	;

item_type :
	TYPE ident generic_params maybe_where_clause '=' /*18R*/ ty_sum ';'
	;

for_sized :
	FOR /*13P*/ '?' /*14P*/ ident
	| FOR /*13P*/ ident '?' /*14P*/
	| /*empty*/
	;

item_trait :
	maybe_unsafe TRAIT ident generic_params for_sized maybe_ty_param_bounds maybe_where_clause '{' /*32P*/ maybe_trait_items '}'
	;

maybe_trait_items :
	trait_items
	| /*empty*/
	;

trait_items :
	trait_item
	| trait_items trait_item
	;

trait_item :
	trait_const
	| trait_type
	| trait_method
	| maybe_outer_attrs item_macro
	;

trait_const :
	maybe_outer_attrs CONST ident maybe_ty_ascription maybe_const_default ';'
	;

maybe_const_default :
	'=' /*18R*/ expr
	| /*empty*/
	;

trait_type :
	maybe_outer_attrs TYPE ty_param ';'
	;

maybe_unsafe :
	UNSAFE
	| /*empty*/
	;

maybe_default_maybe_unsafe :
	DEFAULT /*7P*/ UNSAFE
	| DEFAULT /*7P*/
	| UNSAFE
	| /*empty*/
	;

trait_method :
	type_method
	| method
	;

type_method :
	maybe_outer_attrs maybe_unsafe FN ident generic_params fn_decl_with_self_allow_anon_params maybe_where_clause ';'
	| maybe_outer_attrs CONST maybe_unsafe FN ident generic_params fn_decl_with_self_allow_anon_params maybe_where_clause ';'
	| maybe_outer_attrs maybe_unsafe EXTERN maybe_abi FN ident generic_params fn_decl_with_self_allow_anon_params maybe_where_clause ';'
	;

method :
	maybe_outer_attrs maybe_unsafe FN ident generic_params fn_decl_with_self_allow_anon_params maybe_where_clause inner_attrs_and_block
	| maybe_outer_attrs CONST maybe_unsafe FN ident generic_params fn_decl_with_self_allow_anon_params maybe_where_clause inner_attrs_and_block
	| maybe_outer_attrs maybe_unsafe EXTERN maybe_abi FN ident generic_params fn_decl_with_self_allow_anon_params maybe_where_clause inner_attrs_and_block
	;

impl_method :
	attrs_and_vis maybe_default maybe_unsafe FN ident generic_params fn_decl_with_self maybe_where_clause inner_attrs_and_block
	| attrs_and_vis maybe_default CONST maybe_unsafe FN ident generic_params fn_decl_with_self maybe_where_clause inner_attrs_and_block
	| attrs_and_vis maybe_default maybe_unsafe EXTERN maybe_abi FN ident generic_params fn_decl_with_self maybe_where_clause inner_attrs_and_block
	;

item_impl :
	maybe_default_maybe_unsafe IMPL generic_params ty_prim_sum maybe_where_clause '{' /*32P*/ maybe_inner_attrs maybe_impl_items '}'
	| maybe_default_maybe_unsafe IMPL generic_params '(' /*32P*/ ty ')' maybe_where_clause '{' /*32P*/ maybe_inner_attrs maybe_impl_items '}'
	| maybe_default_maybe_unsafe IMPL generic_params trait_ref FOR /*13P*/ ty_sum maybe_where_clause '{' /*32P*/ maybe_inner_attrs maybe_impl_items '}'
	| maybe_default_maybe_unsafe IMPL generic_params '!' /*31P*/ trait_ref FOR /*13P*/ ty_sum maybe_where_clause '{' /*32P*/ maybe_inner_attrs maybe_impl_items '}'
	| maybe_default_maybe_unsafe IMPL generic_params trait_ref FOR /*13P*/ DOTDOT /*16N*/ '{' /*32P*/ '}'
	| maybe_default_maybe_unsafe IMPL generic_params '!' /*31P*/ trait_ref FOR /*13P*/ DOTDOT /*16N*/ '{' /*32P*/ '}'
	;

maybe_impl_items :
	impl_items
	| /*empty*/
	;

impl_items :
	impl_item
	| impl_item impl_items
	;

impl_item :
	impl_method
	| attrs_and_vis item_macro
	| impl_const
	| impl_type
	;

maybe_default :
	DEFAULT /*7P*/
	| /*empty*/
	;

impl_const :
	attrs_and_vis maybe_default item_const
	;

impl_type :
	attrs_and_vis maybe_default TYPE ident generic_params '=' /*18R*/ ty_sum ';'
	;

item_fn :
	FN ident generic_params fn_decl maybe_where_clause inner_attrs_and_block
	| CONST FN ident generic_params fn_decl maybe_where_clause inner_attrs_and_block
	;

item_unsafe_fn :
	UNSAFE FN ident generic_params fn_decl maybe_where_clause inner_attrs_and_block
	| CONST UNSAFE FN ident generic_params fn_decl maybe_where_clause inner_attrs_and_block
	| UNSAFE EXTERN maybe_abi FN ident generic_params fn_decl maybe_where_clause inner_attrs_and_block
	;

fn_decl :
	fn_params ret_ty
	;

fn_decl_with_self :
	fn_params_with_self ret_ty
	;

fn_decl_with_self_allow_anon_params :
	fn_anon_params_with_self ret_ty
	;

fn_params :
	'(' /*32P*/ maybe_params ')'
	;

fn_anon_params :
	'(' /*32P*/ anon_param anon_params_allow_variadic_tail ')'
	| '(' /*32P*/ ')'
	;

fn_params_with_self :
	'(' /*32P*/ maybe_mut SELF /*3P*/ maybe_ty_ascription maybe_comma_params ')'
	| '(' /*32P*/ '&' /*26L*/ maybe_mut SELF /*3P*/ maybe_ty_ascription maybe_comma_params ')'
	| '(' /*32P*/ '&' /*26L*/ lifetime maybe_mut SELF /*3P*/ maybe_ty_ascription maybe_comma_params ')'
	| '(' /*32P*/ maybe_params ')'
	;

fn_anon_params_with_self :
	'(' /*32P*/ maybe_mut SELF /*3P*/ maybe_ty_ascription maybe_comma_anon_params ')'
	| '(' /*32P*/ '&' /*26L*/ maybe_mut SELF /*3P*/ maybe_ty_ascription maybe_comma_anon_params ')'
	| '(' /*32P*/ '&' /*26L*/ lifetime maybe_mut SELF /*3P*/ maybe_ty_ascription maybe_comma_anon_params ')'
	| '(' /*32P*/ maybe_anon_params ')'
	;

maybe_params :
	params
	| params ','
	| /*empty*/
	;

params :
	param
	| params ',' param
	;

param :
	pat ':' /*11P*/ ty_sum
	;

inferrable_params :
	inferrable_param
	| inferrable_params ',' inferrable_param
	;

inferrable_param :
	pat maybe_ty_ascription
	;

maybe_comma_params :
	','
	| ',' params
	| ',' params ','
	| /*empty*/
	;

maybe_comma_anon_params :
	','
	| ',' anon_params
	| ',' anon_params ','
	| /*empty*/
	;

maybe_anon_params :
	anon_params
	| anon_params ','
	| /*empty*/
	;

anon_params :
	anon_param
	| anon_params ',' anon_param
	;

anon_param :
	named_arg ':' /*11P*/ ty
	| ty
	;

anon_params_allow_variadic_tail :
	',' DOTDOTDOT
	| ',' anon_param anon_params_allow_variadic_tail
	| /*empty*/
	;

named_arg :
	ident
	| UNDERSCORE
	| '&' /*26L*/ ident
	| '&' /*26L*/ UNDERSCORE
	| ANDAND /*21L*/ ident
	| ANDAND /*21L*/ UNDERSCORE
	| MUT /*4P*/ ident
	;

ret_ty :
	RARROW /*11P*/ '!' /*31P*/
	| RARROW /*11P*/ ty
	| %prec IDENT /*5P*/ /*empty*/
	;

generic_params :
	'<' /*23L*/ '>' /*23L*/
	| '<' /*23L*/ lifetimes '>' /*23L*/
	| '<' /*23L*/ lifetimes ',' '>' /*23L*/
	| '<' /*23L*/ lifetimes SHR /*27L*/
	| '<' /*23L*/ lifetimes ',' SHR /*27L*/
	| '<' /*23L*/ lifetimes ',' ty_params '>' /*23L*/
	| '<' /*23L*/ lifetimes ',' ty_params ',' '>' /*23L*/
	| '<' /*23L*/ lifetimes ',' ty_params SHR /*27L*/
	| '<' /*23L*/ lifetimes ',' ty_params ',' SHR /*27L*/
	| '<' /*23L*/ ty_params '>' /*23L*/
	| '<' /*23L*/ ty_params ',' '>' /*23L*/
	| '<' /*23L*/ ty_params SHR /*27L*/
	| '<' /*23L*/ ty_params ',' SHR /*27L*/
	| /*empty*/
	;

maybe_where_clause :
	/*empty*/
	| where_clause
	;

where_clause :
	WHERE where_predicates
	| WHERE where_predicates ','
	;

where_predicates :
	where_predicate
	| where_predicates ',' where_predicate
	;

where_predicate :
	maybe_for_lifetimes lifetime ':' /*11P*/ bounds
	| maybe_for_lifetimes ty ':' /*11P*/ ty_param_bounds
	;

maybe_for_lifetimes :
	FOR /*13P*/ '<' /*23L*/ lifetimes '>' /*23L*/
	| %prec FORTYPE /*12P*/ /*empty*/
	;

ty_params :
	ty_param
	| ty_params ',' ty_param
	;

path_no_types_allowed :
	ident
	| MOD_SEP /*10P*/ ident
	| SELF /*3P*/
	| MOD_SEP /*10P*/ SELF /*3P*/
	| SUPER
	| MOD_SEP /*10P*/ SUPER
	| CRATE
	| path_no_types_allowed MOD_SEP /*10P*/ ident
	;

path_generic_args_without_colons :
	ident %prec IDENT /*5P*/
	| ident generic_args %prec IDENT /*5P*/
	| ident '(' /*32P*/ maybe_ty_sums ')' ret_ty %prec IDENT /*5P*/
	| path_generic_args_without_colons MOD_SEP /*10P*/ ident %prec IDENT /*5P*/
	| path_generic_args_without_colons MOD_SEP /*10P*/ ident generic_args %prec IDENT /*5P*/
	| path_generic_args_without_colons MOD_SEP /*10P*/ ident '(' /*32P*/ maybe_ty_sums ')' ret_ty %prec IDENT /*5P*/
	;

generic_args :
	'<' /*23L*/ generic_values '>' /*23L*/
	| '<' /*23L*/ generic_values SHR /*27L*/
	| '<' /*23L*/ generic_values GE /*23L*/
	| '<' /*23L*/ generic_values SHREQ /*18R*/
	| SHL /*27L*/ ty_qualified_path_and_generic_values '>' /*23L*/
	| SHL /*27L*/ ty_qualified_path_and_generic_values SHR /*27L*/
	| SHL /*27L*/ ty_qualified_path_and_generic_values GE /*23L*/
	| SHL /*27L*/ ty_qualified_path_and_generic_values SHREQ /*18R*/
	;

generic_values :
	maybe_ty_sums_and_or_bindings
	;

maybe_ty_sums_and_or_bindings :
	ty_sums
	| ty_sums ','
	| ty_sums ',' bindings
	| bindings
	| bindings ','
	| /*empty*/
	;

maybe_bindings :
	',' bindings
	| /*empty*/
	;

pat :
	UNDERSCORE
	| '&' /*26L*/ pat
	| '&' /*26L*/ MUT /*4P*/ pat
	| ANDAND /*21L*/ pat
	| '(' /*32P*/ ')'
	| '(' /*32P*/ pat_tup ')'
	| '[' /*32P*/ pat_vec ']'
	| lit_or_path
	| lit_or_path DOTDOTDOT lit_or_path
	| path_expr '{' /*32P*/ pat_struct '}'
	| path_expr '(' /*32P*/ ')'
	| path_expr '(' /*32P*/ pat_tup ')'
	| path_expr '!' /*31P*/ maybe_ident delimited_token_trees
	| binding_mode ident
	| ident '@' pat
	| binding_mode ident '@' pat
	| BOX /*15P*/ pat
	| '<' /*23L*/ ty_sum maybe_as_trait_ref '>' /*23L*/ MOD_SEP /*10P*/ ident
	| SHL /*27L*/ ty_sum maybe_as_trait_ref '>' /*23L*/ MOD_SEP /*10P*/ ident maybe_as_trait_ref '>' /*23L*/ MOD_SEP /*10P*/ ident
	;

pats_or :
	pat
	| pats_or '|' /*24L*/ pat
	;

binding_mode :
	REF
	| REF MUT /*4P*/
	| MUT /*4P*/
	;

lit_or_path :
	path_expr
	| lit
	| '-' /*28L*/ lit
	;

pat_field :
	ident
	| binding_mode ident
	| BOX /*15P*/ ident
	| BOX /*15P*/ binding_mode ident
	| ident ':' /*11P*/ pat
	| binding_mode ident ':' /*11P*/ pat
	| LIT_INTEGER ':' /*11P*/ pat
	;

pat_fields :
	pat_field
	| pat_fields ',' pat_field
	;

pat_struct :
	pat_fields
	| pat_fields ','
	| pat_fields ',' DOTDOT /*16N*/
	| DOTDOT /*16N*/
	| /*empty*/
	;

pat_tup :
	pat_tup_elts
	| pat_tup_elts ','
	| pat_tup_elts DOTDOT /*16N*/
	| pat_tup_elts ',' DOTDOT /*16N*/
	| pat_tup_elts DOTDOT /*16N*/ ',' pat_tup_elts
	| pat_tup_elts DOTDOT /*16N*/ ',' pat_tup_elts ','
	| pat_tup_elts ',' DOTDOT /*16N*/ ',' pat_tup_elts
	| pat_tup_elts ',' DOTDOT /*16N*/ ',' pat_tup_elts ','
	| DOTDOT /*16N*/ ',' pat_tup_elts
	| DOTDOT /*16N*/ ',' pat_tup_elts ','
	| DOTDOT /*16N*/
	;

pat_tup_elts :
	pat
	| pat_tup_elts ',' pat
	;

pat_vec :
	pat_vec_elts
	| pat_vec_elts ','
	| pat_vec_elts DOTDOT /*16N*/
	| pat_vec_elts ',' DOTDOT /*16N*/
	| pat_vec_elts DOTDOT /*16N*/ ',' pat_vec_elts
	| pat_vec_elts DOTDOT /*16N*/ ',' pat_vec_elts ','
	| pat_vec_elts ',' DOTDOT /*16N*/ ',' pat_vec_elts
	| pat_vec_elts ',' DOTDOT /*16N*/ ',' pat_vec_elts ','
	| DOTDOT /*16N*/ ',' pat_vec_elts
	| DOTDOT /*16N*/ ',' pat_vec_elts ','
	| DOTDOT /*16N*/
	| /*empty*/
	;

pat_vec_elts :
	pat
	| pat_vec_elts ',' pat
	;

ty :
	ty_prim
	| ty_closure
	| '<' /*23L*/ ty_sum maybe_as_trait_ref '>' /*23L*/ MOD_SEP /*10P*/ ident
	| SHL /*27L*/ ty_sum maybe_as_trait_ref '>' /*23L*/ MOD_SEP /*10P*/ ident maybe_as_trait_ref '>' /*23L*/ MOD_SEP /*10P*/ ident
	| '(' /*32P*/ ty_sums ')'
	| '(' /*32P*/ ty_sums ',' ')'
	| '(' /*32P*/ ')'
	;

ty_prim :
	path_generic_args_without_colons %prec IDENT /*5P*/
	| MOD_SEP /*10P*/ path_generic_args_without_colons %prec IDENT /*5P*/
	| SELF /*3P*/ MOD_SEP /*10P*/ path_generic_args_without_colons %prec IDENT /*5P*/
	| path_generic_args_without_colons '!' /*31P*/ maybe_ident delimited_token_trees %prec IDENT /*5P*/
	| MOD_SEP /*10P*/ path_generic_args_without_colons '!' /*31P*/ maybe_ident delimited_token_trees %prec IDENT /*5P*/
	| BOX /*15P*/ ty
	| '*' /*30L*/ maybe_mut_or_const ty
	| '&' /*26L*/ ty
	| '&' /*26L*/ MUT /*4P*/ ty
	| ANDAND /*21L*/ ty
	| ANDAND /*21L*/ MUT /*4P*/ ty
	| '&' /*26L*/ lifetime maybe_mut ty
	| ANDAND /*21L*/ lifetime maybe_mut ty
	| '[' /*32P*/ ty ']'
	| '[' /*32P*/ ty ',' DOTDOT /*16N*/ expr ']'
	| '[' /*32P*/ ty ';' expr ']'
	| TYPEOF '(' /*32P*/ expr ')'
	| UNDERSCORE
	| ty_bare_fn
	| for_in_type
	;

ty_bare_fn :
	FN ty_fn_decl
	| UNSAFE FN ty_fn_decl
	| EXTERN maybe_abi FN ty_fn_decl
	| UNSAFE EXTERN maybe_abi FN ty_fn_decl
	;

ty_fn_decl :
	generic_params fn_anon_params ret_ty
	;

ty_closure :
	UNSAFE '|' /*24L*/ anon_params '|' /*24L*/ maybe_bounds ret_ty
	| '|' /*24L*/ anon_params '|' /*24L*/ maybe_bounds ret_ty
	| UNSAFE OROR /*20L*/ maybe_bounds ret_ty
	| OROR /*20L*/ maybe_bounds ret_ty
	;

for_in_type :
	FOR /*13P*/ '<' /*23L*/ maybe_lifetimes '>' /*23L*/ for_in_type_suffix
	;

for_in_type_suffix :
	ty_bare_fn
	| trait_ref
	| ty_closure
	;

maybe_mut :
	MUT /*4P*/
	| %prec MUT /*4P*/ /*empty*/
	;

maybe_mut_or_const :
	MUT /*4P*/
	| CONST
	| /*empty*/
	;

ty_qualified_path_and_generic_values :
	ty_qualified_path maybe_bindings
	| ty_qualified_path ',' ty_sums maybe_bindings
	;

ty_qualified_path :
	ty_sum AS /*29P*/ trait_ref '>' /*23L*/ MOD_SEP /*10P*/ ident
	| ty_sum AS /*29P*/ trait_ref '>' /*23L*/ MOD_SEP /*10P*/ ident '+' /*28L*/ ty_param_bounds
	;

maybe_ty_sums :
	ty_sums
	| ty_sums ','
	| /*empty*/
	;

ty_sums :
	ty_sum
	| ty_sums ',' ty_sum
	;

ty_sum :
	ty_sum_elt
	| ty_sum '+' /*28L*/ ty_sum_elt
	;

ty_sum_elt :
	ty
	| lifetime
	;

ty_prim_sum :
	ty_prim_sum_elt
	| ty_prim_sum '+' /*28L*/ ty_prim_sum_elt
	;

ty_prim_sum_elt :
	ty_prim
	| lifetime
	;

maybe_ty_param_bounds :
	':' /*11P*/ ty_param_bounds
	| /*empty*/
	;

ty_param_bounds :
	boundseq
	| /*empty*/
	;

boundseq :
	polybound
	| boundseq '+' /*28L*/ polybound
	;

polybound :
	FOR /*13P*/ '<' /*23L*/ maybe_lifetimes '>' /*23L*/ bound
	| bound
	| '?' /*14P*/ FOR /*13P*/ '<' /*23L*/ maybe_lifetimes '>' /*23L*/ bound
	| '?' /*14P*/ bound
	;

bindings :
	binding
	| bindings ',' binding
	;

binding :
	ident '=' /*18R*/ ty
	;

ty_param :
	maybe_const ident maybe_ty_param_bounds maybe_ty_default
	| maybe_const ident '?' /*14P*/ ident maybe_ty_param_bounds maybe_ty_default
	;

maybe_const :
	CONST
	| /*empty*/
	;

maybe_bounds :
	':' /*11P*/ bounds %prec SHIFTPLUS /*9P*/
	| %prec SHIFTPLUS /*9P*/ /*empty*/
	;

bounds :
	bound
	| bounds '+' /*28L*/ bound
	;

bound :
	lifetime
	| trait_ref
	;

maybe_ltbounds :
	':' /*11P*/ ltbounds %prec SHIFTPLUS /*9P*/
	| /*empty*/
	;

ltbounds :
	lifetime
	| ltbounds '+' /*28L*/ lifetime
	;

maybe_ty_default :
	'=' /*18R*/ ty_sum
	| /*empty*/
	;

maybe_lifetimes :
	lifetimes
	| lifetimes ','
	| /*empty*/
	;

lifetimes :
	lifetime_and_bounds
	| lifetimes ',' lifetime_and_bounds
	;

lifetime_and_bounds :
	LIFETIME maybe_ltbounds
	| STATIC_LIFETIME
	;

lifetime :
	LIFETIME
	| STATIC_LIFETIME
	;

trait_ref :
	path_generic_args_without_colons %prec IDENT /*5P*/
	| MOD_SEP /*10P*/ path_generic_args_without_colons %prec IDENT /*5P*/
	;

inner_attrs_and_block :
	'{' /*32P*/ maybe_inner_attrs maybe_stmts '}'
	;

block :
	'{' /*32P*/ maybe_stmts '}'
	;

maybe_stmts :
	stmts
	| stmts nonblock_expr
	| nonblock_expr
	| /*empty*/
	;

stmts :
	stmt
	| stmts stmt
	;

stmt :
	maybe_outer_attrs let
	| stmt_item
	| PUB stmt_item
	| outer_attrs stmt_item
	| outer_attrs PUB stmt_item
	| full_block_expr
	| maybe_outer_attrs block
	| nonblock_expr ';'
	| outer_attrs nonblock_expr ';'
	| ';'
	;

maybe_exprs :
	exprs
	| exprs ','
	| /*empty*/
	;

maybe_expr :
	expr
	| /*empty*/
	;

exprs :
	expr
	| exprs ',' expr
	;

path_expr :
	path_generic_args_with_colons
	| MOD_SEP /*10P*/ path_generic_args_with_colons
	| SELF /*3P*/ MOD_SEP /*10P*/ path_generic_args_with_colons
	;

path_generic_args_with_colons :
	ident
	| SUPER
	| path_generic_args_with_colons MOD_SEP /*10P*/ ident
	| path_generic_args_with_colons MOD_SEP /*10P*/ SUPER
	| path_generic_args_with_colons MOD_SEP /*10P*/ generic_args
	;

macro_expr :
	path_expr '!' /*31P*/ maybe_ident parens_delimited_token_trees
	| path_expr '!' /*31P*/ maybe_ident brackets_delimited_token_trees
	;

nonblock_expr :
	lit
	| path_expr %prec IDENT /*5P*/
	| SELF /*3P*/
	| macro_expr
	| path_expr '{' /*32P*/ struct_expr_fields '}'
	| nonblock_expr '?' /*14P*/
	| nonblock_expr '.' /*32P*/ path_generic_args_with_colons
	| nonblock_expr '.' /*32P*/ LIT_INTEGER
	| nonblock_expr '[' /*32P*/ maybe_expr ']'
	| nonblock_expr '(' /*32P*/ maybe_exprs ')'
	| '[' /*32P*/ vec_expr ']'
	| '(' /*32P*/ maybe_exprs ')'
	| CONTINUE
	| CONTINUE lifetime
	| RETURN /*17P*/
	| RETURN /*17P*/ expr
	| BREAK
	| BREAK lifetime
	| YIELD /*17P*/
	| YIELD /*17P*/ expr
	| nonblock_expr LARROW /*19R*/ expr
	| nonblock_expr '=' /*18R*/ expr
	| nonblock_expr SHLEQ /*18R*/ expr
	| nonblock_expr SHREQ /*18R*/ expr
	| nonblock_expr MINUSEQ /*18R*/ expr
	| nonblock_expr ANDEQ /*18R*/ expr
	| nonblock_expr OREQ /*18R*/ expr
	| nonblock_expr PLUSEQ /*18R*/ expr
	| nonblock_expr STAREQ /*18R*/ expr
	| nonblock_expr SLASHEQ /*18R*/ expr
	| nonblock_expr CARETEQ /*18R*/ expr
	| nonblock_expr PERCENTEQ /*18R*/ expr
	| nonblock_expr OROR /*20L*/ expr
	| nonblock_expr ANDAND /*21L*/ expr
	| nonblock_expr EQEQ /*22L*/ expr
	| nonblock_expr NE /*22L*/ expr
	| nonblock_expr '<' /*23L*/ expr
	| nonblock_expr '>' /*23L*/ expr
	| nonblock_expr LE /*23L*/ expr
	| nonblock_expr GE /*23L*/ expr
	| nonblock_expr '|' /*24L*/ expr
	| nonblock_expr '^' /*25L*/ expr
	| nonblock_expr '&' /*26L*/ expr
	| nonblock_expr SHL /*27L*/ expr
	| nonblock_expr SHR /*27L*/ expr
	| nonblock_expr '+' /*28L*/ expr
	| nonblock_expr '-' /*28L*/ expr
	| nonblock_expr '*' /*30L*/ expr
	| nonblock_expr '/' /*30L*/ expr
	| nonblock_expr '%' /*30L*/ expr
	| nonblock_expr DOTDOT /*16N*/
	| nonblock_expr DOTDOT /*16N*/ expr
	| DOTDOT /*16N*/ expr
	| DOTDOT /*16N*/
	| nonblock_expr AS /*29P*/ ty
	| nonblock_expr ':' /*11P*/ ty
	| BOX /*15P*/ expr
	| expr_qualified_path
	| nonblock_prefix_expr
	;

expr :
	lit
	| path_expr %prec IDENT /*5P*/
	| SELF /*3P*/
	| macro_expr
	| path_expr '{' /*32P*/ struct_expr_fields '}'
	| expr '?' /*14P*/
	| expr '.' /*32P*/ path_generic_args_with_colons
	| expr '.' /*32P*/ LIT_INTEGER
	| expr '[' /*32P*/ maybe_expr ']'
	| expr '(' /*32P*/ maybe_exprs ')'
	| '(' /*32P*/ maybe_exprs ')'
	| '[' /*32P*/ vec_expr ']'
	| CONTINUE
	| CONTINUE ident
	| RETURN /*17P*/
	| RETURN /*17P*/ expr
	| BREAK
	| BREAK ident
	| YIELD /*17P*/
	| YIELD /*17P*/ expr
	| expr LARROW /*19R*/ expr
	| expr '=' /*18R*/ expr
	| expr SHLEQ /*18R*/ expr
	| expr SHREQ /*18R*/ expr
	| expr MINUSEQ /*18R*/ expr
	| expr ANDEQ /*18R*/ expr
	| expr OREQ /*18R*/ expr
	| expr PLUSEQ /*18R*/ expr
	| expr STAREQ /*18R*/ expr
	| expr SLASHEQ /*18R*/ expr
	| expr CARETEQ /*18R*/ expr
	| expr PERCENTEQ /*18R*/ expr
	| expr OROR /*20L*/ expr
	| expr ANDAND /*21L*/ expr
	| expr EQEQ /*22L*/ expr
	| expr NE /*22L*/ expr
	| expr '<' /*23L*/ expr
	| expr '>' /*23L*/ expr
	| expr LE /*23L*/ expr
	| expr GE /*23L*/ expr
	| expr '|' /*24L*/ expr
	| expr '^' /*25L*/ expr
	| expr '&' /*26L*/ expr
	| expr SHL /*27L*/ expr
	| expr SHR /*27L*/ expr
	| expr '+' /*28L*/ expr
	| expr '-' /*28L*/ expr
	| expr '*' /*30L*/ expr
	| expr '/' /*30L*/ expr
	| expr '%' /*30L*/ expr
	| expr DOTDOT /*16N*/
	| expr DOTDOT /*16N*/ expr
	| DOTDOT /*16N*/ expr
	| DOTDOT /*16N*/
	| expr AS /*29P*/ ty
	| expr ':' /*11P*/ ty
	| BOX /*15P*/ expr
	| expr_qualified_path
	| block_expr
	| block
	| nonblock_prefix_expr
	;

expr_nostruct :
	lit
	| path_expr %prec IDENT /*5P*/
	| SELF /*3P*/
	| macro_expr
	| expr_nostruct '?' /*14P*/
	| expr_nostruct '.' /*32P*/ path_generic_args_with_colons
	| expr_nostruct '.' /*32P*/ LIT_INTEGER
	| expr_nostruct '[' /*32P*/ maybe_expr ']'
	| expr_nostruct '(' /*32P*/ maybe_exprs ')'
	| '[' /*32P*/ vec_expr ']'
	| '(' /*32P*/ maybe_exprs ')'
	| CONTINUE
	| CONTINUE ident
	| RETURN /*17P*/
	| RETURN /*17P*/ expr
	| BREAK
	| BREAK ident
	| YIELD /*17P*/
	| YIELD /*17P*/ expr
	| expr_nostruct LARROW /*19R*/ expr_nostruct
	| expr_nostruct '=' /*18R*/ expr_nostruct
	| expr_nostruct SHLEQ /*18R*/ expr_nostruct
	| expr_nostruct SHREQ /*18R*/ expr_nostruct
	| expr_nostruct MINUSEQ /*18R*/ expr_nostruct
	| expr_nostruct ANDEQ /*18R*/ expr_nostruct
	| expr_nostruct OREQ /*18R*/ expr_nostruct
	| expr_nostruct PLUSEQ /*18R*/ expr_nostruct
	| expr_nostruct STAREQ /*18R*/ expr_nostruct
	| expr_nostruct SLASHEQ /*18R*/ expr_nostruct
	| expr_nostruct CARETEQ /*18R*/ expr_nostruct
	| expr_nostruct PERCENTEQ /*18R*/ expr_nostruct
	| expr_nostruct OROR /*20L*/ expr_nostruct
	| expr_nostruct ANDAND /*21L*/ expr_nostruct
	| expr_nostruct EQEQ /*22L*/ expr_nostruct
	| expr_nostruct NE /*22L*/ expr_nostruct
	| expr_nostruct '<' /*23L*/ expr_nostruct
	| expr_nostruct '>' /*23L*/ expr_nostruct
	| expr_nostruct LE /*23L*/ expr_nostruct
	| expr_nostruct GE /*23L*/ expr_nostruct
	| expr_nostruct '|' /*24L*/ expr_nostruct
	| expr_nostruct '^' /*25L*/ expr_nostruct
	| expr_nostruct '&' /*26L*/ expr_nostruct
	| expr_nostruct SHL /*27L*/ expr_nostruct
	| expr_nostruct SHR /*27L*/ expr_nostruct
	| expr_nostruct '+' /*28L*/ expr_nostruct
	| expr_nostruct '-' /*28L*/ expr_nostruct
	| expr_nostruct '*' /*30L*/ expr_nostruct
	| expr_nostruct '/' /*30L*/ expr_nostruct
	| expr_nostruct '%' /*30L*/ expr_nostruct
	| expr_nostruct DOTDOT /*16N*/ %prec RANGE /*33P*/
	| expr_nostruct DOTDOT /*16N*/ expr_nostruct
	| DOTDOT /*16N*/ expr_nostruct
	| DOTDOT /*16N*/
	| expr_nostruct AS /*29P*/ ty
	| expr_nostruct ':' /*11P*/ ty
	| BOX /*15P*/ expr
	| expr_qualified_path
	| block_expr
	| block
	| nonblock_prefix_expr_nostruct
	;

nonblock_prefix_expr_nostruct :
	'-' /*28L*/ expr_nostruct
	| '!' /*31P*/ expr_nostruct
	| '*' /*30L*/ expr_nostruct
	| '&' /*26L*/ maybe_mut expr_nostruct
	| ANDAND /*21L*/ maybe_mut expr_nostruct
	| lambda_expr_nostruct
	| MOVE lambda_expr_nostruct
	;

nonblock_prefix_expr :
	'-' /*28L*/ expr
	| '!' /*31P*/ expr
	| '*' /*30L*/ expr
	| '&' /*26L*/ maybe_mut expr
	| ANDAND /*21L*/ maybe_mut expr
	| lambda_expr
	| MOVE lambda_expr
	;

expr_qualified_path :
	'<' /*23L*/ ty_sum maybe_as_trait_ref '>' /*23L*/ MOD_SEP /*10P*/ ident maybe_qpath_params
	| SHL /*27L*/ ty_sum maybe_as_trait_ref '>' /*23L*/ MOD_SEP /*10P*/ ident maybe_as_trait_ref '>' /*23L*/ MOD_SEP /*10P*/ ident
	| SHL /*27L*/ ty_sum maybe_as_trait_ref '>' /*23L*/ MOD_SEP /*10P*/ ident generic_args maybe_as_trait_ref '>' /*23L*/ MOD_SEP /*10P*/ ident
	| SHL /*27L*/ ty_sum maybe_as_trait_ref '>' /*23L*/ MOD_SEP /*10P*/ ident maybe_as_trait_ref '>' /*23L*/ MOD_SEP /*10P*/ ident generic_args
	| SHL /*27L*/ ty_sum maybe_as_trait_ref '>' /*23L*/ MOD_SEP /*10P*/ ident generic_args maybe_as_trait_ref '>' /*23L*/ MOD_SEP /*10P*/ ident generic_args
	;

maybe_qpath_params :
	MOD_SEP /*10P*/ generic_args
	| /*empty*/
	;

maybe_as_trait_ref :
	AS /*29P*/ trait_ref
	| /*empty*/
	;

lambda_expr :
	OROR /*20L*/ ret_ty expr %prec LAMBDA /*2P*/
	| '|' /*24L*/ '|' /*24L*/ ret_ty expr %prec LAMBDA /*2P*/
	| '|' /*24L*/ inferrable_params '|' /*24L*/ ret_ty expr %prec LAMBDA /*2P*/
	| '|' /*24L*/ inferrable_params OROR /*20L*/ lambda_expr_no_first_bar %prec LAMBDA /*2P*/
	;

lambda_expr_no_first_bar :
	'|' /*24L*/ ret_ty expr %prec LAMBDA /*2P*/
	| inferrable_params '|' /*24L*/ ret_ty expr %prec LAMBDA /*2P*/
	| inferrable_params OROR /*20L*/ lambda_expr_no_first_bar %prec LAMBDA /*2P*/
	;

lambda_expr_nostruct :
	OROR /*20L*/ expr_nostruct %prec LAMBDA /*2P*/
	| '|' /*24L*/ '|' /*24L*/ ret_ty expr_nostruct %prec LAMBDA /*2P*/
	| '|' /*24L*/ inferrable_params '|' /*24L*/ expr_nostruct %prec LAMBDA /*2P*/
	| '|' /*24L*/ inferrable_params OROR /*20L*/ lambda_expr_nostruct_no_first_bar %prec LAMBDA /*2P*/
	;

lambda_expr_nostruct_no_first_bar :
	'|' /*24L*/ ret_ty expr_nostruct %prec LAMBDA /*2P*/
	| inferrable_params '|' /*24L*/ ret_ty expr_nostruct %prec LAMBDA /*2P*/
	| inferrable_params OROR /*20L*/ lambda_expr_nostruct_no_first_bar %prec LAMBDA /*2P*/
	;

vec_expr :
	maybe_exprs
	| exprs ';' expr
	;

struct_expr_fields :
	field_inits
	| field_inits ','
	| maybe_field_inits default_field_init
	| /*empty*/
	;

maybe_field_inits :
	field_inits
	| field_inits ','
	| /*empty*/
	;

field_inits :
	field_init
	| field_inits ',' field_init
	;

field_init :
	ident
	| ident ':' /*11P*/ expr
	| LIT_INTEGER ':' /*11P*/ expr
	;

default_field_init :
	DOTDOT /*16N*/ expr
	;

block_expr :
	expr_match
	| expr_if
	| expr_if_let
	| expr_while
	| expr_while_let
	| expr_loop
	| expr_for
	| UNSAFE block
	| path_expr '!' /*31P*/ maybe_ident braces_delimited_token_trees
	;

full_block_expr :
	block_expr
	| block_expr_dot
	;

block_expr_dot :
	block_expr '.' /*32P*/ path_generic_args_with_colons %prec IDENT /*5P*/
	| block_expr_dot '.' /*32P*/ path_generic_args_with_colons %prec IDENT /*5P*/
	| block_expr '.' /*32P*/ path_generic_args_with_colons '[' /*32P*/ maybe_expr ']'
	| block_expr_dot '.' /*32P*/ path_generic_args_with_colons '[' /*32P*/ maybe_expr ']'
	| block_expr '.' /*32P*/ path_generic_args_with_colons '(' /*32P*/ maybe_exprs ')'
	| block_expr_dot '.' /*32P*/ path_generic_args_with_colons '(' /*32P*/ maybe_exprs ')'
	| block_expr '.' /*32P*/ LIT_INTEGER
	| block_expr_dot '.' /*32P*/ LIT_INTEGER
	;

expr_match :
	MATCH expr_nostruct '{' /*32P*/ '}'
	| MATCH expr_nostruct '{' /*32P*/ match_clauses '}'
	| MATCH expr_nostruct '{' /*32P*/ match_clauses nonblock_match_clause '}'
	| MATCH expr_nostruct '{' /*32P*/ nonblock_match_clause '}'
	;

match_clauses :
	match_clause
	| match_clauses match_clause
	;

match_clause :
	nonblock_match_clause ','
	| block_match_clause
	| block_match_clause ','
	;

nonblock_match_clause :
	maybe_outer_attrs pats_or maybe_guard FAT_ARROW nonblock_expr
	| maybe_outer_attrs pats_or maybe_guard FAT_ARROW block_expr_dot
	;

block_match_clause :
	maybe_outer_attrs pats_or maybe_guard FAT_ARROW block
	| maybe_outer_attrs pats_or maybe_guard FAT_ARROW block_expr
	;

maybe_guard :
	IF expr_nostruct
	| /*empty*/
	;

expr_if :
	IF expr_nostruct block
	| IF expr_nostruct block ELSE block_or_if
	;

expr_if_let :
	IF LET pat '=' /*18R*/ expr_nostruct block
	| IF LET pat '=' /*18R*/ expr_nostruct block ELSE block_or_if
	;

block_or_if :
	block
	| expr_if
	| expr_if_let
	;

expr_while :
	maybe_label WHILE expr_nostruct block
	;

expr_while_let :
	maybe_label WHILE LET pat '=' /*18R*/ expr_nostruct block
	;

expr_loop :
	maybe_label LOOP block
	;

expr_for :
	maybe_label FOR /*13P*/ pat IN expr_nostruct block
	;

maybe_label :
	lifetime ':' /*11P*/
	| /*empty*/
	;

let :
	LET pat maybe_ty_ascription maybe_init_expr ';'
	;

lit :
	LIT_BYTE
	| LIT_CHAR
	| LIT_INTEGER
	| LIT_FLOAT
	| TRUE
	| FALSE
	| str
	;

str :
	LIT_STR
	| LIT_STR_RAW
	| LIT_BYTE_STR
	| LIT_BYTE_STR_RAW
	;

maybe_ident :
	/*empty*/
	| ident
	;

ident :
	IDENT /*5P*/
	| CATCH /*6P*/
	| DEFAULT /*7P*/
	| UNION /*8P*/
	;

unpaired_token :
	SHL /*27L*/
	| SHR /*27L*/
	| LE /*23L*/
	| EQEQ /*22L*/
	| NE /*22L*/
	| GE /*23L*/
	| ANDAND /*21L*/
	| OROR /*20L*/
	| LARROW /*19R*/
	| SHLEQ /*18R*/
	| SHREQ /*18R*/
	| MINUSEQ /*18R*/
	| ANDEQ /*18R*/
	| OREQ /*18R*/
	| PLUSEQ /*18R*/
	| STAREQ /*18R*/
	| SLASHEQ /*18R*/
	| CARETEQ /*18R*/
	| PERCENTEQ /*18R*/
	| DOTDOT /*16N*/
	| DOTDOTDOT
	| MOD_SEP /*10P*/
	| RARROW /*11P*/
	| FAT_ARROW
	| LIT_BYTE
	| LIT_CHAR
	| LIT_INTEGER
	| LIT_FLOAT
	| LIT_STR
	| LIT_STR_RAW
	| LIT_BYTE_STR
	| LIT_BYTE_STR_RAW
	| IDENT /*5P*/
	| UNDERSCORE
	| LIFETIME
	| SELF /*3P*/
	| STATIC
	| ABSTRACT
	| ALIGNOF
	| AS /*29P*/
	| BECOME
	| BREAK
	| CATCH /*6P*/
	| CRATE
	| DEFAULT /*7P*/
	| DO
	| ELSE
	| ENUM
	| EXTERN
	| FALSE
	| FINAL
	| FN
	| FOR /*13P*/
	| IF
	| IMPL
	| IN
	| LET
	| LOOP
	| MACRO
	| MATCH
	| MOD
	| MOVE
	| MUT /*4P*/
	| OFFSETOF
	| OVERRIDE
	| PRIV
	| PUB
	| PURE
	| REF
	| RETURN /*17P*/
	| STRUCT
	| SIZEOF
	| SUPER
	| TRUE
	| TRAIT
	| TYPE
	| UNION /*8P*/
	| UNSAFE
	| UNSIZED
	| USE
	| VIRTUAL
	| WHILE
	| YIELD /*17P*/
	| CONTINUE
	| PROC
	| BOX /*15P*/
	| CONST
	| WHERE
	| TYPEOF
	| INNER_DOC_COMMENT
	| OUTER_DOC_COMMENT
	| SHEBANG
	| STATIC_LIFETIME
	| ';'
	| ','
	| '.' /*32P*/
	| '@'
	| '#'
	| '~'
	| ':' /*11P*/
	| '$'
	| '=' /*18R*/
	| '?' /*14P*/
	| '!' /*31P*/
	| '<' /*23L*/
	| '>' /*23L*/
	| '-' /*28L*/
	| '&' /*26L*/
	| '|' /*24L*/
	| '+' /*28L*/
	| '*' /*30L*/
	| '/' /*30L*/
	| '^' /*25L*/
	| '%' /*30L*/
	;

token_trees :
	/*empty*/
	| token_trees token_tree
	;

token_tree :
	delimited_token_trees
	| unpaired_token
	;

delimited_token_trees :
	parens_delimited_token_trees
	| braces_delimited_token_trees
	| brackets_delimited_token_trees
	;

parens_delimited_token_trees :
	'(' /*32P*/ token_trees ')'
	;

braces_delimited_token_trees :
	'{' /*32P*/ token_trees '}'
	;

brackets_delimited_token_trees :
	'[' /*32P*/ token_trees ']'
	;

//Lexer


SELF : 'self' ;
CATCH : 'catch' ;
DEFAULT : 'default' ;
UNION : 'union' ;
MOD_SEP : '::' ;
RARROW : '->' ;
FOR : 'for' ;
BOX : 'box' ;
DOTDOT : '..' ;
RETURN : 'return' ;
YIELD : 'yield' ;
SHLEQ : '<<=' ;
SHREQ : '>>=' ;
MINUSEQ : '-=' ;
ANDEQ : '&=' ;
OREQ : '|=' ;
PLUSEQ : '+=' ;
STAREQ : '*=' ;
SLASHEQ : '/=' ;
CARETEQ : '^=' ;
PERCENTEQ : '%=' ;
LARROW : '<-' ;
OROR : '||' ;
ANDAND : '&&' ;
EQEQ : '==' ;
NE : '!=' ;
LE : '<=' ;
GE : '>=' ;
SHL : '<<' ;
SHR : '>>' ;
AS : 'as' ;
SHEBANG : '#!' ;
SHEBANG_LINE : "#![^\n]+" ;
INNER_DOC_COMMENT : "///[^\n]*" ;
OUTER_DOC_COMMENT : "//![^\n]*" ;
STATIC : 'static' ;
CONST : 'const' ;
EXTERN : 'extern' ;
CRATE : 'crate' ;
USE : 'use' ;
STRUCT : 'struct' ;
ENUM : 'enum' ;
MOD : 'mod' ;
UNSAFE : 'unsafe' ;
TYPE : 'type' ;
FN : 'fn' ;
DOTDOTDOT : '...' ;
PUB : 'pub' ;
SUPER : 'super' ;
TRAIT : 'trait' ;
IMPL : 'impl' ;
UNDERSCORE : '_' ;
WHERE : 'where' ;
REF : 'ref' ;
LIT_INTEGER : "0x[0-9a-fA-F_]+|0o[0-7_]+|0b[01_]+|[0-9][0-9_]*|[0-9][0-9_]*\.(\.|[a-zA-Z])" ;
TYPEOF : 'typeof' ;
LIFETIME : '\''  IDENT ;
STATIC_LIFETIME : '\'static' ;
CONTINUE : 'continue' ;
BREAK : 'break' ;
MOVE : 'move' ;
MATCH : 'match' ;
FAT_ARROW : '=>' ;
IF : 'if' ;
ELSE : 'else' ;
LET : 'let' ;
WHILE : 'while' ;
LOOP : 'loop' ;
IN : 'in' ;
LIT_BYTE : "b'([^'\n]|\\[^\n])'" ;
LIT_CHAR : "'([^'\n]|\\[^\n])'" ;
LIT_FLOAT : "[0-9][0-9_]*\.[0-9_]*([eE][-\+]?[0-9_]+)?|[0-9][0-9_]*(\.[0-9_]*)?[eE][-\+]?[0-9_]+" ;
TRUE : 'true' ;
FALSE : 'false' ;
LIT_STR : "\"([^\"\n]|\\[^\n])*\"" ;
LIT_STR_RAW : 'LIT_STR_RAW' ;
LIT_BYTE_STR : 'LIT_BYTE_STR' ;
LIT_BYTE_STR_RAW : 'LIT_BYTE_STR_RAW' ;
ABSTRACT : 'abstract' ;
ALIGNOF : 'alignof' ;
BECOME : 'become' ;
DO : 'do' ;
FINAL : 'final' ;
MACRO : 'macro' ;
OFFSETOF : 'offsetof' ;
OVERRIDE : 'override' ;
PRIV : 'priv' ;
PURE : 'pure' ;
SIZEOF : 'sizeof' ;
UNSIZED : 'unsized' ;
VIRTUAL : 'virtual' ;
PROC : 'proc' ;
MUT : 'mut' ;  //When commenting this we get lots of shift/reduce conflicts instead of a missing definition
IDENT : "[a-zA-Z\x80-\xff_][a-zA-Z0-9\x80-\xff_]*" ;

//
// TODO: parse generic parameter that end in ">>"
//

}
