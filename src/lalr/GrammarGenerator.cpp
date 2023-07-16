//
// GrammarGenerator.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "GrammarGenerator.hpp"
#include "GrammarProduction.hpp"
#include "GrammarTransition.hpp"
#include "GrammarState.hpp"
#include "GrammarItem.hpp"
#include "Grammar.hpp"
#include "GrammarSymbol.hpp"   
#include "GrammarAction.hpp"
#include "GrammarCompiler.hpp"
#include "GrammarProductionLess.hpp"
#include "GrammarLookahead.hpp"
#include "ParserState.hpp"
#include "ParserAction.hpp"
#include "ParserSymbol.hpp"
#include "ParserTransition.hpp"
#include "ParserStateMachine.hpp"
#include "RegexGenerator.hpp"
#include "RegexCompiler.hpp"
#ifndef LALR_NO_THREADS
#include "ThreadPool.hpp"
#endif
#include "ErrorPolicy.hpp"
#include "ErrorCode.hpp"
#include "assert.hpp"
#include <memory>

using std::set;
using std::vector;
using std::multimap;
using std::string;
using std::unique_ptr;
using std::shared_ptr;
using std::make_shared;
using std::make_unique;
using namespace lalr;

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
: error_policy_( nullptr )
#ifndef LALR_NO_THREADS
, thread_pool_( nullptr )
#endif
, identifier_()
, actions_()
, productions_()
, symbols_()
, states_()
, transitions_()
, lookaheads_()
, start_symbol_( nullptr )
, end_symbol_( nullptr )
, error_symbol_( nullptr )
, whitespace_symbol_( nullptr )
, start_state_( nullptr )
, errors_( 0 )
, shift_reduce_count_( 0 )
, reduce_reduce_count_( 0 )
{
#ifndef LALR_NO_THREADS
    thread_pool_ = new ThreadPool;
    thread_pool_->start( 8 );
#endif
}

GrammarGenerator::~GrammarGenerator()
{
#ifndef LALR_NO_THREADS
    delete thread_pool_;
    thread_pool_ = nullptr;
#endif
}

const std::vector<std::unique_ptr<GrammarAction>>& GrammarGenerator::actions() const
{
    return actions_;
}

const std::vector<std::unique_ptr<GrammarSymbol>>& GrammarGenerator::symbols() const
{
    return symbols_;
}

const std::set<std::shared_ptr<GrammarState>, GrammarStateLess>& GrammarGenerator::states() const
{
    return states_;
}

const std::vector<std::unique_ptr<GrammarTransition>>& GrammarGenerator::transitions() const
{
    return transitions_;
}

const GrammarState* GrammarGenerator::start_state() const
{
    return start_state_;
}

const GrammarSymbol* GrammarGenerator::whitespace_symbol() const
{
    return whitespace_symbol_;
}

std::string GrammarGenerator::label_state( const GrammarState& state ) const
{
    string label;
    for ( const GrammarItem& item : state.items() )
    {
        if ( !label.empty() )
        {
            label += "\\n";
        }
        label += label_item( item );
    }
    return label;
}

std::string GrammarGenerator::label_item( const GrammarItem& item ) const
{
    string label;

    const GrammarProduction* production = productions_[item.production()].get();
    LALR_ASSERT( production );

    label += production->symbol()->lexeme();
    label += " ->";

    int position = 0;
    for ( const GrammarSymbol* symbol : production->symbols() )
    {
        if ( !label.empty() )
        {
            label += " ";
        }
        if ( position == item.position() )
        {
            label += ".";
        }
        label += symbol->literal() ? symbol->lexeme() : symbol->identifier();
        ++position;
    }

    if ( position == item.position() )
    {
        label += ".";
    }

    LALR_ASSERT( item.index() >= 0 && item.index() < int(lookaheads_.size()) );
    const GrammarSymbolSet& lookaheads = lookaheads_[item.index()].lookaheads();
    const char* delimiter = " {";
    for ( int i = 0; i < int(symbols_.size()); ++i )
    {
        if ( lookaheads.contains(i) )
        {
            const GrammarSymbol* symbol = symbols_[i].get();
            LALR_ASSERT( symbol );
            label += delimiter;
            delimiter = ", ";
            label += symbol->lexeme();
        }
    }
    label += "}";

    return label;
}

