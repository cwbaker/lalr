#ifndef SWEET_PARSER_GRAMMAR_HPP_INCLUDED
#define SWEET_PARSER_GRAMMAR_HPP_INCLUDED

#include <sweet/parser/ParserSymbol.hpp>
#include <vector>

namespace sweet
{

namespace parser
{

class AssociativityGrammarHelper;
class WhitespaceGrammarHelper;
class ProductionGrammarHelper;
class ParserGrammar;
class ParserAction;
class ParserSymbol;
class Grammar;

class regex
{
    const char* text_;

public:
    regex( const char* regex );
    const char* text() const;
};

class GrammarHelper
{
    Grammar* grammar_;

public:
    GrammarHelper( Grammar* grammar );
    const AssociativityGrammarHelper left() const;
    const AssociativityGrammarHelper right() const;
    const AssociativityGrammarHelper none() const;
    const WhitespaceGrammarHelper whitespace() const;
    const ProductionGrammarHelper production( const char* id ) const;    
    ParserAction* find_or_create_action( const char* id ) const;
    ParserSymbol* find_or_create_symbol( const char* id ) const;
};

class AssociativityGrammarHelper : public GrammarHelper
{
    ParserGrammar* parser_grammar_;
    SymbolAssociativity associativity_;
    int precedence_;

public:
    AssociativityGrammarHelper( Grammar* grammar, ParserGrammar* parser_grammar, SymbolAssociativity associativity, int precedence );
    const AssociativityGrammarHelper& operator()( char lexeme ) const;
    const AssociativityGrammarHelper& operator()( const char* id ) const;
};

class WhitespaceGrammarHelper : public GrammarHelper
{
    ParserGrammar* parser_grammar_;

public:
    WhitespaceGrammarHelper( Grammar* grammar, ParserGrammar* parser_grammar );
    const WhitespaceGrammarHelper& operator()( char lexeme ) const;
    const WhitespaceGrammarHelper& operator()( const char* literal ) const;
    const WhitespaceGrammarHelper& operator()( const regex& regex ) const;
};

class ProductionGrammarHelper
{
    Grammar* grammar_;
    ParserGrammar* parser_grammar_;
    ParserSymbol* symbol_;
    mutable bool production_created_;

public:
    ProductionGrammarHelper( Grammar* grammar, ParserGrammar* parser_grammar, const char* id );
    const ProductionGrammarHelper& precedence() const;
    const ProductionGrammarHelper& operator()( char lexeme ) const;
    const ProductionGrammarHelper& operator()( const char* id ) const;
    const ProductionGrammarHelper& operator()( const regex& regex ) const;
    const ProductionGrammarHelper& operator[]( const char* id ) const;
    Grammar& end_production() const;
};

class Grammar
{
    ParserGrammar* parser_grammar_; ///< The underlying ParserGrammar that is built up.
    std::vector<ParserAction*> actions_; ///< The actions in the grammar.
    std::vector<ParserSymbol*> symbols_; ///< The symbols in the grammar.
    int precedence_;

public:
    Grammar();
    ~Grammar();
    ParserGrammar& parser_grammar() const;
    const AssociativityGrammarHelper left();
    const AssociativityGrammarHelper right();
    const AssociativityGrammarHelper none();
    const WhitespaceGrammarHelper whitespace();
    const ProductionGrammarHelper production( const char* id );

    ParserAction* find_or_create_action( const char* id );
    ParserSymbol* find_or_create_symbol( const char* id );
};

}

}

#endif
