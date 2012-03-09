
#error "This file is for doxygen documentation only - don't include it!"

namespace sweet
{

/**
// %Error library.
//
// The error component defines the base class for exceptions used by 
// other components.  The class is Error and derives from std::exception 
// so that all exceptions can be caught under a single handler if necessary.
// The constructor takes an error number and a printf style format string 
// that is used to format the description of the error that is provided by 
// what().
//
// The intended use is for each component to provide a class named Error 
// that is derived from exception::Error.  This way all exceptions 
// that are thrown from components can be caught either as error::Errors or as 
// more specific per component exceptions.
//
@code
class Error : virtual public std::exception
{
    public:
        explicit Error( int error );
        Error( int error, const char* format, ... );
        virtual ~Error();

        int error() const;
        const char* what() const;

        static const char* format( int oserror, char* buffer, size_t length );
};
@endcode
*/
namespace error
{
}

}

#endif

