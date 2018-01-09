//
// LuaThreadEventSink.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "LuaThreadEventSink.hpp"
#include <sweet/assert/assert.hpp>

using namespace sweet::lua;

LuaThreadEventSink::~LuaThreadEventSink()
{
}

void LuaThreadEventSink::lua_thread_returned( LuaThread* /*thread*/, void* /*context*/ )
{
}

void LuaThreadEventSink::lua_thread_errored( LuaThread* /*thread*/, void* /*context*/ )
{
}
