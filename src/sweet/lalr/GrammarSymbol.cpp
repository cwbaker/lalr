//
// GrammarSymbol.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "GrammarSymbol.hpp"
#include "GrammarProduction.hpp"
#include "assert.hpp"

using std::shared_ptr;
using namespace sweet::parser;

GrammarSymbol::GrammarSymbol( const char* lexeme )
: lexeme_( lexeme ),
  identifier_(),
  type_( GRAMMAR_SYMBOL_NULL ),
  associativity_( ASSOCIATE_NONE ),
  precedence_( 0 ),
  productions_()
{
}

const std::string& GrammarSymbol::lexeme() const
{
    return lexeme_;
}

const std::string& GrammarSymbol::identifier() const
{
    return identifier_;
}

GrammarSymbolType GrammarSymbol::type() const
{
    return type_;
}

SymbolAssociativity GrammarSymbol::associativity() const
{
    return associativity_;
}

int GrammarSymbol::precedence() const
{
    return precedence_;
}

const std::vector<GrammarProduction*>& GrammarSymbol::productions() const
{
    return productions_;
}

void GrammarSymbol::set_lexeme( const std::string& lexeme )
{
    lexeme_ = lexeme;
}

void GrammarSymbol::set_identifier( const std::string& identifier )
{
    identifier_ = identifier;
}

void GrammarSymbol::set_type( GrammarSymbolType type )
{
    SWEET_ASSERT( type >= GRAMMAR_SYMBOL_NULL && type <= GRAMMAR_NON_TERMINAL );
    type_ = type;
}

void GrammarSymbol::set_associativity( SymbolAssociativity associativity )
{
    SWEET_ASSERT( associativity >= ASSOCIATE_NONE && associativity <= ASSOCIATE_RIGHT );
    associativity_ = associativity;
}

void GrammarSymbol::set_precedence( int precedence )
{
    SWEET_ASSERT( precedence > 0 );
    precedence_ = precedence;
}

void GrammarSymbol::append_production( GrammarProduction* production )
{
    SWEET_ASSERT( production );
    productions_.push_back( production );
}
