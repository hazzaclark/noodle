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
#include <atomic>
#include <string>
#include <type_traits>
#include <utility>

namespace noodle
{
    namespace err
    {
        // TEMPLATING FOR HANDLING GENERIC ERROR MESSAGE TYPES
        template<typename T>
        using ERROR_MESSAGE = typename std::enable_if<std::is_convertible<T, std::string>::value, T>::type;

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

        // GLOBAL THREAD-SAFE IMPL FOR ERROR COUNT
        static std::atomic<int> ERROR_COUNT{0};

        // STANDARD HELPER FUNCTION TO ACCOMMODATE FOR ERROR COUNT
        #define GET_ERROR_CODE() (ERROR_COUNT.fetch_add(1))

        // STANDARD HELPER FUNCTION TO ACCOMMODATE FOR ERROR_SEVERITY
        #define GET_ERR_SEVERITY(SEV)                       \
        ((SEV) == ERROR_SEVERITY::FATAL ? "FATAL" :         \
        (SEV) == ERROR_SEVERITY::CRITICAL ? "CRITICAL" :    \
        (SEV) == ERROR_SEVERITY::STD_ERROR ? "ERROR" :      \
        (SEV) == ERROR_SEVERITY::WARNING ? "WARNING" :      \
        (SEV) == ERROR_SEVERITY::INFO ? "INFO" :            \
        "UNKNOWN")

        // STANDARD HELPER FUNCTION TO ACCOMMODATE FOR ERROR_CATEGORY
        #define GET_ERR_CATEGORY(CAT)                       \
        ((CAT) == ERROR_CATEGORY::LOGIC_ERR ? "LOGIC" :     \
        (CAT) == ERROR_CATEGORY::RUNTIME_ERR ? "RUNTIME" :  \
        (CAT) == ERROR_CATEGORY::RES_ERR ? "RESOURCE" :     \
        (CAT) == ERROR_CATEGORY::INVALID_ARG ? "INVALID_ARG" : \
        (CAT) == ERROR_CATEGORY::OOB ? "OUT_OF_BOUNDS" :    \
        (CAT) == ERROR_CATEGORY::NULL_PTR ? "NULL_POINTER" : \
        (CAT) == ERROR_CATEGORY::UNIMPL ? "UNIMPLEMENTED" : \
        (CAT) == ERROR_CATEGORY::UNREACH ? "UNREACHABLE" :  \
        (CAT) == ERROR_CATEGORY::SYS_ERR ? "SYSTEM" :       \
        (CAT) == ERROR_CATEGORY::CUSTOM_ERR ? "CUSTOM" :    \
        "UNKNOWN")

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
            // UPDATED TO INCLUDE AN AUTO-INCREMENT ARG FOR CATCHING ERROR CODES
            ERROR_CTX(MSG_TYPE M, ERROR_CATEGORY CAT, 
                    ERROR_SEVERITY SEV, const char* F = __FILE__, int L = __LINE__)
            : CODE(static_cast<CODE_TYPE>(GET_ERROR_CODE())), 
              MSG(std::move(M)), CATEGORY(CAT), SEVERITY(SEV), FILE(F), LINE(L) {}
        };

        // COMMON ERROR TYPES - PRESUPPOSED WITH GENERIC ARGS IN RELATION
        // TO THE ERROR CONTEXT TO BE ABLE TO HANDLE A WIDE VARIETY OF TYPES
        using NOODLE_STD_ERROR = ERROR_CTX<int, std::string>;

        // HELPER FUNCTION TO BE ABLE TO FORMAT A MESSAGE LEVERAGING FMT
        template<typename STR, typename... ARGS>
        static inline std::string NOODLE_FMT(ERROR_SEVERITY SEV, STR FMT_STR, ARGS&&... A)
        {
            std::string FMT = sizeof...(ARGS) > 0 
                ? fmt::format(FMT_STR, std::forward<ARGS>(A)...)
                : std::string(FMT_STR);
            
            return fmt::format("{}: {} - {}", GET_ERR_SEVERITY(SEV), GET_ERROR_CODE(), FMT);
        }

        // OVERLOAD FOR BACKWARD COMPATIBILITY
        template<typename STR, typename... ARGS>
        static inline std::string NOODLE_FMT(STR FMT_STR, ARGS&&... A)
        {
            return NOODLE_FMT(ERROR_SEVERITY::INFO, FMT_STR, std::forward<ARGS>(A)...);
        }

        // EXTRA OVERLOAD FUNCTION FOR PRINTING THE ERROR DIRECTLY
        // AUTOMATICALLY FORMAT THE MESSAGE BEFORE THE PRINT STATEMENT
        // THIS PRESUPPOSES A GENERIC LENGTH FOR THE MESSAGE AGAINST THE FMT ARG
        template<typename... ARGS>
        static inline void NOODLE_PRINT(ERROR_CATEGORY CAT, ERROR_SEVERITY SEV,
                                        const char* FILE, int LINE,
                                        const std::string& FMT_STR, ARGS&&... A)
        {
            int ERR_CODE = GET_ERROR_CODE();
            std::string MSG = sizeof ...(A) > 0
                ? fmt::format(FMT_STR, std::forward<ARGS>(A)...)
                : FMT_STR;

            fmt::print("[{}] [{}]       \nERROR: {} - {} ({}:{})\n\n", 
                      GET_ERR_SEVERITY(SEV), GET_ERR_CATEGORY(CAT), 
                      ERR_CODE, MSG, FILE, LINE);
        }

