#ifndef LALR_LEXER_HPP_INCLUDED
#define LALR_LEXER_HPP_INCLUDED

#include "PositionIterator.hpp"
#include <vector>
#include <functional>

namespace lalr
{

class ErrorPolicy;
class LexerAction;
class LexerTransition;
class LexerState;
class LexerStateMachine;

/**
// A lexical analyzer.
*/
template <class Iterator, class Char = typename std::iterator_traits<Iterator>::value_type, class Traits = typename std::char_traits<Char>, class Allocator = typename std::allocator<Char> >
class Lexer
{
    typedef std::function<PositionIterator<Iterator> (const PositionIterator<Iterator>& begin, const PositionIterator<Iterator>& end, std::basic_string<Char, Traits, Allocator>* lexeme, const void** symbol)> LexerActionFunction;

    struct LexerActionHandler
    {
        const LexerAction* action_;
        LexerActionFunction function_;
        LexerActionHandler( const LexerAction* action, LexerActionFunction function );
    };

    const LexerStateMachine* state_machine_; ///< The state machine for this lexer.
    const LexerStateMachine* whitespace_state_machine_; ///< The whitespace state machine for this lexer.
    const void* end_symbol_; ///< The value to return to indicate that the end of the input has been reached.
    ErrorPolicy* error_policy_; ///< The error policy this lexer uses to report errors and debug information.
    std::vector<LexerActionHandler> action_handlers_; ///< The action handlers for this Lexer.
    std::vector<LexerActionHandler> whitespace_action_handlers_; ///< The action handlers for this Lexer.
    PositionIterator<Iterator> position_; ///< The current position of this Lexer in its input sequence.
    Iterator end_; ///< One past the last position of the input sequence for this Lexer.
    std::basic_string<Char, Traits, Allocator> lexeme_; ///< The most recently matched lexeme.
    int line_; ///< The line number at the start of the most recently matched lexeme.
    int column_; ///< The column number at the start of the most recently matched lexeme.
    const void* symbol_; ///< The most recently matched symbol or null if no symbol has been matched.
    bool full_; ///< True when this Lexer scanned all of its input otherwise false.
    PositionIterator<Iterator> matched_position_; ///< The position at the end of the most recent match.
    std::basic_string<Char, Traits, Allocator> matched_lexeme_; ///< The lexeme at the most recent match.
    const void* matched_symbol_; ///< The symbol at the most recent match.

public:
    Lexer( const LexerStateMachine* state_machine, const LexerStateMachine* whitespace_state_machine = nullptr, const void* end_symbol = nullptr, ErrorPolicy* error_policy = nullptr );
    const std::basic_string<Char, Traits, Allocator>& lexeme() const;
    int line() const;
    int column() const;
    const void* symbol() const;
    const Iterator& position() const;
    bool full() const;
    void set_action_handler( const char* identifier, LexerActionFunction function );
    void reset( Iterator start, Iterator finish );
    void advance();
    
private:
    void skip();
    const void* run();
    void error();
    void fire_error( int line, int column, int error, const char* format, ... ) const;
    const LexerTransition* find_transition_by_character( const LexerState* state, int character ) const;
};

}

#include "Lexer.ipp"

#endif
