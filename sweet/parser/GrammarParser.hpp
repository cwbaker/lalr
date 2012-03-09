//
// GrammarParser.hpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//

#ifndef GRAMMARPARSER_INCLUDED
#define GRAMMARPARSER_INCLUDED

#include <sweet/parser/ParserSymbol.hpp>
#include <sweet/parser/ParserGrammar.hpp>
#include <sweet/lexer/LexerToken.hpp>
#include <map>
#include <string>

namespace sweet
{

namespace parser
{

class ParserErrorPolicy;

/**
// @internal
//
// Parse grammars.
*/
class GrammarParser
{
    ParserErrorPolicy* error_policy_;
    std::string identifier_;
    std::string lexeme_;
    SymbolAssociativity associativity_;
    ParserSymbol* symbol_;
    int precedence_;
    std::map<std::pair<lexer::LexerTokenType, std::string>, ParserSymbol*> symbols_;
    std::map<std::string, ParserAction*> actions_;
    ParserGrammar grammar_;
    std::vector<lexer::LexerToken> tokens_;
    std::vector<lexer::LexerToken> whitespace_tokens_;
    int errors_;

    public:
        GrammarParser( const char* filename, ParserErrorPolicy* error_policy );
        GrammarParser( const char* start, const char* finish, ParserErrorPolicy* error_policy );

        ParserGrammar& grammar();        
        std::vector<lexer::LexerToken>& tokens();
        std::vector<lexer::LexerToken>& whitespace_tokens();
        int errors() const;

        ParserSymbol* symbol( SymbolType type, lexer::LexerTokenType token_type, const std::string& lexeme, int line );
        ParserAction* action( const std::string& identifier );
        
        void begin_grammar();
        void associativity( SymbolAssociativity associativity );
        void associate( lexer::LexerTokenType type );
        void precedence( lexer::LexerTokenType type );
        void whitespace_directive( lexer::LexerTokenType type );    
        void begin_production( const std::string& identifier, int line );
        void end_and_begin_production( int line );
        void end_production();
        void cat_expression();
        void action_identifier();
        void literal_terminal();
        void regex_terminal();
        void identifier();
        void lexeme( const std::string& lexeme );
        void syntax_error( int line );
        void syntax_error_in_grammar( int line );
};

}

}

#endif
