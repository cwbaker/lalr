
#include <lalr/ParserStateMachine.hpp>
#include <lalr/ParserState.hpp>
#include <lalr/ParserTransition.hpp>
#include <lalr/ParserSymbol.hpp>
#include <lalr/ParserAction.hpp>
#include <lalr/LexerStateMachine.hpp>
#include <lalr/LexerState.hpp>
#include <lalr/LexerTransition.hpp>
#include <lalr/LexerAction.hpp>

using namespace lalr;

namespace
{

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
    {1, "element"},
    {2, "add_to_element"},
    {3, "create_element"},
    {4, "content"},
    {5, "attribute"},
    {6, "value"},
    {-1, nullptr}
};

const ParserSymbol symbols [] = 
{
    {0, "dot_start", ".start", (SymbolType) 2},
    {1, "dot_end", ".end", (SymbolType) 3},
    {2, "error", "error", (SymbolType) 1},
    {3, "document", "document", (SymbolType) 2},
    {4, "left_curly_brace_terminal", "{", (SymbolType) 1},
    {5, "element", "element", (SymbolType) 2},
    {6, "right_curly_brace_terminal", "}", (SymbolType) 1},
    {7, "colon_terminal", ":", (SymbolType) 1},
    {8, "contents", "contents", (SymbolType) 2},
    {9, "comma_terminal", ",", (SymbolType) 1},
    {10, "content", "content", (SymbolType) 2},
    {11, "attribute", "attribute", (SymbolType) 2},
    {12, "value", "value", (SymbolType) 2},
    {13, "null_terminal", "null", (SymbolType) 1},
    {14, "true_terminal", "true", (SymbolType) 1},
    {15, "false_terminal", "false", (SymbolType) 1},
    {16, "string", "[\"']:string:", (SymbolType) 1},
    {17, "integer", "(\+|\-)?[0-9]+", (SymbolType) 1},
    {18, "real", "(\+|\-)?[0-9]+(\.[0-9]+)?((e|E)(\+|\-)?[0-9]+)?", (SymbolType) 1},
    {-1, nullptr, nullptr, (SymbolType) 0}
};

