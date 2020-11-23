#ifndef USEEXAMPLESTEST_H
#define USEEXAMPLESTEST_H

#include <algorithm>

#include "Catch2/Catch.hpp"
#include "src/UseExamples.h"
#include "File_TestingUtilities.h"

namespace CIo::UseExamples::Testing
{
    template<typename CharT>
    using BasicFilenameCleaner = FileManagerTesting::BasicFilenameCleaner<CharT>;

    using FilenameCleaner = BasicFilenameCleaner<char8>;
    using WFilenameCleaner = BasicFilenameCleaner<charW>;
}

namespace CIo::UseExamples::Testing
{
    TEST_CASE("[UseExamples] : WriteToFile shoudl write to the file all of data provided", "[UseExamples][WriteToFile]")
    {
        FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        SECTION("Size 100")
        {
            constexpr size_t dataSize = 100;
            const std::array<char8, dataSize> data = {'a', 'b', 'x', 'y'};
            std::array<char8, dataSize> buffer;

            REQUIRE(WriteToFile(cleaner.Filename(), data.data(), data.size()) == true);

            File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

            REQUIRE(reader.IsOpen() == true);
            REQUIRE(reader.Read(buffer.data(), buffer.size()) == true);
            REQUIRE(data == buffer);
        }

        SECTION("Size 2415")
        {
            constexpr size_t dataSize = 2415;
            const std::array<char8, dataSize> data = {'a', 'b', 'x', 'y'};
            std::array<char8, dataSize> buffer;

            REQUIRE(WriteToFile(cleaner.Filename(), data.data(), data.size()) == true);

            File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

            REQUIRE(reader.IsOpen() == true);
            REQUIRE(reader.Read(buffer.data(), buffer.size()) == true);
            REQUIRE(data == buffer);
        }

        SECTION("Size 3")
        {
            constexpr size_t dataSize = 3;
            const std::array<char8, dataSize> data = {'a', 'b', 'x'};
            std::array<char8, dataSize> buffer;

            REQUIRE(WriteToFile(cleaner.Filename(), data.data(), data.size()) == true);

            File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

            REQUIRE(reader.IsOpen() == true);
            REQUIRE(reader.Read(buffer.data(), buffer.size()) == true);
            REQUIRE(data == buffer);
        }

        SECTION("Size 0")
        {
            constexpr size_t dataSize = 0;
            const std::array<char8, 1> data = {};
            std::array<char8, 1> buffer = {};

            REQUIRE(WriteToFile(cleaner.Filename(), data.data(), dataSize) == true);

            File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

            REQUIRE(reader.IsOpen() == true);
            REQUIRE(reader.Read(buffer.data(), dataSize) == true);
            REQUIRE(data == buffer);
            REQUIRE(reader.GetFileSize() == 0);
        }

        WHEN("Given invalid filename should fail and return false")
        {
            const std::array<char8, 10> data = {'a', 'b', 'x', 'y'};
            REQUIRE(WriteToFile("", data.data(), data.size()) == false);
        }
    }

