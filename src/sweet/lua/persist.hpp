#ifndef SWEET_LUA_PERSIST_HPP_INCLUDED
#define SWEET_LUA_PERSIST_HPP_INCLUDED

#include <sweet/traits/traits.hpp>

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
