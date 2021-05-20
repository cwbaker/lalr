error_handling_calculator {
    %whitespace "[ \t\r\n]*";
    %none error;
    %left '(' ')';
    %left '+' '-';
    %left '*' '/';
    %none integer;
    stmts: stmts stmt | stmt | %precedence '(';
    stmt: 
        expr ';' [result] | 
        error ';' [unexpected_error]
    ;
    expr:
        expr '+' expr [add] |
        expr '-' expr [subtract] |
        expr '*' expr [multiply] |
        expr '/' expr [divide] |
        expr error expr [unknown_operator_error] |
        '(' expr ')' [compound] |
        integer [integer]
    ;
    integer: "[0-9]+";
}
