#ifndef LALR_GRAMMAR_HPP_INCLUDED
#define LALR_GRAMMAR_HPP_INCLUDED

#include "SymbolType.hpp"
#include "LexemeType.hpp"
#include "Associativity.hpp"
#include "RegexToken.hpp"
#include <vector>
#include <memory>

namespace lalr
{

class GrammarSymbol;
class GrammarProduction;
class GrammarAction;
class LexerErrorPolicy;
class RegexCompiler;
class ParserErrorPolicy;
class GrammarCompiler;
class ParserStateMachine;

class Grammar
{
    std::string identifier_;
    std::vector<std::unique_ptr<GrammarSymbol>> symbols_; ///< The symbols in the grammar.
    std::vector<std::unique_ptr<GrammarProduction>> productions_; ///< The productions in the grammar.
    std::vector<std::unique_ptr<GrammarAction>> actions_; ///< The actions in the grammar.
    std::vector<RegexToken> whitespace_tokens_; ///< Regular expressions that define whitespace in this grammar.
    bool active_whitespace_directive_; ///< True iff a whitespace directive is active.
    bool active_precedence_directive_; ///< True iff a precedence directive is active.
    Associativity associativity_; ///< Most recently set associativity.
    int precedence_; ///< Current precedence.
    GrammarProduction* active_production_; ///< Currently active production.
    GrammarSymbol* active_symbol_; ///< Currently active symbol.
    GrammarSymbol* start_symbol_; ///< The start symbol.
    GrammarSymbol* end_symbol_; ///< The end symbol.
    GrammarSymbol* error_symbol_; ///< The error symbol.
    GrammarSymbol* whitespace_symbol_; ///< The whitespace symbol.

public:
    Grammar();
    ~Grammar();
    const std::string& identifier() const;
    std::vector<std::unique_ptr<GrammarSymbol>>& symbols();
    std::vector<std::unique_ptr<GrammarProduction>>& productions();
    std::vector<std::unique_ptr<GrammarAction>>& actions();
    const std::vector<RegexToken>& whitespace_tokens() const;
    GrammarSymbol* start_symbol() const;
    GrammarSymbol* end_symbol() const;
    GrammarSymbol* error_symbol() const;
    GrammarSymbol* whitespace_symbol() const;
    Grammar& grammar( const std::string& identifier );
    Grammar& left( int line );
    Grammar& right( int line );
    Grammar& none( int line );
    Grammar& whitespace();
    Grammar& precedence();
    Grammar& production( const char* identifier, int line );
    Grammar& end_production();
    Grammar& end_expression( int line );
    Grammar& error( int line );
    Grammar& action( const char* identifier, int line );
    Grammar& literal( const char* literal, int line );
    Grammar& regex( const char* regex, int line );
    Grammar& identifier( const char* identifier, int line );

private:
    GrammarSymbol* literal_symbol( const char* lexeme, int line );
    GrammarSymbol* regex_symbol( const char* lexeme, int line );
    GrammarSymbol* non_terminal_symbol( const char* lexeme, int line );
    GrammarSymbol* add_symbol( const char* lexeme, int line, LexemeType lexeme_type, SymbolType symbol_type );
    GrammarProduction* add_production( GrammarSymbol* symbol, int line );
    GrammarAction* add_action( const char* id );
};

}

#endif
