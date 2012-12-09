
enum jsonSymbol
{
    json_SYMBOL_NULL,
    json_SYMBOL_dot_start, 
    json_SYMBOL_dot_end, 
    json_SYMBOL_dot_error_terminal, 
    json_SYMBOL_document, 
    json_SYMBOL_left_curly_brace_terminal, 
    json_SYMBOL_element, 
    json_SYMBOL_right_curly_brace_terminal, 
    json_SYMBOL_colon_terminal, 
    json_SYMBOL_contents, 
    json_SYMBOL_comma_terminal, 
    json_SYMBOL_content, 
    json_SYMBOL_attribute, 
    json_SYMBOL_value, 
    json_SYMBOL_boolean, 
    json_SYMBOL_string, 
    json_SYMBOL_null, 
    json_SYMBOL_true_terminal, 
    json_SYMBOL_false_terminal, 
    json_SYMBOL_integer, 
    json_SYMBOL_real, 
    json_SYMBOL_COUNT
};


const sweet::parser::LexerAction json_lexer_actions[] = 
{
    { 0, "string" },
    { 0, NULL }
};


extern const sweet::parser::LexerState json_lexer_state_machine_state_0;
extern const sweet::parser::LexerState json_lexer_state_machine_state_1;
extern const sweet::parser::LexerState json_lexer_state_machine_state_2;
extern const sweet::parser::LexerState json_lexer_state_machine_state_3;
extern const sweet::parser::LexerState json_lexer_state_machine_state_4;
extern const sweet::parser::LexerState json_lexer_state_machine_state_5;
extern const sweet::parser::LexerState json_lexer_state_machine_state_6;
extern const sweet::parser::LexerState json_lexer_state_machine_state_7;
extern const sweet::parser::LexerState json_lexer_state_machine_state_8;
extern const sweet::parser::LexerState json_lexer_state_machine_state_9;
extern const sweet::parser::LexerState json_lexer_state_machine_state_10;
extern const sweet::parser::LexerState json_lexer_state_machine_state_11;
extern const sweet::parser::LexerState json_lexer_state_machine_state_12;
extern const sweet::parser::LexerState json_lexer_state_machine_state_13;
extern const sweet::parser::LexerState json_lexer_state_machine_state_14;
extern const sweet::parser::LexerState json_lexer_state_machine_state_15;
extern const sweet::parser::LexerState json_lexer_state_machine_state_16;
extern const sweet::parser::LexerState json_lexer_state_machine_state_17;
extern const sweet::parser::LexerState json_lexer_state_machine_state_18;
extern const sweet::parser::LexerState json_lexer_state_machine_state_19;
extern const sweet::parser::LexerState json_lexer_state_machine_state_20;
extern const sweet::parser::LexerState json_lexer_state_machine_state_21;
extern const sweet::parser::LexerState json_lexer_state_machine_state_22;
extern const sweet::parser::LexerState json_lexer_state_machine_state_23;
extern const sweet::parser::LexerState json_lexer_state_machine_state_24;
extern const sweet::parser::LexerState json_lexer_state_machine_state_25;
extern const sweet::parser::LexerState json_lexer_state_machine_state_26;


