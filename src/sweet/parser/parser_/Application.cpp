//
// Application.cpp
// Copyright (c) 2007 - 2011 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "Application.hpp"
#include <sweet/parser/ParserStateMachine.hpp>
#include <sweet/parser/ParserState.hpp>
#include <sweet/parser/ParserTransition.hpp>
#include <sweet/parser/ParserSymbol.hpp>
#include <sweet/parser/ParserAction.hpp>
#include <sweet/parser/ParserProduction.hpp>
#include <sweet/parser/ParserErrorPolicy.hpp>
#include <sweet/lexer/LexerStateMachine.hpp>
#include <sweet/lexer/LexerState.hpp>
#include <sweet/lexer/LexerTransition.hpp>
#include <sweet/lexer/LexerAction.hpp>
#include <sweet/cmdline/Parser.hpp>
#include <sweet/lua/vector.hpp>
#include <sweet/lua/set.hpp>
#include <sweet/error/Error.hpp>
#include <iostream>

using std::set;
using std::vector;
using std::string;

using namespace sweet;
using namespace sweet::lua;
using namespace sweet::lexer;
using namespace sweet::parser;

SWEET_LUA_TYPE_CONVERSION( ParserStateMachine, LuaByReference );
SWEET_LUA_TYPE_CONVERSION( ParserState, LuaByReference );
SWEET_LUA_TYPE_CONVERSION( ParserTransition, LuaByReference );
SWEET_LUA_TYPE_CONVERSION( ParserSymbol, LuaByReference );
SWEET_LUA_TYPE_CONVERSION( ParserAction, LuaByReference );
SWEET_LUA_TYPE_CONVERSION( ParserProduction, LuaByReference );
SWEET_LUA_TYPE_CONVERSION( LexerStateMachine, LuaByReference );
SWEET_LUA_TYPE_CONVERSION( LexerState, LuaByReference );
SWEET_LUA_TYPE_CONVERSION( LexerTransition, LuaByReference );
SWEET_LUA_TYPE_CONVERSION( LexerAction, LuaByReference );

namespace sweet
{

namespace parser
{

class ParserStateMachineDeleter
{
public:
    Application* application_;

    ParserStateMachineDeleter( Application* application )
    : application_( application )
    {
        SWEET_ASSERT( application_ );            
    }
    
    void operator()( ParserStateMachine* parser_state_machine ) const
    {
        SWEET_ASSERT( parser_state_machine );
        SWEET_ASSERT( application_ );            
        application_->remove_parser_state_machine( parser_state_machine );
        delete parser_state_machine;
        parser_state_machine = NULL;
    }
};

}

}

