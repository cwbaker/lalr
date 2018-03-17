#ifndef SWEET_LALR_PARSERSTATEMACHINE_HPP_INCLUDED
#define SWEET_LALR_PARSERSTATEMACHINE_HPP_INCLUDED

namespace sweet
{

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
    int actions_size;
    int symbols_size;
    int transitions_size;
    int states_size;
    const ParserAction* actions; ///< The parser actions for this ParserStateMachine.
    const ParserSymbol* symbols; ///< The symbols in the grammar for this ParserStateMachine.
    const ParserTransition* transitions; ///< The transitions in the state machine for this ParserStateMachine.
    const ParserState* states; ///< The states in the state machine for this ParserStateMachine.
    const ParserSymbol* start_symbol; ///< The start symbol.
    const ParserSymbol* end_symbol; ///< The end symbol.
    const ParserSymbol* error_symbol; ///< The error symbol.
    const ParserState* start_state; ///< The start state.
    const LexerStateMachine* lexer_state_machine; ///< The state machine used by the lexer to match tokens
    const LexerStateMachine* whitespace_lexer_state_machine; ///< The state machine used by the lexer to skip whitespace

    const ParserSymbol* find_symbol_by_identifier( const char* identifier ) const;
};

}

}

#endif