const ParserTransition transitions [] = 
{
    {&symbols[3], &states[1], nullptr, 0, 0, -1, (TransitionType) 0, 0},
    {&symbols[4], &states[2], nullptr, 0, 0, -1, (TransitionType) 0, 1},
    {&symbols[1], nullptr, &symbols[0], 1, 0, -1, (TransitionType) 1, 2},
    {&symbols[5], &states[3], nullptr, 0, 0, -1, (TransitionType) 0, 3},
    {&symbols[16], &states[7], nullptr, 0, 0, -1, (TransitionType) 0, 4},
    {&symbols[6], &states[4], nullptr, 0, 0, -1, (TransitionType) 0, 5},
    {&symbols[1], nullptr, &symbols[3], 3, 0, 0, (TransitionType) 1, 6},
    {&symbols[5], &states[16], nullptr, 0, 0, -1, (TransitionType) 0, 7},
    {&symbols[8], &states[11], nullptr, 0, 0, -1, (TransitionType) 0, 8},
    {&symbols[10], &states[14], nullptr, 0, 0, -1, (TransitionType) 0, 9},
    {&symbols[11], &states[15], nullptr, 0, 0, -1, (TransitionType) 0, 10},
    {&symbols[16], &states[8], nullptr, 0, 0, -1, (TransitionType) 0, 11},
    {&symbols[5], &states[16], nullptr, 0, 0, -1, (TransitionType) 0, 12},
    {&symbols[10], &states[13], nullptr, 0, 0, -1, (TransitionType) 0, 13},
    {&symbols[11], &states[15], nullptr, 0, 0, -1, (TransitionType) 0, 14},
    {&symbols[16], &states[8], nullptr, 0, 0, -1, (TransitionType) 0, 15},
    {&symbols[7], &states[9], nullptr, 0, 0, -1, (TransitionType) 0, 16},
    {&symbols[7], &states[10], nullptr, 0, 0, -1, (TransitionType) 0, 17},
    {&symbols[4], &states[5], nullptr, 0, 0, -1, (TransitionType) 0, 18},
    {&symbols[4], &states[5], nullptr, 0, 0, -1, (TransitionType) 0, 19},
    {&symbols[12], &states[17], nullptr, 0, 0, -1, (TransitionType) 0, 20},
    {&symbols[13], &states[18], nullptr, 0, 0, -1, (TransitionType) 0, 21},
    {&symbols[14], &states[19], nullptr, 0, 0, -1, (TransitionType) 0, 22},
    {&symbols[15], &states[20], nullptr, 0, 0, -1, (TransitionType) 0, 23},
    {&symbols[16], &states[23], nullptr, 0, 0, -1, (TransitionType) 0, 24},
    {&symbols[17], &states[21], nullptr, 0, 0, -1, (TransitionType) 0, 25},
    {&symbols[18], &states[22], nullptr, 0, 0, -1, (TransitionType) 0, 26},
    {&symbols[6], &states[12], nullptr, 0, 0, -1, (TransitionType) 0, 27},
    {&symbols[9], &states[6], nullptr, 0, 0, -1, (TransitionType) 0, 28},
    {&symbols[6], nullptr, &symbols[5], 5, 0, 1, (TransitionType) 1, 29},
    {&symbols[9], nullptr, &symbols[5], 5, 0, 1, (TransitionType) 1, 30},
    {&symbols[6], nullptr, &symbols[8], 3, 0, 2, (TransitionType) 1, 31},
    {&symbols[9], nullptr, &symbols[8], 3, 0, 2, (TransitionType) 1, 32},
    {&symbols[6], nullptr, &symbols[8], 1, 0, 3, (TransitionType) 1, 33},
    {&symbols[9], nullptr, &symbols[8], 1, 0, 3, (TransitionType) 1, 34},
    {&symbols[6], nullptr, &symbols[10], 1, 0, 4, (TransitionType) 1, 35},
    {&symbols[9], nullptr, &symbols[10], 1, 0, 4, (TransitionType) 1, 36},
    {&symbols[6], nullptr, &symbols[10], 1, 0, 4, (TransitionType) 1, 37},
    {&symbols[9], nullptr, &symbols[10], 1, 0, 4, (TransitionType) 1, 38},
    {&symbols[6], nullptr, &symbols[11], 3, 0, 5, (TransitionType) 1, 39},
    {&symbols[9], nullptr, &symbols[11], 3, 0, 5, (TransitionType) 1, 40},
    {&symbols[6], nullptr, &symbols[12], 1, 0, 6, (TransitionType) 1, 41},
    {&symbols[9], nullptr, &symbols[12], 1, 0, 6, (TransitionType) 1, 42},
    {&symbols[6], nullptr, &symbols[12], 1, 0, 6, (TransitionType) 1, 43},
    {&symbols[9], nullptr, &symbols[12], 1, 0, 6, (TransitionType) 1, 44},
    {&symbols[6], nullptr, &symbols[12], 1, 0, 6, (TransitionType) 1, 45},
    {&symbols[9], nullptr, &symbols[12], 1, 0, 6, (TransitionType) 1, 46},
    {&symbols[6], nullptr, &symbols[12], 1, 0, 6, (TransitionType) 1, 47},
    {&symbols[9], nullptr, &symbols[12], 1, 0, 6, (TransitionType) 1, 48},
    {&symbols[6], nullptr, &symbols[12], 1, 0, 6, (TransitionType) 1, 49},
    {&symbols[9], nullptr, &symbols[12], 1, 0, 6, (TransitionType) 1, 50},
    {&symbols[6], nullptr, &symbols[12], 1, 0, 6, (TransitionType) 1, 51},
    {&symbols[9], nullptr, &symbols[12], 1, 0, 6, (TransitionType) 1, 52},
    {nullptr, nullptr, nullptr, 0, 0, 0, (TransitionType) 0, -1}
};

const ParserState states [] = 
{
    {0, 2, &transitions[0]},
    {1, 1, &transitions[2]},
    {2, 2, &transitions[3]},
    {3, 1, &transitions[5]},
    {4, 1, &transitions[6]},
    {5, 5, &transitions[7]},
    {6, 4, &transitions[12]},
    {7, 1, &transitions[16]},
    {8, 1, &transitions[17]},
    {9, 1, &transitions[18]},
    {10, 8, &transitions[19]},
    {11, 2, &transitions[27]},
    {12, 2, &transitions[29]},
    {13, 2, &transitions[31]},
    {14, 2, &transitions[33]},
    {15, 2, &transitions[35]},
    {16, 2, &transitions[37]},
    {17, 2, &transitions[39]},
    {18, 2, &transitions[41]},
    {19, 2, &transitions[43]},
    {20, 2, &transitions[45]},
    {21, 2, &transitions[47]},
    {22, 2, &transitions[49]},
    {23, 2, &transitions[51]},
    {-1, 0, nullptr}
};

const LexerAction lexer_actions [] = 
{
    {0, "string"},
    {-1, nullptr}
};

