//
// RegexSyntaxTree.cpp
// Copyright Charles Baker. All rights reserved.
//

#include "RegexSyntaxTree.hpp"
#include "RegexParser.hpp"
#include "RegexNode.hpp"
#include "RegexNodeLess.hpp"
#include "RegexCharacter.hpp"
#include "RegexGenerator.hpp"
#include "ErrorCode.hpp"
#include "assert.hpp"
#include <algorithm>

using std::set;
using std::pair;
using std::string;
using std::vector;
using namespace lalr;

/**
// Default constructor.
*/
RegexSyntaxTree::RegexSyntaxTree()
: generator_( nullptr )
, bracket_expression_characters_()
, index_( 0 )
, nodes_()
, errors_( 0 )
{
}

/**
// Is this syntax tree empty?
//
// @return
//  True if this syntax tree is empty otherwise false.
*/
bool RegexSyntaxTree::empty() const
{
    return nodes_.empty();
}

/**
// Get the number of errors that occured while parsing.
//
// @return
//  The number of errors.
*/
int RegexSyntaxTree::errors() const
{
    return errors_;
}

/**
// Get the RegexNode that resulted from the parse.
//
// @return
//  The RegexNode.
*/
const std::shared_ptr<RegexNode>& RegexSyntaxTree::node() const
{
    LALR_ASSERT( !nodes_.empty() );
    LALR_ASSERT( nodes_.front() );
    return nodes_.front();
}

/**
// Print the regular expression parse tree.
*/
void RegexSyntaxTree::print() const
{
    print_nodes( nodes_, 0 );
    printf( "\n\n" );
}

/**
// Reset this syntax tree to its default state.
*/
void RegexSyntaxTree::reset()
{
    generator_ = nullptr;
    bracket_expression_characters_.clear();
    index_ = 0;
    nodes_.clear();
    errors_ = 0;
}

/**
// Parse *token* into this syntax tree.
//
// @param regular_expression
//  The single regular expression to parse.
//
// @param generator
//  The RegexGenerator to retrieve actions from and report errors to.
*/
void RegexSyntaxTree::reset( const RegexToken& token, RegexGenerator* generator )
{
    LALR_ASSERT( generator );
    reset();
    generator_ = generator;
    parse_regular_expression( token );
    calculate_nullable_first_last_and_follow();
}

/**
// Parse *tokens* into this syntax tree.
//
// @param regular_expression
//  The single regular expression to parse.
//
// @param generator
//  The RegexGenerator to retrieve actions from and report errors to.
*/
void RegexSyntaxTree::reset( const std::vector<RegexToken>& tokens, RegexGenerator* generator )
{
    LALR_ASSERT( generator );
    reset();
    generator_ = generator;
    calculate_combined_parse_tree( tokens );
    calculate_nullable_first_last_and_follow();
}

/**
// Reduce the two most recently parsed expressions into a cat expression.
*/
void RegexSyntaxTree::cat_expression()
{
    std::shared_ptr<RegexNode> right_node( nodes_.back() );
    nodes_.pop_back();

    std::shared_ptr<RegexNode> left_node( nodes_.back() );
    nodes_.pop_back();

    std::shared_ptr<RegexNode> node = regex_node( LEXER_NODE_CAT );
    node->add_node( left_node );
    node->add_node( right_node );
    nodes_.push_back( node );
}

/**
// Reduce the two most recently parsed expressions into an or expression.
*/
void RegexSyntaxTree::or_expression()
{
    std::shared_ptr<RegexNode> right_node( nodes_.back() );
    nodes_.pop_back();

    std::shared_ptr<RegexNode> left_node( nodes_.back() );
    nodes_.pop_back();

    std::shared_ptr<RegexNode> node = regex_node( LEXER_NODE_OR );
    node->add_node( left_node );
    node->add_node( right_node );
    nodes_.push_back( node );
}

