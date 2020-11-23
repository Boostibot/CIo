#ifndef UNIVERSALSTRINGTESTING_H
#define UNIVERSALSTRINGTESTING_H

#include "Catch2/Catch.hpp"
#include "src/UniversalString.h"

namespace CIo::CStringRefTesting
{
    #define CStringRefTestedTypes char8, charW, char16, char32, int, long, double

    TEMPLATE_TEST_CASE("[CStringRef] : Default constructor should initialise the Size to 0 and Data to nullptr", "[CStringRef][Default constructor]", CStringRefTestedTypes)
    {
        using CStringRef = CStringRef<TestType>;

        const CStringRef cref;
        REQUIRE(cref.Size == 0);
        REQUIRE(cref.Data == nullptr);
    }

    TEMPLATE_TEST_CASE("[CStringRef] : Constructor with dataPtr and size arguments should set the Data and Size accordingly", "[CStringRef][Constructor]", CStringRefTestedTypes)
    {
        using CStringRef = CStringRef<TestType>;

        WHEN("Provided with normal pair of arguemnts they should be set")
        {
            constexpr size_t dataSize = 100;
            TestType data[dataSize] = {};

            const CStringRef cref1(data, dataSize);
            REQUIRE(cref1.Data == data);
            REQUIRE(cref1.Size == dataSize);

            const CStringRef cref2(data, 10);
            REQUIRE(cref2.Data == data);
            REQUIRE(cref2.Size == 10);

            const CStringRef cref3(data + 42, 13);
            REQUIRE(cref3.Data == data + 42);
            REQUIRE(cref3.Size == 13);

            const CStringRef cref4(data + 3, dataSize - 1);
            REQUIRE(cref4.Data == data + 3);
            REQUIRE(cref4.Size == dataSize - 1);

            const CStringRef cref5(data, 0);
            REQUIRE(cref5.Data == data);
            REQUIRE(cref5.Size == 0);
        }

        WHEN("Provided with invalid arguments they should be used")
        {
            constexpr size_t dataSize = 100;
            TestType data[dataSize] = {};

            const CStringRef cref1(nullptr, dataSize);
            REQUIRE(cref1.Data == nullptr);
            REQUIRE(cref1.Size == dataSize);

            const CStringRef cref2(data, dataSize + 10000);
            REQUIRE(cref2.Data == data);
            REQUIRE(cref2.Size == dataSize + 10000);

            const CStringRef cref3(nullptr, 1);
            REQUIRE(cref3.Data == nullptr);
            REQUIRE(cref3.Size == 1);
        }
    }

    TEST_CASE("[CStringRef] : Constructor with std::basic_string argument should set the Data and Size according to the string given", "[CStringRef][Constructor]")
    {
        GIVEN("char8")
        {
            using CStringRef = CStringRef<char8>;
            using String = std::basic_string<char8>;

            {
                String str;
                const CStringRef cref(str);
                REQUIRE(cref.Data == str.data());
                REQUIRE(cref.Size == str.size());
            }
            {
                String str = "";
                const CStringRef cref(str);
                REQUIRE(cref.Data == str.data());
                REQUIRE(cref.Size == str.size());
            }
            {
                String str = "hello world";
                const CStringRef cref(str);
                REQUIRE(cref.Data == str.data());
                REQUIRE(cref.Size == str.size());
            }
            {
                String str = "%@^&@%$!%^~~56728347264728347";
                const CStringRef cref(str);
                REQUIRE(cref.Data == str.data());
                REQUIRE(cref.Size == str.size());
            }
        }
        GIVEN("charW")
        {
            using CStringRef = CStringRef<charW>;
            using String = std::basic_string<charW>;

            {
                String str;
                const CStringRef cref(str);
                REQUIRE(cref.Data == str.data());
                REQUIRE(cref.Size == str.size());
            }
            {
                String str = L"";
                const CStringRef cref(str);
                REQUIRE(cref.Data == str.data());
                REQUIRE(cref.Size == str.size());
            }
            {
                String str = L"hello world";
                const CStringRef cref(str);
                REQUIRE(cref.Data == str.data());
                REQUIRE(cref.Size == str.size());
            }
            {
                String str = L"%@^&@%$!%^~~56728347264728347";
                const CStringRef cref(str);
                REQUIRE(cref.Data == str.data());
                REQUIRE(cref.Size == str.size());
            }
        }
        GIVEN("char16")
        {
            using CStringRef = CStringRef<char16>;
            using String = std::basic_string<char16>;

            {
                String str;
                const CStringRef cref(str);
                REQUIRE(cref.Data == str.data());
                REQUIRE(cref.Size == str.size());
            }
            {
                String str = u"";
                const CStringRef cref(str);
                REQUIRE(cref.Data == str.data());
                REQUIRE(cref.Size == str.size());
            }
            {
                String str = u"hello world";
                const CStringRef cref(str);
                REQUIRE(cref.Data == str.data());
                REQUIRE(cref.Size == str.size());
            }
            {
                String str = u"%@^&@%$!%^~~56728347264728347";
                const CStringRef cref(str);
                REQUIRE(cref.Data == str.data());
                REQUIRE(cref.Size == str.size());
            }
        }
        GIVEN("char32")
        {
            using CStringRef = CStringRef<char32>;
            using String = std::basic_string<char32>;

            {
                String str;
                const CStringRef cref(str);
                REQUIRE(cref.Data == str.data());
                REQUIRE(cref.Size == str.size());
            }
            {
                String str = U"";
                const CStringRef cref(str);
                REQUIRE(cref.Data == str.data());
                REQUIRE(cref.Size == str.size());
            }
            {
                String str = U"hello world";
                const CStringRef cref(str);
                REQUIRE(cref.Data == str.data());
                REQUIRE(cref.Size == str.size());
            }
            {
                String str = U"%@^&@%$!%^~~56728347264728347";
                const CStringRef cref(str);
                REQUIRE(cref.Data == str.data());
                REQUIRE(cref.Size == str.size());
            }
        }
    }

