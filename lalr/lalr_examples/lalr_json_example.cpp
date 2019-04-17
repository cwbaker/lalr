
#include <string>
#include <vector>
#include <list>
#include <lalr/Parser.ipp>
#include <lalr/PositionIterator.hpp>
#include <string.h>

using namespace std;
using namespace lalr;

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

static void string_( PositionIterator<const char*>* begin, PositionIterator<const char*> end, std::string* lexeme, const void** /*symbol*/ )
{
    LALR_ASSERT( begin );
    LALR_ASSERT( lexeme );
    LALR_ASSERT( lexeme->length() == 1 );

    PositionIterator<const char*> position = *begin;
    int terminator = lexeme->at( 0 );
    LALR_ASSERT( terminator == '\'' || terminator == '"' );
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

static JsonUserData document( const JsonUserData* start, const ParserNode<char>* nodes, size_t length )
{
    return start[1];
}

static JsonUserData element( const JsonUserData* start, const ParserNode<char>* nodes, size_t length )
{
    shared_ptr<Element> element = start[3].element_;
    element->name_ = nodes[0].lexeme();
    return JsonUserData( element );
}

static JsonUserData attribute( const JsonUserData* start, const ParserNode<char>* nodes, size_t length )
{   
    shared_ptr<Attribute> attribute = start[2].attribute_;
    attribute->name_ = nodes[0].lexeme();
    return JsonUserData( attribute );
}

static JsonUserData value( const JsonUserData* start, const ParserNode<char>* nodes, size_t length )
{
    shared_ptr<Attribute> attribute( new Attribute(nodes[0].lexeme()) );
    return JsonUserData( attribute );
}

static JsonUserData content( const JsonUserData* start, const ParserNode<char>* nodes, size_t length )
{
    return start[0];
}

static JsonUserData add_to_element( const JsonUserData* start, const ParserNode<char>* nodes, size_t length )
{
    shared_ptr<Element> element = start[0].element_;   
    if ( start[2].attribute_ )
    {
        element->attributes_.push_back( start[2].attribute_ );
    }
    else if ( start[2].element_ )
    {
        element->elements_.push_back( start[2].element_ );
    }
    return JsonUserData( element );    
}

static JsonUserData create_element( const JsonUserData* start, const ParserNode<char>* nodes, size_t length )
{
    shared_ptr<Element> element( new Element() );
    if ( start[0].attribute_ )
    {
        element->attributes_.push_back( start[0].attribute_ );
    }
    else if ( start[0].element_ )
    {
        element->elements_.push_back( start[0].element_ );
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
    LALR_ASSERT( element );
    indent( level );
    printf( "%s\n", element->name_.c_str() );
    
    for ( list<shared_ptr<Attribute> >::const_iterator i = element->attributes_.begin(); i != element->attributes_.end(); ++i )
    {
        const Attribute* attribute = i->get();
        LALR_ASSERT( attribute );
        indent( level + 1 );
        printf( "%s='%s'\n", attribute->name_.c_str(), attribute->value_.c_str() );
    }
    
    for ( list<shared_ptr<Element> >::const_iterator i = element->elements_.begin(); i != element->elements_.end(); ++i )
    {
        const Element* element = i->get();
        LALR_ASSERT( element );
        print( element, level + 1 );
    }
}

void lalr_json_example()
{
    extern const lalr::ParserStateMachine* json_parser_state_machine;
    Parser<PositionIterator<const char*>, JsonUserData> parser( json_parser_state_machine );
    parser.set_lexer_action_handler( "string", &string_ );
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
    LALR_ASSERT( parser.accepted() );
    LALR_ASSERT( parser.full() );
    print( parser.user_data().element_.get(), 0 );
}
