#ifndef MOCKADAPTOR_H
#define MOCKADAPTOR_H

#include "src/AuxiliaryTypes.h"

namespace cio::TestingUtilities
{
    struct MockAdapter
    {
            using CharType = char8;

        public:
            struct fopenStruct
            {
                    const CharType * filename = nullptr;
                    const CharType * mode = nullptr;
                    i32 called = 0;
            } fopenTest;
            struct freopenStruct
            {
                    const CharType * filename = nullptr;
                    const CharType * mode = nullptr;
                    i32 called = 0;
            } freopenTest;
            struct fcloseStruct
            {
                    CFile PTR stream = nullptr;
                    i32 called = 0;
            } fcloseTest;
            struct feofStruct
            {
                    CFile PTR stream = nullptr;
                    i32 called = 0;
            } feofTest;
            struct fgetposStruct
            {
                    CFile PTR stream = nullptr;
                    Position PTR pos = nullptr;
                    i32 called = 0;
            } fgetposTest;
            struct fsetposStruct
            {
                    CFile PTR stream = nullptr;
                    Position pos = Position();
                    i32 called = 0;
            } fsetposTest;
            struct ftellStruct
            {
                    CFile PTR stream = nullptr;
                    i32 called = 0;
            } ftellTest;
            struct fseekStruct
            {
                    CFile PTR stream = nullptr;
                    Offset offset = 0;
                    Origin origin = Origin::Beggining;
                    i32 called = 0;
            } fseekTest;
            struct rewindStruct
            {
                    CFile PTR stream = nullptr;
                    i32 called = 0;
            } rewindTest;
            struct freadStruct
            {
                    void PTR ptrToData = nullptr;
                    Size elementSize = 0;
                    Size count = 0;
                    CFile PTR stream = nullptr;
                    i32 called = 0;
            } freadTest;
            struct fwriteStruct
            {
                    const void PTR ptrToData = nullptr;
                    Size elementSize = 0;
                    Size count = 0;
                    CFile PTR stream = nullptr;
                    i32 called = 0;
            } fwriteTest;
            struct setvbufStruct
            {
                    CFile PTR stream = nullptr;
                    char8 PTR bufferPtr = nullptr;
                    Size bufferSize = 0;
                    BufferingMode mode = BufferingMode::Full;
                    i32 called = 0;
            } setvbufTest;
            struct fflushStruct
            {
                    CFile PTR stream = nullptr;
                    i32 called = 0;
            } fflushTest;
            struct tmpnam_sStruct
            {
                    const CharType * filename = nullptr;
                    Size size = 0;
                    i32 called = 0;
            } tmpnam_sTest;
            struct renameStruct
            {
                    const CharType * oldFilename = nullptr;
                    const CharType * newFilename = nullptr;
                    i32 called = 0;
            } renameTest;
            struct removeStruct
            {
                    const CharType * filename = nullptr;
                    i32 called = 0;
            } removeTest;


        protected:
            inline auto fopen(const char8 * filename, const CharType * mode)
            {
                fopenTest.filename = filename;
                fopenTest.mode = mode;
                fopenTest.called ++;

                return 0;
            }
            inline auto freopen(const CharType * filename, const CharType * mode)
            {
                freopenTest.filename = filename;
                freopenTest.mode = mode;
                freopenTest.called ++;
                return 0;
            }
            inline auto fclose(CFile PTR stream)
            {
                fcloseTest.stream = stream;
                fcloseTest.called ++;
                return 0;
            }
            inline auto feof(CFile PTR stream)
            {
                feofTest.stream = stream;
                feofTest.called ++;
                return 0;
            }
            inline auto fgetpos(CFile PTR stream, Position REF pos)
            {
                fgetposTest.stream = stream;
                fgetposTest.pos = &pos;
                fgetposTest.called ++;

                return 0;
            }
            inline auto fsetpos(CFile PTR stream, const Position REF pos)
            {
                fsetposTest.stream = stream;
                fsetposTest.pos = pos;
                fsetposTest.called ++;

                return 0;
            }
            inline auto ftell(CFile PTR stream)
            {
                ftellTest.stream = stream;
                ftellTest.called ++;

                return 0;
            }
            inline auto fseek(CFile PTR stream, const Offset offset, const Origin origin)
            {
                fseekTest.stream = stream;
                fseekTest.offset = offset;
                fseekTest.origin = origin;
                fseekTest.called ++;

                return 0;
            }
            inline auto rewind(CFile PTR stream)
            {
                rewindTest.stream = stream;
                rewindTest.called ++;

                return 0;
            }
            inline auto fread(void PTR ptrToData, const Size elementSize, const Size count, CFile PTR stream)
            {
                freadTest.stream = stream;
                freadTest.ptrToData = ptrToData;
                freadTest.elementSize = elementSize;
                freadTest.count = count;
                freadTest.called ++;

                return 0;
            }
            inline auto fwrite(const void PTR ptrToData, const Size elementSize, const Size count, CFile PTR stream)
            {
                fwriteTest.stream = stream;
                fwriteTest.ptrToData = ptrToData;
                fwriteTest.elementSize = elementSize;
                fwriteTest.count = count;
                fwriteTest.called ++;

                return 0;
            }
            inline auto setvbuf(CFile PTR stream, char8 PTR bufferPtr, const Size bufferSize, const BufferingMode mode)
            {
                setvbufTest.stream = stream;
                setvbufTest.bufferPtr = bufferPtr;
                setvbufTest.bufferSize = bufferSize;
                setvbufTest.mode = mode;
                setvbufTest.called ++;

                return 0;
            }
            inline auto fflush(CFile PTR stream)
            {
                fflushTest.stream = stream;
                fflushTest.called ++;

                return 0;
            }
            inline auto tmpnam_s(CharType filename[], const Size size)
            {
                tmpnam_sTest.filename = filename;
                tmpnam_sTest.size = size;
                tmpnam_sTest.called ++;

                return 0;
            }
            inline auto rename(const CharType * oldFilename, const CharType * newFilename)
            {
                renameTest.oldFilename = oldFilename;
                renameTest.newFilename = newFilename;
                renameTest.called ++;

                return 0;
            }
            inline auto remove(const CharType * filename)
            {
                removeTest.filename = filename;
                removeTest.called ++;

                return 0;
            }
    };
}
#endif // MOCKADAPTOR_H
