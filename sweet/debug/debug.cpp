//
// set_exception_handler.cpp
// Copyright (c) 2008 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include <sweet/debug/debug.hpp>

using namespace sweet::debug;

static LONG WINAPI handle_exception( _EXCEPTION_POINTERS* exception_pointers );

/**
// Startup the debug component.
//
// Installs a system exception handler.
*/
void 
sweet::debug::startup()
{
#if defined(SWEET_DEBUG_DUMP_ENABLED)
    ::SetUnhandledExceptionFilter( handle_exception );
#endif
}


/**
// Shutdown the debug component.
*/
void
sweet::debug::shutdown()
{
}


/**
// Is the current process being debugged?
//
// @return
//  True if the current process is being debugged otherwise false.
*/
bool 
sweet::debug::is_debugger()
{
    return ::IsDebuggerPresent() ? true : false;
}


/**
// Perform a stack trace.
//
// Walks back up the call stack (ignoring calling conventions) until \e length
// functions have been stepped through or the top of the call stack (assumed
// to be null) is reached.
//
// The return address of each level of the call stack is stored in 
// \e addresses so that on return \e addresses is populated with a series of
// instruction pointers that can be used in conjunction with a map file or
// debug information to find the names of the functions that contain these
// addresses.
//
// @param addresses
//  An array of void pointers to receive the instruction pointers in the call
//  stack.
//
// @param length
//  The number of calls to trace back.
*/
void 
sweet::debug::stack_trace( void** addresses, size_t length )
{
//
// Get the current stack frame.
//
    void** frame = NULL;
    __asm mov frame, ebp

//
// Walk back up the call stack recording the instruction pointer address
// that will be returned to at the end of each function until the top of
// the call stack is reached (frame is null) or the frame pointer points to
// invalid memory (as may happen if a function with a calling convetion that 
// doesn't store the frame pointer is encountered).
//
    int i = 0;
    while ( i < length && frame != NULL )
    {
        addresses[i] = *(frame + 1);
        frame        = reinterpret_cast<void**>( *frame );
        ++i;

        MEMORY_BASIC_INFORMATION memory_basic_information;
        SIZE_T result = ::VirtualQuery( frame, &memory_basic_information, sizeof(memory_basic_information) );
        if ( result != sizeof(memory_basic_information) || memory_basic_information.AllocationBase == NULL )
        {
            frame = NULL;
        }
    }

//
// Clear the remaining addresses to null.
//
    while ( i < length )
    {
        addresses[i] = NULL;
        ++i;
    }
}


/**
// Generate a filename by combining the current date, time, and thread id 
// with a prefix and suffix.
//
// The filename is guaranteed to be terminated by a null character even if
// the generated filename was greater than the length of the buffer passed
// in.
//
// @param prefix
//  The prefix to append the current date and time to to generate a filename.
//
// @param suffix
//  The suffix to append to the generated filename after the date and time.
//
// @param buffer
//  The buffer to generate the filename into.
//
// @param length
//  The length of the buffer (in characters).
//
// @return
//  The buffer passed in \e buffer.
*/
const char* 
sweet::debug::filename_with_date_time_and_thread( const char* prefix, const char* suffix, char* buffer, size_t length )
{
    SWEET_ASSERT( prefix != NULL );
    SWEET_ASSERT( suffix != NULL );
    SWEET_ASSERT( buffer != NULL );
    SWEET_ASSERT( length > 0 );

    time_t now = time( 0 );

    char date_and_time [MAX_PATH + 1];
    strftime( date_and_time, sizeof(date_and_time), "-%Y-%m-%d-%H%M", localtime(&now) );
    date_and_time[sizeof(date_and_time) - 1] = 0;

    _snprintf( buffer, length, "%s%s-%d%s", prefix, date_and_time, static_cast<int>(::GetCurrentThreadId()), suffix );
    buffer[length - 1] = 0;

    return buffer;
}


/**
// Handle system exceptions.
//
// If the process is being debugged (assumed if ::UnhandledExceptionFilter() 
// returns EXCEPTION_CONTINUE_SEARCH) then this function silently does nothing 
// and returns EXCEPTION_CONTINUE_SEARCH so that the debugger can handle the
// exception.  Otherwise it writes out a mini dump file.
//
// @param exception_pointers
//  The EXCEPTION_POINTERS information returned by ::GetExceptionInformation()
//  in the system exception handler.
//
// @return
//  EXCEPTION_CONTINUE_SEARCH if this process is being debugged as it is 
//  assumed that a debugger is running and should be left to deal with 
//  the exception otherwise EXCEPTION_EXECUTE_HANDLER.
*/
static LONG WINAPI handle_exception( _EXCEPTION_POINTERS* exception_pointers )
{
    SWEET_ASSERT( exception_pointers != NULL );

    if ( !sweet::debug::is_debugger() )
    {
        fprintf( stderr, "A system error occured.\n" );

        HMODULE dbghelp = ::LoadLibraryA( "dbghelp.dll" );
        if ( dbghelp != NULL )
        {
            typedef BOOL (WINAPI *MiniDumpWriteDumpFunction)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, PMINIDUMP_EXCEPTION_INFORMATION, PMINIDUMP_USER_STREAM_INFORMATION, PMINIDUMP_CALLBACK_INFORMATION);
            MiniDumpWriteDumpFunction MiniDumpWriteDump = (MiniDumpWriteDumpFunction) ::GetProcAddress( dbghelp, "MiniDumpWriteDump" );
            if ( MiniDumpWriteDump != NULL )
            {
                char filename [MAX_PATH + 1];
                filename_with_date_time_and_thread( "sweet-software", ".dmp", filename, sizeof(filename) );

                HANDLE file = ::CreateFileA( filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
                if ( file != INVALID_HANDLE_VALUE ) 
                {
                    MINIDUMP_EXCEPTION_INFORMATION exception_information; 
                    memset( &exception_information, 0, sizeof(exception_information) );
                    exception_information.ThreadId          = ::GetCurrentThreadId(); 
                    exception_information.ExceptionPointers = exception_pointers; 
                    exception_information.ClientPointers    = FALSE; 
                    MiniDumpWriteDump( ::GetCurrentProcess(), ::GetCurrentProcessId(), file, MiniDumpWithPrivateReadWriteMemory, &exception_information, 0, 0 ); 
                    ::CloseHandle( file );
                }
            }

            ::FreeLibrary( dbghelp );
            dbghelp = NULL;
        }

        return EXCEPTION_EXECUTE_HANDLER;
    }
    else
    {
        return EXCEPTION_CONTINUE_SEARCH;
    }
}