//
// ParserAllocations.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "ParserAllocations.hpp"
#include "ParserStateMachine.hpp"
#include "ParserSymbol.hpp"
#include "ParserState.hpp"
#include "ParserAction.hpp"
#include "ParserTransition.hpp"
#include "LexerStateMachine.hpp"
#include "assert.hpp"

using std::vector;
using std::copy;
using std::back_inserter;
using std::unique_ptr;
using std::shared_ptr;
using namespace sweet;
using namespace sweet::lalr;

ParserAllocations::ParserAllocations()
: strings_(),
  actions_(),
  symbols_(),
  transitions_(),
  states_(),
  lexer_state_machine_(),
  whitespace_lexer_state_machine_(),
  parser_state_machine_()
{
    lexer_state_machine_.reset( new LexerStateMachine );
    whitespace_lexer_state_machine_.reset( new LexerStateMachine );
    parser_state_machine_.reset( new ParserStateMachine );
    memset( parser_state_machine_.get(), 0, sizeof(*parser_state_machine_) );
}

ParserAllocations::~ParserAllocations()
{
}

const LexerStateMachine* ParserAllocations::lexer_state_machine() const
{
    return lexer_state_machine_.get();
}

const LexerStateMachine* ParserAllocations::whitespace_lexer_state_machine() const
{
    return whitespace_lexer_state_machine_.get();
}

const ParserStateMachine* ParserAllocations::parser_state_machine() const
{
    return parser_state_machine_.get();
}

const char* ParserAllocations::add_string( const std::string& string )
{
    strings_.push_back( string );
    return strings_.back().c_str();
}

void ParserAllocations::set_actions( std::unique_ptr<ParserAction[]>& actions, int actions_size )
{
    SWEET_ASSERT( parser_state_machine_ );
    SWEET_ASSERT( actions || actions_size == 0 );
    SWEET_ASSERT( actions_size >= 0 );
    actions_ = move( actions );
    parser_state_machine_->actions_size = actions_size;
    parser_state_machine_->actions = actions_.get();
}

void ParserAllocations::set_symbols( std::unique_ptr<ParserSymbol[]>& symbols, int symbols_size )
{
    SWEET_ASSERT( parser_state_machine_ );
    SWEET_ASSERT( symbols );
    SWEET_ASSERT( symbols_size >= 3 );
    symbols_ = move( symbols );
    parser_state_machine_->symbols_size = symbols_size;
    parser_state_machine_->symbols = symbols_.get();
    parser_state_machine_->start_symbol = &symbols_[0];
    parser_state_machine_->end_symbol = &symbols_[1];
    parser_state_machine_->error_symbol = &symbols_[2];
} 

void ParserAllocations::set_transitions( std::unique_ptr<ParserTransition[]>& transitions, int transitions_size )
{
    SWEET_ASSERT( transitions );
    SWEET_ASSERT( transitions_size >= 0 );
    transitions_ = move( transitions );
    parser_state_machine_->transitions_size = transitions_size;
    parser_state_machine_->transitions = transitions_.get();
}

void ParserAllocations::set_states( std::unique_ptr<ParserState[]>& states, int states_size, const ParserState* start_state )
{
    SWEET_ASSERT( states );
    SWEET_ASSERT( states_size >= 0 );
    SWEET_ASSERT( start_state );
    states_ = move( states );
    parser_state_machine_->states_size = states_size;
    parser_state_machine_->states = states_.get();
    parser_state_machine_->start_state = start_state;
}

void ParserAllocations::set_lexer_state_machine( std::unique_ptr<LexerStateMachine>& lexer_state_machine )
{
    SWEET_ASSERT( lexer_state_machine.get() );
    if ( lexer_state_machine )
    {
        lexer_state_machine_ = move( lexer_state_machine );
        parser_state_machine_->lexer_state_machine = lexer_state_machine_.get();
    }
}

void ParserAllocations::set_whitespace_lexer_state_machine( std::unique_ptr<LexerStateMachine>& whitespace_lexer_state_machine )
{
    if ( whitespace_lexer_state_machine )
    {
        whitespace_lexer_state_machine_ = move( whitespace_lexer_state_machine );
        parser_state_machine_->whitespace_lexer_state_machine = whitespace_lexer_state_machine_.get();
    }
}