Application::Application( int argc, char** argv )
: result_( EXIT_SUCCESS ),
  error_policy_(),
  lua_( error_policy_ ),
  parser_state_machine_prototype_( lua_ ),
  parser_state_machine_metatable_( lua_ ),
  parser_state_prototype_( lua_ ),
  parser_state_metatable_( lua_ ),
  parser_transition_prototype_( lua_ ),
  parser_transition_metatable_( lua_ ),
  parser_symbol_prototype_( lua_ ),
  parser_symbol_metatable_( lua_ ),
  parser_action_prototype_( lua_ ),
  parser_action_metatable_( lua_ ),
  parser_production_prototype_( lua_ ),
  parser_production_metatable_( lua_ ),
  lexer_state_machine_prototype_( lua_ ),
  lexer_state_machine_metatable_( lua_ ),
  lexer_state_prototype_( lua_ ),
  lexer_state_metatable_( lua_ ),
  lexer_transition_prototype_( lua_ ),
  lexer_transition_metatable_( lua_ ),
  lexer_action_prototype_( lua_ ),
  lexer_action_metatable_( lua_ )
{
    bool help = false;
    bool version = false;
    string require = "parser/cxx";
    bool stack_trace = false;
    vector<string> assignments_and_filenames;

    cmdline::Parser command_line_parser;
    command_line_parser.add_options()
        ( "help", "h", "Print this message and exit", &help )
        ( "version", "v", "Print the version and exit", &version )
        ( "require", "r", "Set the name of the script to require on startup", &require )
        ( "stack-trace", "s", "Enable stack traces in error messages", &stack_trace )
        ( &assignments_and_filenames )
    ;

    command_line_parser.parse( argc, argv );

    if ( version || help || require.empty() )
    {
        if ( version )
        {
            std::cout << "Sweet Parser " << BUILD_VERSION << " \n";
            std::cout << "Copyright (c) 2007 - 2011 Charles Baker.  All rights reserved. \n";
        }

        if ( help || require.empty() )
        {
            std::cout << "Usage: parser [options] [attribute=value] file ... \n";
            std::cout << "Options: \n";
            command_line_parser.print( stdout );
        }
    }
    else
    {
        lua_.set_stack_trace_enabled( stack_trace );
        prototypes();
        
        vector<string> filenames;
        for ( std::vector<std::string>::const_iterator i = assignments_and_filenames.begin(); i != assignments_and_filenames.end(); ++i )
        {
            std::string::size_type position = i->find( "=" );
            if ( position != std::string::npos )
            {
                std::string attribute = i->substr( 0, position );
                std::string value = i->substr( position + 1, std::string::npos );
                lua_.globals()
                    ( attribute.c_str(), value )
                ;
            }
            else
            {
                filenames.push_back( *i );
            }
        }

        lua_.call( "require" )( require ).end();
        for ( vector<string>::const_iterator i = filenames.begin(); i != filenames.end(); ++i )
        {
            lua_.call( "parser" )( *i ).end();
        }
    }
}

int Application::get_result() const
{
    return result_;
}

std::shared_ptr<ParserStateMachine> Application::parser_state_machine( const std::string& grammar, const std::string& filename )
{
    struct ApplicationParserErrorPolicy : public ParserErrorPolicy
    {
        const char* filename_;
        
        ApplicationParserErrorPolicy( const char* filename )
        : filename_( filename )
        {
            SWEET_ASSERT( filename_ );
        }
    
        void parser_error( int line, const error::Error& error )
        {
            fprintf( stderr, "parser: %s(%d) : %s.\n", filename_, line, error.what() );
        }
    };

    ApplicationParserErrorPolicy event_sink( filename.c_str() );
    std::shared_ptr<ParserStateMachine> parser_state_machine;
    parser_state_machine.reset( new ParserStateMachine(grammar.c_str(), grammar.c_str() + grammar.length(), &event_sink), ParserStateMachineDeleter(this) );
    if ( parser_state_machine->start_state() )
    {
        add_parser_state_machine( parser_state_machine );
    }
    else
    {
        parser_state_machine.reset();
    }
    
    return parser_state_machine;
}

