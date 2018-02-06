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
    int index_;
    GrammarSymbol* symbol_;
    std::vector<GrammarSymbol*> symbols_;
    GrammarSymbol* precedence_symbol_;
    GrammarAction* action_;
    int line_;

public:
    GrammarProduction( int index, GrammarSymbol* symbol );  

    int index() const;
    GrammarSymbol* symbol() const;
    const std::vector<GrammarSymbol*>& symbols() const;
    GrammarSymbol* precedence_symbol() const;
    GrammarAction* action() const;
    int line() const;
    int length() const;

    void append_symbol( GrammarSymbol* symbol );
    void set_precedence_symbol( GrammarSymbol* precedence_symbol );
    void set_action( GrammarAction* action );
    void replace_references_to_symbol( GrammarSymbol* to_symbol, GrammarSymbol* with_symbol );
};

}

}

#endif