    TEST_CASE("[UseExamples] : AppendToFile shoudl append to the file all of data provided", "[UseExamples][AppendToFile]")
    {
        FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("Writing to a created file, just the data written should be in the file")
        {
            SECTION("Size 100")
            {
                constexpr size_t dataSize = 100;
                const std::array<char8, dataSize> data = {'a', 'b', 'x', 'y'};
                std::array<char8, dataSize> buffer;

                REQUIRE(AppendToFile(cleaner.Filename(), data.data(), data.size()) == true);

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                REQUIRE(reader.IsOpen() == true);
                REQUIRE(reader.Read(buffer.data(), buffer.size()) == true);
                REQUIRE(data == buffer);
            }

            SECTION("Size 2415")
            {
                constexpr size_t dataSize = 2415;
                const std::array<char8, dataSize> data = {'a', 'b', 'x', 'y'};
                std::array<char8, dataSize> buffer;

                REQUIRE(AppendToFile(cleaner.Filename(), data.data(), data.size()) == true);

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                REQUIRE(reader.IsOpen() == true);
                REQUIRE(reader.Read(buffer.data(), buffer.size()) == true);
                REQUIRE(data == buffer);
            }

            SECTION("Size 3")
            {
                constexpr size_t dataSize = 3;
                const std::array<char8, dataSize> data = {'a', 'b', 'x'};
                std::array<char8, dataSize> buffer;

                REQUIRE(AppendToFile(cleaner.Filename(), data.data(), data.size()) == true);

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                REQUIRE(reader.IsOpen() == true);
                REQUIRE(reader.Read(buffer.data(), buffer.size()) == true);
                REQUIRE(data == buffer);
            }

            SECTION("Size 0")
            {
                constexpr size_t dataSize = 0;
                const std::array<char8, 1> data = {};
                std::array<char8, 1> buffer = {};

                REQUIRE(AppendToFile(cleaner.Filename(), data.data(), dataSize) == true);

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                REQUIRE(reader.IsOpen() == true);
                REQUIRE(reader.Read(buffer.data(), dataSize) == true);
                REQUIRE(data == buffer);
                REQUIRE(reader.GetFileSize() == 0);
            }

            WHEN("Given invalid filename should fail and return false")
            {
                const std::array<char8, 10> data = {'a', 'b', 'x', 'y'};
                REQUIRE(AppendToFile("", data.data(), data.size()) == false);
            }

        }

        WHEN("Writing to already created file, the datat shiuld be appended")
        {
            SECTION("Size 100")
            {
                constexpr size_t prewrittenSize = 88;
                const std::array<char8, prewrittenSize> prewritten = {'a', 'b', 'x', 'y'};
                std::array<char8, prewrittenSize> prewrittenBuffer;

                {
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen());
                    REQUIRE(creator.Write(prewritten.data(), prewritten.size()));
                }

                constexpr size_t dataSize = 100;
                const std::array<char8, dataSize> data = {'a', 'b', 'x', 'y'};
                std::array<char8, dataSize> buffer;

                REQUIRE(AppendToFile(cleaner.Filename(), data.data(), data.size()) == true);

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.Read(prewrittenBuffer.data(), prewrittenSize) == true);
                REQUIRE(prewritten == prewrittenBuffer);

                REQUIRE(reader.Read(buffer.data(), dataSize) == true);
                REQUIRE(data == buffer);
            }
            SECTION("Size 321")
            {
                constexpr size_t prewrittenSize = 1200;
                const std::array<char8, prewrittenSize> prewritten = {'a', 'b', 'x', 'y'};
                std::array<char8, prewrittenSize> prewrittenBuffer;

                {
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen());
                    REQUIRE(creator.Write(prewritten.data(), prewritten.size()));
                }

                constexpr size_t dataSize = 321;
                const std::array<char8, dataSize> data = {'a', 'b', 'x', 'y'};
                std::array<char8, dataSize> buffer;

