
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
    {0, "result"},
    {1, "unexpected_error"},
    {2, "add"},
    {3, "subtract"},
    {4, "multiply"},
    {5, "divide"},
    {6, "unknown_operator_error"},
    {7, "compound"},
    {8, "integer"},
    {-1, nullptr}
};

const ParserSymbol symbols [] = 
{
    {0, "dot_start", ".start", (SymbolType) 2},
    {1, "dot_end", ".end", (SymbolType) 3},
    {2, "error", "error", (SymbolType) 1},
    {3, "left_paren_terminal", "(", (SymbolType) 1},
    {4, "right_paren_terminal", ")", (SymbolType) 1},
    {5, "plus_terminal", "+", (SymbolType) 1},
    {6, "minus_terminal", "-", (SymbolType) 1},
    {7, "star_terminal", "*", (SymbolType) 1},
    {8, "slash_terminal", "/", (SymbolType) 1},
    {9, "stmts", "stmts", (SymbolType) 2},
    {10, "stmt", "stmt", (SymbolType) 2},
    {11, "expr", "expr", (SymbolType) 2},
    {12, "semi_colon_terminal", ";", (SymbolType) 1},
    {13, "integer", "[0-9]+", (SymbolType) 1},
    {-1, nullptr, nullptr, (SymbolType) 0}
};

