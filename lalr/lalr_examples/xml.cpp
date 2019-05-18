
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
    {1, "add_element"},
    {2, "create_element"},
    {3, "short_element"},
    {4, "long_element"},
    {5, "add_attribute"},
    {6, "create_attribute"},
    {7, "attribute"},
    {-1, nullptr}
};

const ParserSymbol symbols [] = 
{
    {0, "dot_start", ".start", (SymbolType) 2},
    {1, "dot_end", ".end", (SymbolType) 3},
    {2, "error", "error", (SymbolType) 1},
    {3, "lt_terminal", "<", (SymbolType) 1},
    {4, "gt_terminal", ">", (SymbolType) 1},
    {5, "document", "document", (SymbolType) 2},
    {6, "prolog", "prolog", (SymbolType) 2},
    {7, "element", "element", (SymbolType) 2},
    {8, "lt__backslash__question_xml_terminal", "<\?xml", (SymbolType) 1},
    {9, "attributes", "attributes", (SymbolType) 2},
    {10, "backslash__question__gt_terminal", "\?>", (SymbolType) 1},
    {11, "elements", "elements", (SymbolType) 2},
    {12, "slash__gt_terminal", "/>", (SymbolType) 1},
    {13, "lt__slash_terminal", "</", (SymbolType) 1},
    {14, "attribute", "attribute", (SymbolType) 2},
    {15, "eq_terminal", "=", (SymbolType) 1},
    {16, "name", "[A-Za-z_:][A-Za-z0-9_:\.-]*", (SymbolType) 1},
    {17, "value", "[\"']:string:", (SymbolType) 1},
    {-1, nullptr, nullptr, (SymbolType) 0}
};

