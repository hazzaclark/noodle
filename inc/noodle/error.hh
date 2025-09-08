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
        using ERROR_CODE = typename std::enable_if<std::is_enum<T>::value || std::is_integral<T>::value, T>::type;

        // TEMPLATING FOR HANDLING GENERIC ERROR MESSAGE TYPES
        template<typename T>
        using ERROR_MESSAGE = typename std::enable_if<std::is_convertible<T, std::string>::value, T>::type;

        // FORMATTABLE TYPE DETECTION
        // PRESUPPOSE A GENERIC RETURN ARG
        template<typename T, typename = void>
        struct IS_FORMATTABLE : std::false_type {};

        // FORMATTABLE TYPE DETECTION
        // BY QUERYING THE TYPE OF AN EXPRESSION BASED ON THE VALUE PROVIDED
        // GREAT FOR HAVING A DETERMINISTIC RETURN TYPE
        template<typename T>
        struct IS_FORMATTABLE<T, typename std::enable_if<true, decltype(fmt::format("{}", std::declval<T>()))>::type> : std::true_type {};

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
        template<typename CODE_TYPE = int, typename MSG_TYPE = std::string>
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

            // INITIALISER WITH BASE CONSTRUCTOR ARGS
            ERROR_CTX(CODE_TYPE C, MSG_TYPE M, ERROR_CATEGORY CAT, 
                    ERROR_SEVERITY SEV, const char* F = __FILE__, int L = __LINE__)
            : CODE(C), MSG(std::move(M)), CATEGORY(CAT), SEVERITY(SEV), FILE(F), LINE(L) {}
        };

        // COMMON ERROR TYPES - PRESUPPOSED WITH GENERIC ARGS IN RELATION
        // TO THE ERROR CONTEXT TO BE ABLE TO HANDLE A WIDE VARIETY OF TYPES
        using NOODLE_STD_ERROR = ERROR_CTX<int, std::string>;

        // HELPER FUNCTION TO BE ABLE TO FORMAT A MESSAGE LEVERAGING FMT
        template<typename STR, typename... ARGS>
        static inline std::string NOODLE_FMT(STR FMT_STR, ARGS&&... A)
        {
            return sizeof...(ARGS) > 0 
                ? fmt::format(FMT_STR, std::forward<ARGS>(A)...)
                : std::string(FMT_STR);
        }
    }
}

    // PRE-PROCESSOR MACROS TO HELP WITH COMPATIBILITY
    // 08/09/25 - decltype NEEDS TO BE ADDED TO LEVERAGE GENERIC TYPES
    #define     NOODLE_ERROR_CTX(CODE, MSG, CAT, SEV) \
    noodle::err::ERROR_CTX<decltype(CODE), std::string>(CODE, std::string(MSG), CAT, SEV, __FILE__, __LINE__)

    #define     NOODLE_ERROR_FMT(CODE, CAT, SEV, FMT_STR, ...) \
    noodle::err::NOODLE_STD_ERROR(CODE, noodle::err::NOODLE_FMT(FMT_STR, ##__VA_ARGS__), \
                                CAT, SEV, __FILE__, __LINE__)

#endif