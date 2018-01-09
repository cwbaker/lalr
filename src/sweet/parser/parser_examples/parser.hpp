
#ifndef PARSER_HPP_INCLUDED
#define PARSER_HPP_INCLUDED

#include <stdarg.h>
#include <iterator>
#include <algorithm>
#include <stdio.h>

namespace sweet
{

namespace parser
{

/**
// An iterator wrapper that adds line number tracking to iterators or 
// pointers.
*/
template <class Iterator>
class PositionIterator
{
    public:
        typedef std::forward_iterator_tag                                iterator_category; ///< The type of this iterator (always forward regardless of the underlying iterator).
        typedef typename std::iterator_traits<Iterator>::difference_type difference_type;   ///< The type to represent the difference between two iterators.
        typedef typename std::iterator_traits<Iterator>::value_type	     value_type;        ///< The type of the element the iterator can point to.
        typedef typename std::iterator_traits<Iterator>::pointer         pointer;           ///< The type of a pointer to an element the iterator can point to.
        typedef typename std::iterator_traits<Iterator>::reference       reference;         ///< Type to represent a reference to an element pointed by the iterator.

    private:
        Iterator position_;
        Iterator end_;
        bool     ended_;
        int      line_;
    
    public:
        PositionIterator()
        : position_(),
          end_(),
          ended_( true ),
          line_( 1 )
        {
        }
    
        PositionIterator( Iterator begin, Iterator end )
        : position_( begin ),
          end_( end ),
          ended_( begin == end ),
          line_( 1 )
        {
        }
        
        
        PositionIterator( const PositionIterator& iterator )
        : position_( iterator.position_ ),
          end_( iterator.end_ ),
          ended_( iterator.ended_ ),
          line_( iterator.line_ )
        {
        }
        
        
        PositionIterator& operator=( const PositionIterator& iterator )
        {
            if ( this != &iterator )
            {
                position_ = iterator.position_;
                end_ = iterator.end_;
                ended_ = iterator.ended_;
                line_ = iterator.line_;
            }
            
            return *this;
        }
        
        
        PositionIterator& operator++()
        {
            int character = *position_;
            ++position_;
            if ( character == '\n' || character == '\r' && (position_ == end_ || *position_ != '\n') )
            {
                ++line_;
            }
            
            ended_ = position_ == end_;
            return *this;
        }
        
        
        value_type operator*() const
        {
            return *position_;
        }
        
        
        int line() const
        {
            return line_;
        }
        
        
        bool operator!=( const PositionIterator& iterator ) const
        {
            return ended_ || iterator.ended_ ? ended_ != iterator.ended_ : position_ != iterator.position_;
        }
        
        
        bool operator==( const PositionIterator& iterator ) const
        {
            return ended_ || iterator.ended_ ? ended_ == iterator.ended_ : position_ == iterator.position_;
        }
};


/**
// A custom action in a lexical analyzer.
*/
struct LexerAction
{
    int         index_;
    const char* identifier_;
};


/**
// A transition between states in a lexical analyzer.
*/
struct LexerTransition 
{
    int                      begin_;
    int                      end_;
    const struct LexerState* state_;
    const LexerAction*       action_;
};


/**
// A state in a lexical analyzer.
*/
struct LexerState
{
    int                    symbol_;
    const LexerTransition* transitions_;
    const LexerTransition* transitions_end_;
};


/**
// The data used by a lexical analyzer.
*/
struct LexerStateMachine
{
    const LexerState*  start_state_;
    const LexerState*  whitespace_start_state_;
    const LexerAction* actions_;
    const LexerAction* actions_end_;
};


/**
// A lexical analyzer.
*/
template <class Iterator, class Char = typename std::iterator_traits<Iterator>::value_type, class Traits = typename std::char_traits<Char>, class Allocator = typename std::allocator<Char> >
class Lexer
{
    typedef void (*LexerActionFunction)( Iterator* begin, Iterator end, std::basic_string<Char, Traits, Allocator>* lexeme, int* symbol );

    struct LexerActionHandler
    {
        const LexerAction*  action_;
        LexerActionFunction function_;        
        LexerActionHandler( const LexerAction* action, LexerActionFunction function );
    };

