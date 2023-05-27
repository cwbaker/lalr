//
// GrammarSymbol.ipp
// Copyright (c) Charles Baker. All rights reserved.
//
#pragma once

#include "GrammarSymbol.hpp"
#include "GrammarProduction.hpp"
#include "assert.hpp"
#include <memory>

namespace lalr
{

const std::string& GrammarSymbol::lexeme() const
{
    return lexeme_;
}

const std::string& GrammarSymbol::identifier() const
{
    return identifier_;
}

SymbolType GrammarSymbol::symbol_type() const
{
    return symbol_type_;
}

LexemeType GrammarSymbol::lexeme_type() const
{
    return lexeme_type_;
}

bool GrammarSymbol::literal() const
{
    return lexeme_type_ == LEXEME_LITERAL;
}

Associativity GrammarSymbol::associativity() const
{
    return associativity_;
}

int GrammarSymbol::precedence() const
{
    return precedence_;
}

int GrammarSymbol::line() const
{
    return line_;
}

int GrammarSymbol::index() const
{
    return index_;
}

bool GrammarSymbol::nullable() const
{
    return nullable_;
}

bool GrammarSymbol::referenced_in_precedence_directive() const
{
    return referenced_in_precedence_directive_;
}

const GrammarSymbolSet& GrammarSymbol::first() const
{
    return first_;
}

const GrammarSymbolSet& GrammarSymbol::follow() const
{
    return follow_;
}

const std::vector<GrammarProduction*>& GrammarSymbol::productions() const
{
    return productions_;
}

const std::multimap<const GrammarSymbol*, GrammarProduction*>& GrammarSymbol::reachable_productions_by_first_symbol() const
{
    return reachable_productions_by_first_symbol_;
}

}
