//
// ParserSymbol.cpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//    

#include "stdafx.hpp"
#include "ParserSymbol.hpp"
#include "ParserProduction.hpp"

using std::vector;
using std::set;
using namespace sweet;
using namespace sweet::parser;

/**
// Constructor.
*/
ParserSymbol::ParserSymbol()
: type_( SYMBOL_NULL ),
  lexeme_(),
  identifier_(),
  line_( 0 ),
  productions_(),
  precedence_( 0 ),
  associativity_( ASSOCIATE_NULL ),
  nullable_( false ),
  first_(),
  follow_(),
  index_( INVALID_INDEX )
{
}

/**
// Constructor.
//
// @param type
//  The type of this symbol.
//
// @param lexeme
//  The lexeme of this symbol.
//
// @param line
//  The line that this symbol is defined on.
*/
ParserSymbol::ParserSymbol( SymbolType type, const std::string& lexeme, int line )
: type_( type ),
  lexeme_( lexeme ),
  identifier_(),
  line_( line ),
  productions_(),
  precedence_( 0 ),
  associativity_( ASSOCIATE_NULL ),
  nullable_( false ),
  first_(),
  follow_(),
  index_( INVALID_INDEX )
{
}

/**
// Get the lexeme of this symbol.
//
// @return
//  The lexeme of this symbol.
*/
const std::string& ParserSymbol::get_lexeme() const
{
    return lexeme_;
}

/**
// Get the identifier for this symbol.
//
// @return
//  The identifier.
*/
const std::string& ParserSymbol::get_identifier() const
{
    return identifier_;
}

/**
// Set the type of this symbol.
//
// @param type
//  The value to set the type of this symbol to.
*/
void ParserSymbol::set_type( SymbolType type )
{
    type_ = type;
}

/**
// Get the type of this symbol.
//
// @return
//  The type of this symbol.
*/
SymbolType ParserSymbol::get_type() const
{
    return type_;
}

/**
// Set the line that this symbol is defined on.
//
// @param line
//  The line to set this symbol as being defined on.
*/
void ParserSymbol::set_line( int line )
{
    line_ = line;
}

/**
// Get the line that this symbol is defined on.
//
// @return
//  The line that this symbol is defined on or 0 if this symbol isn't defined
//  or if state machine generation failed before its line number is set.
*/
int ParserSymbol::get_line() const
{
    return line_;
}

/**
// Append a production to this symbol.
//
// @param production
//  The production to append (assumed not null).
*/
void ParserSymbol::append_production( ptr<ParserProduction> production )
{
    SWEET_ASSERT( production );
    productions_.push_back( production );
}

/**
// Get the productions that reduce to this symbol.
//
// @return
//  The productions that reduce to this symbol.
*/
const std::vector<ptr<ParserProduction> >& ParserSymbol::get_productions() const
{
    return productions_;
}

/**
// Is this symbol nullable?
//
// @return
//  True if this symbol is nullable otherwise false.
*/
bool ParserSymbol::is_nullable() const
{
    return nullable_;
}

/**
// Replace this symbol's identifier and line, precedence, and associativity 
// with those from \e non_terminal_symbol.  This is done as an optimization 
// to remove redundant reductions for implicit terminal symbols.  See 
// ParserGrammar::calculate_implicit_terminal_symbols().
//
// This symbol is assumed to be of type SYMBOL_TERMINAL.
//
// @param non_terminal_symbol
//  The non terminal symbol to replace the identifier, line, precedence, and
//  associativity for this symbol with (assumed not null and of type 
//  SYMBOL_NON_TERMINAL).
*/
void ParserSymbol::replace_by_non_terminal( const ParserSymbol* non_terminal_symbol )
{
    SWEET_ASSERT( get_type() == SYMBOL_TERMINAL );
    SWEET_ASSERT( non_terminal_symbol );    
    SWEET_ASSERT( non_terminal_symbol->get_type() == SYMBOL_NON_TERMINAL );    
    
    identifier_ = non_terminal_symbol->lexeme_;
    line_ = non_terminal_symbol->line_;
    precedence_ = non_terminal_symbol->precedence_;
    associativity_ = non_terminal_symbol->associativity_;
}

/**
// Get the implicit terminal for this symbol.
//
// @return
//  The implict terminal for this symbol or null if this symbol doesn't have
//  an implicit terminal.
*/
ParserSymbol* ParserSymbol::get_implicit_terminal() const
{
    ParserSymbol* implicit_terminal_symbol = NULL;
    
    if ( productions_.size() == 1 )
    {
        const ParserProduction* production = productions_.front().get();
        SWEET_ASSERT( production );

        if ( production->get_length() == 1 && production->get_action() == NULL )
        {
            ParserSymbol* symbol = production->get_symbols().front();
            if ( symbol->get_type() == SYMBOL_TERMINAL )
            {
                implicit_terminal_symbol = symbol;
            }
        }
    }
    
    return implicit_terminal_symbol;
}

