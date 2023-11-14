// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

/*
 * Go language grammar.
 *
 * The Go semicolon rules are:
 *
 *  1. all statements and declarations are terminated by semicolons.
 *  2. semicolons can be omitted before a closing ) or }.
 *  3. semicolons are inserted by the lexer before a newline
 *      following a specific list of tokens.
 *
 * Rules #1 and #2 are accomplished by writing the lists as
 * semicolon-separated lists with an optional trailing semicolon.
 * Rule #3 is implemented in yylex.
 */

go {

%whitespace "[ \t\r\n]*";
%whitespace "//[^\n]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

//%token LLITERAL
//%token LASOP
//%token LCOLAS
//%token LBREAK
//%token LCASE
//%token LCHAN
//%token LCONST
//%token LCONTINUE
//%token LDDD
//%token LDEFAULT
//%token LDEFER
//%token LELSE
//%token LFALL
//%token LFOR
//%token LFUNC
//%token LGO
//%token LGOTO
//%token LIF
//%token LIMPORT
//%token LINTERFACE
//%token LMAP
//%token LNAME
//%token LPACKAGE
//%token LRANGE
//%token LRETURN
//%token LSELECT
//%token LSTRUCT
//%token LSWITCH
//%token LTYPE
//%token LVAR
//%token LANDAND
//%token LANDNOT
//%token LBODY
//%token LCOMM
//%token LDEC
//%token LEQ
//%token LGE
//%token LGT
//%token LIGNORE
//%token LINC
//%token LLE
//%token LLSH
//%token LLT
//%token LNE
//%token LOROR
//%token LRSH
//%token '+'
//%token '-'
//%token '|'
//%token '^'
//%token '*'
//%token '/'
//%token '%'
//%token '&'
//%token NotPackage
//%token NotParen
//%token '('
//%token ')'
//%token PreferToRightParen
//%token ';'
//%token '.'
//%token '$'
//%token '='
//%token ':'
//%token '{'
//%token '}'
//%token '!'
//%token '~'
//%token '['
//%token ']'
//%token '?'
//%token '@'
//%token ','

%left /*1*/ LCOMM ;
%left /*2*/ LOROR ;
%left /*3*/ LANDAND ;
%left /*4*/ LEQ LGE LGT LLE LLT LNE ;
%left /*5*/ '+' '-' '|' '^' ;
%left /*6*/ LANDNOT LLSH LRSH '*' '/' '%' '&' ;
%left /*7*/ NotPackage ;
%left /*8*/ LPACKAGE ;
%left /*9*/ NotParen ;
%left /*10*/ '(' ;
%left /*11*/ ')' ;
%left /*12*/ PreferToRightParen ;

//%start file

//%%

file :
	 loadsys package imports xdcl_list
	;
package :
	 /*empty*/ %prec NotPackage /*7L*/
	| LPACKAGE /*8L*/ sym ';'
	;
loadsys :
	 import_package import_there
	;
imports :
	 /*empty*/
	| imports import ';'
	;
import :
	 LIMPORT import_stmt
	| LIMPORT '(' /*10L*/ import_stmt_list osemi ')' /*11L*/
	| LIMPORT '(' /*10L*/ ')' /*11L*/
	;
import_stmt :
	 import_here import_package import_there
	| import_here import_there
	;
import_stmt_list :
	 import_stmt
	| import_stmt_list ';' import_stmt
	;
import_here :
	 LLITERAL
	| sym LLITERAL
	| '.' LLITERAL
	;
import_package :
	 LPACKAGE /*8L*/ LNAME import_safety ';'
	;
import_safety :
	 /*empty*/
	| LNAME
	;
import_there :
	 hidden_import_list '$' '$'
	;
xdcl :
	 /*empty*/
	| common_dcl
	| xfndcl
	| non_dcl_stmt
	| error
	;
common_dcl :
	 LVAR vardcl
	| LVAR '(' /*10L*/ vardcl_list osemi ')' /*11L*/
	| LVAR '(' /*10L*/ ')' /*11L*/
	| lconst constdcl
	| lconst '(' /*10L*/ constdcl osemi ')' /*11L*/
	| lconst '(' /*10L*/ constdcl ';' constdcl_list osemi ')' /*11L*/
	| lconst '(' /*10L*/ ')' /*11L*/
	| LTYPE typedcl
	| LTYPE '(' /*10L*/ typedcl_list osemi ')' /*11L*/
	| LTYPE '(' /*10L*/ ')' /*11L*/
	;
lconst :
	 LCONST
	;
vardcl :
	 dcl_name_list ntype
	| dcl_name_list ntype '=' expr_list
	| dcl_name_list '=' expr_list
	;
constdcl :
	 dcl_name_list ntype '=' expr_list
	| dcl_name_list '=' expr_list
	;
constdcl1 :
	 constdcl
	| dcl_name_list ntype
	| dcl_name_list
	;
typedclname :
	 sym
	;
typedcl :
	 typedclname ntype
	;
simple_stmt :
	 expr
	| expr LASOP expr
	| expr_list '=' expr_list
	| expr_list LCOLAS expr_list
	| expr LINC
	| expr LDEC
	;
case :
	 LCASE expr_or_type_list ':'
	| LCASE expr_or_type_list '=' expr ':'
	| LCASE expr_or_type_list LCOLAS expr ':'
	| LDEFAULT ':'
	;
compound_stmt :
	 '{' stmt_list '}'
	;
caseblock :
	 case stmt_list
	;
caseblock_list :
	 /*empty*/
	| caseblock_list caseblock
	;
loop_body :
	 LBODY stmt_list '}'
	;
range_stmt :
	 expr_list '=' LRANGE expr
	| expr_list LCOLAS LRANGE expr
	| LRANGE expr
	;
for_header :
	 osimple_stmt ';' osimple_stmt ';' osimple_stmt
	| osimple_stmt
	| range_stmt
	;
for_body :
	 for_header loop_body
	;
for_stmt :
	 LFOR for_body
	;
if_header :
	 osimple_stmt
	| osimple_stmt ';' osimple_stmt
	;
if_stmt :
	 LIF if_header loop_body elseif_list else
	;
elseif :
	 LELSE LIF if_header loop_body
	;
elseif_list :
	 /*empty*/
	| elseif_list elseif
	;
else :
	 /*empty*/
	| LELSE compound_stmt
	;
switch_stmt :
	 LSWITCH if_header LBODY caseblock_list '}'
	;
select_stmt :
	 LSELECT LBODY caseblock_list '}'
	;
expr :
	 uexpr
	| expr LOROR /*2L*/ expr
	| expr LANDAND /*3L*/ expr
	| expr LEQ /*4L*/ expr
	| expr LNE /*4L*/ expr
	| expr LLT /*4L*/ expr
	| expr LLE /*4L*/ expr
	| expr LGE /*4L*/ expr
	| expr LGT /*4L*/ expr
	| expr '+' /*5L*/ expr
	| expr '-' /*5L*/ expr
	| expr '|' /*5L*/ expr
	| expr '^' /*5L*/ expr
	| expr '*' /*6L*/ expr
	| expr '/' /*6L*/ expr
	| expr '%' /*6L*/ expr
	| expr '&' /*6L*/ expr
	| expr LANDNOT /*6L*/ expr
	| expr LLSH /*6L*/ expr
	| expr LRSH /*6L*/ expr
	| expr LCOMM /*1L*/ expr
	;
uexpr :
	 pexpr
	| '*' /*6L*/ uexpr
	| '&' /*6L*/ uexpr
	| '+' /*5L*/ uexpr
	| '-' /*5L*/ uexpr
	| '!' uexpr
	| '~' uexpr
	| '^' /*5L*/ uexpr
	| LCOMM /*1L*/ uexpr
	;
pseudocall :
	 pexpr '(' /*10L*/ ')' /*11L*/
	| pexpr '(' /*10L*/ expr_or_type_list ocomma ')' /*11L*/
	| pexpr '(' /*10L*/ expr_or_type_list LDDD ocomma ')' /*11L*/
	;
pexpr_no_paren :
	 LLITERAL
	| name
	| pexpr '.' sym
	| pexpr '.' '(' /*10L*/ expr_or_type ')' /*11L*/
	| pexpr '.' '(' /*10L*/ LTYPE ')' /*11L*/
	| pexpr '[' expr ']'
	| pexpr '[' oexpr ':' oexpr ']'
	| pexpr '[' oexpr ':' oexpr ':' oexpr ']'
	| pseudocall
	| convtype '(' /*10L*/ expr ocomma ')' /*11L*/
	| comptype lbrace start_complit braced_keyval_list '}'
	| pexpr_no_paren '{' start_complit braced_keyval_list '}'
	| '(' /*10L*/ expr_or_type ')' /*11L*/ '{' start_complit braced_keyval_list '}'
	| fnliteral
	;
start_complit :
	 /*empty*/
	;
keyval :
	 complitexpr ':' complitexpr
	;
bare_complitexpr :
	 expr
	| '{' start_complit braced_keyval_list '}'
	;
complitexpr :
	 expr
	| '{' start_complit braced_keyval_list '}'
	;
pexpr :
	 pexpr_no_paren
	| '(' /*10L*/ expr_or_type ')' /*11L*/
	;
expr_or_type :
	 expr
	| non_expr_type %prec PreferToRightParen /*12L*/
	;
name_or_type :
	 ntype
	;
lbrace :
	 LBODY
	| '{'
	;
new_name :
	 sym
	;
dcl_name :
	 sym
	;
onew_name :
	 /*empty*/
	| new_name
	;
sym :
	 LNAME
	| hidden_importsym
	| '?'
	;
hidden_importsym :
	 '@' LLITERAL '.' LNAME
	| '@' LLITERAL '.' '?'
	;
name :
	 sym %prec NotParen /*9L*/
	;
labelname :
	 new_name
	;
dotdotdot :
	 LDDD
	| LDDD ntype
	;
ntype :
	 recvchantype
	| fntype
	| othertype
	| ptrtype
	| dotname
	| '(' /*10L*/ ntype ')' /*11L*/
	;
non_expr_type :
	 recvchantype
	| fntype
	| othertype
	| '*' /*6L*/ non_expr_type
	;
non_recvchantype :
	 fntype
	| othertype
	| ptrtype
	| dotname
	| '(' /*10L*/ ntype ')' /*11L*/
	;
convtype :
	 fntype
	| othertype
	;
comptype :
	 othertype
	;
fnret_type :
	 recvchantype
	| fntype
	| othertype
	| ptrtype
	| dotname
	;
dotname :
	 name
	| name '.' sym
	;
othertype :
	 '[' oexpr ']' ntype
	| '[' LDDD ']' ntype
	| LCHAN non_recvchantype
	| LCHAN LCOMM /*1L*/ ntype
	| LMAP '[' ntype ']' ntype
	| structtype
	| interfacetype
	;
ptrtype :
	 '*' /*6L*/ ntype
	;
recvchantype :
	 LCOMM /*1L*/ LCHAN ntype
	;
structtype :
	 LSTRUCT lbrace structdcl_list osemi '}'
	| LSTRUCT lbrace '}'
	;
interfacetype :
	 LINTERFACE lbrace interfacedcl_list osemi '}'
	| LINTERFACE lbrace '}'
	;
xfndcl :
	 LFUNC fndcl fnbody
	;
fndcl :
	 sym '(' /*10L*/ oarg_type_list_ocomma ')' /*11L*/ fnres
	| '(' /*10L*/ oarg_type_list_ocomma ')' /*11L*/ sym '(' /*10L*/ oarg_type_list_ocomma ')' /*11L*/ fnres
	;
hidden_fndcl :
	 hidden_pkg_importsym '(' /*10L*/ ohidden_funarg_list ')' /*11L*/ ohidden_funres
	| '(' /*10L*/ hidden_funarg_list ')' /*11L*/ sym '(' /*10L*/ ohidden_funarg_list ')' /*11L*/ ohidden_funres
	;
fntype :
	 LFUNC '(' /*10L*/ oarg_type_list_ocomma ')' /*11L*/ fnres
	;
fnbody :
	 /*empty*/
	| '{' stmt_list '}'
	;
fnres :
	 /*empty*/ %prec NotParen /*9L*/
	| fnret_type
	| '(' /*10L*/ oarg_type_list_ocomma ')' /*11L*/
	;
fnlitdcl :
	 fntype
	;
fnliteral :
	 fnlitdcl lbrace stmt_list '}'
	| fnlitdcl error
	;
xdcl_list :
	 /*empty*/
	| xdcl_list xdcl ';'
	;
vardcl_list :
	 vardcl
	| vardcl_list ';' vardcl
	;
constdcl_list :
	 constdcl1
	| constdcl_list ';' constdcl1
	;
typedcl_list :
	 typedcl
	| typedcl_list ';' typedcl
	;
structdcl_list :
	 structdcl
	| structdcl_list ';' structdcl
	;
interfacedcl_list :
	 interfacedcl
	| interfacedcl_list ';' interfacedcl
	;
structdcl :
	 new_name_list ntype oliteral
	| embed oliteral
	| '(' /*10L*/ embed ')' /*11L*/ oliteral
	| '*' /*6L*/ embed oliteral
	| '(' /*10L*/ '*' /*6L*/ embed ')' /*11L*/ oliteral
	| '*' /*6L*/ '(' /*10L*/ embed ')' /*11L*/ oliteral
	;
packname :
	 LNAME
	| LNAME '.' sym
	;
embed :
	 packname
	;
interfacedcl :
	 new_name indcl
	| packname
	| '(' /*10L*/ packname ')' /*11L*/
	;
indcl :
	 '(' /*10L*/ oarg_type_list_ocomma ')' /*11L*/ fnres
	;
arg_type :
	 name_or_type
	| sym name_or_type
	| sym dotdotdot
	| dotdotdot
	;
arg_type_list :
	 arg_type
	| arg_type_list ',' arg_type
	;
oarg_type_list_ocomma :
	 /*empty*/
	| arg_type_list ocomma
	;
stmt :
	 /*empty*/
	| compound_stmt
	| common_dcl
	| non_dcl_stmt
	| error
	;
non_dcl_stmt :
	 simple_stmt
	| for_stmt
	| switch_stmt
	| select_stmt
	| if_stmt
	;
non_dcl_stmt :
	 labelname ':' stmt
	| LFALL
	| LBREAK onew_name
	| LCONTINUE onew_name
	| LGO pseudocall
	| LDEFER pseudocall
	| LGOTO new_name
	| LRETURN oexpr_list
	;
stmt_list :
	 stmt
	| stmt_list ';' stmt
	;
new_name_list :
	 new_name
	| new_name_list ',' new_name
	;
dcl_name_list :
	 dcl_name
	| dcl_name_list ',' dcl_name
	;
expr_list :
	 expr
	| expr_list ',' expr
	;
expr_or_type_list :
	 expr_or_type
	| expr_or_type_list ',' expr_or_type
	;
keyval_list :
	 keyval
	| bare_complitexpr
	| keyval_list ',' keyval
	| keyval_list ',' bare_complitexpr
	;
braced_keyval_list :
	 /*empty*/
	| keyval_list ocomma
	;
osemi :
	 /*empty*/
	| ';'
	;
ocomma :
	 /*empty*/
	| ','
	;
oexpr :
	 /*empty*/
	| expr
	;
oexpr_list :
	 /*empty*/
	| expr_list
	;
osimple_stmt :
	 /*empty*/
	| simple_stmt
	;
ohidden_funarg_list :
	 /*empty*/
	| hidden_funarg_list
	;
ohidden_structdcl_list :
	 /*empty*/
	| hidden_structdcl_list
	;
ohidden_interfacedcl_list :
	 /*empty*/
	| hidden_interfacedcl_list
	;
oliteral :
	 /*empty*/
	| LLITERAL
	;
hidden_import :
	 LIMPORT LNAME LLITERAL ';'
	| LVAR hidden_pkg_importsym hidden_type ';'
	| LCONST hidden_pkg_importsym '=' hidden_constant ';'
	| LCONST hidden_pkg_importsym hidden_type '=' hidden_constant ';'
	| LTYPE hidden_pkgtype hidden_type ';'
	| LFUNC hidden_fndcl fnbody ';'
	;
hidden_pkg_importsym :
	 hidden_importsym
	;
hidden_pkgtype :
	 hidden_pkg_importsym
	;
hidden_type :
	 hidden_type_misc
	| hidden_type_recv_chan
	| hidden_type_func
	;
hidden_type_non_recv_chan :
	 hidden_type_misc
	| hidden_type_func
	;
hidden_type_misc :
	 hidden_importsym
	| LNAME
	| '[' ']' hidden_type
	| '[' LLITERAL ']' hidden_type
	| LMAP '[' hidden_type ']' hidden_type
	| LSTRUCT '{' ohidden_structdcl_list '}'
	| LINTERFACE '{' ohidden_interfacedcl_list '}'
	| '*' /*6L*/ hidden_type
	| LCHAN hidden_type_non_recv_chan
	| LCHAN '(' /*10L*/ hidden_type_recv_chan ')' /*11L*/
	| LCHAN LCOMM /*1L*/ hidden_type
	;
hidden_type_recv_chan :
	 LCOMM /*1L*/ LCHAN hidden_type
	;
hidden_type_func :
	 LFUNC '(' /*10L*/ ohidden_funarg_list ')' /*11L*/ ohidden_funres
	;
hidden_funarg :
	 sym hidden_type oliteral
	| sym LDDD hidden_type oliteral
	;
hidden_structdcl :
	 sym hidden_type oliteral
	;
hidden_interfacedcl :
	 sym '(' /*10L*/ ohidden_funarg_list ')' /*11L*/ ohidden_funres
	| hidden_type
	;
ohidden_funres :
	 /*empty*/
	| hidden_funres
	;
hidden_funres :
	 '(' /*10L*/ ohidden_funarg_list ')' /*11L*/
	| hidden_type
	;
hidden_literal :
	 LLITERAL
	| '-' /*5L*/ LLITERAL
	| sym
	;
hidden_constant :
	 hidden_literal
	| '(' /*10L*/ hidden_literal '+' /*5L*/ hidden_literal ')' /*11L*/
	;
hidden_import_list :
	 /*empty*/
	| hidden_import_list hidden_import
	;
hidden_funarg_list :
	 hidden_funarg
	| hidden_funarg_list ',' hidden_funarg
	;
hidden_structdcl_list :
	 hidden_structdcl
	| hidden_structdcl_list ';' hidden_structdcl
	;
hidden_interfacedcl_list :
	 hidden_interfacedcl
	| hidden_interfacedcl_list ';' hidden_interfacedcl
	;

//Lexer

LCOMM : '<-' ;
LOROR : '||' ;
LANDAND : '&&' ;
LEQ : '==' ;
LGE : '>=' ;
LGT : '>' ;
LLE : '<=' ;
LLT : '<' ;
LNE : '!=' ;
LANDNOT : '&^' ;
LLSH : '<<' ;
LRSH : '>>' ;
LPACKAGE : 'package' ;
LIMPORT : 'import' ;
LLITERAL : "\"([^\"\n]|\\[^\n])*\"" ;
LLITERAL : "'([^'\n]|\\[^\n])'" ;
LLITERAL : "0[xX][a-fA-F0-9]+|[0-9]+([Ee][+-]?[0-9]+)?|[0-9]*\\.[0-9]+([Ee][+-]?[0-9]+)?" ;
LNAME : "[a-zA-Z_][a-zA-Z_0-9]*" ;
LVAR : 'var' ;
LTYPE : 'type' ;
LCONST : 'const' ;
LASOP : '=' ;
LCOLAS : ':=' ;
LINC : '++' ;
LDEC : '--' ;
LCASE : 'case' ;
LDEFAULT : 'default' ;
LBODY : 'LBODY' ; //'{' ;
LRANGE : 'range' ;
LFOR : 'for' ;
LIF : 'if' ;
LELSE : 'else' ;
LSWITCH : 'switch' ;
LSELECT : 'select' ;
LDDD : '...' ;
LCHAN : 'chan' ;
LMAP : 'map' ;
LSTRUCT : 'struct' ;
LINTERFACE : 'interface' ;
LFUNC : 'func' ;
LFALL : 'fallthrough' ;
LBREAK : 'break' ;
LCONTINUE : 'continue' ;
LGO : 'go' ;
LDEFER : 'defer' ;
LGOTO : 'goto' ;
LRETURN : 'return' ;

}