const sweet::parser::LexerTransition json_lexer_state_machine_state_0_transitions[] =
{
    { 34, 35, &json_lexer_state_machine_state_5, NULL }, 
    { 39, 40, &json_lexer_state_machine_state_5, NULL }, 
    { 43, 44, &json_lexer_state_machine_state_21, NULL }, 
    { 44, 45, &json_lexer_state_machine_state_4, NULL }, 
    { 45, 46, &json_lexer_state_machine_state_21, NULL }, 
    { 48, 58, &json_lexer_state_machine_state_20, NULL }, 
    { 58, 59, &json_lexer_state_machine_state_3, NULL }, 
    { 102, 103, &json_lexer_state_machine_state_15, NULL }, 
    { 110, 111, &json_lexer_state_machine_state_7, NULL }, 
    { 116, 117, &json_lexer_state_machine_state_11, NULL }, 
    { 123, 124, &json_lexer_state_machine_state_1, NULL }, 
    { 125, 126, &json_lexer_state_machine_state_2, NULL }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_0 =
{
    json_SYMBOL_NULL,
    json_lexer_state_machine_state_0_transitions,
    json_lexer_state_machine_state_0_transitions + 12
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_1_transitions[] =
{
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_1 =
{
    json_SYMBOL_left_curly_brace_terminal,
    json_lexer_state_machine_state_1_transitions,
    json_lexer_state_machine_state_1_transitions + 0
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_2_transitions[] =
{
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_2 =
{
    json_SYMBOL_right_curly_brace_terminal,
    json_lexer_state_machine_state_2_transitions,
    json_lexer_state_machine_state_2_transitions + 0
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_3_transitions[] =
{
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_3 =
{
    json_SYMBOL_colon_terminal,
    json_lexer_state_machine_state_3_transitions,
    json_lexer_state_machine_state_3_transitions + 0
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_4_transitions[] =
{
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_4 =
{
    json_SYMBOL_comma_terminal,
    json_lexer_state_machine_state_4_transitions,
    json_lexer_state_machine_state_4_transitions + 0
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_5_transitions[] =
{
    { 0, 2147483647, &json_lexer_state_machine_state_6, &json_lexer_actions[0] }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_5 =
{
    json_SYMBOL_NULL,
    json_lexer_state_machine_state_5_transitions,
    json_lexer_state_machine_state_5_transitions + 1
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_6_transitions[] =
{
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_6 =
{
    json_SYMBOL_string,
    json_lexer_state_machine_state_6_transitions,
    json_lexer_state_machine_state_6_transitions + 0
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_7_transitions[] =
{
    { 117, 118, &json_lexer_state_machine_state_8, NULL }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_7 =
{
    json_SYMBOL_NULL,
    json_lexer_state_machine_state_7_transitions,
    json_lexer_state_machine_state_7_transitions + 1
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_8_transitions[] =
{
    { 108, 109, &json_lexer_state_machine_state_9, NULL }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_8 =
{
    json_SYMBOL_NULL,
    json_lexer_state_machine_state_8_transitions,
    json_lexer_state_machine_state_8_transitions + 1
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_9_transitions[] =
{
    { 108, 109, &json_lexer_state_machine_state_10, NULL }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_9 =
{
    json_SYMBOL_NULL,
    json_lexer_state_machine_state_9_transitions,
    json_lexer_state_machine_state_9_transitions + 1
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_10_transitions[] =
{
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_10 =
{
    json_SYMBOL_null,
    json_lexer_state_machine_state_10_transitions,
    json_lexer_state_machine_state_10_transitions + 0
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_11_transitions[] =
{
    { 114, 115, &json_lexer_state_machine_state_12, NULL }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_11 =
{
    json_SYMBOL_NULL,
    json_lexer_state_machine_state_11_transitions,
    json_lexer_state_machine_state_11_transitions + 1
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_12_transitions[] =
{
    { 117, 118, &json_lexer_state_machine_state_13, NULL }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_12 =
{
    json_SYMBOL_NULL,
    json_lexer_state_machine_state_12_transitions,
    json_lexer_state_machine_state_12_transitions + 1
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_13_transitions[] =
{
    { 101, 102, &json_lexer_state_machine_state_14, NULL }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_13 =
{
    json_SYMBOL_NULL,
    json_lexer_state_machine_state_13_transitions,
    json_lexer_state_machine_state_13_transitions + 1
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_14_transitions[] =
{
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_14 =
{
    json_SYMBOL_true_terminal,
    json_lexer_state_machine_state_14_transitions,
    json_lexer_state_machine_state_14_transitions + 0
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_15_transitions[] =
{
    { 97, 98, &json_lexer_state_machine_state_16, NULL }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_15 =
{
    json_SYMBOL_NULL,
    json_lexer_state_machine_state_15_transitions,
    json_lexer_state_machine_state_15_transitions + 1
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_16_transitions[] =
{
    { 108, 109, &json_lexer_state_machine_state_17, NULL }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_16 =
{
    json_SYMBOL_NULL,
    json_lexer_state_machine_state_16_transitions,
    json_lexer_state_machine_state_16_transitions + 1
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_17_transitions[] =
{
    { 115, 116, &json_lexer_state_machine_state_18, NULL }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_17 =
{
    json_SYMBOL_NULL,
    json_lexer_state_machine_state_17_transitions,
    json_lexer_state_machine_state_17_transitions + 1
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_18_transitions[] =
{
    { 101, 102, &json_lexer_state_machine_state_19, NULL }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_18 =
{
    json_SYMBOL_NULL,
    json_lexer_state_machine_state_18_transitions,
    json_lexer_state_machine_state_18_transitions + 1
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_19_transitions[] =
{
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_19 =
{
    json_SYMBOL_false_terminal,
    json_lexer_state_machine_state_19_transitions,
    json_lexer_state_machine_state_19_transitions + 0
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_20_transitions[] =
{
    { 46, 47, &json_lexer_state_machine_state_22, NULL }, 
    { 48, 58, &json_lexer_state_machine_state_20, NULL }, 
    { 69, 70, &json_lexer_state_machine_state_24, NULL }, 
    { 101, 102, &json_lexer_state_machine_state_24, NULL }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_20 =
{
    json_SYMBOL_integer,
    json_lexer_state_machine_state_20_transitions,
    json_lexer_state_machine_state_20_transitions + 4
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_21_transitions[] =
{
    { 48, 58, &json_lexer_state_machine_state_20, NULL }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_21 =
{
    json_SYMBOL_NULL,
    json_lexer_state_machine_state_21_transitions,
    json_lexer_state_machine_state_21_transitions + 1
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_22_transitions[] =
{
    { 48, 58, &json_lexer_state_machine_state_23, NULL }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_22 =
{
    json_SYMBOL_NULL,
    json_lexer_state_machine_state_22_transitions,
    json_lexer_state_machine_state_22_transitions + 1
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_23_transitions[] =
{
    { 48, 58, &json_lexer_state_machine_state_23, NULL }, 
    { 69, 70, &json_lexer_state_machine_state_24, NULL }, 
    { 101, 102, &json_lexer_state_machine_state_24, NULL }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_23 =
{
    json_SYMBOL_real,
    json_lexer_state_machine_state_23_transitions,
    json_lexer_state_machine_state_23_transitions + 3
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_24_transitions[] =
{
    { 43, 44, &json_lexer_state_machine_state_25, NULL }, 
    { 45, 46, &json_lexer_state_machine_state_25, NULL }, 
    { 48, 58, &json_lexer_state_machine_state_26, NULL }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_24 =
{
    json_SYMBOL_NULL,
    json_lexer_state_machine_state_24_transitions,
    json_lexer_state_machine_state_24_transitions + 3
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_25_transitions[] =
{
    { 48, 58, &json_lexer_state_machine_state_26, NULL }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_25 =
{
    json_SYMBOL_NULL,
    json_lexer_state_machine_state_25_transitions,
    json_lexer_state_machine_state_25_transitions + 1
};


const sweet::parser::LexerTransition json_lexer_state_machine_state_26_transitions[] =
{
    { 48, 58, &json_lexer_state_machine_state_26, NULL }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_26 =
{
    json_SYMBOL_real,
    json_lexer_state_machine_state_26_transitions,
    json_lexer_state_machine_state_26_transitions + 1
};


extern const sweet::parser::LexerState json_lexer_state_machine_state_27;


const sweet::parser::LexerTransition json_lexer_state_machine_state_27_transitions[] =
{
    { 9, 11, &json_lexer_state_machine_state_27, NULL }, 
    { 13, 14, &json_lexer_state_machine_state_27, NULL }, 
    { 32, 33, &json_lexer_state_machine_state_27, NULL }, 
    { 0, 0, NULL }
};


const sweet::parser::LexerState json_lexer_state_machine_state_27 =
{
    json_SYMBOL_NULL,
    json_lexer_state_machine_state_27_transitions,
    json_lexer_state_machine_state_27_transitions + 3
};


const sweet::parser::LexerStateMachine json_lexer_state_machine = 
{
    &json_lexer_state_machine_state_0, 
    &json_lexer_state_machine_state_27, 
    json_lexer_actions,
    json_lexer_actions + 1
};


const sweet::parser::ParserAction json_parser_actions[] = 
{
    { 0, "document" },
    { 1, "element" },
    { 2, "add_to_element" },
    { 3, "create_element" },
    { 4, "content" },
    { 5, "attribute" },
    { 6, "value" },
    { 0, NULL }
};


const sweet::parser::ParserProduction json_parser_production_0 =
{
    json_SYMBOL_dot_start, 
    NULL, 
    1
};


const sweet::parser::ParserProduction json_parser_production_1 =
{
    json_SYMBOL_document, 
    json_parser_actions + 0, 
    3
};


const sweet::parser::ParserProduction json_parser_production_2 =
{
    json_SYMBOL_element, 
    json_parser_actions + 1, 
    5
};


const sweet::parser::ParserProduction json_parser_production_3 =
{
    json_SYMBOL_contents, 
    json_parser_actions + 2, 
    3
};


const sweet::parser::ParserProduction json_parser_production_4 =
{
    json_SYMBOL_contents, 
    json_parser_actions + 3, 
    1
};


const sweet::parser::ParserProduction json_parser_production_5 =
{
    json_SYMBOL_content, 
    json_parser_actions + 4, 
    1
};


const sweet::parser::ParserProduction json_parser_production_6 =
{
    json_SYMBOL_content, 
    json_parser_actions + 4, 
    1
};


const sweet::parser::ParserProduction json_parser_production_7 =
{
    json_SYMBOL_attribute, 
    json_parser_actions + 5, 
    3
};


const sweet::parser::ParserProduction json_parser_production_8 =
{
    json_SYMBOL_value, 
    json_parser_actions + 6, 
    1
};


const sweet::parser::ParserProduction json_parser_production_9 =
{
    json_SYMBOL_value, 
    json_parser_actions + 6, 
    1
};


const sweet::parser::ParserProduction json_parser_production_10 =
{
    json_SYMBOL_value, 
    json_parser_actions + 6, 
    1
};


const sweet::parser::ParserProduction json_parser_production_11 =
{
    json_SYMBOL_value, 
    json_parser_actions + 6, 
    1
};


const sweet::parser::ParserProduction json_parser_production_12 =
{
    json_SYMBOL_value, 
    json_parser_actions + 6, 
    1
};


const sweet::parser::ParserProduction json_parser_production_13 =
{
    json_SYMBOL_string, 
    NULL, 
    1
};


const sweet::parser::ParserProduction json_parser_production_14 =
{
    json_SYMBOL_null, 
    NULL, 
    1
};


const sweet::parser::ParserProduction json_parser_production_15 =
{
    json_SYMBOL_boolean, 
    NULL, 
    1
};


const sweet::parser::ParserProduction json_parser_production_16 =
{
    json_SYMBOL_boolean, 
    NULL, 
    1
};


const sweet::parser::ParserProduction json_parser_production_17 =
{
    json_SYMBOL_integer, 
    NULL, 
    1
};


const sweet::parser::ParserProduction json_parser_production_18 =
{
    json_SYMBOL_real, 
    NULL, 
    1
};


const sweet::parser::ParserProduction json_parser_production_19 =
{
    json_SYMBOL_string, 
    NULL, 
    1
};


extern const sweet::parser::ParserState json_parser_state_0;
extern const sweet::parser::ParserState json_parser_state_1;
extern const sweet::parser::ParserState json_parser_state_2;
extern const sweet::parser::ParserState json_parser_state_3;
extern const sweet::parser::ParserState json_parser_state_4;
extern const sweet::parser::ParserState json_parser_state_5;
extern const sweet::parser::ParserState json_parser_state_6;
extern const sweet::parser::ParserState json_parser_state_7;
extern const sweet::parser::ParserState json_parser_state_8;
extern const sweet::parser::ParserState json_parser_state_9;
extern const sweet::parser::ParserState json_parser_state_10;
extern const sweet::parser::ParserState json_parser_state_11;
extern const sweet::parser::ParserState json_parser_state_12;
extern const sweet::parser::ParserState json_parser_state_13;
extern const sweet::parser::ParserState json_parser_state_14;
extern const sweet::parser::ParserState json_parser_state_15;
extern const sweet::parser::ParserState json_parser_state_16;
extern const sweet::parser::ParserState json_parser_state_17;
extern const sweet::parser::ParserState json_parser_state_18;
extern const sweet::parser::ParserState json_parser_state_19;
extern const sweet::parser::ParserState json_parser_state_20;
extern const sweet::parser::ParserState json_parser_state_21;
extern const sweet::parser::ParserState json_parser_state_22;
extern const sweet::parser::ParserState json_parser_state_23;
extern const sweet::parser::ParserState json_parser_state_24;

/*
0:
. left_curly_brace_terminal element right_curly_brace_terminal ; dot_end
. document ; dot_end
shift to 4 on document ; 
shift to 1 on left_curly_brace_terminal ; 

*/

const sweet::parser::ParserTransition json_parser_state_0_transitions[] = 
{
    { json_SYMBOL_document, sweet::parser::TRANSITION_SHIFT, &json_parser_state_4, NULL },
    { json_SYMBOL_left_curly_brace_terminal, sweet::parser::TRANSITION_SHIFT, &json_parser_state_1, NULL },
};


const sweet::parser::ParserState json_parser_state_0 =
{
    json_parser_state_0_transitions,
    json_parser_state_0_transitions + 2
};

/*
1:
left_curly_brace_terminal . element right_curly_brace_terminal ; dot_end
. string colon_terminal left_curly_brace_terminal contents right_curly_brace_terminal ; right_curly_brace_terminal
shift to 2 on element ; 
shift to 7 on string ; 

*/

const sweet::parser::ParserTransition json_parser_state_1_transitions[] = 
{
    { json_SYMBOL_element, sweet::parser::TRANSITION_SHIFT, &json_parser_state_2, NULL },
    { json_SYMBOL_string, sweet::parser::TRANSITION_SHIFT, &json_parser_state_7, NULL },
};


const sweet::parser::ParserState json_parser_state_1 =
{
    json_parser_state_1_transitions,
    json_parser_state_1_transitions + 2
};

/*
2:
left_curly_brace_terminal element . right_curly_brace_terminal ; dot_end
shift to 3 on right_curly_brace_terminal ; 

*/

const sweet::parser::ParserTransition json_parser_state_2_transitions[] = 
{
    { json_SYMBOL_right_curly_brace_terminal, sweet::parser::TRANSITION_SHIFT, &json_parser_state_3, NULL },
};


const sweet::parser::ParserState json_parser_state_2 =
{
    json_parser_state_2_transitions,
    json_parser_state_2_transitions + 1
};

/*
3:
left_curly_brace_terminal element right_curly_brace_terminal . ; dot_end
reduce to document on dot_end ; 

*/

const sweet::parser::ParserTransition json_parser_state_3_transitions[] = 
{
    { json_SYMBOL_dot_end, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_1 },
};


const sweet::parser::ParserState json_parser_state_3 =
{
    json_parser_state_3_transitions,
    json_parser_state_3_transitions + 1
};

/*
4:
document . ; dot_end
reduce to dot_start on dot_end ; 

*/

const sweet::parser::ParserTransition json_parser_state_4_transitions[] = 
{
    { json_SYMBOL_dot_end, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_0 },
};


const sweet::parser::ParserState json_parser_state_4 =
{
    json_parser_state_4_transitions,
    json_parser_state_4_transitions + 1
};

/*
5:
. string colon_terminal left_curly_brace_terminal contents right_curly_brace_terminal ; comma_terminal, right_curly_brace_terminal
string colon_terminal left_curly_brace_terminal . contents right_curly_brace_terminal ; comma_terminal, right_curly_brace_terminal
. contents comma_terminal content ; comma_terminal, right_curly_brace_terminal
. content ; comma_terminal, right_curly_brace_terminal
. attribute ; comma_terminal, right_curly_brace_terminal
. element ; comma_terminal, right_curly_brace_terminal
. string colon_terminal value ; comma_terminal, right_curly_brace_terminal
shift to 11 on contents ; 
shift to 14 on content ; 
shift to 15 on attribute ; 
shift to 16 on element ; 
shift to 8 on string ; 

*/

const sweet::parser::ParserTransition json_parser_state_5_transitions[] = 
{
    { json_SYMBOL_contents, sweet::parser::TRANSITION_SHIFT, &json_parser_state_11, NULL },
    { json_SYMBOL_content, sweet::parser::TRANSITION_SHIFT, &json_parser_state_14, NULL },
    { json_SYMBOL_attribute, sweet::parser::TRANSITION_SHIFT, &json_parser_state_15, NULL },
    { json_SYMBOL_element, sweet::parser::TRANSITION_SHIFT, &json_parser_state_16, NULL },
    { json_SYMBOL_string, sweet::parser::TRANSITION_SHIFT, &json_parser_state_8, NULL },
};


const sweet::parser::ParserState json_parser_state_5 =
{
    json_parser_state_5_transitions,
    json_parser_state_5_transitions + 5
};

/*
6:
. string colon_terminal left_curly_brace_terminal contents right_curly_brace_terminal ; comma_terminal, right_curly_brace_terminal
contents comma_terminal . content ; comma_terminal, right_curly_brace_terminal
. attribute ; comma_terminal, right_curly_brace_terminal
. element ; comma_terminal, right_curly_brace_terminal
. string colon_terminal value ; comma_terminal, right_curly_brace_terminal
shift to 13 on content ; 
shift to 15 on attribute ; 
shift to 16 on element ; 
shift to 8 on string ; 

*/

const sweet::parser::ParserTransition json_parser_state_6_transitions[] = 
{
    { json_SYMBOL_content, sweet::parser::TRANSITION_SHIFT, &json_parser_state_13, NULL },
    { json_SYMBOL_attribute, sweet::parser::TRANSITION_SHIFT, &json_parser_state_15, NULL },
    { json_SYMBOL_element, sweet::parser::TRANSITION_SHIFT, &json_parser_state_16, NULL },
    { json_SYMBOL_string, sweet::parser::TRANSITION_SHIFT, &json_parser_state_8, NULL },
};


const sweet::parser::ParserState json_parser_state_6 =
{
    json_parser_state_6_transitions,
    json_parser_state_6_transitions + 4
};

/*
7:
string . colon_terminal left_curly_brace_terminal contents right_curly_brace_terminal ; right_curly_brace_terminal
shift to 9 on colon_terminal ; 

*/

const sweet::parser::ParserTransition json_parser_state_7_transitions[] = 
{
    { json_SYMBOL_colon_terminal, sweet::parser::TRANSITION_SHIFT, &json_parser_state_9, NULL },
};


const sweet::parser::ParserState json_parser_state_7 =
{
    json_parser_state_7_transitions,
    json_parser_state_7_transitions + 1
};

/*
8:
string . colon_terminal left_curly_brace_terminal contents right_curly_brace_terminal ; comma_terminal, right_curly_brace_terminal
string . colon_terminal value ; comma_terminal, right_curly_brace_terminal
shift to 10 on colon_terminal ; 

*/

const sweet::parser::ParserTransition json_parser_state_8_transitions[] = 
{
    { json_SYMBOL_colon_terminal, sweet::parser::TRANSITION_SHIFT, &json_parser_state_10, NULL },
};


const sweet::parser::ParserState json_parser_state_8 =
{
    json_parser_state_8_transitions,
    json_parser_state_8_transitions + 1
};

/*
9:
string colon_terminal . left_curly_brace_terminal contents right_curly_brace_terminal ; right_curly_brace_terminal
shift to 5 on left_curly_brace_terminal ; 

*/

const sweet::parser::ParserTransition json_parser_state_9_transitions[] = 
{
    { json_SYMBOL_left_curly_brace_terminal, sweet::parser::TRANSITION_SHIFT, &json_parser_state_5, NULL },
};


const sweet::parser::ParserState json_parser_state_9 =
{
    json_parser_state_9_transitions,
    json_parser_state_9_transitions + 1
};

/*
10:
string colon_terminal . left_curly_brace_terminal contents right_curly_brace_terminal ; comma_terminal, right_curly_brace_terminal
string colon_terminal . value ; comma_terminal, right_curly_brace_terminal
. null ; comma_terminal, right_curly_brace_terminal
. boolean ; comma_terminal, right_curly_brace_terminal
. integer ; comma_terminal, right_curly_brace_terminal
. real ; comma_terminal, right_curly_brace_terminal
. string ; comma_terminal, right_curly_brace_terminal
. true_terminal ; comma_terminal, right_curly_brace_terminal
. false_terminal ; comma_terminal, right_curly_brace_terminal
shift to 5 on left_curly_brace_terminal ; 
shift to 17 on value ; 
shift to 19 on boolean ; 
shift to 22 on string ; 
shift to 18 on null ; 
shift to 23 on true_terminal ; 
shift to 24 on false_terminal ; 
shift to 20 on integer ; 
shift to 21 on real ; 

*/

const sweet::parser::ParserTransition json_parser_state_10_transitions[] = 
{
    { json_SYMBOL_left_curly_brace_terminal, sweet::parser::TRANSITION_SHIFT, &json_parser_state_5, NULL },
    { json_SYMBOL_value, sweet::parser::TRANSITION_SHIFT, &json_parser_state_17, NULL },
    { json_SYMBOL_boolean, sweet::parser::TRANSITION_SHIFT, &json_parser_state_19, NULL },
    { json_SYMBOL_string, sweet::parser::TRANSITION_SHIFT, &json_parser_state_22, NULL },
    { json_SYMBOL_null, sweet::parser::TRANSITION_SHIFT, &json_parser_state_18, NULL },
    { json_SYMBOL_true_terminal, sweet::parser::TRANSITION_SHIFT, &json_parser_state_23, NULL },
    { json_SYMBOL_false_terminal, sweet::parser::TRANSITION_SHIFT, &json_parser_state_24, NULL },
    { json_SYMBOL_integer, sweet::parser::TRANSITION_SHIFT, &json_parser_state_20, NULL },
    { json_SYMBOL_real, sweet::parser::TRANSITION_SHIFT, &json_parser_state_21, NULL },
};


const sweet::parser::ParserState json_parser_state_10 =
{
    json_parser_state_10_transitions,
    json_parser_state_10_transitions + 9
};

/*
11:
string colon_terminal left_curly_brace_terminal contents . right_curly_brace_terminal ; comma_terminal, right_curly_brace_terminal
contents . comma_terminal content ; comma_terminal, right_curly_brace_terminal
shift to 6 on comma_terminal ; 
shift to 12 on right_curly_brace_terminal ; 

*/

const sweet::parser::ParserTransition json_parser_state_11_transitions[] = 
{
    { json_SYMBOL_comma_terminal, sweet::parser::TRANSITION_SHIFT, &json_parser_state_6, NULL },
    { json_SYMBOL_right_curly_brace_terminal, sweet::parser::TRANSITION_SHIFT, &json_parser_state_12, NULL },
};


const sweet::parser::ParserState json_parser_state_11 =
{
    json_parser_state_11_transitions,
    json_parser_state_11_transitions + 2
};

/*
12:
string colon_terminal left_curly_brace_terminal contents right_curly_brace_terminal . ; comma_terminal, right_curly_brace_terminal
reduce to element on comma_terminal ; 
reduce to element on right_curly_brace_terminal ; 

*/

const sweet::parser::ParserTransition json_parser_state_12_transitions[] = 
{
    { json_SYMBOL_comma_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_2 },
    { json_SYMBOL_right_curly_brace_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_2 },
};


const sweet::parser::ParserState json_parser_state_12 =
{
    json_parser_state_12_transitions,
    json_parser_state_12_transitions + 2
};

/*
13:
contents comma_terminal content . ; comma_terminal, right_curly_brace_terminal
reduce to contents on comma_terminal ; 
reduce to contents on right_curly_brace_terminal ; 

*/

const sweet::parser::ParserTransition json_parser_state_13_transitions[] = 
{
    { json_SYMBOL_comma_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_3 },
    { json_SYMBOL_right_curly_brace_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_3 },
};


const sweet::parser::ParserState json_parser_state_13 =
{
    json_parser_state_13_transitions,
    json_parser_state_13_transitions + 2
};

/*
14:
content . ; comma_terminal, right_curly_brace_terminal
reduce to contents on comma_terminal ; 
reduce to contents on right_curly_brace_terminal ; 

*/

const sweet::parser::ParserTransition json_parser_state_14_transitions[] = 
{
    { json_SYMBOL_comma_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_4 },
    { json_SYMBOL_right_curly_brace_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_4 },
};


const sweet::parser::ParserState json_parser_state_14 =
{
    json_parser_state_14_transitions,
    json_parser_state_14_transitions + 2
};

/*
15:
attribute . ; comma_terminal, right_curly_brace_terminal
reduce to content on comma_terminal ; 
reduce to content on right_curly_brace_terminal ; 

*/

const sweet::parser::ParserTransition json_parser_state_15_transitions[] = 
{
    { json_SYMBOL_comma_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_5 },
    { json_SYMBOL_right_curly_brace_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_5 },
};


const sweet::parser::ParserState json_parser_state_15 =
{
    json_parser_state_15_transitions,
    json_parser_state_15_transitions + 2
};

/*
16:
element . ; comma_terminal, right_curly_brace_terminal
reduce to content on comma_terminal ; 
reduce to content on right_curly_brace_terminal ; 

*/

const sweet::parser::ParserTransition json_parser_state_16_transitions[] = 
{
    { json_SYMBOL_comma_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_6 },
    { json_SYMBOL_right_curly_brace_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_6 },
};


const sweet::parser::ParserState json_parser_state_16 =
{
    json_parser_state_16_transitions,
    json_parser_state_16_transitions + 2
};

/*
17:
string colon_terminal value . ; comma_terminal, right_curly_brace_terminal
reduce to attribute on comma_terminal ; 
reduce to attribute on right_curly_brace_terminal ; 

*/

const sweet::parser::ParserTransition json_parser_state_17_transitions[] = 
{
    { json_SYMBOL_comma_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_7 },
    { json_SYMBOL_right_curly_brace_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_7 },
};


const sweet::parser::ParserState json_parser_state_17 =
{
    json_parser_state_17_transitions,
    json_parser_state_17_transitions + 2
};

/*
18:
null . ; comma_terminal, right_curly_brace_terminal
reduce to value on comma_terminal ; 
reduce to value on right_curly_brace_terminal ; 

*/

const sweet::parser::ParserTransition json_parser_state_18_transitions[] = 
{
    { json_SYMBOL_comma_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_8 },
    { json_SYMBOL_right_curly_brace_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_8 },
};


const sweet::parser::ParserState json_parser_state_18 =
{
    json_parser_state_18_transitions,
    json_parser_state_18_transitions + 2
};

/*
19:
boolean . ; comma_terminal, right_curly_brace_terminal
reduce to value on comma_terminal ; 
reduce to value on right_curly_brace_terminal ; 

*/

const sweet::parser::ParserTransition json_parser_state_19_transitions[] = 
{
    { json_SYMBOL_comma_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_9 },
    { json_SYMBOL_right_curly_brace_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_9 },
};


const sweet::parser::ParserState json_parser_state_19 =
{
    json_parser_state_19_transitions,
    json_parser_state_19_transitions + 2
};

/*
20:
integer . ; comma_terminal, right_curly_brace_terminal
reduce to value on comma_terminal ; 
reduce to value on right_curly_brace_terminal ; 

*/

const sweet::parser::ParserTransition json_parser_state_20_transitions[] = 
{
    { json_SYMBOL_comma_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_10 },
    { json_SYMBOL_right_curly_brace_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_10 },
};


const sweet::parser::ParserState json_parser_state_20 =
{
    json_parser_state_20_transitions,
    json_parser_state_20_transitions + 2
};

/*
21:
real . ; comma_terminal, right_curly_brace_terminal
reduce to value on comma_terminal ; 
reduce to value on right_curly_brace_terminal ; 

*/

const sweet::parser::ParserTransition json_parser_state_21_transitions[] = 
{
    { json_SYMBOL_comma_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_11 },
    { json_SYMBOL_right_curly_brace_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_11 },
};


const sweet::parser::ParserState json_parser_state_21 =
{
    json_parser_state_21_transitions,
    json_parser_state_21_transitions + 2
};

/*
22:
string . ; comma_terminal, right_curly_brace_terminal
reduce to value on comma_terminal ; 
reduce to value on right_curly_brace_terminal ; 

*/

const sweet::parser::ParserTransition json_parser_state_22_transitions[] = 
{
    { json_SYMBOL_comma_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_12 },
    { json_SYMBOL_right_curly_brace_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_12 },
};


const sweet::parser::ParserState json_parser_state_22 =
{
    json_parser_state_22_transitions,
    json_parser_state_22_transitions + 2
};

/*
23:
true_terminal . ; comma_terminal, right_curly_brace_terminal
reduce to boolean on comma_terminal ; 
reduce to boolean on right_curly_brace_terminal ; 

*/

const sweet::parser::ParserTransition json_parser_state_23_transitions[] = 
{
    { json_SYMBOL_comma_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_15 },
    { json_SYMBOL_right_curly_brace_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_15 },
};


const sweet::parser::ParserState json_parser_state_23 =
{
    json_parser_state_23_transitions,
    json_parser_state_23_transitions + 2
};

/*
24:
false_terminal . ; comma_terminal, right_curly_brace_terminal
reduce to boolean on comma_terminal ; 
reduce to boolean on right_curly_brace_terminal ; 

*/

const sweet::parser::ParserTransition json_parser_state_24_transitions[] = 
{
    { json_SYMBOL_comma_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_16 },
    { json_SYMBOL_right_curly_brace_terminal, sweet::parser::TRANSITION_REDUCE, NULL, &json_parser_production_16 },
};


const sweet::parser::ParserState json_parser_state_24 =
{
    json_parser_state_24_transitions,
    json_parser_state_24_transitions + 2
};


sweet::parser::ParserStateMachine json_parser_state_machine = 
{
    &json_parser_state_0,
    &json_lexer_state_machine,
    json_SYMBOL_dot_start,
    json_SYMBOL_dot_end,
    json_SYMBOL_dot_error_terminal,
    json_parser_actions,
    json_parser_actions + 7,
};

