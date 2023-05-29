
#include <string>
#include <vector>
#include <lalr/Parser.ipp>
#include <lalr/PositionIterator.hpp>
#include <lalr/string_literal.hpp>
#include <fstream>
#include <string.h>
#include <locale>
#include <clocale>
#include <codecvt>
#ifdef __APPLE__
#include <stdint.h>
#else
#include <uchar.h>
#endif

using namespace std;
using namespace lalr;

namespace 
{

typedef std::basic_string<char32_t> String;

struct Attribute;

struct Value
{
    String value_;
    std::vector<shared_ptr<Attribute>> attributes_;
    std::vector<shared_ptr<Value>> elements_;

    Value()
    : value_()
    , attributes_()
    , elements_()
    {
    }

    Value( const String& value )
    : value_( value )
    , attributes_()
    , elements_()
    {
    }
};

struct Attribute
{
    String name_;
    shared_ptr<Value> value_;

    Attribute()
    : name_()
    , value_()
    {
    }
    
    Attribute( const String& name, const shared_ptr<Value>& value )
    : name_( name )
    , value_( value )
    {
    }    
};

struct JsonUserData
{
    String name_;
    shared_ptr<Value> value_;
    
    JsonUserData()
    : name_()
    , value_()
    {
    }

    JsonUserData( shared_ptr<Value> value )
    : name_()
    , value_( value )
    {
    }    

    JsonUserData( const String& name, shared_ptr<Value> value )
    : name_( name )
    , value_( value )
    {
    }    
};

static JsonUserData document( const JsonUserData* start, const ParserNode<char32_t>* nodes, size_t length )
{
    return start[1];
}

static JsonUserData attribute( const JsonUserData* start, const ParserNode<char32_t>* nodes, size_t length )
{   
    const shared_ptr<Value>& attribute = start[2].value_;
    return JsonUserData( nodes[0].lexeme(), attribute );
}

static JsonUserData null( const JsonUserData* start, const ParserNode<char32_t>* nodes, size_t length )
{
    shared_ptr<Value> null_value = make_shared<Value>();
    return JsonUserData( null_value );
}

static JsonUserData value( const JsonUserData* start, const ParserNode<char32_t>* nodes, size_t length )
{
    shared_ptr<Value> value = make_shared<Value>( nodes[0].lexeme() );
    return JsonUserData( value );
}

static JsonUserData object( const JsonUserData* start, const ParserNode<char32_t>* nodes, size_t length )
{
    return start[1];
}

static JsonUserData add_to_object( const JsonUserData* start, const ParserNode<char32_t>* nodes, size_t length )
{
    const shared_ptr<Value>& object = start[0].value_;
    shared_ptr<Attribute> attribute = make_shared<Attribute>( start[2].name_, start[2].value_ );
    object->attributes_.push_back( attribute );
    return JsonUserData( object );
}

static JsonUserData create_object( const JsonUserData* start, const ParserNode<char32_t>* nodes, size_t length )
{
    shared_ptr<Value> object = make_shared<Value>();
    shared_ptr<Attribute> attribute = make_shared<Attribute>( start[0].name_, start[0].value_ );
    object->attributes_.push_back( attribute );
    return JsonUserData( object );
}

static JsonUserData add_to_array( const JsonUserData* start, const ParserNode<char32_t>* nodes, size_t length )
{
    const shared_ptr<Value>& array = start[0].value_;
    const shared_ptr<Value>& element = start[2].value_;
    array->elements_.push_back( element );
    return JsonUserData( array );
}

static JsonUserData create_array( const JsonUserData* start, const ParserNode<char32_t>* nodes, size_t length )
{
    shared_ptr<Value> array = make_shared<Value>();
    const shared_ptr<Value>& element = start[0].value_;
    array->elements_.push_back( element );
    return JsonUserData( array );
}

static void indent( int level )
{
    for ( int i = 0; i < level; ++i )
    {
        printf( "  " );
    }    
}

static void print( const Value& value, int level )
{
    std::setlocale( LC_ALL, "en_US.UTF-8" );
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf8;

    for ( const shared_ptr<Attribute>& attribute : value.attributes_ )
    {
        LALR_ASSERT( attribute );
        const String& name = attribute->name_;
        const Value& value = *attribute->value_;
        if ( value.attributes_.empty() && value.elements_.empty() )
        {
            indent( level + 1 );
            printf( "%s='%s'\n", 
                utf8.to_bytes(attribute->name_).c_str(),
                utf8.to_bytes(attribute->value_->value_).c_str()
            );
        }
        else
        {
            indent( level + 1 );
            printf( "%s:\n",
                utf8.to_bytes(attribute->name_).c_str()
            );
            print( value, level + 1 );
        }
    }

    int index = 0;
    for ( const shared_ptr<Value>& element : value.elements_ )
    {
        LALR_ASSERT( element );
        const Value& value = *element;
        if ( value.attributes_.empty() && value.elements_.empty() )
        {
            indent( level + 1 );
            printf( "%d: '%s'\n",
                index,
                utf8.to_bytes(value.value_).c_str()
            );
        }
        else
        {
            indent( level + 1 );
            printf( "%d:\n", index );
            print( value, level + 1 );
        }
        ++index;
    }
}

}

void lalr_json_example()
{
    extern const lalr::ParserStateMachine* json_parser_state_machine;
    Parser<istreambuf_iterator<char32_t>, JsonUserData> parser( json_parser_state_machine );
    parser.set_lexer_action_handler( "string", &string_literal<istreambuf_iterator<char32_t>> );
    parser.parser_action_handlers()
        ( "document", &document )
        ( "add_to_object", &add_to_object )
        ( "create_object", &create_object )
        ( "attribute", &attribute )
        ( "add_to_array", &add_to_array )
        ( "create_array", &create_array )
        ( "null", &null )
        ( "value", &value )
        ( "object", &object )
        ( "array", &object )
    ;

    using std::locale;
    using std::codecvt;
    using std::basic_ifstream;
    using std::istreambuf_iterator;
    std::basic_ifstream<char32_t> file( LALR_EXAMPLES "lalr_json_example.json", std::ios_base::binary );
    file.imbue( locale(file.getloc(), new codecvt<char32_t, char, std::mbstate_t>) );
    istreambuf_iterator<char32_t> input( file );
    istreambuf_iterator<char32_t> input_end;

    parser.parse( input, input_end );
    LALR_ASSERT( parser.accepted() );
    LALR_ASSERT( parser.full() );
    print( *parser.user_data().value_, 0 );
}
