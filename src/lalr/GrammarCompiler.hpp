#ifndef LALR_GRAMMARCOMPILER_HPP_INCLUDED
#define LALR_GRAMMARCOMPILER_HPP_INCLUDED

#include <deque>
#include <string>
#include <memory>

namespace lalr
{

class ErrorPolicy;
class Grammar;
class GrammarState;
class GrammarItem;
class GrammarGenerator;
class ParserAction;
class ParserSymbol;
class ParserTransition;
class ParserState;
class ParserStateMachine;
class RegexCompiler;

class GrammarCompiler
{    
    std::deque<std::string> strings_;
    std::unique_ptr<ParserAction[]> actions_; ///< The parser actions for this ParserStateMachine.
    std::unique_ptr<ParserSymbol[]> symbols_; ///< The symbols in the grammar for this ParserStateMachine.
    std::unique_ptr<ParserTransition[]> transitions_; ///< The transitions in the state machine for this ParserStateMachine.
    std::unique_ptr<ParserState[]> states_; ///< The states in the state machine for this ParserStateMachine.
    std::unique_ptr<RegexCompiler> lexer_; ///< Allocated lexer state machine.
    std::unique_ptr<RegexCompiler> whitespace_lexer_; ///< Allocated whitespace lexer state machine.
    std::unique_ptr<ParserStateMachine> parser_state_machine_; ///< Allocated parser state machine.
    bool labels_enabled_; ///< True to generate labels for states and symbols.

public:
    GrammarCompiler();
    ~GrammarCompiler();
    const RegexCompiler* lexer() const;
    const RegexCompiler* whitespace_lexer() const;
    const ParserStateMachine* parser_state_machine() const;
    void labels_enabled( bool enabled );
    int compile( const char* begin, const char* end, ErrorPolicy* error_policy = nullptr, bool genEBNF = false, bool genYACC = false, bool genYACC_HTML = false );
    void showStats();

private:
    const char* add_string( const std::string& string );
    void set_actions( std::unique_ptr<ParserAction[]>& actions, int actions_size );
    void set_symbols( std::unique_ptr<ParserSymbol[]>& symbols, int symbols_size );
    void set_transitions( std::unique_ptr<ParserTransition[]>& transitions, int transitions_size );
    void set_states( std::unique_ptr<ParserState[]>& states, int states_size, const ParserState* start_state );
    void populate_parser_state_machine( const Grammar& grammar, const GrammarGenerator& generator );
};

}

#endif
