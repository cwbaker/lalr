//
// RegexParser.cpp
// Copyright (c) 2007 - 2011 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "RegexParser.hpp"
#include "RegexNode.hpp"
#include "RegexNodeLess.hpp"
#include "RegexCharacter.hpp"
#include "LexerGenerator.hpp"
#include "Error.hpp"
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_file_iterator.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/spirit/include/classic_chset.hpp>
#include <boost/bind.hpp>

using std::set;
using std::pair;
using std::string;
using std::vector;
using namespace sweet;
using namespace sweet::lexer;

namespace sweet
{

namespace lexer
{

/**
// ParserGrammar for regular expressions.
*/
template <class Iterator>
class RegexGrammar : public boost::spirit::grammar<RegexGrammar<Iterator> >
{
    public:
        typedef Iterator iterator;

        template <typename Scanner>
        class definition
        {
            boost::spirit::rule<Scanner> regex;
            boost::spirit::rule<Scanner> or_expression;
            boost::spirit::rule<Scanner> cat_expression;
            boost::spirit::rule<Scanner> postfix_expression;
            boost::spirit::rule<Scanner> base_expression;
            boost::spirit::rule<Scanner> negative_bracket_expression;
            boost::spirit::rule<Scanner> bracket_expression;
            boost::spirit::rule<Scanner> action_expression;
            boost::spirit::rule<Scanner> postfix;
            boost::spirit::rule<Scanner> character_;
            boost::spirit::rule<Scanner> item;
            boost::spirit::rule<Scanner> negative_item;
            boost::spirit::rule<Scanner> ch;

            public:
                definition( const RegexGrammar& self );
                boost::spirit::rule<Scanner> const& start() const;
        };

    private:
        RegexParser* parser_; ///< The RegexParser to report matches to.

    public:
        RegexGrammar( RegexParser* parser );
        RegexParser* get_parser() const;
        void action_expression( iterator start, iterator finish ) const;
        void character( iterator start, iterator finish ) const;
        void item_range( iterator start, iterator finish ) const;
        void item_character( iterator start, iterator finish ) const;
        void negative_item_range( iterator start, iterator finish ) const;
        void negative_item_character( iterator start, iterator finish ) const;
        int escape( iterator start, iterator finish ) const;
};

}

}