const ParserTransition transitions [] = 
{
    {&symbols[3], nullptr, &symbols[6], 0, 0, -1, (TransitionType) 1, 0},
    {&symbols[5], &states[1], nullptr, 0, 0, -1, (TransitionType) 0, 1},
    {&symbols[6], &states[2], nullptr, 0, 0, -1, (TransitionType) 0, 2},
    {&symbols[8], &states[4], nullptr, 0, 0, -1, (TransitionType) 0, 3},
    {&symbols[1], nullptr, &symbols[0], 1, 0, -1, (TransitionType) 1, 4},
    {&symbols[3], &states[11], nullptr, 0, 0, -1, (TransitionType) 0, 5},
    {&symbols[7], &states[3], nullptr, 0, 0, -1, (TransitionType) 0, 6},
    {&symbols[1], nullptr, &symbols[5], 2, 0, 0, (TransitionType) 1, 7},
    {&symbols[9], &states[5], nullptr, 0, 0, -1, (TransitionType) 0, 8},
    {&symbols[10], nullptr, &symbols[9], 0, 2, -1, (TransitionType) 1, 9},
    {&symbols[14], &states[19], nullptr, 0, 0, -1, (TransitionType) 0, 10},
    {&symbols[16], &states[20], nullptr, 0, 0, -1, (TransitionType) 0, 11},
    {&symbols[10], &states[6], nullptr, 0, 0, -1, (TransitionType) 0, 12},
    {&symbols[14], &states[18], nullptr, 0, 0, -1, (TransitionType) 0, 13},
    {&symbols[16], &states[20], nullptr, 0, 0, -1, (TransitionType) 0, 14},
    {&symbols[3], nullptr, &symbols[6], 3, 0, -1, (TransitionType) 1, 15},
    {&symbols[3], &states[11], nullptr, 0, 0, -1, (TransitionType) 0, 16},
    {&symbols[7], &states[10], nullptr, 0, 0, -1, (TransitionType) 0, 17},
    {&symbols[11], &states[8], nullptr, 0, 0, -1, (TransitionType) 0, 18},
    {&symbols[13], nullptr, &symbols[11], 0, 1, -1, (TransitionType) 1, 19},
    {&symbols[3], &states[11], nullptr, 0, 0, -1, (TransitionType) 0, 20},
    {&symbols[7], &states[9], nullptr, 0, 0, -1, (TransitionType) 0, 21},
    {&symbols[13], &states[15], nullptr, 0, 0, -1, (TransitionType) 0, 22},
    {&symbols[3], nullptr, &symbols[11], 2, 0, 1, (TransitionType) 1, 23},
    {&symbols[13], nullptr, &symbols[11], 2, 0, 1, (TransitionType) 1, 24},
    {&symbols[3], nullptr, &symbols[11], 1, 0, 2, (TransitionType) 1, 25},
    {&symbols[13], nullptr, &symbols[11], 1, 0, 2, (TransitionType) 1, 26},
    {&symbols[16], &states[12], nullptr, 0, 0, -1, (TransitionType) 0, 27},
    {&symbols[4], nullptr, &symbols[9], 0, 2, -1, (TransitionType) 1, 28},
    {&symbols[9], &states[13], nullptr, 0, 0, -1, (TransitionType) 0, 29},
    {&symbols[12], nullptr, &symbols[9], 0, 2, -1, (TransitionType) 1, 30},
    {&symbols[14], &states[19], nullptr, 0, 0, -1, (TransitionType) 0, 31},
    {&symbols[16], &states[20], nullptr, 0, 0, -1, (TransitionType) 0, 32},
    {&symbols[4], &states[7], nullptr, 0, 0, -1, (TransitionType) 0, 33},
    {&symbols[12], &states[14], nullptr, 0, 0, -1, (TransitionType) 0, 34},
    {&symbols[14], &states[18], nullptr, 0, 0, -1, (TransitionType) 0, 35},
    {&symbols[16], &states[20], nullptr, 0, 0, -1, (TransitionType) 0, 36},
    {&symbols[1], nullptr, &symbols[7], 4, 0, 3, (TransitionType) 1, 37},
    {&symbols[3], nullptr, &symbols[7], 4, 0, 3, (TransitionType) 1, 38},
    {&symbols[13], nullptr, &symbols[7], 4, 0, 3, (TransitionType) 1, 39},
    {&symbols[16], &states[16], nullptr, 0, 0, -1, (TransitionType) 0, 40},
    {&symbols[4], &states[17], nullptr, 0, 0, -1, (TransitionType) 0, 41},
    {&symbols[1], nullptr, &symbols[7], 8, 1, 4, (TransitionType) 1, 42},
    {&symbols[3], nullptr, &symbols[7], 8, 1, 4, (TransitionType) 1, 43},
    {&symbols[13], nullptr, &symbols[7], 8, 1, 4, (TransitionType) 1, 44},
    {&symbols[4], nullptr, &symbols[9], 2, 0, 5, (TransitionType) 1, 45},
    {&symbols[10], nullptr, &symbols[9], 2, 0, 5, (TransitionType) 1, 46},
    {&symbols[12], nullptr, &symbols[9], 2, 0, 5, (TransitionType) 1, 47},
    {&symbols[16], nullptr, &symbols[9], 2, 0, 5, (TransitionType) 1, 48},
    {&symbols[4], nullptr, &symbols[9], 1, 0, 6, (TransitionType) 1, 49},
    {&symbols[10], nullptr, &symbols[9], 1, 0, 6, (TransitionType) 1, 50},
    {&symbols[12], nullptr, &symbols[9], 1, 0, 6, (TransitionType) 1, 51},
    {&symbols[16], nullptr, &symbols[9], 1, 0, 6, (TransitionType) 1, 52},
    {&symbols[15], &states[21], nullptr, 0, 0, -1, (TransitionType) 0, 53},
    {&symbols[17], &states[22], nullptr, 0, 0, -1, (TransitionType) 0, 54},
    {&symbols[4], nullptr, &symbols[14], 3, 0, 7, (TransitionType) 1, 55},
    {&symbols[10], nullptr, &symbols[14], 3, 0, 7, (TransitionType) 1, 56},
    {&symbols[12], nullptr, &symbols[14], 3, 0, 7, (TransitionType) 1, 57},
    {&symbols[16], nullptr, &symbols[14], 3, 0, 7, (TransitionType) 1, 58},
    {nullptr, nullptr, nullptr, 0, 0, 0, (TransitionType) 0, -1}
};

const ParserState states [] = 
{
    {0, 4, &transitions[0]},
    {1, 1, &transitions[4]},
    {2, 2, &transitions[5]},
    {3, 1, &transitions[7]},
    {4, 4, &transitions[8]},
    {5, 3, &transitions[12]},
    {6, 1, &transitions[15]},
    {7, 4, &transitions[16]},
    {8, 3, &transitions[20]},
    {9, 2, &transitions[23]},
    {10, 2, &transitions[25]},
    {11, 1, &transitions[27]},
    {12, 5, &transitions[28]},
    {13, 4, &transitions[33]},
    {14, 3, &transitions[37]},
    {15, 1, &transitions[40]},
    {16, 1, &transitions[41]},
    {17, 3, &transitions[42]},
    {18, 4, &transitions[45]},
    {19, 4, &transitions[49]},
    {20, 1, &transitions[53]},
    {21, 1, &transitions[54]},
    {22, 4, &transitions[55]},
    {-1, 0, nullptr}
};

const LexerAction lexer_actions [] = 
{
    {0, "string"},
    {-1, nullptr}
};

