//
// LuaPreprocessReader.cpp
// Copyright (c) 2009 - 2010 Charles Baker.  All rights reserved.
//

#include "lua_types.hpp"
#include "LuaPreprocessReader.hpp"
#include <sweet/assert/assert.hpp>

using namespace sweet;
using namespace sweet::lua;

LuaPreprocessReader::LuaPreprocessReader( lua_Reader reader, void* context, size_t block_size )
: prefix_( PREFIX_KEYWORD ),
  prefix_end_( PREFIX_KEYWORD  + strlen(PREFIX_KEYWORD ) ),
  suffix_( SUFFIX_KEYWORD ),
  suffix_end_( SUFFIX_KEYWORD + strlen(SUFFIX_KEYWORD) ),
  pre_literal_( PRE_LITERAL_KEYWORD ),
  pre_literal_end_( PRE_LITERAL_KEYWORD + strlen(PRE_LITERAL_KEYWORD) ),
  post_literal_( POST_LITERAL_KEYWORD ),
  post_literal_end_( POST_LITERAL_KEYWORD + strlen(POST_LITERAL_KEYWORD) ),
  pre_expression_( PRE_EXPRESSION_KEYWORD ),
  pre_expression_end_( PRE_EXPRESSION_KEYWORD + strlen(PRE_EXPRESSION_KEYWORD) ),
  post_expression_( POST_EXPRESSION_KEYWORD ),
  post_expression_end_( POST_EXPRESSION_KEYWORD + strlen(POST_EXPRESSION_KEYWORD) ),
  reader_( reader ),
  context_( context ),
  state_( STATE_NULL ),
  block_( block_size, 0 ),
  position_( NULL ),
  position_end_( NULL ),
  source_( NULL ),
  source_end_( NULL ),
  destination_( &block_[0] ),
  destination_end_( &block_[0] + block_.size() )
{
    SWEET_ASSERT( prefix_ );
    SWEET_ASSERT( suffix_ );
    SWEET_ASSERT( pre_literal_ );
    SWEET_ASSERT( post_literal_ );
    SWEET_ASSERT( pre_expression_ );
    SWEET_ASSERT( post_expression_ );
    SWEET_ASSERT( reader_ );
}

void LuaPreprocessReader::set_prefix_and_suffix( const char* prefix, const char* suffix )
{
    SWEET_ASSERT( prefix );
    prefix_ = prefix;
    prefix_end_ = prefix + strlen( prefix );
    
    SWEET_ASSERT( suffix );
    suffix_ = suffix;
    suffix_end_ = suffix + strlen( suffix );
}

void LuaPreprocessReader::set_pre_and_post_literal( const char* pre_literal, const char* post_literal )
{
    SWEET_ASSERT( pre_literal );
    pre_literal_ = pre_literal;
    pre_literal_end_ = pre_literal + strlen( pre_literal );

    SWEET_ASSERT( post_literal );
    post_literal_ = post_literal;
    post_literal_end_ = post_literal + strlen( post_literal );
}

void LuaPreprocessReader::set_pre_and_post_expression( const char* pre_expression, const char* post_expression )
{
    SWEET_ASSERT( pre_expression );
    pre_expression_ = pre_expression;
    pre_expression_end_ = pre_expression + strlen( pre_expression );

    SWEET_ASSERT( post_expression );
    post_expression_ = post_expression;
    post_expression_end_ = post_expression + strlen( post_expression );
}

const char* LuaPreprocessReader::read( lua_State* lua_state, size_t* size )
{
    destination_ = &block_[0];
    destination_end_ = destination_ + block_.size();
    size_t source_size = 1;
    while ( destination_ < destination_end_ && source_size > 0 )
    {
        if ( position_ >= position_end_ )
        {
            SWEET_ASSERT( reader_ );
            position_ = (*reader_)( lua_state, context_, &source_size );
            position_end_ = position_ + source_size;
            
            if ( source_size == 0 )
            {
                if ( state_ == STATE_LITERAL )
                {
                    goto_post_literal();
                }
                else if ( state_ == STATE_EXPRESSION )
                {
                    goto_post_expression();
                }
            }
        }
    
        switch( state_ )
        {       
            case STATE_NULL:
                null();
                break;

            case STATE_LUA:
                lua();
                break;
            
            case STATE_PRE_LITERAL:
                pre_literal();
                break;
            
            case STATE_LITERAL:
                literal_();
                break;
            
            case STATE_POST_LITERAL:
                post_literal();
                break;
            
            case STATE_ESCAPE_EMBEDDED_POST_LITERAL:
                escape_embedded_post_literal();
                break;
            
            case STATE_PRE_EXPRESSION:
                pre_expression();
                break;
                
            case STATE_EXPRESSION:
                expression();
                break;
                
            case STATE_POST_EXPRESSION:
                post_expression();
                break;
                
            default:
                SWEET_ASSERT( false );
                state_ = STATE_NULL;
                break;
        }
    }

    SWEET_ASSERT( size );
    *size = destination_ - &block_[0];    
    return &block_[0];
}