/**
// The RegexGrammar definition.
//
// @param self
//  The RegexGrammar.
*/
template <class Iterator>
template <class Scanner>
RegexGrammar<Iterator>::definition<Scanner>::definition( const RegexGrammar& self )
{
    using namespace boost::spirit;

    RegexParser* parser = self.get_parser();

    regex = or_expression
          >> end_p
          ;

    or_expression = cat_expression
                  >> !(ch_p('|') >> or_expression) [boost::bind(&RegexParser::or_expression, parser)]
                  ;

    cat_expression = postfix_expression
                   >> !cat_expression [boost::bind(&RegexParser::cat_expression, parser)]
                   ;

    postfix_expression = base_expression 
                       >> !postfix
                       ;

    base_expression = negative_bracket_expression
                    | bracket_expression
                    | action_expression
                    | ch_p('(') >> or_expression >> ch_p(')')
                    | character_
                    ;
                    
    negative_bracket_expression = str_p("[^") [boost::bind(&RegexParser::begin_negative_bracket_expression, parser)]
                                >> !ch_p( '-' ) [boost::bind(&RegexParser::negative_item_character, parser, '-')]
                                >> *negative_item
                                >> !ch_p( '-' ) [boost::bind(&RegexParser::negative_item_character, parser, '-')]
                                >> ch_p(']') [boost::bind(&RegexParser::end_bracket_expression, parser)]
                                ;

    bracket_expression = ch_p('[') [boost::bind(&RegexParser::begin_bracket_expression, parser)]
                       >> !ch_p( '-' ) [boost::bind(&RegexParser::item_character, parser, '-')]
                       >> *item
                       >> !ch_p( '-' ) [boost::bind(&RegexParser::item_character, parser, '-')]
                       >> ch_p(']') [boost::bind(&RegexParser::end_bracket_expression, parser)]
                       ;

    action_expression = lexeme_d[
                            ch_p(':')
                            >> ((alpha_p | ch_p("_")) >> *(alnum_p | ch_p("_"))) [boost::bind(&RegexGrammar::action_expression, &self, _1, _2)]
                            >> ch_p(':')
                        ];

    postfix = ch_p( '*' ) [boost::bind(&RegexParser::star_expression, parser)]
            | ch_p( '+' ) [boost::bind(&RegexParser::plus_expression, parser)]
            | ch_p( '?' ) [boost::bind(&RegexParser::optional_expression, parser)]
            ;

    character_ = ch_p( '.' ) [boost::bind(&RegexParser::dot, parser)]
               | ch [boost::bind(&RegexGrammar::character, &self, _1, _2)]
               ;

    item = str_p("[:alnum:]" ) [boost::bind(&RegexParser::item_alnum, parser)]
         | str_p("[:word:]" ) [boost::bind(&RegexParser::item_word, parser)]
         | str_p("[:alpha:]" ) [boost::bind(&RegexParser::item_alpha, parser)]
         | str_p("[:blank:]" ) [boost::bind(&RegexParser::item_blank, parser)]
         | str_p("[:cntrl:]" ) [boost::bind(&RegexParser::item_cntrl, parser)]
         | str_p("[:digit:]" ) [boost::bind(&RegexParser::item_digit, parser)]
         | str_p("[:graph:]" ) [boost::bind(&RegexParser::item_graph, parser)]
         | str_p("[:lower:]" ) [boost::bind(&RegexParser::item_lower, parser)]
         | str_p("[:print:]" ) [boost::bind(&RegexParser::item_print, parser)]
         | str_p("[:punct:]" ) [boost::bind(&RegexParser::item_punct, parser)]
         | str_p("[:space:]" ) [boost::bind(&RegexParser::item_space, parser)]
         | str_p("[:upper:]" ) [boost::bind(&RegexParser::item_upper, parser)]
         | str_p("[:xdigit:]" ) [boost::bind(&RegexParser::item_xdigit, parser)]
         | (ch >> ch_p('-') >> ch) [boost::bind(&RegexGrammar::item_range, &self, _1, _2)]
         | ch [boost::bind(&RegexGrammar::item_character, &self, _1, _2)]
         ;

    negative_item = str_p("[:alnum:]" ) [boost::bind(&RegexParser::negative_item_alnum, parser)]
                  | str_p("[:word:]" ) [boost::bind(&RegexParser::negative_item_word, parser)]
                  | str_p("[:alpha:]" ) [boost::bind(&RegexParser::negative_item_alpha, parser)]
                  | str_p("[:blank:]" ) [boost::bind(&RegexParser::negative_item_blank, parser)]
                  | str_p("[:cntrl:]" ) [boost::bind(&RegexParser::negative_item_cntrl, parser)]
                  | str_p("[:digit:]" ) [boost::bind(&RegexParser::negative_item_digit, parser)]
                  | str_p("[:graph:]" ) [boost::bind(&RegexParser::negative_item_graph, parser)]
                  | str_p("[:lower:]" ) [boost::bind(&RegexParser::negative_item_lower, parser)]
                  | str_p("[:print:]" ) [boost::bind(&RegexParser::negative_item_print, parser)]
                  | str_p("[:punct:]" ) [boost::bind(&RegexParser::negative_item_punct, parser)]
                  | str_p("[:space:]" ) [boost::bind(&RegexParser::negative_item_space, parser)]
                  | str_p("[:upper:]" ) [boost::bind(&RegexParser::negative_item_upper, parser)]
                  | str_p("[:xdigit:]" ) [boost::bind(&RegexParser::negative_item_xdigit, parser)]
                  | (ch >> ch_p('-') >> ch) [boost::bind(&RegexGrammar::negative_item_range, &self, _1, _2)]
                  | ch [boost::bind(&RegexGrammar::negative_item_character, &self, _1, _2)]
                  ;

    ch = lexeme_d[ 
            ch_p( '\\' ) >> oct_p
            | str_p( "\\x" ) >> hex_p
            | ch_p( '\\' ) >> anychar_p
            | anychar_p - chset_p( "|*+?[]()-" )
         ];                  

    BOOST_SPIRIT_DEBUG_RULE( regex );
    BOOST_SPIRIT_DEBUG_RULE( or_expression );
    BOOST_SPIRIT_DEBUG_RULE( cat_expression );
    BOOST_SPIRIT_DEBUG_RULE( postfix_expression );
    BOOST_SPIRIT_DEBUG_RULE( base_expression );
    BOOST_SPIRIT_DEBUG_RULE( negative_bracket_expression );
    BOOST_SPIRIT_DEBUG_RULE( bracket_expression );
    BOOST_SPIRIT_DEBUG_RULE( action_expression );
    BOOST_SPIRIT_DEBUG_RULE( postfix );
    BOOST_SPIRIT_DEBUG_RULE( character_ );
    BOOST_SPIRIT_DEBUG_RULE( item );
    BOOST_SPIRIT_DEBUG_RULE( negative_item );
    BOOST_SPIRIT_DEBUG_RULE( ch );
}

/**
// Get the starting symbol for the grammar grammar.
//
// @return
//  The starting symbol.
*/
template <class Iterator>
template <class Scanner>
boost::spirit::rule<Scanner> const& 
RegexGrammar<Iterator>::definition<Scanner>::start() const
{
    return regex;
}

