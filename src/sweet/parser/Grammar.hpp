#ifndef SWEET_PARSER_GRAMMAR_HPP_INCLUDED
#define SWEET_PARSER_GRAMMAR_HPP_INCLUDED

#include "GrammarSymbolType.hpp"
#include "SymbolAssociativity.hpp"
#include "GrammarNil.hpp"
#include "LexerToken.hpp"
#include <vector>

namespace sweet
{

namespace parser
{

class GrammarBuilder;
class GrammarDirective;
class GrammarSymbol;
class GrammarProduction;
class GrammarAction;
class GrammarNil;

class Grammar
{
    std::vector<std::shared_ptr<GrammarDirective>> directives_; ///< The directives in the grammar.
    std::vector<std::shared_ptr<GrammarSymbol>> symbols_; ///< The symbols in the grammar.
    std::vector<std::shared_ptr<GrammarProduction>> productions_; ///< The productions in the grammar.
    std::vector<std::shared_ptr<GrammarAction>> actions_; ///< The actions in the grammar.
    std::vector<lexer::LexerToken> whitespace_tokens_;
    bool active_whitespace_directive_;
    GrammarDirective* active_directive_;
    GrammarProduction* active_production_;
    GrammarSymbol* active_symbol_;

public:
    Grammar();
    const std::vector<std::shared_ptr<GrammarDirective>>& directives() const;
    const std::vector<std::shared_ptr<GrammarSymbol>>& symbols() const;
    const std::vector<std::shared_ptr<GrammarProduction>>& productions() const;
    const std::vector<lexer::LexerToken>& whitespace_tokens() const;
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

private:
    GrammarDirective* directive( SymbolAssociativity associativity );
    GrammarSymbol* symbol( char literal );
    GrammarSymbol* symbol( const char* regex );
    GrammarSymbol* symbol( const char* regex, GrammarSymbolType type );
    GrammarSymbol* error_symbol();
    GrammarProduction* production( GrammarSymbol* symbol );
    GrammarAction* action( const char* id );
};

}

}

#endif
