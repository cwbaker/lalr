//
// GrammarGenerator.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "GrammarGenerator.hpp"
#include "GrammarProduction.hpp"
#include "GrammarState.hpp"
#include "GrammarItem.hpp"
#include "Grammar.hpp"
#include "GrammarSymbol.hpp"   
#include "GrammarAction.hpp"
#include "GrammarCompiler.hpp"
#include "ParserState.hpp"
#include "ParserAction.hpp"
#include "ParserSymbol.hpp"
#include "ParserTransition.hpp"
#include "ParserStateMachine.hpp"
#include "LexerGenerator.hpp"
#include "LexerAllocations.hpp"
#include "ErrorPolicy.hpp"
#include "ErrorCode.hpp"
#include "assert.hpp"

using std::set;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using namespace sweet;
using namespace sweet::lalr;

/**
// Constructor.
//
// @param grammar
//  The Grammar to generate a parser for.
//
// @param error_policy
//  The error policy to report errors during generation to or null to silently
//  swallow errors.
*/
GrammarGenerator::GrammarGenerator()
: error_policy_( nullptr ),
  identifier_(),
  actions_(),
  productions_(),
  symbols_(),
  states_(),
  start_symbol_( nullptr ),
  end_symbol_( nullptr ),
  error_symbol_( nullptr ),
  start_state_( nullptr ),
  errors_( 0 )
{
}

GrammarGenerator::~GrammarGenerator()
{
}

const std::vector<std::unique_ptr<GrammarAction>>& GrammarGenerator::actions() const
{
    return actions_;
}

const std::vector<std::unique_ptr<GrammarSymbol>>& GrammarGenerator::symbols() const
{
    return symbols_;
}

const std::set<std::shared_ptr<GrammarState>, shared_ptr_less<GrammarState>>& GrammarGenerator::states() const
{
    return states_;
}

const GrammarState* GrammarGenerator::start_state() const
{
    return start_state_;
}

int GrammarGenerator::generate( Grammar& grammar, GrammarCompiler* parser_allocations, ErrorPolicy* error_policy )
{
    SWEET_ASSERT( parser_allocations );

    error_policy_ = error_policy;
    identifier_ = grammar.identifier();
    actions_.swap( grammar.actions() );
    productions_.swap( grammar.productions() );
    symbols_.swap( grammar.symbols() );
    states_.clear();
    start_symbol_ = grammar.start_symbol();
    end_symbol_ = grammar.end_symbol();
    error_symbol_ = grammar.error_symbol();
    start_state_ = nullptr;
    errors_ = 0;

    calculate_identifiers();
    check_for_undefined_symbol_errors();
    check_for_unreferenced_symbol_errors();
    check_for_error_symbol_on_left_hand_side_errors();    

    if ( errors_ == 0 )
    {        
        calculate_terminal_and_non_terminal_symbols();
        calculate_implicit_terminal_symbols();
        calculate_first();
        calculate_follow();
        calculate_symbol_indices();
        calculate_precedence_of_productions();
        generate_states( start_symbol_, end_symbol_, symbols_ );
    }

    int errors = errors_;
    errors_ = 0;
    return errors;
}

/**
// Record and fire and error at the event sink.
//
// @param line
//  The line that the error occured on.
//
// @param error
//  The error code.
//
// @param format
//  A printf-style format string describing the error
//
// @param ...
//  Arguments described by *format*.
*/
void GrammarGenerator::fire_error( int line, int error, const char* format, ... )
{
    ++errors_;    
    if ( error_policy_ )
    {
        va_list args;
        va_start( args, format );
        error_policy_->lalr_error( line, error, format, args );
        va_end( args );
    }
}

/**
// Fire debug output at the event sink.
//
// @param format
//  The printf-style format string that describes the text to print.
//
// @param args
//  Arguments as described by \e format.
*/
void GrammarGenerator::fire_printf( const char* format, ... ) const
{
    if ( error_policy_ )
    {
        SWEET_ASSERT( format );
        va_list args;
        va_start( args, format );
        error_policy_->lalr_vprintf( format, args );
        va_end( args );
    }
}

