#ifndef LALR_PARSERNODE_IPP_INCLUDED
#define LALR_PARSERNODE_IPP_INCLUDED

#include "ParserNode.hpp"
#include "assert.hpp"

namespace lalr
{

/**
// Constructor.
//
// @param state
//  The %ParserState at this node.
//
// @param started_productions
//  The productions that were started at this node.
//
// @param symbol
//  The Symbol at this node.
*/
template <class Char, class Traits, class Allocator>
ParserNode<Char, Traits, Allocator>::ParserNode( const ParserState* state, const ParserSymbol* symbol )
: state_( state ),
  symbol_( symbol ),
  lexeme_(),
  line_( 0 )
{
    LALR_ASSERT( state );
}

/**
// Constructor.
//
// @param state
//  The state at this node.
//
// @param started_productions
//  The productions that were started at this node.
//
// @param symbol
//  The symbol at this node.
//
// @param lexeme
//  The lexeme at this node.
//
// @param line
//  The line number at the start of the lexeme (assumed >= 0).
*/
template <class Char, class Traits, class Allocator>
ParserNode<Char, Traits, Allocator>::ParserNode( const ParserState* state, const ParserSymbol* symbol, const std::basic_string<Char, Traits, Allocator>& lexeme, int line )
: state_( state ),
  symbol_( symbol ),
  lexeme_( lexeme ),
  line_( line )
{
    LALR_ASSERT( state );
    LALR_ASSERT( line >= 0 );
}

/**
// Get the state at this node.
//
// @return
//  The state.
*/
template <class Char, class Traits, class Allocator>
const ParserState* ParserNode<Char, Traits, Allocator>::state() const
{
    return state_;
}

/**
// Get the symbol at this state.
//
// @return
//  The symbol.
*/
template <class Char, class Traits, class Allocator>
const ParserSymbol* ParserNode<Char, Traits, Allocator>::symbol() const
{
    return symbol_;
}

/**
// Get the lexeme at this state.
//
// @return
//  The lexeme.
*/
template <class Char, class Traits, class Allocator>
const std::basic_string<Char, Traits, Allocator>& ParserNode<Char, Traits, Allocator>::lexeme() const
{
    return lexeme_;
}

/**
// Get the line number at the start of this node's lexeme.
//
// @return
//  The line number.
*/
template <class Char, class Traits, class Allocator>
int ParserNode<Char, Traits, Allocator>::line() const
{
    return line_;
}

}

#endif
