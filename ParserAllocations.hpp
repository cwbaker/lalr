#ifndef SWEET_LALR_PARSERALLOCATIONS_HPP_INCLUDED
#define SWEET_LALR_PARSERALLOCATIONS_HPP_INCLUDED

#include <deque>
#include <string>
#include <memory>

namespace sweet
{

namespace lalr
{

class ParserAction;
class ParserSymbol;
class ParserTransition;
class ParserState;
class ParserStateMachine;
class LexerStateMachine;

class ParserAllocations
{    
    std::deque<std::string> strings_;
    std::unique_ptr<ParserAction[]> actions_; ///< The parser actions for this ParserStateMachine.
    std::unique_ptr<ParserSymbol[]> symbols_; ///< The symbols in the grammar for this ParserStateMachine.
    std::unique_ptr<ParserTransition[]> transitions_; ///< The transitions in the state machine for this ParserStateMachine.
    std::unique_ptr<ParserState[]> states_; ///< The states in the state machine for this ParserStateMachine.
    std::unique_ptr<LexerStateMachine> lexer_state_machine_; ///< Allocated lexer state machine.
    std::unique_ptr<LexerStateMachine> whitespace_lexer_state_machine_; ///< Allocated whitespace lexer state machine.
    std::unique_ptr<ParserStateMachine> parser_state_machine_; ///< Allocated parser state machine.

public:
    ParserAllocations();
    ~ParserAllocations();
    const LexerStateMachine* lexer_state_machine() const;
    const LexerStateMachine* whitespace_lexer_state_machine() const;
    const ParserStateMachine* parser_state_machine() const;
    const char* add_string( const std::string& string );
    void set_actions( std::unique_ptr<ParserAction[]>& actions, int actions_size );
    void set_symbols( std::unique_ptr<ParserSymbol[]>& symbols, int symbols_size );
    void set_transitions( std::unique_ptr<ParserTransition[]>& transitions, int transitions_size );
    void set_states( std::unique_ptr<ParserState[]>& states, int states_size, const ParserState* start_state );
    void set_lexer_state_machine( std::unique_ptr<LexerStateMachine>& lexer_state_machine );
    void set_whitespace_lexer_state_machine( std::unique_ptr<LexerStateMachine>& whitespace_lexer_state_machine );
};

}

}

#endif