/**
// Constructor.
//
// @param parser
//  The RegexParser to report grammar matches to.
*/
template <class Iterator>
RegexGrammar<Iterator>::RegexGrammar( RegexParser* parser )
: parser_( parser )
{
    SWEET_ASSERT( parser_ );
}

/**
// Get the RegexParser that this RegexGrammar reports matches to.
//
// @return
//  The RegexParser.
*/
template <class Iterator>
RegexParser*
RegexGrammar<Iterator>::get_parser() const
{
    return parser_;
}

/**
// Match an action call.
//
// @param start
//  The iterator at the start of the action to matched.
//
// @param finish
//  The iterator one past the end of the action to match.
*/
template <class Iterator>
void 
RegexGrammar<Iterator>::action_expression( iterator start, iterator finish ) const
{
    SWEET_ASSERT( parser_ );
    parser_->action_expression( std::string(start, finish) );
}

/**
// Match a character.
//
// @param start
//  The iterator at the start of the character to match.
//
// @param finish
//  The iterator one past the end of the character to match.
*/
template <class Iterator>
void 
RegexGrammar<Iterator>::character( iterator start, iterator finish ) const
{
    SWEET_ASSERT( parser_ );    
    parser_->character( escape(start, finish) );
}

/**
// Match a range in a bracketed expression.
//
// @param start
//  The iterator at the start of the range to match.
//
// @param finish
//  The iterator one past the end of the range to match.
*/
template <class Iterator>
void 
RegexGrammar<Iterator>::item_range( iterator start, iterator finish ) const
{
    iterator delimiter = std::find( start, finish, '-' );
    SWEET_ASSERT( delimiter != finish );
    SWEET_ASSERT( *delimiter == '-' );
    int begin = escape( start, delimiter );
    ++delimiter;
    int end = escape( delimiter, finish ) + 1;

    SWEET_ASSERT( parser_ );
    parser_->item_range( begin, end );
}

/**
// Match a character in a bracketed expression.
//
// @param start
//  The iterator at the start of the character to match.
//
// @param finish
//  The iterator one past the end of the character to match.
*/
template <class Iterator>
void 
RegexGrammar<Iterator>::item_character( iterator start, iterator finish ) const
{
    SWEET_ASSERT( parser_ );    
    parser_->item_character( escape(start, finish) );
}

/**
// Match a range in a negative bracketed expression.
//
// @param start
//  The iterator at the start of the range to match.
//
// @param finish
//  The iterator one past the end of the range to match.
*/
template <class Iterator>
void 
RegexGrammar<Iterator>::negative_item_range( iterator start, iterator finish ) const
{
    iterator delimiter = std::find( start, finish, '-' );
    SWEET_ASSERT( delimiter != finish );
    SWEET_ASSERT( *delimiter == '-' );
    int begin = escape( start, delimiter );
    ++delimiter;
    int end = escape( delimiter, finish ) + 1;
    
    SWEET_ASSERT( parser_ );
    parser_->negative_item_range( begin, end );
}

/**
// Match a character in a negative bracketed expression.
//
// @param start
//  The iterator at the start of the character to match.
//
// @param finish
//  The iterator one past the end of the character to match.
*/
template <class Iterator>
void 
RegexGrammar<Iterator>::negative_item_character( iterator start, iterator finish ) const
{
    SWEET_ASSERT( parser_ );    
    parser_->negative_item_character( escape(start, finish) );
}

/**
// Convert [\e start, \e finish) into a character converting escape 
// sequences \b, \f, \n, \r, \t, \xHH, \000, or \ followed by any
// character.
//
// @param start
//  The first character in the sequence to convert.
//
// @param finish
//  One past the last character in the sequence to convert.
//
// @return
//  The sequence [\e start, \e finish) converted into a character.
*/
template <class Iterator>
int 
RegexGrammar<Iterator>::escape( iterator start, iterator finish ) const
{
    int character = *start;
    if ( character == '\\' )
    {
        ++start;
        if ( start != finish )
        {
            switch ( *start )
            {
                case 'b':
                    character = '\b';
                    break;
                
                case 'f':
                    character = '\f';
                    break;
                
                case 'n':
                    character = '\n';
                    break;
                
                case 'r':
                    character = '\r';
                    break;
                
                case 't':
                    character = '\t';
                    break;
                    
                case 'x':
                case 'X':
                    ++start;
                    character = 0;
                    while ( isxdigit(*start ) )
                    {
                        character <<= 4;                        
                        if ( *start >= '0' && *start <= '9' )
                        {
                            character += *start - '0';
                        }
                        else if ( *start >= 'a' && *start <= 'f' )
                        {
                            character += *start - 'a' + 10;
                        }
                        else if ( *start >= 'A' && *start <= 'F' )
                        {
                            character += *start - 'A' + 10;
                        }
                        
                        ++start;
                    }
                    break;
                    
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                {
                    character = 0;
                    int count = 0;
                    while ( *start >= '0' && *start <= '7' && count < 3 )
                    {
                        character <<= 3;
                        character += *start - '0';
                        ++start;
                        ++count;
                    }
                    break;
                }
                    
                default:
                    character = *start;
                    break;
            }                   
        }
    }

    return character;
}

