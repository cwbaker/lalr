#ifndef SWEET_LALR_GRAMMAR_HPP_INCLUDED
#define SWEET_LALR_GRAMMAR_HPP_INCLUDED

#include "SymbolType.hpp"
#include "LexemeType.hpp"
#include "Associativity.hpp"
#include "LexerToken.hpp"
#include <vector>

namespace sweet
{

namespace lalr
{

class Directive;
class Symbol;
class Production;
class Action;
class ParserErrorPolicy;
class LexerErrorPolicy;
class ParserStateMachine;

class Grammar
{
    std::string identifier_;
    std::vector<std::unique_ptr<Directive>> directives_; ///< The directives in the grammar.
    std::vector<std::unique_ptr<Symbol>> symbols_; ///< The symbols in the grammar.
    std::vector<std::unique_ptr<Production>> productions_; ///< The productions in the grammar.
    std::vector<std::unique_ptr<Action>> actions_; ///< The actions in the grammar.
    std::vector<LexerToken> whitespace_tokens_;
    bool active_whitespace_directive_;
    bool active_precedence_directive_;
    Directive* active_directive_;
    Production* active_production_;
    Symbol* active_symbol_;
    Symbol* start_symbol_; ///< The start symbol.
    Symbol* end_symbol_; ///< The end symbol.
    Symbol* error_symbol_; ///< The error symbol.

public:
    Grammar();
    ~Grammar();
    const std::string& identifier() const;
    std::vector<std::unique_ptr<Directive>>& directives();
    std::vector<std::unique_ptr<Symbol>>& symbols();
    std::vector<std::unique_ptr<Production>>& productions();
    std::vector<std::unique_ptr<Action>>& actions();
    const std::vector<LexerToken>& whitespace_tokens() const;
    Symbol* start_symbol() const;
    Symbol* end_symbol() const;
    Symbol* error_symbol() const;
    Grammar& left( int line );
    Grammar& right( int line );
    Grammar& none( int line );
    Grammar& whitespace();
    Grammar& precedence();
    Grammar& production( const char* identifier, int line );
    Grammar& end_production();
    Grammar& end_expression();
    Grammar& error();
    Grammar& action( const char* identifier );
    Grammar& literal( const char* literal, int line );
    Grammar& regex( const char* regex, int line );
    Grammar& identifier( const char* identifier, int line );
    bool parse( const char* begin, const char* end );
    bool generate( ParserStateMachine* state_machine, ParserErrorPolicy* parser_error_policy = nullptr, LexerErrorPolicy* lexer_error_policy = nullptr );

private:
    Directive* directive( Associativity associativity );
    Symbol* literal_symbol( const char* lexeme, int line );
    Symbol* regex_symbol( const char* lexeme, int line );
    Symbol* non_terminal_symbol( const char* lexeme, int line );
    Symbol* add_symbol( const char* lexeme, int line, LexemeType lexeme_type, SymbolType symbol_type );
    Production* add_production( Symbol* symbol );
    Action* add_action( const char* id );
};

}

}

#endif
