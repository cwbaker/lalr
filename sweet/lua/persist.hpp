//
// persist.hpp
// Copyright (c) 2009 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_PERSIST_HPP_INCLUDED
#define SWEET_LUA_PERSIST_HPP_INCLUDED

namespace sweet
{

namespace lua
{

template <class Archive> void enter( Archive& archive, Lua& lua );
template <class Archive> void exit( Archive& archive, Lua& lua );
template <class Type, class Storage, class Archive> void persist( Archive& archive, const char* name, typename traits::traits<Type>::parameter_type object );

}

}

#include "persist.ipp"

#endif
