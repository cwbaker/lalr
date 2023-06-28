//
// Grammar.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "Grammar.hpp"
#include "GrammarParser.hpp"
#include "GrammarSymbol.hpp"
#include "GrammarProduction.hpp"
#include "GrammarAction.hpp"
#include "GrammarGenerator.hpp"
#include "GrammarCompiler.hpp"
#include "ParserStateMachine.hpp"
#include "ParserSymbol.hpp"
#include "RegexCompiler.hpp"
#include "RegexTokenType.hpp"
#include "assert.hpp"

using std::string;
using std::vector;
using std::set;
using std::unique_ptr;
using namespace lalr;

Grammar::Grammar()
: identifier_()
, symbols_()
, productions_()
, actions_()
, whitespace_tokens_()
, active_whitespace_directive_( false )
, active_precedence_directive_( false )
, active_case_insensitive_(false)
, associativity_( ASSOCIATE_NULL )
, precedence_( 0 )
, active_production_( nullptr )
, active_symbol_( nullptr )
, start_symbol_( nullptr )
, end_symbol_( nullptr )
, error_symbol_( nullptr )
, whitespace_symbol_( nullptr )
{
    start_symbol_ = add_symbol( ".start", 0, 0, LEXEME_NULL, SYMBOL_NON_TERMINAL );
    end_symbol_ = add_symbol( ".end", 0, 0, LEXEME_NULL, SYMBOL_END );
    error_symbol_ = add_symbol( "error", 0, 0, LEXEME_NULL, SYMBOL_NULL );
    whitespace_symbol_ = add_symbol( ".whitespace", 0, 0, LEXEME_NULL, SYMBOL_NULL );
}

Grammar::~Grammar()
{
}

Grammar& Grammar::grammar( const std::string& identifier )
{
    identifier_ = identifier;
    return *this;
}

Grammar& Grammar::left( int /*line*/ )
{
    associativity_ = ASSOCIATE_LEFT;
    ++precedence_;
    active_whitespace_directive_ = false;
    active_precedence_directive_ = false;
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::right( int /*line*/ )
{
    associativity_ = ASSOCIATE_RIGHT;
    ++precedence_;
    active_whitespace_directive_ = false;
    active_precedence_directive_ = false;
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::none( int /*line*/ )
{
    associativity_ = ASSOCIATE_NONE;
    ++precedence_;
    active_whitespace_directive_ = false;
    active_precedence_directive_ = false;
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::whitespace()
{
    associativity_ = ASSOCIATE_NULL;
    active_whitespace_directive_ = true;
    active_precedence_directive_ = false;
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::assoc_prec( int /*line*/ )
{
    associativity_ = ASSOCIATE_PREC;
    ++precedence_;
    active_whitespace_directive_ = false;
    active_precedence_directive_ = false;
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::case_insensitive()
{
    active_case_insensitive_ = true;
    return *this;
}

Grammar& Grammar::precedence()
{
    LALR_ASSERT( active_symbol_ );
    if ( active_symbol_ )
    {
        active_precedence_directive_ = true;
    }
    return *this;
}

Grammar& Grammar::production( const char* identifier, int line, int column )
{
    LALR_ASSERT( identifier );
    associativity_ = ASSOCIATE_NULL;
    active_whitespace_directive_ = false;
    active_precedence_directive_ = false;
    active_production_ = nullptr;
    active_symbol_ = non_terminal_symbol(identifier, line , column);
    return *this;
}

Grammar& Grammar::end_production()
{
    LALR_ASSERT( active_symbol_ );
    associativity_ = ASSOCIATE_NULL;
    active_whitespace_directive_ = false;
    active_precedence_directive_ = false;
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::end_expression(int line, int column)
{
    LALR_ASSERT( line >= 1 );
    // If there is an active symbol but no active production then an empty
    // production is being specified (the nil action marks the end of a
    // production for which no symbols have been specified).
    if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = add_production( active_symbol_, line , column);
        }
    }
    active_production_ = nullptr;
    return *this;
}

Grammar& Grammar::error(int line, int column)
{
    LALR_ASSERT( line >= 1 );
    if ( associativity_ != ASSOCIATE_NULL )
    {
        GrammarSymbol* symbol = error_symbol();
        symbol->set_associativity( associativity_ );
        symbol->set_precedence( precedence_ );
    }
    else if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = add_production( active_symbol_, line , column );
        }
        active_production_->append_symbol( error_symbol() );
    }
    return *this;
}

Grammar& Grammar::action(const char* identifier, int line , int column)
{
    LALR_ASSERT( identifier );
    LALR_ASSERT( line >= 1 );
    LALR_ASSERT( active_symbol_ );
    if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = add_production(active_symbol_, line , column);
        }
        active_production_->set_action( add_action(identifier) );
        active_production_ = nullptr;
    }
    return *this;
}

