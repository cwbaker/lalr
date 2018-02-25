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
class LalrSymbol;
class LalrProduction;
class Action;
class Nil;
class ParserErrorPolicy;
class LexerErrorPolicy;
class ParserStateMachine;

class Grammar
{
    std::string identifier_;
    std::vector<std::unique_ptr<Directive>> directives_; ///< The directives in the grammar.
    std::vector<std::unique_ptr<LalrSymbol>> symbols_; ///< The symbols in the grammar.
    std::vector<std::unique_ptr<LalrProduction>> productions_; ///< The productions in the grammar.
    std::vector<std::unique_ptr<Action>> actions_; ///< The actions in the grammar.
    std::vector<LexerToken> whitespace_tokens_;
    bool active_whitespace_directive_;
    Directive* active_directive_;
    LalrProduction* active_production_;
    LalrSymbol* active_symbol_;
    LalrSymbol* start_symbol_; ///< The start symbol.
    LalrSymbol* end_symbol_; ///< The end symbol.
    LalrSymbol* error_symbol_; ///< The error symbol.

public:
    Grammar( size_t directives_reserve = 64, size_t symbols_reserve = 64, size_t productions_reserve = 64, size_t actions_reserve = 32, size_t whitespace_tokens_reserve = 8 );
    ~Grammar();
    const std::string& identifier() const;
    std::vector<std::unique_ptr<Directive>>& directives();
    std::vector<std::unique_ptr<LalrSymbol>>& symbols();
    std::vector<std::unique_ptr<LalrProduction>>& productions();
    std::vector<std::unique_ptr<Action>>& actions();
    const std::vector<LexerToken>& whitespace_tokens() const;
    LalrSymbol* start_symbol() const;
    LalrSymbol* end_symbol() const;
    LalrSymbol* error_symbol() const;
    Grammar& left();
    Grammar& right();
    Grammar& none();
    Grammar& whitespace();
    Grammar& precedence( char literal );
    Grammar& precedence( const char* regex );
    Grammar& production( const char* identifier );
    Grammar& end_production();
    Grammar& operator()( char literal );
    Grammar& operator()( const char* regex );
    Grammar& operator()( const Nil& /*nil*/ );
    Grammar& error();
    Grammar& operator[]( const char* identifier );
    Grammar& operator[]( const Nil& /*nil*/ );
    void generate( ParserStateMachine* state_machine, ParserErrorPolicy* parser_error_policy = nullptr, LexerErrorPolicy* lexer_error_policy = nullptr );

private:
    Directive* directive( Associativity associativity );
    LalrSymbol* symbol( char literal );
    LalrSymbol* symbol( const char* regex );
    LalrSymbol* symbol( const char* lexeme, LexemeType lexeme_type, SymbolType symbol_type );
    LalrProduction* production( LalrSymbol* symbol );
    Action* action( const char* id );
};

}

}

#endif