        // BASELINE PRINT ARGUMENT ADJACENT FROM ERROR FORMATTING
        template<typename... ARGS>
        static inline void NOODLE_PRINT_BASIC(ARGS&&... A)
        {
            fmt::print(std::forward<ARGS>(A)...);
        }
    }
}

    // PRE-PROCESSOR MACROS TO HELP WITH COMPATIBILITY
    // THE FOLLOWING ACCESSES THE OVERALL PRINT TEMPLATE 
    // AND PROVIDES UNIQUE MACROS FOR CONTEXT SPECIFIC HANDLERS

    #define NOODLE_ERROR_PRINT(CAT, SEV, FMT_STR, ...) \
    noodle::err::NOODLE_PRINT(CAT, SEV, __FILE__, __LINE__, FMT_STR, ##__VA_ARGS__)

    #define NOODLE_FATAL(FMT_STR, ...) \
    noodle::err::NOODLE_PRINT(noodle::err::ERROR_CATEGORY::CUSTOM_ERR, \
                             noodle::err::ERROR_SEVERITY::FATAL, \
                             __FILE__, __LINE__, FMT_STR, ##__VA_ARGS__)

    #define NOODLE_CRITICAL(FMT_STR, ...) \
    noodle::err::NOODLE_PRINT(noodle::err::ERROR_CATEGORY::CUSTOM_ERR, \
                             noodle::err::ERROR_SEVERITY::CRITICAL, \
                             __FILE__, __LINE__, FMT_STR, ##__VA_ARGS__)

    #define NOODLE_ERROR(FMT_STR, ...) \
    noodle::err::NOODLE_PRINT(noodle::err::ERROR_CATEGORY::CUSTOM_ERR, \
                             noodle::err::ERROR_SEVERITY::STD_ERROR, \
                             __FILE__, __LINE__, FMT_STR, ##__VA_ARGS__)

    #define NOODLE_WARNING(FMT_STR, ...) \
    noodle::err::NOODLE_PRINT(noodle::err::ERROR_CATEGORY::CUSTOM_ERR, \
                             noodle::err::ERROR_SEVERITY::WARNING, \
                             __FILE__, __LINE__, FMT_STR, ##__VA_ARGS__)

    #define NOODLE_INFO(FMT_STR, ...) \
    noodle::err::NOODLE_PRINT(noodle::err::ERROR_CATEGORY::CUSTOM_ERR, \
                             noodle::err::ERROR_SEVERITY::INFO, \
                             __FILE__, __LINE__, FMT_STR, ##__VA_ARGS__)

    #define NOODLE_LOGIC_ERROR(FMT_STR, ...) \
    noodle::err::NOODLE_PRINT(noodle::err::ERROR_CATEGORY::LOGIC_ERR, \
                             noodle::err::ERROR_SEVERITY::STD_ERROR, \
                             __FILE__, __LINE__, FMT_STR, ##__VA_ARGS__)

    #define NOODLE_RUNTIME_ERROR(FMT_STR, ...) \
    noodle::err::NOODLE_PRINT(noodle::err::ERROR_CATEGORY::RUNTIME_ERR, \
                             noodle::err::ERROR_SEVERITY::STD_ERROR, \
                             __FILE__, __LINE__, FMT_STR, ##__VA_ARGS__)

    #define NOODLE_RESOURCE_ERROR(FMT_STR, ...) \
    noodle::err::NOODLE_PRINT(noodle::err::ERROR_CATEGORY::RES_ERR, \
                             noodle::err::ERROR_SEVERITY::STD_ERROR, \
                             __FILE__, __LINE__, FMT_STR, ##__VA_ARGS__)

    #define NOODLE_INVALID_ARG_ERROR(FMT_STR, ...) \
    noodle::err::NOODLE_PRINT(noodle::err::ERROR_CATEGORY::INVALID_ARG, \
                             noodle::err::ERROR_SEVERITY::STD_ERROR, \
                             __FILE__, __LINE__, FMT_STR, ##__VA_ARGS__)

    #define NOODLE_OOB_ERROR(FMT_STR, ...) \
    noodle::err::NOODLE_PRINT(noodle::err::ERROR_CATEGORY::OOB, \
                             noodle::err::ERROR_SEVERITY::STD_ERROR, \
                             __FILE__, __LINE__, FMT_STR, ##__VA_ARGS__)

    #define NOODLE_NULL_PTR_ERROR(FMT_STR, ...) \
    noodle::err::NOODLE_PRINT(noodle::err::ERROR_CATEGORY::NULL_PTR, \
                             noodle::err::ERROR_SEVERITY::STD_ERROR, \
                             __FILE__, __LINE__, FMT_STR, ##__VA_ARGS__)

    #define NOODLE_UNIMPL_ERROR(FMT_STR, ...) \
    noodle::err::NOODLE_PRINT(noodle::err::ERROR_CATEGORY::UNIMPL, \
                             noodle::err::ERROR_SEVERITY::STD_ERROR, \
                             __FILE__, __LINE__, FMT_STR, ##__VA_ARGS__)

    #define NOODLE_UNREACH_ERROR(FMT_STR, ...) \
    noodle::err::NOODLE_PRINT(noodle::err::ERROR_CATEGORY::UNREACH, \
                             noodle::err::ERROR_SEVERITY::STD_ERROR, \
                             __FILE__, __LINE__, FMT_STR, ##__VA_ARGS__)

    #define NOODLE_SYSTEM_ERROR(FMT_STR, ...) \
    noodle::err::NOODLE_PRINT(noodle::err::ERROR_CATEGORY::SYS_ERR, \
                             noodle::err::ERROR_SEVERITY::STD_ERROR, \
                             __FILE__, __LINE__, FMT_STR, ##__VA_ARGS__)

#endif
