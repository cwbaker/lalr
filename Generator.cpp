//
// Generator.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "Generator.hpp"
#include "Production.hpp"
#include "State.hpp"
#include "Item.hpp"
#include "Grammar.hpp"
#include "Symbol.hpp"   
#include "Action.hpp"
#include "ParserErrorPolicy.hpp"
#include "ParserState.hpp"
#include "ParserAction.hpp"
#include "ParserSymbol.hpp"
#include "ParserTransition.hpp"
#include "ParserStateMachine.hpp"
#include "LexerStateMachine.hpp"
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
Generator::Generator( Grammar& grammar, ParserStateMachine* parser_state_machine, ParserErrorPolicy* error_policy, LexerErrorPolicy* lexer_error_policy )
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
    generate( grammar, parser_state_machine, lexer_error_policy );
}

Generator::~Generator()
{
}

/**
// Get the identifier of the grammar.
//
// @return
//  The identifier.
*/
std::string& Generator::identifier()
{
    return identifier_;
}

/**
// Get the actions.
//
// @return
//  The actions.
*/
std::vector<std::unique_ptr<Action> >& Generator::actions()
{
    return actions_;
}

/**
// Get the productions.
//
// @return
//  The productions.
*/
std::vector<std::unique_ptr<Production> >& Generator::productions()
{
    return productions_;
}

/**
// Get the symbols.
//
// @return
//  The symbols.
*/
std::vector<std::unique_ptr<Symbol> >& Generator::symbols()
{
    return symbols_;
}

/**
// Get the states.
//
// @return
//  The states.
*/
std::set<std::shared_ptr<State>, shared_ptr_less<State>>& Generator::states()
{
    return states_;
}

/**
// Get the start symbol.
//
// @return
//  The start symbol.
*/
const Symbol* Generator::start_symbol()
{
    return start_symbol_;
}

/**
// Get the end symbol.
//
// @return
//  The end symbol.
*/
const Symbol* Generator::end_symbol()
{
    return end_symbol_;
}

/**
// Get the error symbol.
//
// @return
//  The error symbol.
*/
const Symbol* Generator::error_symbol()
{
    return error_symbol_;
}

/**
// Get the start state.
//
// @return
//  The start state.
*/
State* Generator::start_state()
{
    return start_state_;
}

/**
// Get the number of errors that occured during parsing and generation.
//
// Other Generator functions are invalid if the number of errors
// returned by this function is not zero after the Generator has
// been constructed.
//
// @return
//  The number of errors.
*/
int Generator::errors() const
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
void Generator::fire_error( int line, int error, const char* format, ... )
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
void Generator::fire_printf( const char* format, ... ) const
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