                REQUIRE(AppendToFile(cleaner.Filename(), data.data(), data.size()) == true);

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.Read(prewrittenBuffer.data(), prewrittenSize) == true);
                REQUIRE(prewritten == prewrittenBuffer);

                REQUIRE(reader.Read(buffer.data(), dataSize) == true);
                REQUIRE(data == buffer);
            }
            SECTION("Size 0")
            {
                constexpr size_t prewrittenSize = 38;
                const std::array<char8, prewrittenSize> prewritten = {'a', 'b', 'x', 'y'};
                std::array<char8, prewrittenSize> prewrittenBuffer;

                {
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen());
                    REQUIRE(creator.Write(prewritten.data(), prewritten.size()));
                }

                constexpr size_t dataSize = 0;
                const std::array<char8, 1> data = {};
                std::array<char8, 1> buffer = {};

                REQUIRE(AppendToFile(cleaner.Filename(), data.data(), data.size()) == true);

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.Read(prewrittenBuffer.data(), prewrittenSize) == true);
                REQUIRE(prewritten == prewrittenBuffer);

                REQUIRE(reader.Read(buffer.data(), dataSize) == true);
                REQUIRE(data == buffer);
            }
            SECTION("Size 4")
            {
                constexpr size_t prewrittenSize = 65;
                const std::array<char8, prewrittenSize> prewritten = {'a', 'b', 'x', 'y'};
                std::array<char8, prewrittenSize> prewrittenBuffer;

                {
                    File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                    REQUIRE(creator.IsOpen());
                    REQUIRE(creator.Write(prewritten.data(), prewritten.size()));
                }

                constexpr size_t dataSize = 100;
                const std::array<char8, dataSize> data = {'a', 'b', 'x', 'y'};
                std::array<char8, dataSize> buffer;

                REQUIRE(AppendToFile(cleaner.Filename(), data.data(), data.size()) == true);

                File reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

                REQUIRE(reader.IsOpen() == true);

                REQUIRE(reader.Read(prewrittenBuffer.data(), prewrittenSize) == true);
                REQUIRE(prewritten == prewrittenBuffer);

                REQUIRE(reader.Read(buffer.data(), dataSize) == true);
                REQUIRE(data == buffer);
            }
        }

    }

    TEST_CASE("[UseExamples] : ReadFileByChunks shoudl read all of the data in the file", "[UseExamples][ReadFileByChunks]")
    {

        FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        size_t bufferFill = 0;
        bool called = false;
        WHEN("Called on existant file, shoudl return true")
        {
            SECTION("Size 100")
            {
                constexpr size_t dataSize = 100;
                const std::array<char8, dataSize> data = {'a', 'b', 'x', 'y'};
                std::array<char8, dataSize> buffer;

                auto writeTobuffer = [&](const char * read, size_t readSize)
                {
                    called = true;
                    memcpy_s(buffer.data() + bufferFill, buffer.size() - bufferFill, read, readSize);
                    bufferFill += readSize;
                };

                REQUIRE(WriteToFile(cleaner.Filename(), data.data(), dataSize) == true);
                REQUIRE(ReadFileByChunks(cleaner.Filename(), writeTobuffer) == true);
                REQUIRE(called == true);
                REQUIRE(bufferFill == dataSize);
                REQUIRE(data == buffer);
            }
            SECTION("Size 0")
            {
                constexpr size_t dataSize = 0;
                const std::array<char8, 1> data = {};
                std::array<char8, 1> buffer = {};

                auto writeTobuffer = [&](const char * read, size_t readSize)
                {
                    called = true;
                    memcpy_s(buffer.data() + bufferFill, buffer.size() - bufferFill, read, readSize);
                    bufferFill += readSize;
                };

                REQUIRE(WriteToFile(cleaner.Filename(), data.data(), dataSize) == true);
                REQUIRE(ReadFileByChunks(cleaner.Filename(), writeTobuffer) == true);
                REQUIRE(called == false);
                REQUIRE(bufferFill == dataSize);
                REQUIRE(data == buffer);
            }
            SECTION("Size 30201")
            {
                constexpr size_t dataSize = 30201;
                const std::array<char8, dataSize> data = {'a', 'b', 'x', 'y'};
                std::array<char8, dataSize> buffer;

                auto writeTobuffer = [&](const char * read, size_t readSize)
                {
                    called = true;
                    memcpy_s(buffer.data() + bufferFill, buffer.size() - bufferFill, read, readSize);
                    bufferFill += readSize;
                };

                REQUIRE(WriteToFile(cleaner.Filename(), data.data(), dataSize) == true);
                REQUIRE(ReadFileByChunks(cleaner.Filename(), writeTobuffer) == true);
                REQUIRE(called == true);
                REQUIRE(bufferFill == dataSize);
                REQUIRE(data == buffer);
            }
            SECTION("Size 3")
            {
                constexpr size_t dataSize = 3;
                const std::array<char8, dataSize> data = {'a', 'b', 'x'};
                std::array<char8, dataSize> buffer;

                auto writeTobuffer = [&](const char * read, size_t readSize)
                {
                    called = true;
                    memcpy_s(buffer.data() + bufferFill, buffer.size() - bufferFill, read, readSize);
                    bufferFill += readSize;
                };

                REQUIRE(WriteToFile(cleaner.Filename(), data.data(), dataSize) == true);
                REQUIRE(ReadFileByChunks(cleaner.Filename(), writeTobuffer) == true);
                REQUIRE(called == true);
                REQUIRE(bufferFill == dataSize);
                REQUIRE(data == buffer);
            }
        }
        WHEN("Called on nonexistant file, shoudl return false")
        {
            auto emptyFunction = [&](const char *, size_t){};
            REQUIRE(ReadFileByChunks(cleaner.Filename(), emptyFunction) == false);
        }
    }

    TEST_CASE("[UseExamples] : ReadFileWithProgress shoudl read all of the data in the file and repeatedly report progress", "[UseExamples][ReadFileWithProgress]")
    {
        FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        size_t bufferFill = 0;
        bool calledWritetoBuffer = false;
        bool calledUpdateProgress = false;
        File::Position currentPos = 0;
        File::Position maxPos = 0;

        auto updateProgress = [&](File::Position current, File::Position max)
        {
            currentPos = current;
            maxPos = max;
            calledUpdateProgress = true;
        };
        WHEN("Called on existant file, shoudl return true")
        {
            SECTION("Size 100")
            {
                constexpr size_t dataSize = 100;
                const std::array<char8, dataSize> data = {'a', 'b', 'x', 'y'};
                std::array<char8, dataSize> buffer;

                auto writeTobuffer = [&](const char * read, size_t readSize)
                {
                    memcpy_s(buffer.data() + bufferFill, buffer.size() - bufferFill, read, readSize);
                    bufferFill += readSize;
                    calledWritetoBuffer = true;
                };

                REQUIRE(WriteToFile(cleaner.Filename(), data.data(), dataSize) == true);
                REQUIRE(ReadFileWithProgress(cleaner.Filename(), writeTobuffer, updateProgress) == true);

                REQUIRE(maxPos > 0);
                REQUIRE(maxPos == currentPos);

                REQUIRE(calledWritetoBuffer == true);
                REQUIRE(calledUpdateProgress == true);

                REQUIRE(bufferFill == dataSize);
                REQUIRE(data == buffer);
            }
            SECTION("Size 0")
            {
                constexpr size_t dataSize = 0;
                const std::array<char8, 1> data = {};
                std::array<char8, 1> buffer = {};

                auto writeTobuffer = [&](const char * read, size_t readSize)
                {
                    memcpy_s(buffer.data() + bufferFill, buffer.size() - bufferFill, read, readSize);
                    bufferFill += readSize;
                    calledWritetoBuffer = true;
                };

                REQUIRE(WriteToFile(cleaner.Filename(), data.data(), dataSize) == true);
                REQUIRE(ReadFileWithProgress(cleaner.Filename(), writeTobuffer, updateProgress) == true);

                REQUIRE(maxPos == 0);
                REQUIRE(currentPos == 0);

                REQUIRE(calledWritetoBuffer == false);
                REQUIRE(calledUpdateProgress == true);

                REQUIRE(bufferFill == dataSize);
                REQUIRE(data == buffer);
            }
            SECTION("Size 30201")
            {
                constexpr size_t dataSize = 30201;
                const std::array<char8, dataSize> data = {'a', 'b', 'x', 'y'};
                std::array<char8, dataSize> buffer;

                auto writeTobuffer = [&](const char * read, size_t readSize)
                {
                    memcpy_s(buffer.data() + bufferFill, buffer.size() - bufferFill, read, readSize);
                    bufferFill += readSize;
                    calledWritetoBuffer = true;
                };

                REQUIRE(WriteToFile(cleaner.Filename(), data.data(), dataSize) == true);
                REQUIRE(ReadFileWithProgress(cleaner.Filename(), writeTobuffer, updateProgress) == true);

                REQUIRE(maxPos > 0);
                REQUIRE(maxPos == currentPos);

                REQUIRE(calledWritetoBuffer == true);
                REQUIRE(calledUpdateProgress == true);

                REQUIRE(bufferFill == dataSize);
                REQUIRE(data == buffer);
            }
            SECTION("Size 3")
            {
                constexpr size_t dataSize = 3;
                const std::array<char8, dataSize> data = {'a', 'b', 'x'};
                std::array<char8, dataSize> buffer;

                auto writeTobuffer = [&](const char * read, size_t readSize)
                {
                    memcpy_s(buffer.data() + bufferFill, buffer.size() - bufferFill, read, readSize);
                    bufferFill += readSize;
                    calledWritetoBuffer = true;
                };

                REQUIRE(WriteToFile(cleaner.Filename(), data.data(), dataSize) == true);
                REQUIRE(ReadFileWithProgress(cleaner.Filename(), writeTobuffer, updateProgress) == true);

                REQUIRE(maxPos > 0);
                REQUIRE(maxPos == currentPos);

                REQUIRE(calledWritetoBuffer == true);
                REQUIRE(calledUpdateProgress == true);

                REQUIRE(bufferFill == dataSize);
                REQUIRE(data == buffer);
            }
        }
        WHEN("Called on nonexistant file, shoudl return false")
        {
            auto emptyFunction = [&](const char *, size_t){};
            REQUIRE(ReadFileWithProgress(cleaner.Filename(), emptyFunction, updateProgress) == false);
        }
    }

    TEST_CASE("[UseExamples] : ReadFileFaster shoudl read all of the data in the file", "[UseExamples][ReadFileFaster]")
    {
        FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        size_t bufferFill = 0;
        bool called = false;
        WHEN("Called on existant file, shoudl return true")
        {
            SECTION("Size 200")
            {
                constexpr size_t dataSize = 200;
                const std::array<char8, dataSize> data = {'a', 'b', 'x', 'y'};
                std::array<char8, dataSize> buffer;

                auto writeTobuffer = [&](const char * read, size_t readSize)
                {
                    called = true;
                    memcpy_s(buffer.data() + bufferFill, buffer.size() - bufferFill, read, readSize);
                    bufferFill += readSize;
                };

                REQUIRE(WriteToFile(cleaner.Filename(), data.data(), dataSize) == true);
                REQUIRE(ReadFileByChunks(cleaner.Filename(), writeTobuffer) == true);
                REQUIRE(called == true);
                REQUIRE(bufferFill == dataSize);
                REQUIRE(data == buffer);
            }
            SECTION("Size 0")
            {
                constexpr size_t dataSize = 0;
                const std::array<char8, 1> data = {};
                std::array<char8, 1> buffer = {};

                auto writeTobuffer = [&](const char * read, size_t readSize)
                {
                    called = true;
                    memcpy_s(buffer.data() + bufferFill, buffer.size() - bufferFill, read, readSize);
                    bufferFill += readSize;
                };

                REQUIRE(WriteToFile(cleaner.Filename(), data.data(), dataSize) == true);
                REQUIRE(ReadFileByChunks(cleaner.Filename(), writeTobuffer) == true);
                REQUIRE(called == false);
                REQUIRE(bufferFill == dataSize);
                REQUIRE(data == buffer);
            }
            SECTION("Size 24892")
            {
                constexpr size_t dataSize = 24892;
                const std::array<char8, dataSize> data = {'a', 'b', 'x', 'y'};
                std::array<char8, dataSize> buffer;

                auto writeTobuffer = [&](const char * read, size_t readSize)
                {
                    called = true;
                    memcpy_s(buffer.data() + bufferFill, buffer.size() - bufferFill, read, readSize);
                    bufferFill += readSize;
                };

                REQUIRE(WriteToFile(cleaner.Filename(), data.data(), dataSize) == true);
                REQUIRE(ReadFileByChunks(cleaner.Filename(), writeTobuffer) == true);
                REQUIRE(called == true);
                REQUIRE(bufferFill == dataSize);
                REQUIRE(data == buffer);
            }
            SECTION("Size 7")
            {
                constexpr size_t dataSize = 7;
                const std::array<char8, dataSize> data = {'a', 'b', 'x'};
                std::array<char8, dataSize> buffer;

                auto writeTobuffer = [&](const char * read, size_t readSize)
                {
                    called = true;
                    memcpy_s(buffer.data() + bufferFill, buffer.size() - bufferFill, read, readSize);
                    bufferFill += readSize;
                };

                REQUIRE(WriteToFile(cleaner.Filename(), data.data(), dataSize) == true);
                REQUIRE(ReadFileByChunks(cleaner.Filename(), writeTobuffer) == true);
                REQUIRE(called == true);
                REQUIRE(bufferFill == dataSize);
                REQUIRE(data == buffer);
            }
        }
        WHEN("Called on nonexistant file, shoudl return false")
        {
            auto emptyFunction = [&](const char *, size_t){};
            REQUIRE(ReadFileFaster(cleaner.Filename(), emptyFunction) == false);
        }
    }

    TEST_CASE("[UseExamples] : ReadDoublesBackwards shoudl read all of the data in the file starting from the back", "[UseExamples][ReadDoublesBackwards]")
    {
        FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        size_t bufferFill = 0;
        bool calledWritetoBuffer = false;

        WHEN("Called on existant file, shoudl return true")
        {
            SECTION("Size 100")
            {
                constexpr size_t dataSize = 100;
                const std::array<double, dataSize> data = {100302, 140194, 958, 85758, 7236};
                std::array<double, dataSize> buffer = {};

                auto writeTobuffer = [&](double unit)
                {
                    buffer[bufferFill] = unit;
                    bufferFill ++;
                    calledWritetoBuffer = true;
                };

                const char * dataPtr = reinterpret_cast<const char *>(data.data());
                REQUIRE(WriteToFile(cleaner.Filename(), dataPtr, dataSize * sizeof (double)) == true);
                REQUIRE(ReadDoublesBackwards(cleaner.Filename(), writeTobuffer) == true);

                std::reverse(buffer.begin(), buffer.end());

                REQUIRE(calledWritetoBuffer == true);
                REQUIRE(bufferFill == dataSize);
                REQUIRE(data == buffer);
            }

            SECTION("Size 6541")
            {
                constexpr size_t dataSize = 6541;
                const std::array<double, dataSize> data = {100302, 140194, 958, 85758, 7236};
                std::array<double, dataSize> buffer = {};

                auto writeTobuffer = [&](double unit)
                {
                    buffer[bufferFill] = unit;
                    bufferFill ++;
                    calledWritetoBuffer = true;
                };

                const char * dataPtr = reinterpret_cast<const char *>(data.data());
                REQUIRE(WriteToFile(cleaner.Filename(), dataPtr, dataSize * sizeof (double)) == true);
                REQUIRE(ReadDoublesBackwards(cleaner.Filename(), writeTobuffer) == true);

                std::reverse(buffer.begin(), buffer.end());

                REQUIRE(calledWritetoBuffer == true);
                REQUIRE(bufferFill == dataSize);
                REQUIRE(data == buffer);
            }

            SECTION("Size 0")
            {
                constexpr size_t dataSize = 0;
                const std::array<double, dataSize> data = {};
                std::array<double, dataSize> buffer = {};

                auto writeTobuffer = [&](double unit)
                {
                    buffer[bufferFill] = unit;
                    bufferFill ++;
                    calledWritetoBuffer = true;
                };

                const char * dataPtr = reinterpret_cast<const char *>(data.data());
                REQUIRE(WriteToFile(cleaner.Filename(), dataPtr, dataSize * sizeof (double)) == true);
                REQUIRE(ReadDoublesBackwards(cleaner.Filename(), writeTobuffer) == false);

                std::reverse(buffer.begin(), buffer.end());

                REQUIRE(bufferFill == dataSize);
                REQUIRE(data == buffer);
            }

            SECTION("Size 6")
            {
                constexpr size_t dataSize = 6;
                const std::array<double, dataSize> data = {100302, 140194, 958, 85758, 7236};
                std::array<double, dataSize> buffer = {};

                auto writeTobuffer = [&](double unit)
                {
                    buffer[bufferFill] = unit;
                    bufferFill ++;
                    calledWritetoBuffer = true;
                };

                const char * dataPtr = reinterpret_cast<const char *>(data.data());
                REQUIRE(WriteToFile(cleaner.Filename(), dataPtr, dataSize * sizeof (double)) == true);
                REQUIRE(ReadDoublesBackwards(cleaner.Filename(), writeTobuffer) == true);

                std::reverse(buffer.begin(), buffer.end());

                REQUIRE(calledWritetoBuffer == true);
                REQUIRE(bufferFill == dataSize);
                REQUIRE(data == buffer);
            }
        }
        WHEN("Called on nonexistant file, shoudl return false")
        {
            auto emptyFunction = [](double){};
            REQUIRE(ReadDoublesBackwards(cleaner.Filename(), emptyFunction) == false);
        }
    }

    TEST_CASE("[UseExamples] : WriteFileUtfString write the specified strings into the file", "[UseExamples][WriteFileUtfString]")
    {
        WHEN("Given valid filename it should write the strings and return false")
        {
            WFilenameCleaner cleaner;
            REQUIRE(cleaner.IsOk() == true);

            REQUIRE(WriteFileUtfStrings(cleaner.Filename()) == true);

            WFile reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);

            const std::basic_string_view<char>     expected1 = "My msg";
            const std::basic_string_view<wchar_t>  expected2 = L"My msg";
            const std::basic_string_view<char16_t> expected3 = u"My msg";
            const std::basic_string_view<char32_t> expected4 = U"My msg";
            const std::basic_string_view<char>     expected5 = "My msg";
            const std::basic_string_view<wchar_t>  expected6 = L"My msg";
            const std::basic_string_view<char32_t> expected7 = U"My msg";

            constexpr size_t strMaxSize = 50;

            char read1[strMaxSize];
            wchar_t read2[strMaxSize];
            char16_t read3[strMaxSize];
            char32_t read4[strMaxSize];
            char read5[strMaxSize];
            wchar_t read6[strMaxSize];
            char32_t read7[strMaxSize];

            REQUIRE(reader.ReadString(ToCStringRef(read1, expected1.size())) == true);
            REQUIRE(reader.ReadString(ToCStringRef(read2, expected2.size())) == true);
            REQUIRE(reader.ReadString(ToCStringRef(read3, expected3.size())) == true);
            REQUIRE(reader.ReadString(ToCStringRef(read4, expected4.size())) == true);
            REQUIRE(reader.ReadString(ToCStringRef(read5, expected5.size())) == true);
            REQUIRE(reader.ReadString(ToCStringRef(read6, expected6.size())) == true);
            REQUIRE(reader.ReadString(ToCStringRef(read7, expected7.size())) == true);


            const std::basic_string_view<char>     view1(read1, expected1.size());
            const std::basic_string_view<wchar_t>  view2(read2, expected2.size());

            const std::basic_string_view<char16_t> view3(read3, expected3.size());
            const std::basic_string_view<char32_t> view4(read4, expected4.size());
            const std::basic_string_view<char>     view5(read5, expected5.size());
            const std::basic_string_view<wchar_t>  view6(read6, expected6.size());
            const std::basic_string_view<char32_t>  view7(read7, expected7.size());


            REQUIRE(view1 == expected1);
            REQUIRE(view2 == expected2);
            REQUIRE(view3 == expected3);
            REQUIRE(view4 == expected4);
            REQUIRE(view5 == expected5);
            REQUIRE(view6 == expected6);

            REQUIRE(reader.ReadString(ToCStringRef(read1, expected1.size())) == true);
            REQUIRE(view1 == expected1);

            REQUIRE(reader.ReadString(ToCStringRef(read1, expected1.size())) == true);
            REQUIRE(view1 == expected1);

        }

        WHEN("Given invalid filename should fail and return false")
        {
            REQUIRE(WriteFileUtfStrings(L"") == false);
        }
    }
}

#endif // USEEXAMPLESTEST_H
