//
// Option.hpp
// Copyright (c) 2008 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_CMDLINE_OPTION_HPP_INCLUDED
#define SWEET_CMDLINE_OPTION_HPP_INCLUDED

#include "declspec.hpp"
#include <string>

namespace sweet
{

namespace cmdline
{

/**
// @internal
//
// The type of data provided by a command line option.
*/
enum OptionType
{
    OPTION_BOOL,
    OPTION_INT,
    OPTION_FLOAT,
    OPTION_STRING
};

/**
// @internal
//
// An option that can be parsed from the command line.
*/
class SWEET_CMDLINE_DECLSPEC Option
{
    std::string name_; ///< The name of the option.
    std::string short_name_; ///< The short name of the option.
    std::string description_; ///< The description of the option.
    void* address_; ///< The address of the variable to receive the value of the option.
    OptionType type_; ///< The type of the option.

    public:
        Option( const std::string& name, const std::string& short_name, const std::string& description, void* address, OptionType type );
        const std::string& get_name() const;
        const std::string& get_short_name() const;
        const std::string& get_description() const;
        void* get_address() const;
        OptionType get_type() const;
};

}

}

#endif
