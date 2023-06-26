//
// RegexGenerator.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "RegexGenerator.hpp"
#include "ErrorCode.hpp"
#include "RegexToken.hpp"
#include "RegexCompiler.hpp"
#include "LexerState.hpp"
#include "LexerTransition.hpp"
#include "LexerAction.hpp"
#include "RegexItem.hpp"
#include "RegexState.hpp"
#include "RegexAction.hpp"
#include "RegexNode.hpp"
#include "RegexSyntaxTree.hpp"
#include "RegexParser.hpp"
#include "ErrorPolicy.hpp"
#include "assert.hpp"
#include <limits.h>

using std::set;
using std::pair;
using std::vector;
using std::make_pair;
using std::unique_ptr;
using std::make_unique;
using namespace lalr;

/**
// Constructor.
*/
RegexGenerator::RegexGenerator()
: error_policy_( nullptr )
, syntax_tree_( nullptr )
, actions_()
, states_()
, start_state_( nullptr )
, ranges_()
{
    syntax_tree_ = new RegexSyntaxTree;
}

RegexGenerator::~RegexGenerator()
{
    delete syntax_tree_;
}

/**
// Fire an error from this generator.
//
// @param line
//  The line number that the error occured on.
//
// @param column
//  The column number that the error occured on.
//
// @param error
//  The error code that indicates the error that occured.
// 
// @param format
//  A printf-style format string describing the error that occured (assumed 
//  not null).
//
// @param ...
//  Parameters as described by *format*.
*/
void RegexGenerator::fire_error( int line, int column, int error, const char* format, ... ) const
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
// Fire a message to be printed from this generator.
//
// @param format
//  A printf-style format string that desribes the message to print.
//
// @param ...
//  Parameters as described by \e format.
*/
void RegexGenerator::fire_printf( const char* format, ... ) const
{
    if ( error_policy_ )
    {
        LALR_ASSERT( format );
        va_list args;
        va_start( args, format );
        error_policy_->lalr_vprintf( format, args );
        va_end( args );
    }
}


/**
// Add a new or retrieve an existing RegexAction.
//
// If the parser already has a RegexAction whose identifier 
// matches \e identifier then that RegexAction is returned.  Otherwise 
// a new RegexAction is created, added to this `Generator` so that it 
// can be returned later if necessary, and returned from this call.
//
// @param identifier
//  The identifier of the RegexAction to add or retrieve.
//
// @return
//  The RegexAction whose identifier matches \e identifier or null if 
//  \e identifier is empty.
*/
const RegexAction* RegexGenerator::add_lexer_action( const std::string& identifier )
{
    LALR_ASSERT( !identifier.empty() );
    if ( !identifier.empty() )
    {    
        vector<std::unique_ptr<RegexAction> >::const_iterator i = actions_.begin();
        while ( i != actions_.end() && (*i)->identifier() != identifier )
        {
            ++i;
        }
        if ( i == actions_.end() )
        {
            unique_ptr<RegexAction> action( new RegexAction(int(actions_.size()), identifier) );
            actions_.push_back( std::move(action) );
            i = actions_.end() - 1;
        }
        return i->get();
    }
    return nullptr;
}

int RegexGenerator::generate( const std::string& regular_expression, void* symbol, ErrorPolicy* error_policy )
{
    error_policy_ = error_policy;
    actions_.clear();
    states_.clear();
    start_state_ = nullptr;
    ranges_.clear();

    RegexToken token( TOKEN_REGULAR_EXPRESSION, 0, 0, symbol, regular_expression.c_str() );
    syntax_tree_->reset( token, this );
    generate_states( *syntax_tree_ );
    error_policy_ = nullptr;
    return syntax_tree_->errors();
}

int RegexGenerator::generate( const std::vector<RegexToken>& tokens, ErrorPolicy* error_policy )
{
    error_policy_ = error_policy;
    actions_.clear();
    states_.clear();
    start_state_ = nullptr;
    ranges_.clear();
 
    syntax_tree_->reset( tokens, this );
    generate_states( *syntax_tree_ );
    error_policy_ = nullptr;
    return syntax_tree_->errors();
}

