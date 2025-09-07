// COPYRIGHT (C) HARRY CLARK 2025
// A LIGHTWEIGHT IMPLEMENTATION OF COMMON UTILITIES 

// THE FOLLOWING FILE PERTAINS TOWARDS THE MODULARISATION AND VERBOSITY OF
// MY ERROR HANDLING SCHEME

#ifndef ERROR_HH
#define ERROR_HH

// NESTED INCLUDES  

#include <common.hh>
#include <fmt/format.h>

// SYSTEM INCLUDES

#include <string>
#include <type_traits>
#include <utility>

namespace noodle
{
    namespace err
    {
        // TEMPLATING FOR HANDLING GENERIC ERROR CODE TYPES
        template<typename T>
        using ERROR_CODE = std::enable_if_t<std::is_enum<T> || std::is_integral<T>>;

        // TEMPLAING FOR HANDLING GENERIC ERROR MESSAGE TYPES
        template<typename T>
        using ERROR_MESSAGE = std::enable_if_t<std::is_convertible<T, std::string_view>>;
    }
}

#endif