Grammar& Grammar::literal(const char* literal, int line , int column)
{
    LALR_ASSERT( literal );
    LALR_ASSERT( line >= 0 );
    LALR_ASSERT( active_whitespace_directive_ || associativity_ != ASSOCIATE_NULL || active_symbol_ );
    if ( active_whitespace_directive_ )
    {
        whitespace_tokens_.emplace_back( TOKEN_LITERAL, 0, 0, whitespace_symbol_, literal );
    }
    else if ( associativity_ != ASSOCIATE_NULL )
    {
        GrammarSymbol* symbol = literal_symbol(literal, line , column);
        symbol->set_associativity( associativity_ );
        symbol->set_precedence( precedence_ );
    }
    else if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = add_production( active_symbol_, line, column );
        }
        if ( active_precedence_directive_ )
        {
            GrammarSymbol* symbol = literal_symbol(literal, line , column);
            active_production_->set_precedence_symbol( symbol );
            symbol->set_referenced_in_precedence_directive( true );
            active_precedence_directive_ = false;
        }
        else
        {
            active_production_->append_symbol( literal_symbol(literal, line, column) );
        }
    }
    return *this;
}

Grammar& Grammar::regex( const char* regex, int line, int column )
{
    LALR_ASSERT( regex );
    LALR_ASSERT( line >= 0 );
    LALR_ASSERT( active_whitespace_directive_ || associativity_ != ASSOCIATE_NULL || active_symbol_ );
    if ( active_whitespace_directive_ )
    {
        whitespace_tokens_.emplace_back( TOKEN_REGULAR_EXPRESSION, 0, 0, whitespace_symbol_, regex );
    }
    else if ( associativity_ != ASSOCIATE_NULL )
    {
        GrammarSymbol* symbol = regex_symbol(regex, line , column);
        symbol->set_associativity( associativity_ );
        symbol->set_precedence( precedence_ );
    }
    else if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = add_production(active_symbol_, line , column);
        }
        if ( active_precedence_directive_ )
        {
            GrammarSymbol* symbol = regex_symbol(regex, line , column);
            symbol->set_referenced_in_precedence_directive( true );
            active_production_->set_precedence_symbol( symbol );
            active_precedence_directive_ = false;
        }
        else
        {
            active_production_->append_symbol( regex_symbol(regex, line , column) );
        }
    }
    return *this;
}

Grammar& Grammar::identifier( const char* identifier, int line, int column )
{
    LALR_ASSERT( identifier );
    LALR_ASSERT( line >= 0 );
    LALR_ASSERT( active_symbol_ || associativity_ != ASSOCIATE_NULL );
    if ( associativity_ != ASSOCIATE_NULL )
    {
        GrammarSymbol* symbol = non_terminal_symbol(identifier, line , column);
        symbol->set_associativity( associativity_ );
        symbol->set_precedence( precedence_ );
    }
    else if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = add_production(active_symbol_, line , column);
        }
        if ( active_precedence_directive_ )
        {
            GrammarSymbol* symbol = non_terminal_symbol(identifier, line , column);
            symbol->set_referenced_in_precedence_directive( true );
            active_production_->set_precedence_symbol( symbol );
            active_precedence_directive_ = false;
        }
        else
        {
            GrammarSymbol* symbol = non_terminal_symbol(identifier, line , column);
            symbol->set_referenced_in_rule(true);
            active_production_->append_symbol( symbol );
        }
    }
    return *this;
}