/**
// Generate the state that results from accepting any character in the range
// [\e begin, \e end) from \e state.
//
// @param state
//  The state to generate from.
//
// @param begin
//  The begin character in the range to accept to generate the goto state.
//
// @param end
//  The end character in the range to accept to generate the goto state.
//
// @return
//  The state generated when accepting [\e begin, \e end) from \e state.
*/
std::unique_ptr<RegexState> RegexGenerator::goto_( const RegexState* state, int begin, int end )
{
    LALR_ASSERT( state );
    LALR_ASSERT( begin != INVALID_BEGIN_CHARACTER && begin != INVALID_END_CHARACTER );
    LALR_ASSERT( begin <= end );

    std::unique_ptr<RegexState> goto_state( new RegexState );
    const std::set<RegexItem>& items = state->get_items();
    for ( std::set<RegexItem>::const_iterator item = items.begin(); item != items.end(); ++item )
    {
        std::set<RegexNode*, RegexNodeLess> next_nodes = item->next_nodes( begin, end );
        if ( !next_nodes.empty() )
        {
            goto_state->add_item( next_nodes );
        }
    }
    return goto_state;
}

/**
// Generate the states for a LexerStateMachine from \e syntax_tree.
//
// @param syntax_tree
//  The RegexSyntaxTree to get the RegexNodes from that are then used to generate 
//  states.
//
// @param states
//  The set of states to populate from the output of the RegexSyntaxTree (assumed
//  not null).
//
// @param start_state
//  A variable to receive the starting state for the lexical analyzer
//  (assumed not null).
*/
void RegexGenerator::generate_states( const RegexSyntaxTree& syntax_tree )
{
    LALR_ASSERT( states_.empty() );
    LALR_ASSERT( !start_state_ );

    if ( !syntax_tree.empty() && syntax_tree.errors() == 0 )
    {
        std::unique_ptr<RegexState> state( new RegexState() );
        state->add_item( syntax_tree.node()->get_first_positions() );
        generate_symbol_for_state( state.get() );
        start_state_ = state.get();
        states_.insert( std::move(state) );

        vector<RegexState*> next_states;
        vector<RegexState*> states;
        states.push_back( start_state_ );

        while ( !states.empty() )
        {
            for ( RegexState* state : states )
            {
                // Create the distinct ranges of characters that can be 
                // transitioned on from the current state.
                clear();
                const std::set<RegexItem>& items = state->get_items();
                for ( std::set<RegexItem>::const_iterator item = items.begin(); item != items.end(); ++item )
                {
                    const std::set<RegexNode*, RegexNodeLess>& next_nodes = item->next_nodes();
                    for ( std::set<RegexNode*, RegexNodeLess>::const_iterator j = next_nodes.begin(); j != next_nodes.end(); ++j )
                    {
                        const RegexNode* next_node = *j;
                        LALR_ASSERT( next_node );
                        if ( !next_node->is_end() )
                        {
                            insert( next_node->get_begin_character(), next_node->get_end_character() );
                        }
                    }
                }
                
                // Create a goto state and a transition from the current 
                // state for each distinct range.
                vector<pair<int, bool>>::const_iterator j = ranges_.begin();
                while ( j != ranges_.end() )
                {               
                    int begin = (j + 0)->first;
                    int end = (j + 1)->first;
                    LALR_ASSERT( begin < end );
                    
                    std::unique_ptr<RegexState> goto_state = goto_( state, begin, end );
                    if ( !goto_state->get_items().empty() )
                    {
                        auto existing_goto_state = states_.find( goto_state );
                        if ( existing_goto_state == states_.end() )
                        {
                            state->add_transition( begin, end, goto_state.get() );
                            generate_symbol_for_state( goto_state.get() );
                            next_states.push_back(goto_state.get() );
                            states_.insert( std::move(goto_state) );
                        }
                        else
                        {
                            state->add_transition( begin, end, existing_goto_state->get() );
                        }
                    }                    

                    ++j;
                    if ( !j->second )
                    {
                        ++j;
                        LALR_ASSERT( j == ranges_.end() || j->second );
                    }
                }
            }

            states.clear();
            states.swap( next_states );
        }
    }

    generate_indices_for_states();
}

