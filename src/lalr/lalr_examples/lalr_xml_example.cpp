
#include <lalr/Parser.ipp>
#include <lalr/string_literal.hpp>
#include <list>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

using namespace std;
using namespace lalr;

namespace
{

typedef std::basic_string<uint8_t> String;

struct Attribute
{
    String name_;
    String value_;
    
    Attribute( const String& name, const String& value )
    : name_( name )
    , value_( value )
    {
    }    
};

struct Element
{
    String name_;
    std::list<shared_ptr<Attribute> > attributes_;
    std::list<shared_ptr<Element> > elements_;
    
    Element()
    : name_(),
      attributes_(),
      elements_()
    {
    }
};

struct XmlUserData
{
    shared_ptr<Attribute> attribute_;
    shared_ptr<Element> element_;
    
    XmlUserData()
    : attribute_(),
      element_()
    {
    }
    
    XmlUserData( shared_ptr<Attribute> attribute )
    : attribute_( attribute ),
      element_()
    {
    }    
    
    XmlUserData( shared_ptr<Element> element )
    : attribute_(),
      element_( element )
    {
    }    
};

static XmlUserData document( const XmlUserData* start, const ParserNode<uint8_t>* nodes, size_t length )
{
    const XmlUserData* end = start + length;
    while ( start != end && !start[0].element_ )
    {
        ++start;
    }    
    return start != end ? start[0] : XmlUserData();
}

static XmlUserData add_element( const XmlUserData* start, const ParserNode<uint8_t>* nodes, size_t length )
{
    shared_ptr<Element> element = start[0].element_;
    element->elements_.push_back( start[1].element_ );
    return XmlUserData( element );
}

static XmlUserData create_element( const XmlUserData* start, const ParserNode<uint8_t>* nodes, size_t length )
{
    shared_ptr<Element> element( new Element() );
    element->elements_.push_back( start[0].element_ );
    return XmlUserData( element );
}

static XmlUserData short_element( const XmlUserData* start, const ParserNode<uint8_t>* nodes, size_t length )
{
    shared_ptr<Element> element = start[2].element_;
    element->name_ = nodes[1].lexeme();
    return XmlUserData( element );
}

static XmlUserData long_element( const XmlUserData* start, const ParserNode<uint8_t>* nodes, size_t length )
{
    shared_ptr<Element> element = start[2].element_;
    if ( !element )
    {
        element.reset( new Element() );
    }

    element->name_ = nodes[1].lexeme();
    if ( start[4].element_ )
    {
        swap( element->elements_, start[4].element_->elements_ );
    }
    return XmlUserData( element );
}

static XmlUserData add_attribute( const XmlUserData* start, const ParserNode<uint8_t>* nodes, size_t length )
{
    LALR_ASSERT( start[0].element_ );
    shared_ptr<Element> element = start[0].element_;
    LALR_ASSERT( start[1].attribute_ );
    element->attributes_.push_back( start[1].attribute_ );
    return XmlUserData( element );
}

static XmlUserData create_attribute( const XmlUserData* start, const ParserNode<uint8_t>* nodes, size_t length )
{
    LALR_ASSERT( start[0].attribute_ );
    shared_ptr<Element> element( new Element() );
    element->attributes_.push_back( start[0].attribute_ );
    return XmlUserData( element );
}

static XmlUserData attribute( const XmlUserData* start, const ParserNode<uint8_t>* nodes, size_t length )
{
    shared_ptr<Attribute> attribute( new Attribute(nodes[0].lexeme(), nodes[2].lexeme()) );
    return XmlUserData( attribute );
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

}

void lalr_xml_example()
{
    extern const lalr::ParserStateMachine* xml_parser_state_machine;
    Parser<const uint8_t*, XmlUserData> parser( xml_parser_state_machine );
    parser.lexer_action_handlers()
        ( "string", &string_literal<const uint8_t*> )
    ;
    parser.parser_action_handlers()
        ( "document", &document )
        ( "add_element", &add_element )
        ( "create_element", &create_element )
        ( "short_element", &short_element )
        ( "long_element", &long_element )
        ( "add_attribute", &add_attribute )
        ( "create_attribute", &create_attribute )
        ( "attribute", &attribute )
    ;

    const char* input = 
        "<?xml version='1.0' encoding='UTF-8' standalone='yes'?>\n"
        "   <document name='example'>\n"
        "       <paragraph id='1.1'/>\n"
        "       <paragraph id='1.2'/>\n"
        "       <paragraph id='1.3'></paragraph>\n"
        "   </document>"
    ;
    
    parser.parse( (const uint8_t*) input, (const uint8_t*) input + strlen(input) );
    LALR_ASSERT( parser.accepted() );
    LALR_ASSERT( parser.full() );
    print( parser.user_data().element_.get(), 0 );
}