/**
// Constructor.
//
// @param regular_expression
//  The single regular expression to parse.
//
// @param symbol
//  The symbol to return when the regular expression matches.
*/
RegexParser::RegexParser( const LexerToken& token, LexerGenerator* lexer_generator )
: lexer_generator_( lexer_generator ),
  bracket_expression_characters_(),
  index_( 0 ),
  nodes_(),
  errors_( 0 )
{
    SWEET_ASSERT( lexer_generator_ );
    parse_regular_expression( token );
    calculate_nullable_first_last_and_follow();
}  

/**
// Constructor.
*/
RegexParser::RegexParser( const std::vector<LexerToken>& tokens, LexerGenerator* lexer_generator )
: lexer_generator_( lexer_generator ),
  bracket_expression_characters_(),
  index_( 0 ),
  nodes_(),
  errors_( 0 )
{
    SWEET_ASSERT( lexer_generator_ );
    calculate_combined_parse_tree( tokens );
    calculate_nullable_first_last_and_follow();
}

bool RegexParser::empty() const
{
    return nodes_.empty();
}

/**
// Get the number of errors that occured while parsing.
//
// @return
//  The number of errors.
*/
int RegexParser::errors() const
{
    return errors_;
}

/**
// Get the RegexNode that resulted from the parse.
//
// @return
//  The RegexNode.
*/
const ptr<RegexNode>& RegexParser::node() const
{
    SWEET_ASSERT( !nodes_.empty() );
    SWEET_ASSERT( nodes_.front() );
    return nodes_.front();
}

/**
// Print the regular expression parse tree.
*/
void RegexParser::print() const
{
    print_nodes( nodes_, 0 );
    printf( "\n\n" );
}

/**
// Reduce the two most recently parsed expressions into a cat expression.
*/
void RegexParser::cat_expression()
{
    ptr<RegexNode> right_node( nodes_.back() );
    nodes_.pop_back();

    ptr<RegexNode> left_node( nodes_.back() );
    nodes_.pop_back();

    ptr<RegexNode> node = regex_node( LEXER_NODE_CAT );
    node->add_node( left_node );
    node->add_node( right_node );
    nodes_.push_back( node );
}

/**
// Reduce the two most recently parsed expressions into an or expression.
*/
void RegexParser::or_expression()
{
    ptr<RegexNode> right_node( nodes_.back() );
    nodes_.pop_back();

    ptr<RegexNode> left_node( nodes_.back() );
    nodes_.pop_back();

    ptr<RegexNode> node = regex_node( LEXER_NODE_OR );
    node->add_node( left_node );
    node->add_node( right_node );
    nodes_.push_back( node );
}

/**
// Reduce the most recently parsed expression into a star expression.
*/
void RegexParser::star_expression()
{
    ptr<RegexNode> star_node( nodes_.back() );
    nodes_.pop_back();

    ptr<RegexNode> node = regex_node( LEXER_NODE_STAR );
    node->add_node( star_node );
    nodes_.push_back( node );
}

/**
// Reduce the most recently parsed expression into a plus expression.
*/
void RegexParser::plus_expression()
{
    ptr<RegexNode> plus_node( nodes_.back() );
    nodes_.pop_back();

    ptr<RegexNode> node = regex_node( LEXER_NODE_PLUS );
    node->add_node( plus_node );
    nodes_.push_back( node );
}

/**
// Reduce the most recently parsed expression into an optional expression.
*/
void RegexParser::optional_expression()
{
    ptr<RegexNode> optional_node( nodes_.back() );
    nodes_.pop_back();

    ptr<RegexNode> node = regex_node( LEXER_NODE_OPTIONAL );
    node->add_node( optional_node );
    nodes_.push_back( node );
}

/**
// Begin a bracket expression '[ ...'.
*/
void RegexParser::begin_bracket_expression()
{
    bracket_expression_characters_.clear();
}

/**
// Begin a negative bracket expression '[^ ...'.
*/
void RegexParser::begin_negative_bracket_expression()
{
    bracket_expression_characters_.clear();
    insert_characters( BEGIN_CHARACTER, END_CHARACTER );
}

/**
// End a bracket or negative bracket expression.
*/
void RegexParser::end_bracket_expression()
{
    set<RegexCharacter>::const_iterator character = bracket_expression_characters_.begin();
    ptr<RegexNode> node = regex_node( character->get_begin_character(), character->get_end_character() );
    nodes_.push_back( node );
    ++character;
    
    while ( character != bracket_expression_characters_.end() )
    {
        node = regex_node( character->get_begin_character(), character->get_end_character() );
        nodes_.push_back( node );
        or_expression();
        ++character;
    }    
}

