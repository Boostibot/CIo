#ifndef NEWUNSAFEFILE_H
#define NEWUNSAFEFILE_H

#include "Catch2/Catch.hpp"
#include "src/UnsafeFile.h"
#include "MockAdapter.h"

namespace cio::TestingUtilities
{
    class MockFileManager
    {
        public:
            using OsCharType    = char8;
            using ThisType      = MockFileManager;

        private:
            using RequiredActions = typename OpenMode::RequiredActions;

        public:
            using OpenMode      = BasicOpenMode<OsCharType>;
            using OsString      = String<OsCharType>;
            using OsStringView  = StringView<OsCharType>;

            struct StateStruct
            {
                    bool IsOpen = false;
                    CFile * File = nullptr;
                    EnabledOperations Enabled = {EnabledOperations::Closed};
            };

            StateStruct State;

            struct DefConstructorStruct
            {
                    i32 called = 0;
            };
            struct DestructorStruct
            {
                    i32 called = 0;
            };
            struct ConstructorStruct
            {
                    OsStringView filename;
                    OsStringView mode;
                    i32 called = 0;
            };
            struct MoveConstructorStruct
            {
                    ThisType * other = nullptr;
                    i32 called = 0;
            };
            struct MoveAssignmentStruct
            {
                    ThisType * other = nullptr;
                    i32 called = 0;
            };
            struct GetCFileStruct
            {
                    bool override = false;
                    i32 called = 0;
                    CFile * returnVal = nullptr;
            };
            struct GetEnabledStruct
            {
                    bool override = false;
                    i32 called = 0;
                    EnabledOperations returnVal = {EnabledOperations::Closed};
            };
            struct SwapStruct
            {
                    ThisType * other = nullptr;
                    i32 called = 0;
            };
            struct OpenNewStruct
            {
                    OsString filename;
                    OsString mode;
                    bool simulateReadOnly = false;
                    i32 called = 0;
                    bool returnVal = true;
            };
            struct IsOpenStruct
            {
                    i32 called = 0;
                    bool returnVal = true;
            };
            struct IsClosedStruct
            {
                    i32 called = 0;
                    bool returnVal = true;
            };
            struct CloseStruct
            {
                    i32 called = 0;
                    bool returnVal = true;
            };

            static DefConstructorStruct     DefConstructorTest;
            static DestructorStruct         DestructorTest;
            static ConstructorStruct        ConstructorTest;
            static MoveConstructorStruct    MoveConstructorTest;
            static MoveAssignmentStruct     MoveAssignmentTest;
            static GetCFileStruct           GetCFileTest;
            static GetEnabledStruct         GetEnabledTest;
            static SwapStruct               SwapTest;
            static OpenNewStruct            OpenNewTest;
            static IsOpenStruct             IsOpenTest;
            static IsClosedStruct           IsClosedTest;
            static CloseStruct              CloseTest;

        public:
            MockFileManager() noexcept
            {
                DefConstructorTest.called++;
            }

            MockFileManager(const OsStringView REF path, const OsStringView REF openMode) noexcept
            {
                ConstructorTest.filename = path;
                ConstructorTest.mode = openMode;

                ConstructorTest.called++;
            }

        public:
            MockFileManager(const ThisType REF) = delete;
            MockFileManager(ThisType RVALUE_REF other) noexcept
            {
                MoveConstructorTest.other = &other;
                MoveConstructorTest.called++;
            }

        public:
            ThisType REF operator=(const ThisType REF) = delete;
            ThisType REF operator=(ThisType RVALUE_REF other) noexcept
            {
                MoveAssignmentTest.other = &other;
                MoveAssignmentTest.called++;
                return PTR_VAL(this);
            }

        public:
            ~MockFileManager() noexcept
            {
                DestructorTest.called++;
                Close();
            }

        public:
            inline CFile * REF           GetCFile() noexcept
            {
                GetCFileTest.called++;

                if(GetCFileTest.override)
                    return GetCFileTest.returnVal;

                return State.File;
            }
            inline EnabledOperations REF GetEnabled() noexcept
            {
                GetEnabledTest.called++;
                if(GetEnabledTest.override)
                    return GetEnabledTest.returnVal;

                return State.Enabled;
            }
            inline EnabledOperations GetEnabled() const noexcept
            {
                GetEnabledTest.called++;
                if(GetEnabledTest.override)
                    return GetEnabledTest.returnVal;

                return State.Enabled;
            }

            inline void Swap(ThisType REF other) noexcept
            {
                SwapTest.other = &other;
                SwapTest.called++;
            }

            inline bool OpenNew(const OsStringView REF path, const OsStringView REF openMode) noexcept
            {
                OpenNewTest.filename = path;
                OpenNewTest.mode = openMode;
                OpenNewTest.called++;

                //Is used for testing OpenNew - somtimes the file cannot be opened
                // from unspecified OS reason - this simulates it
                if(OpenNewTest.simulateReadOnly && openMode != "r")
                    return false;

                if(OpenNewTest.returnVal)
                    State.IsOpen = true;

                return OpenNewTest.returnVal;
            }

            inline bool IsOpen() const noexcept
            {
                IsOpenTest.called++;
                //return IsOpenTest.returnVal;
                return State.IsOpen;
            }
            inline bool IsClosed() const noexcept
            {
                IsClosedTest.called++;
                //return IsClosedTest.returnVal;
                return NOT State.IsOpen;
            }
            bool Close() noexcept
            {
                State.IsOpen = false;
                CloseTest.called++;
                return CloseTest.returnVal;
            }

    };

