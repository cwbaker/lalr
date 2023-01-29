
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
    {-1, nullptr}
};

const ParserSymbol symbols [] = 
{
    {0, "dot_start", ".start", (SymbolType) 2},
    {1, "dot_end", ".end", (SymbolType) 3},
    {2, "error", "error", (SymbolType) 1},
    {3, "unit", "unit", (SymbolType) 2},
    {4, "one", "1", (SymbolType) 1},
    {5, "two", "2", (SymbolType) 1},
    {-1, nullptr, nullptr, (SymbolType) 0}
};

const ParserTransition transitions [] = 
{
    {&symbols[3], &states[1], nullptr, 0, 0, -1, (TransitionType) 0, 0},
    {&symbols[4], &states[2], nullptr, 0, 0, -1, (TransitionType) 0, 1},
    {&symbols[5], &states[3], nullptr, 0, 0, -1, (TransitionType) 0, 2},
    {&symbols[1], nullptr, &symbols[0], 1, 0, -1, (TransitionType) 1, 3},
    {&symbols[1], nullptr, &symbols[3], 1, 0, -1, (TransitionType) 1, 4},
    {&symbols[1], nullptr, &symbols[3], 1, 0, -1, (TransitionType) 1, 5},
    {nullptr, nullptr, nullptr, 0, 0, 0, (TransitionType) 0, -1}
};

const ParserState states [] = 
{
    {0, 3, &transitions[0]},
    {1, 1, &transitions[3]},
    {2, 1, &transitions[4]},
    {3, 1, &transitions[5]},
    {-1, 0, nullptr}
};

const LexerAction lexer_actions [] = 
{
    {-1, nullptr}
};

const LexerTransition lexer_transitions [] = 
{
    {49, 50, &lexer_states[6], nullptr},
    {50, 51, &lexer_states[7], nullptr},
    {101, 102, &lexer_states[1], nullptr},
    {114, 115, &lexer_states[2], nullptr},
    {114, 115, &lexer_states[3], nullptr},
    {111, 112, &lexer_states[4], nullptr},
    {114, 115, &lexer_states[5], nullptr},
    {-1, -1, nullptr, nullptr}
};

const LexerState lexer_states [] = 
{
    {0, 3, &lexer_transitions[0], nullptr},
    {1, 1, &lexer_transitions[3], nullptr},
    {2, 1, &lexer_transitions[4], nullptr},
    {3, 1, &lexer_transitions[5], nullptr},
    {4, 1, &lexer_transitions[6], nullptr},
    {5, 0, &lexer_transitions[7], &symbols[2]},
    {6, 0, &lexer_transitions[7], &symbols[4]},
    {7, 0, &lexer_transitions[7], &symbols[5]},
    {-1, 0, nullptr, nullptr}
};

const LexerStateMachine lexer_state_machine = 
{
    0, // #actions
    7, // #transitions
    8, // #states
    lexer_actions, // actions
    lexer_transitions, // transitions
    lexer_states, // states
    &lexer_states[0] // start state
};

const ParserStateMachine parser_state_machine = 
{
    "or",
    0, // #actions
    6, // #symbols
    6, // #transitions
    4, // #states
    actions,
    symbols,
    transitions,
    states,
    &symbols[0], // start symbol
    &symbols[1], // end symbol
    &symbols[2], // error symbol
    &states[0], // start state
    &lexer_state_machine, // lexer state machine
    null // whitespace lexer state machine
};

}

const lalr::ParserStateMachine* or_parser_state_machine = &parser_state_machine;