/**
// Generate lookaheads for an item.
//
// @param item
//  The item to generate lookaheads from.
//
// @return
//  The generated lookahead symbols.
*/
std::set<const GrammarSymbol*> GrammarGenerator::lookahead( const GrammarItem& item ) const
{
    set<const GrammarSymbol*> lookahead_symbols;

    const GrammarProduction* production = item.production();
    SWEET_ASSERT( production );
        
    const vector<GrammarSymbol*>& symbols = production->symbols();    
    vector<GrammarSymbol*>::const_iterator i = symbols.begin() + item.position();
    if ( i != symbols.end() )
    {
        ++i;
    }
    
    while ( i != symbols.end() && (*i)->nullable() )
    {
        const GrammarSymbol* symbol = *i;
        SWEET_ASSERT( symbol );
        lookahead_symbols.insert( symbol->first().begin(), symbol->first().end() );
        ++i;
    }
    
    if ( i != symbols.end() )
    {
        const GrammarSymbol* symbol = *i;
        SWEET_ASSERT( symbol );
        lookahead_symbols.insert( symbol->first().begin(), symbol->first().end() );
    }
    else
    {
        lookahead_symbols.insert( item.lookahead_symbols().begin(), item.lookahead_symbols().end() );
    }

    return lookahead_symbols;    
}

/**
// Generate the closure of the items contained in \e state.
//
// @param state
//  The GrammarState that contains the items to generate the closure of.
*/
void GrammarGenerator::closure( const std::shared_ptr<GrammarState>& state )
{
    SWEET_ASSERT( state );

    int added = 1;
    while ( added > 0 )
    {
        added = 0;
        const set<GrammarItem>& items = state->items();
        for ( set<GrammarItem>::const_iterator item = items.begin(); item != items.end(); ++item )
        {          
            const GrammarSymbol* symbol = item->production()->symbol_by_position( item->position() );
            if ( symbol )
            {
                const vector<GrammarProduction*>& productions = symbol->productions();
                for ( vector<GrammarProduction*>::const_iterator j = productions.begin(); j != productions.end(); ++j )
                {
                    GrammarProduction* production = *j;
                    SWEET_ASSERT( production );
                    added += state->add_item( production, 0 );
                }
            }
        }
    }
}

/**
// Generate the state that results from accepting \e symbol from \e state.
//
// @param state
//  The state to generate from.
//
// @param symbol
//  The symbol to accept to generate the goto state.
//
// @return
//  The goto state generated when accepting \e symbol from \e state.
*/
std::shared_ptr<GrammarState> GrammarGenerator::goto_( const std::shared_ptr<GrammarState>& state, const GrammarSymbol& symbol )
{
    SWEET_ASSERT( state );

    std::shared_ptr<GrammarState> goto_state( new GrammarState() );

    const set<GrammarItem>& items = state->items();
    for ( set<GrammarItem>::const_iterator item = items.begin(); item != items.end(); ++item )
    {
        if ( item->next_node(symbol) )
        {
            goto_state->add_item( item->production(), item->position() + 1 );
        }
    }

    closure( goto_state );
    return goto_state;
}

/**
// Generate lookaheads from the closure of the items contained in \e state.
//
// Adds the lookahead symbols from each item to the items added during the
// closure of \e state.
//
// @param state
//  The state to generate lookaheads from the closure of.
//
// @return
//  The number of lookahead symbols generated.
*/
int GrammarGenerator::lookahead_closure( GrammarState* state ) const
{
    SWEET_ASSERT( state );

    int added = 0;

    const set<GrammarItem>& items = state->items();
    for ( set<GrammarItem>::const_iterator item = items.begin(); item != items.end(); ++item )
    {          
        const GrammarSymbol* symbol = item->production()->symbol_by_position( item->position() );
        if ( symbol )
        {
            set<const GrammarSymbol*> lookahead_symbols = lookahead( *item );        
            const vector<GrammarProduction*>& productions = symbol->productions();
            for ( vector<GrammarProduction*>::const_iterator j = productions.begin(); j != productions.end(); ++j )
            {
                GrammarProduction* production = *j;
                SWEET_ASSERT( production );
                added += state->add_lookahead_symbols( production, 0, lookahead_symbols );
            }
        }
    }

    return added;
}

