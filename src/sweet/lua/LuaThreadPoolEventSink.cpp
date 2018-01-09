//
// LuaThreadPoolEventSink.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "LuaThreadPoolEventSink.hpp"
#include <sweet/assert/assert.hpp>

using namespace sweet::lua;

LuaThreadPoolEventSink::~LuaThreadPoolEventSink()
{
}

void LuaThreadPoolEventSink::lua_thread_returned( LuaThreadPool* /*thread_pool*/, LuaThread* /*thread*/ )
{
}

void LuaThreadPoolEventSink::lua_thread_errored( LuaThreadPool* /*thread_pool*/, LuaThread* /*thread*/ )
{
}