/**
// Generate indices for the generated states.
*/
void RegexGenerator::generate_indices_for_states()
{
    int index = 0;    
    for ( auto i = states_.begin(); i != states_.end(); ++i )
    {
        RegexState* state = i->get();
        LALR_ASSERT( state );
        state->set_index( index );
        ++index;
    }
}

/**
// Generate the matching symbol for \e state if it has one.
//
// @param state
//  The state to generate a matching symbol for.
*/
void RegexGenerator::generate_symbol_for_state( RegexState* state ) const
{
    LALR_ASSERT( state );

    int line = INT_MAX;
    RegexTokenType type = TOKEN_NULL;
    const RegexToken* token = NULL;

    const std::set<RegexItem>& items = state->get_items();
    for ( std::set<RegexItem>::const_iterator item = items.begin(); item != items.end(); ++item )
    {
        std::set<RegexNode*, RegexNodeLess>::const_iterator i = item->next_nodes().begin();
        while ( i != item->next_nodes().end() )
        {
            const RegexNode* node = *i;
            LALR_ASSERT( node );

            if ( node->is_end() && node->get_token() )
            {
                if ( node->get_token()->type() > type )
                {
                    line = node->get_token()->line();
                    type = node->get_token()->type();
                    token = node->get_token();
                }
                else if ( node->get_token()->type() == type && node->get_token()->line() < line )
                {
                    line = node->get_token()->line();
                    type = node->get_token()->type();
                    token = node->get_token();
                }
                else if ( node->get_token()->type() == type && node->get_token()->line() == line )
                {
                    LALR_ASSERT( type != TOKEN_NULL );
                    LALR_ASSERT( line != INT_MAX );
                    LALR_ASSERT( token );
                    if ( !token->conflicted_with(node->get_token()) )
                    {
                        fire_error( token->line(), token->column(), LEXER_ERROR_SYMBOL_CONFLICT, "'%s' and '%s' conflict but are both defined on line %d", token->lexeme().c_str(), node->get_token()->lexeme().c_str(), token->line() );                        
                        token->add_conflicted_with( node->get_token() );
                    }
                }
            }
            
            ++i;
        }
    }    

    state->set_symbol( token ? token->symbol() : NULL );
}

/**
// Clear the current distinct ranges maintained by this RegexGenerator.
*/
void RegexGenerator::clear()
{
    ranges_.clear();
}

/**
// Insert the range [\e begin, \e end) into the current distinct ranges for 
// this RegexGenerator.
//
// The ranges are stored as a vector of pair<int, bool>.  The first element of
// the pair represents the character and the second element represents whether
// or not that character is considered in or out.
//
// This is done so that transitions can be efficiently calculated for 
// independent ranges of characters.  For example if a state has three next 
// nodes that represent characters in the the ranges [0, 256), [0, 32), and 
// [0, 64) then three goto states should be generated with transitions on 
// [0, 32), [32, 64), and [64, 256) respectively.
//
// @param begin
//  The begin character in the range of characters to insert.
//
// @param end
//  The end character in the range of characters to insert.
*/
void RegexGenerator::insert( int begin, int end )
{
    bool in = false;        

    vector<pair<int, bool> >::iterator i = ranges_.begin();
    while ( i != ranges_.end() && i->first < begin )
    {
        in = i->second;
        ++i;
    }        

    if ( i == ranges_.end() || i->first != begin )
    {
        i = ranges_.insert( i, make_pair(begin, true) );
        ++i;
    }
                   
    while ( i != ranges_.end() && i->first < end )
    {
        in = i->second;
        i->second = true;
        ++i;
    }        
    
    if ( i == ranges_.end() || i->first != end )
    {
        ranges_.insert( i, make_pair(end, in) );
    }
}
