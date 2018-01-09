//
// GrammarParser.cpp
// Copyright (c) 2007 - 2011 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "GrammarParser.hpp"
#include "parser_types.hpp"
#include "Error.hpp"
#include "ParserErrorPolicy.hpp"
#include "ParserAction.hpp"
#include "ParserProduction.hpp"
#include "ParserSymbol.hpp"
#include <sweet/lexer/LexerToken.hpp>
#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/classic_file_iterator.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/bind.hpp>

using std::string;
using std::set;
using std::map;
using std::pair;
using std::make_pair;
using std::vector;
using namespace sweet;
using namespace sweet::lexer;
using namespace sweet::parser;

namespace sweet
{

namespace parser
{

/**
// ParserGrammar for grammars.
*/
template <class Iterator>
class GrammarGrammar : public boost::spirit::grammar<GrammarGrammar<Iterator> >
{
    public:
        typedef Iterator iterator;

        template <typename Scanner>
        class definition
        {
            boost::spirit::rule<Scanner> unit;
            boost::spirit::rule<Scanner> grammar;            
            boost::spirit::rule<Scanner> directive;
            boost::spirit::rule<Scanner> associativity_directive;
            boost::spirit::rule<Scanner> whitespace_directive;
            boost::spirit::rule<Scanner> associativity;
            boost::spirit::rule<Scanner> production;
            boost::spirit::rule<Scanner> expression;
            boost::spirit::rule<Scanner> cat_expression;
            boost::spirit::rule<Scanner> base_expression;
            boost::spirit::rule<Scanner> precedence;
            boost::spirit::rule<Scanner> action;
            boost::spirit::rule<Scanner> literal_terminal;
            boost::spirit::rule<Scanner> regex_terminal;
            boost::spirit::rule<Scanner> identifier;
            boost::spirit::rule<Scanner> syntax_error;
            boost::spirit::rule<Scanner> syntax_error_in_grammar;

            public:
                definition( const GrammarGrammar& self );
                boost::spirit::rule<Scanner> const& start() const;
        };

    private:
        GrammarParser* parser_; ///< The GrammarParser to report matches to.

    public:
        GrammarGrammar( GrammarParser* parser );
        GrammarParser* get_parser() const;
        void syntax_error_( iterator at ) const;
        void syntax_error_in_grammar_( iterator at ) const;
        void begin_production( iterator start, iterator finish ) const;
        void end_and_begin_production( iterator start ) const;
        void lexeme( iterator start, iterator finish ) const;
        const std::string& convert_escape_characters( std::string& lexeme ) const;
};

}

}