const LexerTransition lexer_transitions [] = 
{
    {34, 35, &lexer_states[20], nullptr},
    {39, 40, &lexer_states[20], nullptr},
    {47, 48, &lexer_states[13], nullptr},
    {58, 59, &lexer_states[16], nullptr},
    {60, 61, &lexer_states[6], nullptr},
    {61, 62, &lexer_states[17], nullptr},
    {62, 63, &lexer_states[11], nullptr},
    {63, 64, &lexer_states[14], nullptr},
    {65, 91, &lexer_states[16], nullptr},
    {95, 96, &lexer_states[16], nullptr},
    {97, 101, &lexer_states[16], nullptr},
    {101, 102, &lexer_states[1], nullptr},
    {102, 123, &lexer_states[16], nullptr},
    {45, 47, &lexer_states[16], nullptr},
    {48, 59, &lexer_states[16], nullptr},
    {65, 91, &lexer_states[16], nullptr},
    {95, 96, &lexer_states[16], nullptr},
    {97, 114, &lexer_states[16], nullptr},
    {114, 115, &lexer_states[2], nullptr},
    {115, 123, &lexer_states[16], nullptr},
    {45, 47, &lexer_states[16], nullptr},
    {48, 59, &lexer_states[16], nullptr},
    {65, 91, &lexer_states[16], nullptr},
    {95, 96, &lexer_states[16], nullptr},
    {97, 114, &lexer_states[16], nullptr},
    {114, 115, &lexer_states[3], nullptr},
    {115, 123, &lexer_states[16], nullptr},
    {45, 47, &lexer_states[16], nullptr},
    {48, 59, &lexer_states[16], nullptr},
    {65, 91, &lexer_states[16], nullptr},
    {95, 96, &lexer_states[16], nullptr},
    {97, 111, &lexer_states[16], nullptr},
    {111, 112, &lexer_states[4], nullptr},
    {112, 123, &lexer_states[16], nullptr},
    {45, 47, &lexer_states[16], nullptr},
    {48, 59, &lexer_states[16], nullptr},
    {65, 91, &lexer_states[16], nullptr},
    {95, 96, &lexer_states[16], nullptr},
    {97, 114, &lexer_states[16], nullptr},
    {114, 115, &lexer_states[5], nullptr},
    {115, 123, &lexer_states[16], nullptr},
    {45, 47, &lexer_states[16], nullptr},
    {48, 59, &lexer_states[16], nullptr},
    {65, 91, &lexer_states[16], nullptr},
    {95, 96, &lexer_states[16], nullptr},
    {97, 123, &lexer_states[16], nullptr},
    {47, 48, &lexer_states[12], nullptr},
    {63, 64, &lexer_states[10], nullptr},
    {109, 110, &lexer_states[8], nullptr},
    {108, 109, &lexer_states[9], nullptr},
    {120, 121, &lexer_states[7], nullptr},
    {62, 63, &lexer_states[15], nullptr},
    {62, 63, &lexer_states[18], nullptr},
    {45, 47, &lexer_states[16], nullptr},
    {48, 59, &lexer_states[16], nullptr},
    {65, 91, &lexer_states[16], nullptr},
    {95, 96, &lexer_states[16], nullptr},
    {97, 123, &lexer_states[16], nullptr},
    {0, 2147483647, &lexer_states[19], &lexer_actions[0]},
    {-1, -1, nullptr, nullptr}
};

const LexerState lexer_states [] = 
{
    {0, 13, &lexer_transitions[0], nullptr},
    {1, 7, &lexer_transitions[13], &symbols[16]},
    {2, 7, &lexer_transitions[20], &symbols[16]},
    {3, 7, &lexer_transitions[27], &symbols[16]},
    {4, 7, &lexer_transitions[34], &symbols[16]},
    {5, 5, &lexer_transitions[41], &symbols[2]},
    {6, 2, &lexer_transitions[46], &symbols[3]},
    {7, 1, &lexer_transitions[48], nullptr},
    {8, 1, &lexer_transitions[49], nullptr},
    {9, 0, &lexer_transitions[50], &symbols[8]},
    {10, 1, &lexer_transitions[50], nullptr},
    {11, 0, &lexer_transitions[51], &symbols[4]},
    {12, 0, &lexer_transitions[51], &symbols[13]},
    {13, 1, &lexer_transitions[51], nullptr},
    {14, 1, &lexer_transitions[52], nullptr},
    {15, 0, &lexer_transitions[53], &symbols[12]},
    {16, 5, &lexer_transitions[53], &symbols[16]},
    {17, 0, &lexer_transitions[58], &symbols[15]},
    {18, 0, &lexer_transitions[58], &symbols[10]},
    {19, 0, &lexer_transitions[58], &symbols[17]},
    {20, 1, &lexer_transitions[58], nullptr},
    {-1, 0, nullptr, nullptr}
};

const LexerStateMachine lexer_state_machine = 
{
    1, // #actions
    59, // #transitions
    21, // #states
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
    "xml",
    8, // #actions
    18, // #symbols
    59, // #transitions
    23, // #states
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

const ParserStateMachine* xml_parser_state_machine = &parser_state_machine;