    MockFileManager::DefConstructorStruct   MockFileManager::DefConstructorTest = MockFileManager::DefConstructorStruct();
    MockFileManager::DestructorStruct       MockFileManager::DestructorTest = MockFileManager::DestructorStruct();
    MockFileManager::ConstructorStruct      MockFileManager::ConstructorTest = MockFileManager::ConstructorStruct();
    MockFileManager::MoveConstructorStruct  MockFileManager::MoveConstructorTest = MockFileManager::MoveConstructorStruct();
    MockFileManager::MoveAssignmentStruct   MockFileManager::MoveAssignmentTest = MockFileManager::MoveAssignmentStruct();
    MockFileManager::GetCFileStruct         MockFileManager::GetCFileTest = MockFileManager::GetCFileStruct();
    MockFileManager::GetEnabledStruct       MockFileManager::GetEnabledTest = MockFileManager::GetEnabledStruct();
    MockFileManager::SwapStruct             MockFileManager::SwapTest = MockFileManager::SwapStruct();
    MockFileManager::OpenNewStruct          MockFileManager::OpenNewTest = MockFileManager::OpenNewStruct();
    MockFileManager::IsOpenStruct           MockFileManager::IsOpenTest = MockFileManager::IsOpenStruct();
    MockFileManager::IsClosedStruct         MockFileManager::IsClosedTest = MockFileManager::IsClosedStruct();
    MockFileManager::CloseStruct            MockFileManager::CloseTest = MockFileManager::CloseStruct();


}

//Creating mock classes
// 1. Perserve the connections between functions of the class - ie. open should change the return value of isOpen | object state
// 2. Provide functionality to change these conections      | - Static functionality
// 3. Provide functionality to control any function         | - Static functionality

// (1)
// Open(...)
//      if(OpenTest.control)
//      { do custom behaviour }
//      else
//      { let class change its own state }

// (2)
// Open(...)
//      let class change its own state;
//      if(OpenTest.control)
//      { do custom behaviour }


namespace cio::UnsafeFileTesting 
{
    using namespace TestingUtilities;

    using UnsafeFile = Internal::UnadaptedUnsafeFile<MockAdapter, MockFileManager>;
}

namespace cio::UnsafeFileTesting 
{
    TEST_CASE("[UnsafeFile] : Default constructor should call constructor of base", "[UnsafeFile][Default constructor]")
    {
        ResetTest(MockFileManager::DefConstructorTest);
        UnsafeFile file;

        REQUIRE(MockFileManager::DefConstructorTest.called == 1);
    }
    TEST_CASE("[UnsafeFile] : File should not be copy constructible", "[UnsafeFile][Copy constructor]")
    {
        REQUIRE(std::is_copy_constructible_v<UnsafeFile> == false);
    }
    TEST_CASE("[UnsafeFile] : Move constructor should call move constructor of base", "[UnsafeFile][Move constructor]")
    {
        ResetTest(MockFileManager::MoveConstructorTest);
        UnsafeFile from;
        UnsafeFile file = std::move(from);

        REQUIRE(MockFileManager::MoveConstructorTest.called == 1);
        REQUIRE(MockFileManager::MoveConstructorTest.other == &from);
    }
    TEST_CASE("[UnsafeFile] : File should not be copy assignible", "[UnsafeFile][Copy assignment]")
    {
        REQUIRE(std::is_copy_assignable_v<UnsafeFile> == false);
    }
    TEST_CASE("[UnsafeFile] : Move assignment should call of move assignment of base", "[UnsafeFile][Move assignment]")
    {
        ResetTest(MockFileManager::MoveAssignmentTest);

        UnsafeFile from;
        UnsafeFile file;
        file = std::move(from);

        REQUIRE(MockFileManager::MoveAssignmentTest.called == 1);
        REQUIRE(MockFileManager::MoveAssignmentTest.other == &from);
    }
    TEST_CASE("[UnsafeFile] : Destructor should call constructor of base", "[UnsafeFile][Destructor]")
    {
        ResetTest(MockFileManager::DestructorTest);

        {
            UnsafeFile file;
        }

        REQUIRE(MockFileManager::DestructorTest.called == 1);
    }
    TEST_CASE("[UnsafeFile] : GetUniqueFilename should write the filename to the provided buffers or return false and do nothing", "[UnsafeFile][GetUniqueFilename]")
    {
        ResetTest(MockAdapter::Tmpnam_sTest);

        constexpr Size size = UnsafeFile::TempFileNameMaxLenght;
        UnsafeFile::OsCharType filename[size*2]; //Just for testing - the size does not need to be multiplied by two

        WHEN("Provided with insufficient size the function should always return false")
        {
            UnsafeFile::OsString invalidFilenameStr(size - 1, ' ');

            REQUIRE(UnsafeFile::GetUniqueFilename(filename, 0) == false);
            REQUIRE(UnsafeFile::GetUniqueFilename(filename, size - 1) == false);
            REQUIRE(UnsafeFile::GetUniqueFilename(invalidFilenameStr) == false);
            //The adapter function should not be called
            REQUIRE(MockAdapter::Tmpnam_sTest.called == 0);
        }

        WHEN("Provided with sufficient size the function should write the data if the function adapter function succeeds")
        {
            UnsafeFile::OsString filenameStr(size, ' ');

            WHEN("The function succeeds the function should return true")
            {
                MockAdapter::Tmpnam_sTest.returnVal = MockAdapter::SuccessVal;
                REQUIRE(UnsafeFile::GetUniqueFilename(filename, size) == true);
                REQUIRE(UnsafeFile::GetUniqueFilename(filename, size + 1) == true);
                REQUIRE(UnsafeFile::GetUniqueFilename(filenameStr) == true);
                REQUIRE(MockAdapter::Tmpnam_sTest.called == 3);
            }
            WHEN("The function succeeds the function should return true")
            {
                MockAdapter::Tmpnam_sTest.returnVal = MockAdapter::ErrorVal;
                REQUIRE(UnsafeFile::GetUniqueFilename(filename, size) == false);
                REQUIRE(UnsafeFile::GetUniqueFilename(filename, size + 1) == false);
                REQUIRE(UnsafeFile::GetUniqueFilename(filenameStr) == false);
                REQUIRE(MockAdapter::Tmpnam_sTest.called == 3);
            }
        }

    }

