#ifndef FILE_TESTINGUTILITIES_H
#define FILE_TESTINGUTILITIES_H

#include "src/File.h"
#include "UnsafeFile_TestingUtilities.h"

namespace CIo::FileTesting
{
    template<typename CharT>
    using BasicFilenameCleaner = FileManagerTesting::BasicFilenameCleaner<CharT>;
}

#define File_TEST_COMPILE_ERROR false
#define FileTestedTypes char8, charW

#endif // FILE_TESTINGUTILITIES_H
