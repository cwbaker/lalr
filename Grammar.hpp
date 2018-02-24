#ifndef SWEET_LALR_GRAMMAR_HPP_INCLUDED
#define SWEET_LALR_GRAMMAR_HPP_INCLUDED

#include "SymbolType.hpp"
#include "LexemeType.hpp"
#include "Associativity.hpp"
#include "GrammarNil.hpp"
#include "LexerToken.hpp"
#include <vector>

namespace sweet
{

namespace lalr
{

class GrammarBuilder;
class GrammarDirective;
class LalrSymbol;
class LalrProduction;
class LalrAction;
class GrammarNil;
class ParserErrorPolicy;
class LexerErrorPolicy;
class ParserStateMachine;

class Grammar
{
    std::string identifier_;
    std::vector<std::unique_ptr<GrammarDirective>> directives_; ///< The directives in the grammar.
    std::vector<std::unique_ptr<LalrSymbol>> symbols_; ///< The symbols in the grammar.
    std::vector<std::unique_ptr<LalrProduction>> productions_; ///< The productions in the grammar.
    std::vector<std::unique_ptr<LalrAction>> actions_; ///< The actions in the grammar.
    std::vector<LexerToken> whitespace_tokens_;
    bool active_whitespace_directive_;
    GrammarDirective* active_directive_;
    LalrProduction* active_production_;
    LalrSymbol* active_symbol_;

public:
    Grammar();
    ~Grammar();
    const std::vector<std::unique_ptr<GrammarDirective>>& directives() const;
    const std::vector<std::unique_ptr<LalrSymbol>>& symbols() const;
    const std::vector<std::unique_ptr<LalrProduction>>& productions() const;
    const std::vector<std::unique_ptr<LalrAction>>& actions() const;
    const std::vector<LexerToken>& whitespace_tokens() const;
    Grammar& begin();
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
    Grammar& operator()( const GrammarNil& /*nil*/ );
    Grammar& error();
    Grammar& operator[]( const char* identifier );
    Grammar& operator[]( const GrammarNil& /*nil*/ );
    void end();
    void generate( ParserStateMachine* state_machine, ParserErrorPolicy* parser_error_policy = nullptr, LexerErrorPolicy* lexer_error_policy = nullptr );

private:
    GrammarDirective* directive( Associativity associativity );
    LalrSymbol* symbol( char literal );
    LalrSymbol* symbol( const char* regex );
    LalrSymbol* symbol( const char* lexeme, LexemeType lexeme_type, SymbolType symbol_type );
    LalrSymbol* error_symbol();
    LalrProduction* production( LalrSymbol* symbol );
    LalrAction* action( const char* id );
};

}

}

#endif
