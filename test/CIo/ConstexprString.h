#ifndef CONSTEXPRSTRINGTEST_H
#define CONSTEXPRSTRINGTEST_H

#include "Catch2/Catch.hpp"
#include "src/ConstexprString.h"

#define ConstexprStringTestedTypes char8, char16, char32, charW

namespace cio::ConstexprStringTesting
{
    template<typename T>
    static constexpr T NullTermination {'\0'};
}

namespace cio::ConstexprStringTesting
{
    TEMPLATE_TEST_CASE("[ConstexprString] : ConstexprString should be compilable (and constructible) with all sizes", "[ConstexprString][Compilation]", ConstexprStringTestedTypes)
    {
        [[maybe_unused]] const ConstexprString<TestType, 0> str1;
        [[maybe_unused]] const ConstexprString<TestType, 2> str2;
        [[maybe_unused]] const ConstexprString<TestType, 5> str3;
        [[maybe_unused]] const ConstexprString<TestType, 13> str4;
        [[maybe_unused]] const ConstexprString<TestType, 25> str5;
        [[maybe_unused]] const ConstexprString<TestType, 64> str6;
        [[maybe_unused]] const ConstexprString<TestType, 86> str7;
        [[maybe_unused]] const ConstexprString<TestType, 201> str8;
        [[maybe_unused]] const ConstexprString<TestType, 305> str9;
    }

    TEMPLATE_TEST_CASE("[ConstexprString] : Default constructor should construct an empty string of size 0", "[ConstexprString][Default constructor]", ConstexprStringTestedTypes)
    {
        WHEN("Constructing a string of normal size the result should be empty")
        {
            const ConstexprString<TestType, 5> str;
            REQUIRE(str.Size() == 0);
            REQUIRE(str[0] == NullTermination<TestType>);
        }

        WHEN("Constructing a string of large size the result should be empty")
        {
            const ConstexprString<TestType, 10> str;
            REQUIRE(str.Size() == 0);
            REQUIRE(str[0] == NullTermination<TestType>);
        }

        WHEN("Constructing a string of zero size the result should be empty")
        {
            const ConstexprString<TestType, 0> str;
            REQUIRE(str.Size() == 0);
            REQUIRE(str[0] == NullTermination<TestType>);
        }

    }

