#ifndef SWEET_LALR_LALRGRAMMAR_HPP_INCLUDED
#define SWEET_LALR_LALRGRAMMAR_HPP_INCLUDED

#include "LalrSymbol.hpp"
#include "LexerToken.hpp"
#include <memory>
#include <vector>
#include <string>

namespace sweet
{

namespace lalr
{

class LalrAction;
class LalrProduction;
class LalrGenerator;
class GrammarSymbol;

/**
// A grammar used to generate a state machine for a %parser.
*/
class LalrGrammar
{
    std::string identifier_; ///< The identifier of the grammar.
    std::vector<std::unique_ptr<LalrAction>> actions_; ///< The actions in the grammar.
    std::vector<std::unique_ptr<LalrProduction>> productions_; ///< The productions in the grammar.
    std::vector<std::unique_ptr<LalrSymbol>> symbols_; ///< The symbols in the grammar.
    std::vector<LexerToken> whitespace_tokens_; ///< The tokens skipped as whitespace by the lexer.
    LalrSymbol* start_symbol_; ///< The start symbol.
    LalrSymbol* end_symbol_; ///< The end symbol.
    LalrSymbol* error_symbol_; ///< The error symbol.

public:
    LalrGrammar( size_t actions_reserve = 32, size_t productions_reserve = 64, size_t symbols_reserve = 64, size_t whitespace_tokens_reserve = 8 );
    ~LalrGrammar();

    std::string& identifier();
    std::vector<std::unique_ptr<LalrAction>>& actions();
    std::vector<std::unique_ptr<LalrProduction>>& productions();
    std::vector<std::unique_ptr<LalrSymbol>>& symbols();
    const std::vector<LexerToken>& whitespace_tokens() const;
    LalrSymbol* start_symbol() const;
    LalrSymbol* end_symbol() const;
    LalrSymbol* error_symbol() const;

    LalrSymbol* make_symbol( const LalrSymbol* symbol );
    LalrSymbol* symbol( SymbolType type, const std::string& identifier, int line );
    LalrSymbol* terminal( const std::string& identifier, int line );
    LalrSymbol* non_terminal( const std::string& identifier, int line );
    LalrAction* action( const std::string& identifier );
    void whitespace_tokens( const std::vector<LexerToken>& whitespace_tokens );

    LalrSymbol* add_symbol( SymbolType type, const std::string& identifier, int line );
    LalrSymbol* add_terminal( const std::string& identifier, int line );
    LalrSymbol* add_non_terminal( const std::string& identifier, int line );
    LalrAction* add_action( const std::string& identifier );

    void identifier( const std::string& identifier );
    void begin_production( LalrSymbol* symbol, int line );
    void end_production();
    void symbol( LalrSymbol* symbol );
    void action( LalrAction* action );
    void precedence_symbol( LalrSymbol* symbol );
};

}

}

#endif