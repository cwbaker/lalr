//
// Trace.hpp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_DEBUG_TRACE_HPP_INCLUDED
#define SWEET_DEBUG_TRACE_HPP_INCLUDED

#include <sweet/build.hpp>
#include "declspec.hpp"

namespace sweet
{

namespace debug
{

/**
// The maximum stack trace size.
*/
static const int MAXIMUM_STACK_TRACE_SIZE = 32;

/**
// Write information to a trace file without allocating any memory.
*/
struct SWEET_DEBUG_DECLSPEC Trace
{
    int file_;
    char* buffer_pos_;
    char* buffer_end_;
    int stack_trace_size_;
    char buffer_ [8192];

    public:
        void open( const char* filename );
        void close();
        void set_stack_trace_size( int stack_trace_size );
        int  get_stack_trace_size() const;
        void flush();
        void write( const char* data, size_t length );
        void printf( const char* format, ... );
        void vprintf( const char* format, va_list args );
        void stack_trace();
};

}

}

#endif
