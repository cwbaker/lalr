#ifndef SWEET_PARSER_PARSERTRANSITION_HPP_INCLUDED
#define SWEET_PARSER_PARSERTRANSITION_HPP_INCLUDED

#include <string>
#include <set>

namespace sweet
{

namespace lalr
{

class ParserState;
class ParserSymbol;

/**
// The type of a transition in a parser's state machine.
*/
enum ParserTransitionType
{
    TRANSITION_SHIFT, ///< Shift a terminal symbol onto the parser's stack.
    TRANSITION_REDUCE ///< Reduce one or more symbols on the parser's stack into a non terminal.
};

/**
// A transition in a parser's state machine.
*/
class ParserTransition
{
    public:
        static const int INVALID_INDEX = -1;

    private:
        const ParserSymbol* symbol_; ///< The symbol that the transition is taken on.
        mutable ParserState* state_; ///< The state that is transitioned to.
        mutable const ParserSymbol* reduced_symbol_; ///< The symbol that is reduced to or null if this isn't a reducing transition.
        mutable int reduced_length_; ///< The number of symbols on the right-hand side of the reduced production.
        mutable int precedence_; ///< The precedence of the reduce production or 0 for the default precedence or no reduction.
        mutable int action_; ///< The index of the action taken on reduce or ParserAction::INVALID_INDEX if there is no action associated with the reduce.
        mutable ParserTransitionType type_; ///< The type of transition that this is.
        mutable int index_; ///< The index of this transition.

    public:
        ParserTransition( const ParserSymbol* symbol, const ParserSymbol* reduced_symbol, int reduced_length, int precedence, int action );
        ParserTransition( const ParserSymbol* symbol, ParserState* state );

        int get_type() const;
        ParserState* get_state() const;
        const ParserSymbol* reduced_symbol() const;
        int reduced_length() const;
        int precedence() const;
        int action() const;
        bool is_symbol( const ParserSymbol* symbol ) const;
        const ParserSymbol* get_symbol() const;
        void describe( std::string* description ) const;
        
        void set_index( int index ) const;
        int get_index() const;
        
        void override_shift_to_reduce( const ParserSymbol* symbol, int length, int precedence, int action ) const;
        void override_reduce_to_reduce( const ParserSymbol* symbol, int length, int precedence, int action ) const;
        
        bool operator<( const ParserTransition& transition ) const;
};

}

}

#endif