void Application::prototypes()
{
    parser_state_machine_prototype_.members()
        .type( SWEET_STATIC_TYPEID(ParserStateMachine) )
        .metatable( parser_state_machine_metatable_ )
        ( "id", &ParserStateMachine::identifier )
        ( "actions", &ParserStateMachine::actions )
        ( "productions", &ParserStateMachine::productions )
        ( "symbols", &ParserStateMachine::symbols )
        ( "states", &ParserStateMachine::states )
        ( "start_state", &ParserStateMachine::start_state )
        ( "lexer_state_machine", &ParserStateMachine::lexer_state_machine )
        ( "start_symbol", &ParserStateMachine::start_symbol )
        ( "end_symbol", &ParserStateMachine::end_symbol )
        ( "error_symbol", &ParserStateMachine::error_symbol )
        ( "description", &ParserStateMachine::description )
    ;

    parser_state_machine_metatable_.members()
        ( "__index", parser_state_machine_prototype_ )
        ( "__call", weaken(&Application::parser_state_machine), this, lua::_2, lua::_3 )
    ;

    parser_state_prototype_.members()
        .type( SWEET_STATIC_TYPEID(ParserState) )
        .metatable( parser_state_metatable_ )
        ( "index", &ParserState::get_index )
        ( "transitions", &ParserState::get_transitions )        
        ( "description", &ParserState::description )
    ;

    parser_state_metatable_.members()
        ( "__index", parser_state_prototype_ )
    ;

    parser_transition_prototype_.members()
        .type( SWEET_STATIC_TYPEID(ParserTransition) )
        .metatable( parser_transition_metatable_ )
        ( "TRANSITION_SHIFT", static_cast<int>(TRANSITION_SHIFT) )
        ( "TRANSITION_REDUCE", static_cast<int>(TRANSITION_REDUCE) )
        ( "type", &ParserTransition::get_type )
        ( "state", &ParserTransition::get_state )
        ( "reduced_production", &ParserTransition::get_reduced_production )
        ( "is_symbol", &ParserTransition::is_symbol )
        ( "symbol", &ParserTransition::get_symbol )
        ( "index", &ParserTransition::get_index )
    ;

    parser_transition_metatable_.members()
        ( "__index", parser_transition_prototype_ )
    ;

    parser_symbol_prototype_.members()
        .type( SWEET_STATIC_TYPEID(ParserSymbol) )
        .metatable( parser_symbol_metatable_ )
        ( "index", &ParserSymbol::get_index )
        ( "type", &ParserSymbol::get_type )
        ( "lexeme", &ParserSymbol::get_lexeme )
        ( "id", &ParserSymbol::get_identifier )
        ( "is_nullable", &ParserSymbol::is_nullable )
        ( "first", &ParserSymbol::get_first )
        ( "follow", &ParserSymbol::get_follow )
    ;

    parser_symbol_metatable_.members()
        ( "__index", parser_symbol_prototype_ )
    ;

    parser_action_prototype_.members()
        .type( SWEET_STATIC_TYPEID(ParserAction) )
        .metatable( parser_action_metatable_ )
        ( "index", &ParserAction::get_index )
        ( "id", &ParserAction::get_identifier )
    ;
    
    parser_action_metatable_.members()
        ( "__index", parser_action_prototype_ )
    ;
    
    parser_production_prototype_.members()
        .type( SWEET_STATIC_TYPEID(ParserProduction) )
        .metatable( parser_production_metatable_ )
        ( "index", &ParserProduction::get_index )
        ( "symbol", &ParserProduction::get_symbol )
        ( "action", &ParserProduction::get_action )
        ( "length", &ParserProduction::get_length )
    ;
    
    parser_production_metatable_.members()
        ( "__index", parser_production_prototype_ )
    ;

    lexer_state_machine_prototype_.members()
        .type( SWEET_STATIC_TYPEID(LexerStateMachine) )
        .metatable( lexer_state_machine_metatable_ )
        ( "states", &LexerStateMachine::states )
        ( "start_state", &LexerStateMachine::start_state )
        ( "whitespace_states", &LexerStateMachine::whitespace_states )
        ( "whitespace_start_state", &LexerStateMachine::whitespace_start_state )
        ( "actions", &LexerStateMachine::actions )
        ( "description", &LexerStateMachine::description )
    ;

    lexer_state_machine_metatable_.members()
        ( "__index", lexer_state_machine_prototype_ )
    ;

    lexer_state_prototype_.members()
        .type( SWEET_STATIC_TYPEID(LexerState) )
        .metatable( lexer_state_metatable_ )
        ( "symbol", &Application::symbol, this, lua::_1 )
        ( "find_transition_by_character", &LexerState::find_transition_by_character )
        ( "transitions", &LexerState::get_transitions )
        ( "index", &LexerState::get_index )        
    ;

    lexer_state_metatable_.members()
        ( "__index", lexer_state_prototype_ )
    ;

    lexer_transition_prototype_.members()
        .type( SWEET_STATIC_TYPEID(LexerTransition) )
        .metatable( lexer_transition_prototype_ )
        ( "start", &LexerTransition::get_begin )
        ( "finish", &LexerTransition::get_end )
        ( "is_on_character", &LexerTransition::is_on_character )
        ( "state", &LexerTransition::get_state )
        ( "action", &LexerTransition::get_action )
    ;

    lexer_transition_metatable_.members()
        ( "__index", lexer_transition_prototype_ )
    ;
        
    lexer_action_prototype_.members()
        .type( SWEET_STATIC_TYPEID(LexerAction) )
        .metatable( lexer_action_metatable_ )
        ( "index", &LexerAction::get_index )
        ( "id", &LexerAction::get_identifier )
    ;
        
    lexer_action_metatable_.members()
        ( "__index", lexer_action_prototype_ )
    ;
    
    lua_.globals()
        ( "ParserStateMachine", parser_state_machine_prototype_ )
        ( "ParserState", parser_state_prototype_ )
        ( "ParserTransition", parser_transition_prototype_ )
        ( "ParserSymbol", parser_symbol_prototype_ )
        ( "ParserAction", parser_action_prototype_ )
        ( "ParserProduction", parser_production_prototype_ )
        ( "LexerStateMachine", lexer_state_machine_prototype_ )
        ( "LexerState", lexer_state_prototype_ )
        ( "LexerTransition", lexer_transition_prototype_ )
        ( "LexerAction", lexer_action_prototype_ )
    ;
}