/**
// Propagate lookaheads from \e state to the states that \e state has 
// transitions to.
//
// This copies the lookaheads from each item in \e state to the equivalent 
// item in the state that is transitioned to.  The equivalent item is the 
// item for the same production with its position advanced by the symbol 
// that was accepted on the transition.
//
// @param state
//  The state to propagate lookahead symbols from.
//
// @return
//  The number of lookahead symbols propagated.
*/
int GrammarGenerator::lookahead_goto( GrammarState* state ) const
{
    SWEET_ASSERT( state );

    int added = 0;

    const set<GrammarTransition>& transitions = state->transitions();
    for ( set<GrammarTransition>::const_iterator transition = transitions.begin(); transition != transitions.end(); ++transition )
    {
        const GrammarSymbol* symbol = transition->symbol();
        SWEET_ASSERT( symbol );

        const set<GrammarItem>& items = state->items();
        for ( set<GrammarItem>::const_iterator item = items.begin(); item != items.end(); ++item )
        {
            int position = item->position();
            if ( item->production()->symbol_by_position(position) == symbol )
            {
                GrammarState* goto_state = transition->state();
                added += goto_state->add_lookahead_symbols( item->production(), position + 1, item->lookahead_symbols() );
            }
        }        
    }

    return added;
}

/**
// Replace references to \e to_symbol with references to \e with_symbol.
//
// @param to_symbol
//  The GrammarSymbol to replace references to.
//
// @param with_symbol
//  The GrammarSymbol to replace references with.
*/
void GrammarGenerator::replace_references_to_symbol( GrammarSymbol* to_symbol, GrammarSymbol* with_symbol )
{
    for ( vector<unique_ptr<GrammarProduction>>::const_iterator i = productions_.begin(); i != productions_.end(); ++i )
    {
        GrammarProduction* production = i->get();
        SWEET_ASSERT( production );
        production->replace_references_to_symbol( to_symbol, with_symbol );
    }
}

/**
// Check for symbols in the grammar that are referenced but never defined.
*/
void GrammarGenerator::check_for_undefined_symbol_errors()
{
    if ( errors_ == 0 )
    {
        for ( vector<unique_ptr<GrammarSymbol>>::const_iterator i = symbols_.begin(); i != symbols_.end(); ++i )
        {
            const GrammarSymbol* symbol = i->get();
            SWEET_ASSERT( symbol );
            if ( symbol->symbol_type() == SYMBOL_NON_TERMINAL && symbol->productions().empty() )
            {
                fire_error( 1, PARSER_ERROR_UNDEFINED_SYMBOL, "Undefined symbol '%s' in grammar '%s'", symbol->identifier().c_str(), identifier_.c_str() );
            }
        }
    }
}

/**
// Check for symbols in the grammar that are defined but never referenced.
//
// @param generator
//  The GrammarGenerator for fire any errors from (assumed not null).
*/
void GrammarGenerator::check_for_unreferenced_symbol_errors()
{
    if ( errors_ == 0 )
    {
        for ( vector<unique_ptr<GrammarSymbol>>::const_iterator i = symbols_.begin(); i != symbols_.end(); ++i )
        {
            const GrammarSymbol* symbol = i->get();
            SWEET_ASSERT( symbol );
            
            int references = 0;            
            if ( symbol != start_symbol_ && symbol != end_symbol_ && symbol != error_symbol_ )
            {
                for ( vector<unique_ptr<GrammarProduction>>::const_iterator i = productions_.begin(); i != productions_.end(); ++i )
                {
                    const GrammarProduction* production = i->get();
                    SWEET_ASSERT( production );
                    if ( production->symbol()->symbol_type() != SYMBOL_TERMINAL )
                    {
                        references += production->count_references_to_symbol( symbol );
                    }
                }

                if ( references == 0 )
                {
                    fire_error( 1, PARSER_ERROR_UNREFERENCED_SYMBOL, "Unreferenced symbol '%s'/'%s'", symbol->identifier().c_str(), symbol->lexeme().c_str() );
                }
            }
        }
    }
}

