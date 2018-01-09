
#include <sweet/unit/UnitTest.h>
#include <sweet/cmdline/Splitter.hpp>
#include <sweet/assert/assert.hpp>
#include <string.h>

using namespace sweet::cmdline;

SUITE( TestSplitter )
{
    static void test(const char* command_line, const char **arguments)
    {
        SWEET_ASSERT( command_line );

        Splitter splitter( command_line );
        int i = 0;
        while ( i < int(splitter.arguments().size()) && splitter.arguments()[i] && arguments[i] )        
        {
            CHECK( strcmp(splitter.arguments()[i], arguments[i]) == 0 );
            ++i;
        }
        CHECK( i == int(splitter.arguments().size()) - 1 );
        CHECK( !splitter.arguments()[i] );
        CHECK( !arguments[i] );
    }

    TEST( SpacedDoubleQuotes )
    {        
        const char* COMMAND_LINE = 
            "-a \"foo bar baz\" blah"
        ;
        const char* ARGUMENTS[] =
        {
            "-a",
            "foo bar baz",
            "blah",
            NULL
        };
        test( COMMAND_LINE, ARGUMENTS );
    }
    
    TEST( NonSpacedDoubleQuotes )
    {
        const char* COMMAND_LINE = 
            "-a\"foo bar baz\" blah"
        ;
        const char* ARGUMENTS[] =
        {
            "-afoo bar baz",
            "blah",
            NULL
        };
        test( COMMAND_LINE, ARGUMENTS );
    }
    
    TEST( SpacedSingleQuotes )
    {
        const char* COMMAND_LINE = 
            "-a 'foo bar baz' blah"
        ;
        const char* ARGUMENTS[] =
        {
            "-a",
            "foo bar baz",
            "blah",
            NULL
        };
        test( COMMAND_LINE, ARGUMENTS );
    }
    
    TEST( NonSpacedSingleQuotes )
    {
        const char* COMMAND_LINE = 
            "-a'foo bar baz' blah"
        ;
        const char* ARGUMENTS[] =
        {
            "-afoo bar baz",
            "blah",
            NULL
        };
        test( COMMAND_LINE, ARGUMENTS );
    }

    TEST( GxxCommandLineWithNonSpacedDoubleQuotesAndEscapedDoubleQuotes )
    {    
        const char* COMMAND_LINE = 
            "g++ -I\"/Users/charles/sweet/sweet_build_tool\" -I\"C:/boost/include/boost-1_43\" -DBUILD_PLATFORM_GCC -DBUILD_VARIANT_DEBUG "
            "-DBUILD_LIBRARY_SUFFIX=\"\\\"_gcc_debug.lib\\\"\" -DBUILD_MODULE_ASSERT -DBUILD_LIBRARY_TYPE_STATIC -c -fpermissive "
            "-Wno-deprecated -x c++ -static-libstdc++ -g -fexceptions -frtti "
            "-o/Users/charles/sweet/sweet_build_tool/obj/gcc_debug/assert/stdafx.o stdafx.cpp"
        ;        
        const char* ARGUMENTS[] = 
        {
            "g++",
            "-I/Users/charles/sweet/sweet_build_tool",
            "-IC:/boost/include/boost-1_43",
            "-DBUILD_PLATFORM_GCC",
            "-DBUILD_VARIANT_DEBUG",
            "-DBUILD_LIBRARY_SUFFIX=\"_gcc_debug.lib\"",
            "-DBUILD_MODULE_ASSERT", 
            "-DBUILD_LIBRARY_TYPE_STATIC",
            "-c",
            "-fpermissive",
            "-Wno-deprecated",
            "-x",
            "c++",
            "-static-libstdc++",
            "-g",
            "-fexceptions",
            "-frtti",
            "-o/Users/charles/sweet/sweet_build_tool/obj/gcc_debug/assert/stdafx.o",
            "stdafx.cpp",
            NULL
        };        
        test( COMMAND_LINE, ARGUMENTS );
    }

    TEST( GxxCommandLineWithSpacedDoubleQuotes )
    {
        const char *COMMAND_LINE = 
            "g++ -lbuild_tool_gcc_debug -lcmdline_gcc_debug -llua_gcc_debug -lliblua_gcc_debug "
            "-lprocess_gcc_debug -lrtti_gcc_debug -lthread_gcc_debug -lpersist_gcc_debug "
            "-lpath_gcc_debug -lpointer_gcc_debug -lerror_gcc_debug -lassert_gcc_debug "
            "-llibboost_filesystem.a -llibboost_regex.a -llibboost_system.a -llibboost_thread.a  "
            "stdafx.o Application.o main.o  "
            "-o /Users/charles/sweet/sweet_build_tool/bin/build_gcc_debug "
            "-static-libstdc++ -debug "
            "-L \"/Users/charles/sweet/sweet_build_tool/lib\" "
            "-L \"/Users/charles/boost/boost_1_43_0/stage/lib\""
        ;    
        const char* ARGUMENTS[] =
        {
            "g++",
            "-lbuild_tool_gcc_debug",
            "-lcmdline_gcc_debug",
            "-llua_gcc_debug",
            "-lliblua_gcc_debug",
            "-lprocess_gcc_debug",
            "-lrtti_gcc_debug",
            "-lthread_gcc_debug",
            "-lpersist_gcc_debug",
            "-lpath_gcc_debug",
            "-lpointer_gcc_debug",
            "-lerror_gcc_debug",
            "-lassert_gcc_debug",
            "-llibboost_filesystem.a",
            "-llibboost_regex.a",
            "-llibboost_system.a",
            "-llibboost_thread.a",
            "stdafx.o",
            "Application.o",
            "main.o",
            "-o",
            "/Users/charles/sweet/sweet_build_tool/bin/build_gcc_debug",
            "-static-libstdc++",
            "-debug",
            "-L",
            "/Users/charles/sweet/sweet_build_tool/lib",
            "-L",
            "/Users/charles/boost/boost_1_43_0/stage/lib",
            NULL
        };       
        test( COMMAND_LINE, ARGUMENTS );
    }

    TEST( UnterminatedSingleQuotedString )
    {
        const char* COMMAND_LINE = "'This is an unterminated string";
        const char* ARGUMENTS[] =
        {
            "This is an unterminated string",
            NULL
        };
        test( COMMAND_LINE, ARGUMENTS );
    }

    TEST( UnterminatedDoubleQuotedString )
    {
        const char* COMMAND_LINE = "\"This is an unterminated string";
        const char* ARGUMENTS[] =
        {
            "This is an unterminated string",
            NULL
        };
        test( COMMAND_LINE, ARGUMENTS );
    }
}