/**
// Handle an action expression ': [A-Za-z_][A-Za-z0-9_]* :'.
*/
void RegexParser::action_expression( const std::string& identifier )
{
    SWEET_ASSERT( !identifier.empty() );
    SWEET_ASSERT( lexer_generator_ );
    ptr<RegexNode> node = regex_node( lexer_generator_->add_lexer_action(identifier) );
    nodes_.push_back( node );
}

/**
// Handle a character.
*/
void RegexParser::character( int character )
{
    ptr<RegexNode> node = regex_node( character, character + 1 );
    nodes_.push_back( node );
}

/**
// Handle a dot.
*/
void RegexParser::dot()
{
    ptr<RegexNode> node = regex_node( BEGIN_CHARACTER, END_CHARACTER );
    nodes_.push_back( node );
    
}

/**
// Handle a range specified within a character class.
*/
void RegexParser::item_range( int begin, int end )
{
    SWEET_ASSERT( begin >= BEGIN_CHARACTER && begin < END_CHARACTER );
    SWEET_ASSERT( end >= BEGIN_CHARACTER && end < END_CHARACTER );
    SWEET_ASSERT( begin <= end );
    insert_characters( begin, end );
}

/**
// Handle a character specified within a character class.
*/
void RegexParser::item_character( int character )
{
    SWEET_ASSERT( character >= BEGIN_CHARACTER && character < END_CHARACTER );
    insert_characters( character, character + 1 );
}

/**
// Handle ':alnum:' specified in a character class.
*/
void RegexParser::item_alnum()
{
    item_alpha();
    item_digit();
}

/**
// Handle ':word:' specified in a character class.
*/
void RegexParser::item_word()
{
    item_alpha();
    item_digit();
    insert_characters( '_', '_' + 1 );
}

/**
// Handle ':alpha:' specified in a character class.
*/
void RegexParser::item_alpha()
{
    item_lower();
    item_upper();
}

/**
// Handle ':blank:' specified in a character class.
*/
void RegexParser::item_blank()
{
    insert_characters( " \t" );
}

/**
// Handle ':cntrl:' specified in a character class.
*/
void RegexParser::item_cntrl()
{
    insert_characters( 0x00, 0x1f + 1 );
    insert_characters( 0x7f, 0x7f + 1 );
}

/**
// Handle ':digit:' specified in a character class.
*/
void RegexParser::item_digit()
{
    insert_characters( '0', '9' + 1 );
}

/**
// Handle ':graph:' specified in a character class.
*/
void RegexParser::item_graph()
{
    insert_characters( 0x21, 0x7e + 1 );
}

/**
// Handle ':lower:' specified in a character class.
*/
void RegexParser::item_lower()
{
    insert_characters( 'a', 'z' + 1 );
}

/**
// Handle ':print:' specified in a character class.
*/
void RegexParser::item_print()
{
    insert_characters( 0x20, 0x7e + 1 );
}

/**
// Handle ':punct:' specified in a character class.
*/
void RegexParser::item_punct()
{
    insert_characters( "-!\"#$%&'()*+,./:;<=>?@[\\]_`{|}~" );
}

/**
// Handle ':space:' specified in a character class.
*/
void RegexParser::item_space()
{
    insert_characters( " \t\r\n" );
}

/**
// Handle ':upper:' specified in a character class.
*/
void RegexParser::item_upper()
{
    insert_characters( 'A', 'Z' + 1 );
}

/**
// Handle ':xdigit:' specified in a character class.
*/
void RegexParser::item_xdigit()
{
    insert_characters( "0123456789abcdefABCDEF" );
}

/**
// Handle a range specified in a negative character class.
*/
void RegexParser::negative_item_range( int begin, int end )
{
    SWEET_ASSERT( begin >= BEGIN_CHARACTER && begin < END_CHARACTER );
    SWEET_ASSERT( end >= BEGIN_CHARACTER && end < END_CHARACTER );
    SWEET_ASSERT( begin <= end );
    erase_characters( begin, end );
}

/**
// Handle a character specified in a negative character class.
*/
void RegexParser::negative_item_character( int character )
{
    SWEET_ASSERT( character >= BEGIN_CHARACTER && character < END_CHARACTER );
    erase_characters( character, character + 1 );
}

/**
// Handle ':alnum:' specified in a negative character class.
*/
void RegexParser::negative_item_alnum()
{
    negative_item_alpha();
    negative_item_digit();
}

/**
// Handle ':word:' specified in a negative character class.
*/
void RegexParser::negative_item_word()
{
    negative_item_alpha();
    negative_item_digit();
    insert_characters( '_', '_' + 1 );
}

/**
// Handle ':alpha:' specified in a negative character class.
*/
void RegexParser::negative_item_alpha()
{
    negative_item_lower();
    negative_item_upper();
}