    TEMPLATE_TEST_CASE("[ConstexprString] : Copy constructor shoudl construct an exact copy of the string given", "[ConstexprString][Copy constructor]", ConstexprStringTestedTypes)
    {
        WHEN("Constructing from empty strng, the constructed string should also be empty")
        {
            const ConstexprString<TestType, 3> str;
            const ConstexprString<TestType, 3> constructed = str;

            REQUIRE(constructed.Size() == 0);
            REQUIRE(str == constructed);
        }
        WHEN("Constructing from nonempty string, the constructed string should be equal to the one constructed from")
        {
            constexpr TestType msg[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', NullTermination<TestType>};
            constexpr size_t msgLenght = 10;
            const ConstexprString<TestType, msgLenght> str = msg;
            const ConstexprString<TestType, msgLenght> constructed = str;

            REQUIRE(str == constructed);
        }
    }

    TEMPLATE_TEST_CASE("[ConstexprString] : Move constructor shoudl construct an exact copy of the string given", "[ConstexprString][Move constructor]", ConstexprStringTestedTypes)
    {
        WHEN("Constructing from empty strng, the constructed string should also be empty")
        {
            const ConstexprString<TestType, 3> str;
            const ConstexprString<TestType, 3> constructed = str;

            REQUIRE(constructed.Size() == 0);
            REQUIRE(str == constructed);
        }

        WHEN("Constructing from nonempty string, the constructed string should be equal to the one constructed from")
        {
            constexpr TestType msg[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', NullTermination<TestType>};
            constexpr size_t msgLenght = 10;
            const ConstexprString<TestType, msgLenght> str = msg;
            const ConstexprString<TestType, msgLenght> constructed = str;

            REQUIRE(str == constructed);
        }
    }

    TEMPLATE_TEST_CASE("[ConstexprString] : Constructor with const CharT * argument shoudl construct an exact copy of the string given", "[ConstexprString][Constructor]", ConstexprStringTestedTypes)
    {
        constexpr TestType msg[] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', NullTermination<TestType>};
        constexpr size_t msgLenght = 12;

        WHEN("Constructing from a string literal a the constexpr string should contain it")
        {
            const ConstexprString<TestType, msgLenght> str(msg);
            //CHECK(std::basic_string_view<TestType>(msg) == std::basic_string_view<TestType>());
            CHECK(str.Size() == msgLenght);
            CHECK(str[str.Size()] == NullTermination<TestType>);
        }

        WHEN("The constructed string is not large enough the constructed string shoudl be full but still null terminated")
        {
            const ConstexprString<TestType, msgLenght - 2> str(msg);
            REQUIRE(str.Size() == msgLenght - 2);
            REQUIRE(str[str.Size()] == NullTermination<TestType>);
        }

        WHEN("Constructing from a string literal a string of zero size the constexpr string should be empty")
        {
            const ConstexprString<TestType, 0> str(msg);
            REQUIRE(str.Size() == 0);
            REQUIRE(str[str.Size()] == NullTermination<TestType>);
        }
    }

    TEMPLATE_TEST_CASE("[ConstexprString] : Copy assigning shoudl result in exact copy of the string given", "[ConstexprString][Copy assignment]", ConstexprStringTestedTypes)
    {
        constexpr TestType msg[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', NullTermination<TestType>};
        constexpr size_t msgLenght = 10;

        WHEN("Assigning from empty strng, the assigned string should also be empty")
        {
            const ConstexprString<TestType, 3> str;
            ConstexprString<TestType, 3> constructed;

            constructed = str;

            REQUIRE(constructed.Size() == 0);
            REQUIRE(str == constructed);
        }

        WHEN("Assigning from nonempty string, the assigned string should be equal to the one constructed")
        {
            const ConstexprString<TestType, msgLenght> str = msg;
            ConstexprString<TestType, msgLenght> constructed;

            constructed = str;

            REQUIRE(str == constructed);
        }

        WHEN("Assigning to nonempty string, the contents should be overriden")
        {
            constexpr TestType msg2[] = {'x', 'y', 'z', NullTermination<TestType>};
            constexpr size_t msg2Lenght = 3;

            const ConstexprString<TestType, msgLenght> str = msg;
            ConstexprString<TestType, msgLenght> constructed = msg2;

            constructed = str;

            REQUIRE(str == constructed);
        }
    }

    TEMPLATE_TEST_CASE("[ConstexprString] : Move assigning shoudl result in exact copy of the string given", "[ConstexprString][Move assignment]", ConstexprStringTestedTypes)
    {
        constexpr TestType msg[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', NullTermination<TestType>};
        constexpr size_t msgLenght = 10;

        WHEN("Assigning from empty strng, the assigned string should also be empty")
        {
            const ConstexprString<TestType, 3> str;
            ConstexprString<TestType, 3> constructed;

            constructed = str;

            REQUIRE(constructed.Size() == 0);
            REQUIRE(str == constructed);
        }

        WHEN("Assigning from nonempty string, the assigned string should be equal to the one constructed")
        {
            const ConstexprString<TestType, msgLenght> str = msg;
            ConstexprString<TestType, msgLenght> constructed;

            constructed = str;

            REQUIRE(str == constructed);
        }

        WHEN("Assigning to nonempty string, the contents should be overriden")
        {
            constexpr TestType msg2[] = {'x', 'y', 'z', NullTermination<TestType>};
            constexpr size_t msg2Lenght = 3;

            const ConstexprString<TestType, msgLenght> str = msg;
            ConstexprString<TestType, msgLenght> constructed = msg2;

            constructed = str;

            REQUIRE(str == constructed);
        }
    }

    TEMPLATE_TEST_CASE("[ConstexprString] : Size should return the size of the string minus the null termination", "[ConstexprString][Size]", ConstexprStringTestedTypes)
    {
        constexpr TestType msg[] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', NullTermination<TestType>};;
        constexpr size_t msgLenght = 12;

        WHEN("A empty string is constructed its size should be 0")
        {
            const ConstexprString<TestType, msgLenght> str;
            REQUIRE(str.Size() == 0);
        }

        WHEN("A string is constructed from a string literal its size should be of the string literal")
        {
            const ConstexprString<TestType, msgLenght> str = msg;
            REQUIRE(str.Size() == msgLenght);
        }

        WHEN("A string is appended to its size should increase")
        {
            const ConstexprString<TestType, msgLenght * 2> base(msg);
            ConstexprString<TestType, msgLenght * 2> str(base);

            size_t sizeBefore = base.Size();

            str += base;

            REQUIRE(str.Size() > sizeBefore);
            REQUIRE(str.Size() == base.Size() * 2);
        }

        WHEN("A string is set to to smaller string its size should decrease")
        {
            const ConstexprString<TestType, msgLenght * 2> smallerStr(msg);
            ConstexprString<TestType, msgLenght * 2> largeStr(msg);
            largeStr += msg;

            size_t sizeBefore = largeStr.Size();

            largeStr = smallerStr;

            REQUIRE(largeStr.Size() < sizeBefore);
            REQUIRE(largeStr.Size() * 2 == sizeBefore);
        }
    }

    TEMPLATE_TEST_CASE("[ConstexprString] : Capacity should retun the template specified size", "[ConstexprString][Capacity]", ConstexprStringTestedTypes)
    {
        const ConstexprString<TestType, 0> str1;
        REQUIRE(str1.Capacity() == 0);
        const ConstexprString<TestType, 2> str2;
        REQUIRE(str2.Capacity() == 2);
        const ConstexprString<TestType, 5> str3;
        REQUIRE(str3.Capacity() == 5);
        const ConstexprString<TestType, 18> str4;
        REQUIRE(str4.Capacity() == 18);
        const ConstexprString<TestType, 25> str5;
        REQUIRE(str5.Capacity() == 25);
        const ConstexprString<TestType, 23> str6;
        REQUIRE(str6.Capacity() == 23);
        const ConstexprString<TestType, 75> str7;
        REQUIRE(str7.Capacity() == 75);
        const ConstexprString<TestType, 201> str8;
        REQUIRE(str8.Capacity() == 201);
        const ConstexprString<TestType, 305> str9;
        REQUIRE(str9.Capacity() == 305);
    }

    TEMPLATE_TEST_CASE("[ConstexprString] : operator [] should return the character on the specified index", "[ConstexprString][operator []]", ConstexprStringTestedTypes)
    {
        constexpr TestType msg[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', NullTermination<TestType>};
        constexpr size_t msgLenght = 10;

        WHEN("the operator is used to get a charcter it should be returned")
        {
            const ConstexprString<TestType, msgLenght> constStr = msg;

            REQUIRE(constStr[0] == 'a');
            REQUIRE(constStr[1] == 'b');
            REQUIRE(constStr[2] == 'c');
            REQUIRE(constStr[3] == 'd');
            REQUIRE(constStr[4] == 'e');
            REQUIRE(constStr[5] == 'f');
            REQUIRE(constStr[6] == 'g');
            REQUIRE(constStr[7] == 'h');
            REQUIRE(constStr[8] == 'i');
            REQUIRE(constStr[9] == 'j');
            //Last char (at constStr.Size()) should always be a null termination
            REQUIRE(constStr[constStr.Size()] == NullTermination<TestType>);

        }

        WHEN("the operator is used to set a charcter it should be set")
        {
            ConstexprString<TestType, msgLenght> str = msg;

            str[0] = 'x';
            REQUIRE(str[0] == 'x');
            str[6] = '7';
            REQUIRE(str[6] == '7');
        }
    }

    TEMPLATE_TEST_CASE("[ConstexprString] : operator += should append a string or charcter to the string", "[ConstexprString][operator +=]", ConstexprStringTestedTypes)
    {
        constexpr TestType msg[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', NullTermination<TestType>};
        constexpr size_t msgLenght = 10;

        WHEN("appending a single char it should be appended and the string should grow by one")
        {
            WHEN("appeding to an empty string the string should contain just the char added")
            {
                ConstexprString<TestType, 5> str;
                str += 'x';

                CHECK(str[0] == 'x');
                CHECK(str.Size() == 1);
                CHECK(str[str.Size()] == NullTermination<TestType>);
            }

            WHEN("appending to a nonempty string the string should contain all its previous data and the char added")
            {
                const size_t msgLenghtPlus = msgLenght + 1;
                ConstexprString<TestType, msgLenghtPlus> str = msg;
                str += '9';

                CHECK(str[0] == 'a');
                CHECK(str[3] == 'd');
                CHECK(str[msgLenght] == '9');
                CHECK(str.Size() == msgLenghtPlus);
                CHECK(str[str.Size()] == NullTermination<TestType>);
            }
        }

        WHEN("appending a string it should be appended and the string should grow by the size of the string")
        {
            constexpr TestType msg2[] = {'x', 'y', 'z', NullTermination<TestType>};
            constexpr size_t msg2Lenght = 3;

            WHEN("appeding to an empty string the string should contain just the string added")
            {
                ConstexprString<TestType, 5> str;
                str += msg2;

                CHECK(str[0] == 'x');
                CHECK(str[2] == 'z');
                CHECK(str.Size() == msg2Lenght);
                CHECK(str[str.Size()] == NullTermination<TestType>);
            }

            WHEN("appending to a nonempty string the string should contain all its previous data and the string added")
            {
                const size_t msgLenghtPlus = msgLenght + msg2Lenght;
                ConstexprString<TestType, msgLenghtPlus> str = msg;
                str += msg2;

                CHECK(str[0] == 'a');
                CHECK(str[3] == 'd');
                CHECK(str[msgLenght + 0] == 'x');
                CHECK(str[msgLenght + 1] == 'y');
                CHECK(str[msgLenght + 2] == 'z');
                CHECK(str.Size() == msgLenghtPlus);
                CHECK(str[str.Size()] == NullTermination<TestType>);
            }
        }
    }

    TEMPLATE_TEST_CASE("[ConstexprString] : operator == should return true when the strings are the same", "[ConstexprString][operator ==]", ConstexprStringTestedTypes)
    {
        constexpr TestType msg1[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', NullTermination<TestType>};
        constexpr size_t msg1Lenght = 10;

        WHEN("Comparing strings that are the same it should return true")
        {
            const ConstexprString<TestType, msg1Lenght> str1 = msg1;
            const ConstexprString<TestType, msg1Lenght> str2 = msg1;

            REQUIRE(str1 == str2);
        }

        WHEN("Comparing strings that are different it should return false")
        {
            SECTION("Differ with more than one charcter")
            {
                const ConstexprString<TestType, msg1Lenght> str1 = msg1;
                ConstexprString<TestType, msg1Lenght> str2 = msg1;

                str2[0] = 'x';

                REQUIRE((str1 == str2) == false);
            }

            SECTION("Differ with two or more characters")
            {
                const ConstexprString<TestType, msg1Lenght + 2> str1 = msg1;
                ConstexprString<TestType, msg1Lenght + 2> str2 = msg1;

                str2[0] = 'x';
                str2[1] = 'y';

                REQUIRE((str1 == str2) == false);
            }
        }

        WHEN("Comparing string to itself it should always return true")
        {
            const ConstexprString<TestType, msg1Lenght> str1;
            const ConstexprString<TestType, msg1Lenght> str2 = msg1;

            CHECK(str1 == str1);
            CHECK(str2 == str2);
        }

        WHEN("Comparing empty strings it should always return true")
        {
            const ConstexprString<TestType, 5> str1;
            const ConstexprString<TestType, 5> str2;

            REQUIRE(str1 == str2);
        }
    }

    TEMPLATE_TEST_CASE("[ConstexprString] : operator != should return true when the strings are not the same", "[ConstexprString][operator !=]", ConstexprStringTestedTypes)
    {
        constexpr TestType msg1[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', NullTermination<TestType>};
        constexpr size_t msg1Lenght = 10;

        WHEN("Comparing strings that are the same it should return false")
        {
            const ConstexprString<TestType, msg1Lenght> str1 = msg1;
            const ConstexprString<TestType, msg1Lenght> str2 = msg1;

            REQUIRE((str1 != str2) == false);
        }

        WHEN("Comparing strings that are different it should return true")
        {
            SECTION("Differ with more than one charcter")
            {
                const ConstexprString<TestType, msg1Lenght> str1 = msg1;
                ConstexprString<TestType, msg1Lenght> str2 = msg1;

                str2[0] = 'x';

                REQUIRE((str1 != str2) == true);
            }

            SECTION("Differ with two or more characters")
            {
                const ConstexprString<TestType, msg1Lenght + 2> str1 = msg1;
                ConstexprString<TestType, msg1Lenght + 2> str2 = msg1;

                str2[0] = 'x';
                str2[1] = 'y';

                REQUIRE((str1 != str2) == true);
            }
        }

        WHEN("Comparing string to itself it should always return false")
        {
            const ConstexprString<TestType, msg1Lenght> str1;
            const ConstexprString<TestType, msg1Lenght> str2 = msg1;

            REQUIRE((str1 != str1) == false);
            REQUIRE((str2 != str2) == false);
        }

        WHEN("Comparing empty strings it should always return false")
        {
            const ConstexprString<TestType, 5> str1;
            const ConstexprString<TestType, 5> str2;

            REQUIRE((str1 != str2) == false);
        }
    }

    TEMPLATE_TEST_CASE("[ConstexprString] : operator std::basic_string_view should return a string_view with the contents of this string", "[ConstexprString][operator std::basic_string_view]", ConstexprStringTestedTypes)
    {
        SECTION("With message \'abcdefghij\'")
        {
            constexpr TestType msg1[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', NullTermination<TestType>};
            constexpr size_t msg1Lenght = 10;

            const ConstexprString<TestType, msg1Lenght> str1 = msg1;
            const std::basic_string_view<TestType> expected1 = msg1;

            const std::basic_string_view<TestType> view1 = str1;

            REQUIRE(view1 == expected1);
        }

        SECTION("With message \'xyz\'")
        {

            constexpr TestType msg2[] = {'x', 'y', 'z', NullTermination<TestType>};
            constexpr size_t msg2Lenght = 3;

            const ConstexprString<TestType, msg2Lenght> str2 = msg2;
            const std::basic_string_view<TestType> expected2 = msg2;

            const std::basic_string_view<TestType> view2 = str2;

            REQUIRE(view2 == expected2);
        }
    }

    TEST_CASE("[CIo::PromoteStringCharsTo] : PromoteStringCharsTo should convert compile time string of smaller char lenght to string of bigger char lenght", "[ConstexprString][PromoteStringCharsTo]")
    {
        WHEN("Converting from char8 to char16 the string should be converted as expected")
        {
            WHEN("Converting normal string")
            {
                const ConstexprString<char8, 8> input = "Hello";
                const ConstexprString<char16, 8> expected = u"Hello";
                const ConstexprString<char16, 8> converted = PromoteStringCharsTo<char16>(input);
                REQUIRE(converted == expected);
            }
            WHEN("Converting max sized string")
            {
                const ConstexprString<char8, 5> input = "Hello";
                const ConstexprString<char16, 5> expected = u"Hello";
                const ConstexprString<char16, 5> converted = PromoteStringCharsTo<char16>(input);
                REQUIRE(converted == expected);
            }
            WHEN("Converting single char string")
            {
                const ConstexprString<char8, 5> input = "a";
                const ConstexprString<char16, 5> expected = u"a";
                const ConstexprString<char16, 5> converted = PromoteStringCharsTo<char16>(input);
                REQUIRE(converted == expected);
            }
            WHEN("Converting empty string")
            {
                const ConstexprString<char8, 5> input = "";
                const ConstexprString<char16, 5> expected = u"";
                const ConstexprString<char16, 5> converted = PromoteStringCharsTo<char16>(input);
                REQUIRE(converted == expected);
            }
        }
        WHEN("Converting from char16 to char32 the string should be converted as expected")
        {
            WHEN("Converting normal string")
            {
                const ConstexprString<char16, 8> input = u"Hello";
                const ConstexprString<char32, 8> expected = U"Hello";
                const ConstexprString<char32, 8> converted = PromoteStringCharsTo<char32>(input);
                REQUIRE(converted == expected);
            }
            WHEN("Converting max sized string")
            {
                const ConstexprString<char16, 5> input = u"Hello";
                const ConstexprString<char32, 5> expected = U"Hello";
                const ConstexprString<char32, 5> converted = PromoteStringCharsTo<char32>(input);
                REQUIRE(converted == expected);
            }
            WHEN("Converting single char string")
            {
                const ConstexprString<char16, 5> input = u"a";
                const ConstexprString<char32, 5> expected = U"a";
                const ConstexprString<char32, 5> converted = PromoteStringCharsTo<char32>(input);
                REQUIRE(converted == expected);
            }
            WHEN("Converting empty string")
            {
                const ConstexprString<char16, 5> input = u"";
                const ConstexprString<char32, 5> expected = U"";
                const ConstexprString<char32, 5> converted = PromoteStringCharsTo<char32>(input);
                REQUIRE(converted == expected);
            }
        }
        WHEN("Converting from char8 to char32 the string should be converted as expected")
        {
            WHEN("Converting normal string")
            {
                const ConstexprString<char8, 8> input = "Hello";
                const ConstexprString<char32, 8> expected = U"Hello";
                const ConstexprString<char32, 8> converted = PromoteStringCharsTo<char32>(input);
                REQUIRE(converted == expected);
            }
            WHEN("Converting max sized string")
            {
                const ConstexprString<char8, 5> input = "Hello";
                const ConstexprString<char32, 5> expected = U"Hello";
                const ConstexprString<char32, 5> converted = PromoteStringCharsTo<char32>(input);
                REQUIRE(converted == expected);
            }
            WHEN("Converting single char string")
            {
                const ConstexprString<char8, 5> input = "a";
                const ConstexprString<char32, 5> expected = U"a";
                const ConstexprString<char32, 5> converted = PromoteStringCharsTo<char32>(input);
                REQUIRE(converted == expected);
            }
            WHEN("Converting empty string")
            {
                const ConstexprString<char8, 5> input = "";
                const ConstexprString<char32, 5> expected = U"";
                const ConstexprString<char32, 5> converted = PromoteStringCharsTo<char32>(input);
                REQUIRE(converted == expected);
            }
        }
    }

}

#endif // CONSTEXPRSTRINGTEST_H
