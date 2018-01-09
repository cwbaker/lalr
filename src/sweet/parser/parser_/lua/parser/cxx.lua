--
-- cxx.lua
-- Copyright (c) Charles Baker. All rights reserved.
--

local function cxx_parser( filename )
    local input_file = assert( io.open(filename, "rb") );
    local grammar = assert( input_file:read("*a") );
    input_file:close();
    input_file = nil;

    local parser_state_machine = ParserStateMachine( grammar, filename );
    assert( parser_state_machine ~= nil, "Parsing "..filename.." failed" );
    
    local hpp_filename = string.gsub( filename, "%.g$", ".hpp" );
    local output_file = assert( io.open(hpp_filename, "wb") );
    local function print( value )
        output_file:write( value );
        output_file:write( "\n" );
    end
    
    local function size( container )
        local size = 0;
        for element in container do
            size = size + 1;
        end
        return size;
    end

    local function address( object, identifier )
        if object ~= nil then
            return "&"..identifier;
        else
            return "NULL";
        end
    end

    local function symbol_id( symbol )
        if symbol ~= nil then
            return parser_state_machine:id().."_SYMBOL_"..symbol:id();
        else
            return parser_state_machine:id().."_SYMBOL_NULL";
        end
    end

    local function lexer_action_id( action )
        if action ~= nil then
            return "&"..parser_state_machine:id().."_lexer_actions["..action:index().."]";
        else
            return "NULL";
        end
    end

    local function parser_action_id( action )
        if action ~= nil then
            return parser_state_machine:id().."_parser_actions + "..action:index();
        else
            return "NULL";
        end
    end

    local function lexer_transitions_id( id, state )
        assert( id ~= nil );
        assert( type(id) == "string" );
        assert( state ~= nil );
        return id.."_state_"..state:index().."_transitions";
    end

    local function lexer_state_id( id, lexer_state )
        assert( id ~= nil );
        assert( type(id) == "string" );
        assert( lexer_state ~= nil );        
        return id.."_state_"..lexer_state:index();
    end

    local function lexer_state_machine_id()
        return parser_state_machine:id().."_lexer_state_machine";
    end
    
    local function lexer_state( id, state )
        local transitions = 0;
        print( "" );
        print( "const sweet::parser::LexerTransition "..lexer_transitions_id(id, state).."[] =" );
        print( "{" );
        for transition in state:transitions() do
            print( "    { "..transition:start()..", "..transition:finish()..", &"..lexer_state_id(id, transition:state())..", "..lexer_action_id(transition:action()).." }, " );
            transitions = transitions + 1;
        end
        print( "    { 0, 0, NULL, NULL }" );
        print( "};" );
        print( "" );

        print( "" );
        print( "const sweet::parser::LexerState "..lexer_state_id(id, state).." =" );
        print( "{" );
        print( "    "..symbol_id(state:symbol()).."," );
        print( "    "..lexer_transitions_id(id, state).."," );
        print( "    "..lexer_transitions_id(id, state).." + "..transitions );
        print( "};" );
        print( "" );
    end

    local function lexer_state_machine( id, lexer_state_machine )
        if lexer_state_machine then
            print( "" );
            for state in lexer_state_machine:states() do
                print( "extern const sweet::parser::LexerState "..lexer_state_id(id, state)..";" );
            end
            print( "" );            
            for state in lexer_state_machine:states() do
                lexer_state( id, state );
            end
            
            print( "" );
            for state in lexer_state_machine:whitespace_states() do
                print( "extern const sweet::parser::LexerState "..lexer_state_id(id, state)..";" );
            end
            print( "" );            
            for state in lexer_state_machine:whitespace_states() do
                lexer_state( id, state );
            end
            
            print( "" );
            print( "const sweet::parser::LexerStateMachine "..id.." = " );
            print( "{" );
            print( "    "..address(lexer_state_machine:start_state(), lexer_state_id(id, lexer_state_machine:start_state()))..", " );
            print( "    "..address(lexer_state_machine:whitespace_start_state(), lexer_state_id(id, lexer_state_machine:whitespace_start_state()))..", " );
            print( "    "..parser_state_machine:id().."_lexer_actions," );
            print( "    "..parser_state_machine:id().."_lexer_actions + "..size(lexer_state_machine:actions()) );
            print( "};" );
            print( "" );
            
        end
    end
    
    local function production_id( production )
        if production ~= nil then
            return parser_state_machine:id().."_parser_production_"..production:index();
        else
            return "NULL";
        end
    end

    local function transition_type( transition )
        assert( transition ~= nil );
        if transition:type() == ParserTransition.TRANSITION_SHIFT then
            return "sweet::parser::TRANSITION_SHIFT";
        else
            return "sweet::parser::TRANSITION_REDUCE";
        end
    end

    local function parser_state_id( state )
        if state ~= nil then
            return parser_state_machine:id().."_parser_state_"..state:index();
        else
            return "NULL";
        end
    end   
    
    print( "" );
    print( "enum "..parser_state_machine:id().."Symbol" );
    print( "{" );
    print( "    "..parser_state_machine:id().."_SYMBOL_NULL," );
    for symbol in parser_state_machine:symbols() do
        print( "    "..parser_state_machine:id().."_SYMBOL_"..symbol:id()..", " );
    end    
    print( "    "..parser_state_machine:id().."_SYMBOL_COUNT" );
    print( "};" );
    print( "" );

    print( "" );
    print( "const sweet::parser::LexerAction "..parser_state_machine:id().."_lexer_actions[] = " );
    print( "{" );
    for action in parser_state_machine:lexer_state_machine():actions() do
        print( "    { "..action:index()..", \""..action:id().."\" }," );
    end
    print( "    { 0, NULL }" );
    print( "};" );
    print( "" );

    lexer_state_machine( lexer_state_machine_id(), parser_state_machine:lexer_state_machine() );

    print( "" );
    print( "const sweet::parser::ParserAction "..parser_state_machine:id().."_parser_actions[] = " );
    print( "{" );
    for action in parser_state_machine:actions() do
        print( "    { "..action:index()..", \""..action:id().."\" }," );
    end
    print( "    { 0, NULL }" );
    print( "};" );
    print( "" );

    for production in parser_state_machine:productions() do        
        print( "" );
        print( "const sweet::parser::ParserProduction "..production_id( production).." =" );
        print( "{" );
        print( "    "..symbol_id( production:symbol())..", " );
        print( "    "..parser_action_id( production:action())..", " );
        print( "    "..production:length() );
        print( "};" );
        print( "" );        
    end

    print( "" );
    for state in parser_state_machine:states() do
        print( "extern const sweet::parser::ParserState "..parser_state_id( state)..";" );
    end
    print( "" );

    for state in parser_state_machine:states() do
        print( "/*" );
        print( state:description() );
        print( "*/" );
    
        print( "" );
        print( "const sweet::parser::ParserTransition "..parser_state_id(state).."_transitions[] = " );
        print( "{" );
        local transitions = 0;
        for transition in state:transitions() do
            local symbol = symbol_id( transition:symbol() );
            local ttype = transition_type( transition );
            local to_state = address( transition:state(), parser_state_id( transition:state()) );
            local reduced_production = address( transition:reduced_production(), production_id( transition:reduced_production()) );
            print( "    { "..symbol..", "..ttype..", "..to_state..", "..reduced_production.." }," );
            transitions = transitions + 1;
        end
        print( "};" );
        print( "" );
        
        print( "" );
        print( "const sweet::parser::ParserState "..parser_state_id( state).." =" );
        print( "{" );
        print( "    "..parser_state_id(state).."_transitions," );
        print( "    "..parser_state_id(state).."_transitions + "..transitions );
        print( "};" );
        print( "" );        
    end

    print( "" );
    print( "sweet::parser::ParserStateMachine "..parser_state_machine:id().."_parser_state_machine = " )
    print( "{" );
    print( "    "..address(parser_state_machine:start_state(), parser_state_id(parser_state_machine:start_state())).."," );
    print( "    "..address(parser_state_machine:lexer_state_machine(), lexer_state_machine_id()).."," );
    print( "    "..symbol_id(parser_state_machine:start_symbol()).."," );
    print( "    "..symbol_id(parser_state_machine:end_symbol()).."," );
    print( "    "..symbol_id(parser_state_machine:error_symbol()).."," );
    print( "    "..parser_state_machine:id().."_parser_actions," );
    print( "    "..parser_state_machine:id().."_parser_actions + "..size(parser_state_machine:actions()).."," );
    print( "};" );
    print( "" );
    
    output_file:close();
    output_file = nil;    

    local cpp_source = [[
#include "parser.hpp"
#include "%s"
]];

    local cpp_filename = string.gsub( filename, "%.g$", ".cpp" );
    local output_file = assert( io.open(cpp_filename, "wb") );
    output_file:write( cpp_source:format(hpp_filename) );
    output_file:close();
    output_file = nil;
end

function parser( filename )
    cxx_parser( filename );
end
