//
// ParserStateMachine.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "ParserStateMachine.hpp"
#include "ParserSymbol.hpp"
#include "assert.hpp"
#include <string.h>

using namespace sweet::lalr;

/**
// Find the symbol whose identifier matches \e identifier.
//
// @param identifier
//  The identifier of the ParserSymbol to find (assumed not null).
//
// @return
//  The `ParserSymbol` with matching identifier or null if there was no 
//  matching `ParserSymbol`.
*/
const ParserSymbol* ParserStateMachine::find_symbol_by_identifier( const char* identifier ) const
{
    SWEET_ASSERT( identifier );
    const ParserSymbol* symbol = symbols;
    const ParserSymbol* symbols_end = symbols + symbols_size;
    while ( symbol != symbols_end && strcmp(symbol->identifier, identifier) != 0 )
    {
        ++symbol;
    }    
    return symbol != symbols_end ? symbol : nullptr;
}