    const LexerStateMachine*                    state_machine_;
    int                                         end_symbol_;
    std::vector<LexerActionHandler>             action_handlers_;
    Iterator                                    position_;
    Iterator                                    end_;
    std::basic_string<Char, Traits, Allocator>  lexeme_;
    int                                         symbol_;
    bool                                        full_;

    public:
        Lexer( const LexerStateMachine* state_machine, int end_symbol );
        void set_action_handler( const char* identifier, LexerActionFunction function );
        const std::basic_string<Char, Traits, Allocator>& lexeme() const;
        int symbol() const;
        const Iterator& position() const;
        bool full() const;
        void reset( Iterator start, Iterator finish );
        void advance();
        void recover();
        
    private:
        void skip();
        int run();
        const LexerTransition* find_transition_by_character( const LexerState* state, int character ) const;
        bool is_on_character( const LexerTransition* transition, int character ) const;
        
};


struct ParserSymbol
{
    int         index_;
    const char* identifier_;
};


struct ParserAction
{
    int         index_;
    const char* identifier_;
};


struct ParserProduction
{
    int                 symbol_;
    const ParserAction* action_;
    int                 length_;
};


enum ParserTransitionType
{
    TRANSITION_SHIFT,
    TRANSITION_REDUCE
};


struct ParserState;
struct ParserTransition
{
    int                         symbol_;
    ParserTransitionType        type_;
    const struct ParserState*   state_;
    const ParserProduction*     reduced_production_;
};


struct ParserState
{
    const ParserTransition* transitions_;
    const ParserTransition* transitions_end_;
};


struct ParserStateMachine
{
    const ParserState*       start_state_;
    const LexerStateMachine* lexer_state_machine_;
    int                      start_symbol_;
    int                      end_symbol_;
    int                      error_symbol_;
    const ParserAction*      actions_;
    const ParserAction*      actions_end_;
};


template <class Iterator, class UserData, class Char, class Traits, class Allocator> class Parser;
template <class Iterator, class UserData, class Char = typename std::iterator_traits<Iterator>::value_type, class Traits = typename std::char_traits<Char>, class Allocator = typename std::allocator<Char> >
class ParserEventSink
{
    public:
        virtual ~ParserEventSink();
        virtual void parser_error( const Parser<Iterator, UserData, Char, Traits, Allocator>* parser, const char* format, va_list args );
        virtual void parser_vprintf( const char* format, va_list args );
};


template <class UserData, class Char, class Traits = std::char_traits<Char>, class Allocator = std::allocator<Char> >
struct ParserNode
{
    const ParserState*                          state_;
    int                                         symbol_;
    std::basic_string<Char, Traits, Allocator>  lexeme_;
    UserData                                    user_data_;

    public:
        ParserNode( const ParserState* state, int symbol, const UserData& user_data );
        ParserNode( const ParserState* state, int symbol, const std::basic_string<Char, Traits, Allocator>& lexeme );
};


template <class Iterator, class Char, class Traits, class Allocator>
class AddLexerActionHandler
{
    typedef void (*LexerActionFunction)( Iterator* begin, Iterator end, std::basic_string<Char, Traits, Allocator>* lexeme, int* symbol );

    Lexer<Iterator, Char, Traits, Allocator>* lexer_;

    public:
        AddLexerActionHandler( Lexer<Iterator, Char, Traits, Allocator>* lexer );
        const AddLexerActionHandler& operator()( const char* identifier, LexerActionFunction function ) const;
};


/**
// A helper that provides a convenient syntax for adding handlers to a Parser.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
class AddParserActionHandler
{
    typedef UserData (*ParserActionFunction)( int symbol, const ParserNode<UserData, Char, Traits, Allocator>* start, const ParserNode<UserData, Char, Traits, Allocator>* finish );

    Parser<Iterator, UserData, Char, Traits, Allocator>* parser_; ///< The Parser to add handlers to.

    public:
        AddParserActionHandler( Parser<Iterator, UserData, Char, Traits, Allocator>* parser );
        const AddParserActionHandler& default_action( ParserActionFunction function ) const;
        const AddParserActionHandler& operator()( const char* identifier, ParserActionFunction function ) const;
};


/**
// A parser.
*/
template <class Iterator, class UserData, class Char = typename std::iterator_traits<Iterator>::value_type, class Traits = typename std::char_traits<Char>, class Allocator = typename std::allocator<Char> >
class Parser
{
    public:
        typedef ParserNode<UserData, Char, Traits, Allocator> ParserNode;
        typedef typename std::vector<ParserNode>::const_iterator ParserNodeConstIterator;
        typedef void (*LexerActionFunction)( Iterator* begin, Iterator end, std::basic_string<Char, Traits, Allocator>* lexeme, int* symbol );
        typedef UserData (*ParserActionFunction)( int symbol, const ParserNode* start, const ParserNode* finish );