/**
// Check for the error symbol being used in the left hand side of a 
// production.
//
// @param generator
//  The GrammarGenerator for fire any errors from (assumed not null).
*/
void GrammarGenerator::check_for_error_symbol_on_left_hand_side_errors()
{
    SWEET_ASSERT( error_symbol_ );

    const vector<GrammarProduction*>& productions = error_symbol_->productions();
    if ( !productions.empty() )
    {
        fire_error( 1, PARSER_ERROR_ERROR_SYMBOL_ON_LEFT_HAND_SIDE, "The 'error' symbol appears on the left hand side of a production" );
    }
}

/**
// Calculate identifiers for all symbols.
*/
void GrammarGenerator::calculate_identifiers()
{
    for ( vector<unique_ptr<GrammarSymbol>>::const_iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        GrammarSymbol* symbol = i->get();
        SWEET_ASSERT( symbol );
        symbol->calculate_identifier();
    }
}

/**
// Calculate which symbols are terminal and non-terminal.
//
// Any symbols with one or more productions are assumed to be non-terminals
// and any symbols with no productions are assumed to be terminals.  Another
// pass is made over the symbols in to convert non-terminals symbols that 
// contain only a single production with one terminal symbol into terminals.
// See `Grammar::calculate_implicit_terminal_symbols()`.
//
// The `.start`, `.end`, and `.error` symbols are exempt from the above 
// processing.  They are explicitly assigned their corr
*/
void GrammarGenerator::calculate_terminal_and_non_terminal_symbols()
{
    for ( vector<unique_ptr<GrammarSymbol>>::const_iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        GrammarSymbol* symbol = i->get();
        if ( symbol->symbol_type() == SYMBOL_NULL )
        {
            symbol->set_symbol_type( symbol->productions().empty() ? SYMBOL_TERMINAL : SYMBOL_NON_TERMINAL );
        }
    }
}

/**
// Calculate the non terminal symbols that are really just named terminals.
//
// Any symbols that contain a single production that contains only a terminal 
// symbol are really just acting as names for that terminal symbol.  To make 
// the parser easier to understand and more efficient these symbols are 
// collapsed by making any references to the non terminal symbol refer directly
// to the terminal symbol.  The identifier of the terminal is changed to be 
// the more readable name of the non terminal.
//
// For example the rule 'integer: "[0-9]+";' creates a non terminal
// symbol 'integer' and a terminal symbol '"[0-9]+"'.  The non terminal
// symbol 'integer' is redundant from the point of view of the parser as it
// adds only a trivial reduction from one symbol type to another.  To optimize
// this situation the terminal is collapsed into the non terminal keeping the
// more readable name of the non terminal but removing the redundant 
// reduction.
*/
void GrammarGenerator::calculate_implicit_terminal_symbols()
{
    for ( vector<unique_ptr<GrammarSymbol>>::iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        GrammarSymbol* non_terminal_symbol = i->get();        
        if ( non_terminal_symbol && non_terminal_symbol != error_symbol_ )
        {
            GrammarSymbol* terminal_symbol = non_terminal_symbol->implicit_terminal();
            if ( terminal_symbol )
            {       
                SWEET_ASSERT( terminal_symbol != non_terminal_symbol );
                terminal_symbol->replace_by_non_terminal( non_terminal_symbol );
                replace_references_to_symbol( non_terminal_symbol, terminal_symbol );
                i->reset();
            }
        }
    }
    
    vector<unique_ptr<GrammarSymbol>>::iterator i = symbols_.begin();
    while ( i != symbols_.end() )
    {
        if ( !i->get() )
        {
            i = symbols_.erase( i );
        }
        else        
        {
            ++i;
        }
    }
}

/**
// Calculate the precedence of each production that hasn't had precedence
// set explicitly as the precedence of its rightmost terminal.
*/
void GrammarGenerator::calculate_precedence_of_productions()
{
    for ( vector<unique_ptr<GrammarProduction>>::const_iterator i = productions_.begin(); i != productions_.end(); ++i )
    {
        GrammarProduction* production = i->get();
        SWEET_ASSERT( production );       
        if ( production->precedence() == 0 )
        {
            const GrammarSymbol* symbol = production->find_rightmost_terminal_symbol();
            if ( symbol )
            {
                production->set_precedence_symbol( symbol );
            }
        }
    }
}