/**
// Reduce the most recently parsed expression into a star expression.
*/
void RegexSyntaxTree::star_expression()
{
    std::shared_ptr<RegexNode> star_node( nodes_.back() );
    nodes_.pop_back();

    std::shared_ptr<RegexNode> node = regex_node( LEXER_NODE_STAR );
    node->add_node( star_node );
    nodes_.push_back( node );
}

/**
// Reduce the most recently parsed expression into a plus expression.
*/
void RegexSyntaxTree::plus_expression()
{
    std::shared_ptr<RegexNode> plus_node( nodes_.back() );
    nodes_.pop_back();

    std::shared_ptr<RegexNode> node = regex_node( LEXER_NODE_PLUS );
    node->add_node( plus_node );
    nodes_.push_back( node );
}

/**
// Reduce the most recently parsed expression into an optional expression.
*/
void RegexSyntaxTree::optional_expression()
{
    std::shared_ptr<RegexNode> optional_node( nodes_.back() );
    nodes_.pop_back();

    std::shared_ptr<RegexNode> node = regex_node( LEXER_NODE_OPTIONAL );
    node->add_node( optional_node );
    nodes_.push_back( node );
}

/**
// Begin a bracket expression '[ ...'.
*/
void RegexSyntaxTree::begin_bracket_expression()
{
    bracket_expression_characters_.clear();
}

/**
// Begin a negative bracket expression '[^ ...'.
*/
void RegexSyntaxTree::begin_negative_bracket_expression()
{
    bracket_expression_characters_.clear();
    insert_characters( BEGIN_CHARACTER, END_CHARACTER );
}

/**
// End a bracket or negative bracket expression.
*/
void RegexSyntaxTree::end_bracket_expression()
{
    set<RegexCharacter>::const_iterator character = bracket_expression_characters_.begin();
    std::shared_ptr<RegexNode> node = regex_node( character->begin_character(), character->end_character() );
    nodes_.push_back( node );
    ++character;
    
    while ( character != bracket_expression_characters_.end() )
    {
        node = regex_node( character->begin_character(), character->end_character() );
        nodes_.push_back( node );
        or_expression();
        ++character;
    }    
}

/**
// Handle an action expression ': [A-Za-z_][A-Za-z0-9_]* :'.
*/
void RegexSyntaxTree::action_expression( const std::string& identifier )
{
    LALR_ASSERT( !identifier.empty() );
    LALR_ASSERT( generator_ );
    std::shared_ptr<RegexNode> node = regex_node( generator_->add_lexer_action(identifier) );
    nodes_.push_back( node );
}

/**
// Handle a character.
*/
void RegexSyntaxTree::character( int character )
{
    std::shared_ptr<RegexNode> node = regex_node( character, character + 1 );
    nodes_.push_back( node );
}

/**
// Handle a dot.
*/
void RegexSyntaxTree::dot()
{
    std::shared_ptr<RegexNode> node = regex_node( BEGIN_CHARACTER, END_CHARACTER );
    nodes_.push_back( node );
    
}

/**
// Handle a range specified within a character class.
*/
void RegexSyntaxTree::item_range( int begin, int end )
{
    LALR_ASSERT( begin >= BEGIN_CHARACTER && begin < END_CHARACTER );
    LALR_ASSERT( end >= BEGIN_CHARACTER && end < END_CHARACTER );
    LALR_ASSERT( begin <= end );
    insert_characters( begin, end );
}

/**
// Handle a character specified within a character class.
*/
void RegexSyntaxTree::item_character( int character )
{
    LALR_ASSERT( character >= BEGIN_CHARACTER && character < END_CHARACTER );
    insert_characters( character, character + 1 );
}

/**
// Handle ':alnum:' specified in a character class.
*/
void RegexSyntaxTree::item_alnum()
{
    item_alpha();
    item_digit();
}

/**
// Handle ':word:' specified in a character class.
*/
void RegexSyntaxTree::item_word()
{
    item_alpha();
    item_digit();
    insert_characters( '_', '_' + 1 );
}