const ParserTransition transitions [] = 
{
    {&symbols[1], nullptr, &symbols[9], 0, 2, -1, (TransitionType) 1, 0},
    {&symbols[2], nullptr, &symbols[9], 0, 2, -1, (TransitionType) 1, 1},
    {&symbols[3], &states[13], nullptr, 0, 0, -1, (TransitionType) 0, 2},
    {&symbols[9], &states[1], nullptr, 0, 0, -1, (TransitionType) 0, 3},
    {&symbols[10], &states[3], nullptr, 0, 0, -1, (TransitionType) 0, 4},
    {&symbols[11], &states[4], nullptr, 0, 0, -1, (TransitionType) 0, 5},
    {&symbols[13], &states[21], nullptr, 0, 0, -1, (TransitionType) 0, 6},
    {&symbols[1], nullptr, &symbols[0], 1, 0, -1, (TransitionType) 1, 7},
    {&symbols[2], &states[6], nullptr, 0, 0, -1, (TransitionType) 0, 8},
    {&symbols[3], &states[13], nullptr, 0, 0, -1, (TransitionType) 0, 9},
    {&symbols[10], &states[2], nullptr, 0, 0, -1, (TransitionType) 0, 10},
    {&symbols[11], &states[4], nullptr, 0, 0, -1, (TransitionType) 0, 11},
    {&symbols[13], &states[21], nullptr, 0, 0, -1, (TransitionType) 0, 12},
    {&symbols[1], nullptr, &symbols[9], 2, 0, -1, (TransitionType) 1, 13},
    {&symbols[2], nullptr, &symbols[9], 2, 0, -1, (TransitionType) 1, 14},
    {&symbols[3], nullptr, &symbols[9], 2, 0, -1, (TransitionType) 1, 15},
    {&symbols[13], nullptr, &symbols[9], 2, 0, -1, (TransitionType) 1, 16},
    {&symbols[1], nullptr, &symbols[9], 1, 0, -1, (TransitionType) 1, 17},
    {&symbols[2], nullptr, &symbols[9], 1, 0, -1, (TransitionType) 1, 18},
    {&symbols[3], nullptr, &symbols[9], 1, 0, -1, (TransitionType) 1, 19},
    {&symbols[13], nullptr, &symbols[9], 1, 0, -1, (TransitionType) 1, 20},
    {&symbols[2], &states[12], nullptr, 0, 0, -1, (TransitionType) 0, 21},
    {&symbols[5], &states[8], nullptr, 0, 0, -1, (TransitionType) 0, 22},
    {&symbols[6], &states[9], nullptr, 0, 0, -1, (TransitionType) 0, 23},
    {&symbols[7], &states[10], nullptr, 0, 0, -1, (TransitionType) 0, 24},
    {&symbols[8], &states[11], nullptr, 0, 0, -1, (TransitionType) 0, 25},
    {&symbols[12], &states[5], nullptr, 0, 0, -1, (TransitionType) 0, 26},
    {&symbols[1], nullptr, &symbols[10], 2, 0, 0, (TransitionType) 1, 27},
    {&symbols[2], nullptr, &symbols[10], 2, 0, 0, (TransitionType) 1, 28},
    {&symbols[3], nullptr, &symbols[10], 2, 0, 0, (TransitionType) 1, 29},
    {&symbols[13], nullptr, &symbols[10], 2, 0, 0, (TransitionType) 1, 30},
    {&symbols[12], &states[7], nullptr, 0, 0, -1, (TransitionType) 0, 31},
    {&symbols[1], nullptr, &symbols[10], 2, 0, 1, (TransitionType) 1, 32},
    {&symbols[2], nullptr, &symbols[10], 2, 0, 1, (TransitionType) 1, 33},
    {&symbols[3], nullptr, &symbols[10], 2, 0, 1, (TransitionType) 1, 34},
    {&symbols[13], nullptr, &symbols[10], 2, 0, 1, (TransitionType) 1, 35},
    {&symbols[3], &states[13], nullptr, 0, 0, -1, (TransitionType) 0, 36},
    {&symbols[11], &states[14], nullptr, 0, 0, -1, (TransitionType) 0, 37},
    {&symbols[13], &states[21], nullptr, 0, 0, -1, (TransitionType) 0, 38},
    {&symbols[3], &states[13], nullptr, 0, 0, -1, (TransitionType) 0, 39},
    {&symbols[11], &states[15], nullptr, 0, 0, -1, (TransitionType) 0, 40},
    {&symbols[13], &states[21], nullptr, 0, 0, -1, (TransitionType) 0, 41},
    {&symbols[3], &states[13], nullptr, 0, 0, -1, (TransitionType) 0, 42},
    {&symbols[11], &states[16], nullptr, 0, 0, -1, (TransitionType) 0, 43},
    {&symbols[13], &states[21], nullptr, 0, 0, -1, (TransitionType) 0, 44},
    {&symbols[3], &states[13], nullptr, 0, 0, -1, (TransitionType) 0, 45},
    {&symbols[11], &states[17], nullptr, 0, 0, -1, (TransitionType) 0, 46},
    {&symbols[13], &states[21], nullptr, 0, 0, -1, (TransitionType) 0, 47},
    {&symbols[3], &states[13], nullptr, 0, 0, -1, (TransitionType) 0, 48},
    {&symbols[11], &states[18], nullptr, 0, 0, -1, (TransitionType) 0, 49},
    {&symbols[13], &states[21], nullptr, 0, 0, -1, (TransitionType) 0, 50},
    {&symbols[3], &states[13], nullptr, 0, 0, -1, (TransitionType) 0, 51},
    {&symbols[11], &states[19], nullptr, 0, 0, -1, (TransitionType) 0, 52},
    {&symbols[13], &states[21], nullptr, 0, 0, -1, (TransitionType) 0, 53},
    {&symbols[2], nullptr, &symbols[11], 3, 3, 2, (TransitionType) 1, 54},
    {&symbols[4], nullptr, &symbols[11], 3, 3, 2, (TransitionType) 1, 55},
    {&symbols[5], &states[8], nullptr, 0, 0, -1, (TransitionType) 0, 56},
    {&symbols[6], &states[9], nullptr, 0, 0, -1, (TransitionType) 0, 57},
    {&symbols[7], &states[10], nullptr, 0, 0, -1, (TransitionType) 0, 58},
    {&symbols[8], &states[11], nullptr, 0, 0, -1, (TransitionType) 0, 59},
    {&symbols[12], nullptr, &symbols[11], 3, 3, 2, (TransitionType) 1, 60},
    {&symbols[2], nullptr, &symbols[11], 3, 3, 3, (TransitionType) 1, 61},
    {&symbols[4], nullptr, &symbols[11], 3, 3, 3, (TransitionType) 1, 62},
    {&symbols[5], &states[8], nullptr, 0, 0, -1, (TransitionType) 0, 63},
    {&symbols[6], &states[9], nullptr, 0, 0, -1, (TransitionType) 0, 64},
    {&symbols[7], &states[10], nullptr, 0, 0, -1, (TransitionType) 0, 65},
    {&symbols[8], &states[11], nullptr, 0, 0, -1, (TransitionType) 0, 66},
    {&symbols[12], nullptr, &symbols[11], 3, 3, 3, (TransitionType) 1, 67},
    {&symbols[2], nullptr, &symbols[11], 3, 4, 4, (TransitionType) 1, 68},
    {&symbols[4], nullptr, &symbols[11], 3, 4, 4, (TransitionType) 1, 69},
    {&symbols[5], nullptr, &symbols[11], 3, 4, 4, (TransitionType) 1, 70},
    {&symbols[6], nullptr, &symbols[11], 3, 4, 4, (TransitionType) 1, 71},
    {&symbols[7], &states[10], nullptr, 0, 0, -1, (TransitionType) 0, 72},
    {&symbols[8], &states[11], nullptr, 0, 0, -1, (TransitionType) 0, 73},
    {&symbols[12], nullptr, &symbols[11], 3, 4, 4, (TransitionType) 1, 74},
    {&symbols[2], nullptr, &symbols[11], 3, 4, 5, (TransitionType) 1, 75},
    {&symbols[4], nullptr, &symbols[11], 3, 4, 5, (TransitionType) 1, 76},
    {&symbols[5], nullptr, &symbols[11], 3, 4, 5, (TransitionType) 1, 77},
    {&symbols[6], nullptr, &symbols[11], 3, 4, 5, (TransitionType) 1, 78},
    {&symbols[7], &states[10], nullptr, 0, 0, -1, (TransitionType) 0, 79},
    {&symbols[8], &states[11], nullptr, 0, 0, -1, (TransitionType) 0, 80},
    {&symbols[12], nullptr, &symbols[11], 3, 4, 5, (TransitionType) 1, 81},
    {&symbols[2], &states[12], nullptr, 0, 0, -1, (TransitionType) 0, 82},
    {&symbols[4], nullptr, &symbols[11], 3, 1, 6, (TransitionType) 1, 83},
    {&symbols[5], &states[8], nullptr, 0, 0, -1, (TransitionType) 0, 84},
    {&symbols[6], &states[9], nullptr, 0, 0, -1, (TransitionType) 0, 85},
    {&symbols[7], &states[10], nullptr, 0, 0, -1, (TransitionType) 0, 86},
    {&symbols[8], &states[11], nullptr, 0, 0, -1, (TransitionType) 0, 87},
    {&symbols[12], nullptr, &symbols[11], 3, 1, 6, (TransitionType) 1, 88},
    {&symbols[2], &states[12], nullptr, 0, 0, -1, (TransitionType) 0, 89},
    {&symbols[4], &states[20], nullptr, 0, 0, -1, (TransitionType) 0, 90},
    {&symbols[5], &states[8], nullptr, 0, 0, -1, (TransitionType) 0, 91},
    {&symbols[6], &states[9], nullptr, 0, 0, -1, (TransitionType) 0, 92},
    {&symbols[7], &states[10], nullptr, 0, 0, -1, (TransitionType) 0, 93},
    {&symbols[8], &states[11], nullptr, 0, 0, -1, (TransitionType) 0, 94},
    {&symbols[2], nullptr, &symbols[11], 3, 2, 7, (TransitionType) 1, 95},
    {&symbols[4], nullptr, &symbols[11], 3, 2, 7, (TransitionType) 1, 96},
    {&symbols[5], nullptr, &symbols[11], 3, 2, 7, (TransitionType) 1, 97},
    {&symbols[6], nullptr, &symbols[11], 3, 2, 7, (TransitionType) 1, 98},
    {&symbols[7], nullptr, &symbols[11], 3, 2, 7, (TransitionType) 1, 99},
    {&symbols[8], nullptr, &symbols[11], 3, 2, 7, (TransitionType) 1, 100},
    {&symbols[12], nullptr, &symbols[11], 3, 2, 7, (TransitionType) 1, 101},
    {&symbols[2], nullptr, &symbols[11], 1, 5, 8, (TransitionType) 1, 102},
    {&symbols[4], nullptr, &symbols[11], 1, 5, 8, (TransitionType) 1, 103},
    {&symbols[5], nullptr, &symbols[11], 1, 5, 8, (TransitionType) 1, 104},
    {&symbols[6], nullptr, &symbols[11], 1, 5, 8, (TransitionType) 1, 105},
    {&symbols[7], nullptr, &symbols[11], 1, 5, 8, (TransitionType) 1, 106},
    {&symbols[8], nullptr, &symbols[11], 1, 5, 8, (TransitionType) 1, 107},
    {&symbols[12], nullptr, &symbols[11], 1, 5, 8, (TransitionType) 1, 108},
    {nullptr, nullptr, nullptr, 0, 0, 0, (TransitionType) 0, -1}
};

