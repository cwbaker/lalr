//
// Grammar.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "Grammar.hpp"
#include "ParserGrammar.hpp"
#include "ParserAction.hpp"
#include "ParserSymbol.hpp"
#include <sweet/assert/assert.hpp>

using std::string;
using std::vector;
using std::shared_ptr;
using namespace sweet::parser;

regex::regex( const char* text )
: text_( text )
{
    SWEET_ASSERT( text_ );
}

const char* regex::text() const
{
    return text_;
}

GrammarHelper::GrammarHelper( Grammar* grammar )
: grammar_( grammar )
{
    SWEET_ASSERT( grammar_ );
}

const AssociativityGrammarHelper GrammarHelper::left() const
{
    return grammar_->left();
}

const AssociativityGrammarHelper GrammarHelper::right() const
{
    return grammar_->right();
}

const AssociativityGrammarHelper GrammarHelper::none() const
{
    return grammar_->none();
}

const WhitespaceGrammarHelper GrammarHelper::whitespace() const
{
    return grammar_->whitespace();
}

const ProductionGrammarHelper GrammarHelper::production( const char* id ) const
{
    SWEET_ASSERT( id );
    return grammar_->production( id );
}

ParserAction* GrammarHelper::find_or_create_action( const char* id ) const
{
    SWEET_ASSERT( id );
    return grammar_->find_or_create_action( id );
}

ParserSymbol* GrammarHelper::find_or_create_symbol( const char* id ) const
{
    SWEET_ASSERT( id );
    return grammar_->find_or_create_symbol( id );
}

AssociativityGrammarHelper::AssociativityGrammarHelper( Grammar* grammar, ParserGrammar* parser_grammar, SymbolAssociativity associativity, int precedence )
: GrammarHelper( grammar ),
  parser_grammar_( parser_grammar ),
  associativity_( associativity ),
  precedence_( precedence )
{
    SWEET_ASSERT( parser_grammar_ );
    SWEET_ASSERT( associativity_ > ASSOCIATE_NULL && associativity_ <= ASSOCIATE_RIGHT );
}

const AssociativityGrammarHelper& AssociativityGrammarHelper::operator()( char lexeme ) const
{
    (void) lexeme;
    char id[2] = { lexeme, 0 };
    ParserSymbol* symbol = find_or_create_symbol( id );
    SWEET_ASSERT( symbol );
    symbol->set_associativity( associativity_ );
    symbol->set_precedence( precedence_ );
    return *this;
}

const AssociativityGrammarHelper& AssociativityGrammarHelper::operator()( const char* id ) const
{
    SWEET_ASSERT( id );
    ParserSymbol* symbol = find_or_create_symbol( id );
    SWEET_ASSERT( symbol );
    symbol->set_associativity( associativity_ );
    symbol->set_precedence( precedence_ );
    return *this;
}

WhitespaceGrammarHelper::WhitespaceGrammarHelper( Grammar* grammar, ParserGrammar* parser_grammar )
: GrammarHelper( grammar ),
  parser_grammar_( parser_grammar )
{
    SWEET_ASSERT( parser_grammar_ );
}

const WhitespaceGrammarHelper& WhitespaceGrammarHelper::operator()( char lexeme ) const
{
    (void) lexeme;
    return *this;
}

const WhitespaceGrammarHelper& WhitespaceGrammarHelper::operator()( const char* literal ) const
{
    SWEET_ASSERT( literal );
    return *this;
}

const WhitespaceGrammarHelper& WhitespaceGrammarHelper::operator()( const regex& regex ) const
{
    ParserSymbol* symbol = grammar_->find_or_create_symbol( regex.text() );
    SWEET_ASSERT( symbol );
    symbol->set_lexeme_type( SYMBOL_LEXEME_REGULAR_EXPRESSION );
    parser_grammar_->symbol( symbol );
    return *this;
}

ProductionGrammarHelper::ProductionGrammarHelper( Grammar* grammar, ParserGrammar* parser_grammar, const char* id )
: grammar_( grammar ),
  parser_grammar_( parser_grammar ),
  symbol_( nullptr ),
  production_created_( false )
{
    SWEET_ASSERT( grammar_ );
    SWEET_ASSERT( parser_grammar_ );
    SWEET_ASSERT( id );
    symbol_ = grammar_->find_or_create_symbol( id );
}

