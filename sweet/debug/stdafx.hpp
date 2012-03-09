
#pragma once

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE
#define _SCL_SECURE_NO_DEPRECATE
#define NOMINMAX
#define _WIN32_WINNT 0x500
#define WINVER       0x500

#include <sweet/build.hpp>

#include <windows.h>
#include <dbghelp.h>

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <memory.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <algorithm>

#include <sweet/assert/assert.hpp>