/**
// Calculate the index for each symbol.
*/
void GrammarGenerator::calculate_symbol_indices()
{
    int index = 0;
    for ( vector<unique_ptr<GrammarSymbol>>::iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        GrammarSymbol* symbol = i->get();
        SWEET_ASSERT( symbol );
        symbol->set_index( index );
        ++index;
    }
}

/**
// Calculate the first position sets for each GrammarSymbol until no more 
// terminals can be added to any first position sets.
*/
void GrammarGenerator::calculate_first()
{
    int added = 1;
    while ( added > 0 )
    {
        added = 0;
        for ( vector<unique_ptr<GrammarSymbol>>::iterator i = symbols_.begin(); i != symbols_.end(); ++i )
        {
            GrammarSymbol* symbol = i->get();
            SWEET_ASSERT( symbol );
            added += symbol->calculate_first();
        }
    }
}

/**
// Calculate the follow position sets for each GrammarSymbol until no more 
// terminals can be added to any follow position sets.
*/
void GrammarGenerator::calculate_follow()
{
    start_symbol_->add_symbol_to_follow( end_symbol_ );

    int added = 1;
    while ( added > 0 )
    {
        added = 0;
        for ( vector<unique_ptr<GrammarSymbol>>::iterator i = symbols_.begin(); i != symbols_.end(); ++i )
        {
            GrammarSymbol* symbol = i->get();
            SWEET_ASSERT( symbol );
            added += symbol->calculate_follow();
        }
    }
}

/**
// Generate the states for a grammar with \e symbols starting with 
// \e start_symbol and ending when \e end_symbol is accepted.
//
// @param start_symbol
//  The start symbol for the grammar.
//
// @param end_symbol
//  The end symbol for the grammar.
//
// @param symbols
//  The symbols in the grammar.
*/
void GrammarGenerator::generate_states( const GrammarSymbol* start_symbol, const GrammarSymbol* end_symbol, const std::vector<std::unique_ptr<GrammarSymbol>>& symbols )
{
    SWEET_ASSERT( start_symbol );
    SWEET_ASSERT( end_symbol );
    SWEET_ASSERT( states_.empty() );

    if ( !start_symbol->productions().empty() )
    {
        std::shared_ptr<GrammarState> start_state( new GrammarState() );
        start_state->add_item( start_symbol->productions().front(), 0 );
        closure( start_state );
        states_.insert( start_state );
        start_state_ = start_state.get();

        set<const GrammarSymbol*> lookahead_symbols;
        lookahead_symbols.insert( (GrammarSymbol*) end_symbol );
        start_state->add_lookahead_symbols( start_symbol->productions().front(), 0, lookahead_symbols );
        
        int added = 1;
        while ( added > 0 )
        {
            added = 0;
            for ( std::set<std::shared_ptr<GrammarState>, shared_ptr_less<GrammarState>>::const_iterator i = states_.begin(); i != states_.end(); ++i )
            {
                const std::shared_ptr<GrammarState>& state = *i;
                SWEET_ASSERT( state );

                if ( !state->processed() )
                {
                    state->set_processed( true );
                    for ( vector<unique_ptr<GrammarSymbol>>::const_iterator j = symbols.begin(); j != symbols.end(); ++j )
                    {
                        GrammarSymbol* symbol = j->get();
                        SWEET_ASSERT( symbol );
                        if ( symbol != end_symbol )
                        {
                            std::shared_ptr<GrammarState> goto_state = goto_( state, *symbol );
                            if ( !goto_state->items().empty() )
                            {                    
                                std::shared_ptr<GrammarState> actual_goto_state = *states_.insert( goto_state ).first;
                                added += goto_state == actual_goto_state ? 1 : 0;
                                state->add_transition( symbol, actual_goto_state.get() );
                            }
                        }
                    }
                }
            }
        }
        
        generate_indices_for_states();

        added = 1;
        while ( added > 0 )
        {
            added = 0;
            for ( std::set<std::shared_ptr<GrammarState>, shared_ptr_less<GrammarState>>::const_iterator i = states_.begin(); i != states_.end(); ++i )
            {
                GrammarState* state = i->get();
                SWEET_ASSERT( state );
                added += lookahead_closure( state );
                added += lookahead_goto( state );
            }
        }
        
        generate_reduce_transitions();
        generate_indices_for_transitions();
    }
}

