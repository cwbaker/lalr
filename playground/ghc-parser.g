ghc {

%whitespace "[ \t\r\n]*";
%whitespace "//[^\n]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

/*
%token
 '_'                        //-- Haskell keywords
 'as'
 'case'
 'class'
 'data'
 'default'
 'deriving'
 'else'
 'hiding'
 'if'
 'import'
 'in'
 'infix'
 'infixl'
 'infixr'
 'instance'
 'let'
 'module'
 'newtype'
 'of'
 'qualified'
 'then'
 'type'
 'where'

 'forall'                       //-- GHC extension keywords
 'foreign'
 'export'
 'label'
 'dynamic'
 'safe'
 'interruptible'
 'unsafe'
 'family'
 'role'
 'stdcall'
 'ccall'
 'capi'
 'prim'
 'javascript'
 'proc'                   //-- for arrow notation extension
 'rec'                     //-- for arrow notation extension
 'group'         //-- for list transform extension
 'by'               //-- for list transform extension
 'using'         //-- for list transform extension
 'pattern'       //-- for pattern synonyms
 'static'         //-- for static pointers extension
 'stock'            //-- for DerivingStrategies extension
 'anyclass'      //-- for DerivingStrategies extension
 'via'                //-- for DerivingStrategies extension

 'unit'
 'signature'
 'dependency'

 '{-# INLINE'              //-- INLINE or INLINABLE
 '{-# OPAQUE'
 '{-# SPECIALISE'
 '{-# SPECIALISE_INLINE'
 '{-# SOURCE'
 '{-# RULES'
 '{-# SCC'
 '{-# DEPRECATED'
 '{-# WARNING'
 '{-# UNPACK'
 '{-# NOUNPACK'
 '{-# ANN'
 '{-# MINIMAL'
 '{-# CTYPE'
 '{-# OVERLAPPING'
 '{-# OVERLAPPABLE'
 '{-# OVERLAPS'
 '{-# INCOHERENT'
 '{-# COMPLETE'
 '#-}'

 '..'                                   //-- reserved symbols
 ':'
 '::'
 '='
 '\\'
 'lcase'
 'lcases'
 '|'
 '<-'
 '->'
 '->.'
 TIGHT_INFIX_AT
 '=>'
 '-'
 PREFIX_TILDE
 PREFIX_BANG
 PREFIX_MINUS
 '*'
 '-<'                       //-- for arrow notation
 '>-'                       //-- for arrow notation
 '-<<'                      //-- for arrow notation
 '>>-'                      //-- for arrow notation
 '.'
 PREFIX_PROJ                   //-- RecordDotSyntax
 TIGHT_INFIX_PROJ             //-- RecordDotSyntax
 PREFIX_AT
 PREFIX_PERCENT                    //-- for linear types

 '{'                                    //-- special symbols
 '}'
 vocurly         //-- virtual open curly (from layout)
 vccurly         //-- virtual close curly (from layout)
 '['
 ']'
 '('
 ')'
 '(#'
 '#)'
 '(|'
 '|)'
 ';'
 ','
 '`'
 SIMPLEQUOTE         //-- 'x

 VARID                    //-- identifiers
 CONID
 VARSYM
 CONSYM
 QVARID
 QCONID
 QVARSYM
 QCONSYM


 //-- QualifiedDo
 DO
 MDO

 IPDUPVARID                   //-- GHC extension
 LABELVARID

 CHAR
 STRING
 INTEGER
 RATIONAL

 PRIMCHAR
 PRIMSTRING
 PRIMINTEGER
 PRIMWORD
 PRIMINTEGER8
 PRIMINTEGER16
 PRIMINTEGER32
 PRIMINTEGER64
 PRIMWORD8
 PRIMWORD16
 PRIMWORD32
 PRIMWORD64
 PRIMFLOAT
 PRIMDOUBLE

//-- Template Haskell
'[|'
'[p|'
'[t|'
'[d|'
'|]'
'[||'
'||]'
PREFIX_DOLLAR
PREFIX_DOLLAR_DOLLAR
TH_TY_QUOTE           //-- ''T
TH_QUASIQUOTE
TH_QQUASIQUOTE

%monad
%lexer
  //-- Replace 'lexer' above with 'lexerDbg'
  //-- to dump the tokens fed to the parser.
%tokentype

//-- Exported parsers
%name parseModuleNoHaddock module
%name parseSignature signature
%name parseImport importdecl
%name parseStatement e_stmt
%name parseDeclaration topdecl
%name parseExpression exp
%name parsePattern pat
%name parseTypeSignature sigdecl
%name parseStmt   maybe_stmt
%name parseIdentifier  identifier
%name parseType ktype
%name parseBackpack backpack
%partial parseHeader header
%%
*/

GHC_grammar :
    module
    | signature
    | importdecl
    | e_stmt
    | topdecl
    | exp
    | pat
    | sigdecl
    | maybe_stmt
    | identifier
    | ktype
    | backpack
    | header
    ;

//-----------------------------------------------------------------------------
//-- Identifiers; one of the entry points
identifier :
        qvar
        | qcon
        | qvarop
        | qconop
    | '(' '->' ')'
    | '->'
	;
//-----------------------------------------------------------------------------
//-- Backpack stuff

backpack :
         implicit_top units close
         | '{' units '}'
	;
units :
         units ';' unit
         | units ';'
         | unit
	;
unit :
        'unit' pkgname 'where' unitbody
    ;

unitid :
        pkgname
        | pkgname '[' msubsts ']'
	;
msubsts :
        msubsts ',' msubst
        | msubsts ','
        | msubst
	;
msubst :
        modid '=' moduleid
        | modid VARSYM modid VARSYM
	;
moduleid :
          VARSYM modid VARSYM
          | unitid ':' modid
	;
pkgname :
        STRING
        | litpkgname
	;
litpkgname_segment :
        VARID
        | CONID
        | special_id
	;
//-- Parse a minus sign regardless of whether -XLexicalNegation is turned on or off.
//-- See Note [Minus tokens] in GHC.Parser.Lexer
HYPHEN :
      '-'
      | PREFIX_MINUS
      | VARSYM
	;

litpkgname :
        litpkgname_segment
        //-- a bit of a hack, means p - b is parsed same as p-b, enough for now.
        | litpkgname_segment HYPHEN litpkgname
	;
mayberns :
        /*empty*/
        | '(' rns ')'
	;
rns :
        rns ',' rn
        | rns ','
        | rn
	;
rn :
        modid 'as' modid
        | modid
	;
unitbody :
        '{'     unitdecls '}'
        | vocurly unitdecls close
	;
unitdecls :
        unitdecls ';' unitdecl
        | unitdecls ';'
        | unitdecl
	;
unitdecl :
        'module' maybe_src modid maybemodwarning maybeexports 'where' body
             //-- XXX not accurate

        | 'signature' modid maybemodwarning maybeexports 'where' body

        | 'dependency' unitid mayberns

        | 'dependency' 'signature' unitid
    ;

//-----------------------------------------------------------------------------
//-- Module Header

//-- The place for module deprecation is really too restrictive, but if it
//-- was allowed at its natural place just before 'module', we get an ugly
//-- s/r conflict with the second alternative. Another solution would be the
//-- introduction of a new pragma DEPRECATED_MODULE, but this is not very nice,
//-- either, and DEPRECATED is only expected to be used by people who really
//-- know what they are doing. :-)

signature :
       'signature' modid maybemodwarning maybeexports 'where' body
    ;

module :
       'module' modid maybemodwarning maybeexports 'where' body

        | body2
    ;

missing_module_keyword :
        /*empty*/
    ;

implicit_top :
        /*empty*/
    ;

maybemodwarning :
    '{-# DEPRECATED' strings '#-}'

    | '{-# WARNING' warning_category strings '#-}'

    |  /*empty*/
	;
body    :
        '{'            top '}'
        |      vocurly    top close
	;
body2   :
        '{' top '}'
        |  missing_module_keyword top close
	;

top     :
        semis top1
    ;

top1    :
        importdecls_semi topdecls_cs_semi
        | importdecls_semi topdecls_cs
        | importdecls
	;
//-----------------------------------------------------------------------------
//-- Module declaration & imports only

header  :
        'module' modid maybemodwarning maybeexports 'where' header_body

        | 'signature' modid maybemodwarning maybeexports 'where' header_body

        | header_body2
    ;

header_body :
        '{'            header_top
        |      vocurly    header_top
	;
header_body2 :
        '{' header_top
        |  missing_module_keyword header_top
	;
header_top :
        semis header_top_importdecls
    ;
header_top_importdecls :
        importdecls_semi
        |  importdecls
	;
//-----------------------------------------------------------------------------
//-- The Export List

maybeexports :
        '(' exportlist ')'
        |  /*empty*/
	;
exportlist :
        exportlist1
        | /*empty*/

        //-- trailing comma:
        | exportlist1 ','
        | ','
	;
exportlist1 :
        exportlist1 ',' export

        | export
	;

   //-- No longer allow things like [] and (,,,) to be exported
   //-- They are built in syntax, always available
export  :
        maybeexportwarning qcname_ext export_subspec
        | maybeexportwarning 'module' modid
        | maybeexportwarning 'pattern' qcon
	;
maybeexportwarning :
        '{-# DEPRECATED' strings '#-}'

        | '{-# WARNING' warning_category strings '#-}'

        |  /*empty*/
	;
export_subspec :
        /*empty*/
        | '(' qcnames ')'
	;
qcnames :
  /*empty*/
  | qcnames1
	;
qcnames1 :     //-- A reversed list
        qcnames1 ',' qcname_ext_w_wildcard

        //-- Annotations re-added in mkImpExpSubSpec
        |  qcname_ext_w_wildcard
	;
//-- Variable, data constructor or wildcard
//-- or tagged type constructor
qcname_ext_w_wildcard :
        qcname_ext
        |  '..'
	;
qcname_ext :
        qcname
        |  'type' oqtycon
	;
qcname  :  //-- Variable or type constructor
        qvar                  //-- Things which look like functions
                                       //-- Note: This includes record selectors but
                                       //-- also (-.->), see #11432
        |  oqtycon_no_varcon     //-- see Note [Type constructors in export list]
	;
//-----------------------------------------------------------------------------
//-- Import Declarations

//-- importdecls and topdecls must contain at least one declaration;
//-- top handles the fact that these may be optional.

//-- One or more semicolons
semis1  :
	semis1 ';'
        | ';'
	;
//-- Zero or more semicolons
semis   :
	semis ';'
        | /*empty*/
	;
//-- No trailing semicolons, non-empty
importdecls :
importdecls
        importdecls_semi importdecl
    ;

//-- May have trailing semicolons, can be empty
importdecls_semi :
importdecls_semi
        importdecls_semi importdecl semis1

        | /*empty*/
	;
importdecl :
        'import' maybe_src maybe_safe optqualified maybe_pkg modid optqualified maybeas maybeimpspec
    ;


maybe_src :
        '{-# SOURCE' '#-}'
        | /*empty*/
	;
maybe_safe :
        'safe'
        | /*empty*/
	;
maybe_pkg :
        STRING
        | /*empty*/
	;
optqualified :
        'qualified'
        | /*empty*/
	;
maybeas :
        'as' modid
        | /*empty*/
	;
maybeimpspec :
        impspec
        | /*empty*/
	;
impspec :
        '(' importlist ')'
        |  'hiding' '(' importlist ')'
	;
importlist :
        importlist1
        | /*empty*/
        //-- trailing comma:
        | importlist1 ','
        | ','
	;
importlist1 :
        importlist1 ',' import

        | import
	;
import  :
        qcname_ext export_subspec
        | 'module' modid
        | 'pattern' qcon
	;
//-----------------------------------------------------------------------------
//-- Fixity Declarations

prec    :
        /*empty*/
        | INTEGER
    ;

infix   :
        'infix'
        | 'infixl'
        | 'infixr'
	;
ops     :
        ops ',' op
        | op
	;
//-----------------------------------------------------------------------------
//-- Top-Level Declarations

//-- No trailing semicolons, non-empty
topdecls :
        topdecls_semi topdecl
    ;
//-- May have trailing semicolons, can be empty
topdecls_semi :
        topdecls_semi topdecl semis1
        | /*empty*/
	;

//-----------------------------------------------------------------------------
//-- Each topdecl accumulates prior comments
//-- No trailing semicolons, non-empty
topdecls_cs :
        topdecls_cs_semi topdecl_cs
    ;
//-- May have trailing semicolons, can be empty
topdecls_cs_semi :
        topdecls_cs_semi topdecl_cs semis1
        | /*empty*/
	;
//-- Each topdecl accumulates prior comments
topdecl_cs :
	topdecl
    ;
//-----------------------------------------------------------------------------
topdecl :
        cl_decl
        | ty_decl
        | standalone_kind_sig
        | inst_decl
        | stand_alone_deriving
        | role_annot
        | 'default' '(' comma_types0 ')'
        | 'foreign' fdecl
        | '{-# DEPRECATED' deprecations '#-}'
        | '{-# WARNING' warnings '#-}'
        | '{-# RULES' rules '#-}'
        | annotation
        | decl_no_th
        //-- Template Haskell Extension
        //-- The $(..) form is one possible form of infixexp
        //-- but we treat an arbitrary expression just as if
        //-- it had a $(..) wrapped around it
        | infixexp
	;
//-- Type classes
//--
cl_decl :
        'class' tycl_hdr fds where_cls
    ;

//-- Type declarations (toplevel)
//--
ty_decl :
           //-- ordinary type synonyms
        'type' type '=' ktype
                //-- Note ktype, not sigtype, on the right of '='
                //-- We allow an explicit for-all but we don't insert one
                //-- in   type Foo a = (b,b)
                //-- Instead we just say b is out of scope
                //--
                //-- Note the use of type for the head; this allows
                //-- infix type constructors to be declared


           //-- type family declarations
        | 'type' 'family' type opt_tyfam_kind_sig opt_injective_info
                          where_type_family
                //-- Note the use of type for the head; this allows
                //-- infix type constructors to be declared


          //-- ordinary data type or newtype declaration
        | type_data_or_newtype capi_ctype tycl_hdr constrs maybe_derivings

                                   //-- We need the location on tycl_hdr in case
                                   //-- constrs and deriving are both empty

          //-- ordinary GADT declaration
        | type_data_or_newtype capi_ctype tycl_hdr opt_kind_sig
                 gadt_constrlist
                 maybe_derivings

                                   //-- We need the location on tycl_hdr in case
                                   //-- constrs and deriving are both empty

          //-- data/newtype family
        | 'data' 'family' type opt_datafam_kind_sig
    ;

//-- standalone kind signature
standalone_kind_sig :
  'type' sks_vars '::' sigktype
    ;

//-- See also: sig_vars
sks_vars :  //-- Returned in reverse order
  sks_vars ',' oqtycon

  | oqtycon
	;
inst_decl :
        'instance' overlap_pragma inst_type where_inst


           //-- type instance declarations
        | 'type' 'instance' ty_fam_inst_eqn


          //-- data/newtype instance declaration
        | data_or_newtype 'instance' capi_ctype datafam_inst_hdr constrs
                          maybe_derivings


          //-- GADT instance declaration
        | data_or_newtype 'instance' capi_ctype datafam_inst_hdr opt_kind_sig
                 gadt_constrlist
                 maybe_derivings
    ;

overlap_pragma :
  '{-# OVERLAPPABLE'    '#-}'
  | '{-# OVERLAPPING'     '#-}'
  | '{-# OVERLAPS'        '#-}'
  | '{-# INCOHERENT'      '#-}'
  | /*empty*/
	;
deriv_strategy_no_via :
  'stock'
  | 'anyclass'
  | 'newtype'
	;
deriv_strategy_via :
  'via' sigktype
    ;
deriv_standalone_strategy :
  'stock'
  | 'anyclass'
  | 'newtype'
  | deriv_strategy_via
  | /*empty*/
	;
//-- Injective type families

opt_injective_info :
        /*empty*/
        | '|' injectivity_cond
	;
injectivity_cond :
        tyvarid '->' inj_varids
    ;

inj_varids :
        inj_varids tyvarid
        | tyvarid
	;
//-- Closed type families

where_type_family :
        /*empty*/
        | 'where' ty_fam_inst_eqn_list
    ;

ty_fam_inst_eqn_list :
        '{' ty_fam_inst_eqns '}'
        | vocurly ty_fam_inst_eqns close
        |     '{' '..' '}'
        | vocurly '..' close
	;
ty_fam_inst_eqns :
        ty_fam_inst_eqns ';' ty_fam_inst_eqn

        | ty_fam_inst_eqns ';'
        | ty_fam_inst_eqn
        | /*empty*/
	;
ty_fam_inst_eqn :
        'forall' tv_bndrs '.' type '=' ktype

        | type '=' ktype

              //-- Note the use of type for the head; this allows
              //-- infix type constructors and type patterns
        ;
//-- Associated type family declarations
//--
//-- * They have a different syntax than on the toplevel (no family special
//--   identifier).
//--
//-- * They also need to be separate from instances; otherwise, data family
//--   declarations without a kind signature cause parsing conflicts with empty
//--   data declarations.
//--
at_decl_cls :
        //-- data family declarations, with optional 'family' keyword
          'data' opt_family type opt_datafam_kind_sig


           //-- type family declarations, with optional 'family' keyword
           //-- (can't use opt_instance because you get shift/reduce errors
        | 'type' type opt_at_kind_inj_sig

        | 'type' 'family' type opt_at_kind_inj_sig


           //-- default type instances, with optional 'instance' keyword
        | 'type' ty_fam_inst_eqn

        | 'type' 'instance' ty_fam_inst_eqn
    ;

opt_family   :
              /*empty*/
              | 'family'
	;
opt_instance :
              /*empty*/
              | 'instance'
	;
//-- Associated type instances
//--
at_decl_inst :
           //-- type instance declarations, with optional 'instance' keyword
        'type' opt_instance ty_fam_inst_eqn
                //-- Note the use of type for the head; this allows
                //-- infix type constructors and type patterns


        //-- data/newtype instance declaration, with optional 'instance' keyword
        | data_or_newtype opt_instance capi_ctype datafam_inst_hdr constrs maybe_derivings


        //-- GADT instance declaration, with optional 'instance' keyword
        | data_or_newtype opt_instance capi_ctype datafam_inst_hdr opt_kind_sig
                 gadt_constrlist
                 maybe_derivings
        ;

type_data_or_newtype :
        'data'
        | 'newtype'
        | 'type' 'data'
	;
data_or_newtype :
        'data'
        | 'newtype'
	;
//-- Family result/return kind signatures

opt_kind_sig :
        | '::' kind
	;
opt_datafam_kind_sig :
        | '::' kind
	;
opt_tyfam_kind_sig :
        | '::' kind
        | '='  tv_bndr
	;
opt_at_kind_inj_sig :
        | '::' kind
        | '='  tv_bndr_no_braces '|' injectivity_cond
    ;

//-- tycl_hdr parses the header of a class or data type decl,
//-- which takes the form
//--      T a b
//--      Eq a => T a
//--      (Eq a, Ord b) => T a b
//--      T Int [a]                       //-- for associated types
//-- Rather a lot of inlining here, else we get reduce/reduce errors
tycl_hdr :
        context '=>' type
        | type
	;
datafam_inst_hdr :
        'forall' tv_bndrs '.' context '=>' type
        | 'forall' tv_bndrs '.' type
        | context '=>' type
        | type
	;

capi_ctype :
	'{-# CTYPE' STRING STRING '#-}'


           | '{-# CTYPE'        STRING '#-}'


           |
	;
//-----------------------------------------------------------------------------
//-- Stand-alone deriving

//-- Glasgow extension: stand-alone deriving declarations
stand_alone_deriving :
  'deriving' deriv_standalone_strategy 'instance' overlap_pragma inst_type
    ;

//-----------------------------------------------------------------------------
//-- Role annotations

role_annot :
	'type' 'role' oqtycon maybe_roles
    ;

//-- Reversed!
maybe_roles :
	/*empty*/
            | roles
	;
roles :
	role
      | roles role
	;
//-- read it in as a varid for better error messages
role :
	VARID
     | '_'
	;
//-- Pattern synonyms

//-- Glasgow extension: pattern synonyms
pattern_synonym_decl :
        'pattern' pattern_synonym_lhs '=' pat


        | 'pattern' pattern_synonym_lhs '<-' pat


        | 'pattern' pattern_synonym_lhs '<-' pat where_decls
    ;

pattern_synonym_lhs :
        con vars0
        | varid conop varid
        | con '{' cvars1 '}'
	;
vars0 :
        /*empty*/
        | varid vars0
	;
cvars1 :
       var
       | var ',' cvars1
	;
where_decls :
        'where' '{' decls '}'
        | 'where' vocurly decls close
	;
pattern_synonym_sig :
        'pattern' con_list '::' sigtype
    ;

qvarcon :
        qvar
        | qcon
	;
//-----------------------------------------------------------------------------
//-- Nested declarations

//-- Declaration in class bodies
//--
decl_cls  :
	at_decl_cls
          | decl
          //-- A 'default' signature used with the generic-programming extension
          | 'default' infixexp '::' sigtype
    ;

decls_cls :  //-- Reversed
          decls_cls ';' decl_cls
          | decls_cls ';'
          | decl_cls
          | /*empty*/
	;
decllist_cls
        :      //-- Reversed
        '{'         decls_cls '}'
        |     vocurly decls_cls close
	;
//-- Class body
//--
where_cls :
                                //-- No implicit parameters
                                //-- May have type declarations
        'where' decllist_cls
        | /*empty*/
	;
//-- Declarations in instance bodies
//--
decl_inst  :
	at_decl_inst
           | decl
	;
decls_inst :   //-- Reversed
           decls_inst ';' decl_inst
           | decls_inst ';'
           | decl_inst
           | /*empty*/
	;
decllist_inst
        :      //-- Reversed
        '{'         decls_inst '}'
        |     vocurly decls_inst close
	;
//-- Instance body
//--
where_inst :   //-- Reversed
                                //-- No implicit parameters
                                //-- May have type declarations
        'where' decllist_inst
        | /*empty*/
	;
//-- Declarations in binding groups other than classes and instances
//--
decls   :
        decls ';' decl
        | decls ';'
        | decl
        | /*empty*/
	;
decllist :
        '{'            decls '}'
        |     vocurly    decls close
	;
//-- Binding groups other than those of class and instance declarations
//--
binds   :
                                         //-- May have implicit parameters
                                                //-- No type declarations
        decllist

        | '{'            dbinds '}'
        |     vocurly    dbinds close
    ;

wherebinds :
                                                //-- May have implicit parameters
                                                //-- No type declarations
        'where' binds
        | /*empty*/
	;
//-----------------------------------------------------------------------------
//-- Transformation Rules

rules   : //-- Reversed
        rules ';' rule
        |  rules ';'
        |  rule
        |  /*empty*/
	;
rule    :
        STRING rule_activation rule_foralls infixexp '=' exp
    ;

//-- Rules can be specified to be NeverActive, unlike inline/specialize pragmas
rule_activation :
        //-- See Note [//%shift: rule_activation -> /*empty*/]
        /*empty*/ //%shift
        | rule_explicit_activation
	;
//-- This production is used to parse the tilde syntax in pragmas such as
//--   *
//--   *
//--   *
//-- Note that it can be written either
//--   without a space [~1]  (the PREFIX_TILDE case), or
//--   with    a space [~ 1] (the VARSYM case).
//-- See Note [Whitespace-sensitive operator parsing] in GHC.Parser.Lexer
rule_activation_marker :
      PREFIX_TILDE
      | VARSYM
	;
rule_explicit_activation :  //-- In brackets
        '[' INTEGER ']'
        | '[' rule_activation_marker INTEGER ']'

        | '[' rule_activation_marker ']'
    ;

rule_foralls :
        'forall' rule_vars '.' 'forall' rule_vars '.'

        //-- See Note [//%shift: rule_foralls -> 'forall' rule_vars '.']
        | 'forall' rule_vars '.' //%shift
        //-- See Note [//%shift: rule_foralls -> /*empty*/]
        | /*empty*/            //%shift
	;
rule_vars :
        rule_var rule_vars
        | /*empty*/
	;
rule_var :
        varid
        | '(' varid '::' ctype ')'
	;


//-----------------------------------------------------------------------------
//-- Warnings and deprecations (c.f. rules)

warning_category :
        'in' STRING
        | /*empty*/
	;
warnings :
        warnings ';' warning
        | warnings ';'
        | warning
        | /*empty*/
	;
//-- SUP: TEMPORARY HACK, not checking for `module Foo'
warning :
        warning_category namelist strings
    ;

deprecations :
        deprecations ';' deprecation

        | deprecations ';'
        | deprecation
        | /*empty*/
	;
//-- SUP: TEMPORARY HACK, not checking for `module Foo'
deprecation :
        namelist strings
    ;

strings :
    STRING
    | '[' stringlist ']'
	;
stringlist :
    stringlist ',' STRING
    | STRING
    | /*empty*/
	;
//-----------------------------------------------------------------------------
//-- Annotations
annotation :
    '{-# ANN' name_var aexp '#-}'

    | '{-# ANN' 'type' otycon aexp '#-}'

    | '{-# ANN' 'module' aexp '#-}'
    ;
//-----------------------------------------------------------------------------
//-- Foreign import and export declarations

fdecl :
	'import' callconv safety fspec

      | 'import' callconv        fspec

      | 'export' callconv fspec
    ;

callconv :
          'stdcall'
          | 'ccall'
          | 'capi'
          | 'prim'
          | 'javascript'
	;
safety :
        'unsafe'
        | 'safe'
        | 'interruptible'
	;
fspec :
       STRING var '::' sigtype
       |        var '::' sigtype
         //-- if the entity string is missing, it defaults to the empty string;
         //-- the meaning of an empty entity string depends on the calling
         //-- convention
    ;
//-----------------------------------------------------------------------------
//-- Type signatures

opt_sig :
        /*empty*/
        | '::' ctype
	;
opt_tyconsig :
             /*empty*/
             | '::' gtycon
	;
//-- Like ktype, but for types that obey the forall-or-nothing rule.
//-- See Note [forall-or-nothing rule] in GHC.Hs.Type.
sigktype :
        sigtype
        | ctype '::' kind
	;
//-- Like ctype, but for types that obey the forall-or-nothing rule.
//-- See Note [forall-or-nothing rule] in GHC.Hs.Type. To avoid duplicating the
//-- logic in ctype here, we simply reuse the ctype production and perform
//-- surgery on the LHsType it returns to turn it into an LHsSigType.
sigtype :
        ctype
    ;
sig_vars :    //-- Returned in reversed order
         sig_vars ',' var
         | var
	;
sigtypes1 :
   sigtype
   | sigtype ',' sigtypes1
   ;
//-----------------------------------------------------------------------------
//-- Types

unpackedness :
        '{-# UNPACK' '#-}'
        | '{-# NOUNPACK' '#-}'
	;
forall_telescope :
        'forall' tv_bndrs '.'
        | 'forall' tv_bndrs '->'
	;
//-- A ktype is a ctype, possibly with a kind annotation
ktype :
        ctype
        | ctype '::' kind
	;
//-- A ctype is a for-all type
ctype   :
        forall_telescope ctype
        | context '=>' ctype
        | ipvar '::' ctype
        | type
	;
//----------------------
//-- Notes for 'context'
//-- We parse a context as a btype so that we don't get reduce/reduce
//-- errors in ctype.  The basic problem is that
//--      (Eq a, Ord a)
//-- looks so much like a tuple type.  We can't tell until we find the =>

context :
        btype
    ;


type :
        //-- See Note [//%shift: type -> btype]
        btype //%shift
        | btype '->' ctype
        | btype mult '->' ctype

        | btype '->.' ctype
                                              //-- [mu AnnLollyU $2] }
        ;

mult :
        PREFIX_PERCENT atype
    ;
btype :
        infixtype
    ;
infixtype :
        //-- See Note [//%shift: infixtype -> ftype]
        ftype //%shift
        | ftype tyop infixtype
        | unpackedness infixtype
	;
ftype :
        atype
        | tyop
        | ftype tyarg
        | ftype PREFIX_AT atype
	;
tyarg :
        atype
        | unpackedness atype
	;
tyop :
        qtyconop
        | tyvarop
        | SIMPLEQUOTE qconop
        | SIMPLEQUOTE varop
	;
atype :
        ntgtycon                             //-- Not including unit tuples
        //-- See Note [//%shift: atype -> tyvar]
        | tyvar //%shift                         //-- (See Note [Unit tuples])
        | '*'

        //-- See Note [Whitespace-sensitive operator parsing] in GHC.Parser.Lexer
        | PREFIX_TILDE atype
        | PREFIX_BANG  atype

        | '{' fielddecls '}'
                                                        //-- Constructor sigs only
        | '(' ')'
        | '(' ktype ',' comma_types1 ')'
        | '(#' '#)'
        | '(#' comma_types1 '#)'
        | '(#' bar_types2 '#)'
        | '[' ktype ']'
        | '(' ktype ')'
        | quasiquote
        | splice_untyped
                                      //-- see Note [Promotion] for the followings
        | SIMPLEQUOTE qcon_nowiredlist
        | SIMPLEQUOTE  '(' ktype ',' comma_types1 ')'

        | SIMPLEQUOTE  '[' comma_types0 ']'
        | SIMPLEQUOTE var

        //-- Two or more [ty, ty, ty] must be a promoted list type, just as
        //-- if you had written '[ty, ty, ty]
        //-- (One means a list type, zero means the list type constructor,
        //-- so you have to quote those.)
        | '[' ktype ',' comma_types1 ']'
        | INTEGER
        | CHAR
        | STRING
        | '_'
        //-- Type variables are never exported, so `M.tyvar` will be rejected by the renamer.
        //-- We let it pass the parser because the renamer can generate a better error message.
        | QVARID
	;
//-- An inst_type is what occurs in the head of an instance decl
//--      e.g.  (Foo a, Gaz b) => Wibble a b
//-- It's kept as a single type for convenience.
inst_type :
        sigtype
    ;
deriv_types :
        sigktype

        | sigktype ',' deriv_types
	;
comma_types0  :  //-- Zero or more:  ty,ty,ty
        comma_types1
        | /*empty*/
	;
comma_types1    :  //-- One or more:  ty,ty,ty
        ktype
        | ktype  ',' comma_types1
	;
bar_types2    :  //-- Two or more:  ty|ty|ty
        ktype  '|' ktype
        | ktype  '|' bar_types2
	;
tv_bndrs :
         tv_bndr tv_bndrs
         | /*empty*/
	;
tv_bndr :
        tv_bndr_no_braces
        | '{' tyvar '}'
        | '{' tyvar '::' kind '}'
	;
tv_bndr_no_braces :
        tyvar
        | '(' tyvar '::' kind ')'
	;
fds :
        /*empty*/
        | '|' fds1
	;
fds1 :
        fds1 ',' fd
        | fd
	;
fd :
        varids0 '->' varids0
    ;
varids0 :
        /*empty*/
        | varids0 tyvar
	;
//-----------------------------------------------------------------------------
//-- Kinds

kind :
        ctype
    ;



//-----------------------------------------------------------------------------
//-- Datatype declarations

gadt_constrlist : //-- Returned in order

        'where' '{'        gadt_constrs '}'
        | 'where' vocurly    gadt_constrs close
        | /*empty*/
	;
gadt_constrs :
        gadt_constr ';' gadt_constrs

        | gadt_constr
        | /*empty*/
	;
//-- We allow the following forms:
//--      C :Eq a => a -> T a
//--      C :forall a. Eq a => !a -> T a
//--      D  :T a
//--      forall a. Eq a => D  :T a

gadt_constr :
    //-- see Note [Difference in parsing GADT and data constructors]
    //-- Returns a list because of:   C,D :ty
    //-- TODO:AZ capture the optSemi. Why leading?
        optSemi con_list '::' sigtype
    ;



constrs :
        '=' constrs1
    ;
constrs1 :
        constrs1 '|' constr

        | constr
	;
constr :
        forall context '=>' constr_stuff

        | forall constr_stuff
    ;

forall :
        'forall' tv_bndrs '.'
        | /*empty*/
	;
constr_stuff :
        infixtype
    ;
fielddecls :
        /*empty*/
        | fielddecls1
	;
fielddecls1 :
        fielddecl ',' fielddecls1

        | fielddecl
	;
fielddecl :
                                              //-- A list because of   f,g :Int
        sig_vars '::' ctype
    ;

//-- Reversed!
maybe_derivings :
        /*empty*/
        | derivings
	;
//-- A list of one or more deriving clauses at the end of a datatype
derivings :
        derivings deriving       //-- AZ: order?
        | deriving
	;
//-- The outer Located is just to allow the caller to
//-- know the rightmost extremity of the 'deriving' clause
deriving :
        'deriving' deriv_clause_types


        | 'deriving' deriv_strategy_no_via deriv_clause_types


        | 'deriving' deriv_clause_types deriv_strategy_via
    ;

deriv_clause_types :
        qtycon
        | '(' ')'
        | '(' deriv_types ')'
	;
//-----------------------------------------------------------------------------
//-- Value definitions





decl_no_th :
        sigdecl

        | infixexp     opt_sig rhs
        | pattern_synonym_decl
	;
decl    :
        decl_no_th

        //-- Why do we only allow naked declaration splices in top-level
        //-- declarations and not here? Short answer: because readFail009
        //-- fails terribly with a panic in cvBindsAndSigs otherwise.
        | splice_exp
	;
rhs     :
        '=' exp wherebinds
        | gdrhs wherebinds
	;
gdrhs :
        gdrhs gdrh
        | gdrh
	;
gdrh :
        '|' guardquals '=' exp
    ;
sigdecl :
        //-- See Note [Declaration/signature overlap] for why we need infixexp here
          infixexp     '::' sigtype


        | var ',' sig_vars '::' sigtype


        | infix prec ops


        | pattern_synonym_sig

        | '{-# COMPLETE' qcon_list opt_tyconsig  '#-}'


        //-- This rule is for both INLINE and INLINABLE pragmas
        | '{-# INLINE' activation qvarcon '#-}'

        | '{-# OPAQUE' qvar '#-}'

        | '{-# SCC' qvar '#-}'


        | '{-# SCC' qvar STRING '#-}'


        | '{-# SPECIALISE' activation qvar '::' sigtypes1 '#-}'


        | '{-# SPECIALISE_INLINE' activation qvar '::' sigtypes1 '#-}'


        | '{-# SPECIALISE' 'instance' inst_type '#-}'


        //-- A minimal complete definition
        | '{-# MINIMAL' name_boolformula_opt '#-}'
    ;

activation :
        //-- See Note [//%shift: activation -> /*empty*/]
        /*empty*/ //%shift
        | explicit_activation
	;
explicit_activation :  //-- In brackets
        '[' INTEGER ']'
        | '[' rule_activation_marker INTEGER ']'
    ;

//-----------------------------------------------------------------------------
//-- Expressions

quasiquote :
        TH_QUASIQUOTE
        | TH_QQUASIQUOTE
	;
exp   :
        infixexp '::' ctype

        | infixexp '-<' exp
        | infixexp '>-' exp
        | infixexp '-<<' exp
        | infixexp '>>-' exp
        //-- See Note [//%shift: exp -> infixexp]
        | infixexp //%shift
    ///    | exp_prag(exp)          //-- See Note [Pragmas and operator fixity]
	;
infixexp :
        exp10
        | infixexp qop exp10p    //-- See Note [Pragmas and operator fixity]

                 //-- AnnVal annotation for NPlusKPat, which discards the operator
    ;
exp10p :
  exp10
  ///| exp_prag(exp10p)  //-- See Note [Pragmas and operator fixity]
	;
///exp_prag(e) :
///  prag_e e  //-- See Note [Pragmas and operator fixity]


exp10 :
        //-- See Note [//%shift: exp10 -> '-' fexp]
        '-' fexp //%shift
        //-- See Note [//%shift: exp10 -> fexp]
        | fexp //%shift
	;
optSemi :
        ';'
        | /*empty*/
	;
/*
prag_e :
      '{-# SCC' STRING '#-}'
      | '{-# SCC' VARID  '#-}'
	;
*/
fexp    :
        fexp aexp

        //-- See Note [Whitespace-sensitive operator parsing] in GHC.Parser.Lexer
        | fexp PREFIX_AT atype

        | 'static' aexp

        | aexp
	;
aexp    :
        //-- See Note [Whitespace-sensitive operator parsing] in GHC.Parser.Lexer
        qvar TIGHT_INFIX_AT aexp



        //-- See Note [Whitespace-sensitive operator parsing] in GHC.Parser.Lexer
        | PREFIX_TILDE aexp
        | PREFIX_BANG aexp
        | PREFIX_MINUS aexp

        | '\\' apats '->' exp

        | 'let' binds 'in' exp
        ///| '\\' 'lcase' altslist(pats1)

        ///| '\\' 'lcases' altslist(apats)

        | 'if' exp optSemi 'then' exp optSemi 'else' exp


        | 'if' ifgdpats
        ///| 'case' exp 'of' altslist(pats1)
        //-- QualifiedDo.
        | DO  stmtlist
        | MDO stmtlist
        | 'proc' aexp '->' exp


        | aexp1
	;
aexp1   :
        aexp1 '{' fbinds '}'

        //-- See Note [Whitespace-sensitive operator parsing] in GHC.Parser.Lexer
        | aexp1 TIGHT_INFIX_PROJ field



        | aexp2
	;
aexp2   :
        qvar
        | qcon
        //-- See Note [//%shift: aexp2 -> ipvar]
        | ipvar //%shift
        | overloaded_label
        | literal
//-- This will enable overloaded strings permanently.  Normally the renamer turns HsString
//-- into HsOverLit when -XOverloadedStrings is on.
//--      | STRING
        | INTEGER
        | RATIONAL

        //-- N.B.: sections get parsed by these next two productions.
        //-- This allows you to write, e.g., '(+ 3, 4 -)', which isn't
        //-- correct Haskell (you'd have to write '((+ 3), (4 -))')
        //-- but the less cluttered version fell out of having texps.
        | '(' texp ')'
        | '(' tup_exprs ')'

        //-- This case is only possible when 'OverloadedRecordDotBit' is enabled.
        | '(' projection ')'

        | '(#' texp '#)'
        | '(#' tup_exprs '#)'

        | '[' list ']'
        | '_'

        //-- Template Haskell Extension
        | splice_untyped
        | splice_typed

        | SIMPLEQUOTE  qvar
        | SIMPLEQUOTE  qcon
        | TH_TY_QUOTE tyvar
        | TH_TY_QUOTE gtycon
        //-- See Note [//%shift: aexp2 -> TH_TY_QUOTE]
        | TH_TY_QUOTE //%shift
        | '[|' exp '|]'
        | '[||' exp '||]'
        | '[t|' ktype '|]'
        | '[p|' infixexp '|]'
        | '[d|' cvtopbody '|]'
        | quasiquote

        //-- arrow notation extension
        | '(|' aexp cmdargs '|)'
	;
projection :
projection
        //-- See Note [Whitespace-sensitive operator parsing] in GHC.Parsing.Lexer
        projection TIGHT_INFIX_PROJ field

        | PREFIX_PROJ field
	;
splice_exp :
        splice_untyped
        | splice_typed
	;
splice_untyped :
        //-- See Note [Whitespace-sensitive operator parsing] in GHC.Parser.Lexer
        PREFIX_DOLLAR aexp2
    ;
splice_typed :
        //-- See Note [Whitespace-sensitive operator parsing] in GHC.Parser.Lexer
        PREFIX_DOLLAR_DOLLAR aexp2
    ;

cmdargs :
        cmdargs acmd
        | /*empty*/
	;
acmd    :
        aexp
    ;
cvtopbody :
        '{'            cvtopdecls0 '}'
        |      vocurly    cvtopdecls0 close
	;
cvtopdecls0 :
        topdecls_semi
        | topdecls
	;
//-----------------------------------------------------------------------------
//-- Tuple expressions

//-- "texp" is short for tuple expressions:
//-- things that can appear unparenthesized as long as they're
//-- inside parens or delimited by commas
texp :
        exp

        //-- Note [Parsing sections]
        //-- ~~~~~~~~~~~~~~~~~~~~~~~
        //-- We include left and right sections here, which isn't
        //-- technically right according to the Haskell standard.
        //-- For example (3 +, True) isn't legal.
        //-- However, we want to parse bang patterns like
        //--      (!x, !y)
        //-- and it's convenient to do so here as a section
        //-- Then when converting expr to pattern we unravel it again
        //-- Meanwhile, the renamer checks that real sections appear
        //-- inside parens.
        | infixexp qop

        | qopm infixexp

       //-- View patterns get parenthesized above
        | exp '->' texp
	;
//-- Always at least one comma or bar.
//-- Though this can parse just commas (without any expressions), it won't
//-- in practice, because (,,,) is parsed as a name. See Note [ExplicitTuple]
//-- in GHC.Hs.Expr.
tup_exprs :
           texp commas_tup_tail

           | commas tup_tail


           | texp bars

           | bars texp bars0
    ;

//-- Always starts with commas; always follows an expr
commas_tup_tail :
	commas tup_tail
    ;

//-- Always follows a comma
tup_tail :
          texp commas_tup_tail
          | texp
          //-- See Note [//%shift: tup_tail -> /*empty*/]
          | /*empty*/ //%shift
	;
//-----------------------------------------------------------------------------
//-- List expressions

//-- The rules below are little bit contorted to keep lexps left-recursive while
//-- avoiding another shift/reduce-conflict.
//-- Never empty.
list :
        texp
        | lexps
        | texp '..'
        | texp ',' exp '..'
        | texp '..' exp
        | texp ',' exp '..' exp
        | texp '|' flattenedpquals
    ;

lexps :
        lexps ',' texp
        | texp ',' texp
	;
//-----------------------------------------------------------------------------
//-- List Comprehensions

flattenedpquals :
    pquals
    ;
pquals :
    squals '|' pquals

    | squals
	;
squals :   //-- In reverse order, because the last
                                        //-- one can "grab" the earlier ones
    squals ',' transformqual

    | squals ',' qual

    | transformqual
    | qual
//--  | transformquals1 ',' '{|' pquals '|}'
//--  | '{|' pquals '|}'
    ;
//-- It is possible to enable bracketing (associating) qualifier lists
//-- by uncommenting the lines with  above. Due to a lack of
//-- consensus on the syntax, this feature is not being used until we
//-- get user demand.

transformqual :
                        //-- Function is applied to a list of stmts *in order*
    'then' exp
    | 'then' exp 'by' exp
    | 'then' 'group' 'using' exp


    | 'then' 'group' 'by' exp 'using' exp
    ;

//-- Note that 'group' is a special_id, which means that you can enable
//-- TransformListComp while still using Data.List.group. However, this
//-- introduces a shift/reduce conflict. Happy chooses to resolve the conflict
//-- in by choosing the "group by" variant, which is what we want.

//-----------------------------------------------------------------------------
//-- Guards

guardquals :
    guardquals1
    ;
guardquals1 :
    guardquals1 ',' qual
    | qual
	;
//-----------------------------------------------------------------------------
//-- Case alternatives
/*
altslist(PATS) :
        '{'        alts(PATS) '}'
        | vocurly    alts(PATS)  close
        | '{'              '}'
        | vocurly          close
	;
alts(PATS) :
        alts1(PATS)
        | ';' alts(PATS)
	;
alts1(PATS) :
        alts1(PATS) ';' alt(PATS)
        | alts1(PATS) ';'
        | alt(PATS)
	;
alt(PATS) :
        PATS alt_rhs
    ;
alt_rhs :
        ralt wherebinds
    ;

ralt :
        '->' exp
        | gdpats
	;
*/
gdpats :
        gdpats gdpat
        | gdpat
	;
//-- layout for MultiWayIf doesn't begin with an open brace, because it's hard to
//-- generate the open brace in addition to the vertical bar in the lexer, and
//-- we don't need it.
ifgdpats :
         '{' gdpats '}'
         |     gdpats close
	;
gdpat   :
        '|' guardquals '->' exp
    ;

//-- 'pat' recognises a pattern, including one with a bang at the top
//--      e.g.  "!x" or "!(x,y)" or "C a b" etc
//-- Bangs inside are parsed as infix operator applications, so that
//-- we parse them right when bang-patterns are off
pat     :
	 exp
    ;
//-- 'pats1' does the same thing as 'pat', but returns it as a singleton
//-- list so that it can be used with a parameterized production rule
/*
pats1   :
	pat
    ;
*/
bindpat :
	 exp
    ;
apat   :
	aexp
    ;
apats  :
        apat apats
        | /*empty*/
	;
//-----------------------------------------------------------------------------
//-- Statement sequences

stmtlist :
        '{'           stmts '}'
        |     vocurly   stmts close
	;
//--      do
//-- The last Stmt should be an expression, but that's hard to enforce
//-- here, because we need too much lookahead if we see do
//-- So we use BodyStmts throughout, and switch the last one over
//-- in ParseUtils.checkDo instead

stmts :
        stmts ';' stmt

        | stmts ';'
        | stmt
        | /*empty*/
	;

//-- For typing stmts at the GHCi prompt, where
//-- the input may consist of just comments.
maybe_stmt :
        stmt
        |
	;
//-- For GHC API.
e_stmt :
        stmt
    ;
stmt  :
        qual
        | 'rec' stmtlist
	;
qual  :
    bindpat '<-' exp
    | exp
    | 'let' binds
	;
//-----------------------------------------------------------------------------
//-- Record Field Update/Construction

fbinds  :
        fbinds1
        | /*empty*/
	;
fbinds1 :
        fbind ',' fbinds1

        | fbind
        | '..'
	;
fbind   :
        qvar '=' texp
                        //-- RHS is a 'texp', allowing view patterns (#6038)
                        //-- and, incidentally, sections.  Eg
                        //-- f (R ) = ...

        | qvar
                        //-- In the punning case, use a place-holder
                        //-- The renamer fills in the final value

        //-- See Note [Whitespace-sensitive operator parsing] in GHC.Parser.Lexer
        //-- AZ: need to pull out the let block into a helper
        | field TIGHT_INFIX_PROJ fieldToUpdate '=' texp


        //-- See Note [Whitespace-sensitive operator parsing] in GHC.Parser.Lexer
        //-- AZ: need to pull out the let block into a helper
        | field TIGHT_INFIX_PROJ fieldToUpdate
    ;

fieldToUpdate :
fieldToUpdate
        //-- See Note [Whitespace-sensitive operator parsing] in Lexer.x
        fieldToUpdate TIGHT_INFIX_PROJ field
        | field
	;
//-----------------------------------------------------------------------------
//-- Implicit Parameter Bindings

dbinds  : //-- reversed
        dbinds ';' dbind

        | dbinds ';'
        | dbind
//--      | /*empty*/
    ;
dbind   :
	ipvar '=' exp
    ;
ipvar   :
        IPDUPVARID
    ;
//-----------------------------------------------------------------------------
//-- Overloaded labels

overloaded_label :
        LABELVARID
    ;
//-----------------------------------------------------------------------------
//-- Warnings and deprecations

name_boolformula_opt :
        name_boolformula
        | /*empty*/
	;
name_boolformula :
        name_boolformula_and
        | name_boolformula_and '|' name_boolformula
    ;

name_boolformula_and :
        name_boolformula_and_list
    ;

name_boolformula_and_list :
        name_boolformula_atom
        | name_boolformula_atom ',' name_boolformula_and_list
    ;

name_boolformula_atom :
        '(' name_boolformula ')'
        | name_var
	;
namelist :
	name_var
         | name_var ',' namelist
	;
name_var :
	var
         | con
	;
//-----------------------------------------
//-- Data constructors
//-- There are two different productions here as lifted list constructors
//-- are parsed differently.

qcon_nowiredlist :
        gen_qcon
        | sysdcon_nolist
	;
qcon :
  gen_qcon
  | sysdcon
	;
gen_qcon :
  qconid
  | '(' qconsym ')'
	;
con     :
        conid
        | '(' consym ')'
        | sysdcon
	;
con_list :
	con
         | con ',' con_list
	;
qcon_list :
	qcon
          | qcon ',' qcon_list
	;
//-- See Note [ExplicitTuple] in GHC.Hs.Expr
sysdcon_nolist :  //-- Wired in data constructors
        '(' ')'
        | '(' commas ')'
        | '(#' '#)'
        | '(#' commas '#)'
	;
//-- See Note [Empty lists] in GHC.Hs.Expr
sysdcon :
        sysdcon_nolist
        | '[' ']'
	;
conop :
        consym
        | '`' conid '`'
	;
qconop :
        qconsym
        | '`' qconid '`'
	;
//----------------------------------------------------------------------------
//-- Type constructors


//-- See Note [Unit tuples] in GHC.Hs.Type for the distinction
//-- between gtycon and ntgtycon
gtycon :  //-- A "general" qualified tycon, including unit tuples
        ntgtycon
        | '(' ')'
        | '(#' '#)'
	;
ntgtycon :  //-- A "general" qualified tycon, excluding unit tuples
        oqtycon
        | '(' commas ')'
        | '(#' commas '#)'
        | '(#' bars '#)'
        | '(' '->' ')'
        | '[' ']'
	;
oqtycon :  //-- An "ordinary" qualified tycon;
                                //-- These can appear in export lists
        qtycon
        | '(' qtyconsym ')'
	;
oqtycon_no_varcon :  //-- Type constructor which cannot be mistaken
                                          //-- for variable constructor in export lists
                                          //-- see Note [Type constructors in export list]
        qtycon
        | '(' QCONSYM ')'
        | '(' CONSYM ')'
        | '(' ':' ')'
	;


qtyconop : //-- Qualified or unqualified
        //-- See Note [//%shift: qtyconop -> qtyconsym]
        qtyconsym //%shift
        | '`' qtycon '`'
	;
qtycon :   //-- Qualified or unqualified
        QCONID
        | tycon
	;
tycon   :  //-- Unqualified
        CONID
    ;
qtyconsym :
        QCONSYM
        | QVARSYM
        | tyconsym
	;
tyconsym :
        CONSYM
        | VARSYM
        | ':'
        | '-'
        | '.'
	;
//-- An "ordinary" unqualified tycon. See `oqtycon` for the qualified version.
//-- These can appear in `ANN type` declarations (#19374).
otycon :
        tycon
        | '(' tyconsym ')'
	;
//-----------------------------------------------------------------------------
//-- Operators

op      :   //-- used in infix decls
        varop
        | conop
        | '->'
	;
varop   :
        varsym
        | '`' varid '`'
	;
qop     :   //-- used in sections
        qvarop
        | qconop
        | hole_op
	;
qopm    :   //-- used in sections
        qvaropm
        | qconop
        | hole_op
	;
hole_op :   //-- used in sections
	'`' '_' '`'
    ;
qvarop :
        qvarsym
        | '`' qvarid '`'
	;
qvaropm :
        qvarsym_no_minus
        | '`' qvarid '`'
	;
//-----------------------------------------------------------------------------
//-- Type variables

tyvar   :
	tyvarid
    ;
tyvarop :
	'`' tyvarid '`'
    ;
tyvarid :
        VARID
        | special_id
        | 'unsafe'
        | 'safe'
        | 'interruptible'
        //-- If this changes relative to varid, update 'checkRuleTyVarBndrNames'
        //-- in GHC.Parser.PostProcess
        //-- See Note [Parsing explicit foralls in Rules]
    ;
//-----------------------------------------------------------------------------
//-- Variables

var     :
        varid
        | '(' varsym ')'
	;
qvar    :
        qvarid
        | '(' varsym ')'
        | '(' qvarsym1 ')'
//-- We've inlined qvarsym here so that the decision about
//-- whether it's a qvar or a var can be postponed until
//-- *after* we see the close paren.
    ;
field :
      varid
    ;
qvarid :
        varid
        | QVARID
	;
//-- Note that 'role' and 'family' get lexed separately regardless of
//-- the use of extensions. However, because they are listed here,
//-- this is OK and they can be used as normal varids.
//-- See Note [Lexing type pseudo-keywords] in GHC.Parser.Lexer
varid :
        VARID
        | special_id
        | 'unsafe'
        | 'safe'
        | 'interruptible'
        | 'forall'
        | 'family'
        | 'role'
        //-- If this changes relative to tyvarid, update 'checkRuleTyVarBndrNames'
        //-- in GHC.Parser.PostProcess
        //-- See Note [Parsing explicit foralls in Rules]
    ;
qvarsym :
        varsym
        | qvarsym1
	;
qvarsym_no_minus :
        varsym_no_minus
        | qvarsym1
	;
qvarsym1 :
	QVARSYM
    ;
varsym :
        varsym_no_minus
        | '-'
	;
varsym_no_minus : //-- varsym not including '-'
        VARSYM
        | special_sym
	;

//-- These special_ids are treated as keywords in various places,
//-- but as ordinary ids elsewhere.   'special_id' collects all these
//-- except 'unsafe', 'interruptible', 'forall', 'family', 'role', 'stock', and
//-- 'anyclass', whose treatment differs depending on context
special_id :
special_id
        'as'
        | 'qualified'
        | 'hiding'
        | 'export'
        | 'label'
        | 'dynamic'
        | 'stdcall'
        | 'ccall'
        | 'capi'
        | 'prim'
        | 'javascript'
        //-- See Note [//%shift: special_id -> 'group']
        | 'group' //%shift
        | 'stock'
        | 'anyclass'
        | 'via'
        | 'unit'
        | 'dependency'
        | 'signature'
	;
special_sym :
	'.'
            | '*'
	;
//-----------------------------------------------------------------------------
//-- Data constructors

qconid :   //-- Qualified or unqualified
        conid
        | QCONID
	;
conid   :
        CONID
    ;
qconsym :  //-- Qualified or unqualified
        consym
        | QCONSYM
	;
consym :
        CONSYM

        //-- ':' means only list cons
        | ':'
	;

//-----------------------------------------------------------------------------
//-- Literals

literal :
        CHAR
        | STRING
        | PRIMINTEGER
        | PRIMWORD
        | PRIMINTEGER8
        | PRIMINTEGER16
        | PRIMINTEGER32
        | PRIMINTEGER64
        | PRIMWORD8
        | PRIMWORD16
        | PRIMWORD32
        | PRIMWORD64
        | PRIMCHAR
        | PRIMSTRING
        | PRIMFLOAT
        | PRIMDOUBLE
	;
//-----------------------------------------------------------------------------
//-- Layout

close :
        vccurly                //-- context popped in lexer.
        | error
	;
//-----------------------------------------------------------------------------
//-- Miscellaneous (mostly renamings)

modid   :
        CONID
        | QCONID
	;
commas :   //-- One or more commas
        commas ','
        | ','
	;
bars0 :     //-- Zero or more bars
        bars
        |
	;
bars :     //-- One or more bars
        bars '|'
        | '|'
	;

//Lexer

VARSYM : 'VARSYM' ;
STRING : 'STRING' ;
VARID : 'VARID' ;
CONID : 'CONID' ;
PREFIX_MINUS : 'PREFIX_MINUS' ;
vocurly : 'vocurly' ;
INTEGER : 'INTEGER' ;
PREFIX_TILDE : 'PREFIX_TILDE' ;
PREFIX_PERCENT : 'PREFIX_PERCENT' ;
PREFIX_AT : 'PREFIX_AT' ;
SIMPLEQUOTE : 'SIMPLEQUOTE' ;
PREFIX_BANG : 'PREFIX_BANG' ;
CHAR : 'CHAR' ;
QVARID : 'QVARID' ;
TH_QUASIQUOTE : 'TH_QUASIQUOTE' ;
TH_QQUASIQUOTE : 'TH_QQUASIQUOTE' ;
TIGHT_INFIX_AT : 'TIGHT_INFIX_AT' ;
DO : 'DO' ;
MDO : 'MDO' ;
TIGHT_INFIX_PROJ : 'TIGHT_INFIX_PROJ' ;
RATIONAL : 'RATIONAL' ;
TH_TY_QUOTE : 'TH_TY_QUOTE' ;
PREFIX_PROJ : 'PREFIX_PROJ' ;
PREFIX_DOLLAR : 'PREFIX_DOLLAR' ;
PREFIX_DOLLAR_DOLLAR : 'PREFIX_DOLLAR_DOLLAR' ;
IPDUPVARID : 'IPDUPVARID' ;
LABELVARID : 'LABELVARID' ;
QCONSYM : 'QCONSYM' ;
CONSYM : 'CONSYM' ;
QCONID : 'QCONID' ;
QVARSYM : 'QVARSYM' ;
PRIMINTEGER : 'PRIMINTEGER' ;
PRIMWORD : 'PRIMWORD' ;
PRIMINTEGER8 : 'PRIMINTEGER8' ;
PRIMINTEGER16 : 'PRIMINTEGER16' ;
PRIMINTEGER32 : 'PRIMINTEGER32' ;
PRIMINTEGER64 : 'PRIMINTEGER64' ;
PRIMWORD8 : 'PRIMWORD8' ;
PRIMWORD16 : 'PRIMWORD16' ;
PRIMWORD32 : 'PRIMWORD32' ;
PRIMWORD64 : 'PRIMWORD64' ;
PRIMCHAR : 'PRIMCHAR' ;
PRIMSTRING : 'PRIMSTRING' ;
PRIMFLOAT : 'PRIMFLOAT' ;
PRIMDOUBLE : 'PRIMDOUBLE' ;

vccurly : '}{' ;               //-- context popped in lexer.

}
