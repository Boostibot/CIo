#ifndef CFILEMANAGER_TESTINGUTILITIES_H
#define CFILEMANAGER_TESTINGUTILITIES_H

#include "Catch2/Catch.hpp"
#include "src/FileManager.h"

namespace CIo::FileManagerTesting
{
    template<typename CharT, size_t CharsMax = L_tmpnam>
    struct BasicFilenameCleaner
    {
        private:
            using CharSupport = CharSupport<CharT>;
            using CStringRef = CStringRef<CharT>;

        private:
            bool State;
            CharT FilenameString[CharsMax];

        public:
            BasicFilenameCleaner()
            {
                State = (CharSupport::tmpnam_s(FilenameString, CharsMax) == 0);
            }
            BasicFilenameCleaner(const BasicFilenameCleaner REF) = default;
            ~BasicFilenameCleaner()
            {
                if(IsOk() == false)
                    return;

                //It is ok to remove file that does not exist
                // in that case the functions should do nothing
                CharSupport::remove(FilenameString);
            }
            inline auto Filename() const noexcept {return FilenameString;}
            inline bool IsOk()     const noexcept {return State; }

        public:
            BasicFilenameCleaner(BasicFilenameCleaner RVALUE_REF) = delete;
            BasicFilenameCleaner REF operator=(const BasicFilenameCleaner REF) = delete;
            BasicFilenameCleaner REF operator=(BasicFilenameCleaner RVALUE_REF) = delete;
    };

    template<typename T>
    constexpr BasicOpenMode<T> defaultModeTmpl(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Binary);
    static_assert (defaultModeTmpl<char8>.GetOpenModeString() == OpenMode::OpenModeString("w+b"),   "UNEXPECTED");
    static_assert (defaultModeTmpl<charW>.GetOpenModeString() == WOpenMode::OpenModeString(L"w+b"), "UNEXPECTED");
}

#define FileManager_TEST_COMPILE_ERROR false
#define FileManagerTestedTypes char8, charW

namespace CIo::FileManagerTesting::FilenameCleanerTesting
{
    TEMPLATE_TEST_CASE("[FilenameCleaner] : Constructor should create a unique filename", "[FilenameCleaner][Constructor]", FileManagerTestedTypes)
    {
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        using StringView = std::basic_string_view<TestType>;

        WHEN("a cleaner is constructed its filename should be set to something")
        {
            const FilenameCleaner cleaner;
            REQUIRE(cleaner.IsOk() == true);

            const StringView view = cleaner.Filename();
            const StringView empty;
            REQUIRE(view != empty);
        }

        WHEN("a multiple cleaners are constructed their filenames should differ")
        {
            const FilenameCleaner cleaner1;
            const FilenameCleaner cleaner2;
            const FilenameCleaner cleaner3;

            REQUIRE(cleaner1.IsOk() == true);
            REQUIRE(cleaner2.IsOk() == true);
            REQUIRE(cleaner3.IsOk() == true);

            const StringView view1 = cleaner1.Filename();
            const StringView view2 = cleaner2.Filename();
            const StringView view3 = cleaner3.Filename();

            REQUIRE(view1 != view2);
            REQUIRE(view2 != view3);
            REQUIRE(view3 != view1);
        }
    }

    TEMPLATE_TEST_CASE("[FilenameCleaner] : IsOk should return true if the constructor succeeded", "[FilenameCleaner][IsOk]", FileManagerTestedTypes)
    {
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        const FilenameCleaner cleaner;
        REQUIRE(cleaner.IsOk() == true);
    }

    TEMPLATE_TEST_CASE("[FilenameCleaner] : Copy constructor should copy the filename", "[FilenameCleaner][Copy constructor]", FileManagerTestedTypes)
    {
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        using StringView = std::basic_string_view<TestType>;

        const FilenameCleaner original;
        const FilenameCleaner copy(original);
        const StringView originalView = original.Filename();
        const StringView copyView     = copy.Filename();

        REQUIRE(original.IsOk() == true);
        REQUIRE(copy.IsOk() == true);
        REQUIRE(originalView == copyView);
    }

    TEMPLATE_TEST_CASE("[FilenameCleaner] : Destructor should destruct the file associated with the filename", "[FilenameCleaner][Destructor]", FileManagerTestedTypes)
    {
        using FilenameCleaner = BasicFilenameCleaner<TestType>;
        using FileManager = BasicFileManager<TestType>;

        WHEN("a file is associated with the filename it should get removed")
        {
            const FilenameCleaner cleaner;
            REQUIRE(cleaner.IsOk() == true);

            {
                //Opens a the specified file for writing (Creates a file)
                const FileManager manager(cleaner.Filename(), COpenMode::Write);
                REQUIRE(manager.IsOpen() == true);
            }

            {
                //Copies the associated filanem and then destructs it
                const FilenameCleaner copy(cleaner);
            }

            REQUIRE(FileManager::IsFileOpenable(cleaner.Filename()) == false);


        }
        WHEN("no file is associated with the filename nothing should happen")
        {
            const FilenameCleaner cleaner;
            REQUIRE(cleaner.IsOk() == true);

            REQUIRE(FileManager::IsFileOpenable(cleaner.Filename()) == false);

            {
                //Copies the associated filanem and then destructs it
                const FilenameCleaner copy(cleaner);
            }

            REQUIRE(FileManager::IsFileOpenable(cleaner.Filename()) == false);
        }

    }
}

#endif // CFILEMANAGER_TESTINGUTILITIES_H
