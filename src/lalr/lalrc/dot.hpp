#pragma once

namespace lalr
{

class ParserStateMachine;
class LexerStateMachine;

void write_graphviz_dot( const ParserStateMachine* state_machine );

}