const LexerTransition lexer_transitions [] = 
{
    {34, 35, &lexer_states[15], nullptr},
    {39, 40, &lexer_states[15], nullptr},
    {43, 44, &lexer_states[29], nullptr},
    {44, 45, &lexer_states[11], nullptr},
    {45, 46, &lexer_states[29], nullptr},
    {48, 58, &lexer_states[30], nullptr},
    {58, 59, &lexer_states[8], nullptr},
    {101, 102, &lexer_states[1], nullptr},
    {102, 103, &lexer_states[13], nullptr},
    {110, 111, &lexer_states[7], nullptr},
    {116, 117, &lexer_states[12], nullptr},
    {123, 124, &lexer_states[6], nullptr},
    {125, 126, &lexer_states[10], nullptr},
    {114, 115, &lexer_states[2], nullptr},
    {114, 115, &lexer_states[3], nullptr},
    {111, 112, &lexer_states[4], nullptr},
    {114, 115, &lexer_states[5], nullptr},
    {117, 118, &lexer_states[9], nullptr},
    {108, 109, &lexer_states[18], nullptr},
    {114, 115, &lexer_states[22], nullptr},
    {97, 98, &lexer_states[23], nullptr},
    {0, 2147483647, &lexer_states[16], &lexer_actions[0]},
    {108, 109, &lexer_states[17], nullptr},
    {101, 102, &lexer_states[19], nullptr},
    {115, 116, &lexer_states[24], nullptr},
    {117, 118, &lexer_states[20], nullptr},
    {108, 109, &lexer_states[21], nullptr},
    {101, 102, &lexer_states[14], nullptr},
    {48, 58, &lexer_states[26], nullptr},
    {48, 58, &lexer_states[26], nullptr},
    {69, 70, &lexer_states[31], nullptr},
    {101, 102, &lexer_states[31], nullptr},
    {48, 58, &lexer_states[28], nullptr},
    {48, 58, &lexer_states[28], nullptr},
    {48, 58, &lexer_states[30], nullptr},
    {46, 47, &lexer_states[25], nullptr},
    {48, 58, &lexer_states[30], nullptr},
    {69, 70, &lexer_states[31], nullptr},
    {101, 102, &lexer_states[31], nullptr},
    {43, 44, &lexer_states[27], nullptr},
    {45, 46, &lexer_states[27], nullptr},
    {48, 58, &lexer_states[28], nullptr},
    {-1, -1, nullptr, nullptr}
};

const LexerState lexer_states [] = 
{
    {0, 13, &lexer_transitions[0], nullptr},
    {1, 1, &lexer_transitions[13], nullptr},
    {2, 1, &lexer_transitions[14], nullptr},
    {3, 1, &lexer_transitions[15], nullptr},
    {4, 1, &lexer_transitions[16], nullptr},
    {5, 0, &lexer_transitions[17], &symbols[2]},
    {6, 0, &lexer_transitions[17], &symbols[4]},
    {7, 1, &lexer_transitions[17], nullptr},
    {8, 0, &lexer_transitions[18], &symbols[7]},
    {9, 1, &lexer_transitions[18], nullptr},
    {10, 0, &lexer_transitions[19], &symbols[6]},
    {11, 0, &lexer_transitions[19], &symbols[9]},
    {12, 1, &lexer_transitions[19], nullptr},
    {13, 1, &lexer_transitions[20], nullptr},
    {14, 0, &lexer_transitions[21], &symbols[15]},
    {15, 1, &lexer_transitions[21], nullptr},
    {16, 0, &lexer_transitions[22], &symbols[16]},
    {17, 0, &lexer_transitions[22], &symbols[13]},
    {18, 1, &lexer_transitions[22], nullptr},
    {19, 0, &lexer_transitions[23], &symbols[14]},
    {20, 1, &lexer_transitions[23], nullptr},
    {21, 1, &lexer_transitions[24], nullptr},
    {22, 1, &lexer_transitions[25], nullptr},
    {23, 1, &lexer_transitions[26], nullptr},
    {24, 1, &lexer_transitions[27], nullptr},
    {25, 1, &lexer_transitions[28], nullptr},
    {26, 3, &lexer_transitions[29], &symbols[18]},
    {27, 1, &lexer_transitions[32], nullptr},
    {28, 1, &lexer_transitions[33], &symbols[18]},
    {29, 1, &lexer_transitions[34], nullptr},
    {30, 4, &lexer_transitions[35], &symbols[17]},
    {31, 3, &lexer_transitions[39], nullptr},
    {-1, 0, nullptr, nullptr}
};

const LexerStateMachine lexer_state_machine = 
{
    1, // #actions
    42, // #transitions
    32, // #states
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
    {0, 3, &whitespace_lexer_transitions[0], nullptr},
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
    7, // #actions
    19, // #symbols
    53, // #transitions
    24, // #states
    actions,
    symbols,
    transitions,
    states,
    &symbols[0], // start symbol
    &symbols[1], // end symbol
    &symbols[2], // error symbol
    &states[0], // start state
    &lexer_state_machine, // lexer state machine
    &whitespace_lexer_state_machine // whitespace lexer state machine
};

}

const ParserStateMachine* json_parser_state_machine = &parser_state_machine;

