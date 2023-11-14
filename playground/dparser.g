//From: https://github.com/jplevyak/dparser/blob/master/grammar.g

dparser {

%whitespace "[ \t]*";
%whitespace "//[^\r\n]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

grammar :
	top_level_statement_zom
	;

top_level_statement_zom :
	/*empty*/
	| top_level_statement
	| top_level_statement_zom top_level_statement
	;

top_level_statement :
	global_code
	| production
	| include_statement
	;

include_statement:
	INCLUDE regex
	;

global_code :
	'%<' balanced_code_zom '%>'
	| curly_code
	| '${scanner' balanced_code_oom '}'
	| '${declare' declarationtype identifier_zom '}'
	| '${token' token_identifier_oom '}'
	;

pass_types :
	/*empty*/
	| pass_type pass_types
	;

pass_type :
	'preorder'
	| 'postorder'
	| 'manual'
	| 'for_all'
	| 'for_undefined'
	;

declarationtype :
	'tokenize'
	| 'longest_match'
	| 'whitespace'
	| 'all_matches'
	| 'set_op_priority_from_rule'
	| 'all_subparsers'
	| 'subparser'
	| 'save_parse_tree'
	;

token_identifier_oom :
	token_identifier
	| token_identifier_oom token_identifier
	;

token_identifier :
	identifier
	;

production :
	production_name ':' rules ';'
	| production_name regex_production rules ';'
	| ';'
	;

regex_production :
	'::='
	;

production_name :
	identifier
	| '_'
	;

rules :
	rule rule_alt_zom
	;

rule_alt :
	'|' rule
	;

rule_alt_zom :
	/*empty*/
	| rule_alt
	| rule_alt_zom rule_alt
	;

rule :
	new_rule element_and_modifier_simple_opt rule_modifier_zom rule_code
	;

element_and_modifier_simple_opt :
	/*empty*/
	| element_and_modifier_zom simple_element element_modifier_zom
	;

element_and_modifier_zom :
	/*empty*/
	| element element_modifier_zom
	| element_and_modifier_zom element element_modifier_zom
	;

new_rule :
	/*empty*/
	;

simple_element :
	string
	| regex
	| unicode_char
	| identifier
	| '${scan' balanced_code_oom '}'
	| '(' new_subrule rules ')'
	;

element :
	simple_element
	| bracket_code
	| curly_code
	;

new_subrule :
	/*empty*/
	;

element_modifier_zom :
	/*empty*/
	| element_modifier
	| element_modifier_zom element_modifier
	;

element_modifier :
	'$term' integer
	| '$name' string
	| '$name' regex
	| '/i'
	| '?'
	| '*'
	| '+'
	| '@' integer
	| '@' integer ':' integer
	;

rule_modifier_zom :
	/*empty*/
	| rule_modifier
	| rule_modifier_zom rule_modifier
	;

rule_modifier :
	rule_assoc rule_priority
	| external_action
	;

rule_assoc :
	'$unary_op_right'
	| '$unary_op_left'
	| '$binary_op_right'
	| '$binary_op_left'
	| '$unary_right'
	| '$unary_left'
	| '$binary_right'
	| '$binary_left'
	| '$right'
	| '$left'
	;

rule_priority :
	integer
	;

external_action :
	'${action}'
	| '${action' integer '}'
	;

rule_code :
	speculative_code_opt final_code_opt pass_code_zom
	;


speculative_code_opt :
	/*empty*/
	| speculative_code
	;

speculative_code :
	bracket_code
	;

final_code_opt :
	/*empty*/
	| final_code
	;

final_code :
	curly_code
	;

pass_code_zom :
	/*empty*/
	| pass_code
	| pass_code_zom pass_code
	;

pass_code :
	identifier ':' curly_code
	;

curly_code :
	'{' balanced_code_zom '}'
	;

bracket_code :
	'[' balanced_code_zom ']'
	;

balanced_code_oom :
	balanced_code
	| balanced_code_oom balanced_code
	;

balanced_code_zom :
	/*empty*/
	| balanced_code_oom
	;

balanced_code :
	'(' balanced_code_zom ')'
	| '[' balanced_code_zom ']'
	| '{' balanced_code_zom '}'
	| string
	| identifier
	| regex
	| integer
	| symbols
	;

symbols :
	"[!~`@#$%^&*\-_+=|:;\\<,>.?/]"
	;

string :
	"'([^'\\]|\\[^])*'"
	;

regex :
	"\"([^\"\\]|\\[^])*\""
	;

unicode_char :
	"[uU]\+[0-9a-fA-F]+"
	;

INCLUDE : 'include' ;

identifier_zom :
	/*empty*/
	| identifier
	| identifier_zom identifier
	;

identifier :
	"[a-zA-Z_][a-zA-Z_0-9]*" //$term -1
	;

integer :
	decimalint
	| hexint
	| octalint
	;

decimalint :
	"0|-?[1-9][0-9]*[uUlL]?"
	;

hexint :
	"-?(0x|0X)[0-9a-fA-F]+[uUlL]?"
	;

octalint :
	"-?0[0-7]*[uUlL]?"
	;

}
