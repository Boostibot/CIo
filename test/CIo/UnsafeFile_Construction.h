#ifndef UNSAFEFILE_CONSTRUCTION_H
#define UNSAFEFILE_CONSTRUCTION_H

#include "UnsafeFile_TestingUtilities.h"

namespace CIo::UnsafeFileTesting
{
    TEST_CASE("[UnsafeFile] : Only char and wchar_t specialisations should compile", "[UnsafeFile]")
    {
        [[maybe_unused]] BasicUnsafeFile<char8> file8;
        [[maybe_unused]] BasicUnsafeFile<charW> fileW;

        #if UnsafeFile_TEST_COMPILE_ERROR == true
        //Should not compile
        [[maybe_unused]] BasicUnsafeFile<int> fileInt;
        [[maybe_unused]] BasicUnsafeFile<double> fileDouble;
        [[maybe_unused]] BasicUnsafeFile<short> fileShort;
        [[maybe_unused]] BasicUnsafeFile<long> fileLing;
        [[maybe_unused]] BasicUnsafeFile<std::string> fileString;
        [[maybe_unused]] BasicUnsafeFile<std::wstring> fileWString;
        #endif
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : Default constructor should construct a closed file", "[UnsafeFile][Default constructor]", UnsafeFileTestedTypes)
    {
        using UnsafeFile = BasicUnsafeFile<TestType>;

        const UnsafeFile file;
        REQUIRE(file.IsClosed() == true);
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : Constructing a file with filename and OpenModeFlags should open the file", "[UnsafeFile][Constructor]", UnsafeFileTestedTypes)
    {
        using UnsafeFile = BasicUnsafeFile<TestType>;
        using Cleaner   = BasicFilenameCleaner<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        //Creates a new cleaner that manages the same generated
        // filename and destructs it
        // This leaves us with an unique filename in the first cleaner that is guaranteed to be deleted
        {
            const Cleaner cleanerTemp(cleaner);
        }

        WHEN("A valid filename is passed into the constructor")
        {
            WHEN("provided with COpenMode::ReadMustExist equivalent")
            {
                const UnsafeFile file(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist);
                REQUIRE(file.IsOpen() == false);
            }
            WHEN("provided with COpenMode::Write equivalent")
            {
                const UnsafeFile file(cleaner.Filename(), OpenModeFlag::Write);
                REQUIRE(file.IsOpen() == true);
            }
            WHEN("provided with COpenMode::WriteAppend equivalent")
            {
                const UnsafeFile file(cleaner.Filename(), OpenModeFlag::Append,  OpenModeFlag::Binary, OpenModeFlag::Write);
                REQUIRE(file.IsOpen() == true);
            }
            WHEN("provided with COpenMode::ReadWriteMustExist equivalent")
            {
                REQUIRE(UnsafeFile::CreateFile(cleaner.Filename()) == true);

                const UnsafeFile file(cleaner.Filename(), OpenModeFlag::Write,  OpenModeFlag::Binary, OpenModeFlag::MustExist, OpenModeFlag::Read);
                REQUIRE(file.IsOpen() == true);
            }
            WHEN("provided with COpenMode::ReadWrite equivalent")
            {
                REQUIRE(UnsafeFile::CreateFile(cleaner.Filename()) == true);

                const UnsafeFile file(cleaner.Filename(), OpenModeFlag::Read,  OpenModeFlag::MustNotExist, OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(file.IsOpen() == false);
            }
            WHEN("provided with COpenMode::ReadWriteAppend equivalent")
            {
                const UnsafeFile file(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append);
                REQUIRE(file.IsOpen() == true);
            }

            WHEN("and a invalid openMode is provided no file is opened")
            {
                const UnsafeFile file(cleaner.Filename());
                REQUIRE(file.IsOpen() == false);
                const UnsafeFile file2(cleaner.Filename(), OpenModeFlag::Append);
                REQUIRE(file2.IsOpen() == false);
            }

            WHEN("and a unsupported openMode is provided no file is opened")
            {
                const UnsafeFile file(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::MustExist);
                REQUIRE(file.IsOpen() == false);
            }
        }

        WHEN("A infvalid filename is passed into the constructor no file is opened")
        {
            typename UnsafeFile::OsStringView filename;

            const UnsafeFile file1(filename, OpenModeFlag::MustExist);
            const UnsafeFile file2(filename, OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::MustExist);
            const UnsafeFile file3(filename, OpenModeFlag::Read, OpenModeFlag::MustExist);
            const UnsafeFile file4(filename, OpenModeFlag::Write);
            const UnsafeFile file5(filename, OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append);
            const UnsafeFile file6(filename, OpenModeFlag::Write, OpenModeFlag::MustNotExist);
            const UnsafeFile file7(filename, OpenModeFlag::Binary, OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);
            REQUIRE(file3.IsOpen() == false);
            REQUIRE(file4.IsOpen() == false);
            REQUIRE(file5.IsOpen() == false);
            REQUIRE(file6.IsOpen() == false);
            REQUIRE(file7.IsOpen() == false);
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : Constructing a file with filename and OpenMode should open the file", "[UnsafeFile][Constructor]", UnsafeFileTestedTypes)
    {
        using UnsafeFile = BasicUnsafeFile<TestType>;
        using Cleaner   = BasicFilenameCleaner<TestType>;
        using OpenMode   = BasicOpenMode<TestType>;

        constexpr OpenMode invalidOpenMode;
        constexpr OpenMode unsupportedOpenMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::MustExist);
        static_assert (invalidOpenMode.IsValid() == false, "!");
        static_assert (unsupportedOpenMode.IsSupported() == false, "!");

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        //Creates a new cleaner that manages the same generated
        // filename and destructs it
        // This leaves us with an unique filename in the first cleaner that is guaranteed to be deleted
        {
            const Cleaner cleanerTemp(cleaner);
        }

        WHEN("A valid filename is passed into the constructor")
        {
            WHEN("provided with COpenMode::ReadMustExist equivalent")
            {
                REQUIRE(UnsafeFile::CreateFile(cleaner.Filename()) == true);

                const UnsafeFile file(cleaner.Filename(), OpenMode(OpenModeFlag::MustExist, OpenModeFlag::Read));
                REQUIRE(file.IsOpen() == true);
            }
            WHEN("provided with COpenMode::Write equivalent")
            {
                const UnsafeFile file(cleaner.Filename(), OpenMode(OpenModeFlag::MustNotExist,  OpenModeFlag::Binary, OpenModeFlag::Write));
                REQUIRE(file.IsOpen() == true);
            }
            WHEN("provided with COpenMode::WriteAppend equivalent")
            {
                const UnsafeFile file(cleaner.Filename(), OpenMode(OpenModeFlag::Append, OpenModeFlag::Write));
                REQUIRE(file.IsOpen() == true);
            }
            WHEN("provided with COpenMode::ReadWriteMustExist equivalent")
            {
                const UnsafeFile file(cleaner.Filename(), OpenMode(OpenModeFlag::MustExist, OpenModeFlag::Read, OpenModeFlag::Write));
                REQUIRE(file.IsOpen() == false);
            }
            WHEN("provided with COpenMode::ReadWrite equivalent")
            {
                const UnsafeFile file(cleaner.Filename(), OpenMode(COpenMode::ReadWrite));
                REQUIRE(file.IsOpen() == true);
            }
            WHEN("provided with COpenMode::ReadWriteAppend equivalent")
            {
                const UnsafeFile file(cleaner.Filename(), OpenMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Binary, OpenModeFlag::Append));
                REQUIRE(file.IsOpen() == true);
            }

            WHEN("and a invalid openMode is provided no file is opened")
            {
                const UnsafeFile file(cleaner.Filename(), invalidOpenMode);

                REQUIRE(file.IsOpen() == false);
            }

            WHEN("and a unsupported openMode is provided no file is opened")
            {
                const UnsafeFile file(cleaner.Filename(), unsupportedOpenMode);

                REQUIRE(file.IsOpen() == false);
            }
        }

        WHEN("A infvalid filename is passed into the constructor no file is opened")
        {
            typename UnsafeFile::OsStringView filename;

            const UnsafeFile file1(filename, invalidOpenMode);
            const UnsafeFile file2(filename, unsupportedOpenMode);
            const UnsafeFile file3(filename, OpenMode(OpenModeFlag::Write, OpenModeFlag::Append));
            const UnsafeFile file4(filename, OpenMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustExist));
            const UnsafeFile file5(filename, OpenMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Binary));
            const UnsafeFile file6(filename, OpenMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustNotExist));
            const UnsafeFile file7(filename, OpenMode(OpenModeFlag::MustNotExist, OpenModeFlag::Read, OpenModeFlag::Binary, OpenModeFlag::Write));

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);
            REQUIRE(file3.IsOpen() == false);
            REQUIRE(file4.IsOpen() == false);
            REQUIRE(file5.IsOpen() == false);
            REQUIRE(file6.IsOpen() == false);
            REQUIRE(file7.IsOpen() == false);
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : File should not be copy constructible", "[UnsafeFile][Copy constructor]", UnsafeFileTestedTypes)
    {
        using UnsafeFile = BasicUnsafeFile<TestType>;

        REQUIRE(std::is_copy_constructible_v<UnsafeFile> == false);
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : Move constructor should transfer the file", "[UnsafeFile][Move constructor]", UnsafeFileTestedTypes)
    {
        using UnsafeFile = BasicUnsafeFile<TestType>;
        using Cleaner   = BasicFilenameCleaner<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("Move constructing a opened file, the file should transfer")
        {
            UnsafeFile file(cleaner.Filename(), COpenMode::Write);
            const UnsafeFile constructed = std::move(file);

            REQUIRE(file.IsOpen() == false);
            REQUIRE(constructed.IsOpen() == true);
        }

        WHEN("Move constructing a closed file, both files should be closed")
        {
            UnsafeFile file;
            const UnsafeFile constructed = std::move(file);

            REQUIRE(file.IsOpen() == false);
            REQUIRE(constructed.IsOpen() == false);
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : File should not be copy assignable", "[UnsafeFile][Copy assignment]", UnsafeFileTestedTypes)
    {
        using UnsafeFile = BasicUnsafeFile<TestType>;

        REQUIRE(std::is_copy_assignable_v<UnsafeFile> == false);
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : Move assigning should transfer the file", "[UnsafeFile][Move assignment]", UnsafeFileTestedTypes)
    {
        using UnsafeFile = BasicUnsafeFile<TestType>;
        using Cleaner   = BasicFilenameCleaner<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file is open move assigning from it transfer the file")
        {
            UnsafeFile file(cleaner.Filename(), COpenMode::Write);
            UnsafeFile assigned;

            assigned = std::move(file);

            REQUIRE(file.IsOpen() == false);
            REQUIRE(assigned.IsOpen() == true);
        }

        WHEN("A file is open move assigning to it closed file closes the file")
        {
            UnsafeFile file(cleaner.Filename(), COpenMode::Write);
            UnsafeFile assigned;

            file = std::move(assigned);


            REQUIRE(file.IsOpen() == false);
            REQUIRE(assigned.IsOpen() == false);
        }

        WHEN("A file is open move assigning to it opened file transfers it")
        {
            const Cleaner cleaner2;
            REQUIRE(cleaner2.IsOk() == true);

            {
                UnsafeFile file(cleaner.Filename(), COpenMode::Write);
                UnsafeFile assigned(cleaner2.Filename(), COpenMode::Write);

                file = std::move(assigned);

                REQUIRE(file.IsOpen() == true);
                REQUIRE(assigned.IsOpen() == false);
            }
        }

        WHEN("A file is not open move assigning from it leaves both unopen")
        {
            UnsafeFile file;
            UnsafeFile assigned;

            assigned = std::move(file);

            REQUIRE(file.IsOpen() == false);
            REQUIRE(assigned.IsOpen() == false);
        }

        WHEN("Move assigning a file to itself should not change its state")
        {
            UnsafeFile file1(cleaner.Filename(), COpenMode::Write);
            UnsafeFile file2;

            file1  = std::move(file1);
            file2  = std::move(file2);

            REQUIRE(file1.IsOpen() == true);
            REQUIRE(file2.IsOpen() == false);
        }
    }

    TEMPLATE_TEST_CASE("[UnsafeFile] : CreateFile shoudl create an empty file", "[UnsafeFile][CreateFile]", UnsafeFileTestedTypes)
    {
        using Cleaner    = BasicFilenameCleaner<TestType>;
        using UnsafeFile = BasicUnsafeFile<TestType>;

        const Cleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("Creating a new file, it should return true and create it")
        {
            REQUIRE(UnsafeFile::CreateFile(cleaner.Filename()) == true);
            REQUIRE(UnsafeFile::IsFileOpenable(cleaner.Filename()) == true);
        }

        WHEN("Creating a file tha already exists, it should return true and leave the file unchanged")
        {
            constexpr size_t dataSize = 10;
            const std::array<double, dataSize> data = {21.2, -0.32, 67.666663};
            std::array<double, dataSize> read;

            {
                UnsafeFile writer(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                REQUIRE(writer.IsOpen() == true);

                REQUIRE(writer.Write(data.data(), dataSize) == true);
            }

            //reader needs to be closed before using CreateFile with the opened file
            {
                UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.Read(read.data(), dataSize) == true);
                REQUIRE(read == data);
            }

            REQUIRE(UnsafeFile::CreateFile(cleaner.Filename()) == true);

            {
                UnsafeFile reader(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
                REQUIRE(reader.Read(read.data(), dataSize) == true);
                REQUIRE(read == data);
            }
        }

        WHEN("Creating a file with invalid filename, it should return false")
        {
            const std::basic_string_view<TestType> filename;
            REQUIRE(UnsafeFile::CreateFile(filename) == false);
        }
    }

}

#endif // UNSAFEFILE_CONSTRUCTION_H
