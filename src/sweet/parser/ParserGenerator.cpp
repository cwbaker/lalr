//
// ParserGenerator.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "ParserGenerator.hpp"
#include "ParserProduction.hpp"
#include "ParserState.hpp"
#include "ParserItem.hpp"
#include "ParserErrorPolicy.hpp"
#include "ParserGrammar.hpp"
#include "ErrorCode.hpp"
#include <sweet/assert/assert.hpp>

using std::set;
using std::vector;
using std::shared_ptr;
using namespace sweet;
using namespace sweet::parser;

/**
// Constructor.
//
// @param grammar
//  The ParserGrammar to generate a parser for.
//
// @param error_policy
//  The error policy to report errors during generation to or null to silently
//  swallow errors.
*/
ParserGenerator::ParserGenerator( ParserGrammar& grammar, ParserErrorPolicy* error_policy )
: error_policy_( error_policy ),
  identifier_(),
  actions_(),
  productions_(),
  symbols_(),
  states_(),
  start_symbol_( NULL ),
  end_symbol_( NULL ),
  error_symbol_( NULL ),
  start_state_(),
  errors_( 0 )
{
    generate( grammar );
}

/**
// Get the identifier of the grammar.
//
// @return
//  The identifier.
*/
std::string& ParserGenerator::identifier()
{
    return identifier_;
}

/**
// Get the actions.
//
// @return
//  The actions.
*/
std::vector<std::shared_ptr<ParserAction> >& ParserGenerator::actions()
{
    return actions_;
}

/**
// Get the productions.
//
// @return
//  The productions.
*/
std::vector<std::shared_ptr<ParserProduction> >& ParserGenerator::productions()
{
    return productions_;
}

/**
// Get the symbols.
//
// @return
//  The symbols.
*/
std::vector<std::shared_ptr<ParserSymbol> >& ParserGenerator::symbols()
{
    return symbols_;
}

/**
// Get the states.
//
// @return
//  The states.
*/
std::set<std::shared_ptr<ParserState>, shared_ptr_less<ParserState>>& ParserGenerator::states()
{
    return states_;
}

/**
// Get the start symbol.
//
// @return
//  The start symbol.
*/
const ParserSymbol* ParserGenerator::start_symbol()
{
    return start_symbol_;
}

/**
// Get the end symbol.
//
// @return
//  The end symbol.
*/
const ParserSymbol* ParserGenerator::end_symbol()
{
    return end_symbol_;
}

/**
// Get the error symbol.
//
// @return
//  The error symbol.
*/
const ParserSymbol* ParserGenerator::error_symbol()
{
    return error_symbol_;
}

/**
// Get the start state.
//
// @return
//  The start state.
*/
ParserState* ParserGenerator::start_state()
{
    return start_state_;
}

