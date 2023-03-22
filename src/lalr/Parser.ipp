//
// Parser.ipp
// Copyright (c) Charles Baker. All rights reserved.
//

#ifndef LALR_PARSER_IPP_INCLUDED
#define LALR_PARSER_IPP_INCLUDED

#include "Parser.hpp"
#include "ParserState.hpp"
#include "ParserTransition.hpp"
#include "ParserAction.hpp"
#include "ParserSymbol.hpp"
#include "ParserStateMachine.hpp"
#include "ErrorCode.hpp"
#include "ParserNode.ipp"
#include "ParserUserData.ipp"
#include "AddParserActionHandler.ipp"
#include "Lexer.ipp"
#include "AddLexerActionHandler.ipp"
#include "ErrorPolicy.hpp"
#include "assert.hpp"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

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
Parser<Iterator, UserData, Char, Traits, Allocator>::ParserActionHandler::ParserActionHandler( const ParserAction* action, ParserActionFunction function )
: action_( action )
, function_( function )
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
Parser<Iterator, UserData, Char, Traits, Allocator>::Parser( const ParserStateMachine* state_machine, ErrorPolicy* error_policy )
: state_machine_( state_machine )
, error_policy_( error_policy )
, nodes_()
, user_data_()
, lexer_( state_machine_->lexer_state_machine, state_machine_->whitespace_lexer_state_machine, state_machine_->end_symbol, error_policy )
, action_handlers_()
, default_action_handler_( nullptr )
, debug_enabled_( false )
, accepted_( false )
, full_( false )
{
    LALR_ASSERT( state_machine_ );
    
    action_handlers_.reserve( state_machine_->actions_size );
    const ParserAction* action = state_machine_->actions;
    const ParserAction* actions_end = action + state_machine_->actions_size;
    while ( action != actions_end )
    {
        action_handlers_.push_back( ParserActionHandler(action, nullptr) );
        ++action;
    }

    nodes_.reserve( 64 );       
    user_data_.reserve( 64 );       
    nodes_.push_back( ParserNode(state_machine_->start_state, nullptr, 0, 1) );
    user_data_.push_back( UserData() );
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
    user_data_.clear();
    nodes_.push_back( ParserNode(state_machine_->start_state, nullptr, 0, 1) );
    user_data_.push_back( UserData() );
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
    LALR_ASSERT( state_machine_ );

    reset();
    lexer_.reset( start, finish );    
    lexer_.advance();
    const ParserSymbol* symbol = reinterpret_cast<const ParserSymbol*>( lexer_.symbol() );
    while ( parse(symbol, lexer_.lexeme(), lexer_.line(), lexer_.column()) )
    {
        lexer_.advance();
        symbol = reinterpret_cast<const ParserSymbol*>( lexer_.symbol() );
    }

    full_ = lexer_.full();
}