    private:
        struct ParserActionHandler
        {        
            const ParserAction*  action_;
            ParserActionFunction function_;            

            ParserActionHandler( const ParserAction* action, ParserActionFunction function );
        };

        const ParserStateMachine*                                     state_machine_;
        ParserEventSink<Iterator, UserData, Char, Traits, Allocator>* event_sink_;
        std::vector<ParserNode>                                       nodes_;
        Lexer<Iterator, Char, Traits, Allocator>                      lexer_;
        std::vector<ParserActionHandler>                              action_handlers_;
        ParserActionFunction                                          default_action_handler_;
        bool                                                          debug_enabled_;
        bool                                                          accepted_;
        bool                                                          full_;

    public:
        Parser( const ParserStateMachine* data, ParserEventSink<Iterator, UserData, Char, Traits, Allocator>* event_sink = NULL );
        void parse( Iterator start, Iterator finish );
        bool accepted() const;
        bool full() const;
        const UserData& user_data() const;
        const Iterator& position() const;

        AddLexerActionHandler<Iterator, Char, Traits, Allocator> lexer_action_handlers();
        AddParserActionHandler<Iterator, UserData, Char, Traits, Allocator> parser_action_handlers();
        void set_default_action_handler( ParserActionFunction function );
        void set_action_handler( const char* identifier, ParserActionFunction function );
        void fire_error( const char* format, ... ) const;
        void fire_printf( const char* format, ... ) const;
        
        void set_debug_enabled( bool debug_enabled );
        bool is_debug_enabled() const;
        
