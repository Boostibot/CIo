#ifndef CFILEMANAGERTEST_H
#define CFILEMANAGERTEST_H

#include "FileManager_TestingUtilities.h"

namespace CIo::FileManagerTesting
{
    TEST_CASE("[BasicFileManager] : Only char8 amd charW (char and wchar_t) specialisations should compile", "[BasicFileManager][Compilation]")
    {
        [[maybe_unused]] BasicFileManager<char8> file8;
        [[maybe_unused]] BasicFileManager<charW> fileW;

        #if FileManager_TEST_COMPILE_ERROR == true
        //Should not compile
        //[[maybe_unused]] BasicFileManager<int> fileInt;
        //[[maybe_unused]] BasicFileManager<double> fileD;
        //[[maybe_unused]] BasicFileManager<short> fileShort;
        //[[maybe_unused]] BasicFileManager<long> fileLing;
        //[[maybe_unused]] BasicFileManager<std::string> fileString;
        //[[maybe_unused]] BasicFileManager<std::wstring> fileWString;
        #endif
    }

    TEMPLATE_TEST_CASE("[BasicFileManager] : Default constructor should construct a closed file", "[BasicFileManager][Default constructor]", FileManagerTestedTypes)
    {
        const BasicFileManager<TestType> file;
        REQUIRE(file.IsOpen() == false);
    }

    TEMPLATE_TEST_CASE("[BasicFileManager] : Constructing a file with filename and OpenMode should open the file", "[BasicFileManager][Constructor]", FileManagerTestedTypes)
    {
        using FileManager = BasicFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        using OpenMode = BasicOpenMode<TestType>;
        constexpr OpenMode defaultOpenMode = defaultModeTmpl<TestType>;

        constexpr OpenMode invalidOpenMode;
        constexpr OpenMode unsupportedOpenMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::MustExist);
        static_assert (invalidOpenMode.IsValid() == false, "!");
        static_assert (unsupportedOpenMode.IsSupported() == false, "!");

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        //Creates a new cleaner that manages the same generated
        // filename and destructs it
        // This leaves us with an unique filename in the first cleaner that is guaranteed to be deleted
        {
            const FilenameCleaner cleanerTemp(cleaner);
        }

        WHEN("A valid filename is passed into the constructor")
        {
            WHEN("provided with COpenMode::ReadMustExist")
            {
                WHEN("The filename does not exist no file is opened")
                {
                    const FileManager manager(cleaner.Filename(), COpenMode::ReadMustExist);
                    REQUIRE(manager.IsOpen() == false);
                }
                WHEN("The filename does exist the file is opened")
                {
                    {
                        const FileManager creator(cleaner.Filename(), COpenMode::Write);
                    }

                    const FileManager manager(cleaner.Filename(), COpenMode::ReadMustExist);
                    REQUIRE(manager.IsOpen() == true);
                }
            }
            WHEN("provided with COpenMode::Write")
            {
                WHEN("is without the MustNotExist a file is always opened")
                {
                    const FileManager manager(cleaner.Filename(), COpenMode::Write);
                    REQUIRE(manager.IsOpen() == true);
                }
                WHEN("is with the MustNotExist a file is always opened only when the file does not exist")
                {
                    WHEN("the file does not exists")
                    {
                        const FileManager manager(cleaner.Filename(), OpenMode(OpenModeFlag::Write, OpenModeFlag::MustNotExist));
                        REQUIRE(manager.IsOpen() == true);
                    }
                    WHEN("the file does exist")
                    {
                        {
                            const FileManager creator(cleaner.Filename(), COpenMode::Write);
                        }
                        const FileManager manager(cleaner.Filename(), OpenMode(OpenModeFlag::Write, OpenModeFlag::MustNotExist));
                        REQUIRE(manager.IsOpen() == false);
                    }
                }
            }
            WHEN("provided with COpenMode::WriteAppend")
            {
                THEN("a file is always opened (given sufficient permission - untested)")
                {
                    const FileManager manager(cleaner.Filename(), COpenMode::WriteAppend);
                    REQUIRE(manager.IsOpen() == true);
                }
            }
            WHEN("provided with COpenMode::ReadWriteMustExist")
            {
                WHEN("The filename does not exist no file is opened")
                {
                    const FileManager manager(cleaner.Filename(), COpenMode::ReadWriteMustExist);
                    REQUIRE(manager.IsOpen() == false);
                }
                WHEN("The filename does exist the file is opened")
                {
                    {
                        const FileManager creator(cleaner.Filename(), COpenMode::Write);
                    }

                    const FileManager manager(cleaner.Filename(), COpenMode::ReadWriteMustExist);
                    REQUIRE(manager.IsOpen() == true);
                }
            }
            WHEN("provided with COpenMode::ReadWrite")
            {
                WHEN("is without the MustNotExist a file is always opened")
                {
                    const FileManager manager(cleaner.Filename(), COpenMode::ReadWrite);
                    REQUIRE(manager.IsOpen() == true);
                }
                WHEN("is with the MustNotExist a file is always opened only when the file does not exist")
                {
                    WHEN("the file does not exists")
                    {
                        const FileManager manager(cleaner.Filename(), OpenMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustNotExist));
                        REQUIRE(manager.IsOpen() == true);
                    }
                    WHEN("the file does exist")
                    {
                        {
                            const FileManager creator(cleaner.Filename(), COpenMode::Write);
                        }
                        const FileManager manager(cleaner.Filename(), OpenMode(OpenModeFlag::Read, OpenModeFlag::Write,  OpenModeFlag::MustNotExist));
                        REQUIRE(manager.IsOpen() == false);
                    }
                }
            }
            WHEN("provided with COpenMode::ReadWriteAppend")
            {

                THEN("a file is always opened (given sufficient permission - untested)")
                {
                    const FileManager manager(cleaner.Filename(), COpenMode::ReadWriteAppend);
                    REQUIRE(manager.IsOpen() == true);
                }
            }

