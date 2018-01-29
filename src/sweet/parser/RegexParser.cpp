//
// RegexParser.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "RegexParser.hpp"
#include "RegexSyntaxTree.hpp"
#include <sweet/assert/assert.hpp>
#include <string.h>
#include <ctype.h>

using namespace sweet::parser;

RegexParser::RegexParser( RegexSyntaxTree* syntax_tree )
: syntax_tree_( syntax_tree ),
  position_( nullptr ),
  end_( nullptr ),
  lexeme_begin_( nullptr ),
  lexeme_end_( nullptr ),
  successful_( false )
{
    SWEET_ASSERT( syntax_tree_ );
}

bool RegexParser::parse( const char* begin, const char* end )
{
    SWEET_ASSERT( begin );
    SWEET_ASSERT( end );
    position_ = begin;
    end_ = end;
    successful_ = true;
    return match_or_expression() && successful_;
}

bool RegexParser::match_or_expression()
{
    if ( match_cat_expression() )
    {
        if ( match("|") && match_or_expression() )
        {            
            syntax_tree_->or_expression();
        }
        return true;
    }
    return false;
}

bool RegexParser::match_cat_expression()
{
    if ( match_postfix_expression() )
    {
        if ( match_cat_expression() )
        {
            syntax_tree_->cat_expression();
        }
        return true;
    }
    return false;
}

bool RegexParser::match_postfix_expression()
{
    if ( match_base_expression() )
    {
        if ( match("*") )
        {
            syntax_tree_->star_expression();
        }
        else if ( match("+") )
        {
            syntax_tree_->plus_expression();
        }
        else if ( match("?") )
        {
            syntax_tree_->optional_expression();
        }
        return true;
    }
    return false;
}

bool RegexParser::match_base_expression()
{
    return 
        match_negative_bracket_expression() ||
        match_bracket_expression() ||
        match_action_expression() ||
        match_compound_expression() ||
        match_character_expression()
    ;
}

bool RegexParser::match_negative_bracket_expression()
{
    if ( match("[^") )
    {
        syntax_tree_->begin_negative_bracket_expression();
        if ( match("-") )
        {
            syntax_tree_->negative_item_character( '-' );
        }
        while ( match_negative_item() )
        {
        }
        if ( match("-") )
        {
            syntax_tree_->negative_item_character( '-' );
        }
        expect( "]" );
        syntax_tree_->end_bracket_expression();
        return true;
    }
    return false;
}

bool RegexParser::match_bracket_expression()
{
    if ( match("[") )
    {
        syntax_tree_->begin_bracket_expression();
        if ( match("-") )
        {
            syntax_tree_->item_character( '-' );
        }
        while ( match_item() )
        {
        }
        if ( match("-") )
        {
            syntax_tree_->item_character( '-' );
        }
        expect( "]" );
        syntax_tree_->end_bracket_expression();
        return true;
    }
    return false;
}

bool RegexParser::match_action_expression()
{
    if ( match(":") )
    {
        match_identifier();
        syntax_tree_->action_expression( std::string(lexeme_begin_, lexeme_end_) );
        expect( ":" );
        return true;
    }
    return false;
}

bool RegexParser::match_compound_expression()
{
    if ( match("(") )
    {
        match_or_expression();
        expect( ")" );
        return true;
    }
    return false;
}

bool RegexParser::match_character_expression()
{
    if ( match(".") )
    {
        syntax_tree_->dot();
        return true;
    }
    else if ( match_character() )
    {
        syntax_tree_->character( escape(lexeme_begin_, lexeme_end_) );
        return true;
    }
    return false;
}

bool RegexParser::match_item()
{
    if ( match("[:alnum:]") )
    {
        syntax_tree_->item_alnum();
        return true;
    }
    else if ( match("[:word:]") )
    {
        syntax_tree_->item_word();
        return true;
    }
    else if ( match("[:alpha:]") )
    {
        syntax_tree_->item_alpha();
        return true;
    }
    else if ( match("[:blank:]") )
    {
        syntax_tree_->item_blank();
        return true;
    }
    else if ( match("[:cntrl:]") )
    {
        syntax_tree_->item_cntrl();
        return true;
    }
    else if ( match("[:digit:]") )
    {
        syntax_tree_->item_digit();
        return true;
    }
    else if ( match("[:graph:]") )
    {
        syntax_tree_->item_graph();
        return true;
    }
    else if ( match("[:lower:]") )
    {
        syntax_tree_->item_lower();
        return true;
    }
    else if ( match("[:print:]") )
    {
        syntax_tree_->item_print();
        return true;
    }
    else if ( match("[:punct:]") )
    {
        syntax_tree_->item_punct();
        return true;
    }
    else if ( match("[:space:]") )
    {
        syntax_tree_->item_space();
        return true;
    }
    else if ( match("[:upper:]") )
    {
        syntax_tree_->item_upper();
        return true;
    }
    else if ( match("[:xdigit:]") )
    {
        syntax_tree_->item_xdigit();
        return true;
    }
    else if ( match_character() )
    {
        int character = escape( lexeme_begin_, lexeme_end_ );
        if ( match_end_of_range() )
        {
            int end_character = escape( lexeme_begin_, lexeme_end_ ) + 1;
            syntax_tree_->item_range( character, end_character );
        }
        else
        {
            syntax_tree_->item_character( character );
        }
        return true;
    }
    return false;
}

