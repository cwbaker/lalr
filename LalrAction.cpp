//
// LalrAction.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#define __STDC_WANT_LIB_EXT2__ 1
#include "LalrAction.hpp"
#include "assert.hpp"
#include <string.h>

using namespace sweet::lalr;

/**
// Constructor.
//
// @param index
//  The index of this action (assumed >= 0).
//
// @param identifier
//  The identifier of this action (assumed not null).
*/
LalrAction::LalrAction( int iindex, const char* iidentifier )
: index( iindex ),
  identifier( nullptr )
{
    SWEET_ASSERT( index >= 0 );
    SWEET_ASSERT( iidentifier );
    identifier = strdup( iidentifier );
}

/**
// Free memory allocated by this `LalrAction`.
//
// Memory is freed selectively in `LalrAction::destroy()` rather than in
// `~LalrAction` because `LalrAction` objects may also be defined as 
// static data generated by the parser generator `lalrc`.  `LalrAction` 
// objects that are created at run-time are explicitly destroyed by their
// owning `LalrGrammar`, `LalrGenerator`, or `ParserStateMachine`.
*/
void LalrAction::destroy()
{
    if ( identifier )
    {
        free( (void*) identifier );
        identifier = nullptr;
    }
}