    TEMPLATE_TEST_CASE("[CStringRef] : Copy constructor should copy both of the members", "[CStringRef][Copy constructor]", CStringRefTestedTypes)
    {
        using CStringRef = CStringRef<TestType>;
        constexpr size_t dataSize = 100;
        TestType data[dataSize] = {};

        {
            const CStringRef base1(data, dataSize);
            const CStringRef cref1(base1);
            REQUIRE(cref1.Data == base1.Data);
            REQUIRE(cref1.Size == cref1.Size);
        }
        {
            const CStringRef base1(data + 10, dataSize - 10);
            const CStringRef cref1(base1);
            REQUIRE(cref1.Data == base1.Data);
            REQUIRE(cref1.Size == cref1.Size);
        }
        {
            const CStringRef base1(nullptr, 0);
            const CStringRef cref1(base1);
            REQUIRE(cref1.Data == base1.Data);
            REQUIRE(cref1.Size == cref1.Size);
        }
        {
            const CStringRef base1(data, 0);
            const CStringRef cref1(base1);
            REQUIRE(cref1.Data == base1.Data);
            REQUIRE(cref1.Size == cref1.Size);
        }
    }

    TEMPLATE_TEST_CASE("[CStringRef] : Move constructor should copy both of the members", "[CStringRef][Move constructor]", CStringRefTestedTypes)
    {
        using CStringRef = CStringRef<TestType>;
        constexpr size_t dataSize = 100;
        TestType data[dataSize] = {};

        {
            const CStringRef base1(data, dataSize);
            const CStringRef cref1(std::move(base1));
            REQUIRE(cref1.Data == base1.Data);
            REQUIRE(cref1.Size == cref1.Size);
        }
        {
            const CStringRef base1(data + 10, dataSize - 10);
            const CStringRef cref1(std::move(base1));
            REQUIRE(cref1.Data == base1.Data);
            REQUIRE(cref1.Size == cref1.Size);
        }
        {
            const CStringRef base1(nullptr, 0);
            const CStringRef cref1(std::move(base1));
            REQUIRE(cref1.Data == base1.Data);
            REQUIRE(cref1.Size == cref1.Size);
        }
        {
            const CStringRef base1(data, 0);
            const CStringRef cref1(std::move(base1));
            REQUIRE(cref1.Data == base1.Data);
            REQUIRE(cref1.Size == cref1.Size);
        }
    }

    TEMPLATE_TEST_CASE("[CStringRef] : Operator std::basic_string_view should return string_view of the referenced string", "[CStringRef][operator std::basic_string_view]", CStringRefTestedTypes)
    {
        using CStringRef = CStringRef<TestType>;
        using StringView = std::basic_string_view<TestType>;

        constexpr size_t dataSize = 100;
        TestType data[dataSize] = {};

        {
            const CStringRef cref(data, dataSize);
            const StringView view = cref;
            REQUIRE(view.size() == cref.Size);
            REQUIRE(view.data() == cref.Data);
        }

        {
            const CStringRef cref(data + 13, dataSize - 13);
            const StringView view = cref;
            REQUIRE(view.size() == cref.Size);
            REQUIRE(view.data() == cref.Data);
        }

        {
            const CStringRef cref(nullptr, 0);
            const StringView view = cref;
            REQUIRE(view.size() == cref.Size);
            REQUIRE(view.data() == cref.Data);
        }

        {
            const CStringRef cref(data, 0);
            const StringView view = cref;
            REQUIRE(view.size() == cref.Size);
            REQUIRE(view.data() == cref.Data);
        }
    }
}

