#pragma once

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE
#define _SCL_SECURE_NO_DEPRECATE
#define NOMINMAX

#include <sweet/build.hpp>

#pragma warning( disable: 4351 4355 )

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <time.h>

#include <vector>
#include <set>
#include <string>
#include <fstream>

#include <windows.h>

#include <sweet/lua/lua_/lua.h>
#include <sweet/lua/lua_/lualib.h>

#include <sweet/assert/assert.hpp>
#include <sweet/traits/traits.hpp>
#include <sweet/rtti/rtti.hpp>
#include <sweet/error/Error.hpp>
#include <sweet/error/macros.hpp>