/**
// Constructor.
//
// @param self
//  The ParserGrammar that this grammar definition is for.
*/
template <class Iterator>
template <class Scanner>
GrammarGrammar<Iterator>::definition<Scanner>::definition( const GrammarGrammar& self )
{
    using namespace boost::spirit;

    GrammarParser* parser = self.get_parser();

    unit = (
               grammar 
               | *syntax_error [boost::bind(&GrammarGrammar::syntax_error_, &self, _1)]
         )
         >> end_p
         ;

    grammar = identifier [boost::bind(&GrammarParser::begin_grammar, parser)]
           >> ch_p( '{' )
           >> *(directive | production)
           >> ch_p( '}' )
           ;

    directive = associativity_directive
              | whitespace_directive
              ;

    associativity_directive = associativity
              >> *(
                     literal_terminal [boost::bind(&GrammarParser::associate, parser, TOKEN_LITERAL)]
                     | regex_terminal [boost::bind(&GrammarParser::associate, parser, TOKEN_REGULAR_EXPRESSION)]
                     | identifier [boost::bind(&GrammarParser::associate, parser, TOKEN_NULL)]
              )   
              >> ch_p( ';' )
              ;
              
    whitespace_directive = str_p( "%whitespace" )
                         >> (
                                literal_terminal [boost::bind(&GrammarParser::whitespace_directive, parser, lexer::TOKEN_LITERAL)]
                                | regex_terminal [boost::bind(&GrammarParser::whitespace_directive, parser, lexer::TOKEN_REGULAR_EXPRESSION)]
                         )
                         >> ch_p( ';' )
                         ;

    associativity = str_p( "%left" ) [boost::bind(&GrammarParser::associativity, parser, ASSOCIATE_LEFT)]
                  | str_p( "%right" ) [boost::bind(&GrammarParser::associativity, parser, ASSOCIATE_RIGHT)]
                  | str_p( "%none" ) [boost::bind(&GrammarParser::associativity, parser, ASSOCIATE_NONE)]
                  ;    
    
    production = identifier [boost::bind(&GrammarGrammar::begin_production, &self, _1, _2)]
               >> ch_p( ':' )
               >> !expression
               >> ch_p( ';' ) [boost::bind(&GrammarParser::end_production, parser)]
               | syntax_error_in_grammar  [boost::bind(&GrammarGrammar::syntax_error_in_grammar_, &self, _1)]
               ;

    expression = !cat_expression
               >> !precedence
               >> !action
               >> !(
                      str_p("|") [boost::bind(&GrammarGrammar::end_and_begin_production, &self, _1)]
                      >> expression
               )
               ;

    cat_expression = base_expression
                   >> !cat_expression 
                   ;

    base_expression = literal_terminal [boost::bind(&GrammarParser::literal_terminal, parser)]
                    | regex_terminal [boost::bind(&GrammarParser::regex_terminal, parser)]
                    | identifier [boost::bind(&GrammarParser::identifier, parser)]
                    ;

    precedence = str_p( "%precedence" )
               >> (
                     literal_terminal [boost::bind(&GrammarParser::precedence, parser, TOKEN_LITERAL)]
                     | regex_terminal [boost::bind(&GrammarParser::precedence, parser, TOKEN_REGULAR_EXPRESSION)]
                     | identifier  [boost::bind(&GrammarParser::precedence, parser, TOKEN_NULL)]
               )
               ;             

    action = confix_p(
                '[',
                identifier [boost::bind(&GrammarParser::action_identifier, parser)],
                ']'
             );
             
    literal_terminal = confix_p( 
                   '\'', 
                   (*lex_escape_ch_p) [boost::bind(&GrammarGrammar::lexeme, &self, _1, _2)],
                   '\'' 
               );

    regex_terminal = confix_p( 
                   '"', 
                   (*lex_escape_ch_p) [boost::bind(&GrammarGrammar::lexeme, &self, _1, _2)],
                   '"' 
            );

    identifier = lexeme_d[ 
                    (alpha_p | ch_p("_"))
                    >> *(alnum_p | ch_p("_"))
                 ] [boost::bind(&GrammarGrammar::lexeme, &self, _1, _2)];
                 
    syntax_error = *(anychar_p - ch_p('}')) 
                 >> ch_p( '}' )
                 ;

    syntax_error_in_grammar = *(anychar_p - ch_p(';')) 
                            >> ch_p( ';' )
                            ;

    BOOST_SPIRIT_DEBUG_RULE( unit );
    BOOST_SPIRIT_DEBUG_RULE( grammar );
    BOOST_SPIRIT_DEBUG_RULE( directive );
    BOOST_SPIRIT_DEBUG_RULE( associativity );
    BOOST_SPIRIT_DEBUG_RULE( production );
    BOOST_SPIRIT_DEBUG_RULE( expression );
    BOOST_SPIRIT_DEBUG_RULE( cat_expression );
    BOOST_SPIRIT_DEBUG_RULE( base_expression );
    BOOST_SPIRIT_DEBUG_RULE( precedence );
    BOOST_SPIRIT_DEBUG_RULE( action );
    BOOST_SPIRIT_DEBUG_RULE( literal_terminal );
    BOOST_SPIRIT_DEBUG_RULE( regex_terminal );
    BOOST_SPIRIT_DEBUG_RULE( identifier );
    BOOST_SPIRIT_DEBUG_RULE( syntax_error );
    BOOST_SPIRIT_DEBUG_RULE( syntax_error_in_grammar );
}