namespace CIo::UniversalStringTesting
{
    template< typename ... T>
    struct TestingStruct;

    TEST_CASE("[UniversalString] : GetStringClassType should return at compile time the char type of the provided string class", "[UniversalString][GetStringClassType")
    {
        static_assert (std::is_same_v<typename GetStringClassType<std::string_view>::type,                                  char8>, "");
        static_assert (std::is_same_v<typename GetStringClassType<const std::string_view>::type,                            char8>, "");
        static_assert (std::is_same_v<typename GetStringClassType<const volatile std::string_view>::type,                   char8>, "");
        static_assert (std::is_same_v<typename GetStringClassType<const volatile std::string_view REF>::type,               char8>, "");

        static_assert (std::is_same_v<typename GetStringClassType<std::basic_string<char8> REF>::type,                      char8>, "");
        static_assert (std::is_same_v<typename GetStringClassType<const std::basic_string<char16> REF>::type,               char16>, "");
        static_assert (std::is_same_v<typename GetStringClassType<std::basic_string<char32>>::type,                         char32>, "");
        static_assert (std::is_same_v<typename GetStringClassType<const std::basic_string<charW>>::type,                    charW>, "");
        static_assert (std::is_same_v<typename GetStringClassType<std::basic_string<double>>::type,                         double>, "");

        static_assert (std::is_same_v<typename GetStringClassType<std::basic_string_view<char8>>::type,                     char8>, "");
        static_assert (std::is_same_v<typename GetStringClassType<const std::basic_string_view<char16>>::type,              char16>, "");
        static_assert (std::is_same_v<typename GetStringClassType<std::basic_string_view<char32>>::type,                    char32>, "");
        static_assert (std::is_same_v<typename GetStringClassType<const volatile std::basic_string_view<charW> REF>::type,  charW>, "");
        static_assert (std::is_same_v<typename GetStringClassType<const std::basic_string_view<int>>::type,                 int>, "");

        static_assert (std::is_same_v<typename GetStringClassType<CStringRef<char8>>::type,                                 char8>, "");
        static_assert (std::is_same_v<typename GetStringClassType<const volatile  CStringRef<char16>>::type,                char16>, "");
        static_assert (std::is_same_v<typename GetStringClassType<CStringRef<char32> REF>::type,                            char32>, "");
        static_assert (std::is_same_v<typename GetStringClassType<const CStringRef<charW>>::type,                           charW>, "");

        static_assert (std::is_same_v<typename GetStringClassType<volatile int>::type,                                      FalseType>, "");
        static_assert (std::is_same_v<typename GetStringClassType<double REF>::type,                                        FalseType>, "");
        static_assert (std::is_same_v<typename GetStringClassType<const float>::type,                                       FalseType>, "");
        static_assert (std::is_same_v<typename GetStringClassType<bool *>::type,                                            FalseType>, "");
        static_assert (std::is_same_v<typename GetStringClassType<std::tuple<double, double> REF>::type,                    FalseType>, "");
        static_assert (std::is_same_v<typename GetStringClassType<TestingStruct<int, double>>::type,                        FalseType>, "");
        static_assert (std::is_same_v<typename GetStringClassType<const std::integral_constant<int, 0>>::type,              FalseType>, "");
        static_assert (std::is_same_v<typename GetStringClassType<TestingStruct<int, std::string, float>>::type,            FalseType>, "");
        static_assert (std::is_same_v<typename GetStringClassType<std::array<char, 10>>::type,                              FalseType>, "");
    }

