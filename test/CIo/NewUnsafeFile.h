#ifndef NEWUNSAFEFILE_H
#define NEWUNSAFEFILE_H

#include "Catch2/Catch.hpp"
#include "UnsafeFile.h"
#include "MockAdaptor.h"

namespace cio::UnsafeFileTesting 
{
    using namespace TestingUtilities;

    using UnsafeFileT = Internal::UnadaptedUnsafeFile<MockAdapter>;
    using FileManagerT = Internal::UnadaptedFileManager<MockAdapter>;
}

namespace cio::UnsafeFileTesting 
{
    TEST_CASE("[UnsafeFile] : Default constructor should construct a closed file", "[UnsafeFile][Default constructor]")
    {
        UnsafeFileT file;
        REQUIRE(file.IsClosed() == true);
        REQUIRE(file.IsOpen() == false);
    }
    TEST_CASE("[UnsafeFile] : Move constructor should tranfer ownership of a file", "[UnsafeFile][Move constructor]")
    {
        UnsafeFileT base;
        FileManagerT REF baseRef = base;

        WHEN("Transfering a non null ptr it should be transfered")
        {
            CFile * const setPtr = reinterpret_cast<CFile *>(1);
            baseRef.GetCFile() = setPtr;

            UnsafeFileT constructed(std::move(base));
            FileManagerT REF constructedRef = constructed;

            REQUIRE(base.IsOpen() == false);
            REQUIRE(constructedRef.GetCFile() == setPtr);
        }

        WHEN("Transfering a default constructed file nothing should happen")
        {
            UnsafeFileT constructed(std::move(base));

            REQUIRE(base.IsOpen() == false);
            REQUIRE(constructed.IsOpen() == false);
        }
    }
    TEST_CASE("[UnsafeFile] : Move assignment should tranfer ownership of a file", "[UnsafeFile][Move assignment]")
    {
        UnsafeFileT base;
        FileManagerT REF baseRef = base;

        WHEN("Transfering a non null ptr it should be transfered")
        {
            CFile * const setPtr = reinterpret_cast<CFile *>(1);
            baseRef.GetCFile() = setPtr;

            UnsafeFileT constructed(std::move(base));
            FileManagerT REF constructedRef = constructed;

            REQUIRE(base.IsOpen() == false);
            REQUIRE(constructedRef.GetCFile() == setPtr);
        }

        WHEN("Transfering a default constructed file nothing should happen")
        {
            UnsafeFileT constructed(std::move(base));

            REQUIRE(base.IsOpen() == false);
            REQUIRE(constructed.IsOpen() == false);
        }
    }

    TEST_CASE("FirstTestcase : Test", "[test]")
    {
        UnsafeFileT file;
        FileManagerT REF rawFile = file;
        Position pos;
        file.SavePos(pos);

        REQUIRE(file.fgetposTest.stream == rawFile.GetCFile());
        REQUIRE(file.fgetposTest.called == 1);
    }
}
#endif // NEWUNSAFEFILE_H
