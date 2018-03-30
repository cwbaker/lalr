#ifndef SWEET_LALR_PARSER_HPP_INCLUDED
#define SWEET_LALR_PARSER_HPP_INCLUDED

#include "ParserNode.hpp"
#include "ParserUserData.hpp"
#include "AddParserActionHandler.hpp"
#include "AddLexerActionHandler.hpp"
#include "Lexer.hpp"
#include <vector>

namespace sweet
{

namespace error
{

class Error;

}

namespace lalr
{

class ErrorPolicy;
class ParserAction;
class ParserSymbol;
class ParserTransition;
class ParserState;
class ParserStateMachine;
class ErrorPolicy;

/**
// A %parser.
*/
template <class Iterator, class UserData = std::shared_ptr<ParserUserData<typename std::iterator_traits<Iterator>::value_type> >, class Char = typename std::iterator_traits<Iterator>::value_type, class Traits = typename std::char_traits<Char>, class Allocator = typename std::allocator<Char> >
class Parser
{
    public:
        typedef ParserNode<UserData, Char, Traits, Allocator> ParserNode;
        typedef typename std::vector<ParserNode>::const_iterator ParserNodeConstIterator;
        typedef std::function<void ( Iterator* begin, Iterator end, std::basic_string<Char, Traits, Allocator>* lexeme, const ParserSymbol** symbol)> LexerActionFunction;
        typedef std::function<UserData (const ParserSymbol* symbol, const ParserNode* start, const ParserNode* finish)> ParserActionFunction;

    private:
        struct ParserActionHandler
        {        
            const ParserAction*  action_;
            ParserActionFunction function_;
            ParserActionHandler( const ParserAction* action, ParserActionFunction function );
        };

        const ParserStateMachine* state_machine_; ///< The data that defines the state machine used by this parser.
        ErrorPolicy* error_policy_; ///< The error policy this parser uses to report errors and debug information.
        std::vector<ParserNode> nodes_; ///< The stack of nodes that store symbols that are shifted and reduced during parsing.
        Lexer<Iterator, Char, Traits, Allocator> lexer_; ///< The lexical analyzer used during parsing.
        std::vector<ParserActionHandler> action_handlers_; ///< The action handlers for parser actions taken during reduction.
        ParserActionFunction default_action_handler_; ///< The default action handler for reductions that don't specify any action.
        bool debug_enabled_; ///< True if shift and reduce operations should be printed otherwise false.
        bool accepted_; ///< True if the parser accepted its input otherwise false.
        bool full_; ///< True if the parser processed all of its input otherwise false.

    public:
        Parser( const ParserStateMachine* state_machine, ErrorPolicy* error_policy = nullptr );

        void reset();
        void parse( Iterator start, Iterator finish );
        bool parse( const void* symbol, const std::basic_string<Char, Traits, Allocator>& lexeme );
        bool parse( const ParserSymbol* symbol, const std::basic_string<Char, Traits, Allocator>& lexeme );
        bool accepted() const;
        bool full() const;
        const UserData& user_data() const;
        const Iterator& position() const;

        AddParserActionHandler<Iterator, UserData, Char, Traits, Allocator> parser_action_handlers();
        AddLexerActionHandler<Iterator, Char, Traits, Allocator> lexer_action_handlers();
        void set_default_action_handler( ParserActionFunction function );
        void set_action_handler( const char* identifier, ParserActionFunction function );

        void fire_error( int error, const char* format, ... ) const;
        void fire_printf( const char* format, ... ) const;
        
        void set_debug_enabled( bool debug_shift_enabled );
        bool is_debug_enabled() const;
        
    private:
        const ParserTransition* find_transition( const ParserSymbol* symbol, const ParserState* state ) const;
        typename std::vector<ParserNode>::iterator find_node_to_reduce_to( const ParserTransition* transition, std::vector<ParserNode>& nodes );
        void debug_shift( const ParserNode& node ) const;
        void debug_reduce( const ParserSymbol* reduced_symbol, const ParserNode* start, const ParserNode* finish ) const;
        UserData handle( const ParserTransition* transition, const ParserNode* start, const ParserNode* finish ) const;
        void shift( const ParserTransition* transition, const std::basic_string<Char, Traits, Allocator>& lexeme );
        void reduce( const ParserTransition* transition, bool* accepted, bool* rejected );
        void error( bool* accepted, bool* rejected );
};

}

}

#include "Parser.ipp"

#endif
