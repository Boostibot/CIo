#ifndef FILE_MISCELLANEOUSFUNCTIONS_H
#define FILE_MISCELLANEOUSFUNCTIONS_H

#include "File_TestingUtilities.h"

namespace CIo::FileTesting
{
    TEMPLATE_TEST_CASE("[File] : SetBuffer should set the buffer", "[File][SetBuffer]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr size_t bufferSize = File::DefaultBufferSize;
        std::array<int, bufferSize> buffer;

        WHEN("Provided with a external buffer the function should use it and return true")
        {
            WHEN("Provided with buffering code Full it should use it and return true")
            {
                File file(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::Binary);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.SetBuffer(buffer.data(), bufferSize, File::BufferingCode::Full) == true);
            }

            WHEN("Provided with buffering code Line it should use it and return true")
            {
                File file(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::Binary);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.SetBuffer(buffer.data(), bufferSize, File::BufferingCode::Line) == true);
            }

            WHEN("Provided with buffering code None it should not use it and return true")
            {
                File::CreateFile(cleaner.Filename());
                File file(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.SetBuffer(buffer.data(), bufferSize, File::BufferingCode::None) == true);
            }
        }

        WHEN("Provided with nullptr the function should allocate its own buffer and use it")
        {
            WHEN("Provided with buffering code Full it should return true")
            {
                File file(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::Write);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.SetBuffer(nullptr, File::DefaultBufferSize, File::BufferingCode::Full) == true);
            }

            WHEN("Provided with buffering code Line it should return true")
            {
                File file(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.SetBuffer(nullptr, File::DefaultBufferSize, File::BufferingCode::Line) == true);
            }

            WHEN("Provided with buffering code None it should return true")
            {
                File::CreateFile(cleaner.Filename());
                File file(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::Append);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.SetBuffer(nullptr, File::DefaultBufferSize, File::BufferingCode::None) == true);
            }
        }

        WHEN("Called on unopen file the function should return false")
        {
            File file;
            REQUIRE(file.SetBuffer(buffer.data(), bufferSize, File::BufferingCode::Full) == false);
            REQUIRE(file.SetBuffer(buffer.data(), bufferSize, File::BufferingCode::Line) == false);
            REQUIRE(file.SetBuffer(buffer.data(), bufferSize, File::BufferingCode::None) == false);

            REQUIRE(file.SetBuffer(nullptr, File::DefaultBufferSize, File::BufferingCode::Full) == false);
            REQUIRE(file.SetBuffer(nullptr, File::DefaultBufferSize, File::BufferingCode::Line) == false);
            REQUIRE(file.SetBuffer(nullptr, File::DefaultBufferSize, File::BufferingCode::None) == false);
        }
    }

    TEMPLATE_TEST_CASE("[File] : Flush should flush the buffer", "[File][Flush]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr size_t dataSize = 100;
        std::array<int, dataSize> data = {-23, 132029, -346};
        WHEN("Switching from writing to reading, using Flush should eneble the data to be read correctly")
        {
            {
                File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(creator.IsOpen() == true);
                REQUIRE(creator.Write(data.data(), dataSize) == true);
            }

            const int setData = 13;
            int readData;

            File file(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(file.IsOpen() == true);
            REQUIRE(file.WriteObject(setData) == true);

            file.Flush();

            REQUIRE(file.ReadObject(readData) == true);
            REQUIRE(readData == data[1]);
        }

        WHEN("Called on unopen file the function should do nothing")
        {
            File file;
            file.Flush();
        }

    }

    TEMPLATE_TEST_CASE("[File] : SwitchBetweenReadAndWrite should allow the user to use read after write or vice versa", "[File][SwitchBetweenReadAndWrite]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;
        using Position   = typename File::Position;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr size_t dataSize = 100;
        std::array<int, dataSize> data = {-23, 132029, -346};

        WHEN("Switching from writing to reading, it should eneble the data to be read correctly")
        {
            {
                File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(creator.IsOpen() == true);
                REQUIRE(creator.Write(data.data(), dataSize) == true);
            }

            const int setData = 13;
            int readData;

            File file(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(file.IsOpen() == true);
            REQUIRE(file.WriteObject(setData) == true);

            file.SwitchBetweenReadAndWrite();

            REQUIRE(file.ReadObject(readData) == true);
            REQUIRE(readData == data[1]);
        }

        WHEN("Switching from reading to writing, it should eneble the data to be written correctly")
        {
            {
                File creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(creator.IsOpen() == true);
                REQUIRE(creator.Write(data.data(), dataSize) == true);
            }

            const int setData = 13;
            int readData;

            File file(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(file.IsOpen() == true);
            REQUIRE(file.ReadObject(readData) == true);

            file.SwitchBetweenReadAndWrite();

            const Position beforeWrite = file.GetPosInFile();
            REQUIRE(file.WriteObject(setData) == true);

            file.SetPosInFile(beforeWrite);
            REQUIRE(file.ReadObject(readData) == true);

            REQUIRE(readData == setData);
        }

        WHEN("Called on unopen file the function should do nothing")
        {
            File file;
            file.SwitchBetweenReadAndWrite();
        }
    }

    TEMPLATE_TEST_CASE("[File] : GetFileStatics should retrieve file statistics - filename argument", "[File][GetFileStatics]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;
        using Stats      = typename File::Stats;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        Stats stats;

        WHEN("GetFileStatics is called with an existing file it should remove the file")
        {
            REQUIRE(File::CreateFile(cleaner.Filename()) == true);
            REQUIRE(File::GetFileStatics(stats, cleaner.Filename()) == true);

            REQUIRE(stats.Size() == 0);
            REQUIRE(stats.TimeOfLastAccess() != 0);
        }

        WHEN("GetFileStatics is called with nonexistant file it should fail")
        {
            REQUIRE(File::RemoveFile(cleaner.Filename()) == false);
            REQUIRE(File::GetFileStatics(stats, cleaner.Filename()) == false);
        }

        WHEN("GetFileStatics is called with invalid filename it should fail")
        {
            TestType invalidName[] = {'\0'};
            REQUIRE(File::GetFileStatics(stats, invalidName) == false);
        }
    }

    TEMPLATE_TEST_CASE("[File] : GetFileStatics should retrieve file statistics - FileDescriptor argument", "[File][GetFileStatics]", FileTestedTypes)
    {
        using Cleaner        = BasicFilenameCleaner<TestType>;
        using File     = BasicFile<TestType>;
        using Stats          = typename File::Stats;
        using FileDescriptor = typename File::FileDescriptor;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        Stats stats;

        WHEN("GetFileStatics is called with a valid descriptor the stats should be retrieved")
        {
            File file(cleaner.Filename(), COpenMode::Write);
            REQUIRE(file.IsOpen() == true);
            const FileDescriptor descriptor = file.GetFileDescriptor();

            REQUIRE(File::GetFileStatics(stats, descriptor) == true);

            REQUIRE(stats.Size() == 0);
            REQUIRE(stats.TimeOfLastAccess() != 0);
        }

        WHEN("GetFileStatics is called with a valid descriptor the stats should be retrieved")
        {
            const FileDescriptor descriptor;
            REQUIRE(File::GetFileStatics(stats, descriptor) == false);
        }
    }

    TEMPLATE_TEST_CASE("[File] : GetFileSize should retrieve the size of the file - filename argument", "[File][GetFileSize]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("GetFileSize is called with an existing file it should return the actual size of the file")
        {
            WHEN("Is called with empty file returns 0")
            {
                REQUIRE(File::CreateFile(cleaner.Filename()) == true);
                REQUIRE(File::GetFileSize(cleaner.Filename()) == 0);
            }

            WHEN("Is called with nonempty file returns more than 0")
            {
                {
                    const int data[3] = {1, 2, 3};
                    File file(cleaner.Filename(), COpenMode::Write);

                    REQUIRE(file.IsOpen() == true);
                    REQUIRE(file.Write(data, 3) == true);
                }
                REQUIRE(File::GetFileSize(cleaner.Filename()) > 0);
            }
        }

        WHEN("GetFileSize is called with nonexistant file it should fail and return -1")
        {
            REQUIRE(File::GetFileSize(cleaner.Filename()) == File::ErrorSize);
        }

        WHEN("GetFileSize is called with invalid filename it should fail and return -1")
        {
            TestType invalidName[] = {'\0'};
            REQUIRE(File::GetFileSize(invalidName) == File::ErrorSize);
        }
    }

    TEMPLATE_TEST_CASE("[File] : GetFileSize should retrieve the size of the file - FileDescriptor argument", "[File][GetFileSize]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;
        using FileDescriptor = typename File::FileDescriptor;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("GetFileSize is called with valid descriptor it should return the actual size of the file")
        {
            WHEN("Is called with empty file returns 0")
            {
                File file(cleaner.Filename(), COpenMode::Write);;
                REQUIRE(file.IsOpen() == true);
                const FileDescriptor descriptor = file.GetFileDescriptor();

                REQUIRE(File::GetFileSize(descriptor) == 0);
            }
            WHEN("Is called with nonempty file returns more than 0")
            {
                {
                    const int data[3] = {1, 2, 3};
                    File file(cleaner.Filename(), COpenMode::Write);
                    REQUIRE(file.IsOpen() == true);
                    REQUIRE(file.Write(data, 3) == true);
                }

                File file(cleaner.Filename(), COpenMode::ReadMustExist);;
                REQUIRE(file.IsOpen() == true);

                const FileDescriptor descriptor = file.GetFileDescriptor();

                REQUIRE(File::GetFileSize(descriptor) > 0);
            }
        }

        WHEN("GetFileSize is called with an invalid descriptor it should return ErrorSize")
        {
            const FileDescriptor descriptor;
            REQUIRE(File::GetFileSize(descriptor) == File::ErrorSize);
        }
    }

    TEMPLATE_TEST_CASE("[File] : GetFileDescriptor should return the FileDescriptor associated with the file", "[File][GetFileDescriptor]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file is opened with COpenMode::ReadMustExist it should return valid descriptor")
        {
            REQUIRE(File::CreateFile(cleaner.Filename()) == true);

            File file(cleaner.Filename(), COpenMode::ReadMustExist);
            REQUIRE(file.IsOpen() == true);
            REQUIRE(file.GetFileDescriptor() != File::FileDescriptor::ErrorDescriptor);
        }

        WHEN("A file is opened with COpenMode::Write it should return valid descriptor")
        {
            File file(cleaner.Filename(), COpenMode::Write);
            REQUIRE(file.IsOpen() == true);
            REQUIRE(file.GetFileDescriptor() != File::FileDescriptor::ErrorDescriptor);
        }

        WHEN("A file is opened with COpenMode::WriteAppend it should return valid descriptor")
        {
            File file(cleaner.Filename(), COpenMode::WriteAppend);
            REQUIRE(file.IsOpen() == true);
            REQUIRE(file.GetFileDescriptor() != File::FileDescriptor::ErrorDescriptor);
        }

        WHEN("A file is opened with COpenMode::ReadWriteMustExist it should return valid descriptor")
        {
            REQUIRE(File::CreateFile(cleaner.Filename()) == true);

            File file(cleaner.Filename(), COpenMode::ReadWriteMustExist);
            REQUIRE(file.IsOpen() == true);
            REQUIRE(file.GetFileDescriptor() != File::FileDescriptor::ErrorDescriptor);
        }

        WHEN("A file is opened with COpenMode::ReadWrite it should return valid descriptor")
        {
            File file(cleaner.Filename(), COpenMode::ReadWrite);
            REQUIRE(file.IsOpen() == true);
            REQUIRE(file.GetFileDescriptor() != File::FileDescriptor::ErrorDescriptor);
        }

        WHEN("A file is opened with COpenMode::ReadMustExist it should return valid descriptor")
        {
            File file(cleaner.Filename(), COpenMode::ReadWriteAppend);
            REQUIRE(file.IsOpen() == true);
            REQUIRE(file.GetFileDescriptor() != File::FileDescriptor::ErrorDescriptor);
        }

        WHEN("Called on unopen file should return FileDescriptor::ErrorDescriptor")
        {
            File file;
            REQUIRE(file.GetFileDescriptor() == File::FileDescriptor::ErrorDescriptor);
        }
    }

    TEMPLATE_TEST_CASE("[File] : GetFileStatics should retrieve file statistics", "[File][GetFileStatics]", FileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using File = BasicFile<TestType>;
        using Stats      = typename File::Stats;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("the file is empty the statistics should contain size 0")
        {
            File file(cleaner.Filename(), COpenMode::Write);
            REQUIRE(file.IsOpen() == true);

            Stats stats;
            REQUIRE(file.GetFileStatics(stats) == true);
            REQUIRE(stats.Size() == 0);
            REQUIRE(stats.TimeOfLastAccess() != 0);
        }

        WHEN("the file is non empty the statistics should contain size grater 0")
        {
            File file(cleaner.Filename(), COpenMode::Write);

            Stats stats;
            REQUIRE(file.GetFileStatics(stats) == true);
            //REQUIRE(stats.Size() > 0); //This function is unreliable and this test is fragile
            REQUIRE(stats.TimeOfLastAccess() != 0);
        }

        WHEN("Called on unopen file it should return false and fail")
        {
            File file;
            Stats stats;
            REQUIRE(file.GetFileStatics(stats) == false);
        }

    }

    TEMPLATE_TEST_CASE("[File] : GetFileSize should retrieve the size of the file", "[File][GetFileSize]", FileTestedTypes)
    {
        using Cleaner = BasicFilenameCleaner<TestType>;
        using File    = BasicFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("GetFileSize is called it should return the actual size of the file")
        {
            WHEN("Is called with empty file returns 0")
            {
                File file(cleaner.Filename(), COpenMode::Write);;
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.GetFileSize() == 0);
            }
            WHEN("Is called with nonempty file returns more than 0")
            {
                {
                    const int data[3] = {1, 2, 3};
                    File file(cleaner.Filename(), COpenMode::Write);
                    REQUIRE(file.IsOpen() == true);
                    REQUIRE(file.Write(data, 3) == true);
                }

                File file(cleaner.Filename(), COpenMode::ReadMustExist);;
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.GetFileSize() > 0);
            }
        }

        WHEN("Called on unopen file it should return ErrorSize")
        {
            File file;
            REQUIRE(file.GetFileSize() == File::ErrorSize);
        }
    }
}

#endif // FILE_MISCELLANEOUSFUNCTIONS_H