void Application::add_parser_state_machine( std::shared_ptr<ParserStateMachine> parser_state_machine )
{
    SWEET_ASSERT( parser_state_machine );
      
    lua_.create( parser_state_machine );
    lua_.members( parser_state_machine )
        .metatable( parser_state_machine_metatable_ )
        .this_pointer( parser_state_machine.get() )
        ( "std::shared_ptr", value(parser_state_machine) )
    ;
    
    const vector<std::shared_ptr<ParserState> >& states = parser_state_machine->states();
    for ( vector<std::shared_ptr<ParserState> >::const_iterator i = states.begin(); i != states.end(); ++i )
    {
        ParserState* state = i->get();
        SWEET_ASSERT( state );
        add_parser_state( state );
    }
    
    const vector<std::shared_ptr<ParserAction> >& actions = parser_state_machine->actions();
    for ( vector<std::shared_ptr<ParserAction> >::const_iterator i = actions.begin(); i != actions.end(); ++i )
    {
        ParserAction* action = i->get();
        SWEET_ASSERT( action );
        add_parser_action( action );
    }
    
    const vector<std::shared_ptr<ParserProduction> >& productions = parser_state_machine->productions();
    for ( vector<std::shared_ptr<ParserProduction> >::const_iterator i = productions.begin(); i != productions.end(); ++i )
    {
        ParserProduction* production = i->get();
        SWEET_ASSERT( production );
        add_parser_production( production );
    }

    const vector<std::shared_ptr<ParserSymbol> >& symbols = parser_state_machine->symbols();
    for ( vector<std::shared_ptr<ParserSymbol> >::const_iterator i = symbols.begin(); i != symbols.end(); ++i )
    {
        ParserSymbol* symbol = i->get();
        SWEET_ASSERT( symbol );
        add_parser_symbol( symbol );
    }
    
    const vector<std::shared_ptr<LexerAction> >& lexer_actions = parser_state_machine->lexer_state_machine()->actions();
    for ( vector<std::shared_ptr<LexerAction> >::const_iterator i = lexer_actions.begin(); i != lexer_actions.end(); ++i )
    {
        LexerAction* lexer_action = i->get();
        SWEET_ASSERT( lexer_action );
        add_lexer_action( lexer_action );
    }
    
    add_lexer_state_machine( parser_state_machine->lexer_state_machine() );
}

