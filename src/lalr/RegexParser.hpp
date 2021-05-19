#ifndef LALR_REGEXPARSER_HPP_INCLUDED
#define LALR_REGEXPARSER_HPP_INCLUDED

namespace lalr
{

class RegexSyntaxTree;

class RegexParser
{
    RegexSyntaxTree* syntax_tree_;
    const char* position_;
    const char* end_;
    const char* lexeme_begin_;
    const char* lexeme_end_;
    bool successful_;

public:
    RegexParser( RegexSyntaxTree* syntax_tree );
    bool parse( const char* begin, const char* end );

private:
    bool match_or_expression();
    bool match_cat_expression();
    bool match_postfix_expression();
    bool match_base_expression();
    bool match_negative_bracket_expression();
    bool match_bracket_expression();
    bool match_action_expression();
    bool match_compound_expression();
    bool match_character_expression();
    bool match_item();
    bool match_negative_item();
    bool match_character();
    bool match_end_of_range();
    bool match_identifier();
    bool match( const char* lexeme );
    bool expect( const char* lexeme );
    int escape( const char* start, const char* finish ) const;
};

}

#endif
