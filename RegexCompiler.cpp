
#include "RegexCompiler.hpp"
#include "LexerStateMachine.hpp"
#include "LexerState.hpp"
#include "LexerTransition.hpp"
#include "LexerAction.hpp"

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