    #define TEST_FILENAME     "test _#^%d filename"
    #define NEW_TEST_FILENAME "New " TEST_FILENAME

    TEST_CASE("[UnsafeFile] : IsFileOpenable should return if the file can be opened", "[UnsafeFile][IsFileOpenable]")
    {
        ResetTest(MockFileManager::OpenNewTest);
        UnsafeFile::OsStringView filename = TEST_FILENAME;

        WHEN("A file cannot be opened for read (OpenNewTest returns false) the function should return false")
        {
            MockFileManager::OpenNewTest.returnVal = false;
            REQUIRE(UnsafeFile::IsFileOpenable(filename) == false);
            REQUIRE(MockFileManager::OpenNewTest.mode == "r");
            REQUIRE(MockFileManager::OpenNewTest.filename == filename);
        }

        WHEN("A file can be opened for read (OpenNewTest returns true) the function should return true")
        {
            MockFileManager::OpenNewTest.returnVal = true;
            REQUIRE(UnsafeFile::IsFileOpenable(filename) == true);
            REQUIRE(MockFileManager::OpenNewTest.mode == "r");
            REQUIRE(MockFileManager::OpenNewTest.filename == filename);
        }
    }
    TEST_CASE("[UnsafeFile] : CreateFile should create a file or return false", "[UnsafeFile][IsFileOpenable]")
    {
        ResetTest(MockFileManager::OpenNewTest);
        UnsafeFile::OsStringView filename = TEST_FILENAME;

        WHEN("A file cannot be opened for append (OpenNewTest returns false) the function should return false")
        {
            MockFileManager::OpenNewTest.returnVal = false;
            REQUIRE(UnsafeFile::CreateFile(filename) == false);
            REQUIRE(MockFileManager::OpenNewTest.mode == "a");
            REQUIRE(MockFileManager::OpenNewTest.filename == filename);
        }

        WHEN("A file can be opened for append (OpenNewTest returns true) the function should return true")
        {
            MockFileManager::OpenNewTest.returnVal = true;
            REQUIRE(UnsafeFile::CreateFile(filename) == true);
            REQUIRE(MockFileManager::OpenNewTest.mode == "a");
            REQUIRE(MockFileManager::OpenNewTest.filename == filename);
        }
    }

    TEST_CASE("[UnsafeFile] : RenameFile should rename a file or return false", "[UnsafeFile][RenameFile]")
    {
        ResetTest(MockAdapter::RenameTest);
        UnsafeFile::OsStringView oldFilename = TEST_FILENAME;
        UnsafeFile::OsStringView newFilename = NEW_TEST_FILENAME;

        WHEN("The adapter function fails the function should return false")
        {
            MockAdapter::RenameTest.returnVal = MockAdapter::ErrorVal;
            REQUIRE(UnsafeFile::RenameFile(oldFilename, newFilename) == false);
            REQUIRE(MockAdapter::RenameTest.oldFilename == oldFilename);
            REQUIRE(MockAdapter::RenameTest.newFilename == newFilename);
        }

        WHEN("The adapter function succeeds the function should return true")
        {
            MockAdapter::RenameTest.returnVal = MockAdapter::SuccessVal;
            REQUIRE(UnsafeFile::RenameFile(oldFilename, newFilename) == true);
            REQUIRE(MockAdapter::RenameTest.oldFilename == oldFilename);
            REQUIRE(MockAdapter::RenameTest.newFilename == newFilename);
        }
    }
    TEST_CASE("[UnsafeFile] : RemoveFile should remove a file or return false", "[UnsafeFile][RemoveFile]")
    {
        ResetTest(MockAdapter::RemoveTest);
        UnsafeFile::OsStringView filename = TEST_FILENAME;

        WHEN("The adapter function fails the function should return false")
        {
            MockAdapter::RemoveTest.returnVal = MockAdapter::ErrorVal;
            REQUIRE(UnsafeFile::RemoveFile(filename) == false);
            REQUIRE(MockAdapter::RemoveTest.filename == filename);
        }

        WHEN("The adapter function succeeds the function should return true")
        {
            MockAdapter::RemoveTest.returnVal = MockAdapter::SuccessVal;
            REQUIRE(UnsafeFile::RemoveFile(filename) == true);
            REQUIRE(MockAdapter::RemoveTest.filename == filename);
        }
    }

    TEST_CASE("[UnsafeFile] : GetFileSize should return size of the specified file; If file size could not be retrieved UnsafeFile::ErrorSize (-1) is returned instead", "[UnsafeFile][MoveTo]")
    {

    }

