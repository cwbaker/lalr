//From: https://github.com/ValveSoftware/wine/blob/cd165953c8b379a78418711f07417022e503c81b/dlls/jscript/parser.y

jscript {

%whitespace "[ \t\r\n]*";
%whitespace "//[^\n\r]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

/*Tokens*/
//%token YYEOF
//%token YYerror
//%token YYUNDEF
//%token kBREAK
//%token kCASE
//%token kCATCH
//%token kCONST
//%token kCONTINUE
//%token kDEFAULT
//%token kDELETE
//%token kDO
//%token kELSE
//%token kFUNCTION
//%token kIF
//%token kFINALLY
//%token kFOR
//%token kGET
//%token kIN
//%token kLET
//%token kSET
//%token kINSTANCEOF
//%token kNEW
//%token kNULL
//%token kRETURN
//%token kSWITCH
//%token kTHIS
//%token kTHROW
//%token kTRUE
//%token kFALSE
//%token kTRY
//%token kTYPEOF
//%token kVAR
//%token kVOID
//%token kWHILE
//%token kWITH
//%token tANDAND
//%token tOROR
//%token tINC
//%token tDEC
//%token tHTMLCOMMENT
//%token kDIVEQ
//%token kDCOL
//%token tIdentifier
//%token tAssignOper
//%token tEqOper
//%token tShiftOper
//%token tRelOper
//%token tNumericLiteral
//%token tBooleanLiteral
//%token tStringLiteral
//%token LOWER_THAN_ELSE
//%token ':'
//%token '{'
//%token '}'
//%token ','
//%token '='
//%token ';'
//%token '?'
//%token '|'
//%token '^'
//%token '&'
//%token '+'
//%token '-'
//%token '*'
//%token '/'
//%token '%'
//%token '~'
//%token '!'
//%token '['
//%token ']'
//%token '.'
//%token '('
//%token ')'

%nonassoc /*1*/ LOWER_THAN_ELSE ;
%nonassoc /*2*/ kELSE kIN kINSTANCEOF ':' ;
%nonassoc /*3*/ kGET kLET kSET ;

//%start Script

//%%

Script :
	/*YYEOF
	|*/ ScriptBody HtmlComment
	;

ScriptBody :
	StatementList_opt
	;

HtmlComment :
	tHTMLCOMMENT
	| /*empty*/
	;

FunctionStatementList :
	StatementList_opt
	;

FunctionExpression :
	kFUNCTION left_bracket FormalParameterList_opt right_bracket '{' FunctionBody '}'
	| kFUNCTION Identifier left_bracket FormalParameterList_opt right_bracket '{' FunctionBody '}'
	| kFUNCTION Identifier kDCOL Identifier left_bracket FormalParameterList_opt right_bracket '{' FunctionBody '}'
	;

FunctionBody :
	FunctionStatementList
	;

FormalParameterList :
	Identifier
	| FormalParameterList ',' Identifier
	;

FormalParameterList_opt :
	/*empty*/
	| FormalParameterList
	;

Statement :
	Block
	| VariableStatement
	| EmptyStatement
	| FunctionExpression
	| ExpressionStatement
	| IfStatement
	| IterationStatement
	| ContinueStatement
	| BreakStatement
	| ReturnStatement
	| WithStatement
	| LabelledStatement
	| SwitchStatement
	| ThrowStatement
	| TryStatement
	;

Declaration :
	LexicalDeclaration
	;

StatementListItem :
	Statement
	| Declaration
	;

StatementList :
	StatementListItem
	| StatementList StatementListItem
	;

StatementList_opt :
	/*empty*/
	| StatementList
	;

Block :
	'{' StatementList '}'
	| '{' '}'
	;

LexicalDeclaration :
	kLET /*3N*/ VariableDeclarationList semicolon_opt
	| kCONST VariableDeclarationList semicolon_opt
	;

LexicalDeclarationNoIn :
	kLET /*3N*/ VariableDeclarationListNoIn semicolon_opt
	| kCONST VariableDeclarationListNoIn semicolon_opt
	;

VariableStatement :
	kVAR VariableDeclarationList semicolon_opt
	;

VariableDeclarationList :
	VariableDeclaration
	| VariableDeclarationList ',' VariableDeclaration
	;

VariableDeclarationListNoIn :
	VariableDeclarationNoIn
	| VariableDeclarationListNoIn ',' VariableDeclarationNoIn
	;

VariableDeclaration :
	Identifier Initialiser_opt
	;

VariableDeclarationNoIn :
	Identifier InitialiserNoIn_opt
	;

Initialiser_opt :
	/*empty*/
	| Initialiser
	;

Initialiser :
	'=' AssignmentExpression
	;

InitialiserNoIn_opt :
	/*empty*/
	| InitialiserNoIn
	;

InitialiserNoIn :
	'=' AssignmentExpressionNoIn
	;

EmptyStatement :
	';'
	;

ExpressionStatement :
	Expression semicolon_opt
	;

IfStatement :
	kIF left_bracket Expression_err right_bracket Statement kELSE /*2N*/ Statement
	| kIF left_bracket Expression_err right_bracket Statement %prec LOWER_THAN_ELSE /*1N*/
	;

IterationStatement :
	kDO Statement kWHILE left_bracket Expression_err right_bracket semicolon_opt
	| kWHILE left_bracket Expression_err right_bracket Statement
	| kFOR left_bracket ExpressionNoIn_opt semicolon Expression_opt semicolon Expression_opt right_bracket Statement
	| kFOR left_bracket kVAR VariableDeclarationListNoIn semicolon Expression_opt semicolon Expression_opt right_bracket Statement
	| kFOR left_bracket LeftHandSideExpression kIN /*2N*/ Expression_err right_bracket Statement
	| kFOR left_bracket kVAR VariableDeclarationNoIn kIN /*2N*/ Expression_err right_bracket Statement
	| kFOR left_bracket LexicalDeclarationNoIn Expression_opt semicolon Expression_opt right_bracket Statement
	;

ContinueStatement :
	kCONTINUE Identifier_opt semicolon_opt
	;

BreakStatement :
	kBREAK Identifier_opt semicolon_opt
	;

ReturnStatement :
	kRETURN Expression_opt semicolon_opt
	;

WithStatement :
	kWITH left_bracket Expression right_bracket Statement
	;

LabelledStatement :
	tIdentifier ':' /*2N*/ Statement
	| kGET /*3N*/ ':' /*2N*/ Statement
	| kSET /*3N*/ ':' /*2N*/ Statement
	| kLET /*3N*/ ':' /*2N*/ Statement
	;

SwitchStatement :
	kSWITCH left_bracket Expression right_bracket CaseBlock
	;

CaseBlock :
	'{' CaseClausules_opt '}'
	| '{' CaseClausules_opt DefaultClausule CaseClausules_opt '}'
	;

CaseClausules_opt :
	/*empty*/
	| CaseClausules
	;

CaseClausules :
	CaseClausule
	| CaseClausules CaseClausule
	;

CaseClausule :
	kCASE Expression ':' /*2N*/ StatementList_opt
	;

DefaultClausule :
	kDEFAULT ':' /*2N*/ StatementList_opt
	;

ThrowStatement :
	kTHROW Expression semicolon_opt
	;

TryStatement :
	kTRY Block Catch
	| kTRY Block Finally
	| kTRY Block Catch Finally
	;

Catch :
	kCATCH left_bracket Identifier right_bracket Block
	;

Finally :
	kFINALLY Block
	;

Expression_opt :
	/*empty*/
	| Expression
	;

Expression_err :
	Expression
	| error
	;

Expression :
	AssignmentExpression
	| Expression ',' AssignmentExpression
	;

ExpressionNoIn_opt :
	/*empty*/
	| ExpressionNoIn
	;

ExpressionNoIn :
	AssignmentExpressionNoIn
	| ExpressionNoIn ',' AssignmentExpressionNoIn
	;

AssignOper :
	tAssignOper
	| kDIVEQ
	;

AssignmentExpression :
	ConditionalExpression
	| LeftHandSideExpression '=' AssignmentExpression
	| LeftHandSideExpression AssignOper AssignmentExpression
	;

AssignmentExpressionNoIn :
	ConditionalExpressionNoIn
	| LeftHandSideExpression '=' AssignmentExpressionNoIn
	| LeftHandSideExpression AssignOper AssignmentExpressionNoIn
	;

ConditionalExpression :
	LogicalORExpression
	| LogicalORExpression '?' AssignmentExpression ':' /*2N*/ AssignmentExpression
	;

ConditionalExpressionNoIn :
	LogicalORExpressionNoIn
	| LogicalORExpressionNoIn '?' AssignmentExpressionNoIn ':' /*2N*/ AssignmentExpressionNoIn
	;

LogicalORExpression :
	LogicalANDExpression
	| LogicalORExpression tOROR LogicalANDExpression
	;

LogicalORExpressionNoIn :
	LogicalANDExpressionNoIn
	| LogicalORExpressionNoIn tOROR LogicalANDExpressionNoIn
	;

LogicalANDExpression :
	BitwiseORExpression
	| LogicalANDExpression tANDAND BitwiseORExpression
	;

LogicalANDExpressionNoIn :
	BitwiseORExpressionNoIn
	| LogicalANDExpressionNoIn tANDAND BitwiseORExpressionNoIn
	;

BitwiseORExpression :
	BitwiseXORExpression
	| BitwiseORExpression '|' BitwiseXORExpression
	;

BitwiseORExpressionNoIn :
	BitwiseXORExpressionNoIn
	| BitwiseORExpressionNoIn '|' BitwiseXORExpressionNoIn
	;

BitwiseXORExpression :
	BitwiseANDExpression
	| BitwiseXORExpression '^' BitwiseANDExpression
	;

BitwiseXORExpressionNoIn :
	BitwiseANDExpressionNoIn
	| BitwiseXORExpressionNoIn '^' BitwiseANDExpressionNoIn
	;

BitwiseANDExpression :
	EqualityExpression
	| BitwiseANDExpression '&' EqualityExpression
	;

BitwiseANDExpressionNoIn :
	EqualityExpressionNoIn
	| BitwiseANDExpressionNoIn '&' EqualityExpressionNoIn
	;

EqualityExpression :
	RelationalExpression
	| EqualityExpression tEqOper RelationalExpression
	;

EqualityExpressionNoIn :
	RelationalExpressionNoIn
	| EqualityExpressionNoIn tEqOper RelationalExpressionNoIn
	;

RelationalExpression :
	ShiftExpression
	| RelationalExpression tRelOper ShiftExpression
	| RelationalExpression kINSTANCEOF /*2N*/ ShiftExpression
	| RelationalExpression kIN /*2N*/ ShiftExpression
	;

RelationalExpressionNoIn :
	ShiftExpression
	| RelationalExpressionNoIn tRelOper ShiftExpression
	| RelationalExpressionNoIn kINSTANCEOF /*2N*/ ShiftExpression
	;

ShiftExpression :
	AdditiveExpression
	| ShiftExpression tShiftOper AdditiveExpression
	;

AdditiveExpression :
	MultiplicativeExpression
	| AdditiveExpression '+' MultiplicativeExpression
	| AdditiveExpression '-' MultiplicativeExpression
	;

MultiplicativeExpression :
	UnaryExpression
	| MultiplicativeExpression '*' UnaryExpression
	| MultiplicativeExpression '/' UnaryExpression
	| MultiplicativeExpression '%' UnaryExpression
	;

UnaryExpression :
	PostfixExpression
	| kDELETE UnaryExpression
	| kVOID UnaryExpression
	| kTYPEOF UnaryExpression
	| tINC UnaryExpression
	| tDEC UnaryExpression
	| '+' UnaryExpression
	| '-' UnaryExpression
	| '~' UnaryExpression
	| '!' UnaryExpression
	;

PostfixExpression :
	LeftHandSideExpression
	| LeftHandSideExpression tINC
	| LeftHandSideExpression tDEC
	;

LeftHandSideExpression :
	NewExpression
	| CallExpression
	;

NewExpression :
	MemberExpression
	| kNEW NewExpression
	;

MemberExpression :
	PrimaryExpression
	| FunctionExpression
	| MemberExpression '[' Expression ']'
	| MemberExpression '.' IdentifierName
	| kNEW MemberExpression Arguments
	;

CallExpression :
	MemberExpression Arguments
	| CallExpression Arguments
	| CallExpression '[' Expression ']'
	| CallExpression '.' IdentifierName
	;

Arguments :
	'(' ')'
	| '(' ArgumentList ')'
	;

ArgumentList :
	AssignmentExpression
	| ArgumentList ',' AssignmentExpression
	;

PrimaryExpression :
	kTHIS
	| Identifier
	| Literal
	| ArrayLiteral
	| ObjectLiteral
	| '(' Expression ')'
	;

ArrayLiteral :
	'[' ']'
	| '[' Elision ']'
	| '[' ElementList ']'
	| '[' ElementList ',' Elision_opt ']'
	;

ElementList :
	Elision_opt AssignmentExpression
	| ElementList ',' Elision_opt AssignmentExpression
	;

Elision :
	','
	| Elision ','
	;

Elision_opt :
	/*empty*/
	| Elision
	;

ObjectLiteral :
	'{' '}'
	| '{' PropertyNameAndValueList '}'
	| '{' PropertyNameAndValueList ',' '}'
	;

PropertyNameAndValueList :
	PropertyDefinition
	| PropertyNameAndValueList ',' PropertyDefinition
	;

PropertyDefinition :
	PropertyName ':' /*2N*/ AssignmentExpression
	| kGET /*3N*/ PropertyName GetterSetterMethod
	| kSET /*3N*/ PropertyName GetterSetterMethod
	;

GetterSetterMethod :
	left_bracket FormalParameterList_opt right_bracket '{' FunctionBody '}'
	;

PropertyName :
	IdentifierName
	| tStringLiteral
	| tNumericLiteral
	;

Identifier_opt :
	/*empty*/
	| Identifier
	;

Identifier :
	tIdentifier
	| ES5Keyword
	;

IdentifierName :
	tIdentifier
	| ReservedAsIdentifier
	;

ReservedAsIdentifier :
	kBREAK
	| kCASE
	| kCATCH
	| kCONST
	| kCONTINUE
	| kDEFAULT
	| kDELETE
	| kDO
	| kELSE /*2N*/
	| kFALSE
	| kFINALLY
	| kFOR
	| kFUNCTION
	| kIF
	| kIN /*2N*/
	| kINSTANCEOF /*2N*/
	| kNEW
	| kNULL
	| kRETURN
	| kSWITCH
	| kTHIS
	| kTHROW
	| kTRUE
	| kTRY
	| kTYPEOF
	| kVAR
	| kVOID
	| kWHILE
	| kWITH
	| ES5Keyword
	;

ES5Keyword :
	kGET /*3N*/
	| kLET /*3N*/
	| kSET /*3N*/
	;

Literal :
	kNULL
	| BooleanLiteral
	| tNumericLiteral
	| tStringLiteral
	| '/'
	| kDIVEQ
	;

BooleanLiteral :
	kTRUE
	| kFALSE
	| tBooleanLiteral
	;

semicolon_opt :
	';'
	| error
	;

left_bracket :
	'('
	| error
	;

right_bracket :
	')'
	| error
	;

semicolon :
	';'
	| error
	;

//Lexer

kELSE : 'kELSE' ;
kIN : 'kIN' ;
kINSTANCEOF : 'kINSTANCEOF' ;
kGET : 'kGET' ;
kLET : 'kLET' ;
kSET : 'kSET' ;
tHTMLCOMMENT : 'tHTMLCOMMENT' ;
kFUNCTION : 'kFUNCTION' ;
kDCOL : 'kDCOL' ;
kCONST : 'kCONST' ;
kVAR : 'kVAR' ;
kIF : 'kIF' ;
kDO : 'kDO' ;
kWHILE : 'kWHILE' ;
kFOR : 'kFOR' ;
kCONTINUE : 'kCONTINUE' ;
kBREAK : 'kBREAK' ;
kRETURN : 'kRETURN' ;
kWITH : 'kWITH' ;
tIdentifier : 'tIdentifier' ;
kSWITCH : 'kSWITCH' ;
kCASE : 'kCASE' ;
kDEFAULT : 'kDEFAULT' ;
kTHROW : 'kTHROW' ;
kTRY : 'kTRY' ;
kCATCH : 'kCATCH' ;
kFINALLY : 'kFINALLY' ;
tAssignOper : 'tAssignOper' ;
kDIVEQ : 'kDIVEQ' ;
tOROR : 'tOROR' ;
tANDAND : 'tANDAND' ;
tEqOper : 'tEqOper' ;
tRelOper : 'tRelOper' ;
tShiftOper : 'tShiftOper' ;
kDELETE : 'kDELETE' ;
kVOID : 'kVOID' ;
kTYPEOF : 'kTYPEOF' ;
tINC : 'tINC' ;
tDEC : 'tDEC' ;
kNEW : 'kNEW' ;
kTHIS : 'kTHIS' ;
tStringLiteral : 'tStringLiteral' ;
tNumericLiteral : 'tNumericLiteral' ;
kFALSE : 'kFALSE' ;
kNULL : 'kNULL' ;
kTRUE : 'kTRUE' ;
tBooleanLiteral : 'tBooleanLiteral' ;

}
