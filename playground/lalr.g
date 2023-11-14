lalr {
    //%error_recovery_off;
    //%error_recovery_show;
	%whitespace "[ \t\r\n]*";
	%whitespace "//[^\n]*";
	//%whitespace "/\*[^*]+\*/";
	%whitespace "/\\*:C_MultilineComment:";

	match_grammar:
		match_identifier '{'  match_statements '}'
		;

	match_identifier:
		"[a-zA-Z_][a-zA-Z_0-9]*"
		;

	match_statements:
		match_statement
		| match_statements match_statement
		;

	match_statement:
		match_associativity_statement
		| match_whitespace_statement
		| match_case_insensitive_statement
		| match_error_recovery_statement
		| match_production_statement
		;

	match_associativity_statement:
		match_associativity match_symbols ';'
		;

	match_whitespace_statement:
		'%whitespace' match_regex ';'
		;

	match_case_insensitive_statement:
		'%case_insensitive' ';'
		;

	match_error_recovery_statement:
	    '%error_recovery_off' ';'
	    |'%error_recovery_show' ';'
	    ;

	match_production_statement:
		match_identifier ':' match_expressions ';'
		;

	match_associativity:
		'%left'
		| '%right'
		| none_assoc
		| '%precedence'
		;

	none_assoc:
		'%none'
		| '%nonassoc'
		;

	match_symbols:
		match_symbol
		| match_symbols match_symbol
		;

	match_symbol:
		match_error
		| match_literal
		| match_regex
		| match_identifier
		;

	match_error:
		'error'
		;

	match_literal:
		"'(\\.|[^'\n])+'"
		;

	match_regex:
		"\"(\\.|[^\"\n])+\""
		;

	match_expressions:
	    match_expression
		| match_expressions match_expression_or
		;

	match_expression_or:
		'|' match_expression
		;

	match_expression:
		match_symbols_opt match_precedence match_action
		;

	match_symbols_opt:
	    /*empty*/
	    | match_symbols
	    ;

	match_precedence:
		/*empty*/
		| prec_tag match_symbol
		;

	prec_tag:
		'%precedence'
		| '%prec'
		;

	match_action:
		/*empty*/
		| '[' match_identifier ']'
		;
}