const char* LuaPreprocessReader::reader( lua_State* lua_state, void* context, size_t* size )
{
    SWEET_ASSERT( context );
    LuaPreprocessReader* reader = reinterpret_cast<LuaPreprocessReader*>( context );
    return reader->read( lua_state, size );
}

void LuaPreprocessReader::null()
{
    SWEET_ASSERT( state_ == STATE_NULL );
    SWEET_ASSERT( destination_ >= &block_[0] && destination_ < destination_end_ );

    if ( position_ < position_end_ )
    {
        if ( is_prefix(position_, position_end_) )
        {
            position_ += prefix_end_ - prefix_;
            goto_lua();
        }
        else
        {
            goto_pre_literal();
        }
    }
}

void LuaPreprocessReader::lua()
{
    SWEET_ASSERT( state_ == STATE_LUA );
    SWEET_ASSERT( destination_ >= &block_[0] && destination_ < destination_end_ );

    while ( destination_ < destination_end_ && position_ < position_end_ && !is_suffix(position_, position_end_) )
    {
        *destination_ = *position_;
        ++position_;
        ++destination_;
    }
    
    if ( position_ < position_end_ && is_suffix(position_, position_end_) )
    {
        position_ += suffix_end_ - suffix_;
        goto_null();
    }    
}

void LuaPreprocessReader::pre_literal()
{
    SWEET_ASSERT( state_ == STATE_PRE_LITERAL );
    SWEET_ASSERT( position_ >= pre_literal_ && position_ < pre_literal_end_ );
    SWEET_ASSERT( destination_ >= &block_[0] && destination_ < destination_end_ );

    while ( destination_ < destination_end_ && position_ < position_end_ )
    {
        *destination_ = *position_;
        ++position_;
        ++destination_;
    }
    
    if ( position_ >= position_end_ )
    {
        goto_literal();
    }    
}

void LuaPreprocessReader::literal_()
{
    SWEET_ASSERT( state_ == STATE_LITERAL );
    SWEET_ASSERT( destination_ >= &block_[0] && destination_ < destination_end_ );

    while ( destination_ < destination_end_ && position_ < position_end_ && !is_post_literal(position_, position_end_)  && !is_prefix(position_, position_end_))
    {
        *destination_ = *position_;
        ++position_;
        ++destination_;
    }
    
    if ( destination_ < destination_end_ && position_ < position_end_ )
    {
        if ( is_prefix(position_, position_end_) )
        {
            position_ += prefix_end_ - prefix_;
            goto_post_literal();
        }
        else if ( is_post_literal(position_, position_end_) )
        {
            position_ += post_literal_end_ - post_literal_;
            goto_escape_embedded_post_literal();
        }
    }
}

void LuaPreprocessReader::post_literal()
{
    SWEET_ASSERT( state_ == STATE_POST_LITERAL );
    SWEET_ASSERT( position_ >= post_literal_ && position_ < post_literal_end_ );
    SWEET_ASSERT( destination_ >= &block_[0] && destination_ < destination_end_ );

    while ( destination_ < destination_end_ && position_ != position_end_ )
    {
        *destination_ = *position_;
        ++position_;
        ++destination_;
    }
    
    if ( position_ >= position_end_ )
    {
        pop_source();
        goto_lua();
    }    
}

void LuaPreprocessReader::escape_embedded_post_literal()
{
    SWEET_ASSERT( state_ == STATE_ESCAPE_EMBEDDED_POST_LITERAL );
    SWEET_ASSERT( position_ >= post_literal_ && position_ < post_literal_end_ );
    SWEET_ASSERT( destination_ >= &block_[0] && destination_ < destination_end_ );

//
// Escape the embedded post literal using the decimal values of the characters
// that it contains.  If the escape sequence would overflow the buffer without
// being fully written then move the end of the buffer to the current write
// position to force the read to return.  The end of the buffer will be reset 
// to the true end of buffer position on the next call to 
// LuaPreprocessReader::read().
//
    while ( destination_ < destination_end_ && position_ < position_end_ )
    {
        if ( destination_ + 4 < destination_end_ )
        {
            const char digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
            *(destination_ + 0) = '\\';
            *(destination_ + 1) = digits [*position_ / 100];
            *(destination_ + 2) = digits [*position_ % 100 / 10];
            *(destination_ + 3) = digits [*position_ % 100 % 10 / 1];
            ++position_;
            destination_ += 4;
        }
        else
        {
            destination_end_ = destination_;
        }
    }
    
    if ( position_ >= position_end_ )
    {
        goto_literal();
    }    
}

void LuaPreprocessReader::pre_expression()
{
    SWEET_ASSERT( state_ == STATE_PRE_EXPRESSION );
    SWEET_ASSERT( position_ >= pre_expression_ && position_ < pre_expression_end_ );
    SWEET_ASSERT( destination_ >= &block_[0] && destination_ < destination_end_ );

    while ( destination_ < destination_end_ && position_ < position_end_ )
    {
        *destination_ = *position_;
        ++position_;
        ++destination_;
    }
    
    if ( position_ >= position_end_ )
    {
        goto_expression();
    }    
}

