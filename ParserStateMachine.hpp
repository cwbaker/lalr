#ifndef SWEET_PARSER_PARSERSTATEMACHINE_HPP_INCLUDED
#define SWEET_PARSER_PARSERSTATEMACHINE_HPP_INCLUDED

#include <memory>
#include <vector>
#include <string>

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
    std::unique_ptr<ParserAction[]> allocated_actions_; ///< The parser actions for this ParserStateMachine.
    std::unique_ptr<ParserSymbol[]> allocated_symbols_; ///< The symbols in the grammar for this ParserStateMachine.
    std::unique_ptr<ParserTransition[]> allocated_transitions_; ///< The transitions in the state machine for this ParserStateMachine.
    std::unique_ptr<ParserState[]> allocated_states_; ///< The states in the state machine for this ParserStateMachine.
    int actions_size_;
    int symbols_size_;
    int transitions_size_;
    int states_size_;
    const ParserAction* actions_; ///< The parser actions for this ParserStateMachine.
    const ParserSymbol* symbols_; ///< The symbols in the grammar for this ParserStateMachine.
    const ParserTransition* transitions_; ///< The transitions in the state machine for this ParserStateMachine.
    const ParserState* states_; ///< The states in the state machine for this ParserStateMachine.
    const ParserSymbol* start_symbol_; ///< The start symbol.
    const ParserSymbol* end_symbol_; ///< The end symbol.
    const ParserSymbol* error_symbol_; ///< The error symbol.
    const ParserState* start_state_; ///< The start state.
    std::shared_ptr<LexerStateMachine> lexer_state_machine_; ///< The LexerStateMachine that are used for lexical analysis in this ParserStateMachine.

    public:
        ParserStateMachine();
        ~ParserStateMachine();
        int actions_size() const;
        int symbols_size() const;
        int transitions_size() const;
        int states_size() const;
        const ParserAction* actions() const;
        const ParserSymbol* symbols() const;
        const ParserState* states() const;
        const ParserSymbol* start_symbol() const;
        const ParserSymbol* end_symbol() const;
        const ParserSymbol* error_symbol() const;
        const ParserState* start_state() const;
        const LexerStateMachine* lexer_state_machine() const;
        const ParserSymbol* find_symbol_by_identifier( const char* identifier ) const;
        void set_actions( std::unique_ptr<ParserAction[]>& actions, int actions_size );
        void set_symbols( std::unique_ptr<ParserSymbol[]>& symbols, int symbols_size );
        void set_transitions( std::unique_ptr<ParserTransition[]>& transitions, int transitions_size );
        void set_states( std::unique_ptr<ParserState[]>& states, int states_size, const ParserState* start_state );
        void set_lexer_state_machine( const std::shared_ptr<LexerStateMachine>& lexer_state_machine );
};

}

}

#endif
