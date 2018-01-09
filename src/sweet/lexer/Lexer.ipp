//
// Lexer.ipp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LEXER_LEXER_IPP_INCLUDED
#define SWEET_LEXER_LEXER_IPP_INCLUDED

#include "Lexer.hpp"
#include "LexerStateMachine.hpp"
#include "LexerState.hpp"
#include "LexerAction.hpp"
#include "LexerErrorPolicy.hpp"
#include "Error.hpp"
#include <sweet/assert/assert.hpp>

namespace sweet
{

namespace lexer
{

/**
// Constructor.
//
// @param action
//  The action that this handler is for.
//
// @param function
//  The function to call when the lexer action is recognized.
*/
template <class Iterator, class Char, class Traits, class Allocator>
Lexer<Iterator, Char, Traits, Allocator>::LexerActionHandler::LexerActionHandler( const LexerAction* action, LexerActionFunction function )
: action_( action ),
  function_( function )
{
    SWEET_ASSERT( action_ );
}

/**
// Constructor.
//
// @param state_machine
//  The state machine that this lexer will use or null to not initialize this 
//  lexer.
//
// @param end_symbol
//  The value to return to indicate that the end of the input has been 
//  reached.
//
// @param error_policy
//  The LexerErrorPolicy to use to report errors to or null to silently 
//  swallow errors.
*/
template <class Iterator, class Char, class Traits, class Allocator>
Lexer<Iterator, Char, Traits, Allocator>::Lexer( const LexerStateMachine* state_machine, const void* end_symbol, LexerErrorPolicy* error_policy )
: state_machine_( state_machine ),
  end_symbol_( end_symbol ),
  error_policy_( error_policy ),
  action_handlers_(),
  position_(),
  end_(),
  lexeme_(),
  symbol_( NULL ),
  full_( false )
{
    if ( state_machine_ )
    {
        action_handlers_.reserve( state_machine_->actions().size() );
        for ( std::vector<ptr<LexerAction> >::const_iterator i = state_machine_->actions().begin(); i != state_machine_->actions().end(); ++i )
        {
            LexerAction* action = i->get();
            SWEET_ASSERT( action );
            action_handlers_.push_back( LexerActionHandler(action, NULL) );
        }
    }
}

/**
// Set the action handler for \e identifier to \e function.
//
// @param identifier
//  The identifier of the action to set a handler for.
//
// @param function
//  The function to set as the handler.
*/
template <class Iterator, class Char, class Traits, class Allocator>
void Lexer<Iterator, Char, Traits, Allocator>::set_action_handler( const char* identifier, LexerActionFunction function )
{
    SWEET_ASSERT( identifier );
    
    typename std::vector<LexerActionHandler>::iterator action_handler = action_handlers_.begin();
    while ( action_handler != action_handlers_.end() && action_handler->action_->get_identifier() != identifier )
    {
        ++action_handler;
    }
    
    if ( action_handler != action_handlers_.end() )
    {
        action_handler->function_ = function;
    }
}

/**
// Get the most recently scanned lexeme.
//
// @return
//  The lexeme.
*/
template <class Iterator, class Char, class Traits, class Allocator>
const std::basic_string<Char, Traits, Allocator>& Lexer<Iterator, Char, Traits, Allocator>::lexeme() const
{
    return lexeme_;
}

/**
// Get the most recently scanned symbol.
//
// @return
//  The symbol or null if no symbol was recently matched.
*/
template <class Iterator, class Char, class Traits, class Allocator>
const void* Lexer<Iterator, Char, Traits, Allocator>::symbol() const
{
    return symbol_;
}

/**
// Get the current position of this Lexer.
//
// @return
//  The current position of this Lexer.
*/
template <class Iterator, class Char, class Traits, class Allocator>
const Iterator& Lexer<Iterator, Char, Traits, Allocator>::position() const
{
    return position_;
}

/**
// Had the full input been scanned?
//
// @return
//  True if the entire input has been scanned otherwise false.
*/
template <class Iterator, class Char, class Traits, class Allocator>
bool Lexer<Iterator, Char, Traits, Allocator>::full() const
{
    return full_;
}

/**
// Reset this %Lexer to scan [\e start, \e finish) starting its line count 
// from \e line.
//
// @param start
//  The first character in the input to scan.
//
// @param finish
//  One past the last character in the input to scan.
*/
template <class Iterator, class Char, class Traits, class Allocator>
void Lexer<Iterator, Char, Traits, Allocator>::reset( Iterator start, Iterator finish )
{
    lexeme_.clear();
    position_ = start;
    end_ = finish;
    symbol_ = NULL;
    full_ = false;
}

/**
// Advance one token in the input stream.
//
// @param data
//  The data that defines the state machine that is used in matching 
//  the next token.
//
// @param end_symbol
//  The symbol that is used to indicate that the end of the input
//  stream has been reached.
*/
template <class Iterator, class Char, class Traits, class Allocator>
void Lexer<Iterator, Char, Traits, Allocator>::advance()
{
    SWEET_ASSERT( state_machine_ );
    lexeme_.clear();
    skip();
    full_ = position_ == end_;
    symbol_ = position_ != end_ ? run() : end_symbol_;
}

/**
// Skip this %Lexer over its input using the state machine specified by 
// \e data.
*/
template <class Iterator, class Char, class Traits, class Allocator>
void Lexer<Iterator, Char, Traits, Allocator>::skip()
{    
    SWEET_ASSERT( state_machine_ );

    const LexerState* state = state_machine_->whitespace_start_state();
    if ( state )
    {
        const LexerTransition* transition = NULL;
        while ( position_ != end_ && (transition = state->find_transition_by_character(*position_)) )
        {
            state = transition->get_state();            
            if ( transition->get_action() )
            {
                int index = transition->get_action()->get_index();
                SWEET_ASSERT( index >= 0 && index < (int) action_handlers_.size() );                
                const LexerActionFunction& function = action_handlers_[index].function_;
                SWEET_ASSERT( function );
                const void* symbol = NULL;
                function( &position_, end_, &lexeme_, &symbol );
            }
            else
            {
                ++position_;
            }
        }        
    }
}

/**
// Run this %Lexer over its input using the state machine specified by 
// \e data.
//
// @param state
//  The start state of the state machine to use when running this %Lexer 
//  for this call.
//
// @return
//  The symbol that was matched or null if no symbol was matched from
//  the input.
*/
template <class Iterator, class Char, class Traits, class Allocator>
const void* Lexer<Iterator, Char, Traits, Allocator>::run()
{    
    SWEET_ASSERT( state_machine_ );
    SWEET_ASSERT( state_machine_->start_state() );
    
    const void* symbol = NULL;
    const LexerState* state = state_machine_->start_state();
    if ( state )
    {
        symbol = state->get_symbol();
        const LexerTransition* transition = NULL;
        while ( position_ != end_ && (transition = state->find_transition_by_character(*position_)) )
        {
            state = transition->get_state();
            symbol = state->get_symbol();
            
            if ( transition->get_action() )
            {
                int index = transition->get_action()->get_index();
                SWEET_ASSERT( index >= 0 && index < (int) action_handlers_.size() );                
                const LexerActionFunction& function = action_handlers_[index].function_;
                SWEET_ASSERT( function );
                function( &position_, end_, &lexeme_, &symbol );
            }
            else
            {
                lexeme_ += *position_;
                ++position_;
            }
        }
        
        if ( position_ != end_ && !symbol && lexeme_.empty() )
        {
            error();
        }
    }
    
    return state ? symbol : NULL;
}

/**
// Recover this %Lexer after a lexical %error to make sure that it can recognize
// the next character.
//
// This swallows input characters until a character is found that can be 
// transitioned on for the start state or the end of input is reached.
*/
template <class Iterator, class Char, class Traits, class Allocator>
void Lexer<Iterator, Char, Traits, Allocator>::error()
{   
    SWEET_ASSERT( state_machine_ );
    SWEET_ASSERT( state_machine_->start_state() );
    SWEET_ASSERT( position_ != end_ );
    
    if ( error_policy_ )
    {
        error_policy_->lexer_error( 0, LexerLexicalError("Lexical error on character '%c' (%d)", int(*position_), int(*position_)) );
    }
    
    const LexerTransition* transition = NULL;
    const LexerState* state = state_machine_->start_state();
    while ( position_ != end_ && !(transition = state->find_transition_by_character(*position_)) )
    {
        ++position_;
    }
}

}

}

#endif