    TEST_CASE("[UniversalString] : IsStringClass should return at compile time if the specified type is string class (std::string, std::string_view, CStringRef)", "[UniversalString][IsStringClass]")
    {
        static_assert (IsStringClass<std::string>::value, "");
        static_assert (IsStringClass<std::wstring>::value, "");
        static_assert (IsStringClass<std::u16string>::value, "");

        static_assert (IsStringClass<std::string_view>::value, "");
        static_assert (IsStringClass<std::wstring_view>::value, "");
        static_assert (IsStringClass<std::u16string_view>::value, "");

        static_assert (IsStringClass<std::string_view>::value, "");
        static_assert (IsStringClass<const std::string_view>::value, "");
        static_assert (IsStringClass<const volatile std::string_view>::value, "");
        static_assert (IsStringClass<const volatile std::string_view REF>::value, "");

        static_assert (IsStringClass<std::basic_string<char8>>::value, "");
        static_assert (IsStringClass<std::basic_string<char16>>::value, "");
        static_assert (IsStringClass<std::basic_string<char32>>::value, "");
        static_assert (IsStringClass<std::basic_string<charW>>::value, "");

        static_assert (IsStringClass<std::basic_string_view<char8>>::value, "");
        static_assert (IsStringClass<std::basic_string_view<char16>>::value, "");
        static_assert (IsStringClass<std::basic_string_view<char32>>::value, "");
        static_assert (IsStringClass<std::basic_string_view<charW>>::value, "");

        static_assert (IsStringClass<CStringRef<char8>>::value, "");
        static_assert (IsStringClass<CStringRef<char16>>::value, "");
        static_assert (IsStringClass<CStringRef<char32>>::value, "");
        static_assert (IsStringClass<CStringRef<charW>>::value, "");

        static_assert (NOT IsStringClass<const int>::value, "");
        static_assert (NOT IsStringClass<const volatile double>::value, "");
        static_assert (NOT IsStringClass<float ** const>::value, "");
        static_assert (NOT IsStringClass<long REF>::value, "");
        static_assert (NOT IsStringClass<const std::tuple<double, double>>::value, "");
        static_assert (NOT IsStringClass<TestingStruct<int, double> REF>::value, "");
        static_assert (NOT IsStringClass<std::integral_constant<int, 0>>::value, "");
        static_assert (NOT IsStringClass<const TestingStruct<int, std::string, float> *>::value, "");
        static_assert (NOT IsStringClass<const volatile std::array<char, 10>>::value, "");
    }

    TEST_CASE("[UniversalString] : GetCStringType should return at compile time the char type of the provided c style string class", "[UniversalString][GetCStringType]")
    {
        static_assert (std::is_same_v<typename GetCStringType<std::decay_t<decltype ("Hello world")>>::type, char8>, "");
        static_assert (std::is_same_v<typename GetCStringType<std::decay_t<decltype (L"Hello world")>>::type, charW>, "");
        static_assert (std::is_same_v<typename GetCStringType<std::decay_t<decltype (u"Hello world")>>::type, char16>, "");
        static_assert (std::is_same_v<typename GetCStringType<std::decay_t<decltype (U"Hello world")>>::type, char32>, "");

        static_assert (std::is_same_v<typename GetCStringType<char8 *>::type, char8>, "");
        static_assert (std::is_same_v<typename GetCStringType<const char8 *>::type, char8>, "");
        static_assert (std::is_same_v<typename GetCStringType<const char8 * const>::type, char8>, "");
        static_assert (std::is_same_v<typename GetCStringType<char8>::type, char8>, "");

        static_assert (std::is_same_v<typename GetCStringType<bool *>::type, bool>, "");
        static_assert (std::is_same_v<typename GetCStringType<const double *>::type, double>, "");
        static_assert (std::is_same_v<typename GetCStringType<const std::string>::type, std::string>, "");
        static_assert (std::is_same_v<typename GetCStringType<volatile std::wstring *>::type, std::wstring>, "");
    }

    TEST_CASE("[UniversalString] : IsCString should return at compile time if the specified type is a c style string", "[UniversalString][IsCString]")
    {
        static_assert (IsCString<std::decay_t<decltype ("Hello world")>>::value, "");
        static_assert (IsCString<char8 *>::value, "");
        static_assert (IsCString<const char8 *>::value, "");
        static_assert (IsCString<const char8 * const>::value, "");
        static_assert (IsCString<charW *>::value, "");
        static_assert (IsCString<const charW *>::value, "");
        static_assert (IsCString<const charW * const>::value, "");

        static_assert (NOT IsCString<int>::value, "");
        static_assert (NOT IsCString<volatile float>::value, "");
        static_assert (NOT IsCString<const double>::value, "");
        static_assert (NOT IsCString<bool>::value, "");
        static_assert (NOT IsCString<std::string>::value, "");
        static_assert (NOT IsCString<std::wstring>::value, "");
        static_assert (NOT IsCString<TestingStruct<int, std::string, float>>::value, "");
    }

