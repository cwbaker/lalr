#ifndef LALR_PARSERSTATEMACHINE_HPP_INCLUDED
#define LALR_PARSERSTATEMACHINE_HPP_INCLUDED

namespace lalr
{

class LexerStateMachine;
class LexerErrorPolicy;

}

namespace lalr
{

class ParserAction;
class ParserSymbol;
class ParserTransition;
class ParserState;

/**
// The data that defines the state machine for a %parser.
*/
class ParserStateMachine
{
public:
    const char* identifier; ///< The identifier of this parser.
    int actions_size; ///< The number of actions.
    int symbols_size; ///< The number of symbols.
    int transitions_size; ///< The total number of transitions.
    int states_size; ///< The number of states.
    int shift_reduce_count; ///< The number of shift/reduce resolved conflicts that occured during parsing and generation.
    int reduce_reduce_count; ///< The number of reduce/reduce resolved conflicts that occured during parsing and generation.
    bool error_recovery_off; ///< For debug purposes turn off error recovery
    bool error_recovery_show; ///< For debug purposes show error recovery as it happens
    const ParserAction* actions; ///< The parser actions for this ParserStateMachine.
    const ParserSymbol* symbols; ///< The symbols in the grammar for this ParserStateMachine.
    const ParserTransition* transitions; ///< The transitions in the state machine for this ParserStateMachine.
    const ParserState* states; ///< The states in the state machine for this ParserStateMachine.
    const ParserSymbol* start_symbol; ///< The start symbol.
    const ParserSymbol* end_symbol; ///< The end symbol.
    const ParserSymbol* error_symbol; ///< The error symbol.
    const ParserSymbol* whitespace_symbol; ///< The whitespace symbol.
    const ParserState* start_state; ///< The start state.
    const LexerStateMachine* lexer_state_machine; ///< The state machine used by the lexer to match tokens
    const LexerStateMachine* whitespace_lexer_state_machine; ///< The state machine used by the lexer to skip whitespace
};

}

#endif
