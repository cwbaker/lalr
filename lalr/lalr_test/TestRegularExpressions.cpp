//
// TestRegularExpressions.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include <UnitTest++/UnitTest++.h>
#include <lalr/RegexCompiler.hpp>
#include <lalr/Lexer.ipp>
#include <lalr/PositionIterator.hpp>
#include <string.h>

using std::string;
using namespace lalr;

SUITE( RegularExpressions )
{
    TEST( Concatenation )
    {
        void* ab;
        RegexCompiler compiler;
        compiler.compile( "ab", &ab );
        Lexer<const char*> lexer( compiler.state_machine() );
        
        const char* regex = "ab";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &ab );
        CHECK( lexer.lexeme() == "ab" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "a";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "ac";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    

    TEST( Or )
    {
        void* a_or_b;
        RegexCompiler compiler;
        compiler.compile( "a|b", &a_or_b );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "a";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &a_or_b );
        CHECK( lexer.lexeme() == "a" );
        
        regex = "b";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &a_or_b );
        CHECK( lexer.lexeme() == "b" );
        
        regex = "c";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    
    
    TEST( Star )
    {
        void* ab_star;
        RegexCompiler compiler;
        compiler.compile( "ab*", &ab_star );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "a";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &ab_star );
        CHECK( lexer.lexeme() == "a" );
        
        regex = "ab";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &ab_star );
        CHECK( lexer.lexeme() == "ab" );
        
        regex = "abbb";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &ab_star );
        CHECK( lexer.lexeme() == "abbb" );
        
        regex = "b";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "c";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }    
    

    TEST( Plus )
    {
        void* ab_plus;
        RegexCompiler compiler;
        compiler.compile( "ab+", &ab_plus );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "ab";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &ab_plus );
        CHECK( lexer.lexeme() == "ab" );
        
        regex = "abbb";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &ab_plus );
        CHECK( lexer.lexeme() == "abbb" );
        
        regex = "a";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "b";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "c";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }    
    
    
    TEST( Optional )
    {
        void* ab_question;
        RegexCompiler compiler;
        compiler.compile( "ab?", &ab_question );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "a";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &ab_question );
        CHECK( lexer.lexeme() == "a" );

        regex = "ab";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &ab_question );
        CHECK( lexer.lexeme() == "ab" );

        regex = "c";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    
    
    TEST( Dot )
    {
        void* dot;
        RegexCompiler compiler;
        compiler.compile( ".", &dot );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "abcdef";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &dot );
        CHECK( lexer.lexeme() == "a" );
        lexer.advance();
        CHECK( lexer.symbol() == &dot );
        CHECK( lexer.lexeme() == "b" );
        lexer.advance();
        CHECK( lexer.symbol() == &dot );
        CHECK( lexer.lexeme() == "c" );
        lexer.advance();
        CHECK( lexer.symbol() == &dot );
        CHECK( lexer.lexeme() == "d" );
        lexer.advance();
        CHECK( lexer.symbol() == &dot );
        CHECK( lexer.lexeme() == "e" );
        lexer.advance();
        CHECK( lexer.symbol() == &dot );
        CHECK( lexer.lexeme() == "f" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    
    
    TEST( BracketExpression )
    {
        void* abc;
        RegexCompiler compiler;
        compiler.compile( "[abc]", &abc );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "a";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &abc );
        CHECK( lexer.lexeme() == "a" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "b";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &abc );
        CHECK( lexer.lexeme() == "b" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "c";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &abc );
        CHECK( lexer.lexeme() == "c" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "d";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    
    
    TEST( BracketExpressionMinusAtStart )
    {
        void* minus_abc;
        RegexCompiler compiler;
        compiler.compile( "[-abc]", &minus_abc );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "a";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &minus_abc );
        CHECK( lexer.lexeme() == "a" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "b";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &minus_abc );
        CHECK( lexer.lexeme() == "b" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "c";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &minus_abc );
        CHECK( lexer.lexeme() == "c" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "-";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &minus_abc );
        CHECK( lexer.lexeme() == "-" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "d";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    
    
    TEST( BracketExpressionMinusAtEnd )
    {
        void* abc_minus;
        RegexCompiler compiler;
        compiler.compile( "[abc-]", &abc_minus );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "a";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &abc_minus );
        CHECK( lexer.lexeme() == "a" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "b";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &abc_minus );
        CHECK( lexer.lexeme() == "b" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "c";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &abc_minus );
        CHECK( lexer.lexeme() == "c" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "-";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &abc_minus );
        CHECK( lexer.lexeme() == "-" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "d";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    
    
    TEST( NegativeBracketExpression )
    {
        void* hat_abc;
        RegexCompiler compiler;
        compiler.compile( "[^abc]", &hat_abc );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "d";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hat_abc );
        CHECK( lexer.lexeme() == "d" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "e";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hat_abc );
        CHECK( lexer.lexeme() == "e" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "f";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hat_abc );
        CHECK( lexer.lexeme() == "f" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "a";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "b";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "c";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }


    TEST( NegativeBracketExpressionMinusAtStart )
    {
        void* hat_minus_abc;
        RegexCompiler compiler;
        compiler.compile( "[^-abc]", &hat_minus_abc );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "d";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hat_minus_abc );
        CHECK( lexer.lexeme() == "d" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "e";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hat_minus_abc );
        CHECK( lexer.lexeme() == "e" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "f";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hat_minus_abc );
        CHECK( lexer.lexeme() == "f" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "-";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "a";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "b";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "c";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }


    TEST( NegativeBracketExpressionMinusAtEnd )
    {
        void* hat_abc_minus;
        RegexCompiler compiler;
        compiler.compile( "[^abc-]", &hat_abc_minus );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "d";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hat_abc_minus );
        CHECK( lexer.lexeme() == "d" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "e";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hat_abc_minus );
        CHECK( lexer.lexeme() == "e" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "f";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hat_abc_minus );
        CHECK( lexer.lexeme() == "f" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "-";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "a";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "b";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "c";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }


    TEST( Range )
    {
        void* a_minus_c;
        RegexCompiler compiler;
        compiler.compile( "[a-c]", &a_minus_c );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "a";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &a_minus_c );
        CHECK( lexer.lexeme() == "a" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "b";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &a_minus_c );
        CHECK( lexer.lexeme() == "b" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "c";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &a_minus_c );
        CHECK( lexer.lexeme() == "c" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "d";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    
    
    TEST( RangeMinusAtStart )
    {
        void* minus_a_minus_c;
        RegexCompiler compiler;
        compiler.compile( "[-a-c]", &minus_a_minus_c );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "a";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &minus_a_minus_c );
        CHECK( lexer.lexeme() == "a" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "b";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &minus_a_minus_c );
        CHECK( lexer.lexeme() == "b" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "c";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &minus_a_minus_c );
        CHECK( lexer.lexeme() == "c" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "-";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &minus_a_minus_c );
        CHECK( lexer.lexeme() == "-" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "d";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    
    
    TEST( RangeMinusAtEnd )
    {
        void* a_minus_c_minus;
        RegexCompiler compiler;
        compiler.compile( "[a-c-]", &a_minus_c_minus );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "a";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &a_minus_c_minus );
        CHECK( lexer.lexeme() == "a" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "b";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &a_minus_c_minus );
        CHECK( lexer.lexeme() == "b" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "c";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &a_minus_c_minus );
        CHECK( lexer.lexeme() == "c" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "-";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &a_minus_c_minus );
        CHECK( lexer.lexeme() == "-" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "d";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    
    
    TEST( NegativeRange )
    {
        void* hat_a_minus_c;
        RegexCompiler compiler;
        compiler.compile( "[^a-c]", &hat_a_minus_c );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "d";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hat_a_minus_c );
        CHECK( lexer.lexeme() == "d" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "e";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hat_a_minus_c );
        CHECK( lexer.lexeme() == "e" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "f";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hat_a_minus_c );
        CHECK( lexer.lexeme() == "f" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "a";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "b";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "c";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }


    TEST( NegativeRangeMinusAtStart )
    {
        void* hat_minus_a_minus_c;
        RegexCompiler compiler;
        compiler.compile( "[^-a-c]", &hat_minus_a_minus_c );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "d";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hat_minus_a_minus_c );
        CHECK( lexer.lexeme() == "d" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "e";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hat_minus_a_minus_c );
        CHECK( lexer.lexeme() == "e" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "f";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hat_minus_a_minus_c );
        CHECK( lexer.lexeme() == "f" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "-";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "a";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "b";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "c";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }


    TEST( NegativeRangeMinusAtEnd )
    {
        void* hat_a_minus_c_minus;
        RegexCompiler compiler;
        compiler.compile( "[^a-c-]", &hat_a_minus_c_minus );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "d";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hat_a_minus_c_minus );
        CHECK( lexer.lexeme() == "d" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "e";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hat_a_minus_c_minus );
        CHECK( lexer.lexeme() == "e" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "f";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hat_a_minus_c_minus );
        CHECK( lexer.lexeme() == "f" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "-";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "a";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "b";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "c";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    
    
    TEST( EscapeCharacters )
    {
        void* escape;
        RegexCompiler compiler;
        compiler.compile( "[\\b\\f\\n\\r\\t\\\"\\\'\\\\]", &escape );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "\b";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &escape );
        CHECK( lexer.lexeme() == "\b" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "\f";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &escape );
        CHECK( lexer.lexeme() == "\f" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "\n";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &escape );
        CHECK( lexer.lexeme() == "\n" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "\r";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &escape );
        CHECK( lexer.lexeme() == "\r" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "\t";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &escape );
        CHECK( lexer.lexeme() == "\t" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "\\";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &escape );
        CHECK( lexer.lexeme() == "\\" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "\"";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &escape );
        CHECK( lexer.lexeme() == "\"" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "\'";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &escape );
        CHECK( lexer.lexeme() == "\'" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }


    TEST( OctalEscapeCharacters )
    {
        void* octal_escape;
        RegexCompiler compiler;
        compiler.compile( "[\\1\\01\\001\\177]", &octal_escape );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "\1";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &octal_escape );
        CHECK( lexer.lexeme() == "\1" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "\177";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &octal_escape );
        CHECK( lexer.lexeme() == "\177" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );        
    }


    TEST( HexadecimalEscapeCharacters )
    {
        void* hexadecimal_escape;
        RegexCompiler compiler;
        compiler.compile( "[\\x01\\x7f\\xfffd]", &hexadecimal_escape );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "\x01";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hexadecimal_escape );
        CHECK( lexer.lexeme() == "\x01" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );

        regex = "\x7f";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &hexadecimal_escape );
        CHECK( lexer.lexeme() == "\x7f" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );        
    }
    
    
    TEST( Strings )
    {
        void* string;
        RegexCompiler compiler;
        compiler.compile( "\\\"([^\\\"]|\\\\\\\")*\\\"", &string );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "\"foo\"";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &string );
        CHECK( lexer.lexeme() == "\"foo\"" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }

    
    TEST( PlusOrMinus )
    {
        void* plus_or_minus;
        RegexCompiler compiler;
        compiler.compile( "(\\+|\\-)?", &plus_or_minus );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "+";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &plus_or_minus );
        CHECK( lexer.lexeme() == "+" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        
        regex = "-";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &plus_or_minus );
        CHECK( lexer.lexeme() == "-" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    
    
    TEST( Whitespace )
    {
        void* whitespace;
        RegexCompiler compiler;
        compiler.compile( "[ \t\r\n]*", &whitespace );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = " ";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &whitespace );
        CHECK( lexer.lexeme() == " " );
        
        regex = "  ";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &whitespace );
        CHECK( lexer.lexeme() == "  " );
        
        regex = " \t";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &whitespace );
        CHECK( lexer.lexeme() == " \t" );

        regex = "if";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &whitespace );
        CHECK( lexer.lexeme() == "" );

        regex = "";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
        CHECK( lexer.lexeme() == "" );
    }
    
    
    TEST( OrPrecedence )
    {
        void* ab_or_cd;
        RegexCompiler compiler;
        compiler.compile( "ab|cd", &ab_or_cd );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "ab";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &ab_or_cd );
        CHECK( lexer.lexeme() == "ab" );
        
        regex = "cd";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &ab_or_cd );
        CHECK( lexer.lexeme() == "cd" );
        
        regex = "acd";      
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }


    TEST( MultipleDotNodesLexer )
    {
        void* lt = (void*) 0x1;
        void* lt_question_xml = (void*) 0x2;
        std::vector<RegexToken> tokens;
        tokens.push_back( RegexToken(TOKEN_LITERAL, 0, lt, "<") );
        tokens.push_back( RegexToken(TOKEN_LITERAL, 0, lt_question_xml, "<?xml") );
        RegexCompiler compiler;
        compiler.compile( tokens );
        Lexer<const char*> lexer( compiler.state_machine() );
        
        const char* regex = "<";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == lt );
        CHECK( lexer.lexeme() == "<" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );        

        regex = "<?xml";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == lt_question_xml );
        CHECK( lexer.lexeme() == "<?xml" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    
    
    TEST( Alnum )
    {
        void* alnum;
        RegexCompiler compiler;
        compiler.compile( "[[:alnum:]]*", &alnum );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );
        
        const char* regex = "ABZaez03459";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &alnum );
        CHECK( lexer.lexeme() == "ABZaez03459" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    
    
    TEST( Word )
    {
        void* word;
        RegexCompiler compiler;
        compiler.compile( "[[:word:]]*", &word );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );

        const char* regex = "ABZaez03459_";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &word );
        CHECK( lexer.lexeme() == "ABZaez03459_" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }


    TEST( Alpha )
    {
        void* alpha;
        RegexCompiler compiler;
        compiler.compile( "[[:alpha:]]*", &alpha );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );

        const char* regex = "ABCMNOXYZabcmnoxyz";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &alpha );
        CHECK( lexer.lexeme() == "ABCMNOXYZabcmnoxyz" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }


    TEST( Blank )
    {
        void* blank;
        RegexCompiler compiler;
        compiler.compile( "[[:blank:]]*", &blank );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );

        const char* regex = " \t";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &blank );
        CHECK( lexer.lexeme() == " \t" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }


    TEST( Cntrl )
    {
        void* cntrl;
        RegexCompiler compiler;
        compiler.compile( "[[:cntrl:]]*", &cntrl );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );

        const char* regex = "\x01\x02\x7f";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &cntrl );
        CHECK( lexer.lexeme() == "\x01\x02\x7f" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }


    TEST( Digit )
    {
        void* digit;
        RegexCompiler compiler;
        compiler.compile( "[[:digit:]]*", &digit );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );

        const char* regex = "012456789";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &digit );
        CHECK( lexer.lexeme() == "012456789" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }


    TEST( Graph )
    {
        void* graph;
        RegexCompiler compiler;
        compiler.compile( "[[:graph:]]*", &graph );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );

        const char* regex = "abcdefg";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &graph );
        CHECK( lexer.lexeme() == "abcdefg" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }        

    
    TEST( Lower )
    {
        void* lower;
        RegexCompiler compiler;
        compiler.compile( "[[:lower:]]*", &lower );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );

        const char* regex = "abcmnoxyz";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &lower );
        CHECK( lexer.lexeme() == "abcmnoxyz" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    

    TEST( Print )
    {    
        void* print;
        RegexCompiler compiler;
        compiler.compile( "[[:print:]]*", &print );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );

        const char* regex = "abcd";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &print );
        CHECK( lexer.lexeme() == "abcd" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    

    TEST( Punct ) 
    {
        void* punct;
        RegexCompiler compiler;
        compiler.compile( "[[:punct:]]*", &punct );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );

        const char* regex = "$#@%";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &punct );
        CHECK( lexer.lexeme() == "$#@%" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    

    TEST( Space ) 
    {

        void* space;
        RegexCompiler compiler;
        compiler.compile( "[[:space:]]*", &space );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );

        const char* regex = " \t\n\r";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &space );
        CHECK( lexer.lexeme() == " \t\n\r" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    

    TEST( Upper ) 
    {

        void* upper;
        RegexCompiler compiler;
        compiler.compile( "[[:upper:]]*", &upper );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );

        const char* regex = "ABCMNOXYZ";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &upper );
        CHECK( lexer.lexeme() == "ABCMNOXYZ" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    

    TEST( Xdigit ) 
    {
        void* xdigit;
        RegexCompiler compiler;
        compiler.compile( "[[:xdigit:]]*", &xdigit );
        Lexer<const char*> lexer( compiler.state_machine(), NULL );

        const char* regex = "0129ABDfacef";
        lexer.reset( regex, regex + strlen(regex) );
        lexer.advance();
        CHECK( lexer.symbol() == &xdigit );
        CHECK( lexer.lexeme() == "0129ABDfacef" );
        lexer.advance();
        CHECK( lexer.symbol() == NULL );
    }
    
    
    TEST( PositionIterator )
    {
        void* whitespace;
        RegexCompiler compiler;
        compiler.compile( "[ \\t\\r\\n]", &whitespace );
        Lexer<PositionIterator<const char*> > lexer( compiler.state_machine(), NULL );
        
        const char* regex = "\n \r\n \r \r\n";
        lexer.reset( PositionIterator<const char*>(regex, regex + strlen(regex)), PositionIterator<const char*>() );
        lexer.advance();
        CHECK( lexer.symbol() == &whitespace );
        CHECK( lexer.position().line() == 2 );

        lexer.advance();
        lexer.advance();
        CHECK( lexer.symbol() == &whitespace );
        CHECK( lexer.position().line() == 2 );
        lexer.advance();
        CHECK( lexer.symbol() == &whitespace );
        CHECK( lexer.position().line() == 3 );

        lexer.advance();
        lexer.advance();
        CHECK( lexer.symbol() == &whitespace );
        CHECK( lexer.position().line() == 4 );

        lexer.advance();
        lexer.advance();
        CHECK( lexer.symbol() == &whitespace );
        CHECK( lexer.position().line() == 4 );

        lexer.advance();
        CHECK( lexer.symbol() == &whitespace );
        CHECK( lexer.position().line() == 5 );        
    }
}
