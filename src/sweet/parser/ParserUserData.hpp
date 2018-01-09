//
// ParserUserData.hpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_PARSER_PARSERUSERDATA_HPP_INCLUDED
#define SWEET_PARSER_PARSERUSERDATA_HPP_INCLUDED

#include <memory>
#include <string>
#include <vector>

namespace sweet
{

namespace parser
{

class ParserSymbol;
template <class UserData, class Char, class Traits, class Allocator> class ParserNode;

/**
// The default implementation for data stored in a parser's stack.
*/
template <class Char, class Traits = typename std::char_traits<Char>, class Allocator = typename std::allocator<Char> >
class ParserUserData
{
    typedef ParserNode<std::shared_ptr<ParserUserData>, Char, Traits, Allocator> ParserNode;

    const ParserSymbol* symbol_; ///< The symbol at this user data's node.
    std::basic_string<Char, Traits, Allocator> lexeme_; ///< The lexeme at this user data's node.
    std::vector<std::shared_ptr<ParserUserData<Char, Traits, Allocator> > > user_datas_; ///< Children in the parse tree.
        
    public:
        ParserUserData( const ParserSymbol* symbol, const std::basic_string<Char, Traits, Allocator>& lexeme );  
        ParserUserData( const ParserSymbol* symbol, size_t user_datas );
        ParserUserData( const ParserSymbol* symbol, const ParserNode* start, const ParserNode* finish );

        const ParserSymbol* get_symbol() const;
        const std::basic_string<Char, Traits, Allocator>& get_lexeme() const;        
        void append_user_data( std::shared_ptr<ParserUserData> user_data );
        const std::vector<std::shared_ptr<ParserUserData<Char, Traits, Allocator> > >& get_user_datas() const;
};

}

}

#endif