void LuaPreprocessReader::expression()
{
    SWEET_ASSERT( state_ == STATE_EXPRESSION );
    SWEET_ASSERT( destination_ >= &block_[0] && destination_ < destination_end_ );

    while ( destination_ < destination_end_ && position_ < position_end_ && !is_suffix(position_, position_end_) )
    {
        *destination_ = *position_;
        ++position_;
        ++destination_;
    }
    
    if ( position_ < position_end_ && is_suffix(position_, position_end_) )
    {
        position_ += suffix_end_ - suffix_;
        goto_post_expression();
    }    
}

void LuaPreprocessReader::post_expression()
{
    SWEET_ASSERT( state_ == STATE_POST_EXPRESSION );
    SWEET_ASSERT( position_ >= post_expression_ && position_ < post_expression_end_ );
    SWEET_ASSERT( destination_ >= &block_[0] && destination_ < destination_end_ );

    while ( destination_ < destination_end_ && position_ != position_end_ )
    {
        *destination_ = *position_;
        ++position_;
        ++destination_;
    }
    
    if ( position_ >= position_end_ )
    {
        pop_source();
        goto_null();
    }    
}

void LuaPreprocessReader::goto_null()
{
    SWEET_ASSERT( state_ == STATE_LUA || state_ == STATE_POST_EXPRESSION );
    state_ = STATE_NULL;
}

void LuaPreprocessReader::goto_lua()
{
    SWEET_ASSERT( state_ == STATE_NULL || state_ == STATE_POST_LITERAL );

    if ( position_ < position_end_ && *position_ == '=' )
    {
        position_ += 1;
        push_source( pre_expression_, pre_expression_end_ );
        state_ = STATE_PRE_EXPRESSION;
    }
    else
    {
        state_ = STATE_LUA;
    }
}

void LuaPreprocessReader::goto_pre_literal()
{
    SWEET_ASSERT( state_ == STATE_NULL );
    push_source( pre_literal_, pre_literal_end_ );
    state_ = STATE_PRE_LITERAL;
}

void LuaPreprocessReader::goto_literal()
{
    SWEET_ASSERT( state_ == STATE_PRE_LITERAL || state_ == STATE_ESCAPE_EMBEDDED_POST_LITERAL );
    pop_source();
    state_ = STATE_LITERAL;
}

void LuaPreprocessReader::goto_escape_embedded_post_literal()
{
    SWEET_ASSERT( state_ == STATE_LITERAL );
    push_source( post_literal_, post_literal_end_ );
    state_ = STATE_ESCAPE_EMBEDDED_POST_LITERAL;
}

void LuaPreprocessReader::goto_post_literal()
{
    SWEET_ASSERT( state_ == STATE_LITERAL );
    push_source( post_literal_, post_literal_end_ );
    state_ = STATE_POST_LITERAL;
}

void LuaPreprocessReader::goto_pre_expression()
{
    SWEET_ASSERT( state_ == STATE_NULL );
    push_source( pre_expression_, pre_expression_end_ );
    state_ = STATE_PRE_EXPRESSION;
}

void LuaPreprocessReader::goto_expression()
{
    SWEET_ASSERT( state_ == STATE_PRE_EXPRESSION );
    pop_source();
    state_ = STATE_EXPRESSION;
}

void LuaPreprocessReader::goto_post_expression()
{
    SWEET_ASSERT( state_ == STATE_EXPRESSION );
    push_source( post_expression_, post_expression_end_ );
    state_ = STATE_POST_EXPRESSION;
}

void LuaPreprocessReader::push_source( const char* position, const char* position_end )
{
    SWEET_ASSERT( position );
    SWEET_ASSERT( position_end );

    source_ = position_;
    source_end_ = position_end_;
    position_ = position;
    position_end_ = position_end;
}

void LuaPreprocessReader::pop_source()
{
    position_ = source_;
    position_end_ = source_end_;
    source_ = NULL;
    source_end_ = NULL;
}

bool LuaPreprocessReader::is_prefix( const char* start, const char* finish ) const
{
    const char* prefix = prefix_;
    while ( prefix != prefix_end_ && start != finish && *prefix == *start )
    {
        ++prefix;
        ++start;
    }
    
    return prefix == prefix_end_;
}

bool LuaPreprocessReader::is_suffix( const char* start, const char* finish ) const
{
    const char* suffix = suffix_;
    while ( suffix != suffix_end_ && start != finish && *suffix == *start )
    {
        ++suffix;
        ++start;
    }
    
    return suffix == suffix_end_;
}

bool LuaPreprocessReader::is_post_literal( const char* start, const char* finish ) const
{
    const char* post_literal = post_literal_;
    while ( post_literal != post_literal_end_ && start != finish && *post_literal == *start )
    {
        ++post_literal;
        ++start;
    }
    
    return post_literal == post_literal_end_;
}

bool LuaPreprocessReader::is_post_expression( const char* start, const char* finish ) const
{
    const char* post_expression = post_expression_;
    while ( post_expression != post_expression_end_ && start != finish && *post_expression == *start )
    {
        ++post_expression;
        ++start;
    }
    
    return post_expression == post_expression_end_;
}