    TEST_CASE("[UnsafeFile] : OpenNew with OpenMode should close the currently open file and open a new file according to the given path and openMode", "[UnsafeFile][OpenNew]")
    {
        UnsafeFile file;
        UnsafeFile::OsStringView filename = TEST_FILENAME;
        ResetTest(MockFileManager::CloseTest);
        ResetTest(MockFileManager::OpenNewTest);
        ResetTest(MockFileManager::GetEnabledTest);

        WHEN("Given openMode is not valid the function should return false")
        {
            REQUIRE(file.OpenNew(filename, OpenMode()) == false);
            REQUIRE(MockFileManager::CloseTest.called == 1);
        }

        WHEN("Execution of required before actions fails the function should return false")
        {
            WHEN("is required to exist but the file cannot be opened")
            {
                constexpr OpenMode mode(OpenModeFlag::Write);
                static_assert (mode.IsValid(), "!");
                MockFileManager::OpenNewTest.returnVal = false;

                REQUIRE(file.OpenNew(filename, mode) == false);
                REQUIRE(MockFileManager::CloseTest.called >= 1);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::Closed);
            }
            WHEN("is required to not exist but the file can be opened")
            {
                constexpr OpenMode mode(OpenModeFlag::Write, OpenModeFlag::Create, OpenModeFlag::MustNotExist);
                static_assert (mode.IsValid(), "!");
                MockFileManager::OpenNewTest.returnVal = true;

                REQUIRE(file.OpenNew(filename, mode) == false);
                REQUIRE(MockFileManager::CloseTest.called >= 1);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::Closed);
            }
        }

        WHEN("Execution of the opening itself fails the function returns false")
        {
            constexpr OpenMode mode(OpenModeFlag::Write);
            static_assert (mode.IsValid(), "!");
            MockFileManager::OpenNewTest.simulateReadOnly = true;

            UnsafeFile::OsStringView view = mode;

            REQUIRE(file.OpenNew(filename, mode) == false);
            REQUIRE(MockFileManager::OpenNewTest.filename == filename);
            REQUIRE(MockFileManager::OpenNewTest.mode == view);
            REQUIRE(MockFileManager::OpenNewTest.called == 2);
            REQUIRE(MockFileManager::CloseTest.called == 2);
            REQUIRE(file.GetEnabledOperations() == EnabledOperations::Closed);
        }

        WHEN("All succeeds the function should return true and the enabled action should be set")
        {
            SECTION("1")
            {
                constexpr OpenMode mode(OpenModeFlag::Write);
                static_assert (mode.IsValid(), "!");

                UnsafeFile::OsStringView view = mode;

                REQUIRE(file.OpenNew(filename, mode) == true);
                REQUIRE(MockFileManager::CloseTest.called == 2); //one from must exist one from opening itself
                REQUIRE(MockFileManager::OpenNewTest.called == 2);
                REQUIRE(MockFileManager::OpenNewTest.filename == filename);
                REQUIRE(MockFileManager::OpenNewTest.mode == view);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::Write);

            }
            SECTION("2")
            {
                constexpr OpenMode mode(OpenModeFlag::Write, OpenModeFlag::Read, OpenModeFlag::Create);
                static_assert (mode.IsValid(), "!");

                UnsafeFile::OsStringView view = mode;

                REQUIRE(file.OpenNew(filename, mode) == true);
                REQUIRE(MockFileManager::CloseTest.called == 2); //one from create one from opening itself
                REQUIRE(MockFileManager::OpenNewTest.called == 2);
                REQUIRE(MockFileManager::OpenNewTest.filename == filename);
                REQUIRE(MockFileManager::OpenNewTest.mode == view);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::ReadWrite);
            }
            SECTION("3")
            {
                constexpr OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Translated);
                static_assert (mode.IsValid(), "!");

                UnsafeFile::OsStringView view = mode;

                REQUIRE(file.OpenNew(filename, mode) == true);
                REQUIRE(MockFileManager::CloseTest.called == 1);
                REQUIRE(MockFileManager::OpenNewTest.called == 1);
                REQUIRE(MockFileManager::OpenNewTest.filename == filename);
                REQUIRE(MockFileManager::OpenNewTest.mode == view);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::Read);
            }
            SECTION("4")
            {
                constexpr OpenMode mode(OpenModeFlag::Create, OpenModeFlag::Override);
                static_assert (mode.IsValid(), "!");

                UnsafeFile::OsStringView view = mode;

                REQUIRE(file.OpenNew(filename, mode) == true);
                REQUIRE(MockFileManager::CloseTest.called == 2); //one from create - one from implicite read
                REQUIRE(MockFileManager::OpenNewTest.called == 2);
                REQUIRE(MockFileManager::OpenNewTest.filename == filename);
                REQUIRE(MockFileManager::OpenNewTest.mode == view);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::None);
            }

            WHEN("Move to end is required it is called")
            {
                ResetTest(MockAdapter::FseekTest);

                constexpr OpenMode mode(OpenModeFlag::Write, OpenModeFlag::StartAtEnd);
                static_assert (mode.IsValid(), "!");

                UnsafeFile::OsStringView view = mode;

                REQUIRE(file.OpenNew(filename, mode) == true);
                CHECK(MockFileManager::CloseTest.called == 2); //one from must exist one from opening itself
                CHECK(MockFileManager::OpenNewTest.called == 2);
                CHECK(MockFileManager::OpenNewTest.filename == filename);
                CHECK(MockFileManager::OpenNewTest.mode == view);
                CHECK(MockAdapter::FseekTest.called == 1);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::Write);
            }
        }

    }

    TEST_CASE("[UnsafeFile] : OpenNew with individual OpenModeFlags should close the currently open file and open a new file according to the given path and openMode", "[UnsafeFile][OpenNew]")
    {
        UnsafeFile file;
        UnsafeFile::OsStringView filename = TEST_FILENAME;
        ResetTest(MockFileManager::CloseTest);
        ResetTest(MockFileManager::OpenNewTest);
        ResetTest(MockFileManager::GetEnabledTest);

        WHEN("Given openMode is not valid the function should return false")
        {
            REQUIRE(file.OpenNew(filename, OpenMode()) == false);
            REQUIRE(MockFileManager::CloseTest.called == 1);
        }

        WHEN("Execution of required before actions fails the function should return false")
        {
            WHEN("is required to exist but the file cannot be opened")
            {
                constexpr OpenMode mode(OpenModeFlag::Write);
                static_assert (mode.IsValid(), "!");
                MockFileManager::OpenNewTest.returnVal = false;

                REQUIRE(file.OpenNew(filename, OpenModeFlag::Write) == false);
                REQUIRE(MockFileManager::CloseTest.called >= 1);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::Closed);
            }
            WHEN("is required to not exist but the file can be opened")
            {
                constexpr OpenMode mode(OpenModeFlag::Write, OpenModeFlag::Create, OpenModeFlag::MustNotExist);
                static_assert (mode.IsValid(), "!");
                MockFileManager::OpenNewTest.returnVal = true;

                REQUIRE(file.OpenNew(filename, OpenModeFlag::Write, OpenModeFlag::Create, OpenModeFlag::MustNotExist) == false);
                REQUIRE(MockFileManager::CloseTest.called >= 1);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::Closed);
            }
        }

        WHEN("Execution of the opening itself fails the function returns false")
        {
            constexpr OpenMode mode(OpenModeFlag::Write);
            static_assert (mode.IsValid(), "!");
            MockFileManager::OpenNewTest.simulateReadOnly = true;

            UnsafeFile::OsStringView view = mode;

            REQUIRE(file.OpenNew(filename, OpenModeFlag::Write) == false);
            REQUIRE(MockFileManager::OpenNewTest.filename == filename);
            REQUIRE(MockFileManager::OpenNewTest.mode == view);
            REQUIRE(MockFileManager::OpenNewTest.called == 2);
            REQUIRE(MockFileManager::CloseTest.called == 2);
            REQUIRE(file.GetEnabledOperations() == EnabledOperations::Closed);
        }

        WHEN("All succeeds the function should return true and the enabled action should be set")
        {
            SECTION("1")
            {
                constexpr OpenMode mode(OpenModeFlag::Write);
                static_assert (mode.IsValid(), "!");

                UnsafeFile::OsStringView view = mode;

                REQUIRE(file.OpenNew(filename, OpenModeFlag::Write) == true);
                REQUIRE(MockFileManager::CloseTest.called == 2); //one from must exist one from opening itself
                REQUIRE(MockFileManager::OpenNewTest.called == 2);
                REQUIRE(MockFileManager::OpenNewTest.filename == filename);
                REQUIRE(MockFileManager::OpenNewTest.mode == view);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::Write);

            }
            SECTION("2")
            {
                constexpr OpenMode mode(OpenModeFlag::Write, OpenModeFlag::Read, OpenModeFlag::Create);
                static_assert (mode.IsValid(), "!");

                UnsafeFile::OsStringView view = mode;

                REQUIRE(file.OpenNew(filename, OpenModeFlag::Write, OpenModeFlag::Read, OpenModeFlag::Create) == true);
                REQUIRE(MockFileManager::CloseTest.called == 2); //one from create one from opening itself
                REQUIRE(MockFileManager::OpenNewTest.called == 2);
                REQUIRE(MockFileManager::OpenNewTest.filename == filename);
                REQUIRE(MockFileManager::OpenNewTest.mode == view);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::ReadWrite);
            }
            SECTION("3")
            {
                constexpr OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Translated);
                static_assert (mode.IsValid(), "!");

                UnsafeFile::OsStringView view = mode;

                REQUIRE(file.OpenNew(filename, OpenModeFlag::Read, OpenModeFlag::Translated) == true);
                REQUIRE(MockFileManager::CloseTest.called == 1);
                REQUIRE(MockFileManager::OpenNewTest.called == 1);
                REQUIRE(MockFileManager::OpenNewTest.filename == filename);
                REQUIRE(MockFileManager::OpenNewTest.mode == view);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::Read);
            }
            SECTION("4")
            {
                constexpr OpenMode mode(OpenModeFlag::Create, OpenModeFlag::Override);
                static_assert (mode.IsValid(), "!");

                UnsafeFile::OsStringView view = mode;

                REQUIRE(file.OpenNew(filename, OpenModeFlag::Create, OpenModeFlag::Override) == true);
                REQUIRE(MockFileManager::CloseTest.called == 2); //one from create - one from implicite read
                REQUIRE(MockFileManager::OpenNewTest.called == 2);
                REQUIRE(MockFileManager::OpenNewTest.filename == filename);
                REQUIRE(MockFileManager::OpenNewTest.mode == view);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::None);
            }

            WHEN("Move to end is required it is called")
            {
                ResetTest(MockAdapter::FseekTest);

                constexpr OpenMode mode(OpenModeFlag::Write, OpenModeFlag::StartAtEnd);
                static_assert (mode.IsValid(), "!");

                UnsafeFile::OsStringView view = mode;

                REQUIRE(file.OpenNew(filename, OpenModeFlag::Write, OpenModeFlag::StartAtEnd) == true);
                CHECK(MockFileManager::CloseTest.called == 2); //one from must exist one from opening itself
                CHECK(MockFileManager::OpenNewTest.called == 2);
                CHECK(MockFileManager::OpenNewTest.filename == filename);
                CHECK(MockFileManager::OpenNewTest.mode == view);
                CHECK(MockAdapter::FseekTest.called == 1);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::Write);
            }
        }

    }

    TEST_CASE("[UnsafeFile] : Constructor with OpenMode should open a file as if OpenNew is called", "[UnsafeFile][Constructor]")
    {
        UnsafeFile::OsStringView filename = TEST_FILENAME;
        ResetTest(MockFileManager::CloseTest);
        ResetTest(MockFileManager::OpenNewTest);
        ResetTest(MockFileManager::GetEnabledTest);

        WHEN("Given openMode is not valid the function should return false")
        {
            UnsafeFile file(filename, OpenMode());
            REQUIRE(file.IsOpen() == false);
            REQUIRE(MockFileManager::CloseTest.called == 1);
        }

        WHEN("Execution of required before actions fails the function should return false")
        {
            WHEN("is required to exist but the file cannot be opened")
            {
                constexpr OpenMode mode(OpenModeFlag::Write);
                static_assert (mode.IsValid(), "!");
                MockFileManager::OpenNewTest.returnVal = false;

                UnsafeFile file(filename, mode);
                REQUIRE(file.IsOpen() == false);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::Closed);
            }
            WHEN("is required to not exist but the file can be opened")
            {
                constexpr OpenMode mode(OpenModeFlag::Write, OpenModeFlag::Create, OpenModeFlag::MustNotExist);
                static_assert (mode.IsValid(), "!");
                MockFileManager::OpenNewTest.returnVal = true;

                UnsafeFile file(filename, mode);
                REQUIRE(file.IsOpen() == false);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::Closed);
            }
        }

        WHEN("Execution of the opening itself fails the function returns false")
        {
            constexpr OpenMode mode(OpenModeFlag::Write);
            static_assert (mode.IsValid(), "!");
            MockFileManager::OpenNewTest.simulateReadOnly = true;

            UnsafeFile file(filename, mode);
            REQUIRE(file.IsOpen() == false);
            REQUIRE(file.GetEnabledOperations() == EnabledOperations::Closed);
        }

        WHEN("All succeeds the function should return true and the enabled action should be set")
        {
            SECTION("1")
            {
                constexpr OpenMode mode(OpenModeFlag::Write);
                static_assert (mode.IsValid(), "!");

                UnsafeFile file(filename, mode);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::Write);

            }
            SECTION("2")
            {
                constexpr OpenMode mode(OpenModeFlag::Write, OpenModeFlag::Read, OpenModeFlag::Create);
                static_assert (mode.IsValid(), "!");

                UnsafeFile file(filename, mode);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::ReadWrite);
            }
            SECTION("3")
            {
                constexpr OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Translated);
                static_assert (mode.IsValid(), "!");

                UnsafeFile file(filename, mode);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::Read);
            }
            SECTION("4")
            {
                constexpr OpenMode mode(OpenModeFlag::Create, OpenModeFlag::Override);
                static_assert (mode.IsValid(), "!");

                UnsafeFile file(filename, mode);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::None);
            }
        }

    }

    TEST_CASE("[UnsafeFile] : Constructor with individual OpenModeFlags should open a new file according to the given path and openMode", "[UnsafeFile][Constructor]")
    {
        UnsafeFile file;
        UnsafeFile::OsStringView filename = TEST_FILENAME;
        ResetTest(MockFileManager::CloseTest);
        ResetTest(MockFileManager::OpenNewTest);
        ResetTest(MockFileManager::GetEnabledTest);

        WHEN("Given openMode is not valid the function should return false")
        {
            UnsafeFile file(filename);
            REQUIRE(file.IsOpen() == false);
            REQUIRE(MockFileManager::CloseTest.called == 1);
        }

        WHEN("Execution of required before actions fails the function should return false")
        {
            WHEN("is required to exist but the file cannot be opened")
            {
                constexpr OpenMode mode(OpenModeFlag::Write);
                static_assert (mode.IsValid(), "!");
                MockFileManager::OpenNewTest.returnVal = false;

                UnsafeFile file(filename, OpenModeFlag::Write, OpenModeFlag::Create, OpenModeFlag::MustNotExist);
                REQUIRE(file.IsOpen() == false);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::Closed);
            }
            WHEN("is required to not exist but the file can be opened")
            {
                constexpr OpenMode mode(OpenModeFlag::Write, OpenModeFlag::Create, OpenModeFlag::MustNotExist);
                static_assert (mode.IsValid(), "!");
                MockFileManager::OpenNewTest.returnVal = true;

                UnsafeFile file(filename, OpenModeFlag::Write, OpenModeFlag::Create, OpenModeFlag::MustNotExist);
                REQUIRE(file.IsOpen() == false);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::Closed);
            }
        }

        WHEN("Execution of the opening itself fails the function returns false")
        {
            constexpr OpenMode mode(OpenModeFlag::Write);
            static_assert (mode.IsValid(), "!");
            MockFileManager::OpenNewTest.simulateReadOnly = true;

            UnsafeFile file(filename, OpenModeFlag::Write);
            REQUIRE(file.IsOpen() == false);
            REQUIRE(file.GetEnabledOperations() == EnabledOperations::Closed);
        }

        WHEN("All succeeds the function should return true and the enabled action should be set")
        {
            SECTION("1")
            {
                constexpr OpenMode mode(OpenModeFlag::Write);
                static_assert (mode.IsValid(), "!");

                UnsafeFile file(filename, OpenModeFlag::Write);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::Write);

            }
            SECTION("2")
            {
                constexpr OpenMode mode(OpenModeFlag::Write, OpenModeFlag::Read, OpenModeFlag::Create);
                static_assert (mode.IsValid(), "!");

                UnsafeFile file(filename, OpenModeFlag::Write, OpenModeFlag::Read, OpenModeFlag::Create);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::ReadWrite);
            }
            SECTION("3")
            {
                constexpr OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Translated);
                static_assert (mode.IsValid(), "!");

                UnsafeFile file(filename, OpenModeFlag::Read, OpenModeFlag::Translated);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::Read);
            }
            SECTION("4")
            {
                constexpr OpenMode mode(OpenModeFlag::Create, OpenModeFlag::Override);
                static_assert (mode.IsValid(), "!");

                UnsafeFile file(filename, OpenModeFlag::Create, OpenModeFlag::Override);
                REQUIRE(file.IsOpen() == true);
                REQUIRE(file.GetEnabledOperations() == EnabledOperations::None);
            }
        }

    }

    TEST_CASE("[UnsafeFile] : WasEndOfFileRieched should return if the end of file was reached in the curretnly opened file", "[UnsafeFile][WasEndOfFileRieched]")
    {
        ResetTest(MockAdapter::FeofTest);
        UnsafeFile file;
        MockFileManager REF manager = file;

        WHEN("The adapter feof return error the function should return true")
        {
            MockAdapter::FeofTest.returnVal = MockAdapter::ErrorVal;
            REQUIRE(file.WasEndOfFileRieched() == true);
            REQUIRE(MockAdapter::FeofTest.stream == manager.GetCFile());
            REQUIRE(MockAdapter::FeofTest.called == 1);
        }

        WHEN("The adapter feof return success the function should return false")
        {
            MockAdapter::FeofTest.returnVal = MockAdapter::SuccessVal;
            REQUIRE(file.WasEndOfFileRieched() == false);
            REQUIRE(MockAdapter::FeofTest.stream == manager.GetCFile());
            REQUIRE(MockAdapter::FeofTest.called == 1);
        }
    }
    TEST_CASE("[UnsafeFile] : GetEnabledOperations should return the enebled operations on the file", "[UnsafeFile][GetEnabledOperations]")
    {
        ResetTest(MockFileManager::GetCFileTest);
        UnsafeFile file;
        MockFileManager REF manager = file;

        SECTION("Closed")
        {
            manager.GetEnabled() = EnabledOperations::Closed;
            REQUIRE(file.GetEnabledOperations() == manager.GetEnabled());
        }
        SECTION("Read")
        {
            manager.GetEnabled() = EnabledOperations::Read;
            REQUIRE(file.GetEnabledOperations() == manager.GetEnabled());
        }
        SECTION("Write")
        {
            manager.GetEnabled() = EnabledOperations::Write;
            REQUIRE(file.GetEnabledOperations() == manager.GetEnabled());
        }
        SECTION("ReadWrite")
        {
            manager.GetEnabled() = EnabledOperations::ReadWrite;
            REQUIRE(file.GetEnabledOperations() == manager.GetEnabled());
        }
        SECTION("None")
        {
            manager.GetEnabled() = EnabledOperations::None;
            REQUIRE(file.GetEnabledOperations() == manager.GetEnabled());
        }
    }
    TEST_CASE("[UnsafeFile] : IsReadingEnabled should return if reading is enabled in the opened file", "[UnsafeFile][IsReadingEnabled]")
    {
        ResetTest(MockFileManager::GetCFileTest);
        UnsafeFile file;
        MockFileManager REF manager = file;

        SECTION("Closed")
        {
            manager.GetEnabled() = EnabledOperations::Closed;
            REQUIRE(file.IsReadingEnabled() == false);
        }
        SECTION("Read")
        {
            manager.GetEnabled() = EnabledOperations::Read;
            REQUIRE(file.IsReadingEnabled() == true);
        }
        SECTION("Write")
        {
            manager.GetEnabled() = EnabledOperations::Write;
            REQUIRE(file.IsReadingEnabled() == false);
        }
        SECTION("ReadWrite")
        {
            manager.GetEnabled() = EnabledOperations::ReadWrite;
            REQUIRE(file.IsReadingEnabled() == true);
        }
        SECTION("None")
        {
            manager.GetEnabled() = EnabledOperations::None;
            REQUIRE(file.IsReadingEnabled() == false);
        }
    }
    TEST_CASE("[UnsafeFile] : IsWritingEnabled should return if writing is enabled in the opened file", "[UnsafeFile][IsWritingEnabled]")
    {
        ResetTest(MockAdapter::FeofTest);
        ResetTest(MockFileManager::GetCFileTest);
        UnsafeFile file;
        MockFileManager REF manager = file;

        SECTION("Closed")
        {
            manager.GetEnabled() = EnabledOperations::Closed;
            REQUIRE(file.IsWritingEnabled() == false);
        }
        SECTION("Read")
        {
            manager.GetEnabled() = EnabledOperations::Read;
            REQUIRE(file.IsWritingEnabled() == false);
        }
        SECTION("Write")
        {
            manager.GetEnabled() = EnabledOperations::Write;
            REQUIRE(file.IsWritingEnabled() == true);
        }
        SECTION("ReadWrite")
        {
            manager.GetEnabled() = EnabledOperations::ReadWrite;
            REQUIRE(file.IsWritingEnabled() == true);
        }
        SECTION("None")
        {
            manager.GetEnabled() = EnabledOperations::None;
            REQUIRE(file.IsWritingEnabled() == false);
        }
    }

    TEST_CASE("[UnsafeFile] : SavePos should save the current position in the file and return if the function succeeded", "[UnsafeFile][SavePos]")
    {
        ResetTest(MockAdapter::FgetposTest);
        ResetTest(MockFileManager::GetCFileTest);
        UnsafeFile file;
        Position pos = Position();
        MockFileManager REF manager = file;

        WHEN("Fgetpos returns true the function should return true and set the Position argument")
        {
            MockAdapter::FgetposTest.returnVal = MockAdapter::SuccessVal;
            MockAdapter::FgetposTest.setVal = static_cast<Position>(123); //This is very unsafe but due to the vague specification of fpos_t there is no other way

            REQUIRE(file.SavePos(pos) == true);
            REQUIRE(MockAdapter::FgetposTest.stream == manager.GetCFile());
            REQUIRE(pos == MockAdapter::FgetposTest.setVal);
            REQUIRE(MockAdapter::FgetposTest.called == 1);
        }

        WHEN("Fgetpos returns false the function should return false")
        {
            MockAdapter::FgetposTest.returnVal = MockAdapter::ErrorVal;

            REQUIRE(file.SavePos(pos) == false);
            REQUIRE(MockAdapter::FgetposTest.stream == manager.GetCFile());
            REQUIRE(MockAdapter::FgetposTest.called == 1);
        }
    }

    TEST_CASE("[UnsafeFile] : RestorePos should restore the current position in file as obtained from SavePos", "[UnsafeFile][RestorePos]")
    {
        ResetTest(MockAdapter::FsetposTest);
        ResetTest(MockFileManager::GetCFileTest);
        UnsafeFile file;
        MockFileManager REF manager = file;

        Position pos = static_cast<Position>(123); //This is very unsafe but due to the vague specification of fpos_t there is no other way

        WHEN("Fsetpos returns true the function should return true and set the file position")
        {
            MockAdapter::FsetposTest.returnVal = MockAdapter::SuccessVal;

            REQUIRE(file.RestorePos(pos) == true);
            REQUIRE(MockAdapter::FsetposTest.stream == manager.GetCFile());
            REQUIRE(MockAdapter::FsetposTest.pos == pos);
            REQUIRE(MockAdapter::FsetposTest.called == 1);
        }

        WHEN("Fgetpos returns false the function should return false")
        {
            MockAdapter::FsetposTest.returnVal = MockAdapter::ErrorVal;

            REQUIRE(file.RestorePos(pos) == false);
            REQUIRE(MockAdapter::FsetposTest.stream == manager.GetCFile());
            REQUIRE(MockAdapter::FsetposTest.pos == pos);
            REQUIRE(MockAdapter::FsetposTest.called == 1);
        }
    }

    TEST_CASE("[UnsafeFile] : GetOffset should return the current offset of the file from the file beggining (in bytes when not open in translated mode)", "[UnsafeFile][GetOffset]")
    {
        ResetTest(MockAdapter::FtellTest);
        ResetTest(MockFileManager::GetCFileTest);
        UnsafeFile file;
        MockFileManager REF manager = file;

        MockAdapter::FtellTest.returnVal = 12;

        REQUIRE(file.GetOffset() == 12);
        REQUIRE(MockAdapter::FtellTest.stream == manager.GetCFile());
        REQUIRE(MockAdapter::FtellTest.called == 1);
    }

    TEST_CASE("[UnsafeFile] : MoveTo should move to the specified offset from starting point of origin", "[UnsafeFile][MoveTo]")
    {
        ResetTest(MockAdapter::FsetposTest);
        ResetTest(MockFileManager::GetCFileTest);
        UnsafeFile file;
        MockFileManager REF manager = file;

        Position pos = static_cast<Position>(123); //This is very unsafe but due to the vague specification of fpos_t there is no other way

        WHEN("Fsetpos returns true the function should return true and set the file position")
        {
            MockAdapter::FsetposTest.returnVal = MockAdapter::SuccessVal;

            REQUIRE(file.RestorePos(pos) == true);
            REQUIRE(MockAdapter::FsetposTest.stream == manager.GetCFile());
            REQUIRE(MockAdapter::FsetposTest.pos == pos);
            REQUIRE(MockAdapter::FsetposTest.called == 1);
        }

        WHEN("Fgetpos returns false the function should return false")
        {
            MockAdapter::FsetposTest.returnVal = MockAdapter::ErrorVal;

            REQUIRE(file.RestorePos(pos) == false);
            REQUIRE(MockAdapter::FsetposTest.stream == manager.GetCFile());
            REQUIRE(MockAdapter::FsetposTest.pos == pos);
            REQUIRE(MockAdapter::FsetposTest.called == 1);
        }
    }

    TEST_CASE("[UnsafeFile] : MoveToBeggining should move the position in file to the beggining", "[UnsafeFile][MoveToBeggining]")
    {
    }
    TEST_CASE("[UnsafeFile] : MoveToEnd should move the position in file to the end", "[UnsafeFile][MoveToEnd]")
    {

    }

    TEST_CASE("[UnsafeFile] : MoveBy should offset the current possition in file by the specified offset", "[UnsafeFile][MoveBy]")
    {

    }
    TEST_CASE("[UnsafeFile] : Read should read count elements, store them in the specified buffer and move the position in file to the end of block read; Returns if the read operation was successful", "[UnsafeFile][Read]")
    {

    }
    TEST_CASE("[UnsafeFile] : ReadAndCount should read count elements, store them in the specified buffer and move the position inside file to the end of block read; Returns the number of elements sucessfully read", "[UnsafeFile][ReadAndCount]")
    {

    }
    TEST_CASE("[UnsafeFile] : ReadObject should read a single element and save it to the specified output argument; Returns if the read operation was successful", "[UnsafeFile][ReadObject]")
    {

    }
    TEST_CASE("[UnsafeFile] : ReadString should read the string.size() charcters into the string; Returns if the read operation was successful", "[UnsafeFile][ReadString]")
    {

    }
    TEST_CASE("[UnsafeFile] : Write should write count elements from the specified buffer and move the position in file to the end of block written; Returns if the read operation was successful", "[UnsafeFile][Write]")
    {

    }
    TEST_CASE("[UnsafeFile] : WriteAndCount should write count elements from the specified buffer and move the position in file to the end of block written; Returns the number of elements sucessfully written", "[UnsafeFile][WriteAndCount]")
    {

    }
    TEST_CASE("[UnsafeFile] : WriteObject should write the provided argument and move position in file after it; Returns if the read operation was successful", "[UnsafeFile][WriteObject]")
    {

    }
    TEST_CASE("[UnsafeFile] : WriteString should write all charcters of any string type (string literal, C string, std::basic_string, std::basic_string_view) into the file; Returns if the read operation was successful", "[UnsafeFile][WriteString]")
    {

    }
    TEST_CASE("[UnsafeFile] : SetBuffer should set a buffer for all read write operations as specified by the arguments and return if the operation was successful; If mode is BufferingMode::None the other arguments are ignored and buffering is disabled", "[UnsafeFile][SetBuffer]")
    {

    }
    TEST_CASE("[UnsafeFile] : Flush should flush all contents of the buffer to the file", "[UnsafeFile][Flush]")
    {

    }
    TEST_CASE("[UnsafeFile] : ClearState should clear any state information about the file (except position) this includes clearing errors and flushing buffers", "[UnsafeFile][ClearState]")
    {

    }
    TEST_CASE("[UnsafeFile] : GetFileSize should return size of the currently opened file; This function might be inacurate if file was opened in translated mode", "[UnsafeFile][GetFileSize]")
    {

    }































}
#endif // NEWUNSAFEFILE_H
