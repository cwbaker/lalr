#ifndef SWEET_LALR_GRAMMAR_HPP_INCLUDED
#define SWEET_LALR_GRAMMAR_HPP_INCLUDED

#include "SymbolType.hpp"
#include "LexemeType.hpp"
#include "Associativity.hpp"
#include "Nil.hpp"
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
class Nil;
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
    Grammar( );
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
    Grammar& left();
    Grammar& right();
    Grammar& none();
    Grammar& whitespace();
    Grammar& precedence( char literal );
    Grammar& precedence( const char* regex );
    Grammar& precedence();
    Grammar& production( const char* identifier );
    Grammar& end_production();
    Grammar& operator()( char literal );
    Grammar& operator()( const char* regex );
    Grammar& operator()( const Nil& /*nil*/ );
    Grammar& error();
    Grammar& operator[]( const char* identifier );
    Grammar& operator[]( const Nil& /*nil*/ );
    Grammar& literal( const char* literal );
    Grammar& regex( const char* regex );
    Grammar& identifier( const char* identifier );    
    void generate( ParserStateMachine* state_machine, ParserErrorPolicy* parser_error_policy = nullptr, LexerErrorPolicy* lexer_error_policy = nullptr );

private:
    Directive* directive( Associativity associativity );
    Symbol* symbol( char literal );
    Symbol* symbol( const char* regex );
    Symbol* symbol( const char* lexeme, LexemeType lexeme_type, SymbolType symbol_type );
    Production* production( Symbol* symbol );
    Action* action( const char* id );
};

}

}

#endif
