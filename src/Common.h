#ifndef COMMON_H
#define COMMON_H

#include <memory>

//Operator readability
#define NOT                     !
#define REF                     &
#define RVALUE_REF              &&
#define ADDRESS(item)           (::std::addressof(item))
#define PTR                     *
#define PTR_VAL(pointer)        (*pointer)

namespace cio
{
    using byte  = uint8_t;

    using i8    = int8_t;
    using i16   = int16_t;
    using i32   = int32_t;
    using i64   = int64_t;
    using u8    = uint8_t;
    using u16   = uint16_t;
    using u32   = uint32_t;
    using u64   = uint64_t;

    using char8  = char;
    using char16 = char16_t;
    using char32 = char32_t;
    using charW  = wchar_t;

    using c8 = char8;
    using c16 = char16;
    using c32 = char32;
    using cW = charW;

    using b8 = bool;

    using d32 = float;
    using d64 = double;
}

#endif // COMMON_H