/**
// Handle ':alpha:' specified in a character class.
*/
void RegexSyntaxTree::item_alpha()
{
    item_lower();
    item_upper();
}

/**
// Handle ':blank:' specified in a character class.
*/
void RegexSyntaxTree::item_blank()
{
    insert_characters( " \t" );
}

/**
// Handle ':cntrl:' specified in a character class.
*/
void RegexSyntaxTree::item_cntrl()
{
    insert_characters( 0x00, 0x1f + 1 );
    insert_characters( 0x7f, 0x7f + 1 );
}

/**
// Handle ':digit:' specified in a character class.
*/
void RegexSyntaxTree::item_digit()
{
    insert_characters( '0', '9' + 1 );
}

/**
// Handle ':graph:' specified in a character class.
*/
void RegexSyntaxTree::item_graph()
{
    insert_characters( 0x21, 0x7e + 1 );
}

/**
// Handle ':lower:' specified in a character class.
*/
void RegexSyntaxTree::item_lower()
{
    insert_characters( 'a', 'z' + 1 );
}

/**
// Handle ':print:' specified in a character class.
*/
void RegexSyntaxTree::item_print()
{
    insert_characters( 0x20, 0x7e + 1 );
}

/**
// Handle ':punct:' specified in a character class.
*/
void RegexSyntaxTree::item_punct()
{
    insert_characters( "-!\"#$%&'()*+,./:;<=>?@[\\]_`{|}~" );
}

/**
// Handle ':space:' specified in a character class.
*/
void RegexSyntaxTree::item_space()
{
    insert_characters( " \t\r\n" );
}

/**
// Handle ':upper:' specified in a character class.
*/
void RegexSyntaxTree::item_upper()
{
    insert_characters( 'A', 'Z' + 1 );
}

/**
// Handle ':xdigit:' specified in a character class.
*/
void RegexSyntaxTree::item_xdigit()
{
    insert_characters( "0123456789abcdefABCDEF" );
}

/**
// Handle a range specified in a negative character class.
*/
void RegexSyntaxTree::negative_item_range( int begin, int end )
{
    LALR_ASSERT( begin >= BEGIN_CHARACTER && begin < END_CHARACTER );
    LALR_ASSERT( end >= BEGIN_CHARACTER && end < END_CHARACTER );
    LALR_ASSERT( begin <= end );
    erase_characters( begin, end );
}

/**
// Handle a character specified in a negative character class.
*/
void RegexSyntaxTree::negative_item_character( int character )
{
    LALR_ASSERT( character >= BEGIN_CHARACTER && character < END_CHARACTER );
    erase_characters( character, character + 1 );
}

/**
// Handle ':alnum:' specified in a negative character class.
*/
void RegexSyntaxTree::negative_item_alnum()
{
    negative_item_alpha();
    negative_item_digit();
}

/**
// Handle ':word:' specified in a negative character class.
*/
void RegexSyntaxTree::negative_item_word()
{
    negative_item_alpha();
    negative_item_digit();
    insert_characters( '_', '_' + 1 );
}

/**
// Handle ':alpha:' specified in a negative character class.
*/
void RegexSyntaxTree::negative_item_alpha()
{
    negative_item_lower();
    negative_item_upper();
}

/**
// Handle ':blank:' specified in a negative character class.
*/
void RegexSyntaxTree::negative_item_blank()
{
    insert_characters( " \t" );
}

/**
// Handle ':cntrl:' specified in a negative character class.
*/
void RegexSyntaxTree::negative_item_cntrl()
{
    insert_characters( 0x00, 0x1f + 1 );
    insert_characters( 0x7f, 0x7f + 1 );
}

/**
// Handle ':digit:' specified in a negative character class.
*/
void RegexSyntaxTree::negative_item_digit()
{
    insert_characters( '0', '9' + 1 );
}

/**
// Handle ':graph:' specified in a negative character class.
*/
void RegexSyntaxTree::negative_item_graph()
{
    insert_characters( 0x21, 0x7e + 1 );
}

