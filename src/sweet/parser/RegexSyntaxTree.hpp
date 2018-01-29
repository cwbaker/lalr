#ifndef SWEET_LEXER_REGEXSYNTAXTREE_HPP_INCLUDED
#define SWEET_LEXER_REGEXSYNTAXTREE_HPP_INCLUDED

#include "RegexCharacter.hpp"
#include "RegexNodeLess.hpp"
#include "RegexNodeType.hpp"
#include "LexerToken.hpp"
#include <memory>
#include <string>
#include <vector>
#include <set>

namespace sweet
{

namespace lexer
{

class LexerGenerator;
class LexerAction;
class RegexNode;

/**
// @internal
//
// Parse regular expressions.
*/
class RegexSyntaxTree
{
    LexerGenerator* lexer_generator_; ///< The LexerGenerator to retrieve actions from and report errors and debug information to.
    std::set<RegexCharacter> bracket_expression_characters_; ///< The characters in the current bracket expression.
    int index_; ///< The current node index.
    std::vector<std::shared_ptr<RegexNode> > nodes_; ///< The current nodes.
    int errors_; ///< The number of errors that have occured.

    public:
        RegexSyntaxTree( const LexerToken& token, LexerGenerator* lexer_generator );
        RegexSyntaxTree( const std::vector<LexerToken>& tokens, LexerGenerator* lexer_generator );

        bool empty() const;
        int errors() const;
        const std::shared_ptr<RegexNode>& node() const;
        void print() const;

        void cat_expression();
        void or_expression();
        void star_expression();
        void plus_expression();
        void optional_expression();
        void begin_bracket_expression();
        void begin_negative_bracket_expression();
        void end_bracket_expression();
        void action_expression( const std::string& identifier );
        void character( int character );
        void dot();
        void item_range( int begin, int end );
        void item_character( int character );
        void item_alnum();
        void item_word();
        void item_alpha();
        void item_blank();
        void item_cntrl();
        void item_digit();
        void item_graph();
        void item_lower();
        void item_print();
        void item_punct();
        void item_space();
        void item_upper();
        void item_xdigit();
        void negative_item_range( int begin, int end );
        void negative_item_character( int character );
        void negative_item_alnum();
        void negative_item_word();
        void negative_item_alpha();
        void negative_item_blank();
        void negative_item_cntrl();
        void negative_item_digit();
        void negative_item_graph();
        void negative_item_lower();
        void negative_item_print();
        void negative_item_punct();
        void negative_item_space();
        void negative_item_upper();
        void negative_item_xdigit();

    private:
        std::shared_ptr<RegexNode> regex_node( RegexNodeType type );
        std::shared_ptr<RegexNode> regex_node( int begin, int end );
        std::shared_ptr<RegexNode> regex_node( int begin, int end, const LexerToken* token );
        std::shared_ptr<RegexNode> regex_node( const LexerAction* action );

        void print_positions( const std::set<RegexNode*, RegexNodeLess>& positions ) const;
        void print_nodes( const std::vector<std::shared_ptr<RegexNode> >& nodes, int level ) const;

        void calculate_symbols_for_characters_start_and_end();
        void calculate_combined_parse_tree( const std::vector<LexerToken>& tokens );
        void calculate_nullable_first_last_and_follow();
        void parse_regular_expression( const LexerToken& token );
        void parse_literal( const LexerToken& token );

        int escape( std::string::const_iterator start, std::string::const_iterator end, std::string::const_iterator* next ) const;
        void insert_characters( int begin, int end );
        void erase_characters( int begin, int end );
        void insert_characters( const char* characters );
        void erase_characters( const char* characters );
};

}

}

#endif