/**
// Get the starting symbol for the grammar grammar.
//
// @return
//  The starting symbol.
*/
template <class Iterator>
template <class Scanner>
boost::spirit::rule<Scanner> const& GrammarGrammar<Iterator>::definition<Scanner>::start() const
{
    return unit;
}

/**
// Constructor.
//
// @param parser
//  The GrammarParser to report grammar matches to.
*/
template <class Iterator>
GrammarGrammar<Iterator>::GrammarGrammar( GrammarParser* parser )
: parser_( parser )
{
    SWEET_ASSERT( parser_ );
}

/**
// Get the GrammarParser that this ParserGrammar reports matches to.
//
// @return
//  The GrammarParser.
*/
template <class Iterator>
GrammarParser* GrammarGrammar<Iterator>::get_parser() const
{
    return parser_;
}

/**
// Handle a syntax error that occurs while parsing a grammar.
//
// @param at
//  The iterator at the beginning of the construct that has the error.
*/
template <class Iterator>
void GrammarGrammar<Iterator>::syntax_error_( iterator at ) const
{
    SWEET_ASSERT( parser_  );
    parser_->syntax_error( at.get_position().line );
}

/**
// Handle a syntax error that occurs while parsing a grammar.
//
// @param at
//  The iterator at the beginning of the construct that has the error.
*/
template <class Iterator>
void GrammarGrammar<Iterator>::syntax_error_in_grammar_( iterator at ) const
{
    SWEET_ASSERT( parser_ );
    parser_->syntax_error_in_grammar( at.get_position().line );
}

/**
// Convert [\e start, \e finish) to a string and pass it and the current
// line number through to GrammarParser::begin_production().
//
// @param start
//  The iterator at the first character in the identifier for the ParserSymbol that
//  the production reduces to.
//
// @param finish
//  The iterator one past the last character in the identifier for the 
//  ParserSymbol that the production reduces to.
*/
template <class Iterator>
void GrammarGrammar<Iterator>::begin_production( iterator start, iterator finish ) const
{
    SWEET_ASSERT( parser_ );
    parser_->begin_production( string(start, finish), start.get_position().line );    
}

/**
// Pass it the current line number through to 
// GrammarParser::begin_and_end_production().
//
// @param start
//  The iterator at the '|' character in the grammar that begins and ends the 
//  production.
*/
template <class Iterator>
void GrammarGrammar<Iterator>::end_and_begin_production( iterator start ) const
{
    SWEET_ASSERT( parser_ );
    parser_->end_and_begin_production( start.get_position().line );
}

/**
// Convert [\e start, \e finish) to a string and pass it through to 
// GrammarParser::lexeme().
//
// @param start
//  The iterator at the first character in the lexeme.
//
// @param finish
//  The iterator one past the last character in the lexeme.
*/
template <class Iterator>
void GrammarGrammar<Iterator>::lexeme( iterator start, iterator finish ) const
{
    SWEET_ASSERT( parser_ );
    parser_->lexeme( string(start, finish) );
}

