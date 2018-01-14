
#include <string>
#include <vector>
#include <list>
#include <sweet/build.hpp>
#include <sweet/assert/assert.hpp>
#include "parser.hpp"
#include "json.hpp"
#include <string.h>

using namespace std;
using namespace sweet;
using namespace sweet::parser;

struct Attribute
{
    std::string name_;
    std::string value_;
    
    Attribute( const std::string& value )
    : name_(),
      value_( value )
    {
    }    
};

struct Element
{
    std::string name_;
    std::list<shared_ptr<Attribute> > attributes_;
    std::list<shared_ptr<Element> > elements_;
    
    Element()
    : name_(),
      attributes_(),
      elements_()
    {
    }
};

struct JsonUserData
{
    shared_ptr<Attribute> attribute_;
    shared_ptr<Element> element_;
    
    JsonUserData()
    : attribute_(),
      element_()
    {
    }
    
    JsonUserData( shared_ptr<Attribute> attribute )
    : attribute_( attribute ),
      element_()
    {
    }    
    
    JsonUserData( shared_ptr<Element> element )
    : attribute_(),
      element_( element )
    {
    }    
};

static void string_( PositionIterator<const char*>* begin, PositionIterator<const char*> end, std::string* lexeme, int* symbol )
{
    SWEET_ASSERT( begin );
    SWEET_ASSERT( lexeme );
    SWEET_ASSERT( lexeme->length() == 1 );
    SWEET_ASSERT( symbol );

    PositionIterator<const char*> position = *begin;
    int terminator = lexeme->at( 0 );
    SWEET_ASSERT( terminator == '\'' || terminator == '"' );
    lexeme->clear();
    
    while ( *position != terminator && position != end )
    {
        *lexeme += *position;
        ++position;
    }

    if ( position != end )
    {
        ++position;
    }

    *begin = position;
}

static JsonUserData document( int symbol, const ParserNode<JsonUserData, char>* start, const ParserNode<JsonUserData, char>* finish )
{
    return start[1].user_data_;
}

static JsonUserData element( int symbol, const ParserNode<JsonUserData, char>* start, const ParserNode<JsonUserData, char>* finish )
{
    shared_ptr<Element> element = start[3].user_data_.element_;
    element->name_ = start[0].lexeme_;
    return JsonUserData( element );
}

static JsonUserData attribute( int symbol, const ParserNode<JsonUserData, char>* start, const ParserNode<JsonUserData, char>* finish )
{   
    shared_ptr<Attribute> attribute = start[2].user_data_.attribute_;
    attribute->name_ = start[0].lexeme_;
    return JsonUserData( attribute );
}

static JsonUserData value( int symbol, const ParserNode<JsonUserData, char>* start, const ParserNode<JsonUserData, char>* finish )
{
    shared_ptr<Attribute> attribute( new Attribute(start[0].lexeme_) );
    return JsonUserData( attribute );
}

static JsonUserData content( int symbol, const ParserNode<JsonUserData, char>* start, const ParserNode<JsonUserData, char>* finish )
{
    return start[0].user_data_;
}

static JsonUserData add_to_element( int symbol, const ParserNode<JsonUserData, char>* start, const ParserNode<JsonUserData, char>* finish )
{
    shared_ptr<Element> element = start[0].user_data_.element_;   
    if ( start[2].user_data_.attribute_ )
    {
        element->attributes_.push_back( start[2].user_data_.attribute_ );
    }
    else if ( start[2].user_data_.element_ )
    {
        element->elements_.push_back( start[2].user_data_.element_ );
    }
    return JsonUserData( element );    
}

static JsonUserData create_element( int symbol, const ParserNode<JsonUserData, char>* start, const ParserNode<JsonUserData, char>* finish )
{
    shared_ptr<Element> element( new Element() );
    if ( start[0].user_data_.attribute_ )
    {
        element->attributes_.push_back( start[0].user_data_.attribute_ );
    }
    else if ( start[0].user_data_.element_ )
    {
        element->elements_.push_back( start[0].user_data_.element_ );
    }
    return JsonUserData( element );    
}

static void indent( int level )
{
    for ( int i = 0; i < level; ++i )
    {
        printf( "  " );
    }    
}

static void print( const Element* element, int level )
{
    SWEET_ASSERT( element );
    indent( level );
    printf( "%s\n", element->name_.c_str() );
    
    for ( list<shared_ptr<Attribute> >::const_iterator i = element->attributes_.begin(); i != element->attributes_.end(); ++i )
    {
        const Attribute* attribute = i->get();
        SWEET_ASSERT( attribute );
        indent( level + 1 );
        printf( "%s='%s'\n", attribute->name_.c_str(), attribute->value_.c_str() );
    }
    
    for ( list<shared_ptr<Element> >::const_iterator i = element->elements_.begin(); i != element->elements_.end(); ++i )
    {
        const Element* element = i->get();
        SWEET_ASSERT( element );
        print( element, level + 1 );
    }
}

void parser_json_example()
{
    Parser<PositionIterator<const char*>, JsonUserData> parser( &json_parser_state_machine );
    parser.lexer_action_handlers()
        ( "string", &string_ )
    ;
    parser.parser_action_handlers()
        ( "document", &document )
        ( "element", &element )
        ( "attribute", &attribute )
        ( "value", &value )
        ( "add_to_element", &add_to_element )
        ( "create_element", &create_element )
        ( "content", &content )
    ;

    const char* input =
    "{\n"
    "    \"model\": {\n"
    "        \"format\": \"Model\",\n"
    "        \"version\": 1,\n"
    "        \"address\": \"0017FAB0\",\n"
    "        \"items\": {\n"
    "            \"name\": \"one\"\n"
    "        }\n"
    "    }\n"
    "}\n";

    parser.parse( PositionIterator<const char*>(input, input + strlen(input)), PositionIterator<const char*>() );
    SWEET_ASSERT( parser.accepted() );
    SWEET_ASSERT( parser.full() );
    print( parser.user_data().element_.get(), 0 );
}