bool RegexParser::match_negative_item()
{
    if ( match("[:alnum:]") )
    {
        syntax_tree_->negative_item_alnum();
        return true;
    }
    else if ( match("[:word:]") )
    {
        syntax_tree_->negative_item_word();
        return true;
    }
    else if ( match("[:alpha:]") )
    {
        syntax_tree_->negative_item_alpha();
        return true;
    }
    else if ( match("[:blank:]") )
    {
        syntax_tree_->negative_item_blank();
        return true;
    }
    else if ( match("[:cntrl:]") )
    {
        syntax_tree_->negative_item_cntrl();
        return true;
    }
    else if ( match("[:digit:]") )
    {
        syntax_tree_->negative_item_digit();
        return true;
    }
    else if ( match("[:graph:]") )
    {
        syntax_tree_->negative_item_graph();
        return true;
    }
    else if ( match("[:lower:]") )
    {
        syntax_tree_->negative_item_lower();
        return true;
    }
    else if ( match("[:print:]") )
    {
        syntax_tree_->negative_item_print();
        return true;
    }
    else if ( match("[:punct:]") )
    {
        syntax_tree_->negative_item_punct();
        return true;
    }
    else if ( match("[:space:]") )
    {
        syntax_tree_->negative_item_space();
        return true;
    }
    else if ( match("[:upper:]") )
    {
        syntax_tree_->negative_item_upper();
        return true;
    }
    else if ( match("[:xdigit:]") )
    {
        syntax_tree_->negative_item_xdigit();
        return true;
    }
    else if ( match_character() )
    {
        int character = escape( lexeme_begin_, lexeme_end_ );
        if ( match_end_of_range() )
        {
            int end_character = escape( lexeme_begin_, lexeme_end_ ) + 1;
            syntax_tree_->negative_item_range( character, end_character );
        }
        else
        {
            syntax_tree_->negative_item_character( character );
        }
        return true;
    }
    return false;
}

bool RegexParser::match_character()
{
    const char* position = position_;
    if ( position != end_ )
    {
        if ( *position == '\\' )
        {
            ++position;
            if ( position != end_ && (*position == 'x' || *position == 'X') )
            {
                ++position;
                while ( position != end_ && isxdigit(*position) )
                {
                    ++position;
                }
            }
            else if ( position != end_ && isdigit(*position) )
            {
                ++position;
                while ( position != end_ && isdigit(*position) )
                {
                    ++position;
                }
            }
            else if ( position != end_ )
            {
                ++position;
            }
            lexeme_begin_ = position_;
            lexeme_end_ = position;
            position_ = position;
            return true;
        }
        else if ( !strchr("|*+?[]()-", *position) )
        {
            ++position;
            lexeme_begin_ = position_;
            lexeme_end_ = position;
            position_ = position;
            return true;
        }
    }
    return false;
}

bool RegexParser::match_end_of_range()
{
    const char* position = position_;
    if ( match("-") && match_character() )
    {
        return true;
    }
    position_ = position;
    return false;
}

bool RegexParser::match_identifier()
{
    const char* position = position_;
    if ( position != end_ && (isalpha(*position) || *position == '_') )
    {
        ++position;
        while ( position != end_ && (isalnum(*position) || *position == '_') )
        {
            ++position;
        }
        lexeme_begin_ = position_;
        lexeme_end_ = position;
        position_ = position;
        return true;
    }
    return false;
}

bool RegexParser::match( const char* lexeme )
{
    const char* position = position_;
    while ( position != end_ && *lexeme != 0 && *position == *lexeme )
    {
        ++position;
        ++lexeme;
    }
    if ( *lexeme == 0 )
    {
        lexeme_begin_ = position_;
        lexeme_end_ = position;
        position_ = position;
        return true;
    }
    return false;
}

bool RegexParser::expect( const char* lexeme )
{
    if ( match(lexeme) )
    {
        return true;
    }
    position_ = end_;
    successful_ = false;
    return false;
}

int RegexParser::escape( const char* start, const char* finish ) const
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
