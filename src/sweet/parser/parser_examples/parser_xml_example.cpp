
#include <sweet/parser/Grammar.hpp>
#include <sweet/parser/ParserStateMachine.hpp>
#include <sweet/parser/Parser.ipp>
#include <list>
#include <stdio.h>
#include <string.h>

using namespace std;
using namespace sweet;
using namespace sweet::parser;

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
    SWEET_ASSERT( begin && *begin );
    SWEET_ASSERT( end );
    SWEET_ASSERT( lexeme );
    SWEET_ASSERT( lexeme->length() == 1 );
    SWEET_ASSERT( symbol );

    const char* position = *begin;
    int terminator = lexeme->at( 0 );
    SWEET_ASSERT( terminator == '\'' || terminator == '"' );
    lexeme->clear();
    
    while ( *position != terminator && position < end )
    {
        *lexeme += *position;
        ++position;
    }
    
    *begin = position < end ? position + 1 : position;
}

static XmlUserData document( const ParserSymbol* symbol, const ParserNode<XmlUserData, char>* start, const ParserNode<XmlUserData, char>* finish )
{
    while ( start != finish && !start->get_user_data().element_ )
    {
        ++start;
    }    
    return start != finish ? start->get_user_data() : XmlUserData();
}

static XmlUserData add_element( const ParserSymbol* symbol, const ParserNode<XmlUserData, char>* start, const ParserNode<XmlUserData, char>* finish )
{
    shared_ptr<Element> element = start[0].get_user_data().element_;
    element->elements_.push_back( start[1].get_user_data().element_ );
    return XmlUserData( element );
}

static XmlUserData create_element( const ParserSymbol* symbol, const ParserNode<XmlUserData, char>* start, const ParserNode<XmlUserData, char>* finish )
{
    shared_ptr<Element> element( new Element() );
    element->elements_.push_back( start[0].get_user_data().element_ );
    return XmlUserData( element );
}

static XmlUserData short_element( const ParserSymbol* symbol, const ParserNode<XmlUserData, char>* start, const ParserNode<XmlUserData, char>* finish )
{
    shared_ptr<Element> element = start[2].get_user_data().element_;
    element->name_ = start[1].get_lexeme();
    return XmlUserData( element );
}

static XmlUserData long_element( const ParserSymbol* symbol, const ParserNode<XmlUserData, char>* start, const ParserNode<XmlUserData, char>* finish )
{
    shared_ptr<Element> element = start[2].get_user_data().element_;
    if ( !element )
    {
        element.reset( new Element() );
    }

    element->name_ = start[1].get_lexeme();
    if ( start[4].get_user_data().element_ )
    {
        swap( element->elements_, start[4].get_user_data().element_->elements_ );
    }
    return XmlUserData( element );
}

static XmlUserData add_attribute( const ParserSymbol* symbol, const ParserNode<XmlUserData, char>* start, const ParserNode<XmlUserData, char>* finish )
{
    SWEET_ASSERT( start[0].get_user_data().element_ );
    shared_ptr<Element> element = start[0].get_user_data().element_;
    SWEET_ASSERT( start[1].get_user_data().attribute_ );
    element->attributes_.push_back( start[1].get_user_data().attribute_ );
    return XmlUserData( element );
}

static XmlUserData create_attribute( const ParserSymbol* symbol, const ParserNode<XmlUserData, char>* start, const ParserNode<XmlUserData, char>* finish )
{
    SWEET_ASSERT( start[0].get_user_data().attribute_ );
    shared_ptr<Element> element( new Element() );
    element->attributes_.push_back( start[0].get_user_data().attribute_ );
    return XmlUserData( element );
}

static XmlUserData attribute( const ParserSymbol* symbol, const ParserNode<XmlUserData, char>* start, const ParserNode<XmlUserData, char>* finish )
{
    shared_ptr<Attribute> attribute( new Attribute(start[0].get_lexeme(), start[2].get_lexeme()) );
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

void parser_xml_example()
{
    Grammar grammar;
    grammar.begin()
        .whitespace() ("[ \t\r\n]*")
        .left() ('<') ('>')
        .left() ("name")

        .production( "document" )
            ("prolog") ("element") ["document"]
        .end_production()

        .production( "prolog" )
            ("<\\?xml") ("attributes") ("\\?>") [nil]
            (nil)
        .end_production()

        .production( "elements" )
            ("elements") ("element") ["add_element"]
            ("element") ["create_element"] 
            (nil) .precedence ('<')
        .end_production()

        .production( "element" )
            ('<') ("name") ("attributes") ("/>") ["short_element"]
            ('<') ("name") ("attributes") ('>') ("elements") ("</") ("name") ('>') ["long_element"]
        .end_production()

        .production( "attributes" )
            ("attributes") ("attribute") ["add_attribute"]
            ("attribute") ["create_attribute"] 
            (nil) .precedence ("name")
        .end_production()

        .production( "attribute" )
            ("name") ('=') ("value") ["attribute"]
        .end_production()

        .production( "name" )
            ("[A-Za-z_:][A-Za-z0-9_:\\.-]*")
        .end_production()

        .production( "value" )
            ("[\\\"']:string:")
        .end_production()
    .end();

    ParserStateMachine parser_state_machine( grammar );
    Parser<const char*, XmlUserData> parser( &parser_state_machine );
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
    SWEET_ASSERT( parser.accepted() );
    SWEET_ASSERT( parser.full() );
    print( parser.user_data().element_.get(), 0 );
}
