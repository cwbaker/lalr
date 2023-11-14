// From: https://github.com/Logicalshift/TameParse/blob/master/TameParse/Language/definition.tp
///
/// Parser Language Definition
///
/// Created by Andrew Hunter on 21/05/2011
///

tameparse  {

%whitespace "[ \t\r\n]*";
%whitespace "//[^\n]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";


/// The grammar itself

//
// The top-level definitions
//

Parser_Language :
	TopLevel_Block_zm
	;

TopLevel_Block_zm :
	TopLevel_Block
	| TopLevel_Block_zm TopLevel_Block
	| /*empty*/
	;

TopLevel_Block :
	Language_Block
	| Import_Block
	| Parser_Block
	| Test_Block
	;

Language_Block :
	language identifier Language_Inherits_zo '{' Language_Definition_zm '}'
	;

Import_Block :
	import string
	;

Language_Inherits_zo :
	Language_Inherits
	| /*empty*/
	;

Language_Inherits :
	':' identifier
	;

//
// The language block
//

Language_Definition_zm :
	Language_Definition
	| Language_Definition_zm Language_Definition
	| /*empty*/
	;

Language_Definition :
	Lexer_Symbols_Definition
	| Lexer_Definition
	| Ignore_Definition
	| Keywords_Definition
	| Grammar_Definition
	| Precedence_Definition
	;

//
// Basic language items
//

Lexer_Symbols_Definition :
	Lexer_Symbols_Modifier_zm lexer_symbols '{' Lexeme_Definition_zm '}'
	;

Lexer_Definition :
	Lexer_Modifier_zm lexer '{' Lexeme_Definition_zm '}'
	;

Ignore_Definition :
	ignore '{' Keyword_Definition_zm '}'
	;

Keywords_Definition :
	Lexer_Modifier_zm keywords '{' Keyword_Definition_zm '}'
	;

Lexer_Modifier_zm :
    Lexer_Modifier
    | Lexer_Modifier_zm Lexer_Modifier
    | /*eempty*/
    ;

Lexer_Modifier :
	weak
	| case sensitive
	| case insensitive
	;

Lexer_Symbols_Modifier_zm :
    Lexer_Symbols_Modifier
    | Lexer_Symbols_Modifier_zm Lexer_Symbols_Modifier
    | /*empty*/
    ;

Lexer_Symbols_Modifier :
	case sensitive
	| case insensitive
	;

Keyword_Definition_zm :
    Keyword_Definition
    | Keyword_Definition_zm Keyword_Definition
    | /*empty*/
    ;

Keyword_Definition :
	identifier
	| Lexeme_Definition
	;

Lexeme_Definition_zm :
    Lexeme_Definition
    | Lexer_Definition_zm Lexeme_Definition
    | /*empty*/
    ;

Lexer_Definition_zm :
    Lexer_Definition
    | Lexer_Definition_zm Lexer_Definition
    | /*empty*/
    ;

Lexeme_Definition :
	identifier Assign_sym LX_rhs
	| replace identifier '=' LX_rhs
	| identifier '=' identifier '.' identifier
	;

Assign_sym :
    '='
    | "|="
    ;

LX_rhs :
    regex
    | string
    | character
    ;

//
// Defining grammars
//

Grammar_Definition :
	grammar '{' Nonterminal_Definition_zm '}'
	;

Nonterminal_Definition_zm :
    Nonterminal_Definition
    | Nonterminal_Definition_zm Nonterminal_Definition
    | /*empty*/
    ;

Nonterminal_Definition :
	nonterminal Assign_sym Production OrProduction_zm
	| replace nonterminal '=' Production OrProduction_zm
	;

OrProduction_zm :
    OrProduction
    | OrProduction_zm OrProduction
    | /*empty*/
    ;

OrProduction :
    '|' Production
    ;

// Top level is just a simple EBNF term, as the '|' operator creates a new production at this point
Production :
	Simple_Ebnf_Item_zm
	;

Ebnf_Item :
	Simple_Ebnf_Item_zm
	| Simple_Ebnf_Item_zm '|' Ebnf_Item
	;

Simple_Ebnf_Item_zm :
    Simple_Ebnf_Item
    | Simple_Ebnf_Item_zm Simple_Ebnf_Item
    | /*empty*/
    ;

Simple_Ebnf_Item :
	Nonterminal Semantic_Specification_zo
	| Terminal Semantic_Specification_zo
	| Guard Semantic_Specification_zo
	| Simple_Ebnf_Item '*' Semantic_Specification_zo
	| Simple_Ebnf_Item '+' Semantic_Specification_zo
	| Simple_Ebnf_Item '?' Semantic_Specification_zo
	| '(' Ebnf_Item ')' Semantic_Specification_zo
	;

Guard :
	"[=>" Ebnf_Item ']'
	| "[=>" '[' can_clash ']' Ebnf_Item ']'
	;

Nonterminal :
	nonterminal
	| identifier '.' nonterminal
	;

Terminal :
	Basic_Terminal
	| identifier '.' Basic_Terminal
	;

Basic_Terminal :
	identifier
	| string
	| character
	;

//
// Semantics
//

Semantic_Specification_zo :
    Semantic_Specification
    | /*empty*/
    ;

Semantic_Specification :
	'[' Semantic_Item Comma_Semantic_Item_zm ']'
	;

Comma_Semantic_Item_zm :
    Comma_Semantic_Item
    | Comma_Semantic_Item_zm Comma_Semantic_Item
    | /*empty*/
    ;

Comma_Semantic_Item :
    ',' Semantic_Item
    ;

Semantic_Item :
	identifier
	| conflict '=' shift
	| conflict '=' reduce
	| conflict '=' weak reduce
	;

//
// Defining precedence
//

Precedence_Definition :
	precedence '{' Precedence_Item_zm '}'
	;

Precedence_Item_zm :
    Precedence_Item
    | Precedence_Item_zm Precedence_Item
    | /*empty*/
    ;

Precedence_Item :
	left Equal_Precedence_Items
	| right Equal_Precedence_Items
	| non_associative  Equal_Precedence_Items
	| non_assoc Equal_Precedence_Items
	;

Equal_Precedence_Items :
	Simple_Ebnf_Item
	| '{' Simple_Ebnf_Item_zm '}'
	;

//
// The parser declaration block
//

Parser_Block :
	parser identifier ':' identifier '{' Parser_StartSymbol_zo '}'
	;

Parser_StartSymbol_zo :
    Parser_StartSymbol
    | /*empty*/
    ;

Parser_StartSymbol :
	Nonterminal
	;

//
// Test definition block
//

Test_Block :
	test identifier '{' Test_Definition_zm '}'
	;

Test_Definition_zm :
    Test_Definition
    | Test_Definition_zm Test_Definition
    | /*empty*/
    ;

Test_Definition :
	Nonterminal '=' Test_Specification_om
	| Nonterminal "!=" Test_Specification_om
	| Nonterminal from Test_Specification_om
	;

Test_Specification_om :
    Test_Specification
    | Test_Specification_om Test_Specification
    ;

Test_Specification :
	string
	| identifier '(' string ')'
	;


//Tokens

/// Symbols used by the lexer

// Commonly used character sets
//letter : "[a-zA-Z_-]" ; // /({unicode-letter}|{unicode-punctuation-dash}|{unicode-punctuation-connector})/
//digit : "[0-9]" ; // /{unicode-number}/
//alphanumeric : letter | digit ; // /({letter}|{digit})/
//whitespace		= /{unicode-separator}|[\t]/
//anycharacter	= /[^\n\r]/

// Characters valid in regular expressions and strings
//first_re_char : "[^/\*]|(\\/)" ;
//regex_char : "[^/]|(\\/)" ;
//string_char : "[^\"]|(\\\")" ;

// Identifiers, regular expressions
identifier	: "[a-zA-Z_-][a-zA-Z_0-9-]*" ;
regex : "/([^/\*]|(\\/))([^/]|(\\/))*/" ; // /\/{first-re-char}{regex-char}*\//

/// Lexical symbols
//identifier		= /{identifier}/
nonterminal : "<[a-zA-Z_-][a-zA-Z_0-9-]*>" ; // /\<{identifier}\>/
//regex			= /{regex}/
string : "\"([^\"]|(\\\"))*\"" ; // /"{string-char}*"/
character : "'(.|\\.)'" ; // /'(.|\\.)'/

/// Ignored symbols
//newline			= /[\n\r]+/
//whitespace		= /{whitespace}+/
//comment			= /\/\/({anycharacter}*)/
//c-comment		= /\/\*([^*]|\*[^\/])*\*\//

/// Keywords
/// (We use very few keywords, to extend the range of identifiers available)

/// Weak keywords
/// Declared here to suppress warnings
language : 'language' ;
import : 'import' ;
lexer_symbols : 'lexer-symbols' ;
lexer : 'lexer' ;
ignore : 'ignore' ;
weak : 'weak' ;
keywords : 'keywords' ;
grammar : 'grammar' ;
replace : 'replace' ;
parser : 'parser' ;
test : 'test' ;
from : 'from' ;
case : 'case' ;
sensitive : 'sensitive' ;
insensitive : 'insensitive' ;
precedence : 'precedence' ;

left : 'left' ;
right : 'right' ;
non_associative : 'non-associative' ;
non_assoc : 'non-assoc' ;

conflict : 'conflict' ;
shift : 'shift' ;
reduce : 'reduce' ;

can_clash : 'can-clash' ;

}