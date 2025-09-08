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

        // FORMATTABLE TYPE DETECTION
        // PRESUPPOSE A GENERIC RETURN ARG
        template<typename T, typename ...>
        struct IS_FORMATTABLE : std::false_type {};

        // FORMATTABLE TYPE DETECTION
        // BY QUERYING THE TYPE OF AN EXPRESSION BASED ON THE VALUE PROVIDED
        // GREAT FOR HAVING A DETERMINISTIC RETURN TYPE
        template<typename T>
        struct IS_FORMATTABLE<T, std::void_t<decltype(fmt::format("{}", std::declval<T>()))>> : std::true_type {};

        // FORMATTABLE TYPE DETECTION
        // BASED ON A BOOLEAN EXPRESSION
        template<typename T>
        constexpr bool IS_FORMATTABLE_V = IS_FORMATTABLE<T>::value;

        enum class ERROR_CATEGORY : U8
        {
            LOGIC_ERR = 0,
            RUNTIME_ERR,
            RES_ERR,
            INVALID_ARG,
            OOB,
            NULL_PTR,
            UNIMPL,
            UNREACH,
            SYS_ERR,
            CUSTOM_ERR
        };

        enum class ERROR_SEVERITY : U8
        {
            FATAL = 0,
            CRITICAL,
            STD_ERROR,
            WARNING,
            INFO
        };

        // ERROR CONTEXT HANDLER WHICH PRESUPPOSES THE GENERIC TYPES
        // ASSOCIATED WITH AN ERROR TYPE
        template<typename CODE_TYPE = int, typename MSG_TYPE = std::string, typename ...>
        struct ERROR_CTX
        {
            using ERROR_CODE = CODE_TYPE;
            using ERROR_MSG = MSG_TYPE;
            CODE_TYPE CODE;
            MSG_TYPE MSG;

            ERROR_CATEGORY CATEGORY;
            ERROR_SEVERITY SEVERITY;

            const char* FILE;
            int LINE;

            ERROR_CTX(CODE_TYPE C, MSG_TYPE M, ERROR_CATEGORY CAT, 
                    ERROR_SEVERITY SEV, const char* FILE = __FILE__, int LINE = __LINE__)
            : CODE(C), MSG(M), CATEGORY(CAT), SEVERITY(SEV), FILE(FILE), LINE(LINE) {}
        };
    }
}

#endif
