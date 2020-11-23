#ifndef UNSAFEFILE_POSITIONFUNCTIONS_H
#define UNSAFEFILE_POSITIONFUNCTIONS_H

#include "UnsafeFile_TestingUtilities.h"

namespace CIo::UnsafeFileTesting
{

    TEMPLATE_TEST_CASE("[UnsafeFile] : GetPosInFile should return the current position in file", "[UnsafeFile][GetPosInFile]", UnsafeFileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;
        using Position   = typename UnsafeFile::Position;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr size_t dataSize = 100;
        const std::array<int, dataSize> data = {0, 132029, -346};
        std::array<int, dataSize> buffer;
        WHEN("reading GetPosInFile should return the current position in file")
        {
            WHEN("Reading an empty file it should always return 0")
            {
                REQUIRE(UnsafeFile::CreateFile(cleaner.Filename()) == true);

                UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                //After construction the position should be 0
                REQUIRE(reader.GetPosInFile() == 0);

                //After a failed reading the postion should not increase
                REQUIRE(reader.ReadObject(buffer[0]) == false);
                REQUIRE(reader.GetPosInFile() == 0);
            }

            WHEN("Reading a nonempty file, it should return the ammount of data read")
            {

                {
                    UnsafeFile writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(writer.IsOpen() == true);
                    REQUIRE(writer.Write(data.data(), dataSize) == true);
                }

                UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.IsOpen() == true);

                //After construction the position should be 0
                REQUIRE(reader.GetPosInFile() == 0);

                //After the first writing the position should increase
                REQUIRE(reader.ReadObject(buffer[0]) == true);
                REQUIRE(reader.GetPosInFile() > 0);

                const Position afterFirstRead = reader.GetPosInFile();

                //After the second writing the position should increase
                REQUIRE(reader.ReadObject(buffer[1]) == true);
                REQUIRE(reader.GetPosInFile() > afterFirstRead);

                const Position afterSecondRead = reader.GetPosInFile();

                //The distance from current to afterFirstRead should be the same as from 0 to afterFirstRead
                REQUIRE(afterSecondRead - afterFirstRead == afterFirstRead - 0);
            }
        }

        WHEN("Writing it should return the ammount of data written")
        {
            WHEN("Writing to an empty file, it should return the ammount of data written")
            {
                UnsafeFile writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);

                //After construction the position should be 0
                REQUIRE(writer.GetPosInFile() == 0);

                //After the first writing the position should increase
                REQUIRE(writer.WriteObject(buffer[0]) == true);
                REQUIRE(writer.GetPosInFile() > 0);

                const Position afterFirstRead = writer.GetPosInFile();

                //After the second writing the position should increase
                REQUIRE(writer.WriteObject(buffer[1]) == true);
                REQUIRE(writer.GetPosInFile() > afterFirstRead);

                const Position afterSecondRead = writer.GetPosInFile();

                //The distance from current to afterFirstRead should be the same as from 0 to afterFirstRead
                REQUIRE(afterSecondRead - afterFirstRead == afterFirstRead - 0);
            }

            WHEN("Writing to a nonempty file, it should return the ammount of data written")
            {
                {
                    UnsafeFile creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.Write(data.data(), dataSize) == true);
                }