static void ouptputTerminal(const GrammarSymbol *sym)
{
    if(sym->symbol_type() == SYMBOL_TERMINAL) {
        if(sym->lexeme_type() == LEXEME_LITERAL)
            printf("'%s'", sym->lexeme().c_str());
        else
            printf("\"%s\"", sym->lexeme().c_str());
    }
    else
        printf("%s", sym->lexeme().c_str());
}

static bool isTerminalRegex(const GrammarSymbol *symbol)
{
    return symbol->symbol_type() == SymbolType::SYMBOL_TERMINAL
            && symbol->lexeme_type() == LexemeType::LEXEME_REGULAR_EXPRESSION;
}

void Grammar::genEBNF()
{
    printf(
            "//\n"
            "// EBNF to be viewd at https://www.bottlecaps.de/rr/ui\n"
            "//\n"
            "// Copy and paste this at https://www.bottlecaps.de/rr/ui in the 'Edit Grammar' tab \n"
            "// then click the 'View Diagram' tab.\n"
            "//\n"
            );
    GrammarSymbol* last_production_symbol = NULL;
    bool production_continuation = false;
    for ( vector<unique_ptr<GrammarProduction>>::const_iterator i = productions().begin(); i != productions().end(); ++i )
    {
        GrammarProduction* production = i->get();
        LALR_ASSERT( production );
        GrammarSymbol *curr_symbol = production->symbol();
        bool same_production = last_production_symbol && last_production_symbol == curr_symbol;
        const char *prefix = (isTerminalRegex(curr_symbol) 
                    || (production->length() == 1 && isTerminalRegex(production->symbol_by_position(0)))) 
                    ? "//" : "";
        if(same_production) {
            production_continuation = true;
            //printf(" //%s ::= %d", production->symbol()->lexeme().c_str(), production->length());
        }
        else {
            production_continuation = false;
            const char *sym_prefix = "";
            const char *sym_name = curr_symbol->lexeme().c_str();
            if(sym_name[0] == '.')
            {
                ++sym_name;
                sym_prefix = "ebnf_x_";
            }
            //printf("\n\n%s%s ::= // %d\n\t", sym_prefix, sym_name, production->length());
            printf("\n\n%s%s%s ::=\n%s\t", prefix, sym_prefix, sym_name, prefix);
        }
        if(production->length() > 0) {
            for(int elm=0; elm < production->length(); ++elm) {
                const GrammarSymbol *sym = production->symbol_by_position(elm);
                if(production_continuation) {
                    production_continuation = false;
                    printf("\n%s\t| ", prefix);
                }
                else printf(" ");
                ouptputTerminal(sym);
            }
        }
        else {
            printf("/*empty*/");
        }
        if(production->precedence_symbol()) {
            printf(" //%%precedence ");
            ouptputTerminal(production->precedence_symbol());
        }
        last_production_symbol = curr_symbol;
    }
    printf("\n\n// end EBNF\n");
}

void Grammar::genNakedGrammar()
{
    printf( "%s {\n", identifier_.c_str());
    GrammarSymbol* last_production_symbol = NULL;
    bool production_continuation = false;
    for ( vector<unique_ptr<GrammarProduction>>::const_iterator i = productions().begin(); i != productions().end(); ++i )
    {
        GrammarProduction* production = i->get();
        LALR_ASSERT( production );
        GrammarSymbol *curr_symbol = production->symbol();
        bool same_production = last_production_symbol && last_production_symbol == curr_symbol;
        if(same_production) {
            production_continuation = true;
            //printf(" //%s ::= %d", production->symbol()->lexeme().c_str(), production->length());
        }
        else {
            production_continuation = false;
            const char *sym_prefix = "";
            const char *sym_name = curr_symbol->lexeme().c_str();
            if(sym_name[0] == '.')
            {
                ++sym_name;
                sym_prefix = "ebnf_x_";
            }
            //printf("\n\n%s%s ::= // %d\n\t", sym_prefix, sym_name, production->length());
            printf("\n\t;\n\n%s%s :\n\t", sym_prefix, sym_name);
        }
        if(production->length() > 0) {
            for(int elm=0; elm < production->length(); ++elm) {
                const GrammarSymbol *sym = production->symbol_by_position(elm);
                if(production_continuation) {
                    production_continuation = false;
                    printf("\n\t| ");
                }
                else printf(" ");
                ouptputTerminal(sym);
            }
        }
        else {
            printf("/*empty*/");
        }
        if(production->precedence_symbol()) {
            printf(" %%prec %s", production->precedence_symbol()->lexeme().c_str());
        }
        last_production_symbol = curr_symbol;
    }
    printf("\n\t;\n\n}\n");
}