/**
// Handle ':lower:' specified in a negative character class.
*/
void RegexSyntaxTree::negative_item_lower()
{
    insert_characters( 'a', 'z' + 1 );
}

/**
// Handle ':print:' specified in a negative character class.
*/
void RegexSyntaxTree::negative_item_print()
{
    insert_characters( 0x20, 0x7e + 1 );
}

/**
// Handle ':punct:' specified in a negative character class.
*/
void RegexSyntaxTree::negative_item_punct()
{
    insert_characters( "-!\"#$%&'()*+,./:;<=>?@[\\]_`{|}~" );
}

/**
// Handle ':space:' specified in a negative character class.
*/
void RegexSyntaxTree::negative_item_space()
{
    insert_characters( " \t\r\n" );
}

/**
// Handle ':upper:' specified in a negative character class.
*/
void RegexSyntaxTree::negative_item_upper()
{
    insert_characters( 'A', 'Z' + 1 );
}

/**
// Handle ':xdigit:' specified in a negative character class.
*/
void RegexSyntaxTree::negative_item_xdigit()
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
std::shared_ptr<RegexNode> RegexSyntaxTree::regex_node( RegexNodeType type )
{
    std::shared_ptr<RegexNode> node( new RegexNode(index_, type) );
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
std::shared_ptr<RegexNode> RegexSyntaxTree::regex_node( int begin, int end )
{
    std::shared_ptr<RegexNode> node( new RegexNode(index_, begin, end) );
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
std::shared_ptr<RegexNode> RegexSyntaxTree::regex_node( int begin, int end, const RegexToken* token )
{
    std::shared_ptr<RegexNode> node( new RegexNode(index_, begin, end, token) );
    ++index_;
    return node;
}

/**
// Create a RegexNode to represent a lexical analyzer action.
//
// @param action
//  The RegexAction that this RegexNode represents.
// @return
//  The RegexNode.
*/
std::shared_ptr<RegexNode> RegexSyntaxTree::regex_node( const RegexAction* action )
{
    std::shared_ptr<RegexNode> node( new RegexNode(index_, action) );
    ++index_;
    return node;
}

/**
// Print the positions represented by the RegexNodes in \e positions.
//
// @param positions
//  The RegexNodes that are the positions to print.
*/
void RegexSyntaxTree::print_positions( const std::set<RegexNode*, RegexNodeLess>& positions ) const
{
    set<RegexNode*, RegexNodeLess>::const_iterator i = positions.begin();
    if ( i != positions.end() )
    {
        const RegexNode* node = *i;
        LALR_ASSERT( node );
        printf( "%d", node->get_index() );
        ++i;
    }

    while ( i != positions.end() )
    {
        const RegexNode* node = *i;
        LALR_ASSERT( node );
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
void RegexSyntaxTree::print_nodes( const vector<std::shared_ptr<RegexNode> >& nodes, int level ) const
{
    for ( vector<std::shared_ptr<RegexNode> >::const_iterator i = nodes.begin(); i != nodes.end(); ++i )
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
        LALR_ASSERT( node != NULL );

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
void RegexSyntaxTree::calculate_combined_parse_tree(  const std::vector<RegexToken>& tokens )
{
    for ( vector<RegexToken>::const_iterator token = tokens.begin(); token != tokens.end(); ++token )
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
                LALR_ASSERT( false );
                break;
        }
    }
}

/**
// Calculate nullable, first positions, last positions, and follow positions
// for the regular expressions that have been parsed.
*/
void RegexSyntaxTree::calculate_nullable_first_last_and_follow()
{
    if ( !nodes_.empty() )
    {
        LALR_ASSERT( nodes_.size() == 1 );
        LALR_ASSERT( nodes_.back() );

        std::shared_ptr<RegexNode> node( nodes_.back() );
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
//  The RegexToken that defines the regular expression to parse.
*/
void RegexSyntaxTree::parse_regular_expression( const RegexToken& token )
{
    LALR_ASSERT( token.type() == TOKEN_REGULAR_EXPRESSION );
    LALR_ASSERT( !token.lexeme().empty() );

    // Create position iterators for the beginning and end of the sequence that
    // will be parsed.
    const std::string& regular_expression = token.lexeme();
    RegexParser parser( this );
    bool successful = parser.parse( &regular_expression[0], &regular_expression[0] + regular_expression.size() );
    if ( !successful )
    {
        ++errors_;
        LALR_ASSERT( generator_ );
        generator_->fire_error( token.line(), token.column(), LEXER_ERROR_SYNTAX, "Syntax error in regular expression '%s'", token.lexeme().c_str() );
        nodes_.clear();
    }
    else
    {
        // Add the end character to the regular expression that has just been parsed 
        // and then combine that regular expression with any literals or regular 
        // expressions that have been previously parsed using an or expression.
        LALR_ASSERT( nodes_.size() == 1 || nodes_.size() == 2 );
        LALR_ASSERT( nodes_.back() );
        std::shared_ptr<RegexNode> node = regex_node( INVALID_BEGIN_CHARACTER, INVALID_END_CHARACTER, &token );
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
void RegexSyntaxTree::parse_literal( const RegexToken& token )
{
    LALR_ASSERT( token.type() == TOKEN_LITERAL );
    LALR_ASSERT( !token.lexeme().empty() );
    LALR_ASSERT( token.symbol() );
    
    // Combine all characters in \e literal using cat expressions.
    const std::string& literal = token.lexeme();
    std::string::const_iterator i = literal.begin();
    int character = escape( i, literal.end(), &i );
    std::shared_ptr<RegexNode> node = regex_node( character, character + 1 );
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

    // Add the end character to the literal that has just been parsed and then 
    // combine that literal with any literals or regular expressions that have 
    // been previously parsed using an or expression.
    LALR_ASSERT( nodes_.size() == 1 || nodes_.size() == 2 );
    LALR_ASSERT( nodes_.back().get() != NULL );
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
int RegexSyntaxTree::escape( std::string::const_iterator start, std::string::const_iterator end, std::string::const_iterator* next ) const
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
                    LALR_ASSERT( false );
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
                    LALR_ASSERT( false );
                    break;
                    
                default:
                    character = *start;
                    break;
            }                   
        }
    }

    LALR_ASSERT( next );
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
void RegexSyntaxTree::insert_characters( int begin, int end )
{
    std::pair<set<RegexCharacter>::iterator, bool> result = bracket_expression_characters_.insert( RegexCharacter(begin, end) );
    while ( !result.second )
    {
        LALR_ASSERT( result.first->end_character() >= begin || end >= result.first->begin_character() );
        begin = std::min( begin, result.first->begin_character() );
        end = std::max( end, result.first->end_character() );
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
void RegexSyntaxTree::erase_characters( int begin, int end )
{
    set<RegexCharacter>::iterator i = bracket_expression_characters_.find( RegexCharacter(begin, end) );
    if ( i != bracket_expression_characters_.end() )
    {
        int pre_begin = i->begin_character();
        int pre_end = begin;

        int post_begin = end;
        int post_end = i->end_character();
        
        bracket_expression_characters_.erase( i );

        if ( pre_begin < pre_end )
        {
            bool inserted = bracket_expression_characters_.insert( RegexCharacter(pre_begin, pre_end) ).second;
            LALR_ASSERT( inserted );
            (void) inserted;
        }
        
        if ( post_begin < post_end )
        {
            bool inserted = bracket_expression_characters_.insert( RegexCharacter(post_begin, post_end) ).second;
            LALR_ASSERT( inserted );
            (void) inserted;
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
void RegexSyntaxTree::insert_characters( const char* characters )
{
    LALR_ASSERT( characters );
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
void RegexSyntaxTree::erase_characters( const char* characters )
{
    const char* character = characters;
    while ( *character != '\0' )
    {
        erase_characters( *character, *character + 1 );
        ++character;
    }
}