                UnsafeFile writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);

                //After construction the position should be 0
                REQUIRE(writer.GetPosInFile() == 0);

                //After the first reading the position should increase
                REQUIRE(writer.WriteObject(buffer[0]) == true);
                REQUIRE(writer.GetPosInFile() > 0);

                const Position afterFirstRead = writer.GetPosInFile();

                //After the second reading the position should increase
                REQUIRE(writer.WriteObject(buffer[1]) == true);
                REQUIRE(writer.GetPosInFile() > afterFirstRead);

                const Position afterSecondRead = writer.GetPosInFile();

                //The distance from current to afterFirstRead should be the same as from 0 to afterFirstRead
                REQUIRE(afterSecondRead - afterFirstRead == afterFirstRead - 0);
            }
        }

        WHEN("Appending it should return the ammount of data written")
        {
            WHEN("Writing to an empty file, it should return the ammount of data written")
            {
                UnsafeFile writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);

                //After construction the position should be 0
                REQUIRE(writer.GetPosInFile() == 0);

                //After the first writing the position should increase
                REQUIRE(writer.WriteObject(buffer[0]) == true);
                REQUIRE(writer.GetPosInFile() > 0);

                const Position afterFirstRead = writer.GetPosInFile();

                //After the second writing the position should increase
                REQUIRE(writer.WriteObject(buffer[1]) == true);
                REQUIRE(writer.GetPosInFile() > afterFirstRead);

                const Position afterSecondRead = writer.GetPosInFile();

                //The distance from current to afterFirstRead should be the same as from 0 to afterFirstRead
                REQUIRE(afterSecondRead - afterFirstRead == afterFirstRead - 0);
            }

            WHEN("Writing to a nonempty file, it should return the ammount of data written")
            {
                {
                    UnsafeFile creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.Write(data.data(), dataSize) == true);
                }

                UnsafeFile writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::Binary);

                REQUIRE(writer.IsOpen() == true);

                //After construction the position should be 0
                REQUIRE(writer.GetPosInFile() == 0);

                //After the first writing the position should increase
                REQUIRE(writer.WriteObject(buffer[0]) == true);
                REQUIRE(writer.GetPosInFile() > 0);

                const Position afterFirstRead = writer.GetPosInFile();

                //After the second writing the position should increase
                REQUIRE(writer.WriteObject(buffer[1]) == true);
                REQUIRE(writer.GetPosInFile() > afterFirstRead);

                const Position afterSecondRead = writer.GetPosInFile();

                //After the second writing the position should increase
                REQUIRE(writer.WriteObject(buffer[2]) == true);
                REQUIRE(writer.GetPosInFile() > afterSecondRead);

                const Position afterThirdRead = writer.GetPosInFile();

                REQUIRE(afterThirdRead - afterSecondRead == afterSecondRead - afterFirstRead);
            }
        }

    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : MoveToBeggining should set the possition in the file to the beggining", "[UnsafeFile][MoveToBeggining]", UnsafeFileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr size_t dataSize = 100;
        const std::array<int, dataSize> data = {0, 132029, -346};
        const std::array<int, dataSize> data2 = {10, -1141, 198};
        std::array<int, dataSize> buffer;

        WHEN("MoveToBeggining is used it should set the possition in the file to the beggining")
        {
            GIVEN("Empty file")
            {
                UnsafeFile::CreateFile(cleaner.Filename());
                UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                reader.MoveToBeggining();
                REQUIRE(reader.GetPosInFile() == 0);
                reader.MoveToBeggining();
                REQUIRE(reader.GetPosInFile() == 0);
            }

            GIVEN("Nonempty file")
            {
                {
                    UnsafeFile creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.Write(data.data(), dataSize) == true);
                }

                UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                reader.MoveToBeggining();
                REQUIRE(reader.GetPosInFile() == 0);

                REQUIRE(reader.Read(buffer.data(), dataSize) == true);
                REQUIRE(buffer == data);

                reader.MoveToBeggining();
                REQUIRE(reader.GetPosInFile() == 0);
            }
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : MoveToEnd should set the possition in the file to the end", "[UnsafeFile][MoveToEnd]", UnsafeFileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;
        using Position   = typename UnsafeFile::Position;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr size_t dataSize = 100;
        const std::array<int, dataSize> data = {85752, -2417, -2};
        const std::array<int, dataSize> data2 = {321, 0, 0};
        std::array<int, dataSize> buffer;

        WHEN("MoveToEnd is used it should set the possition in the file to the end")
        {
            GIVEN("Empty file")
            {
                UnsafeFile writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Read, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);

                writer.MoveToEnd();
                REQUIRE(writer.GetPosInFile() == 0);

                REQUIRE(writer.Write(data2.data(), dataSize) == true);
                const Position pos = writer.GetPosInFile();

                writer.MoveToEnd();
                REQUIRE(writer.GetPosInFile() == pos);

                REQUIRE(writer.Read(buffer.data(), dataSize) == false);
            }

            GIVEN("Nonempty file")
            {
                {
                    UnsafeFile creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.Write(data.data(), dataSize) == true);
                }

                UnsafeFile writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);

                writer.MoveToEnd();
                REQUIRE(writer.GetPosInFile() != 0);

                REQUIRE(writer.Write(data2.data(), dataSize) == true);
                const Position pos = writer.GetPosInFile();

                writer.MoveToBeggining();
                writer.MoveToEnd();

                REQUIRE(writer.GetPosInFile() == pos);
                REQUIRE(writer.Read(buffer.data(), dataSize) == false);
            }

        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : MoveBy should offset the current possition by the ammount specified", "[UnsafeFile][MoveBy]", UnsafeFileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr size_t dataSize = 100;
        const std::array<int, dataSize> data = {-231, 54345, 21};

        WHEN("MoveBy is used it should offset the current possition by the ammount specified and fail when the resulting position would be negative")
        {
            GIVEN("Empty file")
            {
                UnsafeFile::CreateFile(cleaner.Filename());
                UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                REQUIRE(reader.MoveBy(0) == true);
                REQUIRE(reader.GetPosInFile() == 0);
                REQUIRE(reader.MoveBy(2) == true);
                REQUIRE(reader.GetPosInFile() == 2);
                REQUIRE(reader.MoveBy(9) == true);
                REQUIRE(reader.GetPosInFile() == 11);
                REQUIRE(reader.MoveBy(0) == true);
                REQUIRE(reader.GetPosInFile() == 11);
                REQUIRE(reader.MoveBy(-21) == false);
                REQUIRE(reader.GetPosInFile() == 11);
                REQUIRE(reader.MoveBy(-reader.GetPosInFile()) == true);
                REQUIRE(reader.GetPosInFile() == 0);
            }

            GIVEN("Nonempty file")
            {
                {
                    UnsafeFile creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.Write(data.data(), dataSize) == true);
                }

                UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                REQUIRE(reader.MoveBy(64) == true);
                REQUIRE(reader.GetPosInFile() == 64);
                REQUIRE(reader.MoveBy(2) == true);
                REQUIRE(reader.GetPosInFile() == 66);
                REQUIRE(reader.MoveBy(-9) == true);
                REQUIRE(reader.GetPosInFile() == 57);
                REQUIRE(reader.MoveBy(-reader.GetPosInFile()) == true);
                REQUIRE(reader.GetPosInFile() == 0);
                REQUIRE(reader.MoveBy(-21) == false);
                REQUIRE(reader.GetPosInFile() == 0);
            }
        }


    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : SetPosInFile should set the possition in the file according to the given arguments", "[UnsafeFile][SetPosInFile]", UnsafeFileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr size_t dataSize = 100;
        const std::array<int, dataSize> data = {0, 132029, -346};

        WHEN("SetPosInFile is used it should set the position to according to the given arguments and fail if the resulting position would be negative")
        {
            GIVEN("Empty file")
            {
                UnsafeFile::CreateFile(cleaner.Filename());
                UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                SECTION("Setting from Beggining")
                {
                    REQUIRE(reader.SetPosInFile(0, UnsafeFile::OriginPosition::Beggining) == true);
                    REQUIRE(reader.GetPosInFile() == 0);
                    REQUIRE(reader.SetPosInFile(113, UnsafeFile::OriginPosition::Beggining) == true);
                    REQUIRE(reader.GetPosInFile() == 113);
                    REQUIRE(reader.SetPosInFile(-1, UnsafeFile::OriginPosition::Beggining) == false);
                    REQUIRE(reader.GetPosInFile() == 113);
                    REQUIRE(reader.SetPosInFile(-113, UnsafeFile::OriginPosition::Beggining) == false);
                    REQUIRE(reader.GetPosInFile() == 113);
                }
                SECTION("Setting from CurrentPosition")
                {
                    REQUIRE(reader.SetPosInFile(0, UnsafeFile::OriginPosition::CurrentPosition) == true);
                    REQUIRE(reader.GetPosInFile() == 0);
                    REQUIRE(reader.SetPosInFile(2, UnsafeFile::OriginPosition::CurrentPosition) == true);
                    REQUIRE(reader.GetPosInFile() == 2);
                    REQUIRE(reader.SetPosInFile(-13, UnsafeFile::OriginPosition::CurrentPosition) == false);
                    REQUIRE(reader.GetPosInFile() == 2);
                    REQUIRE(reader.SetPosInFile(87, UnsafeFile::OriginPosition::CurrentPosition) == true);
                    REQUIRE(reader.GetPosInFile() == 89);
                    REQUIRE(reader.SetPosInFile(1235, UnsafeFile::OriginPosition::CurrentPosition) == true);
                    REQUIRE(reader.GetPosInFile() == 1324);
                }
                SECTION("Setting from End")
                {
                    REQUIRE(reader.SetPosInFile(0, UnsafeFile::OriginPosition::End) == true);
                    REQUIRE(reader.SetPosInFile(-123, UnsafeFile::OriginPosition::End) == false);
                    REQUIRE(reader.SetPosInFile(44, UnsafeFile::OriginPosition::End) == true);
                    REQUIRE(reader.SetPosInFile(120, UnsafeFile::OriginPosition::End) == true);
                }
            }

            GIVEN("Nonempty file")
            {
                {
                    UnsafeFile creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.Write(data.data(), dataSize) == true);
                }

                UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                SECTION("Setting from Beggining")
                {
                    REQUIRE(reader.SetPosInFile(0, UnsafeFile::OriginPosition::Beggining) == true);
                    REQUIRE(reader.GetPosInFile() == 0);
                    REQUIRE(reader.SetPosInFile(112, UnsafeFile::OriginPosition::Beggining) == true);
                    REQUIRE(reader.GetPosInFile() == 112);
                    REQUIRE(reader.SetPosInFile(-1, UnsafeFile::OriginPosition::Beggining) == false);
                    REQUIRE(reader.GetPosInFile() == 112);
                    REQUIRE(reader.SetPosInFile(-133, UnsafeFile::OriginPosition::Beggining) == false);
                    REQUIRE(reader.GetPosInFile() == 112);
                }
                SECTION("Setting from CurrentPosition")
                {
                    REQUIRE(reader.SetPosInFile(0, UnsafeFile::OriginPosition::CurrentPosition) == true);
                    REQUIRE(reader.GetPosInFile() == 0);
                    REQUIRE(reader.SetPosInFile(88, UnsafeFile::OriginPosition::CurrentPosition) == true);
                    REQUIRE(reader.GetPosInFile() == 88);
                    REQUIRE(reader.SetPosInFile(-87, UnsafeFile::OriginPosition::CurrentPosition) == true);
                    REQUIRE(reader.GetPosInFile() == 1);
                    REQUIRE(reader.SetPosInFile(-87, UnsafeFile::OriginPosition::CurrentPosition) == false);
                    REQUIRE(reader.GetPosInFile() == 1);
                    REQUIRE(reader.SetPosInFile(1235, UnsafeFile::OriginPosition::CurrentPosition) == true);
                    REQUIRE(reader.GetPosInFile() == 1236);
                }
                SECTION("Setting from End")
                {
                    REQUIRE(reader.SetPosInFile(0, UnsafeFile::OriginPosition::End) == true);
                    REQUIRE(reader.SetPosInFile(-7, UnsafeFile::OriginPosition::End) == true);
                    REQUIRE(reader.SetPosInFile(4, UnsafeFile::OriginPosition::End) == true);
                    REQUIRE(reader.SetPosInFile(10, UnsafeFile::OriginPosition::End) == true);
                }
            }

        }
    }

}

#endif // UNSAFEFILE_POSITIONFUNCTIONS_H
