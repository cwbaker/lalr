//
// LuaPreprocessReader.hpp
// Copyright (c) 2009 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUAPREPROCESSREADER_HPP_INCLUDED
#define SWEET_LUA_LUAPREPROCESSREADER_HPP_INCLUDED

#include "declspec.hpp"
#include "lua_/lua.h"
#include <vector>

namespace sweet
{

namespace lua
{

/**
// @internal
//
// An implementation of the lua_Reader function and its associated context
// that preprocesses input replacing literal prefix and suffix tags to 
// allow Lua to be used as a PHP like template language.
//
// The source text that occurs between the prefix and suffix tags is passed
// through unchanged.  Source text that occurs outside of the prefix and 
// suffix tags is wrapped between 'io.write([[' and ']])'.  This causes the
// destination text that is compiled to emit the literal portions of the text.
//
// Source text that occurs between prefix and suffix tags where the prefix is
// immediately followed by an equals character, e.g. '<?lua=', is treated as
// a Lua expression that is to be converted into a string and output.  These
// blocks are wrapped between 'io.write(tostring(' and '))'.  This causes the
// destination text that is compiled to emit the value of the expression as 
// a string.
*/
class SWEET_LUA_DECLSPEC LuaPreprocessReader
{
    enum State
    {
        STATE_NULL, ///< The initial or post literal state.
        STATE_LUA, ///< Lua is being output (between <?lua and '?>'.
        STATE_PRE_EXPRESSION, ///< Pre expression text is being output ('io.write(tostring()').
        STATE_EXPRESSION, ///< Lua expression is being output (between <?lua= and ?>).
        STATE_POST_EXPRESSION, ///< Post expression text is being output ('))').
        STATE_PRE_LITERAL, ///< Pre literal text is being output ('io.write([[').
        STATE_LITERAL, ///< Literal text is being output.
        STATE_ESCAPE_EMBEDDED_POST_LITERAL, ///< Escaped post literal text is being output.
        STATE_POST_LITERAL ///< Post literal text is being output ']]);'.
    };

    const char* prefix_;
    const char* prefix_end_;
    const char* suffix_;
    const char* suffix_end_;
    const char* pre_literal_;
    const char* pre_literal_end_;
    const char* post_literal_;
    const char* post_literal_end_;
    const char* pre_expression_;
    const char* pre_expression_end_;
    const char* post_expression_;
    const char* post_expression_end_;
    lua_Reader reader_;
    void* context_;
    State state_;
    std::vector<char> block_;
    const char* position_;
    const char* position_end_;
    const char* source_;
    const char* source_end_;
    char* destination_;
    char* destination_end_;

    public:
        LuaPreprocessReader( lua_Reader reader, void* context, size_t block_size = 4096 );
        void set_prefix_and_suffix( const char* prefix, const char* suffix );
        void set_pre_and_post_literal( const char* pre_literal, const char* post_literal );
        void set_pre_and_post_expression( const char* pre_literal, const char* post_literal );
        const char* read( lua_State* lua_state, size_t* size );
        static const char* reader( lua_State* lua_state, void* context, size_t* size );
        
    private:
        void null();
        void lua();
        void pre_literal();
        void literal_();
        void post_literal();
        void escape_embedded_post_literal();
        void pre_expression();
        void expression();
        void post_expression();
    
        void goto_null();
        void goto_lua();
        void goto_pre_literal();
        void goto_literal();
        void goto_escape_embedded_post_literal();
        void goto_post_literal();
        void goto_pre_expression();
        void goto_expression();
        void goto_post_expression();
        
        void push_source( const char* source, const char* source_end );
        void pop_source();
    
        bool is_prefix( const char* start, const char* finish ) const;
        bool is_suffix( const char* start, const char* finish ) const;
        bool is_post_literal( const char* start, const char* finish ) const;
        bool is_post_expression( const char* start, const char* finish ) const;
};

}

}

#endif