int GrammarGenerator::generate( Grammar& grammar, ErrorPolicy* error_policy )
{
    error_policy_ = error_policy;
    identifier_ = grammar.identifier();
    actions_.swap( grammar.actions() );
    productions_.swap( grammar.productions() );
    symbols_.swap( grammar.symbols() );
    states_.clear();
    transitions_.clear();
    start_symbol_ = grammar.start_symbol();
    end_symbol_ = grammar.end_symbol();
    error_symbol_ = grammar.error_symbol();
    whitespace_symbol_ = grammar.whitespace_symbol();
    start_state_ = nullptr;
    errors_ = 0;

    calculate_identifiers();
    check_for_undefined_symbol_errors();
    check_for_unreferenced_symbol_errors();
    check_for_implicit_terminal_duplicate_associativity();
    check_for_error_symbol_on_left_hand_side_errors();

    if ( errors_ == 0 )
    {        
        calculate_terminal_and_non_terminal_symbols();
        calculate_implicit_terminal_symbols();
        calculate_symbol_indices();
        calculate_first();
        calculate_follow();
        calculate_precedence_of_productions();
        calculate_reachable_productions();
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
void GrammarGenerator::error( int line, int error, const char* format, ... )
{
    ++errors_;    
    if ( error_policy_ )
    {
        va_list args;
        va_start( args, format );
        error_policy_->lalr_error( line, 0, error, format, args );
        va_end( args );
    }
}

GrammarTransition* GrammarGenerator::shift_transition( const GrammarSymbol* symbol, GrammarState* state )
{
    transitions_.emplace_back( make_unique<GrammarTransition>(symbol, state) );
    return transitions_.back().get();
}

GrammarTransition* GrammarGenerator::reduce_transition( const GrammarSymbol* symbol, const GrammarProduction* production )
{
    transitions_.emplace_back( make_unique<GrammarTransition>(symbol, production) );
    return transitions_.back().get();
}

/**
// Generate the spontaneous lookaheads for an item.
//
// @param item
//  The item to generate spontaneous lookaheads for.
//
// @return
//  The generated lookahead symbols.
*/
GrammarSymbolSet GrammarGenerator::spontaneous_lookaheads( const GrammarItem& item ) const
{
    GrammarSymbolSet lookaheads(symbols_.size());

    const GrammarProduction* production = productions_[item.production()].get();
    LALR_ASSERT( production );
        
    const vector<GrammarSymbol*>& symbols = production->symbols();    
    vector<GrammarSymbol*>::const_iterator i = symbols.begin() + item.position();
    if ( i != symbols.end() )
    {
        ++i;
    }
    
    while ( i != symbols.end() && (*i)->nullable() )
    {
        const GrammarSymbol* symbol = *i;
        LALR_ASSERT( symbol );
        lookaheads.insert( symbol->first() );
        ++i;
    }

    if ( i != symbols.end() )
    {
        const GrammarSymbol* symbol = *i;
        LALR_ASSERT( symbol );
        lookaheads.insert( symbol->first() );
    }

    return lookaheads;    
}

/**
// Generate the closure of the items contained in \e state.
//
// @param state
//  The GrammarState that contains the items to generate the closure of.
*/
void GrammarGenerator::closure( const std::shared_ptr<GrammarState>& state )
{
    LALR_ASSERT( state );

    int added = 1;
    while ( added > 0 )
    {
        added = 0;
        for ( const GrammarItem& item : state->items() )
        {
            const GrammarProduction* production = productions_[item.production()].get();
            LALR_ASSERT( production );
            const GrammarSymbol* symbol = production->symbol_by_position( item.position() );
            if ( symbol )
            {
                for ( GrammarProduction* production : symbol->productions() )
                {
                    LALR_ASSERT( production );
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
std::shared_ptr<GrammarState> GrammarGenerator::goto_( const GrammarState* state, const GrammarSymbol& symbol )
{
    LALR_ASSERT( state );

    GrammarState goto_state;
    for ( const GrammarItem& item : state->items() )
    {
        GrammarProduction* production = productions_[item.production()].get();
        LALR_ASSERT( production );

        const GrammarSymbol* next_symbol = production->symbol_by_position( item.position() );
        if ( next_symbol )
        {
            if ( next_symbol == &symbol )
            {
                goto_state.add_item( production, item.position() + 1 );
            }
    
            const multimap<const GrammarSymbol*, GrammarProduction*>& reachable_productions = next_symbol->reachable_productions_by_first_symbol();
            auto i = reachable_productions.find( &symbol );
            while ( i != reachable_productions.end() && i->first == &symbol )
            {
                GrammarProduction* production = i->second;
                const int position = 1;
                goto_state.add_item( production, position );
                ++i;
            }
        }
    }

    if ( !goto_state.items().empty() )
    {
        return make_shared<GrammarState>( std::move(goto_state) );
    }
    return nullptr;
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
        LALR_ASSERT( production );
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
            LALR_ASSERT( symbol );
            if ( symbol->symbol_type() == SYMBOL_NON_TERMINAL && symbol->productions().empty() && (symbol->referenced_in_rule() || !symbol->referenced_in_precedence_directive()) )
            {
                error( symbol->line(), PARSER_ERROR_UNDEFINED_SYMBOL, "undefined symbol '%s'", symbol->identifier().c_str(), identifier_.c_str() );
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
            LALR_ASSERT( symbol );
            
            int references = 0;            
            if ( symbol != start_symbol_ && symbol != end_symbol_ && symbol->lexeme() != error_symbol_->lexeme() && symbol != whitespace_symbol_ )
            {
                for ( vector<unique_ptr<GrammarProduction>>::const_iterator i = productions_.begin(); i != productions_.end(); ++i )
                {
                    const GrammarProduction* production = i->get();
                    LALR_ASSERT( production );
                    if ( production->symbol()->symbol_type() != SYMBOL_TERMINAL )
                    {
                        references += production->count_references_to_symbol( symbol );
                    }
                }

                if ( references == 0 )
                {
                    error( symbol->line(), PARSER_ERROR_UNREFERENCED_SYMBOL, "unreferenced symbol '%s'/'%s'", symbol->identifier().c_str(), symbol->lexeme().c_str() );
                }
            }
        }
    }
}

/**
// Check implicit terminals define precedence and associativity once.
//
// Implicit terminals must define precedence and associativity only on one of
// their forms.  Either the literal/regular expression or the non-terminal that
// labels them until they are combined.  That precedence and associativity is
// moved to the implicit terminal when the two symbols are merged.
*/
void GrammarGenerator::check_for_implicit_terminal_duplicate_associativity()
{
    for ( vector<unique_ptr<GrammarSymbol>>::iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        GrammarSymbol* non_terminal = i->get();        
        if ( non_terminal && non_terminal != error_symbol_ )
        {
            GrammarSymbol* terminal = non_terminal->implicit_terminal();
            if ( terminal )
            {       
                LALR_ASSERT( terminal != non_terminal );
                bool terminal_associates = terminal->associativity() != ASSOCIATE_NULL;
                bool non_terminal_associates = non_terminal->associativity() != ASSOCIATE_NULL;
                if ( terminal_associates && non_terminal_associates )
                {
                    error( terminal->line(), PARSER_ERROR_DUPLICATE_ASSOCIATION_ON_IMPLICIT_TERMINAL, "implicit terminal '%s'/%s' specifies associativity and precedence on both forms", terminal->lexeme().c_str(), non_terminal->lexeme().c_str() );
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
    LALR_ASSERT( error_symbol_ );

    for ( auto i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        GrammarSymbol* symbol = i->get();
        LALR_ASSERT( symbol );
        if ( !symbol->productions().empty() && symbol->lexeme() == error_symbol_->lexeme() )
        {
            error( 1, PARSER_ERROR_ERROR_SYMBOL_ON_LEFT_HAND_SIDE, "error appears on the left hand side of a production" );
        }
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
        LALR_ASSERT( symbol );
        symbol->calculate_identifier();
    }
}

/**
// Calculate which symbols are terminal and non-terminal.
//
// Any symbols with one or more productions are assumed to be non-terminals
// and any symbols with no productions are assumed to be terminals.  Another
// pass is made over the symbols to convert non-terminals symbols with only
// a single production with one terminal symbol into terminals.
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
// Calculate the non-terminal symbols that are really named terminals.
//
// Any symbols that contain a single production that contains only a terminal 
// symbol are aliases for that terminal symbol.  To make the parser easier to
// understand and more efficient these symbols are collapsed by making any
// references to the non-terminal refer directly to the terminal.  The
// identifier of the terminal is changed to be the more readable name of the
// non-terminal.
//
// For example the rule 'integer: "[0-9]+";' creates a non-terminal
// symbol 'integer' and a terminal symbol '"[0-9]+"'.  The non-terminal
// symbol 'integer' is redundant from the point of view of the parser as it
// adds only a trivial reduction from one symbol type to another.  To optimize
// this situation the terminal is collapsed into the non-terminal keeping the
// more readable name of the non-terminal but removing the redundant 
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
                LALR_ASSERT( terminal_symbol != non_terminal_symbol );
                terminal_symbol->replace_by_non_terminal( non_terminal_symbol );
                replace_references_to_symbol( non_terminal_symbol, terminal_symbol );
                i->reset();
            }
            else if(non_terminal_symbol->associativity() != ASSOCIATE_NULL && non_terminal_symbol->productions().size())
            {
                error( non_terminal_symbol->line(), PARSER_ERROR_ASSOCIATIVITY_ASSIGNED_ON_NON_TERMINAL, "associativity has been assigned to a non-terminal: %s",  non_terminal_symbol->identifier().c_str());
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
        LALR_ASSERT( production );       
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
        LALR_ASSERT( symbol );
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
            LALR_ASSERT( symbol );
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
            LALR_ASSERT( symbol );
            added += symbol->calculate_follow();
        }
    }
}

/**
// Calculate the productions reachable by right-most derivation from each
// non-terminal.
//
// This pre-calculation is used to add items to goto states that would have
// been added for transitions from non-kernel items.  This is needed because
// non-kernel items are excluded in an efficient construction of LALR(1)
// states.  The pre-calculation provides a more efficient and convenient
// means of discovering items added for transitions from non-kernel items.
*/
void GrammarGenerator::calculate_reachable_productions()
{
    for ( const auto& symbol : symbols_ )
    {
        set<GrammarProduction*, GrammarProductionLess> productions;
        calculate_reachable_productions_for_symbol( *symbol, &productions );
        for ( const auto& production : productions )
        {
            symbol->append_reachable_production( production );
        }
    }
}

/**
// Recursively calculate productions reachable by derivation from symbol.
*/
void GrammarGenerator::calculate_reachable_productions_for_symbol( const GrammarSymbol& symbol, std::set<GrammarProduction*, GrammarProductionLess>* productions )
{
    LALR_ASSERT( productions );
    for ( const auto& production : symbol.productions() )
    {
        const GrammarSymbol* first_symbol = production->symbol_by_position( 0 );
        if ( first_symbol )
        {
            bool inserted = productions->insert( production ).second;
            if ( inserted )
            {
                calculate_reachable_productions_for_symbol( *first_symbol, productions );
            }
        }
    }
}

/**
// Generate the spontaneous lookaheads for each item in each state.
*/
void GrammarGenerator::generate_spontaneous_lookaheads()
{
    const size_t symbols = symbols_.size();
    lookaheads_.clear();        
    for ( const shared_ptr<GrammarState>& state : states_ )
    {
        for ( const GrammarItem& item : state->items() )
        {
            int index = int(lookaheads_.size());
            item.set_index( index );
            lookaheads_.emplace_back( const_cast<GrammarItem*>(&item), symbols );
            ++index;
        }
    }

    {
        GrammarSymbolSet lookaheads{ symbols };
        lookaheads.insert( end_symbol_ );
        GrammarItem* start_item = start_state_->find_item( start_symbol_->productions().front(), 0 );
        LALR_ASSERT( start_item );
        lookaheads_[start_item->index()].add_lookaheads( lookaheads );
    }

    for ( const shared_ptr<GrammarState>& state : states_ )
    {
        for ( const GrammarItem& item : state->items() )
        {
            const GrammarProduction* production = productions_[item.production()].get();
            LALR_ASSERT( production );

            const GrammarSymbol* symbol = production->symbol_by_position( item.position() );
            if ( symbol )
            {
                GrammarSymbolSet lookaheads = spontaneous_lookaheads( item );
                for ( GrammarProduction* production : symbol->productions() )
                {
                    const GrammarItem* other_item = state->find_item( production, 0 );
                    if ( other_item )
                    {
                        lookaheads_[other_item->index()].add_lookaheads( lookaheads );
                    }
                }
            }
        }
    }
}

/**
// Generate goto items to which lookaheads are propagated.
*/
void GrammarGenerator::generate_goto_items()
{
    for ( const shared_ptr<GrammarState>& state : states_ )
    {
#ifndef LALR_NO_THREADS        
        thread_pool_->push_job( [this, state]()
#endif            
            {
                for ( GrammarTransition* transition : state->transitions() )
                {
                    if ( transition )
                    {
                        const GrammarSymbol* symbol = transition->symbol();
                        for ( const GrammarItem& item : state->items() )
                        {
                            GrammarLookahead& destination_lookaheads = lookaheads_[item.index()];
                            GrammarProduction* production = productions_[item.production()].get();
                            int position = item.position();
                            if ( production->symbol_by_position(position) == symbol )
                            {
                                GrammarState* goto_state = transition->state();
                                LALR_ASSERT( goto_state );
                                GrammarItem* goto_item = goto_state->find_item( production, position + 1 );
                                LALR_ASSERT( goto_item );
                                destination_lookaheads.add_propagate_to( &lookaheads_[goto_item->index()] );
                            }
                        }
                    }
                }

                for ( const GrammarItem& item : state->items() )
                {
                    const GrammarProduction* production = productions_[item.production()].get();
                    LALR_ASSERT( production );
                    const GrammarSymbol* symbol = production->symbol_by_position( item.position() );
                    if ( symbol && production->nullable_after(item.position() + 1) )
                    {
                        GrammarLookahead& lookaheads = lookaheads_[item.index()];
                        for ( GrammarProduction* production : symbol->productions() )
                        {
                            GrammarItem* goto_item = state->find_item( production, 0 );
                            if ( goto_item )
                            {
                                lookaheads.add_propagate_to( &lookaheads_[goto_item->index()] );
                            }
                        }
                    }
                }
            }
#ifndef LALR_NO_THREADS    
        );
#endif
    }
#ifndef LALR_NO_THREADS    
    thread_pool_->wait_idle();
#endif
}

/**
// Propagate lookaheads for each item until no more lookaheads can be added.
*/
void GrammarGenerator::generate_propagated_lookaheads()
{
    vector<GrammarLookahead*> lookaheads;
    lookaheads.reserve( lookaheads_.size() );

    for ( GrammarLookahead& lookahead : lookaheads_ )
    {
        lookaheads.push_back( &lookahead );
    }

    vector<GrammarLookahead*> next_lookaheads;
    next_lookaheads.reserve( lookaheads.size() );

    while ( !lookaheads.empty() )
    {
        for ( GrammarLookahead* lookahead : lookaheads )
        {
            for ( GrammarLookahead* other_lookahead : lookahead->propagate_to() )
            {
                LALR_ASSERT( other_lookahead );
                if ( other_lookahead->add_lookaheads(lookahead->lookaheads()) )
                {
                    next_lookaheads.push_back( other_lookahead );
                }
            }
        }

        lookaheads.clear();
        lookaheads.swap( next_lookaheads );
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
    LALR_ASSERT( start_symbol );
    LALR_ASSERT( end_symbol );
    LALR_ASSERT( states_.empty() );

    if ( !start_symbol->productions().empty() )
    {
        shared_ptr<GrammarState> start_state = make_shared<GrammarState>();
        start_state->add_item( start_symbol->productions().front(), 0 );
        closure( start_state );
        states_.insert( start_state );
        start_state_ = start_state.get();

        {
            vector<GrammarState*> next_states;
            vector<GrammarState*> states;
            states.push_back( start_state_ );

            while ( !states.empty() )
            {
                for ( GrammarState* state : states )
                {
                    for ( vector<unique_ptr<GrammarSymbol>>::const_iterator j = symbols.begin(); j != symbols.end(); ++j )
                    {
                        GrammarSymbol* symbol = j->get();
                        LALR_ASSERT( symbol );
                        if ( symbol != end_symbol )
                        {
                            std::shared_ptr<GrammarState> goto_state = goto_( state, *symbol );
                            if ( goto_state )
                            {
                                LALR_ASSERT( !goto_state->items().empty() );
                                std::shared_ptr<GrammarState> actual_goto_state = *states_.insert( goto_state ).first;
                                if ( goto_state == actual_goto_state )
                                {
                                    next_states.push_back( goto_state.get() );
                                }
                                state->add_shift_transition( shift_transition(symbol, actual_goto_state.get()) );
                            }
                        }
                    }
                }

                states.clear();
                next_states.swap( states );
            }
        }

        vector<shared_ptr<GrammarState>> states;

        states.reserve( states_.size() );
        for ( const auto& state : states_ )
        {
            states.push_back( state );
        }

        states_.clear();
        for ( const auto& state : states )
        {
            closure( state );
            states_.insert( state );
        }
        
        generate_indices_for_states();
        generate_spontaneous_lookaheads();
        generate_goto_items();
        generate_propagated_lookaheads();
        generate_reduce_transitions();
    }
}

/**
// Generate indices for states.
*/
void GrammarGenerator::generate_indices_for_states()
{
    int index = 0;
    for ( std::set<std::shared_ptr<GrammarState>, GrammarStateLess>::iterator i = states_.begin(); i != states_.end(); ++i )
    {
        GrammarState* state = i->get();
        LALR_ASSERT( state );
        state->set_index( index );
        ++index;
    }
}

/**
// Generate reduction transitions.
*/
void GrammarGenerator::generate_reduce_transitions()
{
    for ( set<shared_ptr<GrammarState>, GrammarStateLess>::const_iterator i = states_.begin(); i != states_.end(); ++i )
    {
        GrammarState* state = i->get();
        LALR_ASSERT( state );

        for ( set<GrammarItem>::const_iterator item = state->items().begin(); item != state->items().end(); ++item )
        {
            const GrammarProduction* production = productions_[item->production()].get();
            LALR_ASSERT( production );
            if ( item->position() >= production->length() )
            {
                const GrammarSymbolSet& lookaheads = lookaheads_[item->index()].lookaheads();
                size_t start = lookaheads.minimum_index();
                size_t finish = lookaheads.maximum_index();
                for ( size_t index = start; index <= finish; ++index )
                {
                    if ( lookaheads.contains(index) )
                    {
                        const GrammarSymbol* symbol = symbols_[index].get();
                        LALR_ASSERT( symbol );
                        generate_reduce_transition( state, symbol, production );
                    }
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
    LALR_ASSERT( state );
    LALR_ASSERT( symbol );
    LALR_ASSERT( production );

    GrammarTransition* transition = state->find_transition_by_symbol( symbol );
    if ( !transition )
    {
        state->add_reduce_transition( reduce_transition(symbol, production) );
    }
    else
    {
        if ( transition->is_shift() )
        {
            if ( production->precedence() == 0 || symbol->precedence() == 0 || (symbol->precedence() == production->precedence() && symbol->associativity() == ASSOCIATE_NULL) )
            {
                error( production->line(), PARSER_ERROR_PARSE_TABLE_CONFLICT, "shift/reduce conflict for '%s' on '%s'", production->symbol()->identifier().c_str(), symbol->lexeme().c_str() );
            }
            else if ( production->precedence() > symbol->precedence() || (symbol->precedence() == production->precedence() && symbol->associativity() == ASSOCIATE_RIGHT) )
            {
                transition->override_shift_to_reduce( production );
                ++shift_reduce_count_;
            }
        }
        else
        {
            LALR_ASSERT( transition->is_reduce() );
            if ( production->precedence() == 0 || transition->precedence() == 0 || production->precedence() == transition->precedence() )
            {
                error( production->line(), PARSER_ERROR_PARSE_TABLE_CONFLICT, "reduce/reduce conflict for '%s' and '%s' on '%s'", production->symbol()->identifier().c_str(), transition->reduced_symbol()->identifier().c_str(), symbol->lexeme().c_str() );
            }
            else if ( production->precedence() > transition->precedence() )
            {
                transition->override_reduce_to_reduce( production );
                ++reduce_reduce_count_;
            }
        }
    }
}
