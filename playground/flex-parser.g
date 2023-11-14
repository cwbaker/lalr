//From: https://github.com/westes/flex/blob/master/src/parse.y

flex {

%error_recovery_off;
//%error_recovery_show;
%whitespace "[ \t\r\n]*";
%whitespace "//[^\n\r]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

/*Tokens*/
//%token YYEOF
//%token YYerror
//%token YYUNDEF
//%token CHAR
//%token NUMBER
//%token SECTEND
//%token SCDECL
//%token XSCDECL
//%token NAME
//%token PREVCCL
//%token EOF_OP
//%token TOK_OPTION
//%token TOK_OUTFILE
//%token TOK_PREFIX
//%token TOK_YYCLASS
//%token TOK_HEADER_FILE
//%token TOK_EXTRA_TYPE
//%token TOK_TABLES_FILE
//%token TOK_YYLMAX
//%token TOK_NUMERIC
//%token TOK_YYDECL
//%token TOK_PREACTION
//%token TOK_POSTACTION
//%token TOK_USERINIT
//%token TOK_EMIT
//%token TOK_BUFSIZE
//%token TOK_YYTERMINATE
//%token CCE_ALNUM
//%token CCE_ALPHA
//%token CCE_BLANK
//%token CCE_CNTRL
//%token CCE_DIGIT
//%token CCE_GRAPH
//%token CCE_LOWER
//%token CCE_PRINT
//%token CCE_PUNCT
//%token CCE_SPACE
//%token CCE_UPPER
//%token CCE_XDIGIT
//%token CCE_NEG_ALNUM
//%token CCE_NEG_ALPHA
//%token CCE_NEG_BLANK
//%token CCE_NEG_CNTRL
//%token CCE_NEG_DIGIT
//%token CCE_NEG_GRAPH
//%token CCE_NEG_LOWER
//%token CCE_NEG_PRINT
//%token CCE_NEG_PUNCT
//%token CCE_NEG_SPACE
//%token CCE_NEG_UPPER
//%token CCE_NEG_XDIGIT
//%token CCL_OP_DIFF
//%token CCL_OP_UNION
//%token BEGIN_REPEAT_POSIX
//%token END_REPEAT_POSIX
//%token BEGIN_REPEAT_FLEX
//%token END_REPEAT_FLEX
//%token '='
//%token '\n'
//%token '{'
//%token '}'
//%token '^'
//%token '<'
//%token '>'
//%token '*'
//%token ','
//%token '$'
//%token '|'
//%token '/'
//%token '+'
//%token '?'
//%token '.'
//%token '"'
//%token '('
//%token ')'
//%token '['
//%token ']'
//%token '-'

%left /*1*/ CCL_OP_DIFF CCL_OP_UNION ;

//%start goal

//%%

goal :
	/*YYEOF
	|*/ initlex sect1 sect1end sect2 initforrule
	;

initlex :
	/*empty*/
	| top_user_code
	;

sect1 :
	sect1 startconddecl namelist1
	| sect1 options
	| /*empty*/
	| error
	;

sect1end :
	SECTEND
	;

startconddecl :
	SCDECL
	| XSCDECL
	;

namelist1 :
	namelist1 NAME
	| NAME
	| error
	;

options :
	TOK_OPTION optionlist
	| user_code
	;

optionlist :
	optionlist option
	| /*empty*/
	;

option :
	TOK_OUTFILE '=' NAME
	| TOK_EXTRA_TYPE '=' NAME
	| TOK_PREFIX '=' NAME
	| TOK_YYCLASS '=' NAME
	| TOK_HEADER_FILE '=' NAME
	| TOK_YYLMAX '=' TOK_NUMERIC
	| TOK_YYDECL '=' NAME
	| TOK_PREACTION '=' NAME
	| TOK_POSTACTION '=' NAME
	| TOK_BUFSIZE '=' TOK_NUMERIC
	| TOK_EMIT '=' NAME
	| TOK_USERINIT '=' NAME
	| TOK_YYTERMINATE '=' NAME
	| TOK_TABLES_FILE '=' NAME
	;

sect2 :
	sect2 scon initforrule flexrule '\n'
	| sect2 scon '{' sect2 '}'
	| /*empty*/
	;

initforrule :
	/*empty*/
	;

flexrule :
	'^' rule
	| rule
	| EOF_OP
	| error
	;

scon_stk_ptr :
	/*empty*/
	;

scon :
	'<' scon_stk_ptr namelist2 '>'
	| '<*>'
	| /*empty*/
	;

namelist2 :
	namelist2 ',' sconname
	| sconname
	| error
	;

sconname :
	NAME
	;

rule :
	re2 re
	| re2 re '$'
	| re '$'
	| re
	;

re :
	re '|' series
	| series
	;

re2 :
	re '/'
	;

series :
	series singleton
	| singleton
	| series BEGIN_REPEAT_POSIX NUMBER ',' NUMBER END_REPEAT_POSIX
	| series BEGIN_REPEAT_POSIX NUMBER ',' END_REPEAT_POSIX
	| series BEGIN_REPEAT_POSIX NUMBER END_REPEAT_POSIX
	;

singleton :
	singleton '*'
	| singleton '+'
	| singleton '?'
	| singleton BEGIN_REPEAT_FLEX NUMBER ',' NUMBER END_REPEAT_FLEX
	| singleton BEGIN_REPEAT_FLEX NUMBER ',' END_REPEAT_FLEX
	| singleton BEGIN_REPEAT_FLEX NUMBER END_REPEAT_FLEX
	| '.'
	| fullccl
	| PREVCCL
	| '"' string '"'
	| '(' re ')'
	| CHAR
	;

fullccl :
	fullccl CCL_OP_DIFF /*1L*/ braceccl
	| fullccl CCL_OP_UNION /*1L*/ braceccl
	| braceccl
	;

braceccl :
	'[' ccl ']'
	| '[^' ccl ']'
	;

ccl :
	ccl CHAR '-' CHAR
	| ccl CHAR
	| ccl ccl_expr
	| /*empty*/
	;

ccl_expr :
	CCE_ALNUM
	| CCE_ALPHA
	| CCE_BLANK
	| CCE_CNTRL
	| CCE_DIGIT
	| CCE_GRAPH
	| CCE_LOWER
	| CCE_PRINT
	| CCE_PUNCT
	| CCE_SPACE
	| CCE_XDIGIT
	| CCE_UPPER
	| CCE_NEG_ALNUM
	| CCE_NEG_ALPHA
	| CCE_NEG_BLANK
	| CCE_NEG_CNTRL
	| CCE_NEG_DIGIT
	| CCE_NEG_GRAPH
	| CCE_NEG_PRINT
	| CCE_NEG_PUNCT
	| CCE_NEG_SPACE
	| CCE_NEG_XDIGIT
	| CCE_NEG_LOWER
	| CCE_NEG_UPPER
	;

string :
	string CHAR
	| /*empty*/
	;

//Lexer

CCL_OP_DIFF : '{-}' ;
CCL_OP_UNION : '{+}' ;
SECTEND : 'SECTEND' ;
SCDECL : 'SCDECL' ;
XSCDECL : 'XSCDECL' ;
NAME : "[[:alpha:]_][[:alnum:]_-]*" ;
TOK_OPTION : '%option' ;
TOK_OUTFILE : '%outfile' ;
TOK_EXTRA_TYPE : '%extra_type' ;
TOK_PREFIX : '%prefix' ;
TOK_YYCLASS : '%yyclass' ;
TOK_HEADER_FILE : '%header_file' ;
TOK_YYLMAX : '%yylmax' ;
TOK_NUMERIC : '%numeric' ;
TOK_YYDECL : '%yydecl' ;
TOK_PREACTION : '%pre_action' ;
TOK_POSTACTION : '%post_action' ;
TOK_BUFSIZE : '%bufsize' ;
TOK_EMIT : '%emit' ;
TOK_USERINIT : '%userinit' ;
TOK_YYTERMINATE : '%yyterminate' ;
TOK_TABLES_FILE : '%tables_files' ;
EOF_OP : 'EOF_OP' ;
BEGIN_REPEAT_POSIX : 'BEGIN_REPEAT_POSIX' ;
NUMBER : "[[:digit:]]+" ;
END_REPEAT_POSIX : 'END_REPEAT_POSIX' ;
BEGIN_REPEAT_FLEX : 'BEGIN_REPEAT_FLEX' ;
END_REPEAT_FLEX : 'END_REPEAT_FLEX' ;
PREVCCL : 'PREVCCL' ;
CHAR : "(\\.|[^\n\]])" ;
CCE_ALNUM : '[:alnum:]' ;
CCE_ALPHA : '[:alpha:]' ;
CCE_BLANK : '[:blank:]' ;
CCE_CNTRL : '[:cntrl:]' ;
CCE_DIGIT : '[:digit:]' ;
CCE_GRAPH : '[:graph:]' ;
CCE_LOWER : '[:lower:]' ;
CCE_PRINT : '[:print:]' ;
CCE_PUNCT : '[:punct:]' ;
CCE_SPACE : '[:space:]' ;
CCE_XDIGIT : '[:xdigit:]' ;
CCE_UPPER : '[:upper:]' ;
CCE_NEG_ALNUM : '[:^alnum:]' ;
CCE_NEG_ALPHA : '[:^alpha:]' ;
CCE_NEG_BLANK : '[:^blank:]' ;
CCE_NEG_CNTRL : '[:^cntrl:]' ;
CCE_NEG_DIGIT : '[:^digit:]' ;
CCE_NEG_GRAPH : '[:^graph:]' ;
CCE_NEG_PRINT : '[:^print:]' ;
CCE_NEG_PUNCT : '[:^punct:]' ;
CCE_NEG_SPACE : '[:^space:]' ;
CCE_NEG_XDIGIT : '[:^xdigit:]' ;
CCE_NEG_LOWER : '[:^lower:]' ;
CCE_NEG_UPPER : '[:^upper:]' ;

top_user_code : "%top{[^}]+}";
user_code : "%{[^%]+%}" ;

}
