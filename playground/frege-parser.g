//From: https://github.com/catull/frege/blob/master/frege/compiler/grammar/Frege.y

frege {

%whitespace "[ \t\r\n]*";
%whitespace "//[^\n\r]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

//%token VARID
//%token CONID
//%token QUALIFIER
//%token DOCUMENTATION
//%token EXTENDS
//%token SUPER
//%token PACKAGE
//%token IMPORT
//%token INFIX
//%token INFIXR
//%token INFIXL
//%token NATIVE
//%token NEWTYPE
//%token DATA
//%token WHERE
//%token CLASS
//%token INSTANCE
//%token ABSTRACT
//%token TYPE
//%token TRUE
//%token FALSE
//%token IF
//%token THEN
//%token ELSE
//%token CASE
//%token OF
//%token DERIVE
//%token LET
//%token IN
//%token DO
//%token FORALL
//%token PRIVATE
//%token PROTECTED
//%token PUBLIC
//%token PURE
//%token THROWS
//%token MUTABLE
//%token INTCONST
//%token STRCONST
//%token LONGCONST
//%token FLTCONST
//%token DBLCONST
//%token DECCONST
//%token CHRCONST
//%token REGEXP
//%token BIGCONST
//%token ARROW
//%token DCOLON
//%token GETS
//%token EARROW
//%token DOTDOT
//%token LOP1
//%token LOP2
//%token LOP3
//%token LOP4
//%token LOP5
//%token LOP6
//%token LOP7
//%token LOP8
//%token LOP9
//%token LOP10
//%token LOP11
//%token LOP12
//%token LOP13
//%token LOP14
//%token LOP15
//%token LOP16
//%token ROP1
//%token ROP2
//%token ROP3
//%token ROP4
//%token ROP5
//%token ROP6
//%token ROP7
//%token ROP8
//%token ROP9
//%token ROP10
//%token ROP11
//%token ROP12
//%token ROP13
//%token ROP14
//%token ROP15
//%token ROP16
//%token NOP1
//%token NOP2
//%token NOP3
//%token NOP4
//%token NOP5
//%token NOP6
//%token NOP7
//%token NOP8
//%token NOP9
//%token NOP10
//%token NOP11
//%token NOP12
//%token NOP13
//%token NOP14
//%token NOP15
//%token NOP16
//%token NOP0
//%token LOP0
//%token ROP0
//%token SOMEOP
//%token '-'
//%token ';'
//%token '{'
//%token '}'
//%token '.'
//%token '('
//%token ')'
//%token ','
//%token '|'
//%token '['
//%token ']'
//%token '?'
//%token '!'
//%token '='
//%token '\\'
//%token '_'

%right /*1*/ SOMEOP '-' ;
%right /*2*/ ARROW ;

//%start module

//%%

module :
	 docsO moduleclause ';' definitions
	| docsO moduleclause WHERE '{' definitions '}'
	| docsO '{' definitions '}'
	;
nativename :
	 rawnativename
	;
rawnativename :
	 VARID
	| CONID
	| PACKAGE
	| VARID '.' rawnativename
	| PACKAGE '.' rawnativename
	| QUALIFIER rawnativename
	| STRCONST
	;
modulename1 :
	 CONID
	| varidkw '.' modulename1
	| QUALIFIER modulename1
	;
modulename :
	 modulename1
	;
docs :
	 DOCUMENTATION
	| DOCUMENTATION docs
	;
docsO :
	 /*empty*/
	| docs
	;
moduleclause :
	 PACKAGE modulename
	| PROTECTED PACKAGE modulename
	| moduleclause words '(' qvarids ')'
	;
word :
	 VARID
	;
words :
	 word
	| word words
	;
definitions :
	 definition
	| definition ';'
	| definition ';' definitions
	;
definition :
	 documentation
	| topdefinition
	| visibledefinition
	;
visibledefinition :
	 PRIVATE publicdefinition
	| PROTECTED publicdefinition
	| PUBLIC publicdefinition
	| ABSTRACT datadef
	;
topdefinition :
	 import
	| infix
	| moduledefinition
	| publicdefinition
	;
moduledefinition :
	 NATIVE PACKAGE typeclause interfaces wheretokens
	;
typeclause :
	 /*empty*/
	| TYPE tau
	;
interfaces :
	 /*empty*/
	| CLASS tauSC
	;
wheretokens :
	 WHERE '{' jtokens '}'
	| WHERE '{' '}'
	;
jtoken :
	 VARID
	| CONID
	| QUALIFIER
	| EXTENDS
	| SUPER
	| DOCUMENTATION
	| PACKAGE
	| IMPORT
	| INFIX
	| INFIXR
	| INFIXL
	| NATIVE
	| DATA
	| WHERE
	| CLASS
	| INSTANCE
	| ABSTRACT
	| TYPE
	| TRUE
	| FALSE
	| IF
	| THEN
	| ELSE
	| CASE
	| OF
	| DERIVE
	| LET
	| IN
	| DO
	| FORALL
	| PRIVATE
	| PROTECTED
	| PUBLIC
	| PURE
	| THROWS
	| MUTABLE
	| INTCONST
	| STRCONST
	| LONGCONST
	| FLTCONST
	| DBLCONST
	| CHRCONST
	| ARROW /*2R*/
	| DCOLON
	| GETS
	| EARROW
	| DOTDOT
	| SOMEOP /*1R*/
	| ','
	| '|'
	| '['
	| ']'
	| '('
	| ')'
	| '.'
	| '?'
	| '-' /*1R*/
	| ';'
	| '!'
	| '='
	| '\\'
	;
jtokens :
	 jtoken
	| jtoken jtokens
	| '{' jtokens '}'
	| '{' jtokens '}' jtokens
	| '{' '}'
	| '{' '}' jtokens
	;
documentation :
	 DOCUMENTATION
	;
publicdefinition :
	 typedef
	| datadef
	| classdef
	| instdef
	| derivedef
	| localdef
	;
localdefs :
	 dplocaldef
	| dplocaldef ';'
	| dplocaldef ';' localdefs
	;
localdef :
	 annotation
	| nativedef
	| fundef
	;
plocaldef :
	 localdef
	| PRIVATE localdef
	| PROTECTED localdef
	| PUBLIC localdef
	;
dplocaldef :
	 documentation
	| documentation dplocaldef
	| plocaldef
	;
letdef :
	 annotation
	| fundef
	;
letdefs :
	 letdef
	| letdef ';'
	| letdef ';' letdefs
	;
import :
	 IMPORT modulename importliste
	| IMPORT modulename VARID CONID importliste
	| IMPORT modulename CONID importliste
	;
importliste :
	 /*empty*/
	| varid '(' importspecs ')'
	| '(' ')'
	| '(' importspecs ')'
	| PUBLIC importliste
	;
importspecs :
	 importspec
	| importspec ','
	| importspec ',' importspecs
	;
importitem :
	 qvarid
	| CONID '(' memspecs ')'
	| CONID '(' ')'
	| qconid
	| operator
	| unop
	;
importspec :
	 importitem
	| importitem alias
	| PUBLIC importspec
	;
memspec :
	 alias
	| alias alias
	| PUBLIC memspec
	;
memspecs :
	 memspec
	| memspec ','
	| memspec ',' memspecs
	;
alias :
	 VARID
	| CONID
	| operator
	;
varid :
	 VARID
	;
varidkw :
	 VARID
	| DATA
	| TYPE
	| NATIVE
	| PURE
	| PACKAGE
	| IMPORT
	;
qvarids :
	 qvarop
	| qvarop ',' qvarids
	;
qvarid :
	 QUALIFIER QUALIFIER varop
	| QUALIFIER varop
	| VARID
	;
qconid :
	 QUALIFIER QUALIFIER CONID
	| QUALIFIER CONID
	| CONID
	;
varop :
	 VARID
	| unop
	;
qvarop :
	 QUALIFIER QUALIFIER varop
	| QUALIFIER varop
	| varop
	| operator
	;
operator :
	 SOMEOP /*1R*/
	;
unop :
	 '!'
	| '?'
	;
fixity :
	 INFIX INTCONST
	| INFIXL INTCONST
	| INFIXR INTCONST
	;
opstring :
	 operator
	| VARID
	| '-' /*1R*/
	;
operators :
	 opstring
	| opstring operators
	;
infix :
	 fixity operators
	;
annotation :
	 annoitems DCOLON sigma
	;
annoitem :
	 varid
	| '(' operator ')'
	| '(' unop ')'
	| '(' '-' /*1R*/ ')'
	;
annoitems :
	 annoitem
	| annoitem ',' annoitems
	;
nativedef :
	 PURE impurenativedef
	| impurenativedef
	;
fitem :
	 annoitem
	| unop
	| '-' /*1R*/
	| operator
	;
jitem :
	 nativename
	| operator
	| unop
	;
methodspec :
	 fitem jitem gargs
	| fitem jitem
	| fitem gargs
	| fitem
	;
sigex :
	 sigma THROWS tauSC
	| sigma
	;
sigexs :
	 sigex
	| sigex '|' sigexs
	;
impurenativedef :
	 NATIVE methodspec DCOLON sigexs
	;
sigma :
	 forall
	| rho
	;
forall :
	 FORALL dvars mbdot rho
	;
mbdot :
	 '.'
	| SOMEOP /*1R*/
	;
rho :
	 tapp EARROW rhofun
	| rhofun
	;
rhofun :
	 tapp
	| tapp ARROW /*2R*/ rhofun
	;
tau :
	 tapp
	| forall
	| tapp ARROW /*2R*/ tau
	;
tauSC :
	 tau
	| tau ',' tauSC
	;
tauSB :
	 tau
	| tau '|' tauSB
	;
tapp :
	 simpletypes
	;
simpletype :
	 tyvar
	| tyname
	| '(' tau ')'
	| '(' tau ',' tauSC ')'
	| '(' tau '|' tauSB ')'
	| '[' tau ']'
	;
tyvar :
	 VARID
	| '(' VARID DCOLON kind ')'
	| '(' VARID EXTENDS tauSC ')'
	| '(' EXTENDS tauSC ')'
	| '(' SUPER tapp ')'
	;
tyname :
	 qconid
	| '[' ']'
	| '(' ')'
	| '(' commata ')'
	| '(' ARROW /*2R*/ ')'
	;
kind :
	 simplekind ARROW /*2R*/ kind
	| simplekind
	;
simplekind :
	 SOMEOP /*1R*/
	| '(' kind ')'
	;
scontext :
	 qconid tyvar
	;
scontexts :
	 scontext
	| scontext ','
	| scontext ',' scontexts
	;
ccontext :
	 scontext
	| '(' scontexts ')'
	;
classdef :
	 CLASS ccontext EARROW CONID tyvar wheredef
	| CLASS ccontext wheredef
	;
sicontext :
	 qconid simpletype
	;
sicontexts :
	 sicontext
	| sicontext ','
	| sicontext ',' sicontexts
	;
icontext :
	 sicontext
	| '(' sicontexts ')'
	;
insthead :
	 icontext EARROW tyname simpletype
	| icontext
	;
instdef :
	 INSTANCE insthead wheredef
	;
derivedef :
	 DERIVE insthead
	;
datadef :
	 datainit wheredef
	;
nativepur :
	 PURE NATIVE
	| NATIVE
	;
nativespec :
	 nativename
	| nativename gargs
	;
gargs :
	 '{' tauSC '}'
	| '{' '}'
	;
datainit :
	 DATA CONID '=' nativepur nativespec
	| DATA CONID dvars '=' nativepur nativespec
	| DATA CONID dvars '=' dalts
	| DATA CONID '=' dalts
	| DATA CONID
	| NEWTYPE CONID dvars '=' dalt
	| NEWTYPE CONID '=' dalt
	;
dvars :
	 tyvar
	| tyvar dvars
	;
dalts :
	 dalt
	| dalt '|' dalts
	;
dalt :
	 visdalt
	| visdalt DOCUMENTATION
	| DOCUMENTATION visdalt
	;
visdalt :
	 strictdalt
	| PUBLIC strictdalt
	| PRIVATE strictdalt
	| PROTECTED strictdalt
	;
strictdalt :
	 '!' simpledalt
	| '?' simpledalt
	| simpledalt
	;
simpledalt :
	 CONID
	| CONID '{' conflds '}'
	| CONID contypes
	;
contypes :
	 strictcontype
	| strictcontype contypes
	;
strictcontype :
	 contype
	| '!' contype
	| '?' contype
	;
contype :
	 simpletype
	;
simpletypes :
	 simpletype
	| simpletype simpletypes
	;
conflds :
	 confld
	| confld ','
	| confld DOCUMENTATION
	| confld ',' conflds
	| confld DOCUMENTATION conflds
	;
confld :
	 docsO fldids DCOLON sigma
	;
fldids :
	 fldid
	| fldid ',' fldids
	;
fldid :
	 strictfldid
	| PUBLIC strictfldid
	| PRIVATE strictfldid
	;
strictfldid :
	 plainfldid
	| '!' plainfldid
	| '?' plainfldid
	;
plainfldid :
	 varid
	;
typedef :
	 TYPE CONID '=' sigma
	| TYPE CONID dvars '=' sigma
	;
wheredef :
	 /*empty*/
	| WHERE '{' '}'
	| WHERE '{' localdefs '}'
	;
wherelet :
	 WHERE '{' '}'
	| WHERE '{' letdefs '}'
	;
fundef :
	 funhead '=' expr
	| funhead guards
	| fundef wherelet
	;
funhead :
	 binex
	;
literal :
	 TRUE
	| FALSE
	| CHRCONST
	| STRCONST
	| INTCONST
	| BIGCONST
	| LONGCONST
	| FLTCONST
	| DBLCONST
	| DECCONST
	| REGEXP
	;
pattern :
	 expr
	;
aeq :
	 ARROW /*2R*/
	| '='
	;
lcqual :
	 gqual
	| expr '=' expr
	| LET '{' letdefs '}'
	;
lcquals :
	 lcqual
	| lcqual ',' lcquals
	| lcqual ','
	;
dodefs :
	 lcqual
	| lcqual ';'
	| lcqual ';' dodefs
	;
gqual :
	 expr
	| expr GETS expr
	;
gquals :
	 gqual
	| gqual ',' gquals
	| gqual ','
	;
guard :
	 '|' gquals aeq expr
	;
guards :
	 guard
	| guard guards
	;
calt :
	 pattern aeq expr
	| pattern guards
	| calt wherelet
	;
calts :
	 calt
	| calt ';' calts
	| calt ';'
	;
lambda :
	 '\\' apats lambdabody
	;
lambdabody :
	 lambda
	| ARROW /*2R*/ expr
	;
expr :
	 binex DCOLON sigma
	| binex
	;
thenx :
	 ';' THEN
	| THEN
	;
elsex :
	 ';' ELSE
	| ELSE
	;
binex :
	 binex SOMEOP /*1R*/ binex
	| binex '-' /*1R*/ binex
	| '-' /*1R*/ topex
	| topex
	;
topex :
	 IF expr thenx expr elsex expr
	| CASE expr OF '{' calts '}'
	| LET '{' letdefs '}' IN expr
	| lambda
	| appex
	;
appex :
	 unex
	| appex unex
	;
unex :
	 primary
	| unop unex
	;
apats :
	 unex
	| unex apats
	;
qualifiers :
	 QUALIFIER
	| QUALIFIER QUALIFIER
	;
primary :
	 term
	| DO '{' dodefs '}'
	| primary '.' VARID
	| primary '.' operator
	| primary '.' unop
	| qualifiers '{' VARID '?' '}'
	| qualifiers '{' VARID '=' '}'
	| qualifiers '{' VARID GETS '}'
	| qualifiers '{' getfields '}'
	| primary '.' '{' VARID '?' '}'
	| primary '.' '{' VARID '=' '}'
	| primary '.' '{' VARID GETS '}'
	| primary '.' '{' getfields '}'
	| primary '.' '[' expr ']'
	;
term :
	 qvarid
	| literal
	| '_'
	| qconid
	| qconid '{' '}'
	| qconid '{' fields '}'
	| '(' ')'
	| '(' commata ')'
	| '(' unop ')'
	| '(' operator ')'
	| '(' '-' /*1R*/ ')'
	| '(' operator expr ')'
	| '(' binex operator ')'
	| '(' binex '-' /*1R*/ ')'
	| '(' expr ',' exprSC ')'
	| '(' expr ';' exprSS ')'
	| '(' expr ')'
	| '[' ']'
	| '[' exprSC ']'
	| '[' exprSC DOTDOT ']'
	| '[' exprSC DOTDOT expr ']'
	| '[' expr '|' lcquals ']'
	;
commata :
	 ','
	| ',' commata
	;
fields :
	 field
	| field ',' fields
	| field ','
	;
getfields :
	 getfield
	| getfield ',' getfields
	| getfield ','
	;
getfield :
	 VARID GETS expr
	| VARID '=' expr
	| VARID
	;
field :
	 varid '=' expr
	| varid
	;
exprSC :
	 expr
	| expr ',' exprSC
	| expr ','
	;
exprSS :
	 expr
	| expr ';' exprSS
	| expr ';'
	;

//Lexer

SOMEOP : 'SOMEOP' ;
ARROW : 'ARROW' ;
WHERE : 'WHERE' ;
VARID : 'VARID' ;
CONID : 'CONID' ;
PACKAGE : 'PACKAGE' ;
QUALIFIER : 'QUALIFIER' ;
STRCONST : 'STRCONST' ;
DOCUMENTATION : 'DOCUMENTATION' ;
PROTECTED : 'PROTECTED' ;
PRIVATE : 'PRIVATE' ;
PUBLIC : 'PUBLIC' ;
ABSTRACT : 'ABSTRACT' ;
NATIVE : 'NATIVE' ;
TYPE : 'TYPE' ;
CLASS : 'CLASS' ;
EXTENDS : 'EXTENDS' ;
SUPER : 'SUPER' ;
IMPORT : 'IMPORT' ;
INFIX : 'INFIX' ;
INFIXR : 'INFIXR' ;
INFIXL : 'INFIXL' ;
DATA : 'DATA' ;
INSTANCE : 'INSTANCE' ;
TRUE : 'TRUE' ;
FALSE : 'FALSE' ;
IF : 'IF' ;
THEN : 'THEN' ;
ELSE : 'ELSE' ;
CASE : 'CASE' ;
OF : 'OF' ;
DERIVE : 'DERIVE' ;
LET : 'LET' ;
IN : 'IN' ;
DO : 'DO' ;
FORALL : 'FORALL' ;
PURE : 'PURE' ;
THROWS : 'THROWS' ;
MUTABLE : 'MUTABLE' ;
INTCONST : 'INTCONST' ;
LONGCONST : 'LONGCONST' ;
FLTCONST : 'FLTCONST' ;
DBLCONST : 'DBLCONST' ;
CHRCONST : 'CHRCONST' ;
DCOLON : 'DCOLON' ;
GETS : 'GETS' ;
EARROW : 'EARROW' ;
DOTDOT : 'DOTDOT' ;
NEWTYPE : 'NEWTYPE' ;
BIGCONST : 'BIGCONST' ;
DECCONST : 'DECCONST' ;
REGEXP : 'REGEXP' ;

}
