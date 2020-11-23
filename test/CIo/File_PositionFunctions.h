#ifndef FILE_POSITIONFUNCTIONS_H
#define FILE_POSITIONFUNCTIONS_H

#include "File_TestingUtilities.h"


namespace CIo::FileTesting
{

    TEMPLATE_TEST_CASE("[File] : GetPosInFile should return the current position in file", "[File][GetPosInFile]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;
        using Position   = typename File::Position;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr size_t dataSize = 100;
        const std::array<int, dataSize> data = {0, 132029, -346};
        std::array<int, dataSize> buffer;
        WHEN("reading GetPosInFile should return the current position in file")
        {
            WHEN("Reading an empty file it should always return 0")
            {
                REQUIRE(File::CreateFile(cleaner.Filename()) == true);

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
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
                    File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(writer.IsOpen() == true);
                    REQUIRE(writer.Write(data.data(), dataSize) == true);
                }

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
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
                File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
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
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.Write(data.data(), dataSize) == true);
                }

                File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Binary);
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
                File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::Binary);
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
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.Write(data.data(), dataSize) == true);
                }

                File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::Binary);

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

        WHEN("GetPosInFile is used on unopen file it should return File::ErrorPos")
        {
            File file;
            REQUIRE(file.GetPosInFile() == File::ErrorPos);
        }
    }

    TEMPLATE_TEST_CASE("[File] : MoveToBeggining should set the possition in the file to the beggining", "[File][MoveToBeggining]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;

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
                File::CreateFile(cleaner.Filename());
                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                reader.MoveToBeggining();
                REQUIRE(reader.GetPosInFile() == 0);
                reader.MoveToBeggining();
                REQUIRE(reader.GetPosInFile() == 0);
            }

            GIVEN("Nonempty file")
            {
                {
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.Write(data.data(), dataSize) == true);
                }

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                reader.MoveToBeggining();
                REQUIRE(reader.GetPosInFile() == 0);

                REQUIRE(reader.Read(buffer.data(), dataSize) == true);
                REQUIRE(buffer == data);

                reader.MoveToBeggining();
                REQUIRE(reader.GetPosInFile() == 0);
            }
        }

        WHEN("MoveToBeggining is used on unopen file it should do nothing")
        {
            File file;
            file.MoveToBeggining();
        }
    }

    TEMPLATE_TEST_CASE("[File] : MoveToEnd should set the possition in the file to the end", "[File][MoveToEnd]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;
        using Position   = typename File::Position;

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
                File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Read, OpenModeFlag::Binary);
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
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.Write(data.data(), dataSize) == true);
                }

                File writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
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

        WHEN("MoveToEnd is used on unopen file it should do nothing")
        {
            File file;
            file.MoveToEnd();
        }
    }

    TEMPLATE_TEST_CASE("[File] : MoveBy should offset the current possition by the ammount specified", "[File][MoveBy]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr size_t dataSize = 100;
        const std::array<int, dataSize> data = {-231, 54345, 21};

        WHEN("MoveBy is used it should offset the current possition by the ammount specified and fail when the resulting position would be negative")
        {
            GIVEN("Empty file")
            {
                File::CreateFile(cleaner.Filename());
                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

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
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.Write(data.data(), dataSize) == true);
                }

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

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

        WHEN("MoveBy is used on unopen file it should return false and fail")
        {
            File file;
            REQUIRE(file.MoveBy(0) == false);
            REQUIRE(file.MoveBy(10) == false);
            REQUIRE(file.MoveBy(-875) == false);
            REQUIRE(file.MoveBy(1451) == false);
            REQUIRE(file.MoveBy(888814) == false);
        }
    }

    TEMPLATE_TEST_CASE("[File] : SetPosInFile should set the possition in the file according to the given arguments", "[File][SetPosInFile]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr size_t dataSize = 100;
        const std::array<int, dataSize> data = {0, 132029, -346};

        WHEN("SetPosInFile is used it should set the position to according to the given arguments and fail if the resulting position would be negative")
        {
            GIVEN("Empty file")
            {
                File::CreateFile(cleaner.Filename());
                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                SECTION("Setting from Beggining")
                {
                    REQUIRE(reader.SetPosInFile(0, File::OriginPosition::Beggining) == true);
                    REQUIRE(reader.GetPosInFile() == 0);
                    REQUIRE(reader.SetPosInFile(113, File::OriginPosition::Beggining) == true);
                    REQUIRE(reader.GetPosInFile() == 113);
                    REQUIRE(reader.SetPosInFile(-1, File::OriginPosition::Beggining) == false);
                    REQUIRE(reader.GetPosInFile() == 113);
                    REQUIRE(reader.SetPosInFile(-113, File::OriginPosition::Beggining) == false);
                    REQUIRE(reader.GetPosInFile() == 113);
                }
                SECTION("Setting from CurrentPosition")
                {
                    REQUIRE(reader.SetPosInFile(0, File::OriginPosition::CurrentPosition) == true);
                    REQUIRE(reader.GetPosInFile() == 0);
                    REQUIRE(reader.SetPosInFile(2, File::OriginPosition::CurrentPosition) == true);
                    REQUIRE(reader.GetPosInFile() == 2);
                    REQUIRE(reader.SetPosInFile(-13, File::OriginPosition::CurrentPosition) == false);
                    REQUIRE(reader.GetPosInFile() == 2);
                    REQUIRE(reader.SetPosInFile(87, File::OriginPosition::CurrentPosition) == true);
                    REQUIRE(reader.GetPosInFile() == 89);
                    REQUIRE(reader.SetPosInFile(1235, File::OriginPosition::CurrentPosition) == true);
                    REQUIRE(reader.GetPosInFile() == 1324);
                }
                SECTION("Setting from End")
                {
                    REQUIRE(reader.SetPosInFile(0, File::OriginPosition::End) == true);
                    REQUIRE(reader.SetPosInFile(-123, File::OriginPosition::End) == false);
                    REQUIRE(reader.SetPosInFile(44, File::OriginPosition::End) == true);
                    REQUIRE(reader.SetPosInFile(120, File::OriginPosition::End) == true);
                }
            }

            GIVEN("Nonempty file")
            {
                {
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen() == true);
                    REQUIRE(creator.Write(data.data(), dataSize) == true);
                }

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                SECTION("Setting from Beggining")
                {
                    REQUIRE(reader.SetPosInFile(0, File::OriginPosition::Beggining) == true);
                    REQUIRE(reader.GetPosInFile() == 0);
                    REQUIRE(reader.SetPosInFile(112, File::OriginPosition::Beggining) == true);
                    REQUIRE(reader.GetPosInFile() == 112);
                    REQUIRE(reader.SetPosInFile(-1, File::OriginPosition::Beggining) == false);
                    REQUIRE(reader.GetPosInFile() == 112);
                    REQUIRE(reader.SetPosInFile(-133, File::OriginPosition::Beggining) == false);
                    REQUIRE(reader.GetPosInFile() == 112);
                }
                SECTION("Setting from CurrentPosition")
                {
                    REQUIRE(reader.SetPosInFile(0, File::OriginPosition::CurrentPosition) == true);
                    REQUIRE(reader.GetPosInFile() == 0);
                    REQUIRE(reader.SetPosInFile(88, File::OriginPosition::CurrentPosition) == true);
                    REQUIRE(reader.GetPosInFile() == 88);
                    REQUIRE(reader.SetPosInFile(-87, File::OriginPosition::CurrentPosition) == true);
                    REQUIRE(reader.GetPosInFile() == 1);
                    REQUIRE(reader.SetPosInFile(-87, File::OriginPosition::CurrentPosition) == false);
                    REQUIRE(reader.GetPosInFile() == 1);
                    REQUIRE(reader.SetPosInFile(1235, File::OriginPosition::CurrentPosition) == true);
                    REQUIRE(reader.GetPosInFile() == 1236);
                }
                SECTION("Setting from End")
                {
                    REQUIRE(reader.SetPosInFile(0, File::OriginPosition::End) == true);
                    REQUIRE(reader.SetPosInFile(-7, File::OriginPosition::End) == true);
                    REQUIRE(reader.SetPosInFile(4, File::OriginPosition::End) == true);
                    REQUIRE(reader.SetPosInFile(10, File::OriginPosition::End) == true);
                }
            }

        }

        WHEN("SetPosInFile is used on unopen file it should return false and fail")
        {
            File file;
            REQUIRE(file.SetPosInFile(0, File::OriginPosition::Beggining) == false);
            REQUIRE(file.SetPosInFile(14, File::OriginPosition::Beggining) == false);
            REQUIRE(file.SetPosInFile(-1847, File::OriginPosition::Beggining) == false);
            REQUIRE(file.SetPosInFile(85198, File::OriginPosition::Beggining) == false);

            REQUIRE(file.SetPosInFile(0, File::OriginPosition::CurrentPosition) == false);
            REQUIRE(file.SetPosInFile(90, File::OriginPosition::CurrentPosition) == false);
            REQUIRE(file.SetPosInFile(-241, File::OriginPosition::CurrentPosition) == false);
            REQUIRE(file.SetPosInFile(16471, File::OriginPosition::CurrentPosition) == false);

            REQUIRE(file.SetPosInFile(0, File::OriginPosition::End) == false);
            REQUIRE(file.SetPosInFile(-13, File::OriginPosition::End) == false);
            REQUIRE(file.SetPosInFile(-241, File::OriginPosition::End) == false);
            REQUIRE(file.SetPosInFile(16471, File::OriginPosition::End) == false);
        }
    }

}

#endif // FILE_POSITIONFUNCTIONS_H