            WHEN("and a invalid openMode is provided no file is opened")
            {
                const FileManager file(cleaner.Filename(), invalidOpenMode);

                REQUIRE(file.IsOpen() == false);
            }

            WHEN("and a unsupported openMode is provided no file is opened")
            {
                const FileManager file(cleaner.Filename(), unsupportedOpenMode);

                REQUIRE(file.IsOpen() == false);
            }
        }

        WHEN("A infvalid filename is passed into the constructor no file is opened")
        {
            typename FileManager::OsStringView filename;

            const FileManager file1(filename, invalidOpenMode);
            const FileManager file2(filename, unsupportedOpenMode);
            const FileManager file3(filename, COpenMode::ReadMustExist);
            const FileManager file4(filename, COpenMode::Write);
            const FileManager file5(filename, COpenMode::WriteAppend);
            const FileManager file6(filename, COpenMode::ReadWriteMustExist);
            const FileManager file7(filename, COpenMode::ReadWrite);
            const FileManager file8(filename, COpenMode::ReadWriteAppend);

            const FileManager file9(filename, OpenMode(OpenModeFlag::Write, OpenModeFlag::MustNotExist));
            const FileManager file10(filename, OpenMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustNotExist));

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);
            REQUIRE(file3.IsOpen() == false);
            REQUIRE(file4.IsOpen() == false);
            REQUIRE(file5.IsOpen() == false);
            REQUIRE(file6.IsOpen() == false);
            REQUIRE(file7.IsOpen() == false);
            REQUIRE(file8.IsOpen() == false);
            REQUIRE(file9.IsOpen() == false);
            REQUIRE(file10.IsOpen() == false);
        }
    }

    TEMPLATE_TEST_CASE("[BasicFileManager] : Constructing a file with filename and OpenModeFlags should open the file", "[BasicFileManager][Constructor]", FileManagerTestedTypes)
    {
        using FileManager = BasicFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        using OpenMode = BasicOpenMode<TestType>;
        constexpr OpenMode defaultOpenMode = defaultModeTmpl<TestType>;

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        //Creates a new cleaner that manages the same generated
        // filename and destructs it
        // This leaves us with an unique filename in the first cleaner that is guaranteed to be deleted
        {
            const FilenameCleaner cleanerTemp(cleaner);
        }

        WHEN("A valid filename is passed into the constructor")
        {
            WHEN("provided with COpenMode::ReadMustExist equivalent")
            {
                WHEN("The filename does not exist no file is opened")
                {
                    const FileManager manager(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::MustExist);
                    REQUIRE(manager.IsOpen() == false);
                }
                WHEN("The filename does exist the file is opened")
                {
                    {
                        const FileManager creator(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary);
                        REQUIRE(creator.IsOpen() == true);
                    }

                    const FileManager manager(cleaner.Filename(), OpenModeFlag::MustExist, OpenModeFlag::Read);
                    REQUIRE(manager.IsOpen() == true);
                }
            }
            WHEN("provided with COpenMode::Write equivalent")
            {
                WHEN("is without the MustNotExist a file is always opened")
                {
                    const FileManager manager(cleaner.Filename(), OpenModeFlag::Write);
                    REQUIRE(manager.IsOpen() == true);
                }
                WHEN("is with the MustNotExist a file is always opened only when the file does not exist")
                {
                    WHEN("the file does not exists")
                    {
                        const FileManager manager(cleaner.Filename(), OpenModeFlag::MustNotExist,  OpenModeFlag::Binary, OpenModeFlag::Write);
                        REQUIRE(manager.IsOpen() == true);
                    }
                    WHEN("the file does exist")
                    {
                        {
                            const FileManager creator(cleaner.Filename(), OpenModeFlag::Write);
                            REQUIRE(creator.IsOpen() == true);
                        }
                        const FileManager manager(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::MustNotExist);
                        REQUIRE(manager.IsOpen() == false);
                    }
                }
            }
            WHEN("provided with COpenMode::WriteAppend equivalent")
            {
                THEN("a file is always opened (given sufficient permission - untested)")
                {
                    const FileManager manager(cleaner.Filename(), OpenModeFlag::Append,  OpenModeFlag::Binary, OpenModeFlag::Write);
                    REQUIRE(manager.IsOpen() == true);
                }
            }
            WHEN("provided with COpenMode::ReadWriteMustExist equivalent")
            {
                WHEN("The filename does not exist no file is opened")
                {
                    const FileManager manager(cleaner.Filename(), OpenModeFlag::MustExist, OpenModeFlag::Read, OpenModeFlag::Write);
                    REQUIRE(manager.IsOpen() == false);
                }
                WHEN("The filename does exist the file is opened")
                {
                    {
                        const FileManager creator(cleaner.Filename(), OpenModeFlag::Write);
                        REQUIRE(creator.IsOpen() == true);
                    }

                    const FileManager manager(cleaner.Filename(), OpenModeFlag::Write,  OpenModeFlag::Binary, OpenModeFlag::MustExist, OpenModeFlag::Read);
                    REQUIRE(manager.IsOpen() == true);
                }
            }
            WHEN("provided with COpenMode::ReadWrite equivalent")
            {
                WHEN("is without the MustNotExist a file is always opened")
                {
                    const FileManager manager(cleaner.Filename(), COpenMode::ReadWrite);
                    REQUIRE(manager.IsOpen() == true);
                }
                WHEN("is with the MustNotExist a file is always opened only when the file does not exist")
                {
                    WHEN("the file does not exists")
                    {
                        const FileManager manager(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::MustNotExist, OpenModeFlag::Read);
                        REQUIRE(manager.IsOpen() == true);
                    }
                    WHEN("the file does exist")
                    {
                        {
                            const FileManager creator(cleaner.Filename(), OpenModeFlag::Write);
                            REQUIRE(creator.IsOpen() == true);
                        }
                        const FileManager manager(cleaner.Filename(), OpenModeFlag::Read,  OpenModeFlag::MustNotExist, OpenModeFlag::Write, OpenModeFlag::Binary);
                        REQUIRE(manager.IsOpen() == false);
                    }
                }
            }
            WHEN("provided with COpenMode::ReadWriteAppend equivalent")
            {

                THEN("a file is always opened (given sufficient permission - untested)")
                {
                    const FileManager manager(cleaner.Filename(), OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append);
                    REQUIRE(manager.IsOpen() == true);
                }
            }

            WHEN("and a invalid openMode is provided no file is opened")
            {
                const FileManager file(cleaner.Filename());
                REQUIRE(file.IsOpen() == false);
                const FileManager file2(cleaner.Filename(), OpenModeFlag::MustExist);
                REQUIRE(file2.IsOpen() == false);
            }

            WHEN("and a unsupported openMode is provided no file is opened")
            {
                const FileManager file(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::MustExist);

                REQUIRE(file.IsOpen() == false);
            }
        }

        WHEN("A infvalid filename is passed into the constructor no file is opened")
        {
            typename FileManager::OsStringView filename;

            const FileManager file1(filename, OpenModeFlag::Binary);
            const FileManager file2(filename, OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::MustExist);
            const FileManager file3(filename, OpenModeFlag::Read, OpenModeFlag::MustExist);
            const FileManager file4(filename, OpenModeFlag::Write);
            const FileManager file5(filename, OpenModeFlag::Write, OpenModeFlag::Append);
            const FileManager file6(filename, OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustExist);
            const FileManager file7(filename, OpenModeFlag::Read, OpenModeFlag::Write);
            const FileManager file8(filename, OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append);

            const FileManager file9(filename, OpenModeFlag::Write, OpenModeFlag::MustNotExist);
            const FileManager file10(filename, OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustNotExist);

            const FileManager file11(filename, OpenModeFlag::Write, OpenModeFlag::Binary);
            const FileManager file12(filename, OpenModeFlag::Binary, OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Append);
            const FileManager file13(filename, OpenModeFlag::MustNotExist, OpenModeFlag::Read, OpenModeFlag::Binary, OpenModeFlag::Write);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);
            REQUIRE(file3.IsOpen() == false);
            REQUIRE(file4.IsOpen() == false);
            REQUIRE(file5.IsOpen() == false);
            REQUIRE(file6.IsOpen() == false);
            REQUIRE(file7.IsOpen() == false);
            REQUIRE(file8.IsOpen() == false);
            REQUIRE(file9.IsOpen() == false);
            REQUIRE(file10.IsOpen() == false);
            REQUIRE(file11.IsOpen() == false);
            REQUIRE(file12.IsOpen() == false);
            REQUIRE(file13.IsOpen() == false);
        }
    }

    TEMPLATE_TEST_CASE("[BasicFileManager] : OpenNew should open a new file independent of if a file is already opened", "[BasicFileManager][OpenNew]", FileManagerTestedTypes)
    {
        using FileManager = BasicFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        constexpr BasicOpenMode<TestType> defaultOpenMode = defaultModeTmpl<TestType>;

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("Passing a complete openmode")
        {
            WHEN("A file is not yet opened OpenNew should leave it opened")
            {
                FileManager file;

                //OpenNew should succeed
                REQUIRE(file.OpenNew(cleaner.Filename(), defaultOpenMode) == true);
            }

            WHEN("A file is opened using constructor OpenNew will keep it opened")
            {
                const FilenameCleaner cleanerNew;
                REQUIRE(cleaner.IsOk() == true);

                {
                    FileManager file(cleaner.Filename(), defaultOpenMode);

                    //OpenNew should succeed
                    REQUIRE(file.OpenNew(cleanerNew.Filename(), defaultOpenMode) == true);
                }
            }

            WHEN("A file is opened using OpenNew, OpenNew will keep it opened")
            {
                const FilenameCleaner cleanerNew;
                REQUIRE(cleaner.IsOk() == true);

                {
                    FileManager file;

                    //OpenNew should succeed
                    REQUIRE(file.OpenNew(cleaner.Filename(), defaultOpenMode) == true);

                    //OpenNew should succeed
                    REQUIRE(file.OpenNew(cleanerNew.Filename(), defaultOpenMode) == true);
                }
            }
        }
        WHEN("Passing individual openmode flags")
        {
            WHEN("A file is not yet opened OpenNew should leave it opened")
            {
                FileManager file;

                //OpenNew should succeed
                REQUIRE(file.OpenNew(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Read, OpenModeFlag::Binary) == true);
            }

            WHEN("A file is opened using constructor OpenNew will keep it opened")
            {
                const FilenameCleaner cleanerNew;
                REQUIRE(cleaner.IsOk() == true);

                {
                    FileManager file(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Append);

                    //OpenNew should succeed
                    REQUIRE(file.OpenNew(cleanerNew.Filename(), OpenModeFlag::Write, OpenModeFlag::Binary) == true);
                }
            }

            WHEN("A file is opened using OpenNew, OpenNew will keep it opened")
            {
                const FilenameCleaner cleanerNew;
                REQUIRE(cleaner.IsOk() == true);

                {
                    FileManager file;

                    //OpenNew should succeed
                    REQUIRE(file.OpenNew(cleaner.Filename(), OpenModeFlag::Write, OpenModeFlag::Read, OpenModeFlag::Append) == true);

                    //OpenNew should succeed
                    REQUIRE(file.OpenNew(cleanerNew.Filename(), OpenModeFlag::Write, OpenModeFlag::Read) == true);
                }
            }
        }
    }

    TEMPLATE_TEST_CASE("[BasicFileManager] : IsOpen and IsClosed should tell if the file is opened or closed", "[BasicFileManager][IsOpen]", FileManagerTestedTypes)
    {
        using FileManager = BasicFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        constexpr BasicOpenMode<TestType> defaultOpenMode = defaultModeTmpl<TestType>;

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file was never opened it should be closed")
        {
            const FileManager file;

            REQUIRE(file.IsOpen() == false);

            REQUIRE(file.IsClosed() == true);

            //Calling the function should not chanage the state
            REQUIRE(file.IsOpen() == false);
        }

        WHEN("A file is opened in the constructor it is opened")
        {
            const FileManager file(cleaner.Filename(), defaultOpenMode);

            //It should be declared open
            REQUIRE(file.IsOpen() == true);

            REQUIRE(file.IsClosed() == false);

            //Calling the function should not chanage the state
            REQUIRE(file.IsOpen() == true);
        }

        WHEN("A file is opened using OpenNew it is opened")
        {
            FileManager file;

            REQUIRE(file.OpenNew(cleaner.Filename(), defaultOpenMode) == true);

            //It should be declared open
            REQUIRE(file.IsOpen() == true);

            REQUIRE(file.IsClosed() == false);
        }
    }

    TEMPLATE_TEST_CASE("[BasicFileManager] : File should not be copy constructible", "[BasicFileManager][Copy constructor]", FileManagerTestedTypes)
    {
        using FileManager = BasicFileManager<TestType>;

        REQUIRE(std::is_copy_constructible_v<FileManager> == false);
    }

    TEMPLATE_TEST_CASE("[BasicFileManager] : Move constructing should transfer the file", "[BasicFileManager][Move constructor]", FileManagerTestedTypes)
    {
        using FileManager = BasicFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        constexpr BasicOpenMode<TestType> defaultOpenMode = defaultModeTmpl<TestType>;

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file is open move constructing from it transfer the file")
        {
            FileManager file1(cleaner.Filename(), defaultOpenMode);

            const FILE PTR before = file1.FilePtr;

            const FileManager file2(std::move(file1));

            REQUIRE(file2.FilePtr == before);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == true);

        }

        WHEN("A file is not open move constructing from it leaves both unopen")
        {
            FileManager file1;

            const FileManager file2(std::move(file1));

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);
        }
    }

    TEMPLATE_TEST_CASE("[BasicFileManager] : File should not be copy assignible", "[BasicFileManager][Copy assignment]", FileManagerTestedTypes)
    {
        using FileManager = BasicFileManager<TestType>;

        REQUIRE(std::is_copy_assignable_v<FileManager> == false);
    }

    TEMPLATE_TEST_CASE("[BasicFileManager] : Move assigning should stransfer the file", "[BasicFileManager][Move assignment]", FileManagerTestedTypes)
    {
        using FileManager = BasicFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        constexpr BasicOpenMode<TestType> defaultOpenMode = defaultModeTmpl<TestType>;

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file is open move assigning from it transfer the file")
        {
            FileManager file1(cleaner.Filename(), defaultOpenMode);
            FileManager file2;

            const FILE PTR before = file1.FilePtr;

            file2 = std::move(file1);

            REQUIRE(file2.FilePtr == before);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == true);
        }

        WHEN("A file is open move assigning to it closed file closes the file")
        {
            FileManager file1(cleaner.Filename(), defaultOpenMode);
            FileManager file2;

            const FILE PTR before = file1.FilePtr;

            file1 = std::move(file2);

            REQUIRE(file2.FilePtr != before);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);
        }

        WHEN("A file is open move assigning to it opened file transfers it")
        {
            const FilenameCleaner cleaner2;
            REQUIRE(cleaner2.IsOk() == true);

            {
                FileManager file1(cleaner.Filename(), defaultOpenMode);
                FileManager file2(cleaner2.Filename(), defaultOpenMode);

                const FILE PTR before = file2.FilePtr;

                file1 = std::move(file2);

                REQUIRE(file1.FilePtr == before);

                REQUIRE(file1.IsOpen() == true);

                REQUIRE(file2.IsOpen() == false);
            }
        }

        WHEN("A file is not open move assigning from it leaves both unopen")
        {
            FileManager file1;
            FileManager file2;

            file2 = std::move(file1);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);
        }

        WHEN("Move assigning a file to itself should not change its state")
        {
            FileManager file1(cleaner.Filename(), defaultOpenMode);
            FileManager file2;

            const FILE PTR before1  = file1.FilePtr;
            const FILE PTR before2  = file2.FilePtr;

            file1  = std::move(file1);
            file2  = std::move(file2);

            REQUIRE(file1.FilePtr  == before1);

            REQUIRE(file2.FilePtr  == before2);

            REQUIRE(file1.IsOpen() == true);
            REQUIRE(file2.IsOpen() == false);
        }
    }

    TEMPLATE_TEST_CASE("[BasicFileManager] : operator bool should tell if the file is opened or closed", "[BasicFileManager]", FileManagerTestedTypes)
    {
        using FileManager = BasicFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        constexpr BasicOpenMode<TestType> defaultOpenMode = defaultModeTmpl<TestType>;

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file is closed bool conversion should be false")
        {
            const FileManager file;

            REQUIRE(file.operator bool() == false);

            //Calling the function should not chanage the state
            REQUIRE(file.operator bool() == false);
        }

        WHEN("A file is opened in the constructor bool conversion should be true")
        {
            const FileManager file(cleaner.Filename(), defaultOpenMode);

            REQUIRE(file.operator bool() == true);

            //Calling the function should not chanage the state
            REQUIRE(file.operator bool() == true);
        }

        WHEN("A file is opened using OpenNew bool conversion should be true")
        {
            FileManager file;

            file.OpenNew(cleaner.Filename(), defaultOpenMode);

            REQUIRE(file.operator bool() == true);
        }
    }

    TEMPLATE_TEST_CASE("[BasicFileManager] : Swap shoudl swap the managed files", "[BasicFileManager][Swap]", FileManagerTestedTypes)
    {
        using FileManager = BasicFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        constexpr BasicOpenMode<TestType> defaultOpenMode = defaultModeTmpl<TestType>;

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("Both swapped file are not opened they should remain unopened")
        {
            FileManager file1;
            FileManager file2;

            file1.Swap(file2);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == false);
        }

        WHEN("One swapped file is opened the \"openness\" should swap")
        {
            FileManager file1(cleaner.Filename(), defaultOpenMode);
            FileManager file2;

            const FILE PTR before = file1.FilePtr;

            file1.Swap(file2);

            //Is this check (pointers) needed?
            REQUIRE(file2.FilePtr == before);

            REQUIRE(file1.IsOpen() == false);
            REQUIRE(file2.IsOpen() == true);
        }

        WHEN("Both files are opened both should remain opened but their file pointers need to swap")
        {
            const FilenameCleaner cleanerNew;
            REQUIRE(cleaner.IsOk() == true);

            {
                FileManager file1(cleaner.Filename(), defaultOpenMode);
                FileManager file2(cleanerNew.Filename(), defaultOpenMode);

                const FILE PTR before1  = file1.FilePtr;
                const FILE PTR before2  = file2.FilePtr;

                file1.Swap(file2);

                REQUIRE(file1.FilePtr  == before2);
                REQUIRE(file2.FilePtr  == before1);

                REQUIRE(file1.IsOpen() == true);
                REQUIRE(file2.IsOpen() == true);
            }
        }

    }

    TEMPLATE_TEST_CASE("[BasicFileManager] : Close should close the file", "[BasicFileManager][Close]", FileManagerTestedTypes)
    {
        using FileManager = BasicFileManager<TestType>;
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        constexpr BasicOpenMode<TestType> defaultOpenMode = defaultModeTmpl<TestType>;

        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);

        WHEN("A file is not opened closing it should not change its state")
        {
            FileManager file;

            //The function should not fail
            REQUIRE(file.Close() == true);

            REQUIRE(file.IsOpen() == false);
        }

        //TODO - add a section for all COpenModes
        //     - add a section for closing two times the same file
        WHEN("A file is opened closing it should leave it closed")
        {
            FileManager file(cleaner.Filename(), defaultOpenMode);

            //The function should not fail
            REQUIRE(file.Close() == true);

            REQUIRE(file.IsOpen() == false);
        }
    }

}

#endif // CFILEMANAGERTEST_H