/**
// Get the number of errors that occured during parsing and generation.
//
// Other ParserGenerator functions are invalid if the number of errors
// returned by this function is not zero after the ParserGenerator has
// been constructed.
//
// @return
//  The number of errors.
*/
int ParserGenerator::errors() const
{
    return errors_;
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
void ParserGenerator::fire_error( int line, int error, const char* format, ... )
{
    ++errors_;    
    if ( error_policy_ )
    {
        va_list args;
        va_start( args, format );
        error_policy_->parser_error( line, error, format, args );
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
void ParserGenerator::fire_printf( const char* format, ... ) const
{
    if ( error_policy_ )
    {
        SWEET_ASSERT( format );
        va_list args;
        va_start( args, format );
        error_policy_->parser_vprintf( format, args );
        va_end( args );
    }
}

/**
// Generate the parser state machine from the parsed grammar in 
// \e grammar_parser.
//
// @param grammar_parser
//  The GrammarParser that has successfully parsed a grammar.
*/
void ParserGenerator::generate( ParserGrammar& grammar )
{
    grammar.calculate_identifiers();
    grammar.check_for_undefined_symbol_errors( this );
    grammar.check_for_unreferenced_symbol_errors( this );
    grammar.check_for_error_symbol_on_left_hand_side_errors( this );    

    if ( errors_ == 0 )
    {
        grammar.calculate_terminal_and_non_terminal_symbols();
        grammar.calculate_implicit_terminal_symbols();
        grammar.calculate_first();
        grammar.calculate_follow();
        grammar.calculate_indices();
        grammar.calculate_precedence_of_productions();

        identifier_ = grammar.identifier();
        actions_.swap( grammar.actions() );
        productions_.swap( grammar.productions() );
        symbols_.swap( grammar.symbols() );
        start_symbol_ = grammar.start_symbol();
        end_symbol_ = grammar.end_symbol();
        error_symbol_ = grammar.error_symbol();
        states_.clear();
        start_state_ = NULL;
        
        generate_states( start_symbol_, end_symbol_, symbols_ );
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
std::set<const ParserSymbol*> ParserGenerator::lookahead( const ParserItem& item ) const
{
    set<const ParserSymbol*> lookahead_symbols;

    const ParserProduction* production = item.get_production().get();
    SWEET_ASSERT( production );
        
    const vector<ParserSymbol*>& symbols = production->get_symbols();    
    vector<ParserSymbol*>::const_iterator i = symbols.begin() + item.get_position();
    if ( i != symbols.end() )
    {
        ++i;
    }
    
    while ( i != symbols.end() && (*i)->is_nullable() )
    {
        const ParserSymbol* symbol = *i;
        SWEET_ASSERT( symbol );
        lookahead_symbols.insert( symbol->get_first().begin(), symbol->get_first().end() );
        ++i;
    }
    
    if ( i != symbols.end() )
    {
        const ParserSymbol* symbol = *i;
        SWEET_ASSERT( symbol );
        lookahead_symbols.insert( symbol->get_first().begin(), symbol->get_first().end() );
    }
    else
    {
        lookahead_symbols.insert( item.get_lookahead_symbols().begin(), item.get_lookahead_symbols().end() );
    }

    return lookahead_symbols;    
}

/**
// Generate the closure of the items contained in \e state.
//
// @param state
//  The ParserState that contains the items to generate the closure of.
*/
void ParserGenerator::closure( const std::shared_ptr<ParserState>& state )
{
    SWEET_ASSERT( state );

    int added = 1;
    while ( added > 0 )
    {
        added = 0;
        const set<ParserItem>& items = state->get_items();
        for ( set<ParserItem>::const_iterator item = items.begin(); item != items.end(); ++item )
        {          
            const ParserSymbol* symbol = item->get_production()->get_symbol_by_position( item->get_position() );
            if ( symbol )
            {
                const vector<std::shared_ptr<ParserProduction> >& productions = symbol->get_productions();
                for ( vector<std::shared_ptr<ParserProduction> >::const_iterator j = productions.begin(); j != productions.end(); ++j )
                {
                    const std::shared_ptr<ParserProduction>& production = *j;
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
std::shared_ptr<ParserState> ParserGenerator::goto_( const std::shared_ptr<ParserState>& state, const ParserSymbol& symbol )
{
    SWEET_ASSERT( state );

    std::shared_ptr<ParserState> goto_state( new ParserState() );

    const set<ParserItem>& items = state->get_items();
    for ( set<ParserItem>::const_iterator item = items.begin(); item != items.end(); ++item )
    {
        if ( item->is_next_node(symbol) )
        {
            goto_state->add_item( item->get_production(), item->get_position() + 1 );
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
int ParserGenerator::lookahead_closure( ParserState* state ) const
{
    SWEET_ASSERT( state );

    int added = 0;

    const set<ParserItem>& items = state->get_items();
    for ( set<ParserItem>::const_iterator item = items.begin(); item != items.end(); ++item )
    {          
        const ParserSymbol* symbol = item->get_production()->get_symbol_by_position( item->get_position() );
        if ( symbol )
        {
            std::set<const ParserSymbol*> lookahead_symbols = lookahead( *item );        
            const std::vector<std::shared_ptr<ParserProduction> >& productions = symbol->get_productions();
            for ( std::vector<std::shared_ptr<ParserProduction> >::const_iterator j = productions.begin(); j != productions.end(); ++j )
            {
                const std::shared_ptr<ParserProduction>& production = *j;
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
int ParserGenerator::lookahead_goto( ParserState* state ) const
{
    SWEET_ASSERT( state );

    int added = 0;

    const set<ParserTransition>& transitions = state->get_transitions();
    for ( set<ParserTransition>::const_iterator transition = transitions.begin(); transition != transitions.end(); ++transition )
    {
        const ParserSymbol* symbol = transition->get_symbol();
        SWEET_ASSERT( symbol );

        const set<ParserItem>& items = state->get_items();
        for ( set<ParserItem>::const_iterator item = items.begin(); item != items.end(); ++item )
        {
            int position = item->get_position();
            if ( item->get_production()->get_symbol_by_position(position) == symbol )
            {
                ParserState* goto_state = transition->get_state();
                added += goto_state->add_lookahead_symbols( item->get_production(), position + 1, item->get_lookahead_symbols() );
            }
        }        
    }

    return added;
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
void ParserGenerator::generate_states( const ParserSymbol* start_symbol, const ParserSymbol* end_symbol, const std::vector<std::shared_ptr<ParserSymbol> >& symbols )
{
    SWEET_ASSERT( start_symbol );
    SWEET_ASSERT( end_symbol );
    SWEET_ASSERT( states_.empty() );

    if ( !start_symbol->get_productions().empty() )
    {
        std::shared_ptr<ParserState> start_state( new ParserState() );
        start_state->add_item( start_symbol->get_productions().front(), 0 );
        closure( start_state );
        states_.insert( start_state );
        start_state_ = start_state.get();

        set<const ParserSymbol*> lookahead_symbols;
        lookahead_symbols.insert( (ParserSymbol*) end_symbol );
        start_state->add_lookahead_symbols( start_symbol->get_productions().front(), 0, lookahead_symbols );
        
        int added = 1;
        while ( added > 0 )
        {
            added = 0;
            for ( std::set<std::shared_ptr<ParserState>, shared_ptr_less<ParserState>>::const_iterator i = states_.begin(); i != states_.end(); ++i )
            {
                const std::shared_ptr<ParserState>& state = *i;
                SWEET_ASSERT( state );

                if ( !state->is_processed() )
                {
                    state->set_processed( true );
                    for ( vector<std::shared_ptr<ParserSymbol> >::const_iterator j = symbols.begin(); j != symbols.end(); ++j )
                    {
                        ParserSymbol* symbol = j->get();
                        SWEET_ASSERT( symbol );
                        if ( symbol != end_symbol )
                        {
                            std::shared_ptr<ParserState> goto_state = goto_( state, *symbol );
                            if ( !goto_state->get_items().empty() )
                            {                    
                                std::shared_ptr<ParserState> actual_goto_state = *states_.insert( goto_state ).first;
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
            for ( std::set<std::shared_ptr<ParserState>, shared_ptr_less<ParserState>>::const_iterator i = states_.begin(); i != states_.end(); ++i )
            {
                ParserState* state = i->get();
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
void ParserGenerator::generate_indices_for_states()
{
    int index = 0;
    for ( std::set<std::shared_ptr<ParserState>, shared_ptr_less<ParserState>>::iterator i = states_.begin(); i != states_.end(); ++i )
    {
        ParserState* state = i->get();
        SWEET_ASSERT( state );
        state->set_index( index );
        ++index;
    }
}

/**
// Generate reduction transitions.
*/
void ParserGenerator::generate_reduce_transitions()
{
    for ( std::set<std::shared_ptr<ParserState>, shared_ptr_less<ParserState>>::const_iterator i = states_.begin(); i != states_.end(); ++i )
    {
        ParserState* state = i->get();
        SWEET_ASSERT( state );
            
        for ( std::set<ParserItem>::const_iterator item = state->get_items().begin(); item != state->get_items().end(); ++item )
        {
            if ( item->is_dot_at_end() )
            {
                const set<const ParserSymbol*>& symbols = item->get_lookahead_symbols();
                for ( set<const ParserSymbol*>::const_iterator j = symbols.begin(); j != symbols.end(); ++j )
                {
                    const ParserSymbol* symbol = *j;
                    SWEET_ASSERT( symbol );
                    generate_reduce_transition( state, symbol, item->get_production().get() );
                }
            }                
        }
    }
}

/**
// Generate a reduction transition.
//
// @param state
//  The ParserState that the reduction occurs from.
//
// @param symbol
//  The ParserSymbol that the reduction is to be performed on.
//
// @param production
//  The ParserProduction that is to be reduced.
*/
void ParserGenerator::generate_reduce_transition( ParserState* state, const ParserSymbol* symbol, const ParserProduction* production )
{
    SWEET_ASSERT( state );
    SWEET_ASSERT( symbol );
    SWEET_ASSERT( production );

    ParserTransition* transition = state->find_transition_by_symbol( symbol );
    if ( !transition )
    {
        state->add_transition( symbol, production );
    }
    else
    {
        switch ( transition->get_type() )
        {
            case TRANSITION_SHIFT:
            {
                if ( production->get_precedence() == 0 || symbol->get_precedence() == 0 || (symbol->get_precedence() == production->get_precedence() && symbol->get_associativity() == ASSOCIATE_NULL) )
                {
                    fire_error( production->get_line(), PARSER_ERROR_PARSE_TABLE_CONFLICT, "Shift/reduce conflict on '%s' for '%s'", symbol->get_identifier().c_str(), production->get_symbol()->get_identifier().c_str() );
                }
                else if ( production->get_precedence() > symbol->get_precedence() || (symbol->get_precedence() == production->get_precedence() && symbol->get_associativity() == ASSOCIATE_RIGHT) )
                {
                    transition->override_shift_to_reduce( production );
                }

                break;
            }
            
            case TRANSITION_REDUCE:
            {
                const ParserProduction* other_production = transition->get_reduced_production();
                SWEET_ASSERT( other_production );
                if ( production->get_precedence() == 0 || other_production->get_precedence() == 0 || production->get_precedence() == other_production->get_precedence() )
                {
                    fire_error( production->get_line(), PARSER_ERROR_PARSE_TABLE_CONFLICT, "Reduce/reduce conflict on '%s' for '%s' and '%s'", symbol->get_identifier().c_str(), production->get_symbol()->get_identifier().c_str(), other_production->get_symbol()->get_identifier().c_str() );
                }
                else
                {
                    const ParserProduction* reduced_production = production->get_precedence() > other_production->get_precedence() ? production : other_production;
                    transition->override_reduce_to_reduce( reduced_production );
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
void ParserGenerator::generate_indices_for_transitions()
{
    for ( std::set<std::shared_ptr<ParserState>, shared_ptr_less<ParserState>>::const_iterator i = states_.begin(); i != states_.end(); ++i )
    {
        ParserState* state = i->get();
        SWEET_ASSERT( state );
        state->generate_indices_for_transitions();        
    }
}
