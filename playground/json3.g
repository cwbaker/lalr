// /usr/bin/time /home/mingo/dev/c/A_grammars/lalr/test-grammar-nb/dist/Release/GNU-Linux/test-grammar-nb -g json2.g -i BytecodeList.json
// /usr/bin/time /home/mingo/dev/c/A_grammars/lalr/test-grammar-nb/dist/Release/GNU-Linux/test-grammar-nb -g json2.g -i /home/mingo/dev/c/A_grammars/tree-sitter/tree-sitter-dart/src/grammar.json
json {

%whitespace "[ \t\r\n]*";
%whitespace "//[^\n]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

   document: object | array [document]
	;

   value:
	'null' [value]
	| 'true' [value]
	| 'false' [value]
	| integer [value]
	| real [value]
	| string [value]
	| array [value]
	| object [value]
	;

   object:
	'{' kv_list '}' [object]
	;

   array:
	'[' value_list ']' [array]
	;

   value_list :
	/*empty*/
	| value
	| value_list ',' value
	;

   kv_list:
	/*empty*/
	| key_val
	| kv_list ',' key_val
	;

   key_val:
	string ':' value [key_val]
	;

   integer:
	"(\+|\-)?[0-9]+"
	;

   real:
	"(\+|\-)?[0-9]+(\.[0-9]+)?((e|E)(\+|\-)?[0-9]+)?"
	;

   string:
	//"[\"](\\[\"\\\\]|[^\"\n])*[\"]"
	//| "['](\\['\\\\]|[^'\n])*[']"
	"\"(\\[\"\\]|[^\"\n])*\"|'(\\['\\]|[^'\n])*'"
	;
}
