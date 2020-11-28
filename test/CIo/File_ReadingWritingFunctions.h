#ifndef FILE_READINGWRITINGFUNCTIONS_H
#define FILE_READINGWRITINGFUNCTIONS_H

#include "File_TestingUtilities.h"

namespace cio::FileTesting
{

    TEMPLATE_TEST_CASE("[File] : Read should read data from the file", "[File][Read]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr size_t data0Size = 0;
        constexpr size_t data1Size = 10;
        constexpr size_t data2Size = 20;
        const std::array<int, 1> data0 = {0};
        const std::array<double, data1Size> data1 = {21.2, -0.32, 67.666663};
        const std::array<long, data2Size> data2 = {0, 426, 572734, 2211, -523, 1, 5, 9, -4211};

        std::array<int, 1> buffer0 = {0};
        std::array<double, data1Size> buffer1;
        std::array<long, data2Size> buffer2;

        SECTION("reading data from a validly opened file")
        {
            WHEN("reading data from a validly opened nonempty file they should match the ones written")
            {
                {
                    File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(writer.IsOpen() == true);
                    REQUIRE(writer.Write(data0.data(), data0Size) == true);
                    REQUIRE(writer.Write(data1.data(), data1Size) == true);
                    REQUIRE(writer.Write(data2.data(), data2Size) == true);
                }

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.Read(buffer0.data(), data0Size) == true);
                REQUIRE(reader.Read(buffer1.data(), data1Size) == true);
                REQUIRE(reader.Read(buffer2.data(), data2Size) == true);

                REQUIRE(buffer0 == data0);
                REQUIRE(buffer1 == data1);
                REQUIRE(buffer2 == data2);
            }

            WHEN("reading data from a validly opened empty file the reading should fail")
            {
                REQUIRE(File::CreateFile(cleaner.Filename()));
                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.Read(buffer0.data(), data0Size) == true); //Reading no data (size 0) should always succeeed
                REQUIRE(reader.Read(buffer1.data(), data1Size) == false);
                REQUIRE(reader.Read(buffer2.data(), data2Size) == false);
            }
        }

        WHEN("reading data from a invalidly opened file no data is read")
        {
            WHEN("reading data from a file opened for Write only no data is read")
            {
                File reader(cleaner.Filename(), COpenMode::Write);
                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.Read(buffer0.data(), data0Size) == true); //Reading no data (size 0) should always succeeed
                REQUIRE(reader.Read(buffer1.data(), data1Size) == false);
                REQUIRE(reader.Read(buffer2.data(), data2Size) == false);
            }

            WHEN("reading data from a file opened for Append only no data is read")
            {
                SECTION("Reading from a nonempty file")
                {
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.Write(data0.data(), data0Size) == true);
                    REQUIRE(creator.Write(data1.data(), data1Size) == true);
                    REQUIRE(creator.Write(data2.data(), data2Size) == true);
                }
                SECTION("Reading from an empty file")
                {
                    REQUIRE(File::CreateFile(cleaner.Filename()));
                }


                File reader(cleaner.Filename(), COpenMode::WriteAppend);
                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.Read(buffer0.data(), data0Size) == true); //Reading no data (size 0) should always succeeed
                REQUIRE(reader.Read(buffer1.data(), data1Size) == false);
                REQUIRE(reader.Read(buffer2.data(), data2Size) == false);
            }

        }

        WHEN("Is called on closed file should return false and not change any data")
        {
            File file;
            constexpr size_t dataSize = 10;
            {
                std::array<int, dataSize> data = {123, -123, 81};
                const std::array<int, dataSize> dataClone = data;

                REQUIRE(file.Read(data.data(), dataSize) == false);
                REQUIRE(data == dataClone);
            }
            {
                std::array<double, dataSize> data = {0.012, -3.8, 881874.2};
                const std::array<double, dataSize> dataClone = data;

                REQUIRE(file.Read(data.data(), dataSize) == false);
                REQUIRE(data == dataClone);
            }
            {
                std::array<char, dataSize> data = {'0', 'b', 'x'};
                const std::array<char, dataSize> dataClone = data;

                REQUIRE(file.Read(data.data(), dataSize) == false);
                REQUIRE(data == dataClone);
            }
        }
    }

    TEMPLATE_TEST_CASE("[File] : ReadAndCount should read data from the file and return the ammount of data read", "[File][ReadAndCount]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr size_t data0Size = 0;
        constexpr size_t data1Size = 18;
        constexpr size_t data2Size = 120;
        const std::array<double, 1> data0 = {0};
        const std::array<float, data1Size> data1 = {1.8884f, -0.46f, 88241.0f};
        const std::array<char, data2Size> data2 = {0, 25, -64, -51, 123, 1, 5, 9, -100};

        std::array<double, 1> buffer0 = {0};
        std::array<float, data1Size> buffer1;
        std::array<char, data2Size> buffer2;

        SECTION("reading data from a validly opened file")
        {
            WHEN("reading data from a validly opened nonempty file they should match the ones written")
            {
                {
                    File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(writer.IsOpen() == true);
                    REQUIRE(writer.Write(data0.data(), data0Size) == true);
                    REQUIRE(writer.Write(data1.data(), data1Size) == true);
                    REQUIRE(writer.Write(data2.data(), data2Size) == true);
                }

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.ReadAndCount(buffer0.data(), data0Size) == data0Size);
                REQUIRE(reader.ReadAndCount(buffer1.data(), data1Size) == data1Size);
                REQUIRE(reader.ReadAndCount(buffer2.data(), data2Size) == data2Size);

                REQUIRE(buffer0 == data0);
                REQUIRE(buffer1 == data1);
                REQUIRE(buffer2 == data2);
            }

            WHEN("reading data from a validly opened empty file the reading should fail")
            {
                REQUIRE(File::CreateFile(cleaner.Filename()));
                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.ReadAndCount(buffer0.data(), data0Size) == 0);
                REQUIRE(reader.ReadAndCount(buffer1.data(), data1Size) == 0);
                REQUIRE(reader.ReadAndCount(buffer2.data(), data2Size) == 0);
            }
        }

        WHEN("reading data from a invalidly opened file (open for write only) no data is read")
        {
            WHEN("reading data from a file opened for Write only no data is read")
            {
                File reader(cleaner.Filename(), COpenMode::Write);
                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.ReadAndCount(buffer0.data(), data0Size) == 0);
                REQUIRE(reader.ReadAndCount(buffer1.data(), data1Size) == 0);
                REQUIRE(reader.ReadAndCount(buffer2.data(), data2Size) == 0);
            }

            WHEN("reading data from a file opened for Append only no data is read")
            {
                SECTION("Reading from a nonempty file")
                {
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.Write(data0.data(), data0Size) == true);
                    REQUIRE(creator.Write(data1.data(), data1Size) == true);
                    REQUIRE(creator.Write(data2.data(), data2Size) == true);
                }
                SECTION("Reading from an empty file")
                {
                    REQUIRE(File::CreateFile(cleaner.Filename()));
                }


                File reader(cleaner.Filename(), COpenMode::WriteAppend);
                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.ReadAndCount(buffer0.data(), data0Size) == 0);
                REQUIRE(reader.ReadAndCount(buffer1.data(), data1Size) == 0);
                REQUIRE(reader.ReadAndCount(buffer2.data(), data2Size) == 0);
            }

        }

        WHEN("Is called on closed file should return 0 and not change any data")
        {
            File file;
            constexpr size_t dataSize = 10;
            {
                std::array<int, dataSize> data = {123, -123, 81};
                const std::array<int, dataSize> dataClone = data;

                REQUIRE(file.ReadAndCount(data.data(), dataSize) == 0);
                REQUIRE(data == dataClone);
            }
            {
                std::array<double, dataSize> data = {0.012, -3.8, 881874.2};
                const std::array<double, dataSize> dataClone = data;

                REQUIRE(file.ReadAndCount(data.data(), dataSize) == 0);
                REQUIRE(data == dataClone);
            }
            {
                std::array<char, dataSize> data = {'0', 'b', 'x'};
                const std::array<char, dataSize> dataClone = data;

                REQUIRE(file.ReadAndCount(data.data(), dataSize) == 0);
                REQUIRE(data == dataClone);
            }
        }
    }

    TEMPLATE_TEST_CASE("[File] : ReadObject should read the object data from the file", "[File][ReadObject]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        const struct Obj0 {}                                                      obj0;
        const struct Obj1 {float m1 = 123.0f;}                                    obj1;
        const struct Obj2 {long m1 = -12454; u8 m2 = 222;}                        obj2;
        const struct Obj3 {double m1 = 0.5; double m2 = 0.333333; short m3 = 10;} obj3;

        Obj0 read0;
        Obj1 read1;
        Obj2 read2;
        Obj3 read3;

        SECTION("reading data from a validly opened file")
        {
            WHEN("reading data from a validly opened nonempty file they should match the ones written")
            {
                {
                    File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(writer.IsOpen() == true);
                    REQUIRE(writer.WriteObject(obj0) == true);
                    REQUIRE(writer.WriteObject(obj1) == true);
                    REQUIRE(writer.WriteObject(obj2) == true);
                    REQUIRE(writer.WriteObject(obj3) == true);
                }

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.ReadObject(read0) == true);
                REQUIRE(reader.ReadObject(read1) == true);
                REQUIRE(reader.ReadObject(read2) == true);
                REQUIRE(reader.ReadObject(read3) == true);

                CHECK(read1.m1 == obj1.m1);

                CHECK(read2.m1 == obj2.m1);
                CHECK(read2.m2 == obj2.m2);

                CHECK(read3.m1 == obj3.m1);
                CHECK(read3.m2 == obj3.m2);
                CHECK(read3.m3 == obj3.m3);
            }

            WHEN("reading data from a validly opened empty file the reading should fail")
            {
                REQUIRE(File::CreateFile(cleaner.Filename()));
                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.ReadObject(read0) == false); //Even though Obj0 has no members, has size of > 0 therefore should fail
                REQUIRE(reader.ReadObject(read1) == false);
                REQUIRE(reader.ReadObject(read2) == false);
                REQUIRE(reader.ReadObject(read3) == false);
            }
        }
        WHEN("reading data from a invalidly opened file no data is read")
        {
            WHEN("reading data from a file opened for Write only no data is read")
            {
                File reader(cleaner.Filename(), COpenMode::Write);
                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.ReadObject(read0) == false); //Even though Obj0 has no members, has size of > 0 therefore should fail
                REQUIRE(reader.ReadObject(read1) == false);
                REQUIRE(reader.ReadObject(read2) == false);
                REQUIRE(reader.ReadObject(read3) == false);
            }

            WHEN("reading data from a file opened for Append only no data is read")
            {
                SECTION("Reading from a nonempty file")
                {
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.WriteObject(obj0) == true);
                    REQUIRE(creator.WriteObject(obj1) == true);
                    REQUIRE(creator.WriteObject(obj2) == true);
                    REQUIRE(creator.WriteObject(obj3) == true);
                }
                SECTION("Reading from an empty file")
                {
                    REQUIRE(File::CreateFile(cleaner.Filename()));
                }


                File reader(cleaner.Filename(), COpenMode::WriteAppend);
                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.ReadObject(read0) == false); //Even though Obj0 has no members, has size of > 0 therefore should fail
                REQUIRE(reader.ReadObject(read1) == false);
                REQUIRE(reader.ReadObject(read2) == false);
                REQUIRE(reader.ReadObject(read3) == false);
            }
        }

        WHEN("Is called on closed file should return false and not change any data")
        {
            File file;

            const Obj1 clone1;
            const Obj2 clone2;
            const Obj3 clone3;

            REQUIRE(file.ReadObject(read0) == false);
            REQUIRE(file.ReadObject(read1) == false);
            REQUIRE(file.ReadObject(read2) == false);
            REQUIRE(file.ReadObject(read3) == false);

            CHECK(read1.m1 == clone1.m1);

            CHECK(read2.m1 == clone2.m1);
            CHECK(read2.m2 == clone2.m2);

            CHECK(read3.m1 == clone3.m1);
            CHECK(read3.m2 == clone3.m2);
            CHECK(read3.m3 == clone3.m3);
        }
    }

    TEMPLATE_TEST_CASE("[File] : ReadString should read data from the file into the string argument", "[File][ReadString]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;

        constexpr size_t maxStrSize = 50;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr std::string_view data0 = "";
        constexpr std::string_view data1 = u8"abcdex1234";
        constexpr std::wstring_view data2 = L"abcdex1234";
        constexpr std::u16string_view data3 = u"abcdex1234";
        constexpr std::u32string_view data4 = U"abcdex1234";

        char8 buffer0[1]; //Array cannot be of size 0
        char8 buffer1[data1.size()];
        charW buffer2[data2.size()];
        char16 buffer3[data3.size()];
        char32 buffer4[data4.size()];

        const CStringRef<char8> ref0(buffer0, data0.size());
        const CStringRef<char8> ref1(buffer1, data1.size());
        const CStringRef<charW> ref2(buffer2, data2.size());
        const CStringRef<char16> ref3(buffer3, data3.size());
        const CStringRef<char32> ref4(buffer4, data4.size());

        SECTION("reading data from a validly opened file")
        {
            WHEN("reading data from a validly opened nonempty file they should match the ones written")
            {
                {
                    File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(writer.IsOpen() == true);
                    REQUIRE(writer.WriteString(data0) == true);
                    REQUIRE(writer.WriteString(data1) == true);
                    REQUIRE(writer.WriteString(data2) == true);
                    REQUIRE(writer.WriteString(data3) == true);
                    REQUIRE(writer.WriteString(data4) == true);
                }

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.ReadString(ref0) == true);
                REQUIRE(reader.ReadString(ref1) == true);
                REQUIRE(reader.ReadString(ref2) == true);
                REQUIRE(reader.ReadString(ref3) == true);
                REQUIRE(reader.ReadString(ref4) == true);

                REQUIRE(data0 == ref0);
                REQUIRE(data1 == ref1);
                REQUIRE(data2 == ref2);
                REQUIRE(data3 == ref3);
                REQUIRE(data4 == ref4);
            }

            WHEN("reading data from a validly opened empty file the reading should fail")
            {
                REQUIRE(File::CreateFile(cleaner.Filename()));
                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.ReadString(ref0) == true); //Reading no data (size 0) should always succeeed
                REQUIRE(reader.ReadString(ref1) == false);
                REQUIRE(reader.ReadString(ref2) == false);
                REQUIRE(reader.ReadString(ref3) == false);
                REQUIRE(reader.ReadString(ref4) == false);
            }
        }

        WHEN("reading data from a invalidly opened file no data is read")
        {
            WHEN("reading data from a file opened for Write only no data is read")
            {
                File reader(cleaner.Filename(), COpenMode::Write);
                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.ReadString(ref0) == true); //Reading no data (size 0) should always succeeed
                REQUIRE(reader.ReadString(ref1) == false);
                REQUIRE(reader.ReadString(ref2) == false);
                REQUIRE(reader.ReadString(ref3) == false);
                REQUIRE(reader.ReadString(ref4) == false);
            }

            WHEN("reading data from a file opened for Append only no data is read")
            {
                SECTION("Reading from a nonempty file")
                {
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.WriteString(data0) == true);
                    REQUIRE(creator.WriteString(data1) == true);
                    REQUIRE(creator.WriteString(data2) == true);
                    REQUIRE(creator.WriteString(data3) == true);
                    REQUIRE(creator.WriteString(data4) == true);
                }
                SECTION("Reading from an empty file")
                {
                    REQUIRE(File::CreateFile(cleaner.Filename()));
                }


                File reader(cleaner.Filename(), COpenMode::WriteAppend);
                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.ReadString(ref0) == true); //Reading no data (size 0) should always succeeed
                REQUIRE(reader.ReadString(ref1) == false);
                REQUIRE(reader.ReadString(ref2) == false);
                REQUIRE(reader.ReadString(ref3) == false);
                REQUIRE(reader.ReadString(ref4) == false);
            }
        }

        WHEN("Is called on closed file should return false and not change any data")
        {

            File file;
            constexpr size_t dataSize = 100;
            {
                std::array<char8, dataSize> data = {'x', 'y', 'z'};
                std::array<char8, dataSize> dataClone = {'x', 'y', 'z'};
                const CStringRef<char8> ref(data.data(), dataSize);
                REQUIRE(file.ReadString(ref) == false);
                REQUIRE(data == dataClone);
            }
            {
                std::array<charW, dataSize> data = {'x', 'y', 'z'};
                std::array<charW, dataSize> dataClone = {'x', 'y', 'z'};
                const CStringRef<charW> ref(data.data(), dataSize);
                REQUIRE(file.ReadString(ref) == false);
                REQUIRE(data == dataClone);
            }
            {
                std::array<char16, dataSize> data = {'x', 'y', 'z'};
                std::array<char16, dataSize> dataClone = {'x', 'y', 'z'};
                const CStringRef<char16> ref(data.data(), dataSize);
                REQUIRE(file.ReadString(ref) == false);
                REQUIRE(data == dataClone);
            }
            {
                std::array<char32, dataSize> data = {'x', 'y', 'z'};
                std::array<char32, dataSize> dataClone = {'x', 'y', 'z'};
                const CStringRef<char32> ref(data.data(), dataSize);
                REQUIRE(file.ReadString(ref) == false);
                REQUIRE(data == dataClone);
            }
        }
    }

    TEMPLATE_TEST_CASE("[File] : Write should put the appropriate data in the file", "[File][Write]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;

        constexpr size_t data0Size = 0;
        constexpr size_t data1Size = 41;
        constexpr size_t data2Size = 130;
        const std::array<int, 1> data0 = {0};
        const std::array<char32, data1Size> data1 = {21, 89, 6521};
        const std::array<bool, data2Size> data2 = {true, true, true, false, true, false, false, true};

        std::array<int, 1> buffer0 = {0};
        std::array<char32, data1Size> buffer1;
        std::array<bool, data2Size> buffer2;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("Writing to a valid file opened for Writing the data is written and can be read back")
        {
            WHEN("Writing to a nonempty file the data should be overwritten")
            {
                File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(creator.IsOpen() == true);
                //Prewritten data is in reverse order
                REQUIRE(creator.Write(data2.data(), data2Size) == true);
                REQUIRE(creator.Write(data1.data(), data1Size) == true);
                REQUIRE(creator.Write(data0.data(), data0Size) == true);
            }
            WHEN("Writing to a nonempty file the data should be written")
            {
                REQUIRE(File::CreateFile(cleaner.Filename()));
            }

            {
                File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);
                REQUIRE(writer.Write(data0.data(), data0Size) == true);
                REQUIRE(writer.Write(data1.data(), data1Size) == true);
                REQUIRE(writer.Write(data2.data(), data2Size) == true);
            }

            File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(reader.IsOpen() == true);

            REQUIRE(reader.Read(buffer0.data(), data0Size) == true);
            REQUIRE(reader.Read(buffer1.data(), data1Size) == true);
            REQUIRE(reader.Read(buffer2.data(), data2Size) == true);

            REQUIRE(buffer0 == data0);
            REQUIRE(buffer1 == data1);
            REQUIRE(buffer2 == data2);
        }

        WHEN("Writing to a valid file opened for Appending the data is always wriiten in the back and can be read back")
        {
            WHEN("Writing to a nonempty file the data should be appended in the end")
            {
                {
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.Write(data0.data(), data0Size) == true);
                    REQUIRE(creator.Write(data1.data(), data1Size) == true);
                    REQUIRE(creator.Write(data2.data(), data2Size) == true);
                }

                {
                    File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::Binary);
                    REQUIRE(writer.IsOpen() == true);
                    //Written data in reverse order
                    REQUIRE(writer.Write(data2.data(), data2Size) == true);
                    REQUIRE(writer.Write(data1.data(), data1Size) == true);
                    REQUIRE(writer.Write(data0.data(), data0Size) == true);
                }

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                //Reads the pre-written data (from creator)
                REQUIRE(reader.Read(buffer0.data(), data0Size) == true);
                REQUIRE(reader.Read(buffer1.data(), data1Size) == true);
                REQUIRE(reader.Read(buffer2.data(), data2Size) == true);

                //Reads the appended data (is in reverse order)
                REQUIRE(reader.Read(buffer2.data(), data2Size) == true);
                REQUIRE(reader.Read(buffer1.data(), data1Size) == true);
                REQUIRE(reader.Read(buffer0.data(), data0Size) == true);

                REQUIRE(buffer0 == data0);
                REQUIRE(buffer1 == data1);
                REQUIRE(buffer2 == data2);
            }
            WHEN("Writing to an empty file the data should be written")
            {
                REQUIRE(File::CreateFile(cleaner.Filename()));

                {
                    File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::Binary);
                    REQUIRE(writer.IsOpen() == true);
                    REQUIRE(writer.Write(data2.data(), data2Size) == true);
                    REQUIRE(writer.Write(data1.data(), data1Size) == true);
                    REQUIRE(writer.Write(data0.data(), data0Size) == true);
                }
                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                //Reads the appended data (is in reverse order)
                REQUIRE(reader.Read(buffer2.data(), data2Size) == true);
                REQUIRE(reader.Read(buffer1.data(), data1Size) == true);
                REQUIRE(reader.Read(buffer0.data(), data0Size) == true);

                REQUIRE(buffer0 == data0);
                REQUIRE(buffer1 == data1);
                REQUIRE(buffer2 == data2);
            }
        }

        WHEN("Writing to an invalid file (open for reading only) no data is written")
        {
            SECTION("Writing to a nonempty file")
            {
                File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(creator.IsOpen() == true);
                REQUIRE(creator.Write(data0.data(), data0Size) == true);
                REQUIRE(creator.Write(data1.data(), data1Size) == true);
                REQUIRE(creator.Write(data2.data(), data2Size) == true);
            }
            SECTION("Writing to an empty file")
            {
                REQUIRE(File::CreateFile(cleaner.Filename()));
            }

            File writer(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(writer.IsOpen() == true);

            REQUIRE(writer.Write(data0.data(), data0Size) == true);
            REQUIRE(writer.Write(data1.data(), data1Size) == false);
            REQUIRE(writer.Write(data2.data(), data2Size) == false);
        }

        WHEN("Is called on closed file should return false")
        {
            File file;

            REQUIRE(file.Write(data0.data(), data0Size) == false);
            REQUIRE(file.Write(data1.data(), data1Size) == false);
            REQUIRE(file.Write(data2.data(), data2Size) == false);
        }
    }

    TEMPLATE_TEST_CASE("[File] : WriteAndCount should put the appropriate data in the file and return the ammount of data written", "[File][WriteAndCount]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;

        constexpr size_t data0Size = 0;
        constexpr size_t data1Size = 3;
        constexpr size_t data2Size = 91;
        const std::array<double, 1> data0 = {0};
        const std::array<short, data1Size> data1 = {10, 89, 872,};
        const std::array<int, data2Size> data2 = {0, 89, -32021, 18999, -322};

        std::array<double, 1> buffer0 = {0};
        std::array<short, data1Size> buffer1;
        std::array<int, data2Size> buffer2;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("Writing to a valid file opened for Writing the data is written and can be read back")
        {
            WHEN("Writing to a nonempty file the data should be overwritten")
            {
                File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(creator.IsOpen() == true);
                //Prewritten data is in reverse order
                REQUIRE(creator.WriteAndCount(data2.data(), data2Size) == data2Size);
                REQUIRE(creator.WriteAndCount(data1.data(), data1Size) == data1Size);
                REQUIRE(creator.WriteAndCount(data0.data(), data0Size) == data0Size);
            }
            WHEN("Writing to a nonempty file the data should be written")
            {
                REQUIRE(File::CreateFile(cleaner.Filename()));
            }

            {
                File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);
                REQUIRE(writer.WriteAndCount(data0.data(), data0Size) == data0Size);
                REQUIRE(writer.WriteAndCount(data1.data(), data1Size) == data1Size);
                REQUIRE(writer.WriteAndCount(data2.data(), data2Size) == data2Size);
            }

            File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(reader.IsOpen() == true);

            REQUIRE(reader.Read(buffer0.data(), data0Size) == true);
            REQUIRE(reader.Read(buffer1.data(), data1Size) == true);
            REQUIRE(reader.Read(buffer2.data(), data2Size) == true);

            REQUIRE(buffer0 == data0);
            REQUIRE(buffer1 == data1);
            REQUIRE(buffer2 == data2);
        }

        WHEN("Writing to a valid file opened for Appending the data is always wriiten in the back and can be read back")
        {
            WHEN("Writing to a nonempty file the data should be appended in the end")
            {
                {
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.WriteAndCount(data0.data(), data0Size) == data0Size);
                    REQUIRE(creator.WriteAndCount(data1.data(), data1Size) == data1Size);
                    REQUIRE(creator.WriteAndCount(data2.data(), data2Size) == data2Size);
                }

                {
                    File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::Binary);
                    REQUIRE(writer.IsOpen() == true);
                    //Written data in reverse order
                    REQUIRE(writer.WriteAndCount(data2.data(), data2Size) == data2Size);
                    REQUIRE(writer.WriteAndCount(data1.data(), data1Size) == data1Size);
                    REQUIRE(writer.WriteAndCount(data0.data(), data0Size) == data0Size);
                }

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                //Reads the pre-written data (from creator)
                REQUIRE(reader.Read(buffer0.data(), data0Size) == true);
                REQUIRE(reader.Read(buffer1.data(), data1Size) == true);
                REQUIRE(reader.Read(buffer2.data(), data2Size) == true);

                //Reads the appended data (is in reverse order)
                REQUIRE(reader.Read(buffer2.data(), data2Size) == true);
                REQUIRE(reader.Read(buffer1.data(), data1Size) == true);
                REQUIRE(reader.Read(buffer0.data(), data0Size) == true);

                REQUIRE(buffer0 == data0);
                REQUIRE(buffer1 == data1);
                REQUIRE(buffer2 == data2);
            }
            WHEN("Writing to an empty file the data should be written")
            {
                REQUIRE(File::CreateFile(cleaner.Filename()));

                {
                    File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::Binary);
                    REQUIRE(writer.IsOpen() == true);
                    REQUIRE(writer.WriteAndCount(data2.data(), data2Size) == data2Size);
                    REQUIRE(writer.WriteAndCount(data1.data(), data1Size) == data1Size);
                    REQUIRE(writer.WriteAndCount(data0.data(), data0Size) == data0Size);
                }
                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                //Reads the appended data (is in reverse order)
                REQUIRE(reader.Read(buffer2.data(), data2Size) == true);
                REQUIRE(reader.Read(buffer1.data(), data1Size) == true);
                REQUIRE(reader.Read(buffer0.data(), data0Size) == true);

                REQUIRE(buffer0 == data0);
                REQUIRE(buffer1 == data1);
                REQUIRE(buffer2 == data2);
            }
        }

        WHEN("Writing to an invalid file (open for reading only) no data is written and function returns 0")
        {
            SECTION("Writing to a nonempty file")
            {
                File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(creator.IsOpen() == true);
                REQUIRE(creator.WriteAndCount(data0.data(), data0Size) == data0Size);
                REQUIRE(creator.WriteAndCount(data1.data(), data1Size) == data1Size);
                REQUIRE(creator.WriteAndCount(data2.data(), data2Size) == data2Size);
            }
            SECTION("Writing to an empty file")
            {
                REQUIRE(File::CreateFile(cleaner.Filename()));
            }

            File writer(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(writer.IsOpen() == true);

            REQUIRE(writer.WriteAndCount(data0.data(), data0Size) == 0);
            REQUIRE(writer.WriteAndCount(data1.data(), data1Size) == 0);
            REQUIRE(writer.WriteAndCount(data2.data(), data2Size) == 0);
        }

        WHEN("Is called on closed file should return 0")
        {
            File file;

            REQUIRE(file.WriteAndCount(data0.data(), data0Size) == 0);
            REQUIRE(file.WriteAndCount(data1.data(), data1Size) == 0);
            REQUIRE(file.WriteAndCount(data2.data(), data2Size) == 0);
        }
    }

    TEMPLATE_TEST_CASE("[File] : WriteObject should put the whole object in the file", "[File][WriteObject]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;

        const struct Obj0 {}                                                  obj0;
        const struct Obj1 {char8 m1 = 123;}                                   obj1;
        const struct Obj2 {int m1 = -12454; int m2 = 8899;}                   obj2;
        const struct Obj3 {double m1 = 0.5; double m2 = -0.003; int m3 = 10;} obj3;

        Obj0 read0;
        Obj1 read1;
        Obj2 read2;
        Obj3 read3;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("Writing to a valid file opened for Writing the data is written and can be read back")
        {
            WHEN("Writing to a nonempty file the data should be overwritten")
            {
                File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(creator.IsOpen() == true);
                //Prewritten data is in reverse order
                REQUIRE(creator.WriteObject(obj3) == true);
                REQUIRE(creator.WriteObject(obj2) == true);
                REQUIRE(creator.WriteObject(obj1) == true);
                REQUIRE(creator.WriteObject(obj0) == true);
            }
            WHEN("Writing to a nonempty file the data should be written")
            {
                REQUIRE(File::CreateFile(cleaner.Filename()));
            }

            {
                File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);
                REQUIRE(writer.WriteObject(obj0) == true);
                REQUIRE(writer.WriteObject(obj1) == true);
                REQUIRE(writer.WriteObject(obj2) == true);
                REQUIRE(writer.WriteObject(obj3) == true);
            }

            File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(reader.IsOpen() == true);

            REQUIRE(reader.ReadObject(read0) == true);
            REQUIRE(reader.ReadObject(read1) == true);
            REQUIRE(reader.ReadObject(read2) == true);
            REQUIRE(reader.ReadObject(read3) == true);

            CHECK(read1.m1 == obj1.m1);

            CHECK(read2.m1 == obj2.m1);
            CHECK(read2.m2 == obj2.m2);

            CHECK(read3.m1 == obj3.m1);
            CHECK(read3.m2 == obj3.m2);
            CHECK(read3.m3 == obj3.m3);
        }
        WHEN("Writing to a valid file opened for Appending the data is always wriiten in the back and can be read back")
        {
            WHEN("Writing to a nonempty file the data should be appended in the end")
            {
                {
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.WriteObject(obj0) == true);
                    REQUIRE(creator.WriteObject(obj1) == true);
                    REQUIRE(creator.WriteObject(obj2) == true);
                    REQUIRE(creator.WriteObject(obj3) == true);
                }

                {
                    File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::Binary);
                    REQUIRE(writer.IsOpen() == true);
                    //Written data in reverse order
                    REQUIRE(writer.WriteObject(obj3) == true);
                    REQUIRE(writer.WriteObject(obj2) == true);
                    REQUIRE(writer.WriteObject(obj1) == true);
                    REQUIRE(writer.WriteObject(obj0) == true);
                }

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                //Reads the pre-written data (from creator)
                REQUIRE(reader.ReadObject(read0) == true);
                REQUIRE(reader.ReadObject(read1) == true);
                REQUIRE(reader.ReadObject(read2) == true);
                REQUIRE(reader.ReadObject(read3) == true);

                //Reads the appended data (is in reverse order)
                REQUIRE(reader.ReadObject(read3) == true);
                REQUIRE(reader.ReadObject(read2) == true);
                REQUIRE(reader.ReadObject(read1) == true);
                REQUIRE(reader.ReadObject(read0) == true);

                CHECK(read1.m1 == obj1.m1);

                CHECK(read2.m1 == obj2.m1);
                CHECK(read2.m2 == obj2.m2);

                CHECK(read3.m1 == obj3.m1);
                CHECK(read3.m2 == obj3.m2);
                CHECK(read3.m3 == obj3.m3);
            }
            WHEN("Writing to an empty file the data should be written")
            {
                REQUIRE(File::CreateFile(cleaner.Filename()));

                {
                    File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::Binary);
                    REQUIRE(writer.IsOpen() == true);
                    REQUIRE(writer.WriteObject(obj3) == true);
                    REQUIRE(writer.WriteObject(obj2) == true);
                    REQUIRE(writer.WriteObject(obj1) == true);
                    REQUIRE(writer.WriteObject(obj0) == true);
                }
                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                //Reads the appended data (is in reverse order)
                REQUIRE(reader.ReadObject(read3) == true);
                REQUIRE(reader.ReadObject(read2) == true);
                REQUIRE(reader.ReadObject(read1) == true);
                REQUIRE(reader.ReadObject(read0) == true);

                CHECK(read1.m1 == obj1.m1);

                CHECK(read2.m1 == obj2.m1);
                CHECK(read2.m2 == obj2.m2);

                CHECK(read3.m1 == obj3.m1);
                CHECK(read3.m2 == obj3.m2);
                CHECK(read3.m3 == obj3.m3);
            }
        }
        WHEN("Writing to an invalid file (open for reading only) no data is written")
        {
            SECTION("Writing to a nonempty file")
            {
                File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(creator.IsOpen() == true);
                REQUIRE(creator.WriteObject(obj0) == true);
                REQUIRE(creator.WriteObject(obj1) == true);
                REQUIRE(creator.WriteObject(obj2) == true);
                REQUIRE(creator.WriteObject(obj3) == true);
            }
            SECTION("Writing to an empty file")
            {
                REQUIRE(File::CreateFile(cleaner.Filename()));
            }

            File writer(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(writer.IsOpen() == true);

            REQUIRE(writer.WriteObject(obj0) == false);
            REQUIRE(writer.WriteObject(obj1) == false);
            REQUIRE(writer.WriteObject(obj2) == false);
            REQUIRE(writer.WriteObject(obj3) == false);
        }

        WHEN("Is called on closed file should return false")
        {
            File file;

            REQUIRE(file.WriteObject(obj0) == false);
            REQUIRE(file.WriteObject(obj1) == false);
            REQUIRE(file.WriteObject(obj2) == false);
            REQUIRE(file.WriteObject(obj3) == false);
        }
    }

    TEMPLATE_TEST_CASE("[File] : WriteString should only accept string types", "[File][WriteString]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;

        constexpr size_t maxStrSize = 50;
        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        File file(cleaner.Filename(), COpenMode::ReadWrite);
        WHEN("Is given string literals of any char type should pass")
        {
            REQUIRE(file.WriteString("StrLiteral") == true);
            REQUIRE(file.WriteString(L"StrLiteral") == true);
            REQUIRE(file.WriteString(u"StrLiteral") == true);
            REQUIRE(file.WriteString(U"StrLiteral") == true);
        }
        WHEN("Is given C-strings of any char type should pass")
        {
            const char8 * str1 = "";
            const char8 * str2 = "123456abcd";
            const charW * str3 = L"123456abcd";
            const char16 * str4 = u"xyzw123";
            const char32 * str5 = U"xyzw123";

            REQUIRE(file.WriteString(str1) == true);
            REQUIRE(file.WriteString(str2) == true);
            REQUIRE(file.WriteString(str3) == true);
            REQUIRE(file.WriteString(str4) == true);
            REQUIRE(file.WriteString(str5) == true);
        }
        WHEN("Is given strings of any char type should pass")
        {
            const std::string str1 = "";
            const std::string str2 = "123456abcd";
            const std::wstring str3 = L"123456abcd";
            const std::u16string str4 = u"xyzw123";
            const std::u32string str5 = U"xyzw123";

            REQUIRE(file.WriteString(str1) == true);
            REQUIRE(file.WriteString(str2) == true);
            REQUIRE(file.WriteString(str3) == true);
            REQUIRE(file.WriteString(str4) == true);
            REQUIRE(file.WriteString(str5) == true);
        }
        WHEN("Is given string_views of any char type should pass")
        {
            const std::string_view str1 = "";
            const std::string_view str2 = u8"123456abcd";
            const std::wstring_view str3 = L"123456abcd";
            const std::u16string_view str4 = u"xyzw123";
            const std::u32string_view str5 = U"xyzw123";

            REQUIRE(file.WriteString(str1) == true);
            REQUIRE(file.WriteString(str2) == true);
            REQUIRE(file.WriteString(str3) == true);
            REQUIRE(file.WriteString(str4) == true);
            REQUIRE(file.WriteString(str5) == true);
        }

        WHEN("Is given CStringRefs of any char type should pass")
        {
            std::string data1 = "";
            std::string data2 = "123456abcd";
            std::wstring data3 = L"123456abcd";
            std::u16string data4 = u"xyzw123";
            std::u32string data5 = U"xyzw123";

            const CStringRef<char8> str1 = data1;
            const CStringRef<char8> str2 = data2;
            const CStringRef<charW> str3 = data3;
            const CStringRef<char16> str4 = data4;
            const CStringRef<char32> str5 = data5;

            REQUIRE(file.WriteString(str1) == true);
            REQUIRE(file.WriteString(str2) == true);
            REQUIRE(file.WriteString(str3) == true);
            REQUIRE(file.WriteString(str4) == true);
            REQUIRE(file.WriteString(str5) == true);
        }

        WHEN("Is given other types should not compile")
        {
            #if File_TEST_COMPILE_ERROR == true
            struct TestStruct0 {} test0;
            struct TestStruct1 {int a; int b;} test1;
            struct TestStruct2 {std::string a; float b;} test2;
            struct TestStruct3 {int a; int b; int c; int d; int e;} test3;

            REQUIRE(file.WriteString(static_cast<int>(-1415)) == false);
            REQUIRE(file.WriteString(static_cast<char>(23)) == false);
            REQUIRE(file.WriteString(static_cast<long>(-52511)) == false);
            REQUIRE(file.WriteString(static_cast<unsigned int>(3123)) == false);
            REQUIRE(file.WriteString(static_cast<double>(0.0)) == false);
            REQUIRE(file.WriteString(static_cast<float>(-123.3)) == false);

            REQUIRE(file.WriteString(test0) == false);
            REQUIRE(file.WriteString(test1) == false);
            REQUIRE(file.WriteString(test2) == false);
            REQUIRE(file.WriteString(test3) == false);
            #endif
        }
    }

    TEMPLATE_TEST_CASE("[File] : WriteString should put the contents of the provided string into the file", "[File][WriteString]", FileTestedTypes)
    {
        //Note: we dont have to check the conversions because we know they will compile from
        // previous test and the conversions themselfs are checked elsewhere
        // This is slightly dirty but much better than having to managed same function
        // (except the str1, str2.... definitions) for every type ie. 5x
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;

        constexpr size_t maxStrSize = 100;
        const std::string_view str1 = "";
        const std::string_view str2 = "123456abcd";
        const std::wstring_view str3 = L"123456abcd";
        const std::u16string_view str4 = u"xyzw123";
        const std::u32string_view str5 = U"xyzw123";

        char8 read1[maxStrSize];
        char8 read2[maxStrSize];
        charW read3[maxStrSize];
        char16 read4[maxStrSize];
        char32 read5[maxStrSize];

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("Writing to a valid file opened for Writing the data is written and can be read back")
        {
            WHEN("Writing to a nonempty file the data should be overwritten")
            {
                File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(creator.IsOpen() == true);
                //Prewritten data is in reverse order
                REQUIRE(creator.WriteString(str5) == true);
                REQUIRE(creator.WriteString(str4) == true);
                REQUIRE(creator.WriteString(str3) == true);
                REQUIRE(creator.WriteString(str2) == true);
                REQUIRE(creator.WriteString(str1) == true);
            }
            WHEN("Writing to a nonempty file the data should be written")
            {
                REQUIRE(File::CreateFile(cleaner.Filename()));
            }

            {
                File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);

                REQUIRE(writer.WriteString(str1) == true);
                REQUIRE(writer.WriteString(str2) == true);
                REQUIRE(writer.WriteString(str3) == true);
                REQUIRE(writer.WriteString(str4) == true);
                REQUIRE(writer.WriteString(str5) == true);
            }

            File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(reader.IsOpen() == true);

            REQUIRE(reader.ReadString(ToCStringRef(read1, str1.size())) == true);
            REQUIRE(reader.ReadString(ToCStringRef(read2, str2.size())) == true);
            REQUIRE(reader.ReadString(ToCStringRef(read3, str3.size())) == true);
            REQUIRE(reader.ReadString(ToCStringRef(read4, str4.size())) == true);
            REQUIRE(reader.ReadString(ToCStringRef(read5, str5.size())) == true);

            CHECK(str1 == std::string_view(read1, str1.size()));
            CHECK(str2 == std::string_view(read2, str2.size()));
            CHECK(str3 == std::wstring_view(read3, str3.size()));
            CHECK(str4 == std::u16string_view(read4, str4.size()));
            CHECK(str5 == std::u32string_view(read5, str5.size()));

        }
        WHEN("Writing to a valid file opened for Appending the data is always wriiten in the back and can be read back")
        {
            WHEN("Writing to a nonempty file the data should be appended in the end")
            {
                {
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.WriteString(str1) == true);
                    REQUIRE(creator.WriteString(str2) == true);
                    REQUIRE(creator.WriteString(str3) == true);
                    REQUIRE(creator.WriteString(str4) == true);
                    REQUIRE(creator.WriteString(str5) == true);
                }

                {
                    File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::Binary);
                    REQUIRE(writer.IsOpen() == true);
                    //Written data in reverse order
                    REQUIRE(writer.WriteString(str5) == true);
                    REQUIRE(writer.WriteString(str4) == true);
                    REQUIRE(writer.WriteString(str3) == true);
                    REQUIRE(writer.WriteString(str2) == true);
                    REQUIRE(writer.WriteString(str1) == true);
                }

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                //Reads the pre-written data (from creator)
                REQUIRE(reader.ReadString(ToCStringRef(read1, str1.size())) == true);
                REQUIRE(reader.ReadString(ToCStringRef(read2, str2.size())) == true);
                REQUIRE(reader.ReadString(ToCStringRef(read3, str3.size())) == true);
                REQUIRE(reader.ReadString(ToCStringRef(read4, str4.size())) == true);
                REQUIRE(reader.ReadString(ToCStringRef(read5, str5.size())) == true);

                //Reads the appended data (is in reverse order)
                REQUIRE(reader.ReadString(ToCStringRef(read5, str5.size())) == true);
                REQUIRE(reader.ReadString(ToCStringRef(read4, str4.size())) == true);
                REQUIRE(reader.ReadString(ToCStringRef(read3, str3.size())) == true);
                REQUIRE(reader.ReadString(ToCStringRef(read2, str2.size())) == true);
                REQUIRE(reader.ReadString(ToCStringRef(read1, str1.size())) == true);

                //CMP
                CHECK(str1 == std::string_view(read1, str1.size()));
                CHECK(str2 == std::string_view(read2, str2.size()));
                CHECK(str3 == std::wstring_view(read3, str3.size()));
                CHECK(str4 == std::u16string_view(read4, str4.size()));
                CHECK(str5 == std::u32string_view(read5, str5.size()));
            }
            WHEN("Writing to an empty file the data should be written")
            {
                REQUIRE(File::CreateFile(cleaner.Filename()));

                {
                    File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::Binary);
                    REQUIRE(writer.IsOpen() == true);
                    REQUIRE(writer.WriteString(str5) == true);
                    REQUIRE(writer.WriteString(str4) == true);
                    REQUIRE(writer.WriteString(str3) == true);
                    REQUIRE(writer.WriteString(str2) == true);
                    REQUIRE(writer.WriteString(str1) == true);
                }
                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                //Reads the appended data (is in reverse order)
                REQUIRE(reader.ReadString(ToCStringRef(read5, str5.size())) == true);
                REQUIRE(reader.ReadString(ToCStringRef(read4, str4.size())) == true);
                REQUIRE(reader.ReadString(ToCStringRef(read3, str3.size())) == true);
                REQUIRE(reader.ReadString(ToCStringRef(read2, str2.size())) == true);
                REQUIRE(reader.ReadString(ToCStringRef(read1, str1.size())) == true);

                //CMP
                CHECK(str1 == std::string_view(read1, str1.size()));
                CHECK(str2 == std::string_view(read2, str2.size()));
                CHECK(str3 == std::wstring_view(read3, str3.size()));
                CHECK(str4 == std::u16string_view(read4, str4.size()));
                CHECK(str5 == std::u32string_view(read5, str5.size()));
            }
        }
        WHEN("Writing to an invalid file (open for reading only) no data is written")
        {
            SECTION("Writing to a nonempty file")
            {
                File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(creator.IsOpen() == true);
                REQUIRE(creator.WriteString(str1) == true);
                REQUIRE(creator.WriteString(str2) == true);
                REQUIRE(creator.WriteString(str3) == true);
                REQUIRE(creator.WriteString(str4) == true);
                REQUIRE(creator.WriteString(str5) == true);
            }
            SECTION("Writing to an empty file")
            {
                REQUIRE(File::CreateFile(cleaner.Filename()));
            }

            File writer(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(writer.IsOpen() == true);

            REQUIRE(writer.WriteString(str1) == true); //Writing no data (size 0) should always succeeed
            REQUIRE(writer.WriteString(str2) == false);
            REQUIRE(writer.WriteString(str3) == false);
            REQUIRE(writer.WriteString(str4) == false);
            REQUIRE(writer.WriteString(str5) == false);
        }


        WHEN("Is called on closed file should return false")
        {
            File file;

            REQUIRE(file.WriteString(str1) == false);
            REQUIRE(file.WriteString(str2) == false);
            REQUIRE(file.WriteString(str3) == false);
            REQUIRE(file.WriteString(str4) == false);
            REQUIRE(file.WriteString(str5) == false);
        }
    }

    TEMPLATE_TEST_CASE("[File] : WasEndOfFileRieched should return true if the end of file was reached", "[File][WasEndOfFileRieched]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr size_t dataSize = 3;
        const int data[dataSize] = {0, 132029, -346};
        int buffer[dataSize];

        WHEN("reading WasEndOfFileRieched returns true after reading failed")
        {
            WHEN("Reading an empty file, it should return true after the first read")
            {
                {
                    const File creator(cleaner.Filename(), OpenModeFlag::Write);
                    REQUIRE(creator.IsOpen() == true);
                }

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                //After construction the end of file should never be set
                REQUIRE(reader.WasEndOfFileRieched() == false);

                //After the first reading that should fail it should be set
                REQUIRE(reader.Read(buffer, dataSize) == false);
                REQUIRE(reader.WasEndOfFileRieched() == true);

                //Any subsequent read should be the same
                REQUIRE(reader.Read(buffer, dataSize) == false);
                REQUIRE(reader.WasEndOfFileRieched() == true);
            }

            WHEN("Reading a nonempty file, it should return true after all of the data was read")
            {

                {
                    File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(writer.IsOpen() == true);
                    REQUIRE(writer.Write(data, dataSize) == true);
                }

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                //After construction the end of file should never be set
                REQUIRE(reader.WasEndOfFileRieched() == false);

                //After a reading that succeeds it should be not be set
                REQUIRE(reader.Read(buffer, dataSize) == true);
                REQUIRE(reader.WasEndOfFileRieched() == false);

                //After a reading that fails it should be set
                REQUIRE(reader.Read(buffer, dataSize) == false);
                REQUIRE(reader.WasEndOfFileRieched() == true);
            }
        }

        WHEN("writing WasEndOfFileRieched should never return true")
        {
            WHEN("Writing to an empty file, it should always return false")
            {
                File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);

                //After construction the end of file should never be set
                REQUIRE(writer.WasEndOfFileRieched() == false);

                //After first write the end of file should not be present
                REQUIRE(writer.Write(data, dataSize) == true);
                REQUIRE(writer.WasEndOfFileRieched() == false);

                //Any subsequent write should be the same
                REQUIRE(writer.Write(data, dataSize) == true);
                REQUIRE(writer.WasEndOfFileRieched() == false);
            }

            WHEN("Writing to a nonempty file, it should always return false")
            {
                {
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.Write(data, dataSize) == true);
                }

                File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);

                //After construction the end of file should never be set
                REQUIRE(writer.WasEndOfFileRieched() == false);

                //After first write the end of file should not be present
                REQUIRE(writer.Write(data, dataSize) == true);
                REQUIRE(writer.WasEndOfFileRieched() == false);

                //Any subsequent write should be the same
                REQUIRE(writer.Write(data, dataSize) == true);
                REQUIRE(writer.WasEndOfFileRieched() == false);
            }
        }

        WHEN("appending WasEndOfFileRieched should never return true")
        {
            WHEN("Appending to an empty file, it should always return false")
            {
                File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);

                //After construction the end of file should never be set
                REQUIRE(writer.WasEndOfFileRieched() == false);

                //After first write the end of file should not be present
                REQUIRE(writer.Write(data, dataSize) == true);
                REQUIRE(writer.WasEndOfFileRieched() == false);

                //Any subsequent write should be the same
                REQUIRE(writer.Write(data, dataSize) == true);
                REQUIRE(writer.WasEndOfFileRieched() == false);
            }

            WHEN("Appending to a nonempty file, it should always return false")
            {
                {
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.Write(data, dataSize) == true);
                }

                File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);

                //After construction the end of file should never be set
                REQUIRE(writer.WasEndOfFileRieched() == false);

                //After first write the end of file should not be present
                REQUIRE(writer.Write(data, dataSize) == true);
                REQUIRE(writer.WasEndOfFileRieched() == false);

                //Any subsequent write should be the same
                REQUIRE(writer.Write(data, dataSize) == true);
                REQUIRE(writer.WasEndOfFileRieched() == false);
            }
        }

        WHEN("Is called on closed file should return false")
        {
            File file;
            REQUIRE(file.WasEndOfFileRieched() == false);
        }
    }


}

#endif // FILE_READINGWRITINGFUNCTIONS_H
