//
// TestParser.cpp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#include <sweet/unit/UnitTest.h>
#include <sweet/cmdline/cmdline.hpp>

using namespace sweet::cmdline;

SUITE( TestParser )
{
    TEST( TestBoolShort )
    {
        bool boolean = false;

        int   argc = 2;
        char* argv[] = { "", "-b" };

        Parser command_line_parser;
        command_line_parser.add_options()
            ( "boolean", "b", "Test a boolean option", &boolean )
        ;
        command_line_parser.parse( argc, argv );  

        CHECK( boolean );      
    }

    TEST( TestBoolLong )
    {
        bool boolean = false;

        int    argc = 2;
        char* argv[] = { "", "--boolean" };

        Parser command_line_parser;
        command_line_parser.add_options()
            ( "boolean", "b", "Test a boolean option", &boolean )
        ;
        command_line_parser.parse( argc, argv );        

        CHECK( boolean );      
    }

    TEST( TestIntShort )
    {
        int integer = 0;

        int    argc = 2;
        char* argv[] = { "", "-i1" };

        Parser command_line_parser;
        command_line_parser.add_options()
            ( "integer", "i", "Test an integer option", &integer )
        ;
        command_line_parser.parse( argc, argv );  

        CHECK( integer == 1 );      
    }

    TEST( TestIntLong )
    {
        int integer = 0;

        int    argc = 2;
        char* argv[] = { "", "--integer=1" };

        Parser command_line_parser;
        command_line_parser.add_options()
            ( "integer", "i", "Test an integer option", &integer )
        ;
        command_line_parser.parse( argc, argv );  

        CHECK( integer == 1 );      
    }

    TEST( TestFloatShort )
    {
        float real = 0.0f;

        int    argc = 2;
        char* argv[] = { "", "-r1.0" };

        Parser command_line_parser;
        command_line_parser.add_options()
            ( "real", "r", "Test a floating point option", &real )
        ;
        command_line_parser.parse( argc, argv );  

        CHECK( real == 1.0f );      
    }

    TEST( TestFloatLong )
    {
        float real = 0.0f;

        int    argc = 2;
        char* argv[] = { "", "--real=1.0" };

        Parser command_line_parser;
        command_line_parser.add_options()
            ( "real", "r", "Test a floating point option", &real )
        ;
        command_line_parser.parse( argc, argv );  

        CHECK( real == 1.0f );      
    }

    TEST( TestStringShort )
    {
        std::string string;

        int    argc = 2;
        char* argv[] = { "", "-sfoo" };

        Parser command_line_parser;
        command_line_parser.add_options()
            ( "string", "s", "Test a string option", &string )
        ;
        command_line_parser.parse( argc, argv );  

        CHECK( string == "foo" );      
    }

    TEST( TestStringLong )
    {
        std::string string;

        int    argc = 2;
        char* argv[] = { "", "--string=foo" };

        Parser command_line_parser;
        command_line_parser.add_options()
            ( "string", "s", "Test a string option", &string )
        ;
        command_line_parser.parse( argc, argv );  

        CHECK( string == "foo" );      
    }
}