/**
// Add a symbol to the first set of this symbol.
//
// @param symbol
//  The symbol to add to the first set of this symbol.
//
// @return
//  The number of symbols added (0 or 1).
*/
int ParserSymbol::add_symbol_to_first( const ParserSymbol* symbol )
{
    SWEET_ASSERT( symbol );
    return first_.insert( symbol ).second ? 1 : 0;
}

/**
// Add one or more symbols to the first set of this symbol.
//
// @param symbols
//  The symbols to add to the first set of this symbol.
//
// @return
//  The number of symbols added.
*/
int ParserSymbol::add_symbols_to_first( const std::set<const ParserSymbol*>& symbols )
{
    size_t original_size = first_.size();
    first_.insert( symbols.begin(), symbols.end() );
    return first_.size() - original_size;
}

/**
// Get the first set for this ParserSymbol.
//
// @return
//  The first set for this ParserSymbol.
*/
const std::set<const ParserSymbol*>& ParserSymbol::get_first() const
{
    return first_;
}

/**
// Add a symbol to the follow set of this symbol.
//
// @param symbol
//  The symbol to add to the follow set of this symbol.
//
// @return
//  The number of symbols added (0 or 1).
*/
int ParserSymbol::add_symbol_to_follow( const ParserSymbol* symbol )
{
    SWEET_ASSERT( symbol );
    return follow_.insert( symbol ).second ? 1 : 0;
}

/**
// Add symbols to the follow set of this symbol.
//
// @param symbols
//  The symbols to add to the follow set of this symbol.
//
// @return
//  The number of symbols added.
*/
int ParserSymbol::add_symbols_to_follow( const std::set<const ParserSymbol*>& symbols )
{
    size_t original_size = follow_.size();
    follow_.insert( symbols.begin(), symbols.end() );
    return follow_.size() - original_size;
}

/**
// Get the follow set for this ParserSymbol.
//
// @return
//  The follow set for this ParserSymbol.
*/
const std::set<const ParserSymbol*>& ParserSymbol::get_follow() const
{
    return follow_;
}

/**
// Set the precedence of this ParserSymbol.
//
// @param precedence
//  The value to set the precedence of this ParserSymbol to.
*/
void ParserSymbol::set_precedence( int precedence )
{
    precedence_ = precedence;
}

/**
// Get the precedence of this ParserSymbol.
//
// @return
//  The precedence (0 indicates no precedence).
*/
int ParserSymbol::get_precedence() const
{
    return precedence_;
}

/**
// Set the associativity of this ParserSymbol.
//
// @param associativity
//  The value to set the associativity of this ParserSymbol to.
*/
void ParserSymbol::set_associativity( SymbolAssociativity associativity )
{
    associativity_ = associativity;
}

/**
// Get the associativity of this ParserSymbol.
//
// @return
//  The associativity.
*/
SymbolAssociativity ParserSymbol::get_associativity() const
{
    return associativity_;
}

/**
// Calculate the identifier for this symbol.
*/
void ParserSymbol::calculate_identifier()
{
    SWEET_ASSERT( !lexeme_.empty() );

    identifier_.reserve( lexeme_.size() );

    std::string::const_iterator i = lexeme_.begin();     
    while ( i != lexeme_.end() )
    {
        if ( isalnum(*i) || *i == '_' )
        {
            identifier_.push_back( *i );
            ++i;
        }
        else
        {
            if ( i != lexeme_.begin() )
            {
                identifier_.append( "_" );
            }

            identifier_.append( name(*i) );

            ++i;
            if ( i != lexeme_.end() )
            {
                identifier_.append( "_" );             
            }
        }
    }

    if ( type_ == SYMBOL_TERMINAL )
    {
        identifier_.append( "_terminal" );
    }    
}

/**
// Calculate the first set and nullable for this symbol.
//
// @return
//  The number of symbols added to the first set and plus one if nullable
//  changed for this symbol.
*/
int ParserSymbol::calculate_first()
{
    int added = 0;

    if ( type_ == SYMBOL_NON_TERMINAL )
    {
        for ( vector<ptr<ParserProduction> >::const_iterator i = productions_.begin(); i != productions_.end(); ++i )
        {
            const ParserProduction* production = i->get();
            SWEET_ASSERT( production );  
                  
            const vector<ParserSymbol*>& symbols = production->get_symbols();
            vector<ParserSymbol*>::const_iterator j = symbols.begin(); 
            while ( j != symbols.end() && (*j)->is_nullable() )
            {
                const ParserSymbol* symbol = *j;
                SWEET_ASSERT( symbol );
                added += add_symbols_to_first( symbol->get_first() );
                ++j;
            }
            
            if ( j != symbols.end() )
            {
                const ParserSymbol* symbol = *j;
                SWEET_ASSERT( symbol );
                added += add_symbols_to_first( symbol->get_first() );
            }
            else
            {
                added += nullable_ ? 0 : 1;
                nullable_ = true;
            }
        }
    }
    else
    {
        added += add_symbol_to_first( this );
    }
    
    return added;
}