void Generator::generate( Grammar& grammar, ParserStateMachine* parser_state_machine, LexerErrorPolicy* lexer_error_policy )
{
    SWEET_ASSERT( parser_state_machine );

    identifier_ = grammar.identifier();
    actions_.swap( grammar.actions() );
    productions_.swap( grammar.productions() );
    symbols_.swap( grammar.symbols() );
    start_symbol_ = grammar.start_symbol();
    end_symbol_ = grammar.end_symbol();
    error_symbol_ = grammar.error_symbol();
    states_.clear();
    start_state_ = NULL;

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
        populate_parser_state_machine( grammar.whitespace_tokens(), parser_state_machine, lexer_error_policy );
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
std::set<const Symbol*> Generator::lookahead( const Item& item ) const
{
    set<const Symbol*> lookahead_symbols;

    const Production* production = item.get_production();
    SWEET_ASSERT( production );
        
    const vector<Symbol*>& symbols = production->symbols();    
    vector<Symbol*>::const_iterator i = symbols.begin() + item.get_position();
    if ( i != symbols.end() )
    {
        ++i;
    }
    
    while ( i != symbols.end() && (*i)->nullable() )
    {
        const Symbol* symbol = *i;
        SWEET_ASSERT( symbol );
        lookahead_symbols.insert( symbol->first().begin(), symbol->first().end() );
        ++i;
    }
    
    if ( i != symbols.end() )
    {
        const Symbol* symbol = *i;
        SWEET_ASSERT( symbol );
        lookahead_symbols.insert( symbol->first().begin(), symbol->first().end() );
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
//  The State that contains the items to generate the closure of.
*/
void Generator::closure( const std::shared_ptr<State>& state )
{
    SWEET_ASSERT( state );

    int added = 1;
    while ( added > 0 )
    {
        added = 0;
        const set<Item>& items = state->get_items();
        for ( set<Item>::const_iterator item = items.begin(); item != items.end(); ++item )
        {          
            const Symbol* symbol = item->get_production()->symbol_by_position( item->get_position() );
            if ( symbol )
            {
                const vector<Production*>& productions = symbol->productions();
                for ( vector<Production*>::const_iterator j = productions.begin(); j != productions.end(); ++j )
                {
                    Production* production = *j;
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
std::shared_ptr<State> Generator::goto_( const std::shared_ptr<State>& state, const Symbol& symbol )
{
    SWEET_ASSERT( state );

    std::shared_ptr<State> goto_state( new State() );

    const set<Item>& items = state->get_items();
    for ( set<Item>::const_iterator item = items.begin(); item != items.end(); ++item )
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
int Generator::lookahead_closure( State* state ) const
{
    SWEET_ASSERT( state );

    int added = 0;

    const set<Item>& items = state->get_items();
    for ( set<Item>::const_iterator item = items.begin(); item != items.end(); ++item )
    {          
        const Symbol* symbol = item->get_production()->symbol_by_position( item->get_position() );
        if ( symbol )
        {
            set<const Symbol*> lookahead_symbols = lookahead( *item );        
            const vector<Production*>& productions = symbol->productions();
            for ( vector<Production*>::const_iterator j = productions.begin(); j != productions.end(); ++j )
            {
                Production* production = *j;
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
int Generator::lookahead_goto( State* state ) const
{
    SWEET_ASSERT( state );

    int added = 0;

    const set<Transition>& transitions = state->get_transitions();
    for ( set<Transition>::const_iterator transition = transitions.begin(); transition != transitions.end(); ++transition )
    {
        const Symbol* symbol = transition->get_symbol();
        SWEET_ASSERT( symbol );

        const set<Item>& items = state->get_items();
        for ( set<Item>::const_iterator item = items.begin(); item != items.end(); ++item )
        {
            int position = item->get_position();
            if ( item->get_production()->symbol_by_position(position) == symbol )
            {
                State* goto_state = transition->get_state();
                added += goto_state->add_lookahead_symbols( item->get_production(), position + 1, item->get_lookahead_symbols() );
            }
        }        
    }

    return added;
}

/**
// Replace references to \e to_symbol with references to \e with_symbol.
//
// @param to_symbol
//  The Symbol to replace references to.
//
// @param with_symbol
//  The Symbol to replace references with.
*/
void Generator::replace_references_to_symbol( Symbol* to_symbol, Symbol* with_symbol )
{
    for ( vector<unique_ptr<Production>>::const_iterator i = productions_.begin(); i != productions_.end(); ++i )
    {
        Production* production = i->get();
        SWEET_ASSERT( production );
        production->replace_references_to_symbol( to_symbol, with_symbol );
    }
}

/**
// Check for symbols in the grammar that are referenced but never defined.
*/
void Generator::check_for_undefined_symbol_errors()
{
    if ( errors() == 0 )
    {
        for ( vector<unique_ptr<Symbol>>::const_iterator i = symbols_.begin(); i != symbols_.end(); ++i )
        {
            const Symbol* symbol = i->get();
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
//  The Generator for fire any errors from (assumed not null).
*/
void Generator::check_for_unreferenced_symbol_errors()
{
    if ( errors() == 0 )
    {
        for ( vector<unique_ptr<Symbol>>::const_iterator i = symbols_.begin(); i != symbols_.end(); ++i )
        {
            const Symbol* symbol = i->get();
            SWEET_ASSERT( symbol );
            
            int references = 0;            
            if ( symbol != start_symbol_ && symbol != end_symbol_ && symbol != error_symbol_ )
            {
                for ( vector<unique_ptr<Production>>::const_iterator i = productions_.begin(); i != productions_.end(); ++i )
                {
                    const Production* production = i->get();
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
//  The Generator for fire any errors from (assumed not null).
*/
void Generator::check_for_error_symbol_on_left_hand_side_errors()
{
    SWEET_ASSERT( error_symbol_ );

    const vector<Production*>& productions = error_symbol_->productions();
    if ( !productions.empty() )
    {
        fire_error( 1, PARSER_ERROR_ERROR_SYMBOL_ON_LEFT_HAND_SIDE, "The 'error' symbol appears on the left hand side of a production" );
    }
}

/**
// Calculate identifiers for all symbols.
*/
void Generator::calculate_identifiers()
{
    for ( vector<unique_ptr<Symbol>>::const_iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        Symbol* symbol = i->get();
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
void Generator::calculate_terminal_and_non_terminal_symbols()
{
    for ( vector<unique_ptr<Symbol>>::const_iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        Symbol* symbol = i->get();
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
void Generator::calculate_implicit_terminal_symbols()
{
    for ( vector<unique_ptr<Symbol>>::iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        Symbol* non_terminal_symbol = i->get();        
        if ( non_terminal_symbol && non_terminal_symbol != error_symbol_ )
        {
            Symbol* terminal_symbol = non_terminal_symbol->implicit_terminal();
            if ( terminal_symbol )
            {       
                SWEET_ASSERT( terminal_symbol != non_terminal_symbol );
                terminal_symbol->replace_by_non_terminal( non_terminal_symbol );
                replace_references_to_symbol( non_terminal_symbol, terminal_symbol );
                i->reset();
            }
        }
    }
    
    vector<unique_ptr<Symbol>>::iterator i = symbols_.begin();
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
void Generator::calculate_precedence_of_productions()
{
    for ( vector<unique_ptr<Production>>::const_iterator i = productions_.begin(); i != productions_.end(); ++i )
    {
        Production* production = i->get();
        SWEET_ASSERT( production );       
        if ( production->precedence() == 0 )
        {
            const Symbol* symbol = production->find_rightmost_terminal_symbol();
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
void Generator::calculate_symbol_indices()
{
    int index = 0;
    for ( vector<unique_ptr<Symbol>>::iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        Symbol* symbol = i->get();
        SWEET_ASSERT( symbol );
        symbol->set_index( index );
        ++index;
    }
}

/**
// Calculate the first position sets for each Symbol until no more 
// terminals can be added to any first position sets.
*/
void Generator::calculate_first()
{
    int added = 1;
    while ( added > 0 )
    {
        added = 0;
        for ( vector<unique_ptr<Symbol>>::iterator i = symbols_.begin(); i != symbols_.end(); ++i )
        {
            Symbol* symbol = i->get();
            SWEET_ASSERT( symbol );
            added += symbol->calculate_first();
        }
    }
}

/**
// Calculate the follow position sets for each Symbol until no more 
// terminals can be added to any follow position sets.
*/
void Generator::calculate_follow()
{
    start_symbol_->add_symbol_to_follow( end_symbol_ );

    int added = 1;
    while ( added > 0 )
    {
        added = 0;
        for ( vector<unique_ptr<Symbol>>::iterator i = symbols_.begin(); i != symbols_.end(); ++i )
        {
            Symbol* symbol = i->get();
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
void Generator::generate_states( const Symbol* start_symbol, const Symbol* end_symbol, const std::vector<std::unique_ptr<Symbol>>& symbols )
{
    SWEET_ASSERT( start_symbol );
    SWEET_ASSERT( end_symbol );
    SWEET_ASSERT( states_.empty() );

    if ( !start_symbol->productions().empty() )
    {
        std::shared_ptr<State> start_state( new State() );
        start_state->add_item( start_symbol->productions().front(), 0 );
        closure( start_state );
        states_.insert( start_state );
        start_state_ = start_state.get();

        set<const Symbol*> lookahead_symbols;
        lookahead_symbols.insert( (Symbol*) end_symbol );
        start_state->add_lookahead_symbols( start_symbol->productions().front(), 0, lookahead_symbols );
        
        int added = 1;
        while ( added > 0 )
        {
            added = 0;
            for ( std::set<std::shared_ptr<State>, shared_ptr_less<State>>::const_iterator i = states_.begin(); i != states_.end(); ++i )
            {
                const std::shared_ptr<State>& state = *i;
                SWEET_ASSERT( state );

                if ( !state->is_processed() )
                {
                    state->set_processed( true );
                    for ( vector<unique_ptr<Symbol>>::const_iterator j = symbols.begin(); j != symbols.end(); ++j )
                    {
                        Symbol* symbol = j->get();
                        SWEET_ASSERT( symbol );
                        if ( symbol != end_symbol )
                        {
                            std::shared_ptr<State> goto_state = goto_( state, *symbol );
                            if ( !goto_state->get_items().empty() )
                            {                    
                                std::shared_ptr<State> actual_goto_state = *states_.insert( goto_state ).first;
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
            for ( std::set<std::shared_ptr<State>, shared_ptr_less<State>>::const_iterator i = states_.begin(); i != states_.end(); ++i )
            {
                State* state = i->get();
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
void Generator::generate_indices_for_states()
{
    int index = 0;
    for ( std::set<std::shared_ptr<State>, shared_ptr_less<State>>::iterator i = states_.begin(); i != states_.end(); ++i )
    {
        State* state = i->get();
        SWEET_ASSERT( state );
        state->set_index( index );
        ++index;
    }
}

/**
// Generate reduction transitions.
*/
void Generator::generate_reduce_transitions()
{
    for ( std::set<std::shared_ptr<State>, shared_ptr_less<State>>::const_iterator i = states_.begin(); i != states_.end(); ++i )
    {
        State* state = i->get();
        SWEET_ASSERT( state );
            
        for ( std::set<Item>::const_iterator item = state->get_items().begin(); item != state->get_items().end(); ++item )
        {
            if ( item->is_dot_at_end() )
            {
                const set<const Symbol*>& symbols = item->get_lookahead_symbols();
                for ( set<const Symbol*>::const_iterator j = symbols.begin(); j != symbols.end(); ++j )
                {
                    const Symbol* symbol = *j;
                    SWEET_ASSERT( symbol );
                    generate_reduce_transition( state, symbol, item->get_production() );
                }
            }                
        }
    }
}

/**
// Generate a reduction transition.
//
// @param state
//  The State that the reduction occurs from.
//
// @param symbol
//  The Symbol that the reduction is to be performed on.
//
// @param production
//  The Production that is to be reduced.
*/
void Generator::generate_reduce_transition( State* state, const Symbol* symbol, const Production* production )
{
    SWEET_ASSERT( state );
    SWEET_ASSERT( symbol );
    SWEET_ASSERT( production );

    Transition* transition = state->find_transition_by_symbol( symbol );
    if ( !transition )
    {
        state->add_transition( symbol, production->symbol(), production->length(), production->precedence(), production->action_index() );
    }
    else
    {
        switch ( transition->get_type() )
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
void Generator::generate_indices_for_transitions()
{
    for ( std::set<std::shared_ptr<State>, shared_ptr_less<State>>::const_iterator i = states_.begin(); i != states_.end(); ++i )
    {
        State* state = i->get();
        SWEET_ASSERT( state );
        state->generate_indices_for_transitions();        
    }
}

void Generator::populate_parser_state_machine( const std::vector<LexerToken>& whitespace_tokens, ParserStateMachine* parser_state_machine, LexerErrorPolicy* lexer_error_policy )
{
    SWEET_ASSERT( parser_state_machine );

    int actions_size = actions_.size();
    unique_ptr<ParserAction[]> actions( new ParserAction [actions_size] );
    for ( int i = 0; i < actions_size; ++i )
    {
        const Action* source_action = actions_[i].get();
        SWEET_ASSERT( source_action );
        ParserAction* action = &actions[i];
        SWEET_ASSERT( action );
        action->reset( source_action->index(), source_action->identifier().c_str() );
    }

    int symbols_size = symbols_.size();
    unique_ptr<ParserSymbol[]> symbols( new ParserSymbol [symbols_size] );
    for ( int i = 0; i < symbols_size; ++i )
    {
        const Symbol* source_symbol = symbols_[i].get();
        SWEET_ASSERT( source_symbol );
        ParserSymbol* symbol = &symbols[i];
        SWEET_ASSERT( symbol );
        symbol->reset( source_symbol->index(), source_symbol->identifier().c_str(), source_symbol->lexeme().c_str(), source_symbol->symbol_type() );
    }

    int states_size = states_.size();
    unique_ptr<ParserState[]> states( new ParserState [states_size] );

    int transitions_size = 0;
    for ( auto i = states_.begin(); i != states_.end(); ++i )
    {
        const State* source_state = i->get();
        SWEET_ASSERT( source_state );
        transitions_size += source_state->get_transitions().size();
    }
    unique_ptr<ParserTransition[]> transitions( new ParserTransition [transitions_size] );

    const ParserState* start_state = nullptr;
    int state_index = 0;
    int transition_index = 0;
    for ( auto i = states_.begin(); i != states_.end(); ++i )
    {
        const State* source_state = i->get();
        SWEET_ASSERT( source_state );
        ParserState* state = &states[state_index];
        SWEET_ASSERT( state );
        const set<Transition>& source_transitions = source_state->get_transitions();
        state->index = state_index;
        state->length = source_transitions.size();
        state->transitions = &transitions[transition_index];
        if ( source_state == start_state_ )
        {
            start_state = state;
        }
        for ( auto j = source_transitions.begin(); j != source_transitions.end(); ++j )
        {
            const Transition* source_transition = &(*j);
            SWEET_ASSERT( source_transition );
            const Symbol* source_symbol = source_transition->get_symbol();
            SWEET_ASSERT( source_symbol );
            const State* state_transitioned_to = source_transition->get_state();
            const Symbol* reduced_symbol = source_transition->reduced_symbol();
            ParserTransition* transition = &transitions[transition_index];
            transition->symbol = &symbols[source_symbol->index()];
            transition->state = state_transitioned_to ? &states[state_transitioned_to->get_index()] : nullptr;
            transition->reduced_symbol = reduced_symbol ? &symbols[reduced_symbol->index()] : nullptr;
            transition->reduced_length = source_transition->reduced_length();
            transition->precedence = source_transition->precedence();
            transition->action = source_transition->action();
            transition->type = source_transition->get_type();
            transition->index = transition_index;
            ++transition_index;
        }
        ++state_index;
    }

    parser_state_machine->set_actions( actions, actions_size );
    parser_state_machine->set_symbols( symbols, symbols_size );
    parser_state_machine->set_transitions( transitions, transitions_size );
    parser_state_machine->set_states( states, states_size, start_state );

    // Generate tokens for generating the lexical analyzer from each of 
    // the terminal symbols in the grammar.
    vector<LexerToken> tokens;
    for ( size_t i = 0; i < symbols_.size(); ++i )
    {
        const Symbol* source_symbol = symbols_[i].get();
        SWEET_ASSERT( source_symbol );
        const ParserSymbol* symbols = parser_state_machine->symbols();
        SWEET_ASSERT( symbols );
        if ( source_symbol->symbol_type() == SYMBOL_TERMINAL )
        {
            const ParserSymbol* symbol = &symbols[i];
            SWEET_ASSERT( symbol );
            int line = source_symbol->line();
            LexerTokenType token_type = source_symbol->lexeme_type() == LEXEME_REGULAR_EXPRESSION ? TOKEN_REGULAR_EXPRESSION : TOKEN_LITERAL;
            tokens.push_back( LexerToken(token_type, line, symbol, symbol->lexeme) );                
        }
    }

    shared_ptr<LexerStateMachine> lexer_state_machine( new LexerStateMachine(identifier_, tokens, whitespace_tokens, lexer_error_policy) );
    parser_state_machine->set_lexer_state_machine( lexer_state_machine );
}
