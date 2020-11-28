#ifndef UNIVERSALSTRING_H
#define UNIVERSALSTRING_H

#include <string>
#include <string_view>
#include "Common.h"
namespace cio
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
    }

    //GetStringClassType
    template <typename T>
    struct GetStringClassType : Detail::GetStringClassTypeImpl<std::decay_t<T>> {};

    template <typename T>
    using GetStringClassType_t = typename GetStringClassType<T>::type;

    //IsCharType
    template <typename T>
    struct IsCharType : std::bool_constant<std::is_same_v<T, char8> || std::is_same_v<T, char16> || std::is_same_v<T, char32> || std::is_same_v<T, charW>>{};

    template <typename T>
    constexpr bool IsCharType_v{IsCharType<T>::value};

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
