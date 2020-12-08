TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt
QMAKE_CXXFLAGS += -bigobj

INCLUDEPATH += \
        ../

#User files
HEADERS += \
    ../src/AuxiliaryTypes.h \
    ../src/CstdioAdapter.h \
    ../src/File.h \
    ../src/FileManager.h \
    ../src/OpenMode.h \
    ../src/OsSupport.h \
    ../src/UnsafeFile.h \
        CIo/FileManager.h \
        CIo/FileManager_TestingUtilities.h \
    CIo/MockAdaptor.h \
    CIo/NewUnsafeFile.h \
    CIo/Temp.h \
        CIo/UseExamples.h \
        CIo/ConstexprString.h \
        CIo/File.h \
        CIo/File_Construction.h \
        CIo/File_MiscellaneousFunctions.h \
        CIo/File_PositionFunctions.h \
        CIo/File_ReadingWritingFunctions.h \
        CIo/File_TestingUtilities.h \
        CIo/OpenMode.h \
        CIo/UniversalString.h \
        CIo/UnsafeFile.h \
        CIo/UnsafeFile_Construction.h \
        CIo/UnsafeFile_MiscellaneousFunctions.h \
        CIo/UnsafeFile_PositionFunctions.h \
        CIo/UnsafeFile_ReadingWritingFunctions.h \
        CIo/UnsafeFile_TestingUtilities.h \
        ../src/Common.h \
        ../src/ConstexprString.h \
        ../src/UniversalString.h \
        ../src/UseExamples.h \

SOURCES += \


#Prerequisites
HEADERS += Catch2/Catch.hpp \

SOURCES += UnitTestsMain.cpp



