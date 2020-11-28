#ifndef UNSAFEFILE_TESTINGUTILITIES_H
#define UNSAFEFILE_TESTINGUTILITIES_H

#include <array>
#include "src/UnsafeFile.h"
#include "FileManager_TestingUtilities.h"

namespace cio::UnsafeFileTesting
{
    template<typename CharT>
    using BasicFilenameCleaner = FileManagerTesting::BasicFilenameCleaner<CharT>;
}

#define UnsafeFile_TEST_COMPILE_ERROR false
#define UnsafeFileTestedTypes char8, charW

#endif // UNSAFEFILE_TESTINGUTILITIES_H
