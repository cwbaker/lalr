//
// trace.cpp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "Trace.hpp"
#include "debug.hpp"

using namespace sweet;
using namespace sweet::debug;

/**
// Open the trace file and activate tracing.
//
// This is a dangerous function because it assumes that the trace file is not
// already open.  If you call this function twice without any intervening call
// to Trace::close() the file descriptor for the first opened trace file will
// be leaked.
//
// This is by design because the Trace class is designed to be used in 
// situations where its constructor may not have run (e.g. during the 
// initialization of static data when static constructors are called).  For
// this reason it is considered safer to assume that the client knows whether 
// or not the Trace that it is going to use has been opened or not (because
// this can be done with a static data variable that is guaranteed to have its
// value at startup time).
//
// For example:
@code
static bool  memory_tracing = false
static Trace memory_trace;

void 
memory::start_tracing( const char* filename )
{
    if ( !memory_tracing )
    {
        memory_trace.open( filename );
    }
}

void
memory::stop_tracing()
{
    if ( memory_tracing )
    {
        memory_trace.close();
    }
}
@endcode
//
// The example assumes that the client calls memory::start_tracing() and 
// memory::stop_tracing() at appropriate times.  It is especially important 
// to call Trace::close() because the Trace object will buffer information
// that is written to it and any buffered information will not be written
// to the file if Trace::close() isn't called.
//
// @param trace
//  The Trace structure to open the trace file for.
//
// @param filename
//  The name of the file to write trace information to.
*/
void Trace::open( const char* filename )
{
    SWEET_ASSERT( filename );

    file_ = ::_open( filename, _O_WRONLY | _O_CREAT | _O_TRUNC, _S_IREAD | _S_IWRITE );
    buffer_pos_ = buffer_;
    buffer_end_ = buffer_ + sizeof(buffer_);
    stack_trace_size_ = 6;
    memset( buffer_, 0, sizeof(buffer_) );
}

/**
// Close the trace file and deactivate tracing.
//
// This function is dangerous for the same reasons that Trace::open() is.  It
// assumes that this Trace has actually been successfully opened.
*/
void Trace::close()
{
    if ( file_ != -1 )
    {
        flush();
        ::_close( file_ );        
        file_ = -1;
        buffer_pos_ = buffer_;
    }
}

/**
// Set the stack trace size.
//
// @param stack_trace_size
//  The value to set the stack trace size to.
*/
void Trace::set_stack_trace_size( int stack_trace_size )
{
    if ( stack_trace_size > MAXIMUM_STACK_TRACE_SIZE )
    {
        stack_trace_size = MAXIMUM_STACK_TRACE_SIZE;
    }    

    stack_trace_size_ = stack_trace_size;
}

/**
// Get the stack trace size.
//
// @return
//  The stack trace size.
*/
int Trace::get_stack_trace_size() const
{
    return stack_trace_size_;
}

/**
// Flush any buffered trace information to the trace file.
*/
void Trace::flush()
{
    if ( file_ != -1 && buffer_pos_ > buffer_ )
    {
        ::_write( file_, buffer_, buffer_pos_ - buffer_ );
        buffer_pos_ = buffer_;
    }
}

/**
// Write data to the trace file.
//
// @param data
//  The data to write.
//
// @param length
//  The length of the data to write (in bytes).
*/
void Trace::write( const char* data, size_t length )
{
    if ( file_ != -1 )
    {
        while ( length > 0 )
        {
            size_t written = std::min( static_cast<size_t>(buffer_end_ - buffer_pos_), length );
            memcpy( buffer_pos_, data, written );

            data += written;
            length -= written;
            buffer_pos_ += written;

            if ( buffer_pos_ >= buffer_end_ )
            {
                flush();
            }
        }
    }
}

/**
// Print formatted text to the trace file.
//
// @param format
//  A printf style format string that describes the text to write.
//
// @param ...
//  Arguments as specified by \e format.
*/
void Trace::printf( const char* format, ... )
{
    if ( file_ != -1 )
    {
        va_list args;
        va_start( args, format );
        char buffer [128];
        size_t length = vsnprintf( buffer, sizeof(buffer), format, args );        
        va_end( args );
        write( buffer, length );
    }
}

/**
// Print formatted text to the trace file.
//
// @param format
//  A printf style format string that describes the text to write.
//
// @param args
//  Arguments as specified by \e format.
*/
void Trace::vprintf( const char* format, va_list args )
{
    if ( file_ != -1 )
    {
        char buffer [128];
        size_t length = vsnprintf( buffer, sizeof(buffer), format, args );
        write( buffer, length );
    }
}

/**
// Write a stack trace to the trace file.
//
// Skips the first two levels of the stack trace because they will correspond
// to Trace::stack_trace() and the function that called it which is also 
// assumed to be a tracing or debugging function.
*/
void Trace::stack_trace()
{
    SWEET_ASSERT( file_ != -1 );

    if ( stack_trace_size_ > 0 )
    {
        void* addresses [MAXIMUM_STACK_TRACE_SIZE + 2];
        sweet::debug::stack_trace( addresses, stack_trace_size_ + 2 );

        Trace::printf( "{0x%08x", addresses[2] );
        for ( int i = 3; i < stack_trace_size_; ++i )
        {
            Trace::printf( ", 0x%08x", addresses[i] );
        }
        Trace::printf( "}" );
    }
}
