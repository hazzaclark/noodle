// COPYRIGHT (C) HARRY CLARK 2025
// A LIGHTWEIGHT IMPLEMENTATION OF COMMON UTILITIES 

// THIS FILE PERTAINS TOWARDS THE MODULARISATION OF A FULL FLEDGED MEMORY MAPPER UTILITY
// MORE SPECIFICALLY, THIS IS A PORT OF THE PRESUPPOSED MEMORY MAPPER DESIGNED SPECIFICALLY FOR
// SYSTEM EMULATION - CONSTRUCTED OUT OF A VERBOSE MEMORY PAGING SCHEME

// THE FOLLOWING WILL AIM TO PROVIDE AN EASE OF USE MEANS OF BEING ABLE TO INCLUDE
// THAT ACROSS MULTIPLE INSTANCES - SHOULD IT BE APPLICABLE FOR THE DESIGN OF SAID PROJECT
// TEMPLATING HELPS ACCORDINGLY WITH THIS TO BE ABLE TO PRESUPPOSE AND FILL IN ANY GENERIC ARGS NEEDED

#ifndef MEMORY_HH
#define MEMORY_HH

// NESTED INCLUDES

#include <common.hh>

// SYSTEM INCLUDES

#include <array>
#include <functional>
#include <memory>
#include <vector>

namespace fujiko
{
    namespace memory
    {
        // GENERIC RAW POINTERS TO HELP WITH READ AND WRITES
        using FUJIKO_READ_8     = std::function<U8(U32 ADDRESS, void* CTX)>;
        using FUJIKO_READ_16    = std::function<U16(U32 ADDRESS, void* CTX)>;
        using FUJIKO_READ_32    = std::function<U32(U32 ADDRESS, void* CTX)>;

        using FUJIKO_WRITE_8    = std::function<void(U32, U8, void*)>;
        using FUJIKO_WRITE_16   = std::function<void(U32, U16, void*)>;
        using FUJIKO_WRITE_32   = std::function<void(U32, U32, void*)>;

        // GENERIC CONVERTIBLE CLAUSE FOR BEING ABLE TO CONVERT BETWEEN TWO GENERICS
        // RETURNS: AN ASSIGNED INTEGRAL CONSTANT
        template <typename T1, typename T2>
        static inline constexpr bool IS_ASSIGNABLE = std::is_convertible<T1, T2>::value;

        // THE FOLLOWING SERVES TO PROVIDE A TYPE-SAFE MEANS OF BEING ABLE TO ASSIGN
        // RELEVANT SIZES, AND ARGUMENTS IN RELATION TO THE SIZE OF THE MEMORY BUS
        // WHICH PRESUPPOSES THE ARGS PROVIDED THROUGH A GENERIC
        template<typename T>
        static inline constexpr bool FUJIKO_BUS_HANDLER = IS_ASSIGNABLE<FUJIKO_READ_8, T> 
                                                        || IS_ASSIGNABLE<FUJIKO_READ_16, T>
                                                        || IS_ASSIGNABLE<FUJIKO_READ_32, T>
                                                        || IS_ASSIGNABLE<FUJIKO_WRITE_8, T>
                                                        || IS_ASSIGNABLE<FUJIKO_WRITE_16, T>
                                                        || IS_ASSIGNABLE<FUJIKO_WRITE_32, T>;


        // THE FOLLOWING REPRESENTS THE OVERARCHING BUS INTERCONNECTING COMPONENTS
        // KEEP IN MIND THAT THIS IS QUITE A DEPARTURE FROM A STANDARD EMULATION PERSAY.
        // AS WE ARE ONLY CONCERNED WITH PROVIDING A BASE FOR PAGING, MEMORY MANAGEMENT AND BASIC R/W.
        // WHENEVER WE CREATE AN INSTANCE OF THE MEMORY BUS, WE WILL PRESUPPOSE
        // THE DEFAULT ELEMENTS TO BE THE FOLLOWING:
        //
        // ADDRESS BITS - TO BE OF THAT IN RELATION TO THE AMOUNT OF REGISTERS
        // PAGE GRANULARITY - DEFINE THE SMALLEST UNIT OF PROTECTED MEMORY
        //
        // THE FOLLOWING PARAMETERS CAN BE CHANGED TO SUIT ANY AND ALL CIRCUMSTANCE
        class MEMORY_BUS
        {
            public:
                static constexpr U32 ADDRESS_BITS = 27;
                static constexpr U32 ADDRESS_MASK = (1U << ADDRESS_BITS) - 1;
                static constexpr U32 PAGE_BITS = 16;
                static constexpr U32 PAGE_SIZE = 1U << PAGE_BITS;
                static constexpr U32 PAGE_MASK = PAGE_SIZE - 1;
                static constexpr U32 PAGE_COUNT = (1U << (ADDRESS_BITS - PAGE_BITS));

            private:

                // GENERIC BASELINE IMPLEMMENTATION OF A MEMORY PAGE
                // UTILISES SMART POINTERS TO HELP WITH DELETE AND MOVE CONSTRUCTS
                struct MEMORY_PAGE
                {
                    std::unique_ptr<U8> ARRAY;
                    std::function<void>& CTX;
                    bool WRITEABLE = false;
                    bool READONLY = false;
                };
        };
    }
}

#endif