void Application::remove_parser_state_machine( ParserStateMachine* parser_state_machine )
{   
    SWEET_ASSERT( parser_state_machine );

    if ( parser_state_machine->start_state() )
    {
        remove_lexer_state_machine( parser_state_machine->lexer_state_machine() );

        const vector<std::shared_ptr<LexerAction> >& lexer_actions = parser_state_machine->lexer_state_machine()->actions();
        for ( vector<std::shared_ptr<LexerAction> >::const_reverse_iterator i = lexer_actions.rbegin(); i != lexer_actions.rend(); ++i )
        {
            LexerAction* lexer_action = i->get();
            SWEET_ASSERT( lexer_action );
            remove_lexer_action( lexer_action );
        }

        const vector<std::shared_ptr<ParserSymbol> >& symbols = parser_state_machine->symbols();
        for ( vector<std::shared_ptr<ParserSymbol> >::const_reverse_iterator i = symbols.rbegin(); i != symbols.rend(); ++i )
        {
            ParserSymbol* symbol = i->get();
            SWEET_ASSERT( symbol );
            remove_parser_symbol( symbol );
        }

        const vector<std::shared_ptr<ParserProduction> >& productions = parser_state_machine->productions();
        for ( vector<std::shared_ptr<ParserProduction> >::const_reverse_iterator i = productions.rbegin(); i != productions.rend(); ++i )
        {
            ParserProduction* production = i->get();
            SWEET_ASSERT( production );
            remove_parser_production( production );
        }
        
        const vector<std::shared_ptr<ParserAction> >& actions = parser_state_machine->actions();
        for ( vector<std::shared_ptr<ParserAction> >::const_reverse_iterator i = actions.rbegin(); i != actions.rend(); ++i )
        {
            ParserAction* action = i->get();
            SWEET_ASSERT( action );
            remove_parser_action( action );
        }
        
        const vector<std::shared_ptr<ParserState> >& states = parser_state_machine->states();
        for ( vector<std::shared_ptr<ParserState> >::const_reverse_iterator i = states.rbegin(); i != states.rend(); ++i )
        {
            ParserState* state = i->get();
            SWEET_ASSERT( state );
            remove_parser_state( state );
        }

        lua_.destroy( parser_state_machine );
    }
}

void Application::add_parser_state( ParserState* state )
{
    SWEET_ASSERT( state );
    
    lua_.create( state );
    lua_.members( state )
        .metatable( parser_state_metatable_ )
        .this_pointer( state );
    ;
    
    const std::set<ParserTransition>& transitions = state->get_transitions();
    for ( std::set<ParserTransition>::const_iterator i = transitions.begin(); i != transitions.end(); ++i )
    {
        ParserTransition* transition = const_cast<ParserTransition*>( &(*i) );
        SWEET_ASSERT( transition );
        add_parser_transition( transition );        
    }
}

void Application::remove_parser_state( ParserState* state )
{
    SWEET_ASSERT( state );

    const std::set<ParserTransition>& transitions = state->get_transitions();
    for ( std::set<ParserTransition>::const_reverse_iterator i = transitions.rbegin(); i != transitions.rend(); ++i )
    {
        ParserTransition* transition = const_cast<ParserTransition*>( &(*i) );
        SWEET_ASSERT( transition );
        remove_parser_transition( transition );
    }

    lua_.destroy( state );
}

void Application::add_parser_transition( ParserTransition* transition )
{
    SWEET_ASSERT( transition );

    lua_.create( transition );        
    lua_.members( transition )
        .metatable( parser_transition_metatable_ )
        .this_pointer( transition )
    ;
}

void Application::remove_parser_transition( ParserTransition* transition )
{
    SWEET_ASSERT( transition );
    lua_.destroy( transition );
}

void Application::add_parser_symbol( ParserSymbol* symbol )
{
    SWEET_ASSERT( symbol );
    lua_.create( symbol );
    lua_.members( symbol )
        .metatable( parser_symbol_metatable_ )
        .this_pointer( symbol )
    ;
}

void Application::remove_parser_symbol( ParserSymbol* symbol )
{
    SWEET_ASSERT( symbol );
    lua_.destroy( symbol );
}
       
void Application::add_parser_action( ParserAction* action )
{
    SWEET_ASSERT( action );
    lua_.create( action );
    lua_.members( action )
        .metatable( parser_action_metatable_ )
        .this_pointer( action )
    ;
}

void Application::remove_parser_action( ParserAction* action )
{
    SWEET_ASSERT( action );
    lua_.destroy( action );
}

void Application::add_parser_production( ParserProduction* production )
{
    SWEET_ASSERT( production );
    lua_.create( production );
    lua_.members( production )
        .metatable( parser_production_metatable_ )
        .this_pointer( production )
    ;
}

void Application::remove_parser_production( ParserProduction* production )
{
    SWEET_ASSERT( production );
    lua_.destroy( production );
}