GrammarSymbol* Grammar::literal_symbol( const char* lexeme, int line , int column)
{
    LALR_ASSERT( lexeme );
    LALR_ASSERT( line >= 0 );
    return add_symbol(lexeme, line, column, LEXEME_LITERAL, SYMBOL_TERMINAL );
}

GrammarSymbol* Grammar::regex_symbol(const char* lexeme, int line , int column)
{
    LALR_ASSERT( lexeme );
    LALR_ASSERT( line >= 0 );
    return add_symbol(lexeme, line, column, LEXEME_REGULAR_EXPRESSION, SYMBOL_TERMINAL );
}

GrammarSymbol* Grammar::non_terminal_symbol(const char* lexeme, int line , int column)
{
    LALR_ASSERT( lexeme );
    LALR_ASSERT( line >= 0 );
    return add_symbol(lexeme, line, column, LEXEME_NULL, SYMBOL_NON_TERMINAL );
}

GrammarSymbol* Grammar::add_symbol(const char* lexeme, int line, int column, LexemeType lexeme_type, SymbolType symbol_type )
{
    LALR_ASSERT( lexeme );
    LALR_ASSERT( line >= 0 );
    vector<unique_ptr<GrammarSymbol>>::const_iterator i = symbols_.begin();
    while ( i != symbols_.end() && !(*i)->matches(lexeme, symbol_type) )
    {
        ++i;
    }
    if ( i == symbols_.end() )
    {
        unique_ptr<GrammarSymbol> &symbol = symbols_.emplace_back( new GrammarSymbol(lexeme) );
        symbol->set_line( line );
        symbol->set_column( column );
        symbol->set_lexeme_type( lexeme_type );
        symbol->set_symbol_type( symbol_type );
        return symbol.get();
    }

    GrammarSymbol* symbol = i->get();
    LALR_ASSERT( symbol );
    LALR_ASSERT( symbol->symbol_type() == symbol_type );
    return symbol;
}

GrammarProduction* Grammar::add_production(GrammarSymbol* symbol, int line , int column)
{
    LALR_ASSERT( symbol );
    LALR_ASSERT( line > 0 );
    if ( productions_.empty() )
    {
        LALR_ASSERT( start_symbol_ );
        unique_ptr<GrammarProduction> &production = productions_.emplace_back( new GrammarProduction(int(productions_.size()), start_symbol_, 0, 0, NULL) );
        production->append_symbol( symbol );
        start_symbol_->append_production( production.get() );
    }

    unique_ptr<GrammarProduction> &production = productions_.emplace_back( new GrammarProduction(int(productions_.size()), symbol, line, column, nullptr) );
    symbol->append_production( production.get() );
    return production.get();
}

GrammarAction* Grammar::add_action( const char* identifier )
{
    LALR_ASSERT( identifier );
    vector<unique_ptr<GrammarAction>>::const_iterator i = actions_.begin();
    while ( i != actions_.end() && (*i)->identifier() != identifier )
    {
        ++i;
    }
    if ( i == actions_.end() )
    {
        int index = int(actions_.size());
        unique_ptr<GrammarAction> &action = actions_.emplace_back( new GrammarAction(index, identifier) );
        return action.get();
    }
    return i->get();
}
