
#include <lalr/ParserStateMachine.hpp>
#include <lalr/ParserState.hpp>
#include <lalr/ParserTransition.hpp>
#include <lalr/ParserSymbol.hpp>
#include <lalr/ParserAction.hpp>
#include <lalr/LexerStateMachine.hpp>
#include <lalr/LexerState.hpp>
#include <lalr/LexerTransition.hpp>
#include <lalr/LexerAction.hpp>

namespace
{

using namespace lalr;

extern const LexerAction lexer_actions [];
extern const LexerTransition lexer_transitions [];
extern const LexerState lexer_states [];
extern const LexerAction whitespace_lexer_actions [];
extern const LexerTransition whitespace_lexer_transitions [];
extern const LexerState whitespace_lexer_states [];
extern const ParserAction actions [];
extern const ParserSymbol symbols [];
extern const ParserTransition transitions [];
extern const ParserState states [];

const ParserAction actions [] = 
{
    {0, "document"},
    {1, "add_to_object"},
    {2, "create_object"},
    {3, "attribute"},
    {4, "add_to_array"},
    {5, "create_array"},
    {6, "null"},
    {7, "value"},
    {8, "object"},
    {9, "array"},
    {-1, nullptr}
};

const ParserSymbol symbols [] = 
{
    {0, "dot_start", ".start", "dot_start", (SymbolType) 2},
    {1, "dot_end", ".end", "dot_end", (SymbolType) 3},
    {2, "error", "error", "error", (SymbolType) 1},
    {3, "dot_whitespace", ".whitespace", "dot_whitespace", (SymbolType) 1},
    {4, "document", "document", "document", (SymbolType) 2},
    {5, "left_curly_brace_terminal", "{", "{", (SymbolType) 1},
    {6, "attributes", "attributes", "attributes", (SymbolType) 2},
    {7, "right_curly_brace_terminal", "}", "}", (SymbolType) 1},
    {8, "comma_terminal", ",", ",", (SymbolType) 1},
    {9, "attribute", "attribute", "attribute", (SymbolType) 2},
    {10, "colon_terminal", ":", ":", (SymbolType) 1},
    {11, "value", "value", "value", (SymbolType) 2},
    {12, "elements", "elements", "elements", (SymbolType) 2},
    {13, "left_square_paren_terminal", "[", "[", (SymbolType) 1},
    {14, "right_square_paren_terminal", "]", "]", (SymbolType) 1},
    {15, "string", "[\\\"']:string:", "string", (SymbolType) 1},
    {16, "null", "null", "null", (SymbolType) 1},
    {17, "boolean", "true|false", "boolean", (SymbolType) 1},
    {18, "integer", "(\\+|\\-)?[0-9]+", "integer", (SymbolType) 1},
    {19, "real", "(\\+|\\-)?[0-9]+(\\.[0-9]+)?((e|E)(\\+|\\-)?[0-9]+)?", "real", (SymbolType) 1},
    {-1, nullptr, nullptr, nullptr, (SymbolType) 0}
};

const ParserTransition transitions [] = 
{
    {&symbols[4], &states[1], nullptr, 0, 0, 0, 0},
    {&symbols[5], &states[2], nullptr, 0, 0, 0, 1},
    {&symbols[1], nullptr, &symbols[4], 0, 0, -1, 2},
    {&symbols[1], nullptr, &symbols[0], 1, 0, -1, 3},
    {&symbols[6], &states[3], nullptr, 0, 0, 0, 4},
    {&symbols[9], &states[9], nullptr, 0, 0, 0, 5},
    {&symbols[15], &states[10], nullptr, 0, 0, 0, 6},
    {&symbols[7], nullptr, &symbols[6], 0, 0, -1, 7},
    {&symbols[8], nullptr, &symbols[6], 0, 0, -1, 8},
    {&symbols[7], &states[4], nullptr, 0, 0, 0, 9},
    {&symbols[8], &states[7], nullptr, 0, 0, 0, 10},
    {&symbols[1], nullptr, &symbols[4], 3, 0, 0, 11},
    {&symbols[6], &states[6], nullptr, 0, 0, 0, 12},
    {&symbols[9], &states[9], nullptr, 0, 0, 0, 13},
    {&symbols[15], &states[10], nullptr, 0, 0, 0, 14},
    {&symbols[7], nullptr, &symbols[6], 0, 0, -1, 15},
    {&symbols[8], nullptr, &symbols[6], 0, 0, -1, 16},
    {&symbols[7], &states[23], nullptr, 0, 0, 0, 17},
    {&symbols[8], &states[7], nullptr, 0, 0, 0, 18},
    {&symbols[9], &states[8], nullptr, 0, 0, 0, 19},
    {&symbols[15], &states[10], nullptr, 0, 0, 0, 20},
    {&symbols[7], nullptr, &symbols[6], 3, 0, 1, 21},
    {&symbols[8], nullptr, &symbols[6], 3, 0, 1, 22},
    {&symbols[7], nullptr, &symbols[6], 1, 0, 2, 23},
    {&symbols[8], nullptr, &symbols[6], 1, 0, 2, 24},
    {&symbols[10], &states[11], nullptr, 0, 0, 0, 25},
    {&symbols[5], &states[5], nullptr, 0, 0, 0, 26},
    {&symbols[11], &states[12], nullptr, 0, 0, 0, 27},
    {&symbols[13], &states[13], nullptr, 0, 0, 0, 28},
    {&symbols[15], &states[22], nullptr, 0, 0, 0, 29},
    {&symbols[16], &states[18], nullptr, 0, 0, 0, 30},
    {&symbols[17], &states[19], nullptr, 0, 0, 0, 31},
    {&symbols[18], &states[20], nullptr, 0, 0, 0, 32},
    {&symbols[19], &states[21], nullptr, 0, 0, 0, 33},
    {&symbols[7], nullptr, &symbols[9], 3, 0, 3, 34},
    {&symbols[8], nullptr, &symbols[9], 3, 0, 3, 35},
    {&symbols[5], &states[5], nullptr, 0, 0, 0, 36},
    {&symbols[11], &states[17], nullptr, 0, 0, 0, 37},
    {&symbols[12], &states[14], nullptr, 0, 0, 0, 38},
    {&symbols[13], &states[13], nullptr, 0, 0, 0, 39},
    {&symbols[15], &states[22], nullptr, 0, 0, 0, 40},
    {&symbols[16], &states[18], nullptr, 0, 0, 0, 41},
    {&symbols[17], &states[19], nullptr, 0, 0, 0, 42},
    {&symbols[18], &states[20], nullptr, 0, 0, 0, 43},
    {&symbols[19], &states[21], nullptr, 0, 0, 0, 44},
    {&symbols[8], nullptr, &symbols[12], 0, 0, -1, 45},
    {&symbols[14], nullptr, &symbols[12], 0, 0, -1, 46},
    {&symbols[8], &states[15], nullptr, 0, 0, 0, 47},
    {&symbols[14], &states[24], nullptr, 0, 0, 0, 48},
    {&symbols[5], &states[5], nullptr, 0, 0, 0, 49},
    {&symbols[11], &states[16], nullptr, 0, 0, 0, 50},
    {&symbols[13], &states[13], nullptr, 0, 0, 0, 51},
    {&symbols[15], &states[22], nullptr, 0, 0, 0, 52},
    {&symbols[16], &states[18], nullptr, 0, 0, 0, 53},
    {&symbols[17], &states[19], nullptr, 0, 0, 0, 54},
    {&symbols[18], &states[20], nullptr, 0, 0, 0, 55},
    {&symbols[19], &states[21], nullptr, 0, 0, 0, 56},
    {&symbols[8], nullptr, &symbols[12], 3, 0, 4, 57},
    {&symbols[14], nullptr, &symbols[12], 3, 0, 4, 58},
    {&symbols[8], nullptr, &symbols[12], 1, 0, 5, 59},
    {&symbols[14], nullptr, &symbols[12], 1, 0, 5, 60},
    {&symbols[7], nullptr, &symbols[11], 1, 0, 6, 61},
    {&symbols[8], nullptr, &symbols[11], 1, 0, 6, 62},
    {&symbols[14], nullptr, &symbols[11], 1, 0, 6, 63},
    {&symbols[7], nullptr, &symbols[11], 1, 0, 7, 64},
    {&symbols[8], nullptr, &symbols[11], 1, 0, 7, 65},
    {&symbols[14], nullptr, &symbols[11], 1, 0, 7, 66},
    {&symbols[7], nullptr, &symbols[11], 1, 0, 7, 67},
    {&symbols[8], nullptr, &symbols[11], 1, 0, 7, 68},
    {&symbols[14], nullptr, &symbols[11], 1, 0, 7, 69},
    {&symbols[7], nullptr, &symbols[11], 1, 0, 7, 70},
    {&symbols[8], nullptr, &symbols[11], 1, 0, 7, 71},
    {&symbols[14], nullptr, &symbols[11], 1, 0, 7, 72},
    {&symbols[7], nullptr, &symbols[11], 1, 0, 7, 73},
    {&symbols[8], nullptr, &symbols[11], 1, 0, 7, 74},
    {&symbols[14], nullptr, &symbols[11], 1, 0, 7, 75},
    {&symbols[7], nullptr, &symbols[11], 3, 0, 8, 76},
    {&symbols[8], nullptr, &symbols[11], 3, 0, 8, 77},
    {&symbols[14], nullptr, &symbols[11], 3, 0, 8, 78},
    {&symbols[7], nullptr, &symbols[11], 3, 0, 9, 79},
    {&symbols[8], nullptr, &symbols[11], 3, 0, 9, 80},
    {&symbols[14], nullptr, &symbols[11], 3, 0, 9, 81},
    {nullptr, nullptr, nullptr, 0, 0, 0, -1}
};

const ParserState states [] = 
{
    {0, 3, &transitions[0], "(null)"},
    {1, 1, &transitions[3], "(null)"},
    {2, 5, &transitions[4], "(null)"},
    {3, 2, &transitions[9], "(null)"},
    {4, 1, &transitions[11], "(null)"},
    {5, 5, &transitions[12], "(null)"},
    {6, 2, &transitions[17], "(null)"},
    {7, 2, &transitions[19], "(null)"},
    {8, 2, &transitions[21], "(null)"},
    {9, 2, &transitions[23], "(null)"},
    {10, 1, &transitions[25], "(null)"},
    {11, 8, &transitions[26], "(null)"},
    {12, 2, &transitions[34], "(null)"},
    {13, 11, &transitions[36], "(null)"},
    {14, 2, &transitions[47], "(null)"},
    {15, 8, &transitions[49], "(null)"},
    {16, 2, &transitions[57], "(null)"},
    {17, 2, &transitions[59], "(null)"},
    {18, 3, &transitions[61], "(null)"},
    {19, 3, &transitions[64], "(null)"},
    {20, 3, &transitions[67], "(null)"},
    {21, 3, &transitions[70], "(null)"},
    {22, 3, &transitions[73], "(null)"},
    {23, 3, &transitions[76], "(null)"},
    {24, 3, &transitions[79], "(null)"},
    {-1, 0, nullptr}
};

const LexerAction lexer_actions [] = 
{
    {0, "string"},
    {-1, nullptr}
};

const LexerTransition lexer_transitions [] = 
{
    {34, 35, &lexer_states[43], nullptr},
    {39, 40, &lexer_states[43], nullptr},
    {43, 44, &lexer_states[37], nullptr},
    {44, 45, &lexer_states[19], nullptr},
    {45, 46, &lexer_states[37], nullptr},
    {46, 47, &lexer_states[6], nullptr},
    {48, 58, &lexer_states[36], nullptr},
    {58, 59, &lexer_states[20], nullptr},
    {91, 92, &lexer_states[21], nullptr},
    {93, 94, &lexer_states[22], nullptr},
    {101, 102, &lexer_states[1], nullptr},
    {102, 103, &lexer_states[31], nullptr},
    {110, 111, &lexer_states[24], nullptr},
    {116, 117, &lexer_states[28], nullptr},
    {123, 124, &lexer_states[17], nullptr},
    {125, 126, &lexer_states[18], nullptr},
    {114, 115, &lexer_states[2], nullptr},
    {114, 115, &lexer_states[3], nullptr},
    {111, 112, &lexer_states[4], nullptr},
    {114, 115, &lexer_states[5], nullptr},
    {119, 120, &lexer_states[7], nullptr},
    {104, 105, &lexer_states[8], nullptr},
    {105, 106, &lexer_states[9], nullptr},
    {116, 117, &lexer_states[10], nullptr},
    {101, 102, &lexer_states[11], nullptr},
    {115, 116, &lexer_states[12], nullptr},
    {112, 113, &lexer_states[13], nullptr},
    {97, 98, &lexer_states[14], nullptr},
    {99, 100, &lexer_states[15], nullptr},
    {101, 102, &lexer_states[16], nullptr},
    {117, 118, &lexer_states[25], nullptr},
    {108, 109, &lexer_states[26], nullptr},
    {108, 109, &lexer_states[27], nullptr},
    {114, 115, &lexer_states[29], nullptr},
    {117, 118, &lexer_states[30], nullptr},
    {101, 102, &lexer_states[35], nullptr},
    {97, 98, &lexer_states[32], nullptr},
    {108, 109, &lexer_states[33], nullptr},
    {115, 116, &lexer_states[34], nullptr},
    {101, 102, &lexer_states[35], nullptr},
    {46, 47, &lexer_states[38], nullptr},
    {48, 58, &lexer_states[36], nullptr},
    {69, 70, &lexer_states[40], nullptr},
    {101, 102, &lexer_states[40], nullptr},
    {48, 58, &lexer_states[36], nullptr},
    {48, 58, &lexer_states[39], nullptr},
    {48, 58, &lexer_states[39], nullptr},
    {69, 70, &lexer_states[40], nullptr},
    {101, 102, &lexer_states[40], nullptr},
    {43, 44, &lexer_states[41], nullptr},
    {45, 46, &lexer_states[41], nullptr},
    {48, 58, &lexer_states[42], nullptr},
    {48, 58, &lexer_states[42], nullptr},
    {48, 58, &lexer_states[42], nullptr},
    {0, 2147483647, &lexer_states[23], &lexer_actions[0]},
    {-1, -1, nullptr, nullptr}
};

const LexerState lexer_states [] = 
{
    {0, 16, &lexer_transitions[0], nullptr},
    {1, 1, &lexer_transitions[16], nullptr},
    {2, 1, &lexer_transitions[17], nullptr},
    {3, 1, &lexer_transitions[18], nullptr},
    {4, 1, &lexer_transitions[19], nullptr},
    {5, 0, &lexer_transitions[20], &symbols[2]},
    {6, 1, &lexer_transitions[20], nullptr},
    {7, 1, &lexer_transitions[21], nullptr},
    {8, 1, &lexer_transitions[22], nullptr},
    {9, 1, &lexer_transitions[23], nullptr},
    {10, 1, &lexer_transitions[24], nullptr},
    {11, 1, &lexer_transitions[25], nullptr},
    {12, 1, &lexer_transitions[26], nullptr},
    {13, 1, &lexer_transitions[27], nullptr},
    {14, 1, &lexer_transitions[28], nullptr},
    {15, 1, &lexer_transitions[29], nullptr},
    {16, 0, &lexer_transitions[30], &symbols[3]},
    {17, 0, &lexer_transitions[30], &symbols[5]},
    {18, 0, &lexer_transitions[30], &symbols[7]},
    {19, 0, &lexer_transitions[30], &symbols[8]},
    {20, 0, &lexer_transitions[30], &symbols[10]},
    {21, 0, &lexer_transitions[30], &symbols[13]},
    {22, 0, &lexer_transitions[30], &symbols[14]},
    {23, 0, &lexer_transitions[30], &symbols[15]},
    {24, 1, &lexer_transitions[30], nullptr},
    {25, 1, &lexer_transitions[31], nullptr},
    {26, 1, &lexer_transitions[32], nullptr},
    {27, 0, &lexer_transitions[33], &symbols[16]},
    {28, 1, &lexer_transitions[33], nullptr},
    {29, 1, &lexer_transitions[34], nullptr},
    {30, 1, &lexer_transitions[35], nullptr},
    {31, 1, &lexer_transitions[36], nullptr},
    {32, 1, &lexer_transitions[37], nullptr},
    {33, 1, &lexer_transitions[38], nullptr},
    {34, 1, &lexer_transitions[39], nullptr},
    {35, 0, &lexer_transitions[40], &symbols[17]},
    {36, 4, &lexer_transitions[40], &symbols[18]},
    {37, 1, &lexer_transitions[44], nullptr},
    {38, 1, &lexer_transitions[45], nullptr},
    {39, 3, &lexer_transitions[46], &symbols[19]},
    {40, 3, &lexer_transitions[49], nullptr},
    {41, 1, &lexer_transitions[52], nullptr},
    {42, 1, &lexer_transitions[53], &symbols[19]},
    {43, 1, &lexer_transitions[54], nullptr},
    {-1, 0, nullptr, nullptr}
};

const LexerStateMachine lexer_state_machine = 
{
    1, // #actions
    55, // #transitions
    44, // #states
    lexer_actions, // actions
    lexer_transitions, // transitions
    lexer_states, // states
    &lexer_states[0] // start state
};

const LexerAction whitespace_lexer_actions [] = 
{
    {-1, nullptr}
};

const LexerTransition whitespace_lexer_transitions [] = 
{
    {9, 11, &whitespace_lexer_states[0], nullptr},
    {13, 14, &whitespace_lexer_states[0], nullptr},
    {32, 33, &whitespace_lexer_states[0], nullptr},
    {-1, -1, nullptr, nullptr}
};

const LexerState whitespace_lexer_states [] = 
{
    {0, 3, &whitespace_lexer_transitions[0], &symbols[3]},
    {-1, 0, nullptr, nullptr}
};

const LexerStateMachine whitespace_lexer_state_machine = 
{
    0, // #actions
    3, // #transitions
    1, // #states
    whitespace_lexer_actions, // actions
    whitespace_lexer_transitions, // transitions
    whitespace_lexer_states, // states
    &whitespace_lexer_states[0] // start state
};

const ParserStateMachine parser_state_machine = 
{
    "json",
    10, // #actions
    20, // #symbols
    82, // #transitions
    25, // #states
    actions,
    symbols,
    transitions,
    states,
    &symbols[0], // start symbol
    &symbols[1], // end symbol
    &symbols[2], // error symbol
    &symbols[3], // whitespace symbol
    &states[0], // start state
    &lexer_state_machine, // lexer state machine
    &whitespace_lexer_state_machine // whitespace lexer state machine
};

}

const lalr::ParserStateMachine* json_parser_state_machine = &parser_state_machine;
