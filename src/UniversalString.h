#ifndef UNIVERSALSTRING_H
#define UNIVERSALSTRING_H

#include <string>
#include <string_view>
#include "Common.h"

namespace CIo
{
    //This class is ment as a input for string outputing or
    // altering functions
    // this is due to the fact that std::basic_string cannot point
    // to statically allocated data and outputing to static buffers
    // is often desired.
    template<typename CharT>
    struct CStringRef
    {
            using CharType = CharT;
            using SizeType = size_t;
            using ThisType = CStringRef;

            const SizeType Size = {0};
            CharType PTR const Data = {nullptr};

            constexpr CStringRef(CharType PTR dataPtr, SizeType size) noexcept
                : Size(size), Data(dataPtr)
            {}

            constexpr CStringRef(std::basic_string<CharType> REF str) noexcept
                : Size(str.size()), Data(str.data())
            {}

            constexpr CStringRef() noexcept = default;
            constexpr CStringRef(const ThisType REF) noexcept = default;
            constexpr CStringRef(ThisType RVALUE_REF) noexcept = default;

            constexpr operator std::basic_string_view<CharType>() const noexcept
            {
                return std::basic_string_view<CharType>(Data, Size);
            }
    };

    template<typename CharType>
    constexpr inline auto ToCStringRef(std::basic_string<CharType> REF arg) noexcept
    {
        return  CStringRef<CharType>(arg);
    }

    template<typename CharType>
    constexpr inline auto ToCStringRef(CharType PTR dataPtr, typename CStringRef<CharType>::SizeType size) noexcept
    {
        return  CStringRef<CharType>(dataPtr, size);
    }
}

namespace CIo
{
    using FalseType = std::false_type;
    using TrueType = std::true_type;

    namespace Detail
    {
        template<typename Type>
        struct TypeIdentity
        {
                using type = Type;
        };

        template <typename T>
        struct GetStringClassTypeImpl : TypeIdentity<FalseType> {};

        template <typename T>
        struct GetStringClassTypeImpl<std::basic_string<T>> : TypeIdentity<T> {};

        template <typename T>
        struct GetStringClassTypeImpl<std::basic_string_view<T>> : TypeIdentity<T> {};

        template <typename T>
        struct GetStringClassTypeImpl<CStringRef<T>> : TypeIdentity<T> {};
    }

    //GetStringClassType
    template <typename T>
    struct GetStringClassType : Detail::GetStringClassTypeImpl<std::decay_t<T>> {};

    template <typename T>
    using GetStringClassType_t = typename GetStringClassType<T>::type;


    //IsStringClass
    template <typename T>
    struct IsStringClass : std::bool_constant<NOT std::is_same_v<typename GetStringClassType<T>::type, FalseType>>{};

    template <typename T>
    constexpr bool IsStringClass_v{IsStringClass<T>::value};


    //GetCStringType
    template<typename T>
    struct GetCStringType : std::decay<std::remove_pointer_t<std::decay_t<T>>>
    {};

    template<typename T>
    using GetCStringType_t = typename GetCStringType<T>::type;


    //IsCString
    template<class T>
    struct IsCString : std::bool_constant<
          std::is_same_v<GetCStringType_t<T> const *,   std::decay_t<T>> ||
          std::is_same_v<GetCStringType_t<T> *,         std::decay_t<T>>>
    {};

    template <typename T>
    constexpr bool IsCString_v{IsCString<T>::value};


    //IsStringLiteral
    template<typename T>
    struct IsStringLiteral :
        std::is_same<T, std::add_lvalue_reference_t<const GetCStringType_t<T> [std::extent_v<std::remove_reference_t<T>>]>>
    {};

    template <typename T>
    constexpr bool IsStringLiteral_v{IsStringLiteral<T>::value};


    //IsAnyString
    template<typename T>
    struct IsAnyString : std::bool_constant<IsCString_v<T> || IsStringClass_v<T>>
    {};

    template<typename T>
    constexpr bool IsAnyString_v {IsAnyString<T>::value};


    //GetAnyStringType
    template<typename T>
    struct GetAnyStringType : std::conditional<IsCString<T>::value, GetCStringType_t<T>, GetStringClassType_t<T>>
    {};

    template<typename T>
    using GetAnyStringType_t = typename GetAnyStringType<T>::type;
}
#endif // UNIVERSALSTRINGNEW_H
