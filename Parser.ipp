//
// Parser.ipp
// Copyright (c) Charles Baker. All rights reserved.
//

#ifndef SWEET_PARSER_PARSER_IPP_INCLUDED
#define SWEET_PARSER_PARSER_IPP_INCLUDED

#include "Parser.hpp"
#include "LalrState.hpp"
#include "LalrTransition.hpp"
#include "LalrAction.hpp"
#include "LalrProduction.hpp"
#include "LalrSymbol.hpp"
#include "ParserErrorPolicy.hpp"
#include "ParserStateMachine.hpp"
#include "ErrorCode.hpp"
#include "ParserNode.ipp"
#include "ParserUserData.ipp"
#include "AddParserActionHandler.ipp"
#include "Lexer.ipp"
#include "AddLexerActionHandler.ipp"
#include "assert.hpp"
#include <stdarg.h>
#include <stdio.h>

namespace sweet
{

namespace lalr
{

/**
// Constructor.
//
// @param identifier
//  The identifier of the action handler.
//
// @param function
//  The function to call when matching a reduction for this action handler.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
Parser<Iterator, UserData, Char, Traits, Allocator>::LalrActionHandler::LalrActionHandler( const LalrAction* action, LalrActionFunction function )
: action_( action ),
  function_( function )
{
}

/**
// Constructor.
//
// @param state_machine
//  The state machine and actions that this %Parser will use (assumed not 
//  null).
//
// @param error_policy
//  The error policy to notify syntax errors and debug information to or null 
//  to silently swallow syntax errors and print debug information to stdout.
//
// @param lexer_error_policy
//  The error policy to notifiy errors from the lexer to or null to silently
//  swallow lexical errors.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
Parser<Iterator, UserData, Char, Traits, Allocator>::Parser( const ParserStateMachine* state_machine, ParserErrorPolicy* error_policy, LexerErrorPolicy* lexer_error_policy )
: state_machine_( state_machine ),
  error_policy_( error_policy ),
  nodes_(),
  lexer_( state_machine_->lexer_state_machine(), state_machine_->end_symbol(), lexer_error_policy ),
  action_handlers_(),
  default_action_handler_( NULL ),
  debug_enabled_( false ),
  accepted_( false ),
  full_( false )
{
    SWEET_ASSERT( state_machine_ );
    
    action_handlers_.reserve( state_machine_->actions().size() );
    for ( std::vector<std::unique_ptr<LalrAction> >::const_iterator i = state_machine_->actions().begin(); i != state_machine_->actions().end(); ++i )
    {
        LalrAction* action = i->get();
        SWEET_ASSERT( action );
        action_handlers_.push_back( LalrActionHandler(action, NULL) );
    }

    nodes_.reserve( 64 );       
    nodes_.push_back( ParserNode(state_machine_->start_state(), NULL, UserData()) );
}

/**
// Reset this Parser so that it can parse another sequence of input.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void Parser<Iterator, UserData, Char, Traits, Allocator>::reset()
{
    accepted_ = false;
    full_ = false;
    nodes_.clear();
    nodes_.push_back( ParserNode(state_machine_->start_state(), NULL, UserData()) );
}

/**
// Parse [\e start, \e finish).
//
// After the parse the Parser::full() and Parser::accepted() functions can 
// be used to determine whether or not the parse was successful and whether
// or not it consumed all of the available input.
//
// In the case of a successful parse the Parser::user_data() function can
// be used to retrieve the user data that resulted from the parse.
//
// @param start
//  The first character in the sequence to parse.
//
// @param finish
//  One past the last character in the sequence to parse.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void Parser<Iterator, UserData, Char, Traits, Allocator>::parse( Iterator start, Iterator finish )
{
    SWEET_ASSERT( state_machine_ );

    reset();
    lexer_.reset( start, finish );    
    lexer_.advance();
    const LalrSymbol* symbol = reinterpret_cast<const LalrSymbol*>( lexer_.symbol() );
    while ( parse(symbol, lexer_.lexeme()) )
    {
        lexer_.advance();
        symbol = reinterpret_cast<const LalrSymbol*>( lexer_.symbol() );
    }

    full_ = lexer_.full();
}

/**
// Continue a parse by accepting \e symbol as the next token.
//
// @param symbol
//  The next token from the lexical analyzer in the current parse (assumed to
//  be a LalrSymbol).
//
// @param lexeme
//  The lexeme of the next token from the lexical analyzer.
//
// @return
//  True until parsing is complete or an error occurs.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
bool Parser<Iterator, UserData, Char, Traits, Allocator>::parse( const void* symbol, const std::basic_string<Char, Traits, Allocator>& lexeme )
{
    return parse( reinterpret_cast<const LalrSymbol*>(symbol), lexeme );
}

/**
// Continue a parse by accepting \e symbol as the next token.
//
// @param symbol
//  The next token from the lexical analyzer in the current parse.
//
// @param lexeme
//  The lexeme of the next token from the lexical analyzer.
//
// @return
//  True until parsing is complete or an error occurs.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
bool Parser<Iterator, UserData, Char, Traits, Allocator>::parse( const LalrSymbol* symbol, const std::basic_string<Char, Traits, Allocator>& lexeme )
{
    bool accepted = false;
    bool rejected = false;
    
    const LalrTransition* transition = find_transition( symbol, nodes_.back().state() );
    while ( !accepted && !rejected && transition && transition->get_type() == TRANSITION_REDUCE )
    {
        reduce( transition, &accepted, &rejected );
        transition = find_transition( symbol, nodes_.back().state() );
    }
    
    if ( transition && transition->get_type() == TRANSITION_SHIFT )
    {
        shift( transition, lexeme );
    }
    else
    {
        error( &accepted, &rejected );
    }
    
    accepted_ = accepted;
    return !accepted_ && !rejected;
}

/**
// Did the most recent parse accept input successfully?
//
// @return
//  True if the input was parsed successfully otherwise false.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
bool Parser<Iterator, UserData, Char, Traits, Allocator>::accepted() const
{
    return accepted_;
}

/**
// Did the most recent parse consume all of the input?
//
// @return
//  True if all of the input was consumed otherwise false.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
bool Parser<Iterator, UserData, Char, Traits, Allocator>::full() const
{
    return full_;
}

/**
// Get the user data that resulted from the most recent call to 
// Parser::parser() on this %Parser.
//
// Assumes that the most recent parse was accepted.
//
// @return
//  The user data.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
const UserData& Parser<Iterator, UserData, Char, Traits, Allocator>::user_data() const
{
    SWEET_ASSERT( accepted() );
    SWEET_ASSERT( nodes_.size() == 1 );
    return nodes_.front().user_data();
}

/**
// Get the position that this %Parser is up to.
//
// @return
//  The iterator at the position that this %Parser is up to.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
const Iterator& Parser<Iterator, UserData, Char, Traits, Allocator>::position() const
{
    return lexer_.position();
}

/**
// Add action handlers to this %Parser.
//
// @return
//  An %AddParserActionHandler helper that provides a convenient syntax for adding
//  action handlers to this %Parser.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
AddParserActionHandler<Iterator, UserData, Char, Traits, Allocator> Parser<Iterator, UserData, Char, Traits, Allocator>::parser_action_handlers()
{
    return AddParserActionHandler<Iterator, UserData, Char, Traits, Allocator>( this );
}

/**
// Add action handlers to this %Parser's %Lexer.
//
// @return
//  An %AddLexerActionHandler helper that provides a convenient syntax for
//  adding action handlers to the %Lexer.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
AddLexerActionHandler<Iterator, Char, Traits, Allocator> Parser<Iterator, UserData, Char, Traits, Allocator>::lexer_action_handlers()
{
    return AddLexerActionHandler<Iterator, Char, Traits, Allocator>( &lexer_ );
}

/**
// Set the default action handler for this %Parser to \e function.
//
// @param function
//  The function to set the default action handler for this %Parser to or
//  null to set this %Parser to have no default action handler.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void Parser<Iterator, UserData, Char, Traits, Allocator>::set_default_action_handler( LalrActionFunction function )
{
    default_action_handler_ = function;
}

/**
// Set the action handler for \e identifier to \e function.
//
// @param identifier
//  The identifier of the action handler to set the function for.
//
// @param function
//  The function to set the action handler to or null to set the action 
//  handler to have no function.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void Parser<Iterator, UserData, Char, Traits, Allocator>::set_action_handler( const char* identifier, LalrActionFunction function )
{
    SWEET_ASSERT( identifier );
    
    typename std::vector<LalrActionHandler>::iterator action_handler = action_handlers_.begin();
    while ( action_handler != action_handlers_.end() && strcmp(action_handler->action_->identifier, identifier) != 0 )
    {
        ++action_handler;
    }
    
    if ( action_handler != action_handlers_.end() )
    {
        action_handler->function_ = function;
    }
}

/**
// Fire an %error event.
//
// @param line
//  The line number to associate with the %error (or 0 if there is no line
//  to associate with the %error).
//
// @param error
//  The %Error that describes the %error that has occured.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void Parser<Iterator, UserData, Char, Traits, Allocator>::fire_error( int error, const char* format, ... ) const
{
    if ( error_policy_ )
    {
        va_list args;
        va_start( args, format );
        error_policy_->parser_error( 0, error, format, args );
        va_end( args );
    }
}

/**
// Fire a printf event.
//
// @param format
//  A printf style format string that describes the message to print.
//
// @param ...
//  Parameters to fill in the message as specified by \e format.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void Parser<Iterator, UserData, Char, Traits, Allocator>::fire_printf( const char* format, ... ) const
{
    if ( error_policy_ )
    {
        SWEET_ASSERT( format );
        va_list args;
        va_start( args, format );
        error_policy_->parser_vprintf( format, args );
        va_end( args );
    }
    else
    {
        va_list args;
        va_start( args, format );
        vfprintf( stdout, format, args );
        va_end( args );
    }
}

/**
// Set whether or not shift operations are printed.
//
// @param debug_enabled
//  True to cause any shift or reduce operations to be printed or false to 
//  suppress this behaviour.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void Parser<Iterator, UserData, Char, Traits, Allocator>::set_debug_enabled( bool debug_enabled )
{
    debug_enabled_ = debug_enabled;
}

/**
// Are shift and reduce operations printed?
//
// @return
//  True if shift and reduce operations are printed otherwise false.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
bool Parser<Iterator, UserData, Char, Traits, Allocator>::is_debug_enabled() const
{
    return debug_enabled_;
}

/**
// Find the Transition for \e symbol in \e state.
//
// @param symbol
//  The symbol to find the transition for.
//
// @param state
//  The state to search for transitions in (assumed not null).
//
// @return
//  The transition to take on \e symbol or null if there was no such transition from
//  \e state.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
const LalrTransition* Parser<Iterator, UserData, Char, Traits, Allocator>::find_transition( const LalrSymbol* symbol, const LalrState* state ) const
{
    SWEET_ASSERT( state );
    SWEET_ASSERT( state_machine_ );
    const LalrTransition* transition = state->find_transition_by_symbol( symbol );
    return transition;
}

/**
// @internal
//
// Find the Node to reduce to when \e production is reduced.
//
// @param transition
//  The reducing transition.
//
// @param nodes
//  The ParserNodes currently on the stack.
//
// @return
//  The iterator to the Node to reduce to or the end iterator for \e nodes
//  if no Node to reduce to could be found.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
typename std::vector<ParserNode<UserData, Char, Traits, Allocator> >::iterator Parser<Iterator, UserData, Char, Traits, Allocator>::find_node_to_reduce_to( const LalrTransition* transition, std::vector<ParserNode>& nodes )
{
    SWEET_ASSERT( transition );
    SWEET_ASSERT( transition->reduced_length() < int(nodes.size()) );
    typename std::vector<ParserNode>::reverse_iterator node = nodes.rbegin() + transition->reduced_length();
    return node != nodes.rend() ? node.base() : nodes_.begin();
}

/**
// Debug a shift operation.
//
// @param node
//  The ParserNode that has been shifted onto the stack.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void Parser<Iterator, UserData, Char, Traits, Allocator>::debug_shift( const ParserNode& node ) const
{
    if ( debug_enabled_ )
    {
        const LalrSymbol* symbol = node.symbol();
        const std::string& lexeme = node.lexeme();
        fire_printf( "SHIFT: (%s %s)\n", symbol ? symbol->get_identifier().c_str() : "", lexeme.c_str() );
    }
}

/**
// Debug a reduce operation.
//
// @param reduced_symbol
//  The symbol that will be reduced to.
//
// @param start
//  The first ParserNode in the stack that will be reduced.
//
// @param finish
//  One past the last ParserNode in the stack that will be reduced.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void Parser<Iterator, UserData, Char, Traits, Allocator>::debug_reduce( const LalrSymbol* reduced_symbol, const ParserNode* start, const ParserNode* finish ) const
{
    SWEET_ASSERT( start );
    SWEET_ASSERT( finish );
    SWEET_ASSERT( start <= finish );
    SWEET_ASSERT( reduced_symbol );

    if ( debug_enabled_ )
    {
        fire_printf( "REDUCE: %s <- ", reduced_symbol->get_identifier().c_str() );        

        const ParserNode* node = start; 
        if ( node != finish )
        {
            const LalrSymbol* symbol = node->symbol();
            const std::string& lexeme = node->lexeme();
            fire_printf( "(%s %s)", symbol ? symbol->get_identifier().c_str() : "", lexeme.c_str() );
            ++node;
        }
        
        while ( node != finish )
        {
            const LalrSymbol* symbol = node->symbol();
            const std::string& lexeme = node->lexeme();
            fire_printf( " (%s %s)", symbol ? symbol->get_identifier().c_str() : "", lexeme.c_str() );
            ++node;
        }
        
        fire_printf( "\n" );
    }
}

/**
// Handle any parser action that needs to be taken when \e reduced_production
// is reduced.
//
// @param reduced_producion
//  The production that has been reduced.
//
// @param start
//  The first ParserNode in the stack that will be reduced.
//
// @param finish
//  One past the last ParserNode in the stack that will be reduced.
//
// @return
//  The user data that results from the reduction.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
UserData Parser<Iterator, UserData, Char, Traits, Allocator>::handle( const LalrTransition* transition, const ParserNode* start, const ParserNode* finish ) const
{
    SWEET_ASSERT( start );
    SWEET_ASSERT( finish );
    SWEET_ASSERT( start <= finish );
    SWEET_ASSERT( transition );

    int action = transition->action();
    if ( action != LalrAction::INVALID_INDEX )
    {
        SWEET_ASSERT( action >= 0 && action < static_cast<int>(action_handlers_.size()) );            
        if ( action_handlers_[action].function_ )
        {
            return action_handlers_[action].function_( transition->reduced_symbol(), start, finish );
        }
    }

    return default_action_handler_ ? default_action_handler_( transition->reduced_symbol(), start, finish ) : UserData();
}

/**
// Shift the current token onto the stack.
//
// @param transition
//  The shift transition that specifies the state that will be transitioned
//  into after the shift and the productions that were potentially started
//  at this point.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void Parser<Iterator, UserData, Char, Traits, Allocator>::shift( const LalrTransition* transition, const std::basic_string<Char, Traits, Allocator>& lexeme )
{
    SWEET_ASSERT( state_machine_ );
    SWEET_ASSERT( transition );    
    ParserNode node( transition->get_state(), transition->get_symbol(), lexeme );
    debug_shift( node );
    nodes_.push_back( node );
}

/**
// Reduce the current stack.
//
// @param transition
//  The transition that specifies the production that is to be reduced.
//
// @param accepted
//  A variable to receive whether or not this Parser is still accepting input.
//
// @param rejected
//  A variable to receive whether or not this Parser has rejected its input.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void Parser<Iterator, UserData, Char, Traits, Allocator>::reduce( const LalrTransition* transition, bool* accepted, bool* /*rejected*/ )
{
    SWEET_ASSERT( state_machine_ );
    SWEET_ASSERT( transition );
    SWEET_ASSERT( accepted );
    
    const LalrSymbol* symbol = transition->reduced_symbol();
    if ( symbol != state_machine_->start_symbol() )
    {
        typename std::vector<ParserNode>::iterator i = find_node_to_reduce_to( transition, nodes_ );
        const ParserNode* start = i != nodes_.end() ? &(*i) : &nodes_.back() + 1;
        const ParserNode* finish = &nodes_.back() + 1;

        debug_reduce( transition->reduced_symbol(), start, finish );
        UserData user_data = handle( transition, start, finish );
        nodes_.erase( i, nodes_.end() );
        const LalrTransition* transition = find_transition( symbol, nodes_.back().state() );
        SWEET_ASSERT( transition );
        ParserNode node( transition->get_state(), symbol, user_data );
        nodes_.push_back( node );
    }
    else
    {    
        SWEET_ASSERT( nodes_.size() == 2 );
        nodes_.erase( nodes_.begin() );
        *accepted = true;
    }              
}

/**
// Handle an error.
//
// Pops states from the stack until the 'error' token can be shifted and then
// shifts the error token.  Any transitions that call for a reduce on the 
// 'error' token are taken.
//
// @param accepted
//  A variable to receive whether or not this Parser is still accepting input.
//
// @param rejected
//  A variable to receive whether or not this Parser has rejected its input.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void Parser<Iterator, UserData, Char, Traits, Allocator>::error( bool* accepted, bool* rejected )
{
    SWEET_ASSERT( state_machine_ );
    SWEET_ASSERT( !nodes_.empty() );
    SWEET_ASSERT( accepted );
    SWEET_ASSERT( rejected );

    bool handled = false;
    while ( !nodes_.empty() && !handled && !*accepted && !*rejected )
    {
        const LalrTransition* transition = find_transition( state_machine_->error_symbol(), nodes_.back().state() );
        if ( transition )
        {
            switch ( transition->get_type() )
            {
                case TRANSITION_SHIFT:
                    shift( transition, std::basic_string<Char, Traits, Allocator>() );
                    handled = true;
                    break;

                case TRANSITION_REDUCE:
                    reduce( transition, accepted, rejected );
                    break;
                    
                default:
                    SWEET_ASSERT( false );
                    fire_error( PARSER_ERROR_UNEXPECTED, "Unexpected transition type '%d'", transition->get_type() );
                    *rejected = true;
                    break;
            }
        }
        else
        {
            nodes_.pop_back();
        }
    }
    
    if ( nodes_.empty() )
    {
        fire_error( PARSER_ERROR_SYNTAX, "Syntax error" );
        *rejected = true;
    }
}

}

}

#endif