    TEST_CASE("[UniversalString] : IsStringLiteral should return at compile time if the specified type is string literal", "[UniversalString][IsStringLiteral]")
    {
        static_assert (IsStringLiteral<decltype ("afkaokg")>::value, "");
        static_assert (IsStringLiteral<decltype (L"afkaokg")>::value, "");
        static_assert (IsStringLiteral<decltype (u"afkaokg")>::value, "");
        static_assert (IsStringLiteral<decltype (U"afkaokg")>::value, "");

        static_assert (IsStringLiteral<decltype ("")>::value, "");
        static_assert (IsStringLiteral<decltype (L"")>::value, "");
        static_assert (IsStringLiteral<decltype (u"")>::value, "");
        static_assert (IsStringLiteral<decltype (U"")>::value, "");

        static_assert (IsStringLiteral<decltype ("!@#$%^&&&*")>::value, "");
        static_assert (IsStringLiteral<decltype (L"!@#$%^&&&*")>::value, "");
        static_assert (IsStringLiteral<decltype (u"!@#$%^&&&*")>::value, "");
        static_assert (IsStringLiteral<decltype (U"!@#$%^&&&*")>::value, "");

        static_assert (NOT IsStringLiteral<int>::value, "");
        static_assert (NOT IsStringLiteral<volatile float >::value, "");
        static_assert (NOT IsStringLiteral<const int *>::value, "");
        static_assert (NOT IsStringLiteral<char * const>::value, "");
        static_assert (NOT IsStringLiteral<const bool ** const>::value, "");
        static_assert (NOT IsStringLiteral<std::string>::value, "");
        static_assert (NOT IsStringLiteral<TestingStruct<int, std::string, float>>::value, "");
    }

    TEST_CASE("[UniversalString] : IsAnyString should return at compile time if the specified type is any of the string types (String literal, c string, std::string, std::string_view, CStringRef)", "[UniversalString][IsAnyString]")
    {
        static_assert (IsAnyString_v<char8 *>, "");
        static_assert (IsAnyString_v<const char8 *>, "");
        static_assert (IsAnyString_v<const char8 * const>, "");

        static_assert (IsAnyString_v<char16 *>, "");
        static_assert (IsAnyString_v<const char16 *>, "");
        static_assert (IsAnyString_v<const char16 * const>, "");

        static_assert (IsAnyString_v<std::basic_string<char8>>, "");
        static_assert (IsAnyString_v<std::basic_string_view<char8>>, "");

        static_assert (IsAnyString_v<std::basic_string<char16>>, "");
        static_assert (IsAnyString_v<std::basic_string_view<char16>>, "");

        static_assert (IsAnyString_v<decltype ("Hello")>, "");
        static_assert (IsAnyString_v<decltype (u"Hello")>, "");

        static_assert (IsAnyString_v<decltype (u"Hello")>, "");

        static_assert (NOT IsAnyString_v<int REF>, "");
        static_assert (NOT IsAnyString_v<const double volatile>, "");
        static_assert (NOT IsAnyString_v<TestingStruct<int> REF>, "");
        static_assert (NOT IsAnyString_v<const volatile TestingStruct<int, double>>, "");
        static_assert (NOT IsAnyString_v<std::integral_constant<int, 0>>, "");
        static_assert (NOT IsAnyString_v<const TestingStruct<int, std::string, float>>, "");
    }

    TEST_CASE("[UniversalString] : GetAnyStringType should return at compile time the char type of the specified string type (string type according to IsAnyString)", "[UniversalString][GetAnyStringType]")
    {
        static_assert (std::is_same_v<GetAnyStringType_t<char8 *>, char8>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<const char8 *>, char8>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<const char8 * const>, char8>, "");

        static_assert (std::is_same_v<GetAnyStringType_t<char16 *>, char16>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<const char16 *>, char16>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<const char16 * const>, char16>, "");

        static_assert (std::is_same_v<GetAnyStringType_t<std::basic_string<char8>>, char8>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<std::basic_string_view<char8>>, char8>, "");

        static_assert (std::is_same_v<GetAnyStringType_t<std::basic_string<char16>>, char16>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<std::basic_string_view<char16>>, char16>, "");

        static_assert (std::is_same_v<GetAnyStringType_t<decltype ("Hello")>, char8>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<decltype (u"Hello")>, char16>, "");

        static_assert (std::is_same_v<GetAnyStringType_t<decltype (u"Hello")>, char16>, "");

        static_assert (std::is_same_v<GetAnyStringType_t<int>, FalseType>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<double>, FalseType>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<TestingStruct<int>>, FalseType>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<TestingStruct<int, double>>, FalseType>, "");
        static_assert (std::is_same_v<GetAnyStringType_t<std::integral_constant<int, 0>>, FalseType>, "");
    }

}
#endif // UNIVERSALSTRINGTESTING_H