/**
// Generate indices for states.
*/
void GrammarGenerator::generate_indices_for_states()
{
    int index = 0;
    for ( std::set<std::shared_ptr<GrammarState>, shared_ptr_less<GrammarState>>::iterator i = states_.begin(); i != states_.end(); ++i )
    {
        GrammarState* state = i->get();
        SWEET_ASSERT( state );
        state->set_index( index );
        ++index;
    }
}

/**
// Generate reduction transitions.
*/
void GrammarGenerator::generate_reduce_transitions()
{
    for ( std::set<std::shared_ptr<GrammarState>, shared_ptr_less<GrammarState>>::const_iterator i = states_.begin(); i != states_.end(); ++i )
    {
        GrammarState* state = i->get();
        SWEET_ASSERT( state );
            
        for ( std::set<GrammarItem>::const_iterator item = state->items().begin(); item != state->items().end(); ++item )
        {
            if ( item->dot_at_end() )
            {
                const set<const GrammarSymbol*>& symbols = item->lookahead_symbols();
                for ( set<const GrammarSymbol*>::const_iterator j = symbols.begin(); j != symbols.end(); ++j )
                {
                    const GrammarSymbol* symbol = *j;
                    SWEET_ASSERT( symbol );
                    generate_reduce_transition( state, symbol, item->production() );
                }
            }                
        }
    }
}

/**
// Generate a reduction transition.
//
// @param state
//  The GrammarState that the reduction occurs from.
//
// @param symbol
//  The GrammarSymbol that the reduction is to be performed on.
//
// @param production
//  The GrammarProduction that is to be reduced.
*/
void GrammarGenerator::generate_reduce_transition( GrammarState* state, const GrammarSymbol* symbol, const GrammarProduction* production )
{
    SWEET_ASSERT( state );
    SWEET_ASSERT( symbol );
    SWEET_ASSERT( production );

    GrammarTransition* transition = state->find_transition_by_symbol( symbol );
    if ( !transition )
    {
        state->add_transition( symbol, production->symbol(), production->length(), production->precedence(), production->action_index() );
    }
    else
    {
        switch ( transition->type() )
        {
            case TRANSITION_SHIFT:
            {
                if ( production->precedence() == 0 || symbol->precedence() == 0 || (symbol->precedence() == production->precedence() && symbol->associativity() == ASSOCIATE_NULL) )
                {
                    fire_error( production->line(), PARSER_ERROR_PARSE_TABLE_CONFLICT, "Shift/reduce conflict on '%s' for '%s'", symbol->identifier().c_str(), production->symbol()->identifier().c_str() );
                }
                else if ( production->precedence() > symbol->precedence() || (symbol->precedence() == production->precedence() && symbol->associativity() == ASSOCIATE_RIGHT) )
                {
                    transition->override_shift_to_reduce( production->symbol(), production->length(), production->precedence(), production->action_index() );
                }

                break;
            }
            
            case TRANSITION_REDUCE:
            {
                if ( production->precedence() == 0 || transition->precedence() == 0 || production->precedence() == transition->precedence() )
                {
                    fire_error( production->line(), PARSER_ERROR_PARSE_TABLE_CONFLICT, "Reduce/reduce conflict on '%s' for '%s' and '%s'", symbol->identifier().c_str(), production->symbol()->identifier().c_str(), transition->reduced_symbol()->identifier().c_str() );
                }
                else if ( production->precedence() > transition->precedence() )
                {
                    transition->override_reduce_to_reduce( production->symbol(), production->length(), production->precedence(), production->action_index() );
                }
                break;
            }
                
            default:
                SWEET_ASSERT( false );
                break;
        }
    }
}

/**
// Generate indices for the transitions in each state.
*/
void GrammarGenerator::generate_indices_for_transitions()
{
    for ( std::set<std::shared_ptr<GrammarState>, shared_ptr_less<GrammarState>>::const_iterator i = states_.begin(); i != states_.end(); ++i )
    {
        GrammarState* state = i->get();
        SWEET_ASSERT( state );
        state->generate_indices_for_transitions();        
    }
}
