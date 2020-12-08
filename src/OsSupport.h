#ifndef COMPILERSUPPORT_H
#define COMPILERSUPPORT_H

//Define one of the following before including cio library
// if automatic detection of OS didnt work properly:

//#define POSIX_OS
//#define WINDOWS_OS
//#define PURE_C

//Automatically detects target OS
#if defined (POSIX_OS) || defined (WINDOWS_OS) || defined (PURE_C)
#define OS_USER_DECLARED
#define OS_FOUND
#endif

#ifndef OS_USER_DECLARED

    #if defined (__linux__) || defined (__linux) || \
        defined (__unix__) || defined (__unix) || \
        defined (__posix__) || defined(__posix) || defined (_POSIX_VERSION) || \
        defined (__APPLE__)

        #define OS_FOUND
        #define POSIX_OS
    #endif

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        #ifndef OS_FOUND
            #define OS_FOUND
            #define WINDOWS_OS
        #endif
    #endif

#endif

#ifndef OS_FOUND
#define OS_FOUND
#define PURE_C
#endif

#undef OS_FOUND
#undef OS_USER_DECLARED


#ifdef POSIX_OS
    #define _FILE_OFFSET_BITS 64
    #define _LARGEFILE_SOURCE
#endif

//Removes deprecated warning on fopen under MSVC compiler
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdio>
#include "Common.h"

namespace cio::OsSupport
{

#ifdef WINDOWS_OS
    using OffsetType = __int64;

    inline auto fopen(const char8 * fileName, const char8 * arguments) noexcept
    {
        return std::fopen(fileName, arguments);
    }

    inline auto ftell(FILE PTR stream) noexcept
    {
        return ::_ftelli64(stream);
    }

    inline auto fseek(FILE PTR stream, OffsetType offset, i32 origin) noexcept
    {
        return ::_fseeki64(stream, offset, origin);
    }
#endif

#ifdef POSIX_OS
#include <unistd.h>

    using OffsetType = off64_t;

    inline auto fopen(const char8 * fileName, const char8 * arguments) noexcept
    {
        return ::fopen64(fileName, arguments);
    }
    inline auto ftell(FILE PTR stream) noexcept
    {
        return ::ftello64(stream);
    }

    inline auto fseek(FILE PTR stream, OffsetType offset, i32 origin) noexcept
    {
        return ::fseeko64(stream, offset, origin);
    }
#endif

#ifdef PURE_C
    using OffsetType = decltype (std::ftell(nullptr));

    inline auto fopen(const char8 * fileName, const char8 * arguments) noexcept
    {
        return std::fopen(fileName, arguments);
    }

    inline auto ftell(FILE PTR stream) noexcept
    {
        return std::ftell(stream);
    }

    inline auto fseek(FILE PTR stream, OffsetType offset, i32 origin) noexcept
    {
        return std::fseek(stream, offset, origin);
    }

#endif

}


#endif // COMPILERSUPPORT_H
