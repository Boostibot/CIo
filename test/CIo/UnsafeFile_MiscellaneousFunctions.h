#ifndef UNSAFEFILE_MISCELLANEOUSFUNCTIONS_H
#define UNSAFEFILE_MISCELLANEOUSFUNCTIONS_H

#include "UnsafeFile_TestingUtilities.h"

namespace cio::UnsafeFileTesting
{
    TEMPLATE_TEST_CASE("[UnsafeFile] : SetBuffer should set the buffer", "[UnsafeFile][SetBuffer]", UnsafeFileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("Provided with a external buffer the function should use it and return true")
        {
            constexpr size_t bufferSize = UnsafeFile::DefaultBufferSize;
            std::array<int, bufferSize> buffer;

            WHEN("Provided with buffering code Full it should use it and return true")
            {
                UnsafeFile file(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::Binary);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.SetBuffer(buffer.data(), bufferSize, UnsafeFile::BufferingCode::Full) == true);
            }

            WHEN("Provided with buffering code Line it should use it and return true")
            {
                UnsafeFile file(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::Binary);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.SetBuffer(buffer.data(), bufferSize, UnsafeFile::BufferingCode::Line) == true);
            }

            WHEN("Provided with buffering code None it should not use it and return true")
            {
                UnsafeFile::CreateFile(cleaner.Filename());
                UnsafeFile file(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.SetBuffer(buffer.data(), bufferSize, UnsafeFile::BufferingCode::None) == true);
            }
        }
        WHEN("Provided with nullptr the function should allocate its own buffer and use it")
        {
            WHEN("Provided with buffering code Full it should return true")
            {
                UnsafeFile file(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::Write);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.SetBuffer(nullptr, UnsafeFile::DefaultBufferSize, UnsafeFile::BufferingCode::Full) == true);
            }

            WHEN("Provided with buffering code Line it should return true")
            {
                UnsafeFile file(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.SetBuffer(nullptr, UnsafeFile::DefaultBufferSize, UnsafeFile::BufferingCode::Line) == true);
            }

            WHEN("Provided with buffering code None it should return true")
            {
                UnsafeFile::CreateFile(cleaner.Filename());
                UnsafeFile file(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::Append);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.SetBuffer(nullptr, UnsafeFile::DefaultBufferSize, UnsafeFile::BufferingCode::None) == true);
            }
        }

    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : Flush should flush the buffer", "[UnsafeFile][Flush]", UnsafeFileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr size_t dataSize = 100;
        std::array<int, dataSize> data = {-23, 132029, -346};
        WHEN("Switching from writing to reading, it should eneble the data to be read correctly")
        {
            {
                UnsafeFile creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(creator.IsOpen() == true);
                REQUIRE(creator.Write(data.data(), dataSize) == true);
            }

            const int setData = 13;
            int readData;

            UnsafeFile file(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(file.IsOpen() == true);
            REQUIRE(file.WriteObject(setData) == true);

            file.Flush();

            REQUIRE(file.ReadObject(readData) == true);
            REQUIRE(readData == data[1]);
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : SwitchBetweenReadAndWrite should allow the user to use read after write or vice versa", "[UnsafeFile][SwitchBetweenReadAndWrite]", UnsafeFileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;
        using Position   = typename UnsafeFile::Position;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        constexpr size_t dataSize = 100;
        std::array<int, dataSize> data = {-23, 132029, -346};

        WHEN("Switching from writing to reading, it should eneble the data to be read correctly")
        {
            {
                UnsafeFile creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(creator.IsOpen() == true);
                REQUIRE(creator.Write(data.data(), dataSize) == true);
            }

            const int setData = 13;
            int readData;

            UnsafeFile file(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(file.IsOpen() == true);
            REQUIRE(file.WriteObject(setData) == true);

            file.SwitchBetweenReadAndWrite();

            REQUIRE(file.ReadObject(readData) == true);
            REQUIRE(readData == data[1]);
        }

        WHEN("Switching from reading to writing, it should eneble the data to be written correctly")
        {
            {
                UnsafeFile creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(creator.IsOpen() == true);
                REQUIRE(creator.Write(data.data(), dataSize) == true);
            }

            const int setData = 13;
            int readData;

            UnsafeFile file(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustExist, OpenModeFlag::Binary);
            REQUIRE(file.IsOpen() == true);
            REQUIRE(file.ReadObject(readData) == true);

            file.SwitchBetweenReadAndWrite();

            const Position beforeWrite = file.GetPosInFile();
            REQUIRE(file.WriteObject(setData) == true);

            file.SetPosInFile(beforeWrite);
            REQUIRE(file.ReadObject(readData) == true);

            REQUIRE(readData == setData);
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : GetUniqueTempFileName should create a unique filename", "[UnsafeFile][GetUniqueTempFileName]", UnsafeFileTestedTypes)
    {
        using UnsafeFile = BasicUnsafeFile<TestType>;
        using CStringRef = typename UnsafeFile::OsCStringRef;
        using StringView = typename UnsafeFile::OsStringView;

        constexpr i32 filenameSize = UnsafeFile::TempFileNameMaxLenght;


        WHEN("is called with valid sized ref should succeed")
        {
            TestType filename[filenameSize];

            REQUIRE(UnsafeFile::GetUniqueTempFileName(CStringRef(filename, filenameSize)) == true);

            const StringView view = filename;

            REQUIRE(view.empty() == false);
        }
        WHEN("is called more than once should create different names")
        {
            TestType filename1[filenameSize];
            TestType filename2[filenameSize];
            TestType filename3[filenameSize];

            REQUIRE(UnsafeFile::GetUniqueTempFileName(CStringRef(filename1, filenameSize)) == true);
            REQUIRE(UnsafeFile::GetUniqueTempFileName(CStringRef(filename2, filenameSize)) == true);
            REQUIRE(UnsafeFile::GetUniqueTempFileName(CStringRef(filename3, filenameSize)) == true);

            const StringView view1 = filename1;
            const StringView view2 = filename2;
            const StringView view3 = filename3;

            REQUIRE(view1 != view2);
            REQUIRE(view2 != view3);
            REQUIRE(view3 != view1);
        }

        WHEN("is called with ref of unsufficient size should fail and return false")
        {
            TestType filename[filenameSize];
            REQUIRE(UnsafeFile::GetUniqueTempFileName(CStringRef(filename, 0)) == false);
            REQUIRE(UnsafeFile::GetUniqueTempFileName(CStringRef(filename, 1)) == false);
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : RenameFile should rename already existing file", "[UnsafeFile][RenameFile]", UnsafeFileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;

        const Cleaner oldName;
        const Cleaner newName;
        REQUIRE(oldName.IsOk() == true);
        REQUIRE(newName.IsOk() == true);

        GIVEN("An existing file")
        {
            WHEN("Renaming a valid file, it should be renamed")
            {
                REQUIRE(UnsafeFile::CreateFile(oldName.Filename()) == true);

                REQUIRE(UnsafeFile::RenameFile(oldName.Filename(), newName.Filename()) == true);

                REQUIRE(UnsafeFile::IsFileOpenable(oldName.Filename()) == false);
                REQUIRE(UnsafeFile::IsFileOpenable(newName.Filename()) == true);
            }
            WHEN("Renaming a valid file to a filename that already exists, it should fail and leave both unchanged")
            {
                REQUIRE(UnsafeFile::CreateFile(oldName.Filename()) == true);
                REQUIRE(UnsafeFile::CreateFile(newName.Filename()) == true);

                REQUIRE(UnsafeFile::RenameFile(oldName.Filename(), newName.Filename()) == false);

                REQUIRE(UnsafeFile::IsFileOpenable(oldName.Filename()) == true);
                REQUIRE(UnsafeFile::IsFileOpenable(newName.Filename()) == true);
            }
        }
        GIVEN("A nonexistant file")
        {
            WHEN("Renaming a nonexistant file to valid filename, it should fail")
            {
                REQUIRE(UnsafeFile::RenameFile(oldName.Filename(), newName.Filename()) == false);

                REQUIRE(UnsafeFile::IsFileOpenable(oldName.Filename()) == false);
                REQUIRE(UnsafeFile::IsFileOpenable(newName.Filename()) == false);
            }
            WHEN("Renaming a nonexistant file to existing file, it should fail fail")
            {
                REQUIRE(UnsafeFile::CreateFile(newName.Filename()) == true);
                REQUIRE(UnsafeFile::RenameFile(oldName.Filename(), newName.Filename()) == false);

                REQUIRE(UnsafeFile::IsFileOpenable(oldName.Filename()) == false);
                REQUIRE(UnsafeFile::IsFileOpenable(newName.Filename()) == true);
            }
        }
        GIVEN("A invalid filenames")
        {
            TestType invalidName1[] = {'\0'};
            TestType invalidName2[] = {'\0'};
            WHEN("Renaming a invalid filename to invalid filename, it should fail")
            {
                REQUIRE(UnsafeFile::RenameFile(invalidName1, invalidName2) == false);

                REQUIRE(UnsafeFile::IsFileOpenable(invalidName1) == false);
                REQUIRE(UnsafeFile::IsFileOpenable(invalidName2) == false);
            }
            WHEN("Renaming a valid filename to invalid filename, it should fail")
            {
                REQUIRE(UnsafeFile::CreateFile(oldName.Filename()) == true);
                REQUIRE(UnsafeFile::RenameFile(oldName.Filename(), invalidName2) == false);

                REQUIRE(UnsafeFile::IsFileOpenable(oldName.Filename()) == true);
                REQUIRE(UnsafeFile::IsFileOpenable(invalidName2) == false);
            }
            WHEN("Renaming a invalid filename to valid filename, it should fail")
            {
                REQUIRE(UnsafeFile::CreateFile(newName.Filename()) == true);
                REQUIRE(UnsafeFile::RenameFile(invalidName1, newName.Filename()) == false);

                REQUIRE(UnsafeFile::IsFileOpenable(invalidName1) == false);
                REQUIRE(UnsafeFile::IsFileOpenable(newName.Filename()) == true);
            }
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : RemoveFile should remove already existing file", "[UnsafeFile][RemoveFile]", UnsafeFileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("Removing an existing file it should remove the file")
        {
            REQUIRE(UnsafeFile::CreateFile(cleaner.Filename()) == true);
            REQUIRE(UnsafeFile::RemoveFile(cleaner.Filename()) == true);
            REQUIRE(UnsafeFile::IsFileOpenable(cleaner.Filename()) == false);
        }

        WHEN("Removing nonexistant file it should fail")
        {
            REQUIRE(UnsafeFile::RemoveFile(cleaner.Filename()) == false);
            REQUIRE(UnsafeFile::IsFileOpenable(cleaner.Filename()) == false);
        }

        WHEN("Removing invalid filename it should fail")
        {
            TestType invalidName[] = {'\0'};
            REQUIRE(UnsafeFile::RemoveFile(invalidName) == false);
            REQUIRE(UnsafeFile::IsFileOpenable(invalidName) == false);
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : GetFileStatics should retrieve file statistics - filename argument", "[UnsafeFile][GetFileStatics][", UnsafeFileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;
        using Stats      = typename UnsafeFile::Stats;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        Stats stats;

        WHEN("GetFileStatics is called with an existing file it should remove the file")
        {
            REQUIRE(UnsafeFile::CreateFile(cleaner.Filename()) == true);
            REQUIRE(UnsafeFile::GetFileStatics(stats, cleaner.Filename()) == true);

            REQUIRE(stats.Size() == 0);
            REQUIRE(stats.TimeOfLastAccess() != 0);
        }

        WHEN("GetFileStatics is called with nonexistant file it should fail")
        {
            REQUIRE(UnsafeFile::RemoveFile(cleaner.Filename()) == false);
            REQUIRE(UnsafeFile::GetFileStatics(stats, cleaner.Filename()) == false);
        }

        WHEN("GetFileStatics is called with invalid filename it should fail")
        {
            TestType invalidName[] = {'\0'};
            REQUIRE(UnsafeFile::GetFileStatics(stats, invalidName) == false);
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : GetFileStatics should retrieve file statistics - FileDescriptor argument", "[UnsafeFile][GetFileStatics]", UnsafeFileTestedTypes)
    {
        using Cleaner        = BasicFilenameCleaner<TestType>;
        using UnsafeFile     = BasicUnsafeFile<TestType>;
        using Stats          = typename UnsafeFile::Stats;
        using FileDescriptor = typename UnsafeFile::FileDescriptor;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        Stats stats;

        WHEN("GetFileStatics is called with a valid descriptor the stats should be retrieved")
        {
            UnsafeFile file(cleaner.Filename(), COpenMode::Write);;
            REQUIRE(file.IsOpen() == true);
            const FileDescriptor descriptor = file.GetFileDescriptor();

            REQUIRE(UnsafeFile::GetFileStatics(stats, descriptor) == true);

            REQUIRE(stats.Size() == 0);
            REQUIRE(stats.TimeOfLastAccess() != 0);
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : GetFileSize should retrieve the size of the file - filename argument", "[UnsafeFile][GetFileSize]", UnsafeFileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("GetFileSize is called with an existing file it should return the actual size of the file")
        {
            WHEN("Is called with empty file returns 0")
            {
                REQUIRE(UnsafeFile::CreateFile(cleaner.Filename()) == true);
                REQUIRE(UnsafeFile::GetFileSize(cleaner.Filename()) == 0);
            }

            WHEN("Is called with nonempty file returns more than 0")
            {
                {
                    const int data[3] = {1, 2, 3};
                    UnsafeFile file(cleaner.Filename(), COpenMode::Write);

                    REQUIRE(file.IsOpen() == true);
                    REQUIRE(file.Write(data, 3) == true);
                }
                REQUIRE(UnsafeFile::GetFileSize(cleaner.Filename()) > 0);
            }
        }

        WHEN("GetFileSize is called with nonexistant file it should fail and return -1")
        {
            REQUIRE(UnsafeFile::GetFileSize(cleaner.Filename()) == UnsafeFile::ErrorSize);
        }

        WHEN("GetFileSize is called with invalid filename it should fail and return -1")
        {
            TestType invalidName[] = {'\0'};
            REQUIRE(UnsafeFile::GetFileSize(invalidName) == UnsafeFile::ErrorSize);
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : GetFileSize should retrieve the size of the file - FileDescriptor argument", "[UnsafeFile][GetFileSize]", UnsafeFileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;
        using FileDescriptor = typename UnsafeFile::FileDescriptor;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("GetFileSize is called with valid descriptor it should return the actual size of the file")
        {
            WHEN("Is called with empty file returns 0")
            {
                UnsafeFile file(cleaner.Filename(), COpenMode::Write);;
                REQUIRE(file.IsOpen() == true);
                const FileDescriptor descriptor = file.GetFileDescriptor();

                REQUIRE(UnsafeFile::GetFileSize(descriptor) == 0);
            }
            WHEN("Is called with nonempty file returns more than 0")
            {
                {
                    const int data[3] = {1, 2, 3};
                    UnsafeFile file(cleaner.Filename(), COpenMode::Write);
                    REQUIRE(file.IsOpen() == true);
                    REQUIRE(file.Write(data, 3) == true);
                }

                UnsafeFile file(cleaner.Filename(), COpenMode::ReadMustExist);;
                REQUIRE(file.IsOpen() == true);

                const FileDescriptor descriptor = file.GetFileDescriptor();

                REQUIRE(UnsafeFile::GetFileSize(descriptor) > 0);
            }
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : GetFileDescriptor should return the FileDescriptor associated with the file", "[UnsafeFile][GetFileDescriptor]", UnsafeFileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file is opened with COpenMode::ReadMustExist it should return valid descriptor")
        {
            REQUIRE(UnsafeFile::CreateFile(cleaner.Filename()) == true);

            UnsafeFile file(cleaner.Filename(), COpenMode::ReadMustExist);
            REQUIRE(file.IsOpen() == true);
            REQUIRE(file.GetFileDescriptor() != UnsafeFile::FileDescriptor::ErrorDescriptor);
        }

        WHEN("A file is opened with COpenMode::Write it should return valid descriptor")
        {
            UnsafeFile file(cleaner.Filename(), COpenMode::Write);
            REQUIRE(file.IsOpen() == true);
            REQUIRE(file.GetFileDescriptor() != UnsafeFile::FileDescriptor::ErrorDescriptor);
        }

        WHEN("A file is opened with COpenMode::WriteAppend it should return valid descriptor")
        {
            UnsafeFile file(cleaner.Filename(), COpenMode::WriteAppend);
            REQUIRE(file.IsOpen() == true);
            REQUIRE(file.GetFileDescriptor() != UnsafeFile::FileDescriptor::ErrorDescriptor);
        }

        WHEN("A file is opened with COpenMode::ReadWriteMustExist it should return valid descriptor")
        {
            REQUIRE(UnsafeFile::CreateFile(cleaner.Filename()) == true);

            UnsafeFile file(cleaner.Filename(), COpenMode::ReadWriteMustExist);
            REQUIRE(file.IsOpen() == true);
            REQUIRE(file.GetFileDescriptor() != UnsafeFile::FileDescriptor::ErrorDescriptor);
        }

        WHEN("A file is opened with COpenMode::ReadWrite it should return valid descriptor")
        {
            UnsafeFile file(cleaner.Filename(), COpenMode::ReadWrite);
            REQUIRE(file.IsOpen() == true);
            REQUIRE(file.GetFileDescriptor() != UnsafeFile::FileDescriptor::ErrorDescriptor);
        }

        WHEN("A file is opened with COpenMode::ReadMustExist it should return valid descriptor")
        {
            UnsafeFile file(cleaner.Filename(), COpenMode::ReadWriteAppend);
            REQUIRE(file.IsOpen() == true);
            REQUIRE(file.GetFileDescriptor() != UnsafeFile::FileDescriptor::ErrorDescriptor);
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : GetFileStatics should retrieve file statistics", "[UnsafeFile][GetFileStatics]", UnsafeFileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;
        using Stats      = typename UnsafeFile::Stats;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("the file is empty the statistics should contain size 0")
        {
            UnsafeFile file(cleaner.Filename(), COpenMode::Write);
            REQUIRE(file.IsOpen() == true);

            Stats stats;
            REQUIRE(file.GetFileStatics(stats) == true);
            REQUIRE(stats.Size() == 0);
            REQUIRE(stats.TimeOfLastAccess() != 0);
        }

        WHEN("the file is non empty the statistics should contain size grater than 0")
        {
            UnsafeFile file(cleaner.Filename(), COpenMode::Write);

            Stats stats;
            REQUIRE(file.GetFileStatics(stats) == true);
            //REQUIRE(stats.Size() > 0); //This function is unreliable and this test is fragile
            REQUIRE(stats.TimeOfLastAccess() != 0);
        }

    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : GetFileSize should retrieve the size of the file", "[UnsafeFile][GetFileSize]", UnsafeFileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("GetFileSize is called it should return the actual size of the file")
        {
            WHEN("Is called with empty file returns 0")
            {
                UnsafeFile file(cleaner.Filename(), COpenMode::Write);;
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.GetFileSize() == 0);
            }
            WHEN("Is called with nonempty file returns more than 0")
            {
                {
                    const int data[3] = {1, 2, 3};
                    UnsafeFile file(cleaner.Filename(), COpenMode::Write);
                    REQUIRE(file.IsOpen() == true);
                    REQUIRE(file.Write(data, 3) == true);
                }

                UnsafeFile file(cleaner.Filename(), COpenMode::ReadMustExist);;
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.GetFileSize() > 0);
            }
        }
    }

}

#endif // UNSAFEFILE_MISCELLANEOUSFUNCTIONS_H
