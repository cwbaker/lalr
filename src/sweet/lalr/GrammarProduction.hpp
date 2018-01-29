#ifndef SWEET_PARSER_GRAMMARPRODUCTION_HPP_INCLUDED
#define SWEET_PARSER_GRAMMARPRODUCTION_HPP_INCLUDED

#include <vector>

namespace sweet
{

namespace lalr
{

class GrammarAction;
class GrammarSymbol;

class GrammarProduction
{
    GrammarSymbol* symbol_;
    std::vector<GrammarSymbol*> symbols_;
    GrammarSymbol* precedence_symbol_;
    GrammarAction* action_;

public:
    GrammarProduction( GrammarSymbol* symbol );  

    GrammarSymbol* symbol() const;
    const std::vector<GrammarSymbol*>& symbols() const;
    GrammarSymbol* precedence_symbol() const;
    GrammarAction* action() const;

    void append_symbol( GrammarSymbol* symbol );
    void set_precedence_symbol( GrammarSymbol* precedence_symbol );
    void set_action( GrammarAction* action );
};

}

}

#endif