/**
// Continue a parse by accepting \e symbol as the next token.
//
// @param symbol
//  The next token from the lexical analyzer in the current parse (assumed to
//  be a ParserSymbol).
//
// @param lexeme
//  The lexeme of the next token from the lexical analyzer.
//
// @param line
//  The line number at the start of the next token.
//
// @return
//  True until parsing is complete or an error occurs.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
bool Parser<Iterator, UserData, Char, Traits, Allocator>::parse( const void* symbol, const std::basic_string<Char, Traits, Allocator>& lexeme, int line, int column )
{
    return parse( reinterpret_cast<const ParserSymbol*>(symbol), lexeme, line, column );
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
// @param line
//  The line number at the start of the next token.
//
// @return
//  True until parsing is complete or an error occurs.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
bool Parser<Iterator, UserData, Char, Traits, Allocator>::parse( const ParserSymbol* symbol, const std::basic_string<Char, Traits, Allocator>& lexeme, int line, int column )
{
    bool accepted = false;
    bool rejected = false;
    
    const ParserTransition* transition = find_transition( symbol, nodes_.back().state() );
    while ( !accepted && !rejected && transition && transition->type == TRANSITION_REDUCE )
    {
        reduce( transition, &accepted, &rejected );
        transition = find_transition( symbol, nodes_.back().state() );
    }
    
    if ( transition && transition->type == TRANSITION_SHIFT )
    {
        shift( transition, lexeme, line, column );
    }
    else
    {
        error( &accepted, &rejected, line, column);
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
    LALR_ASSERT( accepted() );
    LALR_ASSERT( nodes_.size() == 1 );
    LALR_ASSERT( user_data_.size() == 1 );
    return user_data_.front();
}


/**
// Get the Lexer that is being used by this Parser.
//
// @return
//  The Lexer used by this Parser.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
const Lexer<Iterator, Char, Traits, Allocator>& Parser<Iterator, UserData, Char, Traits, Allocator>::lexer() const
{
    return lexer_;
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
void Parser<Iterator, UserData, Char, Traits, Allocator>::set_default_action_handler( ParserActionFunction function )
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
void Parser<Iterator, UserData, Char, Traits, Allocator>::set_action_handler( const char* identifier, ParserActionFunction function )
{
    LALR_ASSERT( identifier );
    
    typename std::vector<ParserActionHandler>::iterator action_handler = action_handlers_.begin();
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
// Set the lexer action handler for \e identifier to \e function.
//
// @param identifier
//  The identifier of the action handler to set the function for.
//
// @param function
//  The function to set the action handler to or null to set the action 
//  handler to have no function.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void Parser<Iterator, UserData, Char, Traits, Allocator>::set_lexer_action_handler( const char* identifier, LexerActionFunction function )
{
    LALR_ASSERT( identifier );
    lexer_.set_action_handler( identifier, function );
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
void Parser<Iterator, UserData, Char, Traits, Allocator>::fire_error( int line, int column, int error, const char* format, ... ) const
{
    if ( error_policy_ )
    {
        va_list args;
        va_start( args, format );
        error_policy_->lalr_error( line, column, error, format, args );
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
        LALR_ASSERT( format );
        va_list args;
        va_start( args, format );
        error_policy_->lalr_vprintf( format, args );
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
const ParserTransition* Parser<Iterator, UserData, Char, Traits, Allocator>::find_transition( const ParserSymbol* symbol, const ParserState* state ) const
{
    LALR_ASSERT( state );
    LALR_ASSERT( state_machine_ );
    const ParserTransition* transition = state->transitions;
    const ParserTransition* transitions_end = state->transitions + state->length;
    while ( transition != transitions_end && transition->symbol != symbol )
    {
        ++transition;
    }
    return transition != transitions_end ? transition : nullptr;
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
typename std::vector<ParserNode<Char, Traits, Allocator> >::iterator Parser<Iterator, UserData, Char, Traits, Allocator>::find_node_to_reduce_to( const ParserTransition* transition, std::vector<ParserNode>& nodes )
{
    LALR_ASSERT( transition );
    LALR_ASSERT( transition->reduced_length < int(nodes.size()) );
    typename std::vector<ParserNode>::reverse_iterator node = nodes.rbegin() + transition->reduced_length;
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
        const ParserSymbol* symbol = node.symbol();
        const std::string& lexeme = node.lexeme();
        int line = node.line();
        int column = node.column();
        fire_printf( "SHIFT: (%s %s %d:%d)\n", symbol ? symbol->identifier : "", lexeme.c_str(), line, column );
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
void Parser<Iterator, UserData, Char, Traits, Allocator>::debug_reduce( const ParserSymbol* reduced_symbol, std::ptrdiff_t start, std::ptrdiff_t finish ) const
{
    LALR_ASSERT( start <= finish );
    LALR_ASSERT( reduced_symbol );

    if ( debug_enabled_ )
    {
        fire_printf( "REDUCE: %s <- ", reduced_symbol->identifier );        

        const ParserNode* node = nodes_.data() + start;
        const ParserNode* node_end = nodes_.data() + finish;
        if ( node != node_end )
        {
            const ParserSymbol* symbol = node->symbol();
            const std::string& lexeme = node->lexeme();
            int line = node->line();
            int column = node->column();
            fire_printf( "(%s %s %d:%d)", symbol ? symbol->identifier : "", lexeme.c_str(), line, column );
            ++node;
        }
        
        while ( node != node_end )
        {
            const ParserSymbol* symbol = node->symbol();
            const std::string& lexeme = node->lexeme();
            int line = node->line();
            int column = node->column();
            fire_printf( " (%s %s %d:%d)", symbol ? symbol->identifier : "", lexeme.c_str(), line, column );
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
UserData Parser<Iterator, UserData, Char, Traits, Allocator>::handle( const ParserTransition* transition, std::ptrdiff_t start, std::ptrdiff_t finish ) const
{
    LALR_ASSERT( start >= 0 && size_t(start) <= nodes_.size() );
    LALR_ASSERT( start >= 0 && size_t(start) <= user_data_.size() );
    LALR_ASSERT( finish >= 0 && size_t(finish) <= nodes_.size() );
    LALR_ASSERT( finish >= 0 && size_t(finish) <= user_data_.size() );
    LALR_ASSERT( start <= finish );
    LALR_ASSERT( transition );

    const UserData* user_data = size_t(start) < user_data_.size() ? &user_data_[start] : nullptr;
    const ParserNode* nodes = size_t(start) < nodes_.size() ? &nodes_[start] : nullptr;
    size_t length = finish - start;

    int action = transition->action;
    if ( action != ParserAction::INVALID_INDEX )
    {
        LALR_ASSERT( action >= 0 && action < static_cast<int>(action_handlers_.size()) );            
        if ( action_handlers_[action].function_ )
        {
            return action_handlers_[action].function_( user_data, nodes, length );
        }
    }

    if ( default_action_handler_ )
    {
        return default_action_handler_( user_data, nodes, length );
    }

    return UserData();
}

/**
// Generate a list of expected symbols to report in an error.
//
// Generates a list of the symbols that transition out of *state* so that
// they can be reported when an unhandled syntax error is encountered.
//
// @return
//  A string containing the expected symbols.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
std::string Parser<Iterator, UserData, Char, Traits, Allocator>::expected_symbols( const ParserState* state ) const
{
    std::string expected_symbols;
    if ( state )
    {
        for ( int i = 0; i < state->length; ++i )
        {
            const ParserTransition* transition = &state->transitions[i];
            if ( transition->symbol )
            {
                if ( !expected_symbols.empty() )
                {
                    expected_symbols += ", ";
                }
                expected_symbols += transition->symbol->identifier;
            }
        }
    }
    return expected_symbols;
}

/**
// Shift the current token onto the stack.
//
// @param transition
//  The shift transition that specifies the state that will be transitioned
//  into after the shift and the productions that were potentially started
//  at this point.
//
// @param lexeme
//  The lexeme of the token that is being shifted onto the stack.
//
// @param line
//  The line number at the start of the token that is being shifted onto the 
//  stack (assumed >= 0).
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void Parser<Iterator, UserData, Char, Traits, Allocator>::shift( const ParserTransition* transition, const std::basic_string<Char, Traits, Allocator>& lexeme, int line, int column )
{
    LALR_ASSERT( state_machine_ );
    LALR_ASSERT( transition );
    LALR_ASSERT( line >= 0 );
    LALR_ASSERT( column >= 1 );
    ParserNode node( transition->state, transition->symbol, lexeme, line, column );
    debug_shift( node );
    nodes_.push_back( node );
    user_data_.push_back( UserData() );
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
void Parser<Iterator, UserData, Char, Traits, Allocator>::reduce( const ParserTransition* transition, bool* accepted, bool* /*rejected*/ )
{
    LALR_ASSERT( state_machine_ );
    LALR_ASSERT( transition );
    LALR_ASSERT( accepted );
    
    const ParserSymbol* symbol = transition->reduced_symbol;
    if ( symbol != state_machine_->start_symbol )
    {
        typename std::vector<ParserNode>::iterator i = find_node_to_reduce_to( transition, nodes_ );
        std::ptrdiff_t start = i - nodes_.begin();
        std::ptrdiff_t finish = nodes_.end() - nodes_.begin();

        debug_reduce( transition->reduced_symbol, start, finish );
        int line = i != nodes_.end() ? nodes_[start].line() : 0;
        int column = i != nodes_.end() ? nodes_[start].column() : 1;
        UserData user_data = handle( transition, start, finish );
        nodes_.erase( nodes_.begin() + start, nodes_.end() );
        user_data_.erase( user_data_.begin() + start, user_data_.end() );
        const ParserTransition* transition = find_transition( symbol, nodes_.back().state() );
        LALR_ASSERT( transition );
        ParserNode node( transition->state, symbol, line, column );
        nodes_.push_back( node );
        user_data_.push_back( user_data );
    }
    else
    {    
        LALR_ASSERT( nodes_.size() == 2 );
        LALR_ASSERT( user_data_.size() == 2 );
        nodes_.erase( nodes_.begin() );
        user_data_.erase( user_data_.begin() );
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
void Parser<Iterator, UserData, Char, Traits, Allocator>::error( bool* accepted, bool* rejected, int line, int column )
{
    LALR_ASSERT( state_machine_ );
    LALR_ASSERT( !nodes_.empty() );
    LALR_ASSERT( !user_data_.empty() );
    LALR_ASSERT( accepted );
    LALR_ASSERT( rejected );

    const std::basic_string<Char, Traits, Allocator> lexeme = lexer_.lexeme();
    const ParserSymbol* symbol = reinterpret_cast<const ParserSymbol*>( lexer_.symbol() );
    const ParserState* state = !nodes_.empty() ? nodes_.back().state() : nullptr;

    bool handled = false;
    while ( !nodes_.empty() && !handled && !*accepted && !*rejected )
    {
        const ParserTransition* transition = find_transition( state_machine_->error_symbol, nodes_.back().state() );
        if ( transition )
        {
            switch ( transition->type )
            {
                case TRANSITION_SHIFT:
                    shift( transition, std::basic_string<Char, Traits, Allocator>(), line, column );
                    handled = true;
                    break;

                case TRANSITION_REDUCE:
                    reduce( transition, accepted, rejected );
                    break;
                    
                default:
                    LALR_ASSERT( false );
                    fire_error( line, column, PARSER_ERROR_UNEXPECTED, "Unexpected transition type '%d'", transition->type );
                    *rejected = true;
                    break;
            }
        }
        else
        {
            nodes_.pop_back();
            user_data_.pop_back();
        }
    }
    
    if ( nodes_.empty() )
    {
        if ( symbol )
        {
            fire_error( line, column, PARSER_ERROR_SYNTAX, "Syntax error on %s '%s' when expecting %s", symbol->identifier, lexeme.c_str(), expected_symbols(state).c_str() );
        }
        else
        {
            fire_error( line, column, PARSER_ERROR_SYNTAX, "Syntax error on '%s' when expecting %s", lexeme.c_str(), expected_symbols(state).c_str() );
        }
        *rejected = true;
    }
}

}

#endif
