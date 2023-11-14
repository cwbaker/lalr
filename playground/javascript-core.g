JavaScriptCore {

%whitespace "[ \t\r\n]*";
%whitespace "//[^\n\r]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

//%token NULLTOKEN
//%token TRUETOKEN
//%token FALSETOKEN
//%token BREAK
//%token CASE
//%token DEFAULT
//%token FOR
//%token NEW
//%token VAR
//%token CONSTTOKEN
//%token CONTINUE
//%token FUNCTION
//%token RETURN
//%token VOIDTOKEN
//%token DELETETOKEN
//%token IF
//%token THISTOKEN
//%token DO
//%token WHILE
//%token INTOKEN
//%token INSTANCEOF
//%token TYPEOF
//%token SWITCH
//%token WITH
//%token RESERVED
//%token THROW
//%token TRY
//%token CATCH
//%token FINALLY
//%token DEBUGGER
//%token IF_WITHOUT_ELSE
//%token ELSE
//%token EQEQ
//%token NE
//%token STREQ
//%token STRNEQ
//%token LE
//%token GE
//%token OR
//%token AND
//%token PLUSPLUS
//%token MINUSMINUS
//%token LSHIFT
//%token RSHIFT
//%token URSHIFT
//%token PLUSEQUAL
//%token MINUSEQUAL
//%token MULTEQUAL
//%token DIVEQUAL
//%token LSHIFTEQUAL
//%token RSHIFTEQUAL
//%token URSHIFTEQUAL
//%token ANDEQUAL
//%token MODEQUAL
//%token XOREQUAL
//%token OREQUAL
//%token OPENBRACE
//%token CLOSEBRACE
//%token NUMBER
//%token IDENT
//%token STRING
//%token AUTOPLUSPLUS
//%token AUTOMINUSMINUS
//%token '/'
//%token ':'
//%token '('
//%token ')'
//%token ','
//%token '['
//%token ']'
//%token '.'
//%token '+'
//%token '-'
//%token '~'
//%token '!'
//%token '*'
//%token '%'
//%token '<'
//%token '>'
//%token '&'
//%token '^'
//%token '|'
//%token '?'
//%token '='
//%token ';'

%nonassoc /*1*/ IF_WITHOUT_ELSE ;
%nonassoc /*2*/ ELSE ;

//%start Program

//%%

Program :
	 /*empty*/
	| SourceElements
	;

Literal :
	 NULLTOKEN
	| TRUETOKEN
	| FALSETOKEN
	| NUMBER
	| STRING
	| '/'
	| DIVEQUAL
	;
Property :
	 IDENT ':' AssignmentExpr
	| STRING ':' AssignmentExpr
	| NUMBER ':' AssignmentExpr
	| IDENT IDENT '(' ')' OPENBRACE FunctionBody CLOSEBRACE
	| IDENT IDENT '(' FormalParameterList ')' OPENBRACE FunctionBody CLOSEBRACE
	;
PropertyList :
	 Property
	| PropertyList ',' Property
	;
PrimaryExpr :
	 PrimaryExprNoBrace
	| OPENBRACE CLOSEBRACE
	| OPENBRACE PropertyList CLOSEBRACE
	| OPENBRACE PropertyList ',' CLOSEBRACE
	;
PrimaryExprNoBrace :
	 THISTOKEN
	| Literal
	| ArrayLiteral
	| IDENT
	| '(' Expr ')'
	;
ArrayLiteral :
	 '[' ElisionOpt ']'
	| '[' ElementList ']'
	| '[' ElementList ',' ElisionOpt ']'
	;
ElementList :
	 ElisionOpt AssignmentExpr
	| ElementList ',' ElisionOpt AssignmentExpr
	;
ElisionOpt :
	 /*empty*/
	| Elision
	;
Elision :
	 ','
	| Elision ','
	;
MemberExpr :
	 PrimaryExpr
	| FunctionExpr
	| MemberExpr '[' Expr ']'
	| MemberExpr '.' IDENT
	| NEW MemberExpr Arguments
	;
MemberExprNoBF :
	 PrimaryExprNoBrace
	| MemberExprNoBF '[' Expr ']'
	| MemberExprNoBF '.' IDENT
	| NEW MemberExpr Arguments
	;
NewExpr :
	 MemberExpr
	| NEW NewExpr
	;
NewExprNoBF :
	 MemberExprNoBF
	| NEW NewExpr
	;
CallExpr :
	 MemberExpr Arguments
	| CallExpr Arguments
	| CallExpr '[' Expr ']'
	| CallExpr '.' IDENT
	;
CallExprNoBF :
	 MemberExprNoBF Arguments
	| CallExprNoBF Arguments
	| CallExprNoBF '[' Expr ']'
	| CallExprNoBF '.' IDENT
	;
Arguments :
	 '(' ')'
	| '(' ArgumentList ')'
	;
ArgumentList :
	 AssignmentExpr
	| ArgumentList ',' AssignmentExpr
	;
LeftHandSideExpr :
	 NewExpr
	| CallExpr
	;
LeftHandSideExprNoBF :
	 NewExprNoBF
	| CallExprNoBF
	;
PostfixExpr :
	 LeftHandSideExpr
	| LeftHandSideExpr PLUSPLUS
	| LeftHandSideExpr MINUSMINUS
	;
PostfixExprNoBF :
	 LeftHandSideExprNoBF
	| LeftHandSideExprNoBF PLUSPLUS
	| LeftHandSideExprNoBF MINUSMINUS
	;
UnaryExprCommon :
	 DELETETOKEN UnaryExpr
	| VOIDTOKEN UnaryExpr
	| TYPEOF UnaryExpr
	| PLUSPLUS UnaryExpr
	| AUTOPLUSPLUS UnaryExpr
	| MINUSMINUS UnaryExpr
	| AUTOMINUSMINUS UnaryExpr
	| '+' UnaryExpr
	| '-' UnaryExpr
	| '~' UnaryExpr
	| '!' UnaryExpr
	;
UnaryExpr :
	 PostfixExpr
	| UnaryExprCommon
	;
UnaryExprNoBF :
	 PostfixExprNoBF
	| UnaryExprCommon
	;
MultiplicativeExpr :
	 UnaryExpr
	| MultiplicativeExpr '*' UnaryExpr
	| MultiplicativeExpr '/' UnaryExpr
	| MultiplicativeExpr '%' UnaryExpr
	;
MultiplicativeExprNoBF :
	 UnaryExprNoBF
	| MultiplicativeExprNoBF '*' UnaryExpr
	| MultiplicativeExprNoBF '/' UnaryExpr
	| MultiplicativeExprNoBF '%' UnaryExpr
	;
AdditiveExpr :
	 MultiplicativeExpr
	| AdditiveExpr '+' MultiplicativeExpr
	| AdditiveExpr '-' MultiplicativeExpr
	;
AdditiveExprNoBF :
	 MultiplicativeExprNoBF
	| AdditiveExprNoBF '+' MultiplicativeExpr
	| AdditiveExprNoBF '-' MultiplicativeExpr
	;
ShiftExpr :
	 AdditiveExpr
	| ShiftExpr LSHIFT AdditiveExpr
	| ShiftExpr RSHIFT AdditiveExpr
	| ShiftExpr URSHIFT AdditiveExpr
	;
ShiftExprNoBF :
	 AdditiveExprNoBF
	| ShiftExprNoBF LSHIFT AdditiveExpr
	| ShiftExprNoBF RSHIFT AdditiveExpr
	| ShiftExprNoBF URSHIFT AdditiveExpr
	;
RelationalExpr :
	 ShiftExpr
	| RelationalExpr '<' ShiftExpr
	| RelationalExpr '>' ShiftExpr
	| RelationalExpr LE ShiftExpr
	| RelationalExpr GE ShiftExpr
	| RelationalExpr INSTANCEOF ShiftExpr
	| RelationalExpr INTOKEN ShiftExpr
	;
RelationalExprNoIn :
	 ShiftExpr
	| RelationalExprNoIn '<' ShiftExpr
	| RelationalExprNoIn '>' ShiftExpr
	| RelationalExprNoIn LE ShiftExpr
	| RelationalExprNoIn GE ShiftExpr
	| RelationalExprNoIn INSTANCEOF ShiftExpr
	;
RelationalExprNoBF :
	 ShiftExprNoBF
	| RelationalExprNoBF '<' ShiftExpr
	| RelationalExprNoBF '>' ShiftExpr
	| RelationalExprNoBF LE ShiftExpr
	| RelationalExprNoBF GE ShiftExpr
	| RelationalExprNoBF INSTANCEOF ShiftExpr
	| RelationalExprNoBF INTOKEN ShiftExpr
	;
EqualityExpr :
	 RelationalExpr
	| EqualityExpr EQEQ RelationalExpr
	| EqualityExpr NE RelationalExpr
	| EqualityExpr STREQ RelationalExpr
	| EqualityExpr STRNEQ RelationalExpr
	;
EqualityExprNoIn :
	 RelationalExprNoIn
	| EqualityExprNoIn EQEQ RelationalExprNoIn
	| EqualityExprNoIn NE RelationalExprNoIn
	| EqualityExprNoIn STREQ RelationalExprNoIn
	| EqualityExprNoIn STRNEQ RelationalExprNoIn
	;
EqualityExprNoBF :
	 RelationalExprNoBF
	| EqualityExprNoBF EQEQ RelationalExpr
	| EqualityExprNoBF NE RelationalExpr
	| EqualityExprNoBF STREQ RelationalExpr
	| EqualityExprNoBF STRNEQ RelationalExpr
	;
BitwiseANDExpr :
	 EqualityExpr
	| BitwiseANDExpr '&' EqualityExpr
	;
BitwiseANDExprNoIn :
	 EqualityExprNoIn
	| BitwiseANDExprNoIn '&' EqualityExprNoIn
	;
BitwiseANDExprNoBF :
	 EqualityExprNoBF
	| BitwiseANDExprNoBF '&' EqualityExpr
	;
BitwiseXORExpr :
	 BitwiseANDExpr
	| BitwiseXORExpr '^' BitwiseANDExpr
	;
BitwiseXORExprNoIn :
	 BitwiseANDExprNoIn
	| BitwiseXORExprNoIn '^' BitwiseANDExprNoIn
	;
BitwiseXORExprNoBF :
	 BitwiseANDExprNoBF
	| BitwiseXORExprNoBF '^' BitwiseANDExpr
	;
BitwiseORExpr :
	 BitwiseXORExpr
	| BitwiseORExpr '|' BitwiseXORExpr
	;
BitwiseORExprNoIn :
	 BitwiseXORExprNoIn
	| BitwiseORExprNoIn '|' BitwiseXORExprNoIn
	;
BitwiseORExprNoBF :
	 BitwiseXORExprNoBF
	| BitwiseORExprNoBF '|' BitwiseXORExpr
	;
LogicalANDExpr :
	 BitwiseORExpr
	| LogicalANDExpr AND BitwiseORExpr
	;
LogicalANDExprNoIn :
	 BitwiseORExprNoIn
	| LogicalANDExprNoIn AND BitwiseORExprNoIn
	;
LogicalANDExprNoBF :
	 BitwiseORExprNoBF
	| LogicalANDExprNoBF AND BitwiseORExpr
	;
LogicalORExpr :
	 LogicalANDExpr
	| LogicalORExpr OR LogicalANDExpr
	;
LogicalORExprNoIn :
	 LogicalANDExprNoIn
	| LogicalORExprNoIn OR LogicalANDExprNoIn
	;
LogicalORExprNoBF :
	 LogicalANDExprNoBF
	| LogicalORExprNoBF OR LogicalANDExpr
	;
ConditionalExpr :
	 LogicalORExpr
	| LogicalORExpr '?' AssignmentExpr ':' AssignmentExpr
	;
ConditionalExprNoIn :
	 LogicalORExprNoIn
	| LogicalORExprNoIn '?' AssignmentExprNoIn ':' AssignmentExprNoIn
	;
ConditionalExprNoBF :
	 LogicalORExprNoBF
	| LogicalORExprNoBF '?' AssignmentExpr ':' AssignmentExpr
	;
AssignmentExpr :
	 ConditionalExpr
	| LeftHandSideExpr AssignmentOperator AssignmentExpr
	;
AssignmentExprNoIn :
	 ConditionalExprNoIn
	| LeftHandSideExpr AssignmentOperator AssignmentExprNoIn
	;
AssignmentExprNoBF :
	 ConditionalExprNoBF
	| LeftHandSideExprNoBF AssignmentOperator AssignmentExpr
	;
AssignmentOperator :
	 '='
	| PLUSEQUAL
	| MINUSEQUAL
	| MULTEQUAL
	| DIVEQUAL
	| LSHIFTEQUAL
	| RSHIFTEQUAL
	| URSHIFTEQUAL
	| ANDEQUAL
	| XOREQUAL
	| OREQUAL
	| MODEQUAL
	;
Expr :
	 AssignmentExpr
	| Expr ',' AssignmentExpr
	;
ExprNoIn :
	 AssignmentExprNoIn
	| ExprNoIn ',' AssignmentExprNoIn
	;
ExprNoBF :
	 AssignmentExprNoBF
	| ExprNoBF ',' AssignmentExpr
	;
Statement :
	 Block
	| VariableStatement
	| ConstStatement
	| FunctionDeclaration
	| EmptyStatement
	| ExprStatement
	| IfStatement
	| IterationStatement
	| ContinueStatement
	| BreakStatement
	| ReturnStatement
	| WithStatement
	| SwitchStatement
	| LabelledStatement
	| ThrowStatement
	| TryStatement
	| DebuggerStatement
	;
Block :
	 OPENBRACE CLOSEBRACE
	| OPENBRACE SourceElements CLOSEBRACE
	;
VariableStatement :
	 VAR VariableDeclarationList ';'
	//| VAR VariableDeclarationList error
	;
VariableDeclarationList :
	 IDENT
	| IDENT Initializer
	| VariableDeclarationList ',' IDENT
	| VariableDeclarationList ',' IDENT Initializer
	;
VariableDeclarationListNoIn :
	 IDENT
	| IDENT InitializerNoIn
	| VariableDeclarationListNoIn ',' IDENT
	| VariableDeclarationListNoIn ',' IDENT InitializerNoIn
	;
ConstStatement :
	 CONSTTOKEN ConstDeclarationList ';'
	//| CONSTTOKEN ConstDeclarationList error
	;
ConstDeclarationList :
	 ConstDeclaration
	| ConstDeclarationList ',' ConstDeclaration
	;
ConstDeclaration :
	 IDENT
	| IDENT Initializer
	;
Initializer :
	 '=' AssignmentExpr
	;
InitializerNoIn :
	 '=' AssignmentExprNoIn
	;
EmptyStatement :
	 ';'
	;
ExprStatement :
	 ExprNoBF ';'
	| ExprNoBF error
	;
IfStatement :
	 IF '(' Expr ')' Statement %prec IF_WITHOUT_ELSE /*1N*/
	| IF '(' Expr ')' Statement ELSE /*2N*/ Statement
	;
IterationStatement :
	 DO Statement WHILE '(' Expr ')' ';'
	| DO Statement WHILE '(' Expr ')' error
	| WHILE '(' Expr ')' Statement
	| FOR '(' ExprNoInOpt ';' ExprOpt ';' ExprOpt ')' Statement
	| FOR '(' VAR VariableDeclarationListNoIn ';' ExprOpt ';' ExprOpt ')' Statement
	| FOR '(' LeftHandSideExpr INTOKEN Expr ')' Statement
	| FOR '(' VAR IDENT INTOKEN Expr ')' Statement
	| FOR '(' VAR IDENT InitializerNoIn INTOKEN Expr ')' Statement
	;
ExprOpt :
	 /*empty*/
	| Expr
	;
ExprNoInOpt :
	 /*empty*/
	| ExprNoIn
	;
ContinueStatement :
	 CONTINUE ';'
	//| CONTINUE error
	| CONTINUE IDENT ';'
	//| CONTINUE IDENT error
	;
BreakStatement :
	 BREAK ';'
	//| BREAK error
	| BREAK IDENT ';'
	//| BREAK IDENT error
	;
ReturnStatement :
	 RETURN ';'
	//| RETURN error
	| RETURN Expr ';'
	//| RETURN Expr error
	;
WithStatement :
	 WITH '(' Expr ')' Statement
	;
SwitchStatement :
	 SWITCH '(' Expr ')' CaseBlock
	;
CaseBlock :
	 OPENBRACE CaseClausesOpt CLOSEBRACE
	| OPENBRACE CaseClausesOpt DefaultClause CaseClausesOpt CLOSEBRACE
	;
CaseClausesOpt :
	 /*empty*/
	| CaseClauses
	;
CaseClauses :
	 CaseClause
	| CaseClauses CaseClause
	;
CaseClause :
	 CASE Expr ':'
	| CASE Expr ':' SourceElements
	;
DefaultClause :
	 DEFAULT ':'
	| DEFAULT ':' SourceElements
	;
LabelledStatement :
	 IDENT ':' Statement
	;
ThrowStatement :
	 THROW Expr ';'
	//| THROW Expr error
	;
TryStatement :
	 TRY Block FINALLY Block
	| TRY Block CATCH '(' IDENT ')' Block
	| TRY Block CATCH '(' IDENT ')' Block FINALLY Block
	;
DebuggerStatement :
	 DEBUGGER ';'
	//| DEBUGGER error
	;
FunctionDeclaration :
	 FUNCTION IDENT '(' ')' OPENBRACE FunctionBody CLOSEBRACE
	| FUNCTION IDENT '(' FormalParameterList ')' OPENBRACE FunctionBody CLOSEBRACE
	;
FunctionExpr :
	 FUNCTION '(' ')' OPENBRACE FunctionBody CLOSEBRACE
	| FUNCTION '(' FormalParameterList ')' OPENBRACE FunctionBody CLOSEBRACE
	| FUNCTION IDENT '(' ')' OPENBRACE FunctionBody CLOSEBRACE
	| FUNCTION IDENT '(' FormalParameterList ')' OPENBRACE FunctionBody CLOSEBRACE
	;
FormalParameterList :
	 IDENT
	| FormalParameterList ',' IDENT
	;
FunctionBody :
	 /*empty*/
	| SourceElements_NoNode
	;
SourceElements :
	 Statement
	| SourceElements Statement
	;
Literal_NoNode :
	 NULLTOKEN
	| TRUETOKEN
	| FALSETOKEN
	| NUMBER
	| STRING
	| '/'
	| DIVEQUAL
	;
Property_NoNode :
	 IDENT ':' AssignmentExpr_NoNode
	| STRING ':' AssignmentExpr_NoNode
	| NUMBER ':' AssignmentExpr_NoNode
	| IDENT IDENT '(' ')' OPENBRACE FunctionBody_NoNode CLOSEBRACE
	| IDENT IDENT '(' FormalParameterList_NoNode ')' OPENBRACE FunctionBody_NoNode CLOSEBRACE
	;
PropertyList_NoNode :
	 Property_NoNode
	| PropertyList_NoNode ',' Property_NoNode
	;
PrimaryExpr_NoNode :
	 PrimaryExprNoBrace_NoNode
	| OPENBRACE CLOSEBRACE
	| OPENBRACE PropertyList_NoNode CLOSEBRACE
	| OPENBRACE PropertyList_NoNode ',' CLOSEBRACE
	;
PrimaryExprNoBrace_NoNode :
	 THISTOKEN
	| Literal_NoNode
	| ArrayLiteral_NoNode
	| IDENT
	| '(' Expr_NoNode ')'
	;
ArrayLiteral_NoNode :
	 '[' ElisionOpt_NoNode ']'
	| '[' ElementList_NoNode ']'
	| '[' ElementList_NoNode ',' ElisionOpt_NoNode ']'
	;
ElementList_NoNode :
	 ElisionOpt_NoNode AssignmentExpr_NoNode
	| ElementList_NoNode ',' ElisionOpt_NoNode AssignmentExpr_NoNode
	;
ElisionOpt_NoNode :
	 /*empty*/
	| Elision_NoNode
	;
Elision_NoNode :
	 ','
	| Elision_NoNode ','
	;
MemberExpr_NoNode :
	 PrimaryExpr_NoNode
	| FunctionExpr_NoNode
	| MemberExpr_NoNode '[' Expr_NoNode ']'
	| MemberExpr_NoNode '.' IDENT
	| NEW MemberExpr_NoNode Arguments_NoNode
	;
MemberExprNoBF_NoNode :
	 PrimaryExprNoBrace_NoNode
	| MemberExprNoBF_NoNode '[' Expr_NoNode ']'
	| MemberExprNoBF_NoNode '.' IDENT
	| NEW MemberExpr_NoNode Arguments_NoNode
	;
NewExpr_NoNode :
	 MemberExpr_NoNode
	| NEW NewExpr_NoNode
	;
NewExprNoBF_NoNode :
	 MemberExprNoBF_NoNode
	| NEW NewExpr_NoNode
	;
CallExpr_NoNode :
	 MemberExpr_NoNode Arguments_NoNode
	| CallExpr_NoNode Arguments_NoNode
	| CallExpr_NoNode '[' Expr_NoNode ']'
	| CallExpr_NoNode '.' IDENT
	;
CallExprNoBF_NoNode :
	 MemberExprNoBF_NoNode Arguments_NoNode
	| CallExprNoBF_NoNode Arguments_NoNode
	| CallExprNoBF_NoNode '[' Expr_NoNode ']'
	| CallExprNoBF_NoNode '.' IDENT
	;
Arguments_NoNode :
	 '(' ')'
	| '(' ArgumentList_NoNode ')'
	;
ArgumentList_NoNode :
	 AssignmentExpr_NoNode
	| ArgumentList_NoNode ',' AssignmentExpr_NoNode
	;
LeftHandSideExpr_NoNode :
	 NewExpr_NoNode
	| CallExpr_NoNode
	;
LeftHandSideExprNoBF_NoNode :
	 NewExprNoBF_NoNode
	| CallExprNoBF_NoNode
	;
PostfixExpr_NoNode :
	 LeftHandSideExpr_NoNode
	| LeftHandSideExpr_NoNode PLUSPLUS
	| LeftHandSideExpr_NoNode MINUSMINUS
	;
PostfixExprNoBF_NoNode :
	 LeftHandSideExprNoBF_NoNode
	| LeftHandSideExprNoBF_NoNode PLUSPLUS
	| LeftHandSideExprNoBF_NoNode MINUSMINUS
	;
UnaryExprCommon_NoNode :
	 DELETETOKEN UnaryExpr_NoNode
	| VOIDTOKEN UnaryExpr_NoNode
	| TYPEOF UnaryExpr_NoNode
	| PLUSPLUS UnaryExpr_NoNode
	| AUTOPLUSPLUS UnaryExpr_NoNode
	| MINUSMINUS UnaryExpr_NoNode
	| AUTOMINUSMINUS UnaryExpr_NoNode
	| '+' UnaryExpr_NoNode
	| '-' UnaryExpr_NoNode
	| '~' UnaryExpr_NoNode
	| '!' UnaryExpr_NoNode
	;
UnaryExpr_NoNode :
	 PostfixExpr_NoNode
	| UnaryExprCommon_NoNode
	;
UnaryExprNoBF_NoNode :
	 PostfixExprNoBF_NoNode
	| UnaryExprCommon_NoNode
	;
MultiplicativeExpr_NoNode :
	 UnaryExpr_NoNode
	| MultiplicativeExpr_NoNode '*' UnaryExpr_NoNode
	| MultiplicativeExpr_NoNode '/' UnaryExpr_NoNode
	| MultiplicativeExpr_NoNode '%' UnaryExpr_NoNode
	;
MultiplicativeExprNoBF_NoNode :
	 UnaryExprNoBF_NoNode
	| MultiplicativeExprNoBF_NoNode '*' UnaryExpr_NoNode
	| MultiplicativeExprNoBF_NoNode '/' UnaryExpr_NoNode
	| MultiplicativeExprNoBF_NoNode '%' UnaryExpr_NoNode
	;
AdditiveExpr_NoNode :
	 MultiplicativeExpr_NoNode
	| AdditiveExpr_NoNode '+' MultiplicativeExpr_NoNode
	| AdditiveExpr_NoNode '-' MultiplicativeExpr_NoNode
	;
AdditiveExprNoBF_NoNode :
	 MultiplicativeExprNoBF_NoNode
	| AdditiveExprNoBF_NoNode '+' MultiplicativeExpr_NoNode
	| AdditiveExprNoBF_NoNode '-' MultiplicativeExpr_NoNode
	;
ShiftExpr_NoNode :
	 AdditiveExpr_NoNode
	| ShiftExpr_NoNode LSHIFT AdditiveExpr_NoNode
	| ShiftExpr_NoNode RSHIFT AdditiveExpr_NoNode
	| ShiftExpr_NoNode URSHIFT AdditiveExpr_NoNode
	;
ShiftExprNoBF_NoNode :
	 AdditiveExprNoBF_NoNode
	| ShiftExprNoBF_NoNode LSHIFT AdditiveExpr_NoNode
	| ShiftExprNoBF_NoNode RSHIFT AdditiveExpr_NoNode
	| ShiftExprNoBF_NoNode URSHIFT AdditiveExpr_NoNode
	;
RelationalExpr_NoNode :
	 ShiftExpr_NoNode
	| RelationalExpr_NoNode '<' ShiftExpr_NoNode
	| RelationalExpr_NoNode '>' ShiftExpr_NoNode
	| RelationalExpr_NoNode LE ShiftExpr_NoNode
	| RelationalExpr_NoNode GE ShiftExpr_NoNode
	| RelationalExpr_NoNode INSTANCEOF ShiftExpr_NoNode
	| RelationalExpr_NoNode INTOKEN ShiftExpr_NoNode
	;
RelationalExprNoIn_NoNode :
	 ShiftExpr_NoNode
	| RelationalExprNoIn_NoNode '<' ShiftExpr_NoNode
	| RelationalExprNoIn_NoNode '>' ShiftExpr_NoNode
	| RelationalExprNoIn_NoNode LE ShiftExpr_NoNode
	| RelationalExprNoIn_NoNode GE ShiftExpr_NoNode
	| RelationalExprNoIn_NoNode INSTANCEOF ShiftExpr_NoNode
	;
RelationalExprNoBF_NoNode :
	 ShiftExprNoBF_NoNode
	| RelationalExprNoBF_NoNode '<' ShiftExpr_NoNode
	| RelationalExprNoBF_NoNode '>' ShiftExpr_NoNode
	| RelationalExprNoBF_NoNode LE ShiftExpr_NoNode
	| RelationalExprNoBF_NoNode GE ShiftExpr_NoNode
	| RelationalExprNoBF_NoNode INSTANCEOF ShiftExpr_NoNode
	| RelationalExprNoBF_NoNode INTOKEN ShiftExpr_NoNode
	;
EqualityExpr_NoNode :
	 RelationalExpr_NoNode
	| EqualityExpr_NoNode EQEQ RelationalExpr_NoNode
	| EqualityExpr_NoNode NE RelationalExpr_NoNode
	| EqualityExpr_NoNode STREQ RelationalExpr_NoNode
	| EqualityExpr_NoNode STRNEQ RelationalExpr_NoNode
	;
EqualityExprNoIn_NoNode :
	 RelationalExprNoIn_NoNode
	| EqualityExprNoIn_NoNode EQEQ RelationalExprNoIn_NoNode
	| EqualityExprNoIn_NoNode NE RelationalExprNoIn_NoNode
	| EqualityExprNoIn_NoNode STREQ RelationalExprNoIn_NoNode
	| EqualityExprNoIn_NoNode STRNEQ RelationalExprNoIn_NoNode
	;
EqualityExprNoBF_NoNode :
	 RelationalExprNoBF_NoNode
	| EqualityExprNoBF_NoNode EQEQ RelationalExpr_NoNode
	| EqualityExprNoBF_NoNode NE RelationalExpr_NoNode
	| EqualityExprNoBF_NoNode STREQ RelationalExpr_NoNode
	| EqualityExprNoBF_NoNode STRNEQ RelationalExpr_NoNode
	;
BitwiseANDExpr_NoNode :
	 EqualityExpr_NoNode
	| BitwiseANDExpr_NoNode '&' EqualityExpr_NoNode
	;
BitwiseANDExprNoIn_NoNode :
	 EqualityExprNoIn_NoNode
	| BitwiseANDExprNoIn_NoNode '&' EqualityExprNoIn_NoNode
	;
BitwiseANDExprNoBF_NoNode :
	 EqualityExprNoBF_NoNode
	| BitwiseANDExprNoBF_NoNode '&' EqualityExpr_NoNode
	;
BitwiseXORExpr_NoNode :
	 BitwiseANDExpr_NoNode
	| BitwiseXORExpr_NoNode '^' BitwiseANDExpr_NoNode
	;
BitwiseXORExprNoIn_NoNode :
	 BitwiseANDExprNoIn_NoNode
	| BitwiseXORExprNoIn_NoNode '^' BitwiseANDExprNoIn_NoNode
	;
BitwiseXORExprNoBF_NoNode :
	 BitwiseANDExprNoBF_NoNode
	| BitwiseXORExprNoBF_NoNode '^' BitwiseANDExpr_NoNode
	;
BitwiseORExpr_NoNode :
	 BitwiseXORExpr_NoNode
	| BitwiseORExpr_NoNode '|' BitwiseXORExpr_NoNode
	;
BitwiseORExprNoIn_NoNode :
	 BitwiseXORExprNoIn_NoNode
	| BitwiseORExprNoIn_NoNode '|' BitwiseXORExprNoIn_NoNode
	;
BitwiseORExprNoBF_NoNode :
	 BitwiseXORExprNoBF_NoNode
	| BitwiseORExprNoBF_NoNode '|' BitwiseXORExpr_NoNode
	;
LogicalANDExpr_NoNode :
	 BitwiseORExpr_NoNode
	| LogicalANDExpr_NoNode AND BitwiseORExpr_NoNode
	;
LogicalANDExprNoIn_NoNode :
	 BitwiseORExprNoIn_NoNode
	| LogicalANDExprNoIn_NoNode AND BitwiseORExprNoIn_NoNode
	;
LogicalANDExprNoBF_NoNode :
	 BitwiseORExprNoBF_NoNode
	| LogicalANDExprNoBF_NoNode AND BitwiseORExpr_NoNode
	;
LogicalORExpr_NoNode :
	 LogicalANDExpr_NoNode
	| LogicalORExpr_NoNode OR LogicalANDExpr_NoNode
	;
LogicalORExprNoIn_NoNode :
	 LogicalANDExprNoIn_NoNode
	| LogicalORExprNoIn_NoNode OR LogicalANDExprNoIn_NoNode
	;
LogicalORExprNoBF_NoNode :
	 LogicalANDExprNoBF_NoNode
	| LogicalORExprNoBF_NoNode OR LogicalANDExpr_NoNode
	;
ConditionalExpr_NoNode :
	 LogicalORExpr_NoNode
	| LogicalORExpr_NoNode '?' AssignmentExpr_NoNode ':' AssignmentExpr_NoNode
	;
ConditionalExprNoIn_NoNode :
	 LogicalORExprNoIn_NoNode
	| LogicalORExprNoIn_NoNode '?' AssignmentExprNoIn_NoNode ':' AssignmentExprNoIn_NoNode
	;
ConditionalExprNoBF_NoNode :
	 LogicalORExprNoBF_NoNode
	| LogicalORExprNoBF_NoNode '?' AssignmentExpr_NoNode ':' AssignmentExpr_NoNode
	;
AssignmentExpr_NoNode :
	 ConditionalExpr_NoNode
	| LeftHandSideExpr_NoNode AssignmentOperator_NoNode AssignmentExpr_NoNode
	;
AssignmentExprNoIn_NoNode :
	 ConditionalExprNoIn_NoNode
	| LeftHandSideExpr_NoNode AssignmentOperator_NoNode AssignmentExprNoIn_NoNode
	;
AssignmentExprNoBF_NoNode :
	 ConditionalExprNoBF_NoNode
	| LeftHandSideExprNoBF_NoNode AssignmentOperator_NoNode AssignmentExpr_NoNode
	;
AssignmentOperator_NoNode :
	 '='
	| PLUSEQUAL
	| MINUSEQUAL
	| MULTEQUAL
	| DIVEQUAL
	| LSHIFTEQUAL
	| RSHIFTEQUAL
	| URSHIFTEQUAL
	| ANDEQUAL
	| XOREQUAL
	| OREQUAL
	| MODEQUAL
	;
Expr_NoNode :
	 AssignmentExpr_NoNode
	| Expr_NoNode ',' AssignmentExpr_NoNode
	;
ExprNoIn_NoNode :
	 AssignmentExprNoIn_NoNode
	| ExprNoIn_NoNode ',' AssignmentExprNoIn_NoNode
	;
ExprNoBF_NoNode :
	 AssignmentExprNoBF_NoNode
	| ExprNoBF_NoNode ',' AssignmentExpr_NoNode
	;
Statement_NoNode :
	 Block_NoNode
	| VariableStatement_NoNode
	| ConstStatement_NoNode
	| FunctionDeclaration_NoNode
	| EmptyStatement_NoNode
	| ExprStatement_NoNode
	| IfStatement_NoNode
	| IterationStatement_NoNode
	| ContinueStatement_NoNode
	| BreakStatement_NoNode
	| ReturnStatement_NoNode
	| WithStatement_NoNode
	| SwitchStatement_NoNode
	| LabelledStatement_NoNode
	| ThrowStatement_NoNode
	| TryStatement_NoNode
	| DebuggerStatement_NoNode
	;
Block_NoNode :
	 OPENBRACE CLOSEBRACE
	| OPENBRACE SourceElements_NoNode CLOSEBRACE
	;
VariableStatement_NoNode :
	 VAR VariableDeclarationList_NoNode ';'
	//| VAR VariableDeclarationList_NoNode error
	;
VariableDeclarationList_NoNode :
	 IDENT
	| IDENT Initializer_NoNode
	| VariableDeclarationList_NoNode ',' IDENT
	| VariableDeclarationList_NoNode ',' IDENT Initializer_NoNode
	;
VariableDeclarationListNoIn_NoNode :
	 IDENT
	| IDENT InitializerNoIn_NoNode
	| VariableDeclarationListNoIn_NoNode ',' IDENT
	| VariableDeclarationListNoIn_NoNode ',' IDENT InitializerNoIn_NoNode
	;
ConstStatement_NoNode :
	 CONSTTOKEN ConstDeclarationList_NoNode ';'
	//| CONSTTOKEN ConstDeclarationList_NoNode error
	;
ConstDeclarationList_NoNode :
	 ConstDeclaration_NoNode
	| ConstDeclarationList_NoNode ',' ConstDeclaration_NoNode
	;
ConstDeclaration_NoNode :
	 IDENT
	| IDENT Initializer_NoNode
	;
Initializer_NoNode :
	 '=' AssignmentExpr_NoNode
	;
InitializerNoIn_NoNode :
	 '=' AssignmentExprNoIn_NoNode
	;
EmptyStatement_NoNode :
	 ';'
	;
ExprStatement_NoNode :
	 ExprNoBF_NoNode ';'
	//| ExprNoBF_NoNode error
	;
IfStatement_NoNode :
	 IF '(' Expr_NoNode ')' Statement_NoNode %prec IF_WITHOUT_ELSE /*1N*/
	| IF '(' Expr_NoNode ')' Statement_NoNode ELSE /*2N*/ Statement_NoNode
	;
IterationStatement_NoNode :
	 DO Statement_NoNode WHILE '(' Expr_NoNode ')' ';'
	//| DO Statement_NoNode WHILE '(' Expr_NoNode ')' error
	| WHILE '(' Expr_NoNode ')' Statement_NoNode
	| FOR '(' ExprNoInOpt_NoNode ';' ExprOpt_NoNode ';' ExprOpt_NoNode ')' Statement_NoNode
	| FOR '(' VAR VariableDeclarationListNoIn_NoNode ';' ExprOpt_NoNode ';' ExprOpt_NoNode ')' Statement_NoNode
	| FOR '(' LeftHandSideExpr_NoNode INTOKEN Expr_NoNode ')' Statement_NoNode
	| FOR '(' VAR IDENT INTOKEN Expr_NoNode ')' Statement_NoNode
	| FOR '(' VAR IDENT InitializerNoIn_NoNode INTOKEN Expr_NoNode ')' Statement_NoNode
	;
ExprOpt_NoNode :
	 /*empty*/
	| Expr_NoNode
	;
ExprNoInOpt_NoNode :
	 /*empty*/
	| ExprNoIn_NoNode
	;
ContinueStatement_NoNode :
	 CONTINUE ';'
	//| CONTINUE error
	| CONTINUE IDENT ';'
	//| CONTINUE IDENT error
	;
BreakStatement_NoNode :
	 BREAK ';'
	//| BREAK error
	| BREAK IDENT ';'
	//| BREAK IDENT error
	;
ReturnStatement_NoNode :
	 RETURN ';'
	//| RETURN error
	| RETURN Expr_NoNode ';'
	//| RETURN Expr_NoNode error
	;
WithStatement_NoNode :
	 WITH '(' Expr_NoNode ')' Statement_NoNode
	;
SwitchStatement_NoNode :
	 SWITCH '(' Expr_NoNode ')' CaseBlock_NoNode
	;
CaseBlock_NoNode :
	 OPENBRACE CaseClausesOpt_NoNode CLOSEBRACE
	| OPENBRACE CaseClausesOpt_NoNode DefaultClause_NoNode CaseClausesOpt_NoNode CLOSEBRACE
	;
CaseClausesOpt_NoNode :
	 /*empty*/
	| CaseClauses_NoNode
	;
CaseClauses_NoNode :
	 CaseClause_NoNode
	| CaseClauses_NoNode CaseClause_NoNode
	;
CaseClause_NoNode :
	 CASE Expr_NoNode ':'
	| CASE Expr_NoNode ':' SourceElements_NoNode
	;
DefaultClause_NoNode :
	 DEFAULT ':'
	| DEFAULT ':' SourceElements_NoNode
	;
LabelledStatement_NoNode :
	 IDENT ':' Statement_NoNode
	;
ThrowStatement_NoNode :
	 THROW Expr_NoNode ';'
	//| THROW Expr_NoNode error
	;
TryStatement_NoNode :
	 TRY Block_NoNode FINALLY Block_NoNode
	| TRY Block_NoNode CATCH '(' IDENT ')' Block_NoNode
	| TRY Block_NoNode CATCH '(' IDENT ')' Block_NoNode FINALLY Block_NoNode
	;
DebuggerStatement_NoNode :
	 DEBUGGER ';'
	//| DEBUGGER error
	;
FunctionDeclaration_NoNode :
	 FUNCTION IDENT '(' ')' OPENBRACE FunctionBody_NoNode CLOSEBRACE
	| FUNCTION IDENT '(' FormalParameterList_NoNode ')' OPENBRACE FunctionBody_NoNode CLOSEBRACE
	;
FunctionExpr_NoNode :
	 FUNCTION '(' ')' OPENBRACE FunctionBody_NoNode CLOSEBRACE
	| FUNCTION '(' FormalParameterList_NoNode ')' OPENBRACE FunctionBody_NoNode CLOSEBRACE
	| FUNCTION IDENT '(' ')' OPENBRACE FunctionBody_NoNode CLOSEBRACE
	| FUNCTION IDENT '(' FormalParameterList_NoNode ')' OPENBRACE FunctionBody_NoNode CLOSEBRACE
	;
FormalParameterList_NoNode :
	 IDENT
	| FormalParameterList_NoNode ',' IDENT
	;
FunctionBody_NoNode :
	 /*empty*/
	| SourceElements_NoNode
	;
SourceElements_NoNode :
	 Statement_NoNode
	| SourceElements_NoNode Statement_NoNode
	;

//Lexer

ELSE : 'else' ;
NULLTOKEN : 'null' ;
TRUETOKEN : 'true' ;
FALSETOKEN : 'false' ;
NUMBER : "0[xX][a-fA-F0-9]+|[0-9]+([Ee][+-]?[0-9]+)?|[0-9]*\\.[0-9]+([Ee][+-]?[0-9]+)?" ;
STRING : "\"[^\"\n]*\"|'[^'\n]*'" ;
DIVEQUAL : '/=' ;
IDENT : "[a-zA-Z_$][a-zA-Z_0-9]*" ;
OPENBRACE : '{' ;
CLOSEBRACE : '}' ;
THISTOKEN : 'this' ;
NEW : 'new' ;
PLUSPLUS : '++' ;
MINUSMINUS : '--' ;
DELETETOKEN : 'delete' ;
VOIDTOKEN : 'void' ;
TYPEOF : 'typeof' ;
AUTOPLUSPLUS : 'AUTOPLUSPLUS' ;
AUTOMINUSMINUS : 'AUTOMINUSMINUS' ;
LSHIFT : '<<' ;
RSHIFT : '>>' ;
URSHIFT : '>>>' ;
LE : '<=' ;
GE : '>=' ;
INSTANCEOF : 'instanceof' ;
INTOKEN : 'in' ;
EQEQ : '==' ;
NE : '!=' ;
STREQ : '===' ;
STRNEQ : '!==' ;
AND : '&&' ;
OR : '||' ;
PLUSEQUAL : '+=' ;
MINUSEQUAL : '-=' ;
MULTEQUAL : '*=' ;
LSHIFTEQUAL : '<<=' ;
RSHIFTEQUAL : '>>=' ;
URSHIFTEQUAL : '>>>=' ;
ANDEQUAL : '&=' ;
XOREQUAL : '^=' ;
OREQUAL : '|=' ;
MODEQUAL : '%=' ;
VAR : 'var' ;
CONSTTOKEN : 'const' ;
IF : 'if' ;
DO : 'do' ;
WHILE : 'while' ;
FOR : 'for' ;
CONTINUE : 'continue' ;
BREAK : 'break' ;
RETURN : 'return' ;
WITH : 'with' ;
SWITCH : 'switch' ;
CASE : 'case' ;
DEFAULT : 'default' ;
THROW : 'throw' ;
TRY : 'try' ;
FINALLY : 'finally' ;
CATCH : 'catch' ;
DEBUGGER : 'debugger' ;
FUNCTION : 'function' ;

}