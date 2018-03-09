//
// LexerStateMachine.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "LexerStateMachine.hpp"
#include "LexerState.hpp"
#include "LexerTransition.hpp"
#include "LexerAction.hpp"
#include "assert.hpp"
#include <algorithm>

using std::move;
using std::unique_ptr;
using namespace sweet;
using namespace sweet::lalr;

LexerStateMachine::LexerStateMachine()
: allocated_actions_(),
  allocated_transitions_(),
  allocated_states_(),
  actions_size_( 0 ),
  transitions_size_( 0 ),
  states_size_( 0 ),
  actions_( nullptr ),
  transitions_( nullptr ),
  states_( nullptr ),
  start_state_( nullptr )
{
}

LexerStateMachine::~LexerStateMachine()
{
}

int LexerStateMachine::actions_size() const
{
    return actions_size_;
}

int LexerStateMachine::transitions_size() const
{
    return transitions_size_;
}

int LexerStateMachine::states_size() const
{
    return states_size_;
}

const LexerAction* LexerStateMachine::actions() const
{
    return actions_;
}

const LexerTransition* LexerStateMachine::transitions() const
{
    return transitions_;
}

const LexerState* LexerStateMachine::states() const
{
    return states_;
}

const LexerState* LexerStateMachine::start_state() const
{
    return start_state_;
}

void LexerStateMachine::set_actions( std::unique_ptr<LexerAction[]>& actions, int actions_size )
{
    SWEET_ASSERT( actions || actions_size == 0 );
    SWEET_ASSERT( actions_size >= 0 );
    allocated_actions_ = move( actions );
    actions_size_ = actions_size;
    actions_ = allocated_actions_.get();
}

void LexerStateMachine::set_transitions( std::unique_ptr<LexerTransition[]>& transitions, int transitions_size )
{
    SWEET_ASSERT( transitions );
    SWEET_ASSERT( transitions_size >= 0 );
    allocated_transitions_ = move( transitions );
    transitions_size_ = transitions_size;
    transitions_ = allocated_transitions_.get();
}

void LexerStateMachine::set_states( std::unique_ptr<LexerState[]>& states, int states_size, const LexerState* start_state )
{
    SWEET_ASSERT( states || states_size == 0 );
    SWEET_ASSERT( states_size >= 0 );
    SWEET_ASSERT( start_state || states_size == 0 );
    allocated_states_ = move( states );
    states_size_ = states_size;
    states_ = allocated_states_.get();
    start_state_ = start_state;
}