/**
// Handle ':blank:' specified in a negative character class.
*/
void RegexParser::negative_item_blank()
{
    insert_characters( " \t" );
}

/**
// Handle ':cntrl:' specified in a negative character class.
*/
void RegexParser::negative_item_cntrl()
{
    insert_characters( 0x00, 0x1f + 1 );
    insert_characters( 0x7f, 0x7f + 1 );
}

/**
// Handle ':digit:' specified in a negative character class.
*/
void RegexParser::negative_item_digit()
{
    insert_characters( '0', '9' + 1 );
}

/**
// Handle ':graph:' specified in a negative character class.
*/
void RegexParser::negative_item_graph()
{
    insert_characters( 0x21, 0x7e + 1 );
}

/**
// Handle ':lower:' specified in a negative character class.
*/
void RegexParser::negative_item_lower()
{
    insert_characters( 'a', 'z' + 1 );
}

/**
// Handle ':print:' specified in a negative character class.
*/
void RegexParser::negative_item_print()
{
    insert_characters( 0x20, 0x7e + 1 );
}

/**
// Handle ':punct:' specified in a negative character class.
*/
void RegexParser::negative_item_punct()
{
    insert_characters( "-!\"#$%&'()*+,./:;<=>?@[\\]_`{|}~" );
}

/**
// Handle ':space:' specified in a negative character class.
*/
void RegexParser::negative_item_space()
{
    insert_characters( " \t\r\n" );
}

/**
// Handle ':upper:' specified in a negative character class.
*/
void RegexParser::negative_item_upper()
{
    insert_characters( 'A', 'Z' + 1 );
}

/**
// Handle ':xdigit:' specified in a negative character class.
*/
void RegexParser::negative_item_xdigit()
{
    insert_characters( "0123456789abcdefABCDEF" );
}

/**
// Create a specific type of RegexNode.
//
// @param type
//  The type of RegexNode to create.
//
// @return
//  The RegeNode.
*/
ptr<RegexNode> RegexParser::regex_node( RegexNodeType type )
{
    ptr<RegexNode> node( new RegexNode(index_, type) );
    ++index_;
    return node;
}

/**
// Create a RegexNode to represent characters in the interval
// [\e begin, \e end).
//
// @param begin
//  The first character in the interval to match.
//
// @param end
//  One past the last character in the interval to match.
//
// @return
//  The RegexNode.
*/
ptr<RegexNode> RegexParser::regex_node( int begin, int end )
{
    ptr<RegexNode> node( new RegexNode(index_, begin, end) );
    ++index_;
    return node;
}

/**
// Create a RegexNode to represent characters in the interval
// [\e begin, \e end).
//
// @param begin
//  The first character in the interval to match.
//
// @param end
//  One past the last character in the interval to match.
//
// @param token
//  The token to associate with the RegexNode.
//
// @return
//  The RegexNode.
*/
ptr<RegexNode> RegexParser::regex_node( int begin, int end, const LexerToken* token )
{
    ptr<RegexNode> node( new RegexNode(index_, begin, end, token) );
    ++index_;
    return node;
}

/**
// Create a RegexNode to represent a lexical analyzer action.
//
// @param action
//  The LexerAction that this RegexNode represents.
// @return
//  The RegexNode.
*/
ptr<RegexNode> RegexParser::regex_node( const LexerAction* action )
{
    ptr<RegexNode> node( new RegexNode(index_, action) );
    ++index_;
    return node;
}

/**
// Print the positions represented by the RegexNodes in \e positions.
//
// @param positions
//  The RegexNodes that are the positions to print.
*/
void RegexParser::print_positions( const std::set<RegexNode*, RegexNodeLess>& positions ) const
{
    set<RegexNode*, RegexNodeLess>::const_iterator i = positions.begin();
    if ( i != positions.end() )
    {
        const RegexNode* node = *i;
        SWEET_ASSERT( node );
        printf( "%d", node->get_index() );
        ++i;
    }

    while ( i != positions.end() )
    {
        const RegexNode* node = *i;
        SWEET_ASSERT( node );
        printf( ", %d", node->get_index() );
        ++i;
    }
}

