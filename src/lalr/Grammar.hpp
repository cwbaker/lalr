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
    bool active_case_insensitive_; ///< True iff a case insensitive directive is active.
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
    const std::string& identifier() const { return identifier_;}
    std::vector<std::unique_ptr<GrammarSymbol>>& symbols() { return symbols_;}
    std::vector<std::unique_ptr<GrammarProduction>>& productions() { return productions_;}
    std::vector<std::unique_ptr<GrammarAction>>& actions() { return actions_;}
    const std::vector<RegexToken>& whitespace_tokens() const { return whitespace_tokens_;}
    GrammarSymbol* start_symbol() const { return start_symbol_;}
    GrammarSymbol* end_symbol() const { return end_symbol_;}
    GrammarSymbol* error_symbol() const { return error_symbol_;}
    GrammarSymbol* whitespace_symbol() const { return whitespace_symbol_;}
    Grammar& grammar( const std::string& identifier );
    Grammar& left( int line );
    Grammar& right( int line );
    Grammar& none( int line );
    Grammar& assoc_prec( int line );
    Grammar& whitespace();
    Grammar& case_insensitive();
    Grammar& precedence();
    Grammar& production( const char* identifier, int line, int column );
    Grammar& end_production();
    Grammar& end_expression( int line, int column);
    Grammar& error( int line, int column );
    Grammar& action( const char* identifier, int line, int column );
    Grammar& literal( const char* literal, int line, int column );
    Grammar& regex( const char* regex, int line, int column );
    Grammar& identifier( const char* identifier, int line, int column );
    bool is_case_insensitive() const {return active_case_insensitive_;}
    void genEBNF();
    void genNakedGrammar();

private:
    GrammarSymbol* literal_symbol(const char* lexeme, int line , int column);
    GrammarSymbol* regex_symbol(const char* lexeme, int line , int column);
    GrammarSymbol* non_terminal_symbol(const char* lexeme, int line , int column);
    GrammarSymbol* add_symbol(const char* lexeme, int line, int column, LexemeType lexeme_type, SymbolType symbol_type );
    GrammarProduction* add_production(GrammarSymbol* symbol, int line , int column);
    GrammarAction* add_action( const char* id );
};

}

#endif
