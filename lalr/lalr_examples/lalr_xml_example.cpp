
#include <lalr/Parser.ipp>
#include <list>
#include <stdio.h>
#include <string.h>

using namespace std;
using namespace lalr;

struct Attribute
{
    std::string name_;
    std::string value_;
    
    Attribute( const std::string& name, const std::string& value )
    : name_( name ),
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

static void string_( const char** begin, const char* end, std::string* lexeme, const void** symbol )
{
    LALR_ASSERT( begin && *begin );
    LALR_ASSERT( end );
    LALR_ASSERT( lexeme );
    LALR_ASSERT( lexeme->length() == 1 );
    LALR_ASSERT( symbol );

    const char* position = *begin;
    int terminator = lexeme->at( 0 );
    LALR_ASSERT( terminator == '\'' || terminator == '"' );
    lexeme->clear();
    
    while ( *position != terminator && position < end )
    {
        *lexeme += *position;
        ++position;
    }
    
    *begin = position < end ? position + 1 : position;
}

static XmlUserData document( const ParserNode<XmlUserData, char>* start, const ParserNode<XmlUserData, char>* finish )
{
    while ( start != finish && !start->user_data().element_ )
    {
        ++start;
    }    
    return start != finish ? start->user_data() : XmlUserData();
}

static XmlUserData add_element( const ParserNode<XmlUserData, char>* start, const ParserNode<XmlUserData, char>* finish )
{
    shared_ptr<Element> element = start[0].user_data().element_;
    element->elements_.push_back( start[1].user_data().element_ );
    return XmlUserData( element );
}

static XmlUserData create_element( const ParserNode<XmlUserData, char>* start, const ParserNode<XmlUserData, char>* finish )
{
    shared_ptr<Element> element( new Element() );
    element->elements_.push_back( start[0].user_data().element_ );
    return XmlUserData( element );
}

static XmlUserData short_element( const ParserNode<XmlUserData, char>* start, const ParserNode<XmlUserData, char>* finish )
{
    shared_ptr<Element> element = start[2].user_data().element_;
    element->name_ = start[1].lexeme();
    return XmlUserData( element );
}

static XmlUserData long_element( const ParserNode<XmlUserData, char>* start, const ParserNode<XmlUserData, char>* finish )
{
    shared_ptr<Element> element = start[2].user_data().element_;
    if ( !element )
    {
        element.reset( new Element() );
    }

    element->name_ = start[1].lexeme();
    if ( start[4].user_data().element_ )
    {
        swap( element->elements_, start[4].user_data().element_->elements_ );
    }
    return XmlUserData( element );
}

static XmlUserData add_attribute( const ParserNode<XmlUserData, char>* start, const ParserNode<XmlUserData, char>* finish )
{
    LALR_ASSERT( start[0].user_data().element_ );
    shared_ptr<Element> element = start[0].user_data().element_;
    LALR_ASSERT( start[1].user_data().attribute_ );
    element->attributes_.push_back( start[1].user_data().attribute_ );
    return XmlUserData( element );
}

static XmlUserData create_attribute( const ParserNode<XmlUserData, char>* start, const ParserNode<XmlUserData, char>* finish )
{
    LALR_ASSERT( start[0].user_data().attribute_ );
    shared_ptr<Element> element( new Element() );
    element->attributes_.push_back( start[0].user_data().attribute_ );
    return XmlUserData( element );
}

static XmlUserData attribute( const ParserNode<XmlUserData, char>* start, const ParserNode<XmlUserData, char>* finish )
{
    shared_ptr<Attribute> attribute( new Attribute(start[0].lexeme(), start[2].lexeme()) );
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

void lalr_xml_example()
{
    extern const lalr::ParserStateMachine* xml_parser_state_machine;
    Parser<const char*, XmlUserData> parser( xml_parser_state_machine );
    parser.lexer_action_handlers()
        ( "string", &string_ )
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
    
    parser.parse( input, input + strlen(input) );
    LALR_ASSERT( parser.accepted() );
    LALR_ASSERT( parser.full() );
    print( parser.user_data().element_.get(), 0 );
}