/**
// Print the RegexNodes \e nodes.
//
// @param nodes
//  The RegexNodes to print.
//
// @param level
//  The recursion level to use when identing lines.
*/
void RegexParser::print_nodes( const vector<ptr<RegexNode> >& nodes, int level ) const
{
    for ( vector<ptr<RegexNode> >::const_iterator i = nodes.begin(); i != nodes.end(); ++i )
    {
        static const char* LEXER_NODE_TYPES [LEXER_NODE_COUNT] =
        {
            "LEXER_NODE_NULL",              
            "LEXER_NODE_CAT",
            "LEXER_NODE_OR",
            "LEXER_NODE_STAR",
            "LEXER_NODE_PLUS",
            "LEXER_NODE_OPTIONAL",
            "LEXER_NODE_SYMBOL",
            "LEXER_NODE_ACTION"
        };

        RegexNode* node = i->get();
        SWEET_ASSERT( node != NULL );

        for ( int i = 0; i < level; ++i )
        {
            printf( "  " );
        }

        printf( "%d, %s, %s, [%d, %d), nullable=%s", node->get_index(), LEXER_NODE_TYPES[node->get_type()], node->get_lexeme(), node->get_begin_character(), node->get_end_character(), node->is_nullable() ? "true" : "false" );
        printf( ", first={" ); 
        print_positions( node->get_first_positions() );
        printf( "}, last={" );
        print_positions( node->get_last_positions() );
        printf( "}, follow={" );
        print_positions( node->get_follow_positions() );    
        printf( "}\n" );

        if ( !node->get_nodes().empty() )
        {
            print_nodes( node->get_nodes(), level + 1 );
        }
    }
}

/**
// Calculate the combined parse tree for all of the literal and regular 
// expression Symbols in \e symbols.
//
// @param symbols
//  The Symbols to calculate the combined parse tree for.
*/
void RegexParser::calculate_combined_parse_tree(  const std::vector<LexerToken>& tokens )
{
    for ( vector<LexerToken>::const_iterator token = tokens.begin(); token != tokens.end(); ++token )
    {
        switch ( token->type() )
        {
            case TOKEN_REGULAR_EXPRESSION:
                parse_regular_expression( *token );
                break;
        
            case TOKEN_LITERAL:
                parse_literal( *token );
                break;
                
            case TOKEN_NULL:
            default:
                SWEET_ASSERT( false );
                break;
        }
    }
}

/**
// Calculate nullable, first positions, last positions, and follow positions
// for the regular expressions that have been parsed.
*/
void RegexParser::calculate_nullable_first_last_and_follow()
{
    if ( !nodes_.empty() )
    {
        SWEET_ASSERT( nodes_.size() == 1 );
        SWEET_ASSERT( nodes_.back() );

        ptr<RegexNode> node( nodes_.back() );
        node->calculate_nullable();
        node->calculate_first_positions();
        node->calculate_last_positions();
        node->calculate_follow_positions();
    }
}

/**
// Parse the regular expression [\e start, \e finish) for \e symbol.
//
// Parses the regular expression [\e start, \e finish) and combines it with
// all of the previously parsed regular expressions and literals using an or 
// operator.
//
// @param token
//  The LexerToken that defines the regular expression to parse.
*/
void RegexParser::parse_regular_expression( const LexerToken& token )
{
    SWEET_ASSERT( token.type() == TOKEN_REGULAR_EXPRESSION );
    SWEET_ASSERT( !token.lexeme().empty() );

//
// Create position iterators for the beginning and end of the sequence that
// will be parsed.
//
    const std::string& regular_expression = token.lexeme();
    typedef boost::spirit::position_iterator<const char*> iterator;
    iterator first( regular_expression.c_str(), regular_expression.c_str() + regular_expression.length() );
    iterator last;

//
// Parse the regular expression.
//
    using namespace boost;
    RegexGrammar<iterator> grammar( this );
    spirit::parse_info<iterator> info = spirit::parse( first, last, grammar );
    if ( !info.hit || !info.full )
    {
        ++errors_;
        SWEET_ASSERT( lexer_generator_ );
        lexer_generator_->fire_error( token.line(), SyntaxError("Syntax error in regular expression '%s'", token.lexeme().c_str()) );
        nodes_.clear();
    }
    else
    {
    //
    // Add the end character to the regular expression that has just been parsed 
    // and then combine that regular expression with any literals or regular 
    // expressions that have been previously parsed using an or expression.
    //
        SWEET_ASSERT( nodes_.size() == 1 || nodes_.size() == 2 );
        SWEET_ASSERT( nodes_.back() );
        ptr<RegexNode> node = regex_node( INVALID_BEGIN_CHARACTER, INVALID_END_CHARACTER, &token );
        nodes_.push_back( node );
        cat_expression();
        while ( nodes_.size() > 1 )
        {
            or_expression();
        }
    }
}

