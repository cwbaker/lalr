//
// LuaPolicyWrapper.hpp
// Copyright (c) 2008 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUAPOLICYWRAPPER_HPP_INCLUDED
#define SWEET_LUA_LUAPOLICYWRAPPER_HPP_INCLUDED

namespace sweet
{

namespace lua
{

/**
// @internal
//
// A wrapper that captures the signature and address of a function and 
// indicates that a yielding function should be generated for it instead of
// a standard function.
*/
template <class Function, int PPOLICY>
class LuaPolicyWrapper
{
    static const int POLICY = PPOLICY;
    Function function_; ///< The address of the function to generate a yielding function for.

    public:
        LuaPolicyWrapper( Function function );
        Function get_function() const;
};

}

}

#endif