/**
// Convert escape characters in \e lexeme to their escaped values.
//
// @param lexeme
//  The string to convert escape characters in.
//
// @return
//  Returns \e lexeme with escape characters converted to their escaped 
//  values.
*/
template <class Iterator>
const std::string& GrammarGrammar<Iterator>::convert_escape_characters( std::string& lexeme ) const
{
    string::iterator destination = lexeme.begin();
    string::const_iterator source = lexeme.begin();
    string::iterator end = lexeme.end();
    
    while ( source != end )
    {
        switch ( *source )
        {
            case '\\':
                ++source;
                if ( source != lexeme.end() )
                {
                    switch ( *source )
                    {
                        case 'b':
                            *destination = '\b';
                            break;
                        
                        case 'f':
                            *destination = '\f';
                            break;
                        
                        case 'n':
                            *destination = '\n';
                            break;
                        
                        case 'r':
                            *destination = '\r';
                            break;
                        
                        case 't':
                            *destination = '\t';
                            break;
                            
                        default:
                            *destination = *source;
                            break;
                    }
                    ++destination;
                    ++source;
                }
                break;
                
            default:
                *destination = *source;
                ++destination;
                ++source;
                break;
        }
    }
    
    lexeme.erase( destination, end );
    return lexeme;
}

/**
// Constructor.
//
// @param filename
//  The name of the file to parse the grammar from.
//
// @param error_policy
//  The ParserErrorPolicy to report errors to or null to ignore errors.
*/
GrammarParser::GrammarParser( const char* filename, ParserErrorPolicy* error_policy )
: error_policy_( error_policy ),
  identifier_(),
  lexeme_(),
  associativity_( ASSOCIATE_NULL ),
  symbol_( NULL ),
  precedence_( 0 ),
  symbols_(),
  grammar_(),
  errors_( 0 )
{
    symbols_[make_pair(TOKEN_NULL, ERROR_KEYWORD)] = grammar_.error_symbol();

    boost::spirit::file_iterator<char> file_first( filename );
    if ( !file_first )
    {
        SWEET_ERROR( OpeningFileFailedError("Opening '%s' failed", filename) );
    }

    typedef boost::spirit::position_iterator<boost::spirit::file_iterator<char> > iterator;
    iterator first( file_first, file_first.make_end() );
    iterator last;

    using namespace boost;
    GrammarGrammar<iterator> grammar( this );
    spirit::parse_info<iterator> info = spirit::parse( first, last, grammar, spirit::space_p | spirit::comment_p("//") | spirit::comment_p("/*", "*/") );
    if ( !info.hit || !info.full )
    {
        SWEET_ERROR( ParsingFailedError("Parsing '%s' failed", filename) );
    }
}

/**
// Constructor.
//
// @param start
//  The first character in the string to parse.
//
// @param finish
//  The last character in the string to parse.
//
// @param error_policy
//  The ParserErrorPolicy to report errors to or null to ignore errors.
*/
GrammarParser::GrammarParser( const char* start, const char* finish, ParserErrorPolicy* error_policy )
: error_policy_( error_policy ),
  identifier_(),
  lexeme_(),
  associativity_( ASSOCIATE_NULL ),
  symbol_( NULL ),
  precedence_( 0 ),
  symbols_(),
  grammar_(),
  errors_( 0 )
{
    SWEET_ASSERT( start );
    SWEET_ASSERT( finish );
    SWEET_ASSERT( finish >= start );

    symbols_[make_pair(TOKEN_NULL, ERROR_KEYWORD)] = grammar_.error_symbol();

    typedef boost::spirit::position_iterator<const char*> iterator;
    iterator first( start, finish );
    iterator last;

    using namespace boost;
    GrammarGrammar<iterator> grammar( this );
    spirit::parse_info<iterator> info = spirit::parse( first, last, grammar, spirit::space_p | spirit::comment_p("//") | spirit::comment_p("/*", "*/") );
    if ( !info.hit || !info.full )
    {
        SWEET_ERROR( ParsingFailedError("Parsing failed") );
    }
}

/**
// Get the grammar resulting from parsing the grammar.
//
// @return
//  The grammar.
*/
ParserGrammar& GrammarParser::grammar()
{
    return grammar_;
}

/**
// Get the tokens resulting from parsing the grammar.
//
// @return
//  The tokens.
*/
std::vector<lexer::LexerToken>& GrammarParser::tokens()
{
    return tokens_;
}

