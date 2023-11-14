cfront3 {

%whitespace "[ \t\n\r]*";
%whitespace "//[^\n\r]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

//%token EOFTOK
//%token ASM
//%token AUTO
//%token BREAK
//%token CASE
//%token CONTINUE
//%token DEFAULT
//%token DELETE
//%token DO
//%token ELSE
//%token ENUM
//%token FOR
//%token FORTRAN
//%token FRIEND
//%token GOTO
//%token IF
//%token NEW
//%token OPERATOR
//%token RETURN
//%token SIZEOF
//%token SWITCH
//%token THIS
//%token WHILE
//%token LP
//%token RP
//%token LB
//%token RB
//%token REF
//%token DOT
//%token NOT
//%token COMPL
//%token MUL
//%token AND
//%token PLUS
//%token MINUS
//%token LT
//%token GT
//%token ER
//%token OR
//%token ANDAND
//%token OROR
//%token QUEST
//%token COLON
//%token ASSIGN
//%token CM
//%token SM
//%token LC
//%token RC
//%token ID
//%token STRING
//%token ICON
//%token FCON
//%token CCON
//%token NAME
//%token ZERO
//%token ASOP
//%token RELOP
//%token EQUOP
//%token DIVOP
//%token SHIFTOP
//%token ICOP
//%token TYPE
//%token CATCH
//%token THROW
//%token TRY
//%token TNAME
//%token EMPTY
//%token NO_ID
//%token NO_EXPR
//%token FDEF
//%token ELLIPSIS
//%token AGGR
//%token MEM
//%token MEMPTR
//%token PR
//%token MEMQ
//%token TSCOPE
//%token DECL_MARKER
//%token REFMUL
//%token LDOUBLE
//%token LINKAGE
//%token TEMPLATE
//%token XVIRT
//%token XNLIST
//%token XILINE
//%token XIA
//%token SM_PARAM
//%token PTNAME
//%token NEW_INIT_KLUDGE
//%token XDELETED_NODE
//%token VEC_DELETE
//%token VEC_NEW
//%token DUMMY_LAST_NODE

%left /*1*/ EMPTY ;
%left /*2*/ NO_ID ;
%left /*3*/ BREAK CASE CONTINUE DEFAULT DELETE DO ENUM FOR GOTO IF RETURN WHILE LC RC ID TYPE TNAME AGGR TSCOPE ;
%left /*4*/ NO_EXPR ;
%left /*5*/ CM ;
%right /*6*/ ASSIGN ASOP ;
%right /*7*/ QUEST COLON ;
%left /*8*/ OROR ;
%left /*9*/ ANDAND ;
%left /*10*/ OR ;
%left /*11*/ ER ;
%left /*12*/ AND ;
%left /*13*/ EQUOP ;
%left /*14*/ LT GT RELOP ;
%left /*15*/ SHIFTOP ;
%left /*16*/ PLUS MINUS ;
%left /*17*/ MUL DIVOP MEMPTR ;
%left /*18*/ REFMUL ;
%right /*19*/ NEW NOT COMPL ;
%right /*20*/ SIZEOF ICOP ;
%left /*21*/ LP LB REF DOT MEM ;

//%start ext_def

//%%

ext_def :
	 external_def
	| SM
	| EOFTOK
	| LINKAGE LC /*3L*/
	| RC /*3L*/
	| template
	;
template :
	 TEMPLATE LT /*14L*/ template_parm_list GT /*14L*/ template_def
	;
template_def :
	 data_dcl
	| att_fct_def
	| fct_def
	| fct_dcl
	| class_dcl SM
	;
identifier :
	 ID /*3L*/
	| qualified_tname
	;
external_def :
	 data_dcl
	| att_fct_def
	| fct_def
	| fct_dcl
	| ASM LP /*21L*/ STRING RP SM
	;
fct_dcl :
	 decl ASSIGN /*6R*/ initializer SM
	| decl SM
	;
att_fct_def :
	 type decl arg_dcl_list check_inline base_init block
	| type decl arg_dcl_list check_inline EMPTY /*1L*/
	| type decl arg_dcl_list check_inline NO_ID /*2L*/
	;
fct_def :
	 decl arg_dcl_list check_inline base_init block
	| decl arg_dcl_list check_inline EMPTY /*1L*/
	| decl arg_dcl_list check_inline NO_ID /*2L*/
	;
inline_fct_def :
	 FDEF base_init block
	;
check_inline :
	 /*empty*/
	;
base_init :
	 COLON /*7R*/ init_list
	| /*empty*/ %prec EMPTY /*1L*/
	;
init_list :
	 binit
	| init_list CM /*5L*/ binit
	;
binit :
	 LP /*21L*/ elist RP
	| ttag LP /*21L*/ elist RP
	;
arg_dcl_list :
	 arg_dcl_list data_dcl
	| /*empty*/ %prec EMPTY /*1L*/
	;
dl :
	 decl
	;
dl :
	 ID /*3L*/ COLON /*7R*/ e %prec CM /*5L*/
	| COLON /*7R*/ e %prec CM /*5L*/
	;
dl :
	 decl ASSIGN /*6R*/ initializer
	;
decl_list :
	 dl
	| decl_list CM /*5L*/ dl
	;
data_dcl :
	 type decl_list SM
	| type SM
	;
lt :
	 LT /*14L*/
	;
gt :
	 GT /*14L*/
	;
tname :
	 qualified_tname
	| qualified_tname lt temp_inst_parms gt
	| NAME lt temp_inst_parms gt
	;
tp :
	 TYPE /*3L*/
	| LINKAGE
	| tname %prec NO_ID /*2L*/
	| tn_list DECL_MARKER
	| class_dcl
	| enum_dcl
	| DECL_MARKER
	;
type :
	 tp
	| type TYPE /*3L*/
	| type tname %prec NO_ID /*2L*/
	| type class_dcl
	| type enum_dcl
	| type DECL_MARKER
	;
cm :
	 CM /*5L*/
	;
temp_inst_parms :
	 temp_inst_parms cm temp_inst_parm
	| temp_inst_parm
	;
new_decl2 :
	 /*empty*/ %prec NO_ID /*2L*/
	| arg_lp new_decl2 RP
	| ptr new_decl2 %prec MUL /*17L*/
	| new_decl2 arg_list
	| new_decl2 vec %prec LB /*21L*/
	;
new_type2 :
	 type new_decl2
	;
temp_inst_parm :
	 new_type2
	| e %prec GT /*14L*/
	;
enumtag :
	 tag
	| DECL_MARKER
	| tn_list DECL_MARKER
	;
enum_dcl :
	 ENUM /*3L*/ LC /*3L*/ moe_list RC /*3L*/
	;
enum_dcl :
	 ENUM /*3L*/ enumtag LC /*3L*/ moe_list RC /*3L*/
	| ENUM /*3L*/ enumtag
	;
moe_list :
	 moe
	| moe_list CM /*5L*/ moe
	;
template_parm_list :
	 template_parm_list CM /*5L*/ template_parm
	| template_parm
	| /*empty*/
	;
template_parm :
	 AGGR /*3L*/ identifier
	| type formal_decl
	;
formal_decl :
	 ID /*3L*/
	| ptr formal_decl %prec MUL /*17L*/
	| formal_decl vec %prec LB /*21L*/
	| formal_decl arg_list
	| arg_lp formal_decl RP
	;
moe :
	 ID /*3L*/ %prec NO_ID /*2L*/
	;
moe :
	 ID /*3L*/ ASSIGN /*6R*/ e
	| /*empty*/
	;
class_dcl :
	 class_head cl_mem_list RC /*3L*/ inline_mem_defs
	| AGGR /*3L*/ tag
	| AGGR /*3L*/ qualified_tname lt temp_inst_parms gt
	| AGGR /*3L*/ DECL_MARKER
	;
inline_mem_defs :
	 /*empty*/
	| inline_mem_defs inline_fct_def
	;
base_list :
	 COLON /*7R*/ base_unit_list
	| /*empty*/ %prec EMPTY /*1L*/
	;
base_unit_list :
	 base_unit
	| base_unit_list CM /*5L*/ base_unit
	;
base_unit :
	 ttag
	| PR ttag
	| TYPE /*3L*/ ttag
	| PR TYPE /*3L*/ ttag
	| TYPE /*3L*/ PR ttag
	;
class_head :
	 AGGR /*3L*/ LC /*3L*/
	| AGGR /*3L*/ tag base_list LC /*3L*/
	| AGGR /*3L*/ qualified_tname lt temp_inst_parms gt base_list LC /*3L*/
	;
tag :
	 ID /*3L*/
	| qualified_tname
	;
ttag :
	 ID /*3L*/
	| tname
	;
cl_mem_list :
	 cl_mem_list cl_mem
	| /*empty*/ %prec EMPTY /*1L*/
	| cl_mem_list template
	;
cl_mem :
	 data_dcl
	| att_fct_def SM
	| fct_def SM
	| fct_def
	| att_fct_def
	| fct_dcl
	| PR COLON /*7R*/
	| scope_qualifiers fname SM
	;
fname :
	 ID /*3L*/
	| COMPL /*19R*/ TNAME /*3L*/
	| OPERATOR oper
	| OPERATOR c_type
	;
oper :
	 PLUS /*16L*/
	| MINUS /*16L*/
	| MUL /*17L*/
	| AND /*12L*/
	| OR /*10L*/
	| ER /*11L*/
	| SHIFTOP /*15L*/
	| EQUOP /*13L*/
	| DIVOP /*17L*/
	| RELOP /*14L*/
	| LT /*14L*/
	| GT /*14L*/
	| ANDAND /*9L*/
	| OROR /*8L*/
	| LP /*21L*/ RP
	| LB /*21L*/ RB
	| NOT /*19R*/
	| COMPL /*19R*/
	| ICOP /*20R*/
	| ASOP /*6R*/
	| ASSIGN /*6R*/
	| NEW /*19R*/
	| NEW /*19R*/ LB /*21L*/ RB
	| DELETE /*3L*/
	| DELETE /*3L*/ LB /*21L*/ RB
	| REF /*21L*/
	| CM /*5L*/
	| REFMUL /*18L*/
	;
scope_qualifiers :
	 tn_list
	;
tn_list :
	 tscope
	| tn_list tscope
	;
qualified_tname :
	 tn_list TNAME /*3L*/
	| TNAME /*3L*/
	;
fct_attributes :
	 /*empty*/
	| fct_attributes TYPE /*3L*/
	| fct_attributes THROW LP /*21L*/ type_list RP
	;
type_list :
	 tag
	| type_list CM /*5L*/ tag
	;
decl :
	 decl arg_list
	| decl LP /*21L*/ RP fct_attributes
	| tname arg_list
	| decl arg_lp elist RP
	| tname LP /*21L*/ MUL /*17L*/ ID /*3L*/ RP arg_list
	| tname LP /*21L*/ AND /*12L*/ ID /*3L*/ RP arg_list
	| tname LP /*21L*/ elist RP
	| tname LP /*21L*/ RP fct_attributes
	| tname LP /*21L*/ MEMPTR /*17L*/ decl RP arg_list
	| fname
	| ID /*3L*/ DOT /*21L*/ fname
	| scope_qualifiers fname
	| scope_qualifiers ID /*3L*/ DOT /*21L*/ fname
	| ptr decl %prec MUL /*17L*/
	| ptr tname %prec MUL /*17L*/
	| tname vec %prec LB /*21L*/
	| decl vec %prec LB /*21L*/
	| arg_lp decl RP
	;
arg_decl :
	 ID /*3L*/
	| ptr qualified_tname %prec MUL /*17L*/
	| /*empty*/ %prec NO_ID /*2L*/
	| ptr arg_decl %prec MUL /*17L*/
	| arg_decl vec %prec LB /*21L*/
	| arg_decl arg_list
	| arg_lp arg_decl RP
	;
new_decl :
	 /*empty*/ %prec NO_ID /*2L*/
	| ptr new_decl %prec MUL /*17L*/
	| new_decl vec %prec LB /*21L*/
	;
cast_decl :
	 /*empty*/ %prec NO_ID /*2L*/
	| ptr cast_decl %prec MUL /*17L*/
	| cast_decl vec %prec LB /*21L*/
	| LP /*21L*/ cast_decl RP arg_list
	| LP /*21L*/ cast_decl RP vec
	;
c_decl :
	 /*empty*/ %prec NO_ID /*2L*/
	| ptr c_decl %prec MUL /*17L*/
	;
stmt_list :
	 /*empty*/
	| stmt_list TEMPLATE
	| stmt_list caselab_stmt
	;
caselab_stmt :
	 caselablist statement
	;
caselablist :
	 /*empty*/
	;
condition :
	 LP /*21L*/ e RP
	;
block :
	 LC /*3L*/ stmt_list RC /*3L*/
	| LC /*3L*/ RC /*3L*/
	| LC /*3L*/ error RC /*3L*/
	;
simple :
	 ee
	| BREAK /*3L*/
	| CONTINUE /*3L*/
	| GOTO /*3L*/ ID /*3L*/
	;
simple :
	 DO /*3L*/ caselab_stmt WHILE /*3L*/ condition
	| ASM LP /*21L*/ STRING RP
	;
sm :
	 SM
	;
statement :
	 simple sm
	| SM
	| RETURN /*3L*/ e SM
	| TYPE /*3L*/ STRING block
	| data_dcl
	| att_fct_def
	| block
	| IF /*3L*/ condition caselab_stmt
	| IF /*3L*/ condition caselab_stmt ELSE caselab_stmt
	| WHILE /*3L*/ condition caselab_stmt
	;
statement :
	 FOR /*3L*/ LP /*21L*/ caselab_stmt e SM e RP caselab_stmt
	;
statement :
	 SWITCH condition caselab_stmt
	;
statement :
	 ID /*3L*/ COLON /*7R*/ caselab_stmt
	;
statement :
	 TNAME /*3L*/ COLON /*7R*/ caselab_stmt
	;
statement :
	 CASE /*3L*/ e COLON /*7R*/ caselab_stmt
	;
statement :
	 DEFAULT /*3L*/ COLON /*7R*/ caselab_stmt
	| TRY block handler_list
	;
handler_list :
	 /*empty*/
	| handler_list handler
	;
handler :
	 CATCH exception_type block
	;
exception_type :
	 arg_lp type arg_decl RP
	| LP /*21L*/ ELLIPSIS RP
	;
elist :
	 ex_list
	;
ex_list :
	 initializer %prec CM /*5L*/
	| ex_list CM /*5L*/ initializer
	;
initializer :
	 e %prec CM /*5L*/
	| LC /*3L*/ elist RC /*3L*/
	;
ee :
	 ee ASSIGN /*6R*/ ee
	| ee PLUS /*16L*/ ee
	| ee MINUS /*16L*/ ee
	| ee MUL /*17L*/ ee
	| ee AND /*12L*/ ee
	| ee OR /*10L*/ ee
	| ee ER /*11L*/ ee
	| ee SHIFTOP /*15L*/ ee
	| ee EQUOP /*13L*/ ee
	| ee DIVOP /*17L*/ ee
	| ee RELOP /*14L*/ ee
	| ee GT /*14L*/ ee
	| ee LT /*14L*/ ee
	| ee ANDAND /*9L*/ ee
	| ee OROR /*8L*/ ee
	| ee ASOP /*6R*/ ee
	| ee CM /*5L*/ ee
	| ee QUEST /*7R*/ ee COLON /*7R*/ ee
	| ee REFMUL /*18L*/ ee
	| DELETE /*3L*/ term
	| DELETE /*3L*/ LB /*21L*/ e RB term
	| MEM /*21L*/ DELETE /*3L*/ term
	| MEM /*21L*/ DELETE /*3L*/ LB /*21L*/ e RB term
	| term
	| THROW term
	;
e :
	 e ASSIGN /*6R*/ e
	| e PLUS /*16L*/ e
	| e MINUS /*16L*/ e
	| e MUL /*17L*/ e
	| e AND /*12L*/ e
	| e OR /*10L*/ e
	| e ER /*11L*/ e
	| e SHIFTOP /*15L*/ e
	| e EQUOP /*13L*/ e
	| e DIVOP /*17L*/ e
	| e RELOP /*14L*/ e
	| e LT /*14L*/ e
	| e GT /*14L*/ e
	| e ANDAND /*9L*/ e
	| e OROR /*8L*/ e
	| e ASOP /*6R*/ e
	| e CM /*5L*/ e
	| e QUEST /*7R*/ e COLON /*7R*/ e
	| e REFMUL /*18L*/ e
	| DELETE /*3L*/ term
	| DELETE /*3L*/ LB /*21L*/ e RB term
	| MEM /*21L*/ DELETE /*3L*/ term
	| MEM /*21L*/ DELETE /*3L*/ LB /*21L*/ e RB term
	| term
	| THROW term
	| /*empty*/ %prec NO_EXPR /*4L*/
	;
term :
	 NEW /*19R*/ cast_type
	| NEW /*19R*/ new_type
	| MEM /*21L*/ NEW /*19R*/ cast_type
	| MEM /*21L*/ NEW /*19R*/ new_type
	| term ICOP /*20R*/
	| cast_type term %prec ICOP /*20R*/
	| MUL /*17L*/ term
	| AND /*12L*/ term
	| MINUS /*16L*/ term
	| PLUS /*16L*/ term
	| NOT /*19R*/ term
	| COMPL /*19R*/ term
	| ICOP /*20R*/ term
	| SIZEOF /*20R*/ term
	| SIZEOF /*20R*/ cast_type %prec SIZEOF /*20R*/
	| term LB /*21L*/ e RB
	| term REF /*21L*/ prim
	| term REF /*21L*/ MEMQ prim
	| term REF /*21L*/ MEMQ TNAME /*3L*/
	| term REF /*21L*/ dtorspec
	| term REF /*21L*/ scope_qualifiers prim
	| term REF /*21L*/ scope_qualifiers TNAME /*3L*/
	| term DOT /*21L*/ prim
	| term DOT /*21L*/ MEMQ prim
	| term DOT /*21L*/ MEMQ TNAME /*3L*/
	| term DOT /*21L*/ dtorspec
	| term DOT /*21L*/ scope_qualifiers prim
	| term DOT /*21L*/ scope_qualifiers TNAME /*3L*/
	| prim
	| scope_qualifiers prim
	| tn_list COMPL /*19R*/ tag
	| tn_list COMPL /*19R*/ TYPE /*3L*/
	| term_elist
	| term_lp e RP
	| ZERO
	| ICON
	| FCON
	| STRING
	| CCON
	| THIS
	;
dtorspec :
	 COMPL /*19R*/ tag
	| MEMQ COMPL /*19R*/ tag
	| TYPE /*3L*/ MEM /*21L*/ COMPL /*19R*/ TYPE /*3L*/
	| COMPL /*19R*/ TYPE /*3L*/
	| TYPE /*3L*/ MEM /*21L*/ COMPL /*19R*/ tag
	| MEMQ COMPL /*19R*/ TYPE /*3L*/
	| tn_list COMPL /*19R*/ tag
	| tn_list COMPL /*19R*/ TYPE /*3L*/
	;
term_elist :
	 TYPE /*3L*/ LP /*21L*/ elist RP
	| tname LP /*21L*/ elist RP
	| NEW /*19R*/ term_lp elist RP cast_type
	| NEW /*19R*/ term_lp elist RP new_type
	| MEM /*21L*/ NEW /*19R*/ term_lp elist RP cast_type
	| MEM /*21L*/ NEW /*19R*/ term_lp elist RP new_type
	| term LP /*21L*/ elist RP
	;
ptname :
	 PTNAME lt temp_inst_parms gt
	;
tscope :
	 TSCOPE /*3L*/
	| MEM /*21L*/
	| ptname TSCOPE /*3L*/
	;
prim :
	 ID /*3L*/
	| OPERATOR oper
	| OPERATOR c_type
	;
cast_type :
	 term_lp type cast_decl RP
	;
term_lp :
	 LP /*21L*/
	;
c_tp :
	 TYPE /*3L*/
	| tname
	| c_tp TYPE /*3L*/
	| c_tp tname
	;
c_type :
	 c_tp c_decl
	;
new_type :
	 type new_decl
	;
arg_type :
	 type arg_decl
	;
arg_type :
	 type arg_decl ASSIGN /*6R*/ initializer
	;
arg_lp :
	 LP /*21L*/
	;
arg_list :
	 arg_lp arg_type_list ellipsis_opt RP fct_attributes
	;
arg_type_list :
	 arg_type_list CM /*5L*/ at
	| at %prec CM /*5L*/
	;
at :
	 arg_type
	| /*empty*/ %prec EMPTY /*1L*/
	;
ellipsis_opt :
	 /*empty*/
	| ELLIPSIS
	| CM /*5L*/ ELLIPSIS
	;
ptr :
	 MUL /*17L*/ %prec NO_ID /*2L*/
	| AND /*12L*/ %prec NO_ID /*2L*/
	| MUL /*17L*/ TYPE /*3L*/ %prec NO_ID /*2L*/
	| ptr TYPE /*3L*/ %prec NO_ID /*2L*/
	| AND /*12L*/ TYPE /*3L*/ %prec NO_ID /*2L*/
	| ptname MEMPTR /*17L*/ %prec NO_ID /*2L*/
	| MEMPTR /*17L*/ %prec NO_ID /*2L*/
	| ptname MEMPTR /*17L*/ TYPE /*3L*/ %prec NO_ID /*2L*/
	| MEMPTR /*17L*/ TYPE /*3L*/ %prec NO_ID /*2L*/
	;
vec :
	 LB /*21L*/ e RB
	| NOT /*19R*/ %prec LB /*21L*/
	;

//Lexer

//lalr (\d+:\d+): ERROR: undefined symbol '\([^']+\)'
EMPTY : 'EMPTY' ;
NO_ID : 'NO_ID' ;
BREAK : 'break' ;
CASE : 'case' ;
CONTINUE : 'continue' ;
DEFAULT : 'default' ;
DELETE : 'delete' ;
DO : 'do' ;
ENUM : 'enum' ;
FOR : 'for' ;
GOTO : 'goto' ;
IF : 'if' ;
RETURN : 'return' ;
WHILE : 'while' ;
LC : '{' ;
RC : '}' ;
ID : "[a-zA-Z_][a-zA-Z_0-9]*" ;
TYPE : 'TYPE' ;
TNAME : 'TNAME' ;
AGGR : 'AGGR' ;
TSCOPE : 'TSCOPE' ;
CM : ',' ;
ASSIGN : '=' ;
ASOP : 'ASOP' ;
QUEST : '?' ;
COLON : ':' ;
OROR : '||' ;
ANDAND : '&&' ;
OR : '|' ;
ER : '^' ;
AND : '&' ;
EQUOP : '==' ;
LT : '<' ;
GT : '>' ;
RELOP : 'RELOP' ;
SHIFTOP : 'SHIFTOP' ;
PLUS : '+' ;
MINUS : '-' ;
MUL : '*' ;
DIVOP : '/' ;
MEMPTR : 'MEMPTR' ;
REFMUL : 'REFMUL' ;
NEW : 'new' ;
NOT : '!' ;
COMPL : '~' ;
SIZEOF : 'sizeof' ;
ICOP : 'ICOP' ;
LP : ')' ;
LB : '[' ;
REF : 'REF' ;
DOT : '.' ;
MEM : 'MEM' ;
SM : ';' ;
EOFTOK : 'EOFTOK' ;
LINKAGE : 'LINKAGE' ;
TEMPLATE : 'template' ;
ASM : 'asm' ;
STRING : "\"(\\.|[^\"\n])*\"" ;
RP : '(' ;
FDEF : 'FDEF' ;
NAME : 'NAME' ;
DECL_MARKER : 'DECL_MARKER' ;
PR : 'PR' ;
OPERATOR : 'operator' ;
RB : ']' ;
THROW : 'throw' ;
ELSE : 'else' ;
SWITCH : 'switch' ;
TRY : 'try' ;
CATCH : 'catch' ;
ELLIPSIS : '...' ;
MEMQ : 'MEMQ' ;
ZERO : 'ZERO' ;
ICON : 'ICON' ;
FCON : 'FCON' ;
CCON : 'CCON' ;
THIS : 'this' ;
PTNAME : 'PTNAME' ;

}