const ParserState states [] = 
{
    {0, 7, &transitions[0]},
    {1, 6, &transitions[7]},
    {2, 4, &transitions[13]},
    {3, 4, &transitions[17]},
    {4, 6, &transitions[21]},
    {5, 4, &transitions[27]},
    {6, 1, &transitions[31]},
    {7, 4, &transitions[32]},
    {8, 3, &transitions[36]},
    {9, 3, &transitions[39]},
    {10, 3, &transitions[42]},
    {11, 3, &transitions[45]},
    {12, 3, &transitions[48]},
    {13, 3, &transitions[51]},
    {14, 7, &transitions[54]},
    {15, 7, &transitions[61]},
    {16, 7, &transitions[68]},
    {17, 7, &transitions[75]},
    {18, 7, &transitions[82]},
    {19, 6, &transitions[89]},
    {20, 7, &transitions[95]},
    {21, 7, &transitions[102]},
    {-1, 0, nullptr}
};

const LexerAction lexer_actions [] = 
{
    {-1, nullptr}
};

const LexerTransition lexer_transitions [] = 
{
    {40, 41, &lexer_states[6], nullptr},
    {41, 42, &lexer_states[9], nullptr},
    {42, 43, &lexer_states[8], nullptr},
    {43, 44, &lexer_states[10], nullptr},
    {45, 46, &lexer_states[7], nullptr},
    {47, 48, &lexer_states[13], nullptr},
    {48, 58, &lexer_states[11], nullptr},
    {59, 60, &lexer_states[12], nullptr},
    {101, 102, &lexer_states[1], nullptr},
    {114, 115, &lexer_states[2], nullptr},
    {114, 115, &lexer_states[3], nullptr},
    {111, 112, &lexer_states[4], nullptr},
    {114, 115, &lexer_states[5], nullptr},
    {48, 58, &lexer_states[11], nullptr},
    {-1, -1, nullptr, nullptr}
};

