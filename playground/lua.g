// /home/mingo/dev/c/A_grammars/lalr/lalr-nb/dist/Release/GNU-Linux/lalr-nb lua.g

lua {
%whitespace "([ \t\r\n]*)|(\-\-[^\n]*)";

/*Tokens*/
//%token YYEOF
//%token YYerror
//%token YYUNDEF
//%token AND
//%token BREAK
//%token DO
//%token ELSE
//%token ELSEIF
//%token END
//%token FALSE
//%token FOR
//%token FUNCTION
//%token IF
//%token IN
//%token LOCAL
//%token NIL
//%token NOT
//%token OR
//%token REPEAT
//%token RETURN
//%token THEN
//%token TRUE
//%token UNTIL
//%token WHILE
//%token CONCAT
//%token ELLIPSES
//%token EQ
//%token GE
//%token LE
//%token NE
//%token SPECIAL
//%token SPECIAL_CONST
//%token SPECIAL_NUMBER
//%token NUMBER
//%token STRING
//%token IDENTIFIER
//%token '<'
//%token '>'
//%token '+'
//%token '-'
//%token '*'
//%token '/'
//%token '%'
//%token UNARY_OPERATOR
//%token '^'
//%token ';'
//%token '='
//%token ','
//%token ':'
//%token '.'
//%token '('
//%token ')'
//%token '#'
//%token '['
//%token ']'
//%token '{'
//%token '}'

//declared to eliminate/silence the 2 shift reduce
%left '(' ;
%left IDENTIFIER ;
%left STRING ;

%left /*1*/ OR ;
%left /*2*/ AND ;
%left /*3*/ GE LE '<' '>' ;
%left /*4*/ EQ NE ;
%right /*5*/ CONCAT ;
%left /*6*/ '+' '-' ;
%left /*7*/ '*' '/' '%' ;
%left /*8*/ UNARY_OPERATOR ;
%right /*9*/ '^' ;

//%start file

//%%

file :
	opt_block
	;

opt_block :
	opt_block_statements
	;

opt_block_statements :
	/*empty*/
	| last_statement
	| statement_list
	| statement_list last_statement
	;

class_1_statement :
	nobr_function_call
	;

class_2_statement :
	br_function_call
	;

class_3_statement :
	nobr_statement opt_special
	| nobr_statement ';' opt_special
	| nobr_function_call ';'
	;

class_4_statement :
	br_statement
	| br_statement ';'
	| br_function_call ';'
	;

statement_list :
	statement_list_1
	| statement_list_2
	| statement_list_3
	| statement_list_4
	;

statement_list_1 :
	class_1_statement
	| statement_list_1 class_1_statement
	| statement_list_2 class_1_statement
	| statement_list_3 class_1_statement
	| statement_list_4 class_1_statement
	;

statement_list_2 :
	class_2_statement
	| statement_list_3 class_2_statement
	| statement_list_4 class_2_statement
	;

statement_list_3 :
	class_3_statement
	| statement_list_1 class_3_statement
	| statement_list_2 class_3_statement
	| statement_list_3 class_3_statement
	| statement_list_4 class_3_statement
	;

statement_list_4 :
	class_4_statement
	| statement_list_3 class_4_statement
	| statement_list_4 class_4_statement
	;

nobr_statement :
	nobr_variable_list '=' expression_list
	| DO opt_block END
	| WHILE expression DO opt_block END
	| REPEAT opt_block UNTIL expression
	| IF expression THEN opt_block opt_elseif_block_list opt_else_block END
	| FOR IDENTIFIER '=' expression ',' expression DO opt_block END
	| FOR IDENTIFIER '=' expression ',' expression ',' expression DO opt_block END
	| FOR identifier_list IN expression_list DO opt_block END
	| FUNCTION func_name_list function_body
	| FUNCTION func_name_list ':' IDENTIFIER function_body
	| LOCAL FUNCTION IDENTIFIER function_body
	| LOCAL identifier_list
	| LOCAL identifier_list '=' expression_list
	| IDENTIFIER STRING
	;

br_statement :
	br_variable_list '=' expression_list
	;

opt_elseif_block_list :
	/*empty*/
	| elseif_block_list
	;

elseif_block_list :
	elseif_block
	| elseif_block_list elseif_block
	;

elseif_block :
	ELSEIF expression THEN opt_block
	;

opt_else_block :
	/*empty*/
	| ELSE opt_block
	;

last_statement :
	RETURN opt_semicolon
	| RETURN expression_list opt_semicolon
	| BREAK opt_semicolon
	;

nobr_variable_list :
	nobr_variable
	| nobr_variable_list ',' nobr_variable
	| nobr_variable_list ',' br_variable
	;

br_variable_list :
	br_variable
	| br_variable_list ',' nobr_variable
	| br_variable_list ',' br_variable
	;

func_name_list :
	IDENTIFIER
	| func_name_list '.' IDENTIFIER
	;

expression :
	NIL
	| FALSE
	| TRUE
	| NUMBER
	| STRING
	| ELLIPSES
	| FUNCTION function_body
	| nobr_prefix_expression %prec '('
	| '(' expression ')'
	| table_constructor
	| expression CONCAT /*5R*/ expression
	| expression '+' /*6L*/ expression
	| expression '-' /*6L*/ expression
	| expression '*' /*7L*/ expression
	| expression '/' /*7L*/ expression
	| expression '^' /*9R*/ expression
	| expression '%' /*7L*/ expression
	| expression '<' /*3L*/ expression
	| expression LE /*3L*/ expression
	| expression '>' /*3L*/ expression
	| expression GE /*3L*/ expression
	| expression EQ /*4L*/ expression
	| expression NE /*4L*/ expression
	| expression AND /*2L*/ expression
	| expression OR /*1L*/ expression
	| NOT expression %precedence UNARY_OPERATOR /*8L*/
	| '-' /*6L*/ expression %precedence UNARY_OPERATOR /*8L*/
	| '#' expression %precedence UNARY_OPERATOR /*8L*/
	;

expression_list :
	expression
	| expression_list ',' expression
	;

nobr_prefix_expression :
	nobr_variable
	| nobr_function_call
	;

nobr_variable :
	IDENTIFIER
	| nobr_prefix_expression '[' expression ']'
	| nobr_prefix_expression '.' IDENTIFIER
	;

br_variable :
	'(' expression ')' '[' expression ']'
	| '(' expression ')' '.' IDENTIFIER
	;

nobr_function_call :
	nobr_prefix_expression arguments
	| nobr_prefix_expression ':' IDENTIFIER arguments
	;

br_function_call :
	'(' expression ')' arguments
	| '(' expression ')' ':' IDENTIFIER arguments
	;

arguments :
	'(' ')'
	| '(' expression_list ')'
	| table_constructor
	| STRING
	;

function_body :
	'(' opt_parameter_list ')' opt_block END
	;

opt_parameter_list :
	/*empty*/
	| ELLIPSES
	| identifier_list
	| identifier_list ',' ELLIPSES
	;

table_constructor :
	'{' '}'
	| '{' field_list opt_field_separator '}'
	;

field_list :
	field
	| field_list field_separator field
	;

field :
	'[' expression ']' '=' expression
	| IDENTIFIER '=' expression
	| expression
	;

opt_semicolon :
	/*empty*/
	| ';'
	;

field_separator :
	','
	| ';'
	;

opt_field_separator :
	/*empty*/
	| field_separator
	;

identifier_list :
	IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;

opt_special :
	/*empty*/
	| SPECIAL
	;

// Tokens
//\("[^"]+"\)\s+{ TOKEN(\([^)]+\)); }
AND : 'and' ;
BREAK : 'break' ;
DO : 'do' ;
ELSE : 'else' ;
ELSEIF : 'elseif' ;
END : 'end' ;
FALSE : 'false' ;
FOR : 'for' ;
FUNCTION : 'function' ;
IF : 'if' ;
IN : 'in' ;
LOCAL : 'local' ;
NIL : 'nil' ;
NOT : 'not' ;
OR : 'or' ;
REPEAT : 'repeat' ;
RETURN : 'return' ;
THEN : 'then' ;
TRUE : 'true' ;
UNTIL : 'until' ;
WHILE : 'while' ;
CONCAT : '..' ;
ELLIPSES : '...' ;
EQ : '==' ;
GE : '>=' ;
LE : '<=' ;
NE : '~=' ;

IDENTIFIER : "[a-zA-Z_][a-zA-Z_0-9]*" ;
NUMBER : "0[xX][a-fA-F0-9]+|[0-9]+([Ee][+-]?[0-9]+)?|[0-9]*\\.[0-9]+([Ee][+-]?[0-9]+)?" ;
STRING : "\"[^\"\n]*\"|'[^'\n]*'" ;
SPECIAL : '--@' ;
}