const ProductionGrammarHelper& ProductionGrammarHelper::operator()( char lexeme ) const
{
    if ( !production_created_ )
    {
        parser_grammar_->begin_production( symbol_, 0 );
        production_created_ = true;
    }

    char id [2] = { lexeme, 0 };
    ParserSymbol* symbol = grammar_->find_or_create_symbol( id );
    SWEET_ASSERT( symbol );
    parser_grammar_->symbol( symbol );
    return *this;
}

const ProductionGrammarHelper& ProductionGrammarHelper::operator()( const char* id ) const
{
    if ( !production_created_ )
    {
        parser_grammar_->begin_production( symbol_, 0 );
        production_created_ = true;
    }

    ParserSymbol* symbol = grammar_->find_or_create_symbol( id );
    SWEET_ASSERT( symbol );
    parser_grammar_->symbol( symbol );
    return *this;
}

const ProductionGrammarHelper& ProductionGrammarHelper::operator()( const regex& regex ) const
{
    if ( !production_created_ )
    {
        parser_grammar_->begin_production( symbol_, 0 );
        production_created_ = true;
    }

    ParserSymbol* symbol = grammar_->find_or_create_symbol( regex.text() );
    SWEET_ASSERT( symbol );
    symbol->set_lexeme_type( SYMBOL_LEXEME_REGULAR_EXPRESSION );
    parser_grammar_->symbol( symbol );
    return *this;
}

const ProductionGrammarHelper& ProductionGrammarHelper::operator[]( const char* id ) const
{
    if ( id )
    {
        ParserAction* action = grammar_->find_or_create_action( id );
        SWEET_ASSERT( action );
        parser_grammar_->action( action );
    }
    parser_grammar_->end_production();
    production_created_ = false;
    return *this;
}

Grammar& ProductionGrammarHelper::end_production() const
{
    return *grammar_;
}

Grammar::Grammar()
: parser_grammar_( nullptr ),
  actions_(),
  symbols_(),
  precedence_( 1 )
{
    parser_grammar_ = new ParserGrammar;
}

Grammar::~Grammar()
{
    delete parser_grammar_;
}

ParserGrammar& Grammar::parser_grammar() const
{
    return *parser_grammar_;
}

const AssociativityGrammarHelper Grammar::left()
{
    int precedence = precedence_;
    ++precedence_;
    return AssociativityGrammarHelper( this, parser_grammar_, ASSOCIATE_LEFT, precedence );
}

const AssociativityGrammarHelper Grammar::right()
{
    int precedence = precedence_;
    ++precedence_;
    return AssociativityGrammarHelper( this, parser_grammar_, ASSOCIATE_RIGHT, precedence );
}

const AssociativityGrammarHelper Grammar::none()
{
    int precedence = precedence_;
    ++precedence_;
    return AssociativityGrammarHelper( this, parser_grammar_, ASSOCIATE_NONE, precedence );
}

const WhitespaceGrammarHelper Grammar::whitespace()
{
    return WhitespaceGrammarHelper( this, parser_grammar_ );
}

const ProductionGrammarHelper Grammar::production( const char* id )
{
    return ProductionGrammarHelper( this, parser_grammar_, id );
}

ParserAction* Grammar::find_or_create_action( const char* id )
{
    vector<ParserAction*>::const_iterator i = actions_.begin();
    while ( i != actions_.end() && (*i)->get_identifier() != id )
    {
        ++i;
    }
    if ( i == actions_.end() )
    {
        ParserAction* action = parser_grammar_->add_action( string(id) );
        actions_.push_back( action );
        return action;
    }
    return *i;
}

ParserSymbol* Grammar::find_or_create_symbol( const char* id )
{
    vector<ParserSymbol*>::const_iterator i = symbols_.begin();
    // while ( i != symbols_.end() && (*i)->get_identifier() != id )
    while ( i != symbols_.end() && (*i)->get_lexeme() != id )
    {
        ++i;
    }
    if ( i == symbols_.end() )
    {
        ParserSymbol* symbol = parser_grammar_->add_symbol( SYMBOL_NULL, string(id), 0 );
        symbols_.push_back( symbol );
        return symbol;
    }
    return *i;
}