void Application::add_lexer_state_machine( const LexerStateMachine* lexer_state_machine )
{
    if ( lexer_state_machine )
    {
        lua_.create( lexer_state_machine );
        lua_.members( lexer_state_machine )
            .metatable( lexer_state_machine_metatable_ )
            .this_pointer( const_cast<LexerStateMachine*>(lexer_state_machine) )
        ;
        
        const vector<std::shared_ptr<LexerState> >& states = lexer_state_machine->states();
        for ( vector<std::shared_ptr<LexerState> >::const_iterator i = states.begin(); i != states.end(); ++i )
        {
            LexerState* state = i->get();
            SWEET_ASSERT( state );
            add_lexer_state( state );
        }
        
        const vector<std::shared_ptr<LexerState> >& whitespace_states = lexer_state_machine->whitespace_states();
        for ( vector<std::shared_ptr<LexerState> >::const_iterator i = whitespace_states.begin(); i != whitespace_states.end(); ++i )
        {
            LexerState* state = i->get();
            SWEET_ASSERT( state );
            add_lexer_state( state );
        }
    }
}

void Application::remove_lexer_state_machine( const LexerStateMachine* lexer_state_machine )
{
    if ( lexer_state_machine )
    {
        const vector<std::shared_ptr<LexerState> >& states = lexer_state_machine->states();
        for ( vector<std::shared_ptr<LexerState> >::const_reverse_iterator i = states.rbegin(); i != states.rend(); ++i )
        {
            LexerState* state = i->get();
            SWEET_ASSERT( state );
            remove_lexer_state( state );
        }

        const vector<std::shared_ptr<LexerState> >& whitespace_states = lexer_state_machine->whitespace_states();
        for ( vector<std::shared_ptr<LexerState> >::const_reverse_iterator i = whitespace_states.rbegin(); i != whitespace_states.rend(); ++i )
        {
            LexerState* state = i->get();
            SWEET_ASSERT( state );
            remove_lexer_state( state );
        }

        lua_.destroy( lexer_state_machine );
    }
}

void Application::add_lexer_state( LexerState* state )
{
    SWEET_ASSERT( state );
    
    lua_.create( state );
    lua_.members( state )
        .metatable( lexer_state_metatable_ )
        .this_pointer( state )
    ;
    
    const std::set<LexerTransition>& transitions = state->get_transitions();
    for ( std::set<LexerTransition>::const_iterator i = transitions.begin(); i != transitions.end(); ++i )
    {
        LexerTransition* transition = const_cast<LexerTransition*>( &(*i) );
        SWEET_ASSERT( transition );
        add_lexer_transition( transition );
    }
}

void Application::remove_lexer_state( LexerState* state )
{
    SWEET_ASSERT( state );
    
    const std::set<LexerTransition>& transitions = state->get_transitions();
    for ( std::set<LexerTransition>::const_reverse_iterator i = transitions.rbegin(); i != transitions.rend(); ++i )
    {
        LexerTransition* transition = const_cast<LexerTransition*>( &(*i) );
        SWEET_ASSERT( transition );
        remove_lexer_transition( transition );
    }

    lua_.destroy( state );
}

void Application::add_lexer_transition( LexerTransition* transition )
{
    SWEET_ASSERT( transition );
    lua_.create( transition );
    lua_.members( transition )
        .metatable( lexer_transition_metatable_ )
        .this_pointer( transition )
    ;
}

void Application::remove_lexer_transition( LexerTransition* transition )
{
    SWEET_ASSERT( transition );
    lua_.destroy( transition );
}

void Application::add_lexer_action( LexerAction* action )
{
    SWEET_ASSERT( action );    
    lua_.create( action );
    lua_.members( action )
        .metatable( lexer_action_metatable_ )
        .this_pointer( action )
    ;
}

void Application::remove_lexer_action( LexerAction* action )
{
    SWEET_ASSERT( action );
    lua_.destroy( action );
}

ParserSymbol* Application::symbol( lexer::LexerState* state ) const
{
    SWEET_ASSERT( state );
    return state ? reinterpret_cast<ParserSymbol*>(const_cast<void*>(state->get_symbol())) : NULL;
}