/**
// Parse the literal [\e start, \e finish) for \e symbol.
//
// Parses the literal [\e start, \e finish) and combines it with all of the 
// previously parsed regular expressions and literals using an or operator.
//
// @param start
//  The first character in the literal to parse.
//
// @param finish
//  One past the last character in the literal to parse.
//
// @param matched_symbol
//  The symbol that represents the token that the literal matches.
*/
void RegexParser::parse_literal( const LexerToken& token )
{
    SWEET_ASSERT( token.type() == TOKEN_LITERAL );
    SWEET_ASSERT( !token.lexeme().empty() );
    SWEET_ASSERT( token.symbol() );
    
//
// Combine all characters in \e literal using cat expressions.
//
    const std::string& literal = token.lexeme();
    std::string::const_iterator i = literal.begin();
    int character = escape( i, literal.end(), &i );
    ptr<RegexNode> node = regex_node( character, character + 1 );
    nodes_.push_back( node );
    ++i;

    while ( i != literal.end() )
    {
        int character = escape( i, literal.end(), &i );
        node = regex_node( character, character + 1 );
        nodes_.push_back( node );
        cat_expression();
        ++i;
    }

//
// Add the end character to the literal that has just been parsed and then 
// combine that literal with any literals or regular expressions that have 
// been previously parsed using an or expression.
//
    SWEET_ASSERT( nodes_.size() == 1 || nodes_.size() == 2 );
    SWEET_ASSERT( nodes_.back().get() != NULL );
    node = regex_node( INVALID_BEGIN_CHARACTER, INVALID_END_CHARACTER, &token );
    nodes_.push_back( node );
    cat_expression();
    while ( nodes_.size() > 1 )
    {
        or_expression();
    }
}

/**
// The first character in [\e start, \e end) to its potentially escaped 
// character equivalent.
//
// @param start
//  The iterator to the first character to convert from.
//
// @param end
//  One past the last character to convert from.
//
// @param next
//  An iterator to update to the position after the potentially escaped 
//  character.
//
// @return
//  The character value.
*/
int RegexParser::escape( std::string::const_iterator start, std::string::const_iterator end, std::string::const_iterator* next ) const
{
    int character = *start;
    if ( character == '\\' )
    {
        ++start;
        if ( start != end )
        {
            switch ( *start )
            {
                case 'b':
                    character = '\b';
                    break;
                
                case 'f':
                    character = '\f';
                    break;
                
                case 'n':
                    character = '\n';
                    break;
                
                case 'r':
                    character = '\r';
                    break;
                
                case 't':
                    character = '\t';
                    break;
                    
                case 'x':
                case 'X':
                    SWEET_ASSERT( false );
                    break;
                    
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    SWEET_ASSERT( false );
                    break;
                    
                default:
                    character = *start;
                    break;
            }                   
        }
    }

    SWEET_ASSERT( next );
    *next = start;
    
    return character;
}

/**
// Insert characters in the interval [\e begin, \e end) into the current
// set of bracket expression characters.
//
// @param begin
//  The first character in the interval of characters to insert.
//
// @param end
//  One past the last character in the interval of characters to insert.
*/
void RegexParser::insert_characters( int begin, int end )
{
    std::pair<set<RegexCharacter>::iterator, bool> result = bracket_expression_characters_.insert( RegexCharacter(begin, end) );
    while ( !result.second )
    {
        SWEET_ASSERT( result.first->get_end_character() >= begin || end >= result.first->get_begin_character() );
        begin = std::min( begin, result.first->get_begin_character() );
        end = std::max( end, result.first->get_end_character() );
        bracket_expression_characters_.erase( result.first );
        result = bracket_expression_characters_.insert( RegexCharacter(begin, end) );
    }
}

/**
// Remove characters in the interval [\e begin, \e end) from the current
// set of bracket expression characters.
//
// @param begin
//  The first character in the interval of characters to remove.
//
// @param end
//  One past the last character in the interval of characters to remove.
*/
void RegexParser::erase_characters( int begin, int end )
{
    set<RegexCharacter>::iterator i = bracket_expression_characters_.find( RegexCharacter(begin, end) );
    if ( i != bracket_expression_characters_.end() )
    {
        int pre_begin = i->get_begin_character();
        int pre_end = begin;

        int post_begin = end;
        int post_end = i->get_end_character();
        
        bracket_expression_characters_.erase( i );

        if ( pre_begin < pre_end )
        {
            bool inserted = bracket_expression_characters_.insert( RegexCharacter(pre_begin, pre_end) ).second;
            SWEET_ASSERT( inserted );
        }
        
        if ( post_begin < post_end )
        {
            bool inserted = bracket_expression_characters_.insert( RegexCharacter(post_begin, post_end) ).second;
            SWEET_ASSERT( inserted );
        }
    }        
}

/**
// Insert the characters in \e characters into the current set of bracket
// expression characters.
//
// @param characters
//  A nul terminated string that contains the characters to insert.
*/
void RegexParser::insert_characters( const char* characters )
{
    SWEET_ASSERT( characters );
    const char* character = characters;
    while ( *character != '\0' )
    {
        insert_characters( *character, *character + 1 );
        ++character;
    }
}

/**
// Remove the characters in \e characters from the current set of bracket
// expression characters.
//
// @param characters
//  A nul terminated string that contains the characters to remove.
*/
void RegexParser::erase_characters( const char* characters )
{
    const char* character = characters;
    while ( *character != '\0' )
    {
        erase_characters( *character, *character + 1 );
        ++character;
    }
}
