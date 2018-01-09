
#error "This file is just for Doxygen documentation - don't include it!"

namespace sweet
{

/**
 %Lua binding library.

    Sweet %Lua is a C++ library for providing %Lua bindings to C++ variables, functions, 
    and objects.  It provides classes to represent the %Lua virtual machine (Lua), 
    a table or object (LuaObject), and a coroutine or %thread (LuaThread).

    The Lua class represents the %Lua global environment and virtual machine.  It provides
    functions to execute %Lua scripts from files and memory and to define %Lua bindings
    to C++ variables, functions, and objects.

    The LuaObject class represents an object in the %Lua virtual machine.  It is typically
    used to create prototypes or metatables in %Lua that have no equivalent C++ object 
    to associate with non-intrusively.  When a %LuaObject is constructed a corresponding 
    table is created in the %Lua global environment.  Both the C++ application and the %Lua 
    virtual machine can then set variables on and call member functions of that table.

    The LuaThread class represents a %Lua coroutine.  It provides functions to start and
    resume execution in a separate yieldable %thread in the %Lua virtual machine.  The 
    calls can %yield and return control from the virtual machine back to the program
    allowing asynchronous behaviour to be abstracted behind an interface that scripts 
    can treat as synchronous.

@section introduction Introduction

    Initializing the library is as simple as constructing a %Lua object to represent
    the global environment and virtual machine and registering the variables, functions,
    and objects in the interface being bound.  
    
    The library has no static state.  Creating multiple %Lua objects poses no problems.
    However, like %Lua itself, objects are not thread-safe.  Only one thread can call 
    into a %Lua virtual machine at once.
    
    %LuaObjects and %LuaThreads may only be used only with the %Lua object that they 
    were created in (as they can only represent a table or coroutine in a single %Lua 
    virtual machine).

    Once a %Lua object has been created the values of variables can be set to the 
    values of C++ booleans, integers, floats, strings, functions, and objects.
    
    Setting a variable to the value of a C++ function sets the value of that variable
    to a C++ function that decodes the arguments expected by the target C++ function 
    from the %Lua stack, makes the call to the target C++ function, and pushes any 
    return value back onto the %Lua stack.

    Parameters to a C++ function can be bound as upvalues when the function is 
    registered.  Simply passing a value at the time of registration stores that
    value as an upvalue of the registered function in the %Lua virtual machine.  The 
    upvalue is then provided as a parameter when the C++ function is called by %Lua.
    Any other parameters expected by the C++ function are mapped as per normal after
    the bound upvalues.

    Parameters to a C++ function can be bound out of order using one of the out of 
    order parameter values _1, _2, _3, _4, _5, or _6.  The number of the out of order 
    parameter specifies the index of the %Lua parameter that is to be provided to the C++ 
    parameter at the position that it appears in (the same as Boost Bind).
    
    Out of order parameters and upvalue binding can be used together to provide any 
    combination of bound values and out of order parameters.

@include lua_hello_world_example.cpp

@section executing_scripts_and_functions Executing Scripts and Functions

    Scripts and functions are executed by calling Lua::call(), LuaThread::call(), 
    or LuaThread::resume().

    All of these functions return a helper object that provides a convenient syntax 
    for supplying the parameters to the call and getting the return value via 
    AddParameter::end().  It is important to call AddParameter::end() even if no 
    parameters are passed and no return value is expected as this call does the 
    actual execution.  If AddParameter::end() is not called then nothing will be executed.

@include lua_factorial_example.cpp

@section objects Objects

    By default C++ objects are stored in %Lua as user data.  C++ objects passed to 
    %Lua are copy constructed into user data values.  When these user data values are 
    garbage collected the destructor for the c++ object is automatically called and 
    the memory allocated for them is freed.  
    
    More usefully C++ objects can be stored in %Lua as tables.  C++ objects stored 
    as tables are automatically converted to and from their equivalent table when 
    passed to and from %Lua.
    
    The C++ class of objects to be represented by reference must have its storage type 
    trait defined to LuaReference using the SWEET_LUA_TYPE_CONVERSION() macro so that 
    the correct conversions are performed when passing objects of that class between 
    C++ and %Lua.
    
    %Lua tables that are associated with C++ objects are explicitly created and 
    destroyed by users of the library through calls to Lua::create(), 
    Lua::create_with_existing_table(), and Lua::destroy().
    
    Once the table has been created its fields can be set to the values of C++ booleans, 
    integers, floats, strings, objects, and functions.  The fields can be cleared by 
    setting their value to nil.  See Lua::members() and LuaObject::members().
    
    Two important fields that must always be set are the '__type' and '__this' fields 
    that store the type information and this pointer for the C++ object respectively.  
    This must be done explicitly by calling the AddMember::type() and 
    AddMember::this_pointer() functions.

    The value of a variable can be set to a member function.  The first parameter of
    these calls is always assumed to provide the this pointer of the object to make 
    the call on.  This works with %Lua's mechanism of implicitly providing the 'self' 
    parameter to calls made using the ':' syntax.  The %Lua table passed as the 
    implicit 'self' parameter is converted into its equivalent C++ object using 
    its '__type' and '__this' fields.

    Member functions that have been defined in a %Lua script (using the ":" syntax or that
    take an explicit self parameter) on an object that corresponds to a C++ object can be 
    called using the overload of the Lua::call() function that takes an object as its 
    second parameter.  This looks up the function as a member of the object's table 
    but doesn't pass the object's table as the implicit first parameter to the call 
    (the `self` parameter) - this must be done by the application itself.  This gives 
    the application the opportunity to provide a different value for the 'self' 
    parameter.

@include lua_logging_example.cpp

@section classes_and_prototypes Classes and Prototypes

    %Lua is a prototype based language.  

    A prototype defines the behaviour of an object in a prototype based language.
    The relationship between an object and its prototype in a prototype based language
    is similar to the relationship between an object and a class in C++.

    The prototype of an object is another object that index operations that fail on
    the first object are resolved on.  For example if a script calls the `get_value`
    function on an object but that doesn't define a `get_value` function then the
    'get_value' function will then be looked up on that object's prototype.  Because 
    prototypes are themselves objects they can also have prototypes allowing for
    multiple levels of inheritance.

    For more descriptions of object oriented programming in %Lua and prototype based
    languages in general have a look at:
        - "Programming in Lua" by Roberto Ierusalimschy.
        - http://lua-users.org/wiki/SampleCode.
        - http://en.wikipedia.org/wiki/Prototype-based_programming

    Prototypes can be provided using the library by creating a %LuaObject to use
    as the prototype table and a %LuaObject to use as the metatable that redirects
    failed index operations to the prototype.  One or more objects are then set to 
    use that prototype by setting their metatable to be the metatable created by 
    the second %LuaObject.  See AddMember::metatable().
    
    Note that in the following example the prototype table stores the '__type' 
    field while the object itself stores the '__this' field.  This is correct.  
    It is the prototype that is representing the type in this scenario while 
    the object itself must provide the this pointer in order to resolve back 
    to the correct C++ object.
    
@include lua_logging_prototype_example.cpp

@section weak-objects Weak Objects

    Often an application will give %Lua script ownership of the lifetime of the
    objects that it creates.  The simplest way to do this is to store an
    object by value in the %Lua table to manage the lifetime of the C++ object
    (for example a reference counted %pointer back to the original C++ object).  
    This unfortunately causes a memory leak because the C++ object is associated 
    with the table through a strong reference stored in the %Lua registry at the
    same time as the table is referencing the C++ object via a reference 
    counted smart pointer or some other mechanism that maintains a strong reference.  
    This cycle allows the C++ object and the %Lua table to live forever.

    To break the cycle the table can be stored in a special weak objects table.
    This weakens the reference from the C++ object to the %Lua table allowing
    %Lua to garbage collect the table when it is no longer referenced (because 
    the C++ side is no longer holding a strong reference to it only the references
    that %Lua itself holds are counted towards keeping it alive) and when this 
    happens, provided that the lifetime management object (e.g. the reference 
    counted %pointer) has an appropriate destructor, the C++ object will also 
    be destroyed.

    Objects are weakened when returned from C++ functions that have been registered
    using the weaken() policy.  The weakened table is moved from the %Lua registry
    to the weak objects table while a strong reference is still held to it from
    the returned value on the %Lua stack.  Weakening is implemented this way so that 
    there is always a strong reference to the object to prevent it being garbage 
    collected at least up until the point where user code can maintain its own strong 
    reference to it.  This prevents the weak object being garbage collected before 
    user code has had an opportunity to do something with it.
    
    The typical implementation of this uses shared_ptrs with a custom deleter to
    ensure that Lua::destroy() is called to remove the dangling weak reference 
    from the weak objects table.

@include lua_logging_owned_by_lua_example.cpp

@section coroutines Coroutines

    The library provides support for coroutines through the %LuaThread object and
    the yield() function that is used to generate a C++ function that yields after 
    it has been called.

    The %LuaThread object is constructed just like a %LuaObject.  Once constructed
    it then behaves similarly to a %Lua object except that it can resume functions 
    as well as call them.

    The yield() function is used to wrap the function %pointer passed in when 
    setting the value of C++ functions.  It marks the function as needing to generate 
    a %yield on return instead of returning control to %Lua.

@section standard_template_library_integration Standard Template Library Integration

    There is a small amount of integration with the Standard Template Library.  The
    standard containers vector, list, set, multiset, map, and multimap can all be
    implicitly converted into %Lua iterators that iterates over their [begin, end)
    sequences.

    There is also a 'lua_push()' implementation that will take two STL iterators (\e
    first and \e last) and convert them into a %Lua iterator that iterates over the
    [\e first, \e last) sequence.  This makes it simple to convert other C++ sequences
    to %Lua iterators by providing the right overloaded 'lua_push()' function and
    converting the sequence into a %Lua iterator.

@include lua_vector_example.cpp

@section boost_integration Boost Integration

    There is a small amount of integration with the Boost Filesystem library.  The
    basic_directory_iterator<> and basic_recursive_directory_iterator<> iterators 
    are implicitly converted into %Lua iterators that iterate from the specified 
    iterator to the default constructed "end" iterator.  The basic_directory_enrty<>
    class template is implicitly converted into a string.

@code
#include <sweet/lua/filesystem.hpp>
@endcode

*/
namespace lua
{

}

}