/**
// Get the whitespace tokens resulting from parsing the grammar.
//
// @return
//  The whitespace tokens.
*/
std::vector<lexer::LexerToken>& GrammarParser::whitespace_tokens()
{
    return whitespace_tokens_;
}

/**
// Get the number of errors that occured while parsing a grammar.
//
// @return
//  The number of errors.
*/
int GrammarParser::errors() const
{
    return errors_;
}

/**
// Add a new or find an existing symbol identified by \e identifier.
//
// @param type
//  The type of symbol to find or create.
//
// @param token_type
//  The token type of the symbol to find or create.
//
// @param lexeme
//  The identifier of the symbol to find or create.
//
// @param line
//  The line that the symbol is defined on or 0 if the symbol is referenced
//  not defined.
//
// @return
//  The ParserSymbol.
*/
ParserSymbol* GrammarParser::symbol( SymbolType type, lexer::LexerTokenType token_type, const string& lexeme, int line )
{   
    pair<lexer::LexerTokenType, string> key( make_pair(token_type, lexeme) );
    map<pair<lexer::LexerTokenType, string>, ParserSymbol*>::iterator i = symbols_.find( key );
    if ( i == symbols_.end() )
    {
        i = symbols_.insert( make_pair(key, grammar_.add_symbol(type, lexeme, line)) ).first;
    }

    if ( type != SYMBOL_NULL && i->second->get_type() == SYMBOL_NULL )
    {
        i->second->set_type( type );
    }

    if ( line != 0 )
    {
        i->second->set_line( line );
    }

    return i->second;
}

/**
// Add a new or find an existing action identified by the most recently 
// parsed lexeme.
//
// @return
//  The action.
*/
ParserAction* GrammarParser::action( const std::string& identifier )
{
    SWEET_ASSERT( !identifier.empty() );    
    map<string, ParserAction*>::iterator i = actions_.find( identifier );
    if ( i == actions_.end() )
    {
        i = actions_.insert( make_pair(identifier, grammar_.add_action(identifier)) ).first;
    }    
    return i->second;
}

/**
// Start a ParserGrammar.
*/
void GrammarParser::begin_grammar()
{
    SWEET_ASSERT( !lexeme_.empty() );
    grammar_.identifier( lexeme_ );
    lexeme_.clear();
}

/**
// Set the associativity that will apply to following terminals.
//
// @param associativity
//  The value to set associativity to.
*/
void GrammarParser::associativity( SymbolAssociativity associativity )
{
    associativity_ = associativity;
    ++precedence_;
}

/**
// Associate a terminal with the current associativity and precedence.
//
// @param type
//  The LexerTokenType of the ParserSymbol to operate on.
*/
void GrammarParser::associate( lexer::LexerTokenType type )
{
    SWEET_ASSERT( !lexeme_.empty() );
    ParserSymbol* symbol = GrammarParser::symbol( SYMBOL_NULL, type, lexeme_, 0 );
    SWEET_ASSERT( symbol );
    symbol->set_associativity( associativity_ );
    symbol->set_precedence( precedence_ );
    lexeme_.clear();
}

/**
// Set the precedence of the current production to the precedence of a 
// terminal.
//
// @param type
//  The type of token to set the precedence from.
*/
void GrammarParser::precedence( lexer::LexerTokenType type )
{
    SWEET_ASSERT( !lexeme_.empty() );
    ParserSymbol* symbol = GrammarParser::symbol( SYMBOL_NULL, type, lexeme_, 0 );
    grammar_.precedence_symbol( symbol );
    lexeme_.clear();
}

/**
// Add a whitespace token to the lexer.
//
// @param type
//  The type of whitespace token to add to the lexer.
*/
void GrammarParser::whitespace_directive( lexer::LexerTokenType type )
{
    SWEET_ASSERT( !lexeme_.empty() );
    whitespace_tokens_.push_back( lexer::LexerToken(type, 0, NULL, lexeme_) );
    lexeme_.clear();
}

