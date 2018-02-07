#ifndef SWEET_PARSER_PARSER_HPP_INCLUDED
#define SWEET_PARSER_PARSER_HPP_INCLUDED

#include "ParserNode.hpp"
#include "ParserUserData.hpp"
#include "AddParserActionHandler.hpp"
#include "Lexer.hpp"
#include "AddLexerActionHandler.hpp"
#include <memory>
#include <functional>
#include <vector>

namespace sweet
{

namespace error
{

class Error;

}

namespace lalr
{

class ParserErrorPolicy;
class LalrAction;
class LalrSymbol;
class LalrProduction;
class LalrTransition;
class LalrState;
class ParserStateMachine;
class ParserErrorPolicy;

/**
// A %parser.
*/
template <class Iterator, class UserData = std::shared_ptr<ParserUserData<typename std::iterator_traits<Iterator>::value_type> >, class Char = typename std::iterator_traits<Iterator>::value_type, class Traits = typename std::char_traits<Char>, class Allocator = typename std::allocator<Char> >
class Parser
{
    public:
        typedef ParserNode<UserData, Char, Traits, Allocator> ParserNode;
        typedef typename std::vector<ParserNode>::const_iterator ParserNodeConstIterator;
        typedef std::function<void ( Iterator* begin, Iterator end, std::basic_string<Char, Traits, Allocator>* lexeme, const LalrSymbol** symbol)> LexerActionFunction;
        typedef std::function<UserData (const LalrSymbol* symbol, const ParserNode* start, const ParserNode* finish)> LalrActionFunction;

    private:
        struct LalrActionHandler
        {        
            const LalrAction*  action_;
            LalrActionFunction function_;
            LalrActionHandler( const LalrAction* action, LalrActionFunction function );
        };

        const ParserStateMachine* state_machine_; ///< The data that defines the state machine used by this parser.
        ParserErrorPolicy* error_policy_; ///< The error policy this parser uses to report errors and debug information.
        std::vector<ParserNode> nodes_; ///< The stack of nodes that store symbols that are shifted and reduced during parsing.
        Lexer<Iterator, Char, Traits, Allocator> lexer_; ///< The lexical analyzer used during parsing.
        std::vector<LalrActionHandler> action_handlers_; ///< The action handlers for parser actions taken during reduction.
        LalrActionFunction default_action_handler_; ///< The default action handler for reductions that don't specify any action.
        bool debug_enabled_; ///< True if shift and reduce operations should be printed otherwise false.
        bool accepted_; ///< True if the parser accepted its input otherwise false.
        bool full_; ///< True if the parser processed all of its input otherwise false.

    public:
        Parser( const ParserStateMachine* state_machine, ParserErrorPolicy* error_policy = NULL, LexerErrorPolicy* lexer_error_policy = NULL );

        void reset();
        void parse( Iterator start, Iterator finish );
        bool parse( const void* symbol, const std::basic_string<Char, Traits, Allocator>& lexeme );
        bool parse( const LalrSymbol* symbol, const std::basic_string<Char, Traits, Allocator>& lexeme );
        bool accepted() const;
        bool full() const;
        const UserData& user_data() const;
        const Iterator& position() const;

        AddParserActionHandler<Iterator, UserData, Char, Traits, Allocator> parser_action_handlers();
        AddLexerActionHandler<Iterator, Char, Traits, Allocator> lexer_action_handlers();
        void set_default_action_handler( LalrActionFunction function );
        void set_action_handler( const char* identifier, LalrActionFunction function );

        void fire_error( int error, const char* format, ... ) const;
        void fire_printf( const char* format, ... ) const;
        
        void set_debug_enabled( bool debug_shift_enabled );
        bool is_debug_enabled() const;
        
    private:
        const LalrTransition* find_transition( const LalrSymbol* symbol, const LalrState* state ) const;
        typename std::vector<ParserNode>::iterator find_node_to_reduce_to( const LalrTransition* transition, std::vector<ParserNode>& nodes );
        void debug_shift( const ParserNode& node ) const;
        void debug_reduce( const LalrSymbol* reduced_symbol, const ParserNode* start, const ParserNode* finish ) const;
        UserData handle( const LalrTransition* transition, const ParserNode* start, const ParserNode* finish ) const;
        void shift( const LalrTransition* transition, const std::basic_string<Char, Traits, Allocator>& lexeme );
        void reduce( const LalrTransition* transition, bool* accepted, bool* rejected );
        void error( bool* accepted, bool* rejected );
};

}

}

#endif
