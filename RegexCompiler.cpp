
#include "RegexCompiler.hpp"
#include "RegexGenerator.hpp"
#include "RegexState.hpp"
#include "RegexTransition.hpp"
#include "RegexAction.hpp"
#include "LexerStateMachine.hpp"
#include "LexerState.hpp"
#include "LexerTransition.hpp"
#include "LexerAction.hpp"
#include "assert.hpp"

using std::set;
using std::vector;
using std::unique_ptr;
using namespace sweet::lalr;

RegexCompiler::RegexCompiler()
: strings_(),
  actions_(),
  transitions_(),
  states_(),
  state_machine_() 
{
    state_machine_.reset( new LexerStateMachine );
    memset( state_machine_.get(), 0, sizeof(*state_machine_) );
}

RegexCompiler::~RegexCompiler()
{
}

const LexerStateMachine* RegexCompiler::state_machine() const
{
    return state_machine_.get();
}

void RegexCompiler::compile( const std::string& regular_expression, void* symbol, ErrorPolicy* error_policy )
{
    RegexGenerator generator;
    int errors = generator.generate( regular_expression, symbol, error_policy );
    if ( errors == 0 )
    {
        populate_lexer_state_machine( generator );
    }
}

void RegexCompiler::compile( const std::vector<RegexToken>& tokens, ErrorPolicy* error_policy )
{
    RegexGenerator generator;
    int errors = generator.generate( tokens, error_policy );
    if ( errors == 0 )
    {
        populate_lexer_state_machine( generator );
    }
}

const char* RegexCompiler::add_string( const std::string& string )
{
    strings_.push_back( string );
    return strings_.back().c_str();
}

void RegexCompiler::set_actions( std::unique_ptr<LexerAction[]>& actions, int actions_size )
{
    actions_ = move( actions );
    state_machine_->actions_size = actions_size;
    state_machine_->actions = actions_.get();
}

void RegexCompiler::set_transitions( std::unique_ptr<LexerTransition[]>& transitions, int transitions_size )
{
    transitions_ = move( transitions );
    state_machine_->transitions_size = transitions_size;
    state_machine_->transitions = transitions_.get();
}

void RegexCompiler::set_states( std::unique_ptr<LexerState[]>& states, int states_size, const LexerState* start_state )
{
    states_ = move( states );
    state_machine_->states_size = states_size;
    state_machine_->states = states_.get();
    state_machine_->start_state = start_state;
}

void RegexCompiler::populate_lexer_state_machine( const RegexGenerator& generator )
{
    const vector<unique_ptr<RegexAction>>& source_actions = generator.actions();
    const set<unique_ptr<RegexState>, RegexStateLess>& source_states = generator.states();

    size_t transitions_size = 0;
    for ( auto i = source_states.begin(); i != source_states.end(); ++i )
    {
        const RegexState* source_state = i->get();
        SWEET_ASSERT( source_state );
        transitions_size += source_state->get_transitions().size();
    }

    unique_ptr<LexerAction[]> actions( new LexerAction [source_actions.size()] );
    unique_ptr<LexerTransition[]> transitions( new LexerTransition [transitions_size] );
    unique_ptr<LexerState[]> states( new LexerState [source_states.size()] );

    for ( size_t i = 0; i < source_actions.size(); ++i )
    {
        const RegexAction* source_action = source_actions[i].get();
        SWEET_ASSERT( source_action );
        LexerAction* action = &actions[i];
        action->index = source_action->index();
        action->identifier = add_string( source_action->identifier() );
    }

    int state_index = 0;
    int transition_index = 0;
    const LexerState* start_state = nullptr;
    for ( auto i = source_states.begin(); i != source_states.end(); ++i )
    {
        const RegexState* source_state = i->get();
        SWEET_ASSERT( source_state );
        LexerState* state = &states[state_index];
        SWEET_ASSERT( state );
        const set<RegexTransition>& source_transitions = source_state->get_transitions();
        state->index = state_index;
        state->length = source_transitions.size();
        state->transitions = &transitions[transition_index];
        state->symbol = source_state->get_symbol();
        if ( source_state == generator.start_state() )
        {
            start_state = state;
        }
        for ( auto j = source_transitions.begin(); j != source_transitions.end(); ++j )
        {
            const RegexTransition* source_transition = &(*j);
            SWEET_ASSERT( source_transition );
            const RegexState* state_transitioned_to = source_transition->state();
            const RegexAction* action = source_transition->action();
            LexerTransition* transition = &transitions[transition_index];
            transition->begin = source_transition->begin();
            transition->end = source_transition->end();
            transition->state = state_transitioned_to ? &states[state_transitioned_to->get_index()] : nullptr;
            transition->action = action ? &actions[action->index()] : nullptr;
            ++transition_index;
        }
        ++state_index;
    }

    set_actions( actions, int(source_actions.size()) );
    set_transitions( transitions, transitions_size );
    set_states( states, int(source_states.size()), start_state );
}
