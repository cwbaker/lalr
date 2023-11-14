//From: chaperon-3.0-final

Java11 {

%whitespace "[ \t\r\n]*";
%whitespace "//[^\n]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

//%token ABSTRACT
//%token BOOLEAN
//%token BREAK
//%token BYTE
//%token BYVALUE
//%token CASE
//%token CAST
//%token CATCH
//%token CHAR
//%token CLASS
//%token CONST
//%token CONTINUE
//%token DEFAULT
//%token DO
//%token DOUBLE
//%token ELSE
//%token EXTENDS
//%token FINAL
//%token FINALLY
//%token FLOAT
//%token FOR
//%token FUTURE
//%token GENERIC
//%token GOTO
//%token IF
//%token IMPLEMENTS
//%token IMPORT
//%token INNER
//%token INSTANCEOF
//%token INT
//%token INTERFACE
//%token LONG
//%token NATIVE
//%token NEW
//%token JNULL
//%token OPERATOR
//%token OUTER
//%token PACKAGE
//%token PRIVATE
//%token PROTECTED
//%token PUBLIC
//%token REST
//%token RETURN
//%token SHORT
//%token STATIC
//%token SUPER
//%token SWITCH
//%token SYNCHRONIZED
//%token THIS
//%token THROW
//%token THROWS
//%token TRANSIENT
//%token TRY
//%token VAR
//%token VOID
//%token VOLATILE
//%token WHILE
//%token OP_INC
//%token OP_DEC
//%token OP_SHL
//%token OP_SHR
//%token OP_SHRR
//%token OP_GE
//%token OP_LE
//%token OP_EQ
//%token OP_NE
//%token OP_LAND
//%token OP_LOR
//%token OP_DIM
//%token ASS_MUL
//%token ASS_DIV
//%token ASS_MOD
//%token ASS_ADD
//%token ASS_SUB
//%token ASS_SHL
//%token ASS_SHR
//%token ASS_SHRR
//%token ASS_AND
//%token ASS_XOR
//%token ASS_OR
//%token IDENTIFIER
//%token LITERAL
//%token BOOLLIT
//%token ','
//%token ';'
//%token '.'
//%token '*'
//%token '{'
//%token '}'
//%token '='
//%token '('
//%token ')'
//%token ':'
//%token '['
//%token ']'
//%token '~'
//%token '!'
//%token '+'
//%token '-'
//%token '/'
//%token '%'
//%token '<'
//%token '>'
//%token '&'
//%token '^'
//%token '|'
//%token '?'

%nonassoc /*1*/ IF_WITHOUT_ELSE ;
%nonassoc /*2*/ ELSE ;

//%start CompilationUnit

//%%

CompilationUnit :
	 ProgramFile
	;

TypeSpecifier :
	 TypeName
	| TypeName Dims
	;
TypeName :
	 PrimitiveType
	| QualifiedName
	;
ClassNameList :
	 QualifiedName
	| ClassNameList ',' QualifiedName
	;
PrimitiveType :
	 BOOLEAN
	| CHAR
	| BYTE
	| SHORT
	| INT
	| LONG
	| FLOAT
	| DOUBLE
	| VOID
	;
SemiColons :
	 ';'
	| SemiColons ';'
	;
ProgramFile :
	 PackageStatement ImportStatements TypeDeclarations
	| PackageStatement ImportStatements
	| PackageStatement TypeDeclarations
	| ImportStatements TypeDeclarations
	| PackageStatement
	| ImportStatements
	| TypeDeclarations
	;
PackageStatement :
	 PACKAGE QualifiedName SemiColons
	;
TypeDeclarations :
	 TypeDeclarationOptSemi
	| TypeDeclarations TypeDeclarationOptSemi
	;
TypeDeclarationOptSemi :
	 TypeDeclaration
	| TypeDeclaration SemiColons
	;
ImportStatements :
	 ImportStatement
	| ImportStatements ImportStatement
	;
ImportStatement :
	 IMPORT QualifiedName SemiColons
	| IMPORT QualifiedName '.' '*' SemiColons
	;
QualifiedName :
	 IDENTIFIER
	| QualifiedName '.' IDENTIFIER
	;
TypeDeclaration :
	 ClassHeader '{' FieldDeclarations '}'
	| ClassHeader '{' '}'
	;
ClassHeader :
	 Modifiers ClassWord IDENTIFIER Extends Interfaces
	| Modifiers ClassWord IDENTIFIER Extends
	| Modifiers ClassWord IDENTIFIER Interfaces
	| ClassWord IDENTIFIER Extends Interfaces
	| Modifiers ClassWord IDENTIFIER
	| ClassWord IDENTIFIER Extends
	| ClassWord IDENTIFIER Interfaces
	| ClassWord IDENTIFIER
	;
Modifiers :
	 Modifier
	| Modifiers Modifier
	;
Modifier :
	 ABSTRACT
	| FINAL
	| PUBLIC
	| PROTECTED
	| PRIVATE
	| STATIC
	| TRANSIENT
	| VOLATILE
	| NATIVE
	| SYNCHRONIZED
	;
ClassWord :
	 CLASS
	| INTERFACE
	;
Interfaces :
	 IMPLEMENTS ClassNameList
	;
FieldDeclarations :
	 FieldDeclarationOptSemi
	| FieldDeclarations FieldDeclarationOptSemi
	;
FieldDeclarationOptSemi :
	 FieldDeclaration
	| FieldDeclaration SemiColons
	;
FieldDeclaration :
	 FieldVariableDeclaration ';'
	| MethodDeclaration
	| ConstructorDeclaration
	| StaticInitializer
	| NonStaticInitializer
	| TypeDeclaration
	;
FieldVariableDeclaration :
	 Modifiers TypeSpecifier VariableDeclarators
	| TypeSpecifier VariableDeclarators
	;
VariableDeclarators :
	 VariableDeclarator
	| VariableDeclarators ',' VariableDeclarator
	;
VariableDeclarator :
	 DeclaratorName
	| DeclaratorName '=' VariableInitializer
	;
VariableInitializer :
	 Expression
	| '{' '}'
	| '{' ArrayInitializers '}'
	;
ArrayInitializers :
	 VariableInitializer
	| ArrayInitializers ',' VariableInitializer
	| ArrayInitializers ','
	;
MethodDeclaration :
	 Modifiers TypeSpecifier MethodDeclarator Throws MethodBody
	| Modifiers TypeSpecifier MethodDeclarator MethodBody
	| TypeSpecifier MethodDeclarator Throws MethodBody
	| TypeSpecifier MethodDeclarator MethodBody
	;
MethodDeclarator :
	 DeclaratorName '(' ParameterList ')'
	| DeclaratorName '(' ')'
	| MethodDeclarator OP_DIM
	;
ParameterList :
	 Parameter
	| ParameterList ',' Parameter
	;
Parameter :
	 TypeSpecifier DeclaratorName
	| FINAL TypeSpecifier DeclaratorName
	;
DeclaratorName :
	 IDENTIFIER
	| DeclaratorName OP_DIM
	;
Throws :
	 THROWS ClassNameList
	;
MethodBody :
	 Block
	| ';'
	;
ConstructorDeclaration :
	 Modifiers ConstructorDeclarator Throws Block
	| Modifiers ConstructorDeclarator Block
	| ConstructorDeclarator Throws Block
	| ConstructorDeclarator Block
	;
ConstructorDeclarator :
	 IDENTIFIER '(' ParameterList ')'
	| IDENTIFIER '(' ')'
	;
StaticInitializer :
	 STATIC Block
	;
NonStaticInitializer :
	 Block
	;
Extends :
	 EXTENDS TypeName
	| Extends ',' TypeName
	;
Block :
	 '{' LocalVariableDeclarationsAndStatements '}'
	| '{' '}'
	;
LocalVariableDeclarationsAndStatements :
	 LocalVariableDeclarationOrStatement
	| LocalVariableDeclarationsAndStatements LocalVariableDeclarationOrStatement
	;
LocalVariableDeclarationOrStatement :
	 LocalVariableDeclarationStatement
	| Statement
	;
LocalVariableDeclarationStatement :
	 TypeSpecifier VariableDeclarators ';'
	| FINAL TypeSpecifier VariableDeclarators ';'
	;
Statement :
	 EmptyStatement
	| LabelStatement
	| ExpressionStatement ';'
	| SelectionStatement
	| IterationStatement
	| JumpStatement
	| GuardingStatement
	| Block
	;
EmptyStatement :
	 ';'
	;
LabelStatement :
	 IDENTIFIER ':'
	| CASE ConstantExpression ':'
	| DEFAULT ':'
	;
ExpressionStatement :
	 Expression
	;
SelectionStatement :
	 IF '(' Expression ')' Statement %prec IF_WITHOUT_ELSE
	| IF '(' Expression ')' Statement ELSE Statement
	| SWITCH '(' Expression ')' Block
	;
IterationStatement :
	 WHILE '(' Expression ')' Statement
	| DO Statement WHILE '(' Expression ')' ';'
	| FOR '(' ForInit ForExpr ForIncr ')' Statement
	| FOR '(' ForInit ForExpr ')' Statement
	;
ForInit :
	 ExpressionStatements ';'
	| LocalVariableDeclarationStatement
	| ';'
	;
ForExpr :
	 Expression ';'
	| ';'
	;
ForIncr :
	 ExpressionStatements
	;
ExpressionStatements :
	 ExpressionStatement
	| ExpressionStatements ',' ExpressionStatement
	;
JumpStatement :
	 BREAK IDENTIFIER ';'
	| BREAK ';'
	| CONTINUE IDENTIFIER ';'
	| CONTINUE ';'
	| RETURN Expression ';'
	| RETURN ';'
	| THROW Expression ';'
	;
GuardingStatement :
	 SYNCHRONIZED '(' Expression ')' Statement
	| TRY Block Finally
	| TRY Block Catches
	| TRY Block Catches Finally
	;
Catches :
	 Catch
	| Catches Catch
	;
Catch :
	 CatchHeader Block
	;
CatchHeader :
	 CATCH '(' TypeSpecifier IDENTIFIER ')'
	| CATCH '(' TypeSpecifier ')'
	;
Finally :
	 FINALLY Block
	;
PrimaryExpression :
	 QualifiedName
	| NotJustName
	;
NotJustName :
	 SpecialName
	| NewAllocationExpression
	| ComplexPrimary
	;
ComplexPrimary :
	 '(' Expression ')'
	| ComplexPrimaryNoParenthesis
	;
ComplexPrimaryNoParenthesis :
	 LITERAL
	| BOOLLIT
	| ArrayAccess
	| FieldAccess
	| MethodCall
	;
ArrayAccess :
	 QualifiedName '[' Expression ']'
	| ComplexPrimary '[' Expression ']'
	;
FieldAccess :
	 NotJustName '.' IDENTIFIER
	| RealPostfixExpression '.' IDENTIFIER
	| QualifiedName '.' THIS
	| QualifiedName '.' CLASS
	| PrimitiveType '.' CLASS
	;
MethodCall :
	 MethodAccess '(' ArgumentList ')'
	| MethodAccess '(' ')'
	;
MethodAccess :
	 ComplexPrimaryNoParenthesis
	| SpecialName
	| QualifiedName
	;
SpecialName :
	 THIS
	| SUPER
	| JNULL
	;
ArgumentList :
	 Expression
	| ArgumentList ',' Expression
	;
NewAllocationExpression :
	 PlainNewAllocationExpression
	| QualifiedName '.' PlainNewAllocationExpression
	;
PlainNewAllocationExpression :
	 ArrayAllocationExpression
	| ClassAllocationExpression
	| ArrayAllocationExpression '{' '}'
	| ClassAllocationExpression '{' '}'
	| ArrayAllocationExpression '{' ArrayInitializers '}'
	| ClassAllocationExpression '{' FieldDeclarations '}'
	;
ClassAllocationExpression :
	 NEW TypeName '(' ArgumentList ')'
	| NEW TypeName '(' ')'
	;
ArrayAllocationExpression :
	 NEW TypeName DimExprs Dims
	| NEW TypeName DimExprs
	| NEW TypeName Dims
	;
DimExprs :
	 DimExpr
	| DimExprs DimExpr
	;
DimExpr :
	 '[' Expression ']'
	;
Dims :
	 OP_DIM
	| Dims OP_DIM
	;
PostfixExpression :
	 PrimaryExpression
	| RealPostfixExpression
	;
RealPostfixExpression :
	 PostfixExpression OP_INC
	| PostfixExpression OP_DEC
	;
UnaryExpression :
	 OP_INC UnaryExpression
	| OP_DEC UnaryExpression
	| ArithmeticUnaryOperator CastExpression
	| LogicalUnaryExpression
	;
LogicalUnaryExpression :
	 PostfixExpression
	| LogicalUnaryOperator UnaryExpression
	;
LogicalUnaryOperator :
	 '~'
	| '!'
	;
ArithmeticUnaryOperator :
	 '+'
	| '-'
	;
CastExpression :
	 UnaryExpression
	| '(' PrimitiveTypeExpression ')' CastExpression
	| '(' ClassTypeExpression ')' CastExpression
	| '(' Expression ')' LogicalUnaryExpression
	;
PrimitiveTypeExpression :
	 PrimitiveType
	| PrimitiveType Dims
	;
ClassTypeExpression :
	 QualifiedName Dims
	;
MultiplicativeExpression :
	 CastExpression
	| MultiplicativeExpression '*' CastExpression
	| MultiplicativeExpression '/' CastExpression
	| MultiplicativeExpression '%' CastExpression
	;
AdditiveExpression :
	 MultiplicativeExpression
	| AdditiveExpression '+' MultiplicativeExpression
	| AdditiveExpression '-' MultiplicativeExpression
	;
ShiftExpression :
	 AdditiveExpression
	| ShiftExpression OP_SHL AdditiveExpression
	| ShiftExpression OP_SHR AdditiveExpression
	| ShiftExpression OP_SHRR AdditiveExpression
	;
RelationalExpression :
	 ShiftExpression
	| RelationalExpression '<' ShiftExpression
	| RelationalExpression '>' ShiftExpression
	| RelationalExpression OP_LE ShiftExpression
	| RelationalExpression OP_GE ShiftExpression
	| RelationalExpression INSTANCEOF TypeSpecifier
	;
EqualityExpression :
	 RelationalExpression
	| EqualityExpression OP_EQ RelationalExpression
	| EqualityExpression OP_NE RelationalExpression
	;
AndExpression :
	 EqualityExpression
	| AndExpression '&' EqualityExpression
	;
ExclusiveOrExpression :
	 AndExpression
	| ExclusiveOrExpression '^' AndExpression
	;
InclusiveOrExpression :
	 ExclusiveOrExpression
	| InclusiveOrExpression '|' ExclusiveOrExpression
	;
ConditionalAndExpression :
	 InclusiveOrExpression
	| ConditionalAndExpression OP_LAND InclusiveOrExpression
	;
ConditionalOrExpression :
	 ConditionalAndExpression
	| ConditionalOrExpression OP_LOR ConditionalAndExpression
	;
ConditionalExpression :
	 ConditionalOrExpression
	| ConditionalOrExpression '?' Expression ':' ConditionalExpression
	;
AssignmentExpression :
	 ConditionalExpression
	| UnaryExpression AssignmentOperator AssignmentExpression
	;
AssignmentOperator :
	 '='
	| ASS_MUL
	| ASS_DIV
	| ASS_MOD
	| ASS_ADD
	| ASS_SUB
	| ASS_SHL
	| ASS_SHR
	| ASS_SHRR
	| ASS_AND
	| ASS_XOR
	| ASS_OR
	;
Expression :
	 AssignmentExpression
	;
ConstantExpression :
	 ConditionalExpression
	;

//Lexer

BOOLEAN : 'boolean' ;
CHAR : 'char' ;
BYTE : 'byte' ;
SHORT : 'short' ;
INT : 'int' ;
LONG : 'long' ;
FLOAT : 'float' ;
DOUBLE : 'double' ;
VOID : 'void' ;
PACKAGE : 'package' ;
IMPORT : 'import' ;
IDENTIFIER : "[a-zA-Z_][a-zA-Z_0-9]*" ;
ABSTRACT : 'abstract' ;
FINAL : 'final' ;
PUBLIC : 'public' ;
PROTECTED : 'protected' ;
PRIVATE : 'private' ;
STATIC : 'static' ;
TRANSIENT : 'transient' ;
VOLATILE : 'volatile' ;
NATIVE : 'native' ;
SYNCHRONIZED : 'synchronized' ;
CLASS : 'class' ;
INTERFACE : 'interface' ;
IMPLEMENTS : 'implements' ;
OP_DIM : '[]' ;
THROWS : 'throws' ;
EXTENDS : 'extends' ;
CASE : 'case' ;
DEFAULT : 'default' ;
IF : 'if' ;
ELSE : 'else' ;
SWITCH : 'switch' ;
WHILE : 'while' ;
DO : 'do' ;
FOR : 'for' ;
BREAK : 'break' ;
CONTINUE : 'continue' ;
RETURN : 'return' ;
THROW : 'throw' ;
TRY : 'try' ;
CATCH : 'catch' ;
FINALLY : 'finally' ;
LITERAL : "\"([^\"\n]|\\[^\n])*\"" ;
LITERAL : "'([^'\n]|\\[^\n])'" ;
LITERAL : "0[xX][a-fA-F0-9]+|[0-9]+([Ee][+-]?[0-9]+)?|[0-9]*\\.[0-9]+([Ee][+-]?[0-9]+)?" ;
BOOLLIT : 'true' | 'false' ;
THIS : 'this' ;
SUPER : 'super' ;
JNULL : 'null' ;
NEW : 'new' ;
OP_INC : '++' ;
OP_DEC : '--' ;
OP_SHL : '<<' ;
OP_SHR : '>>' ;
OP_SHRR : '>>>' ;
OP_LE : '<=' ;
OP_GE : '>=' ;
INSTANCEOF : 'instanceof' ;
OP_EQ : '==' ;
OP_NE : '!=' ;
OP_LAND : '&&' ;
OP_LOR : '||' ;
ASS_MUL : '*=' ;
ASS_DIV : '/=' ;
ASS_MOD : '%=' ;
ASS_ADD : '+=' ;
ASS_SUB : '-=' ;
ASS_SHL : '<<=' ;
ASS_SHR : '>>=' ;
ASS_SHRR : '>>>=' ;
ASS_AND : '&=' ;
ASS_XOR : '^=' ;
ASS_OR : '|=' ;

}