    private:
        const ParserTransition* find_transition( int symbol, const ParserState* state ) const;
        typename std::vector<ParserNode>::iterator find_node_to_reduce_to( const ParserProduction* production, std::vector<ParserNode>& nodes );
        typename std::vector<ParserNode>::const_iterator find_start_node( const std::vector<ParserNode>& nodes ) const;
        void debug_shift( const ParserNode& node ) const;
        void debug_reduce( const ParserProduction* reduced_production, const ParserNode* start, const ParserNode* finish ) const;
        UserData handle( const ParserProduction* reduced_production, const ParserNode* start, const ParserNode* finish ) const;
        bool parse( const ParserState* parser_start_state );
        void shift( const ParserTransition* transition );
        void reduce( const ParserTransition* transition, bool* accepted, bool* rejected );
        void error( bool* accepted, bool* rejected );
        const ParserTransition* find_transition_by_symbol( const ParserState* state, int symbol ) const;
        bool is_on_symbol( const ParserTransition* transition, int symbol ) const;
        bool is_production_in_kernel( const ParserNode& node, const ParserProduction* production ) const;
};


template <class Iterator, class Char, class Traits, class Allocator>
Lexer<Iterator, Char, Traits, Allocator>::LexerActionHandler::LexerActionHandler( const LexerAction* action, LexerActionFunction function )
: action_( action ),
  function_( function )
{
    SWEET_ASSERT( action_ != NULL );
}


template <class Iterator, class Char, class Traits, class Allocator>
Lexer<Iterator, Char, Traits, Allocator>::Lexer( const LexerStateMachine* state_machine, int end_symbol )
: state_machine_( state_machine ),
  end_symbol_( end_symbol ),
  action_handlers_(),
  position_(),
  end_(),
  lexeme_(),
  symbol_( 0 ),
  full_( false )
{
    SWEET_ASSERT( state_machine_ );
    SWEET_ASSERT( state_machine_->actions_ );
    SWEET_ASSERT( state_machine_->actions_end_ );
    SWEET_ASSERT( state_machine_->actions_ <= state_machine_->actions_end_ );

    const LexerAction* actions = state_machine_->actions_;
    const LexerAction* actions_end = state_machine_->actions_end_;
    action_handlers_.reserve( actions_end - actions );
    for ( const LexerAction* action = actions; action != actions_end; ++action )
    {
        action_handlers_.push_back( LexerActionHandler(action, NULL) );
    }    
}


template <class Iterator, class Char, class Traits, class Allocator>
void 
Lexer<Iterator, Char, Traits, Allocator>::set_action_handler( const char* identifier, LexerActionFunction function )
{
    SWEET_ASSERT( identifier != NULL );
    
    typename std::vector<LexerActionHandler>::iterator action_handler = action_handlers_.begin();
    while ( action_handler != action_handlers_.end() && strcmp(action_handler->action_->identifier_, identifier) != 0 )
    {
        ++action_handler;
    }
    
    if ( action_handler != action_handlers_.end() )
    {
        action_handler->function_ = function;
    }
}


template <class Iterator, class Char, class Traits, class Allocator>
const std::basic_string<Char, Traits, Allocator>&
Lexer<Iterator, Char, Traits, Allocator>::lexeme() const
{
    return lexeme_;
}


template <class Iterator, class Char, class Traits, class Allocator>
int
Lexer<Iterator, Char, Traits, Allocator>::symbol() const
{
    return symbol_;
}


template <class Iterator, class Char, class Traits, class Allocator>
const Iterator&
Lexer<Iterator, Char, Traits, Allocator>::position() const
{
    return position_;
}


template <class Iterator, class Char, class Traits, class Allocator>
bool 
Lexer<Iterator, Char, Traits, Allocator>::full() const
{
    return full_;
}


template <class Iterator, class Char, class Traits, class Allocator>
void 
Lexer<Iterator, Char, Traits, Allocator>::reset( Iterator start, Iterator finish )
{
    position_ = start;
    end_ = finish;
    lexeme_.clear();
    symbol_ = 0;
    full_ = false;
}


template <class Iterator, class Char, class Traits, class Allocator>
void 
Lexer<Iterator, Char, Traits, Allocator>::advance()
{
    lexeme_.clear();
    skip();
    full_ = position_ == end_;
    symbol_ = position_ != end_ ? run() : end_symbol_;
}


template <class Iterator, class Char, class Traits, class Allocator>
void 
Lexer<Iterator, Char, Traits, Allocator>::recover()
{   
    const LexerState* state = state_machine_ ? state_machine_->start_state_ : NULL;
    if ( state )
    {
        const LexerTransition* transition = NULL;
        while ( position_ != end_ && (transition = find_transition_by_character(state, *position_)) )
        {
            ++position_;
        }
    }

    lexeme_.clear();
    full_ = position_ == end_;
    symbol_ = position_ != end_ ? run() : end_symbol_;    
}


template <class Iterator, class Char, class Traits, class Allocator>
void Lexer<Iterator, Char, Traits, Allocator>::skip()
{    
    SWEET_ASSERT( state_machine_ );

    const LexerState* state = state_machine_->whitespace_start_state_;
    if ( state )
    {
        const LexerTransition* transition = NULL;
        while ( position_ != end_ && (transition = find_transition_by_character(state, *position_)) )
        {
            state = transition->state_;            
            if ( transition->action_ )
            {
                int index = transition->action_->index_;
                SWEET_ASSERT( index >= 0 && index < (int) action_handlers_.size() );                
                const LexerActionFunction& function = action_handlers_[index].function_;
                SWEET_ASSERT( function );
                int symbol = 0;
                function( &position_, end_, &lexeme_, &symbol );
            }
            else
            {
                ++position_;
            }
        }        
    }
}


template <class Iterator, class Char, class Traits, class Allocator>
int
Lexer<Iterator, Char, Traits, Allocator>::run()
{    
    int symbol = 0;        

    const LexerState* state = state_machine_ ? state_machine_->start_state_ : NULL;
    if ( state )
    {
        symbol = state->symbol_;

        const LexerTransition* transition = NULL;
        while ( position_ != end_ && (transition = find_transition_by_character(state, *position_)) )
        {
            state = transition->state_;
            symbol = state->symbol_;
            
            if ( transition->action_ )
            {
                int index = transition->action_->index_;
                SWEET_ASSERT( index >= 0 && index < (int) action_handlers_.size() );                
                const LexerActionFunction& function = action_handlers_[index].function_;
                SWEET_ASSERT( function );
                function( &position_, end_, &lexeme_, &symbol );
            }
            else
            {
                lexeme_ += *position_;
                ++position_;
            }
        }        
    }
    
    return state ? symbol : 0;
}


template <class Iterator, class Char, class Traits, class Allocator>
const LexerTransition* 
Lexer<Iterator, Char, Traits, Allocator>::find_transition_by_character( const LexerState* state, int character ) const
{
    SWEET_ASSERT( state != NULL );

    const LexerTransition* transition = state->transitions_;
    while ( transition != state->transitions_end_ && !is_on_character(transition, character) )
    {
        ++transition;
    }

    return transition != state->transitions_end_ ? transition : NULL;
}


template <class Iterator, class Char, class Traits, class Allocator>
bool 
Lexer<Iterator, Char, Traits, Allocator>::is_on_character( const LexerTransition* transition, int character ) const
{
    SWEET_ASSERT( transition != NULL );
    return character >= transition->begin_ && character < transition->end_;
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
ParserEventSink<Iterator, UserData, Char, Traits, Allocator>::~ParserEventSink()
{
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void 
ParserEventSink<Iterator, UserData, Char, Traits, Allocator>::parser_error( const Parser<Iterator, UserData, Char, Traits, Allocator>* parser, const char* format, va_list args )
{
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void 
ParserEventSink<Iterator, UserData, Char, Traits, Allocator>::parser_vprintf( const char* format, va_list args )
{
}


template <class UserData, class Char, class Traits, class Allocator>
ParserNode<UserData, Char, Traits, Allocator>::ParserNode( const ParserState* state, int symbol, const UserData& user_data )
: state_( state ),
  symbol_( symbol ),
  user_data_( user_data ),
  lexeme_()
{
    SWEET_ASSERT( state != NULL );
}


template <class UserData, class Char, class Traits, class Allocator>
ParserNode<UserData, Char, Traits, Allocator>::ParserNode( const ParserState* state, int symbol, const std::basic_string<Char, Traits, Allocator>& lexeme )
: state_( state ),
  symbol_( symbol ),
  user_data_(),
  lexeme_( lexeme )
{
    SWEET_ASSERT( state != NULL );
}


template <class Iterator, class Char, class Traits, class Allocator>
AddLexerActionHandler<Iterator, Char, Traits, Allocator>::AddLexerActionHandler( Lexer<Iterator, Char, Traits, Allocator>* lexer )
: lexer_( lexer )
{
    SWEET_ASSERT( lexer_ != NULL );
}


template <class Iterator, class Char, class Traits, class Allocator>
const AddLexerActionHandler<Iterator, Char, Traits, Allocator>& 
AddLexerActionHandler<Iterator, Char, Traits, Allocator>::operator()( const char* identifier, LexerActionFunction function ) const
{
    lexer_->set_action_handler( identifier, function );
    return *this;
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
AddParserActionHandler<Iterator, UserData, Char, Traits, Allocator>::AddParserActionHandler( Parser<Iterator, UserData, Char, Traits, Allocator>* parser )
: parser_( parser )
{
    SWEET_ASSERT( parser_ != NULL );
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
const AddParserActionHandler<Iterator, UserData, Char, Traits, Allocator>& 
AddParserActionHandler<Iterator, UserData, Char, Traits, Allocator>::default_action( ParserActionFunction function ) const
{
    SWEET_ASSERT( parser_ != NULL );
    parser_->set_default_action_handler( function );
    return *this;
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
const AddParserActionHandler<Iterator, UserData, Char, Traits, Allocator>& 
AddParserActionHandler<Iterator, UserData, Char, Traits, Allocator>::operator()( const char* identifier, ParserActionFunction function ) const
{
    SWEET_ASSERT( identifier != NULL );
    SWEET_ASSERT( parser_ != NULL );
    parser_->set_action_handler( identifier, function );
    return *this;
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
Parser<Iterator, UserData, Char, Traits, Allocator>::ParserActionHandler::ParserActionHandler( const ParserAction* action, ParserActionFunction function )
: action_( action ),
  function_( function )
{
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
Parser<Iterator, UserData, Char, Traits, Allocator>::Parser( const ParserStateMachine* state_machine, ParserEventSink<Iterator, UserData, Char, Traits, Allocator>* event_sink )
: state_machine_( state_machine ),
  event_sink_( event_sink ),
  nodes_(),
  lexer_( state_machine->lexer_state_machine_, state_machine->end_symbol_ ),
  action_handlers_(),
  default_action_handler_( NULL ),
  debug_enabled_( false ),
  accepted_( false ),
  full_( false )
{
    SWEET_ASSERT( state_machine_ != NULL );

    nodes_.reserve( 64 );
    action_handlers_.reserve( state_machine_->actions_end_ - state_machine_->actions_ );
    for ( const ParserAction* action = state_machine_->actions_; action != state_machine_->actions_end_; ++action )
    {
        SWEET_ASSERT( action != NULL );
        action_handlers_.push_back( ParserActionHandler(action, NULL) );
    }
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void 
Parser<Iterator, UserData, Char, Traits, Allocator>::parse( Iterator start, Iterator finish )
{
    SWEET_ASSERT( state_machine_ != NULL );
    accepted_ = false;
    full_ = false;
    nodes_.clear();
    lexer_.reset( start, finish );
    accepted_ = parse( state_machine_->start_state_ );
    full_ = lexer_.full();
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
bool 
Parser<Iterator, UserData, Char, Traits, Allocator>::accepted() const
{
    return accepted_;
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
bool 
Parser<Iterator, UserData, Char, Traits, Allocator>::full() const
{
    return full_;
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
const UserData&
Parser<Iterator, UserData, Char, Traits, Allocator>::user_data() const
{
    SWEET_ASSERT( accepted() );
    SWEET_ASSERT( nodes_.size() == 1 );
    return nodes_.front().user_data_;
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
const Iterator& 
Parser<Iterator, UserData, Char, Traits, Allocator>::position() const
{
    return lexer_.position();
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
AddLexerActionHandler<Iterator, Char, Traits, Allocator>
Parser<Iterator, UserData, Char, Traits, Allocator>::lexer_action_handlers()
{
    return AddLexerActionHandler<Iterator, Char, Traits, Allocator>( &lexer_ );
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
AddParserActionHandler<Iterator, UserData, Char, Traits, Allocator>
Parser<Iterator, UserData, Char, Traits, Allocator>::parser_action_handlers()
{
    return AddParserActionHandler<Iterator, UserData, Char, Traits, Allocator>( this );
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void 
Parser<Iterator, UserData, Char, Traits, Allocator>::set_default_action_handler( ParserActionFunction function )
{
    default_action_handler_ = function;
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void 
Parser<Iterator, UserData, Char, Traits, Allocator>::set_action_handler( const char* identifier, ParserActionFunction function )
{
    SWEET_ASSERT( identifier != NULL );
    SWEET_ASSERT( function != NULL );
    
    typename std::vector<ParserActionHandler>::iterator action_handler = action_handlers_.begin();
    while ( action_handler != action_handlers_.end() && strcmp(action_handler->action_->identifier_, identifier) != 0 )
    {
        ++action_handler;
    }
    
    if ( action_handler != action_handlers_.end() )
    {
        action_handler->function_ = function;
    }
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void 
Parser<Iterator, UserData, Char, Traits, Allocator>::fire_error( const char* format, ... ) const
{
    if ( event_sink_ != NULL )
    {
        SWEET_ASSERT( format != NULL );
        va_list args;
        va_start( args, format );
        event_sink_->parser_error( this, format, args );
        va_end( args );
    }
    else
    {
        va_list args;
        va_start( args, format );
        vfprintf( stderr, format, args );
        va_end( args );
    }
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void 
Parser<Iterator, UserData, Char, Traits, Allocator>::fire_printf( const char* format, ... ) const
{
    if ( event_sink_ != NULL )
    {
        SWEET_ASSERT( format != NULL );
        va_list args;
        va_start( args, format );
        event_sink_->parser_vprintf( format, args );
        va_end( args );
    }
    else
    {
        va_list args;
        va_start( args, format );
        vfprintf( stdout, format, args );
        va_end( args );
    }
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void 
Parser<Iterator, UserData, Char, Traits, Allocator>::set_debug_enabled( bool debug_enabled )
{
    debug_enabled_ = debug_enabled;
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
bool 
Parser<Iterator, UserData, Char, Traits, Allocator>::is_debug_enabled() const
{
    return debug_enabled_;
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
const ParserTransition* 
Parser<Iterator, UserData, Char, Traits, Allocator>::find_transition( int symbol, const ParserState* state ) const
{
    SWEET_ASSERT( state != NULL );
    SWEET_ASSERT( state_machine_ != NULL );
    const ParserTransition* transition = find_transition_by_symbol( state, symbol );
    return transition;
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
typename std::vector<ParserNode<UserData, Char, Traits, Allocator> >::iterator 
Parser<Iterator, UserData, Char, Traits, Allocator>::find_node_to_reduce_to( const ParserProduction* production, std::vector<ParserNode>& nodes )
{
    SWEET_ASSERT( production->length_ < nodes.size() );
    typename std::vector<ParserNode>::reverse_iterator node = nodes.rbegin() + production->length_;
    return node != nodes.rend() ? node.base() : nodes_.begin();
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
typename std::vector<ParserNode<UserData, Char, Traits, Allocator> >::const_iterator 
Parser<Iterator, UserData, Char, Traits, Allocator>::find_start_node( const std::vector<ParserNode>& nodes ) const
{
    typename std::vector<ParserNode>::const_reverse_iterator node = nodes.rbegin();    
    while ( node != nodes.rend() && node.symbol_ != NULL )
    {
        ++node;
    }
    
    return node != nodes_.rend() ? node.base() - 1 : nodes_.begin() + 1;
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void 
Parser<Iterator, UserData, Char, Traits, Allocator>::debug_shift( const ParserNode& node ) const
{
    if ( debug_enabled_ )
    {
        int symbol = node.symbol_;
        const std::string& lexeme = node.lexeme_;
        printf( "SHIFT: (%d %s)\n", symbol, lexeme.c_str() );
    }
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void 
Parser<Iterator, UserData, Char, Traits, Allocator>::debug_reduce( const ParserProduction* reduced_production, const ParserNode* start, const ParserNode* finish ) const
{
    SWEET_ASSERT( start != NULL );
    SWEET_ASSERT( finish != NULL );
    SWEET_ASSERT( start <= finish );
    SWEET_ASSERT( reduced_production != NULL );
    SWEET_ASSERT( reduced_production->symbol_ != 0 );

    if ( debug_enabled_ )
    {
        fire_printf( "REDUCE: %d <- ", reduced_production->symbol_ );        

        const ParserNode* node = start; 
        if ( node != finish )
        {
            int symbol = node->symbol_;
            const std::string& lexeme = node->lexeme_;
            fire_printf( "(%d %s)", symbol, lexeme.c_str() );
            ++node;
        }
        
        while ( node != finish )
        {
            int symbol = node->symbol_;
            const std::string& lexeme = node->lexeme_;
            fire_printf( " (%d %s)", symbol, lexeme.c_str() );
            ++node;
        }
        
        fire_printf( "\n" );
    }
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
UserData
Parser<Iterator, UserData, Char, Traits, Allocator>::handle( const ParserProduction* reduced_production, const ParserNode* start, const ParserNode* finish ) const
{
    SWEET_ASSERT( start != NULL );
    SWEET_ASSERT( finish != NULL );
    SWEET_ASSERT( start <= finish );
    SWEET_ASSERT( reduced_production != NULL );

    if ( reduced_production->action_ != NULL )
    {
        int index = reduced_production->action_->index_;
        SWEET_ASSERT( index >= 0 && index < static_cast<int>(action_handlers_.size()) );            
        if ( action_handlers_[index].function_ != NULL )
        {
            return (*action_handlers_[index].function_)( reduced_production->symbol_, start, finish );
        }
    }

    return default_action_handler_ != NULL ? (*default_action_handler_)( reduced_production->symbol_, start, finish ) : UserData();
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
bool
Parser<Iterator, UserData, Char, Traits, Allocator>::parse( const ParserState* parser_start_state )
{
    SWEET_ASSERT( state_machine_ != NULL );
    SWEET_ASSERT( parser_start_state != NULL );
    
    nodes_.push_back( ParserNode(parser_start_state, 0, UserData()) );
    lexer_.advance();

    bool accepted = false;
    bool rejected = false;
    while ( !accepted && !rejected )
    {
        const ParserTransition* transition = find_transition( lexer_.symbol(), nodes_.back().state_ );
        if ( transition != NULL )
        {
            switch ( transition->type_ )
            {
                case TRANSITION_SHIFT:
                    shift( transition );
                    break;

                case TRANSITION_REDUCE:
                    reduce( transition, &accepted, &rejected );
                    break;
                    
                default:
                    SWEET_ASSERT( false );
                    fire_error( "Unexpected transition type '%d'", transition->type_ );
                    rejected = true;
                    break;
            }
        }
        else
        {
            error( &accepted, &rejected );
        }        
    }
    
    return accepted;
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void 
Parser<Iterator, UserData, Char, Traits, Allocator>::shift( const ParserTransition* transition )
{
    SWEET_ASSERT( state_machine_ != NULL );
    SWEET_ASSERT( transition != NULL );
    ParserNode node( transition->state_, lexer_.symbol(), lexer_.lexeme() );
    debug_shift( node );
    nodes_.push_back( node );
    lexer_.advance();
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void 
Parser<Iterator, UserData, Char, Traits, Allocator>::reduce( const ParserTransition* transition, bool* accepted, bool* rejected )
{
    SWEET_ASSERT( state_machine_ );
    SWEET_ASSERT( transition );
    SWEET_ASSERT( accepted );
    SWEET_ASSERT( rejected );
    
    int symbol = transition->reduced_production_->symbol_;
    if ( symbol != state_machine_->start_symbol_ )
    {
        const ParserProduction* reduced_production = transition->reduced_production_;
        typename std::vector<ParserNode>::iterator i = find_node_to_reduce_to( reduced_production, nodes_ );
        const ParserNode* start = i != nodes_.end() ? &(*i) : &nodes_.back() + 1;
        const ParserNode* finish = &nodes_.back() + 1;

        debug_reduce( reduced_production, start, finish );
        UserData user_data = handle( reduced_production, start, finish );
        nodes_.erase( i, nodes_.end() );
        const ParserTransition* transition = find_transition( symbol, nodes_.back().state_ );
        SWEET_ASSERT( transition );
        ParserNode node( transition->state_, symbol, user_data );
        nodes_.push_back( node );
    }
    else
    {    
        SWEET_ASSERT( nodes_.size() == 2 );
        nodes_.erase( nodes_.begin() );
        *accepted = true;
    }              
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
void 
Parser<Iterator, UserData, Char, Traits, Allocator>::error( bool* accepted, bool* rejected )
{
    SWEET_ASSERT( state_machine_ );
    SWEET_ASSERT( !nodes_.empty() );
    SWEET_ASSERT( accepted );
    SWEET_ASSERT( rejected );

    bool handled = false;
    while ( !nodes_.empty() && !handled && !*accepted && !*rejected )
    {
        const ParserTransition* transition = find_transition( state_machine_->error_symbol_, nodes_.back().state_ );
        if ( transition )
        {
            switch ( transition->type_ )
            {
                case TRANSITION_SHIFT:
                    shift( transition );
                    lexer_.recover();
                    handled = true;
                    break;

                case TRANSITION_REDUCE:
                    reduce( transition, accepted, rejected );
                    break;
                    
                default:
                    SWEET_ASSERT( false );
                    fire_error( "Unexpected transition type '%d'", transition->type_ );
                    *rejected = true;
                    break;
            }
        }
        else
        {
            nodes_.pop_back();
        }
    }
    
    if ( nodes_.empty() )
    {
        fire_error( "Syntax error" );
        *rejected = true;
    }
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
const ParserTransition* 
Parser<Iterator, UserData, Char, Traits, Allocator>::find_transition_by_symbol( const ParserState* state, int symbol ) const
{
    SWEET_ASSERT( state );

    const ParserTransition* transition = NULL;
    
    if ( symbol != 0 )
    {
        transition = state->transitions_;
        while ( transition != state->transitions_end_ && !is_on_symbol(transition, symbol) )
        {
            ++transition;
        }

        if ( transition == state->transitions_end_ )
        {
            transition = NULL;
        }
    }
    
    return transition;
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
bool 
Parser<Iterator, UserData, Char, Traits, Allocator>::is_on_symbol( const ParserTransition* transition, int symbol ) const
{
    SWEET_ASSERT( transition );
    return transition->symbol_ == symbol;
}


template <class Iterator, class UserData, class Char, class Traits, class Allocator>
bool 
Parser<Iterator, UserData, Char, Traits, Allocator>::is_production_in_kernel( const ParserNode& node, const ParserProduction* production ) const
{
    return node.started_productions_ 
        && std::find( node.started_productions_, node.started_productions_end_, production ) != node.started_productions_end_
        ;
}

}

}

#endif
