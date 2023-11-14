gringoNonGroundedInput {

%whitespace "[ \t\r\n]*";
%whitespace "%[^\n]*";

/*Tokens*/
//%token END
//%token YYerror
//%token YYUNDEF
//%token ADD
//%token AND
//%token EQ
//%token AT
//%token BASE
//%token BNOT
//%token COLON
//%token COMMA
//%token CONST
//%token COUNT
//%token CUMULATIVE
//%token DOT
//%token DOTS
//%token EXTERNAL
//%token DEFINED
//%token FALSE
//%token FORGET
//%token GEQ
//%token GT
//%token IF
//%token INCLUDE
//%token INFIMUM
//%token LBRACE
//%token LBRACK
//%token LEQ
//%token LPAREN
//%token LT
//%token MAX
//%token MAXIMIZE
//%token MIN
//%token MINIMIZE
//%token MOD
//%token MUL
//%token NEQ
//%token POW
//%token QUESTION
//%token RBRACE
//%token RBRACK
//%token RPAREN
//%token SEM
//%token SHOW
//%token EDGE
//%token PROJECT
//%token HEURISTIC
//%token SHOWSIG
//%token SLASH
//%token SUB
//%token SUM
//%token SUMP
//%token SUPREMUM
//%token TRUE
//%token BLOCK
//%token UBNOT
//%token UMINUS
//%token VBAR
//%token VOLATILE
//%token WIF
//%token XOR
//%token PARSE_LP
//%token PARSE_DEF
//%token ANY
//%token UNARY
//%token BINARY
//%token LEFT
//%token RIGHT
//%token HEAD
//%token BODY
//%token DIRECTIVE
//%token THEORY
//%token SYNC
//%token NUMBER
//%token ANONYMOUS
//%token IDENTIFIER
//%token SCRIPT
//%token CODE
//%token STRING
//%token VARIABLE
//%token THEORY_OP
//%token NOT
//%token DEFAULT
//%token OVERRIDE

%left /*1*/ DOTS ;
%left /*2*/ XOR ;
%left /*3*/ QUESTION ;
%left /*4*/ AND ;
%left /*5*/ ADD SUB ;
%left /*6*/ MOD MUL SLASH ;
%right /*7*/ POW ;
%left /*8*/ UBNOT UMINUS ;

//%start start

//%%

start :
	/*END
	|*/ PARSE_LP program
	| PARSE_DEF define SYNC
	;

program :
	program statement
	| /*empty*/
	;

statement :
	SYNC
	| DOT
	| error disable_theory_lexing DOT
	| error disable_theory_lexing SYNC
	;

identifier :
	IDENTIFIER
	| DEFAULT
	| OVERRIDE
	;

constterm :
	constterm XOR /*2L*/ constterm
	| constterm QUESTION /*3L*/ constterm
	| constterm AND /*4L*/ constterm
	| constterm ADD /*5L*/ constterm
	| constterm SUB /*5L*/ constterm
	| constterm MUL /*6L*/ constterm
	| constterm SLASH /*6L*/ constterm
	| constterm MOD /*6L*/ constterm
	| constterm POW /*7R*/ constterm
	| SUB /*5L*/ constterm %prec UMINUS /*8L*/
	| BNOT constterm %prec UBNOT /*8L*/
	| LPAREN RPAREN
	| LPAREN COMMA RPAREN
	| LPAREN consttermvec RPAREN
	| LPAREN consttermvec COMMA RPAREN
	| identifier LPAREN constargvec RPAREN
	| AT identifier LPAREN constargvec RPAREN
	| VBAR constterm VBAR
	| identifier
	| AT identifier
	| NUMBER
	| STRING
	| INFIMUM
	| SUPREMUM
	;

consttermvec :
	constterm
	| consttermvec COMMA constterm
	;

constargvec :
	consttermvec
	| /*empty*/
	;

term :
	term DOTS /*1L*/ term
	| term XOR /*2L*/ term
	| term QUESTION /*3L*/ term
	| term AND /*4L*/ term
	| term ADD /*5L*/ term
	| term SUB /*5L*/ term
	| term MUL /*6L*/ term
	| term SLASH /*6L*/ term
	| term MOD /*6L*/ term
	| term POW /*7R*/ term
	| SUB /*5L*/ term %prec UMINUS /*8L*/
	| BNOT term %prec UBNOT /*8L*/
	| LPAREN tuplevec RPAREN
	| identifier LPAREN argvec RPAREN
	| AT identifier LPAREN argvec RPAREN
	| VBAR unaryargvec VBAR
	| identifier
	| AT identifier
	| NUMBER
	| STRING
	| INFIMUM
	| SUPREMUM
	| VARIABLE
	| ANONYMOUS
	;

unaryargvec :
	term
	| unaryargvec SEM term
	;

ntermvec :
	term
	| ntermvec COMMA term
	;

termvec :
	ntermvec
	| /*empty*/
	;

tuple :
	ntermvec COMMA
	| ntermvec
	| COMMA
	| /*empty*/
	;

tuplevec_sem :
	tuple SEM
	| tuplevec_sem tuple SEM
	;

tuplevec :
	tuple
	| tuplevec_sem tuple
	;

argvec :
	termvec
	| argvec SEM termvec
	;

binaryargvec :
	term COMMA term
	| binaryargvec SEM term COMMA term
	;

cmp :
	GT
	| LT
	| GEQ
	| LEQ
	| EQ
	| NEQ
	;

atom :
	identifier
	| identifier LPAREN argvec RPAREN
	| SUB /*5L*/ identifier
	| SUB /*5L*/ identifier LPAREN argvec RPAREN
	;

rellitvec :
	cmp term
	| rellitvec cmp term
	;

literal :
	TRUE
	| NOT TRUE
	| NOT NOT TRUE
	| FALSE
	| NOT FALSE
	| NOT NOT FALSE
	| atom
	| NOT atom
	| NOT NOT atom
	| term rellitvec
	| NOT term rellitvec
	| NOT NOT term rellitvec
	;

nlitvec :
	literal
	| nlitvec COMMA literal
	;

litvec :
	nlitvec
	| /*empty*/
	;

optcondition :
	COLON litvec
	| /*empty*/
	;

aggregatefunction :
	SUM
	| SUMP
	| MIN
	| MAX
	| COUNT
	;

bodyaggrelem :
	COLON litvec
	| ntermvec optcondition
	;

bodyaggrelemvec :
	bodyaggrelem
	| bodyaggrelemvec SEM bodyaggrelem
	;

altbodyaggrelem :
	literal optcondition
	;

altbodyaggrelemvec :
	altbodyaggrelem
	| altbodyaggrelemvec SEM altbodyaggrelem
	;

bodyaggregate :
	LBRACE RBRACE
	| LBRACE altbodyaggrelemvec RBRACE
	| aggregatefunction LBRACE RBRACE
	| aggregatefunction LBRACE bodyaggrelemvec RBRACE
	;

upper :
	term
	| cmp term
	| /*empty*/
	;

lubodyaggregate :
	term bodyaggregate upper
	| term cmp bodyaggregate upper
	| bodyaggregate upper
	| theory_atom
	;

headaggrelemvec :
	headaggrelemvec SEM termvec COLON literal optcondition
	| termvec COLON literal optcondition
	;

altheadaggrelemvec :
	literal optcondition
	| altheadaggrelemvec SEM literal optcondition
	;

headaggregate :
	aggregatefunction LBRACE RBRACE
	| aggregatefunction LBRACE headaggrelemvec RBRACE
	| LBRACE RBRACE
	| LBRACE altheadaggrelemvec RBRACE
	;

luheadaggregate :
	term headaggregate upper
	| term cmp headaggregate upper
	| headaggregate upper
	| theory_atom
	;

conjunction :
	literal COLON litvec
	;

dsym :
	SEM
	| VBAR
	;

disjunctionsep :
	disjunctionsep literal COMMA
	| disjunctionsep literal dsym
	| disjunctionsep literal COLON SEM
	| disjunctionsep literal COLON nlitvec dsym
	| literal COMMA
	| literal dsym
	| literal COLON nlitvec dsym
	| literal COLON SEM
	;

disjunction :
	disjunctionsep literal optcondition
	| literal COLON litvec
	;

bodycomma :
	bodycomma literal COMMA
	| bodycomma literal SEM
	| bodycomma lubodyaggregate COMMA
	| bodycomma lubodyaggregate SEM
	| bodycomma NOT lubodyaggregate COMMA
	| bodycomma NOT lubodyaggregate SEM
	| bodycomma NOT NOT lubodyaggregate COMMA
	| bodycomma NOT NOT lubodyaggregate SEM
	| bodycomma conjunction SEM
	| /*empty*/
	;

bodydot :
	bodycomma literal DOT
	| bodycomma lubodyaggregate DOT
	| bodycomma NOT lubodyaggregate DOT
	| bodycomma NOT NOT lubodyaggregate DOT
	| bodycomma conjunction DOT
	;

bodyconddot :
	DOT
	| COLON DOT
	| COLON bodydot
	;

head :
	literal
	| disjunction
	| luheadaggregate
	;

statement :
	head DOT
	| head IF DOT
	| head IF bodydot
	| IF bodydot
	| IF DOT
	;

optimizetuple :
	COMMA ntermvec
	| /*empty*/
	;

optimizeweight :
	term AT term
	| term
	;

optimizelitvec :
	literal
	| optimizelitvec COMMA literal
	;

optimizecond :
	COLON optimizelitvec
	| COLON
	| /*empty*/
	;

statement :
	WIF bodydot LBRACK optimizeweight optimizetuple RBRACK
	| WIF DOT LBRACK optimizeweight optimizetuple RBRACK
	;

maxelemlist :
	optimizeweight optimizetuple optimizecond
	| maxelemlist SEM optimizeweight optimizetuple optimizecond
	;

minelemlist :
	optimizeweight optimizetuple optimizecond
	| minelemlist SEM optimizeweight optimizetuple optimizecond
	;

statement :
	MINIMIZE LBRACE RBRACE DOT
	| MAXIMIZE LBRACE RBRACE DOT
	| MINIMIZE LBRACE minelemlist RBRACE DOT
	| MAXIMIZE LBRACE maxelemlist RBRACE DOT
	| SHOWSIG identifier SLASH /*6L*/ NUMBER DOT
	| SHOWSIG SUB /*5L*/ identifier SLASH /*6L*/ NUMBER DOT
	| SHOW DOT
	| SHOW term COLON bodydot
	| SHOW term DOT
	| DEFINED identifier SLASH /*6L*/ NUMBER DOT
	| DEFINED SUB /*5L*/ identifier SLASH /*6L*/ NUMBER DOT
	| EDGE LPAREN binaryargvec RPAREN bodyconddot
	| HEURISTIC atom bodyconddot LBRACK term AT term COMMA term RBRACK
	| HEURISTIC atom bodyconddot LBRACK term COMMA term RBRACK
	| PROJECT identifier SLASH /*6L*/ NUMBER DOT
	| PROJECT SUB /*5L*/ identifier SLASH /*6L*/ NUMBER DOT
	| PROJECT atom bodyconddot
	;

define :
	identifier EQ constterm
	;

statement :
	CONST identifier EQ constterm DOT
	| CONST identifier EQ constterm DOT LBRACK DEFAULT RBRACK
	| CONST identifier EQ constterm DOT LBRACK OVERRIDE RBRACK
	| SCRIPT LPAREN IDENTIFIER RPAREN CODE DOT
	| INCLUDE STRING DOT
	| INCLUDE LT identifier GT DOT
	;

nidlist :
	nidlist COMMA identifier
	| identifier
	;

idlist :
	/*empty*/
	| nidlist
	;

statement :
	BLOCK identifier LPAREN idlist RPAREN DOT
	| BLOCK identifier DOT
	| EXTERNAL atom COLON bodydot
	| EXTERNAL atom COLON DOT
	| EXTERNAL atom DOT
	| EXTERNAL atom COLON bodydot LBRACK term RBRACK
	| EXTERNAL atom COLON DOT LBRACK term RBRACK
	| EXTERNAL atom DOT LBRACK term RBRACK
	;

theory_op :
	THEORY_OP
	| NOT
	;

theory_op_list :
	theory_op_list theory_op
	| theory_op
	;

theory_term :
	LBRACE theory_opterm_list RBRACE
	| LBRACK theory_opterm_list RBRACK
	| LPAREN RPAREN
	| LPAREN theory_opterm RPAREN
	| LPAREN theory_opterm COMMA RPAREN
	| LPAREN theory_opterm COMMA theory_opterm_nlist RPAREN
	| identifier LPAREN theory_opterm_list RPAREN
	| identifier
	| NUMBER
	| STRING
	| INFIMUM
	| SUPREMUM
	| VARIABLE
	;

theory_opterm :
	theory_opterm theory_op_list theory_term
	| theory_op_list theory_term
	| theory_term
	;

theory_opterm_nlist :
	theory_opterm_nlist COMMA theory_opterm
	| theory_opterm
	;

theory_opterm_list :
	theory_opterm_nlist
	| /*empty*/
	;

theory_atom_element :
	theory_opterm_nlist disable_theory_lexing optcondition
	| disable_theory_lexing COLON litvec
	;

theory_atom_element_nlist :
	theory_atom_element_nlist enable_theory_lexing SEM theory_atom_element
	| theory_atom_element
	;

theory_atom_element_list :
	theory_atom_element_nlist
	| /*empty*/
	;

theory_atom_name :
	identifier
	| identifier LPAREN argvec RPAREN
	;

theory_atom :
	AND /*4L*/ theory_atom_name
	| AND /*4L*/ theory_atom_name enable_theory_lexing LBRACE theory_atom_element_list enable_theory_lexing RBRACE disable_theory_lexing
	| AND /*4L*/ theory_atom_name enable_theory_lexing LBRACE theory_atom_element_list enable_theory_lexing RBRACE theory_op theory_opterm disable_theory_lexing
	;

theory_operator_nlist :
	theory_op
	| theory_operator_nlist COMMA theory_op
	;

theory_operator_list :
	theory_operator_nlist
	| /*empty*/
	;

theory_operator_definition :
	theory_op enable_theory_definition_lexing COLON NUMBER COMMA UNARY
	| theory_op enable_theory_definition_lexing COLON NUMBER COMMA BINARY COMMA LEFT
	| theory_op enable_theory_definition_lexing COLON NUMBER COMMA BINARY COMMA RIGHT
	;

theory_operator_definition_nlist :
	theory_operator_definition
	| theory_operator_definition_nlist enable_theory_lexing SEM theory_operator_definition
	;

theory_operator_definiton_list :
	theory_operator_definition_nlist
	| /*empty*/
	;

theory_definition_identifier :
	identifier
	| LEFT
	| RIGHT
	| UNARY
	| BINARY
	| HEAD
	| BODY
	| ANY
	| DIRECTIVE
	;

theory_term_definition :
	theory_definition_identifier enable_theory_lexing LBRACE theory_operator_definiton_list enable_theory_definition_lexing RBRACE
	;

theory_atom_type :
	HEAD
	| BODY
	| ANY
	| DIRECTIVE
	;

theory_atom_definition :
	AND /*4L*/ theory_definition_identifier SLASH /*6L*/ NUMBER COLON theory_definition_identifier COMMA enable_theory_lexing LBRACE theory_operator_list enable_theory_definition_lexing RBRACE COMMA theory_definition_identifier COMMA theory_atom_type
	| AND /*4L*/ theory_definition_identifier SLASH /*6L*/ NUMBER COLON theory_definition_identifier COMMA theory_atom_type
	;

theory_definition_nlist :
	theory_definition_list SEM theory_atom_definition
	| theory_definition_list SEM theory_term_definition
	| theory_atom_definition
	| theory_term_definition
	;

theory_definition_list :
	theory_definition_nlist
	| /*empty*/
	;

statement :
	THEORY identifier enable_theory_definition_lexing LBRACE theory_definition_list RBRACE disable_theory_lexing DOT
	;

enable_theory_lexing :
	/*empty*/
	;

enable_theory_definition_lexing :
	/*empty*/
	;

disable_theory_lexing :
	/*empty*/
	;
//Lexer

ADD : '+' ;
AND : '&' ;
EQ : '=' ;
AT : '@' ;
//BASE : '#base' ;
BNOT : '~' ;
COLON : ':' ;
COMMA : ',' ;
CONST : '#const' ;
COUNT : '#count' ;
//CUMULATIVE : '#cumulative' ;
DOT : '.' ;
DOTS : '..' ;
EXTERNAL : '#external' ;
DEFINED : '#defined' ;
FALSE : '#false' ;
//FORGET : '#forget' ;
GEQ : '>=' ;
GT : '>' ;
IF : ':-' ;
INCLUDE : '#include' ;
INFIMUM : '#inf' ;
LBRACE : '{' ;
LBRACK : '[' ;
LEQ : '<=' ;
LPAREN : '(' ;
LT : '<' ;
MAX : '#max' ;
MAXIMIZE : '#maximize' ;
MIN : '#min' ;
MINIMIZE : '#minimize' ;
MOD : '\\' ;
MUL : '*' ;
NEQ : '!=' ;
POW : '**' ;
QUESTION : '?' ;
RBRACE : '}' ;
RBRACK : ']' ;
RPAREN : ')' ;
SEM : ';' ;
SHOW : '#show' ;
EDGE : '#edge' ;
PROJECT : '#project' ;
HEURISTIC : '#heuristic' ;
SHOWSIG : '#showsig' ;
SLASH : '/' ;
SUB : '-' ;
SUM : '#sum' ;
SUMP : '#sum+' ;
SUPREMUM : '#sup' ;
TRUE : '#true' ;
BLOCK : '#program' ;
VBAR : '|' ;
//VOLATILE : '#volatile' ;
WIF : ':~' ;
XOR : '^' ;
PARSE_LP : '#program' ;
PARSE_DEF : '#define' ;
ANY : 'any' ;
UNARY : 'unary' ;
BINARY : 'binary' ;
LEFT : 'left' ;
RIGHT : 'right' ;
HEAD : 'head' ;
BODY : 'body' ;
DIRECTIVE : 'directive' ;
THEORY : '#theory' ;

SYNC : 'EOF' ;
IDENTIFIER : "[_']*[a-z]['A-Za-z0-9_]*" ;
DEFAULT : 'default' ;
OVERRIDE : 'override' ;
NUMBER : "(0|([1-9][0-9]*))|(0x([0-9A-Fa-f]+))|(0o([1-7]+))|(0b([0-1]+))" ;
STRING : "\"[^\"\n]*\"|'[^'\n]*'" ;
VARIABLE : "[_']*[A-Z]['A-Za-z0-9_]*" ;
ANONYMOUS : '_' ;
NOT : 'not' ;
SCRIPT : '#script' ;
CODE : 'CODE' ;
THEORY_OP : "[/!<=>+\-*\\?&@|:;~\^\.]+" ;

}