const LexerState lexer_states [] = 
{
    {0, 9, &lexer_transitions[0], nullptr},
    {1, 1, &lexer_transitions[9], nullptr},
    {2, 1, &lexer_transitions[10], nullptr},
    {3, 1, &lexer_transitions[11], nullptr},
    {4, 1, &lexer_transitions[12], nullptr},
    {5, 0, &lexer_transitions[13], &symbols[2]},
    {6, 0, &lexer_transitions[13], &symbols[3]},
    {7, 0, &lexer_transitions[13], &symbols[6]},
    {8, 0, &lexer_transitions[13], &symbols[7]},
    {9, 0, &lexer_transitions[13], &symbols[4]},
    {10, 0, &lexer_transitions[13], &symbols[5]},
    {11, 1, &lexer_transitions[13], &symbols[13]},
    {12, 0, &lexer_transitions[14], &symbols[12]},
    {13, 0, &lexer_transitions[14], &symbols[8]},
    {-1, 0, nullptr, nullptr}
};

const LexerStateMachine lexer_state_machine = 
{
    0, // #actions
    14, // #transitions
    14, // #states
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
    "error_handling_calculator",
    9, // #actions
    14, // #symbols
    109, // #transitions
    22, // #states
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

const ParserStateMachine* error_handling_calculator_parser_state_machine = &parser_state_machine;