/**
// Start a production for the ParserSymbol whose identifier is \e identifier.
//
// @param identifier
//  The identifier of the symbol on the left hand side of the production.
//
// @param line
//  The line that the symbol is defined on.
*/
void GrammarParser::begin_production( const string& identifier, int line )
{
    SWEET_ASSERT( !lexeme_.empty() );
    SWEET_ASSERT( lexeme_ == identifier );

    lexeme_.clear();
    symbol_ = symbol( SYMBOL_NON_TERMINAL, TOKEN_NULL, identifier, line );    
    grammar_.begin_production( symbol_, line );
}

/**
// End the current production and start a new production for the same symbol.
//
// @param line
//  The line that the production is defined on.
*/
void GrammarParser::end_and_begin_production( int line )
{
    SWEET_ASSERT( symbol_ );
    grammar_.end_production();
    grammar_.begin_production( symbol_, line );
}

/**
// End a production.
*/
void GrammarParser::end_production()
{
    grammar_.end_production();
    symbol_ = NULL;
}

/**
// Accept an action.
*/
void GrammarParser::action_identifier()
{
    SWEET_ASSERT( !lexeme_.empty() );
    grammar_.action( action(lexeme_) );
    lexeme_.clear();
}

/**
// Reduce the most recently parsed lexeme into a symbol expression for a 
// literal terminal symbol.
*/
void GrammarParser::literal_terminal()
{
    SWEET_ASSERT( !lexeme_.empty() );
    ParserSymbol* terminal_symbol = symbol( SYMBOL_TERMINAL, TOKEN_LITERAL, lexeme_, 0 );
    grammar_.symbol( terminal_symbol );
    tokens_.push_back( lexer::LexerToken(lexer::TOKEN_LITERAL, 0, terminal_symbol, lexeme_) );
    lexeme_.clear();
}

/**
// Reduce the most recently parsed lexeme into a symbol expression for a 
// regular expression terminal symbol.
*/
void GrammarParser::regex_terminal()
{
    SWEET_ASSERT( !lexeme_.empty() );
    ParserSymbol* terminal_symbol = symbol( SYMBOL_TERMINAL, TOKEN_REGULAR_EXPRESSION, lexeme_, 0 );
    grammar_.symbol( terminal_symbol );
    tokens_.push_back( lexer::LexerToken(lexer::TOKEN_REGULAR_EXPRESSION, 0, terminal_symbol, lexeme_) );
    lexeme_.clear();
}

/**
// Reduce the most recently parsed lexeme into a symbol expression for a 
// non terminal symbol.
*/
void GrammarParser::identifier()
{
    SWEET_ASSERT( !lexeme_.empty() );
    grammar_.symbol( symbol(SYMBOL_NON_TERMINAL, TOKEN_NULL, lexeme_, 0) );
    lexeme_.clear();
}

/**
// Accept a terminal or regular expression and store its lexeme for later use.
//
// @param lexeme
//  The lexeme of the terminal or regular expression to accept.
*/
void GrammarParser::lexeme( const string& lexeme )
{
    SWEET_ASSERT( lexeme_.empty() );
    lexeme_ = lexeme;
}

/**
// Handle a syntax error that occurs outside of parsing a grammar.
//
// @param line
//  The line that the syntax error occured on.
*/
void GrammarParser::syntax_error( int line )
{
    ++errors_;
    if ( error_policy_ )
    {
        error_policy_->parser_error( line, SyntaxError("Syntax error while parsing grammar") );
    }
}

/**
// Handle a syntax error that occurs during parsing a grammar.
//
// @param line
//  The line that the syntax error occured on.
*/
void GrammarParser::syntax_error_in_grammar( int line )
{
    ++errors_;
    if ( error_policy_ )
    {
        error_policy_->parser_error( line, SyntaxError("Syntax error while parsing grammar '%s'", identifier_.c_str()) );
    }
}