/**
// Calculate the follow set for this symbol.
//
// @return
//  The number of symbols added to the follow set.
*/
int ParserSymbol::calculate_follow()
{
    int added = 0;

    for ( std::vector<ptr<ParserProduction> >::const_iterator i = productions_.begin(); i != productions_.end(); ++i )
    {
        const ParserProduction* production = i->get();
        SWEET_ASSERT( production );
                    
        const vector<ParserSymbol*>& symbols = production->get_symbols();
        if ( !symbols.empty() )
        {
            vector<ParserSymbol*>::const_reverse_iterator j = symbols.rbegin(); 
            ParserSymbol* symbol = *j;
            added += symbol->add_symbols_to_follow( get_follow() );
            ++j;
        
            while ( j != symbols.rend() && symbol->is_nullable() )
            {
                ParserSymbol* previous_symbol = *j;
                SWEET_ASSERT( previous_symbol );
                added += previous_symbol->add_symbols_to_follow( symbol->get_first() );
                added += previous_symbol->add_symbols_to_follow( get_follow() );
                symbol = previous_symbol;
                ++j;                
            }
            
            while ( j != symbols.rend() )
            {
                ParserSymbol* previous_symbol = *j;
                SWEET_ASSERT( previous_symbol );
                added += previous_symbol->add_symbols_to_follow( symbol->get_first() );
                ++j;
            }
        }
    }

    return added;
}

/**
// Set the index of this symbol.
//
// @param index
//  The value to set the index of this symbol to.
*/
void ParserSymbol::set_index( int index )
{
    index_ = index;
}

/**
// Get the index of this symbol.
//
// @return
//  The index of this symbol.
*/
int ParserSymbol::get_index() const
{
    return index_;
}

/**
// Describe this symbol.
//
// @param description
//  A variable to append the description to (assumed not null).
*/
void ParserSymbol::describe( std::string* description ) const
{
    SWEET_ASSERT( description );    
    description->append( get_identifier() );
}

/**
// Describe \e symbols.
//
// @param description
//  A variable to append the description to (assumed not null).
*/
void ParserSymbol::describe( const std::set<const ParserSymbol*>& symbols, std::string* description )
{
    SWEET_ASSERT( description );

    std::set<const ParserSymbol*>::const_iterator i = symbols.begin(); 
    if ( i != symbols.end() )
    {
        const ParserSymbol* symbol = *i;
        SWEET_ASSERT( symbol );        
        symbol->describe( description );
        ++i;        
    }
    
    while ( i != symbols.end() )
    {
        const ParserSymbol* symbol = *i;
        SWEET_ASSERT( symbol );
        description->append( ", " );
        symbol->describe( description );
        ++i;        
    }
}

/**
// Describe this symbol.
//
// @return
//  The description.
*/
std::string ParserSymbol::description() const
{
    std::string description;
    description.reserve( 1024 );
    describe( &description );
    return description;
}

/**
// Convert \e character into a name.
//
// @param character
//  The character to convert into a name.
//
// @return
//  The name of \e character.
*/
const char* ParserSymbol::name( int character ) const
{
    const char* NAMES[] =
    {
        "nul",
        "soh",
        "stx",
        "etx",
        "eot",
        "enq",
        "ack",
        "bel",
        "bs",
        "tab",
        "lf",
        "vt",
        "ff",
        "cr",
        "so",
        "si",
        "dle",
        "dc1",        
        "dc2",        
        "dc3",
        "dc4",
        "nak",
        "syn",
        "etb",
        "can",
        "em",
        "sub",
        "esc",
        "fs",
        "gs",
        "rs",
        "us",
        "space",
        "bang",
        "double_quote",
        "hash",
        "dollar",
        "percent",
        "amp",
        "single_quote",
        "left_paren",
        "right_paren",
        "star",
        "plus",
        "comma",
        "minus",
        "dot",
        "slash",
        "0",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "colon",
        "semi_colon",
        "lt",
        "eq",
        "gt",
        "question",
        "at",
        "A",
        "B",
        "C",
        "D",
        "E",
        "F",
        "G",
        "H",
        "I",
        "J",
        "K",
        "L",
        "M",
        "N",
        "O",
        "P",
        "Q",
        "R",
        "S",
        "T",
        "U",
        "V",
        "W",
        "X",
        "Y",
        "Z",
        "left_square_paren",
        "backslash",
        "right_square_paren",
        "hat",
        "underscore",
        "backtick",
        "a",
        "b",
        "c",
        "d",
        "e",
        "f",
        "g",
        "h",
        "i",
        "j",
        "k",
        "l",
        "m",
        "n",
        "o",
        "p",
        "q",
        "r",
        "s",
        "t",
        "u",
        "v",
        "w",
        "x",
        "y",
        "z",
        "left_curly_brace",
        "pipe",
        "right_curly_brace",
        "tilde",
        "del"
    };

    SWEET_ASSERT( character >= 0 && character < 128 );
    return NAMES[character];
}
