#ifndef MOCKADAPTOR_H
#define MOCKADAPTOR_H

#include "src/AuxiliaryTypes.h"

namespace cio::TestingUtilities
{
    struct MockAdapter
    {
            using CharType = char8;

        public:
            struct FopenStruct
            {
                    const CharType * filename = nullptr;
                    const CharType * mode = nullptr;
                    i32 called = 0;
                    i32 returnVal = 0;
            };
            struct FreopenStruct
            {
                    const CharType * filename = nullptr;
                    const CharType * mode = nullptr;
                    i32 called = 0;
                    i32 returnVal = 0;
            };
            struct FcloseStruct
            {
                    CFile PTR stream = nullptr;
                    i32 called = 0;
                    i32 returnVal = 0;
            };
            struct FeofStruct
            {
                    CFile PTR stream = nullptr;
                    i32 called = 0;
                    i32 returnVal = 0;
            };
            struct FgetposStruct
            {
                    CFile PTR stream = nullptr;
                    Position PTR pos = nullptr;
                    i32 called = 0;
                    i32 returnVal = 0;
                    Position setVal = Position();
            };
            struct FsetposStruct
            {
                    CFile PTR stream = nullptr;
                    Position pos = Position();
                    i32 called = 0;
                    i32 returnVal = 0;
            };
            struct FtellStruct
            {
                    CFile PTR stream = nullptr;
                    i32 called = 0;
                    i32 returnVal = 0;
            };
            struct FseekStruct
            {
                    CFile PTR stream = nullptr;
                    Offset offset = 0;
                    Origin origin = Origin::Beggining;
                    i32 called = 0;
                    i32 returnVal = 0;
            };
            struct RewindStruct
            {
                    CFile PTR stream = nullptr;
                    i32 called = 0;
                    i32 returnVal = 0;
            };
            struct FreadStruct
            {
                    void PTR ptrToData = nullptr;
                    Size elementSize = 0;
                    Size count = 0;
                    CFile PTR stream = nullptr;
                    i32 called = 0;
                    i32 returnVal = 0;
            };
            struct FwriteStruct
            {
                    const void PTR ptrToData = nullptr;
                    Size elementSize = 0;
                    Size count = 0;
                    CFile PTR stream = nullptr;
                    i32 called = 0;
                    i32 returnVal = 0;
            };
            struct SetvbufStruct
            {
                    CFile PTR stream = nullptr;
                    char8 PTR bufferPtr = nullptr;
                    Size bufferSize = 0;
                    BufferingMode mode = BufferingMode::Full;
                    i32 called = 0;
                    i32 returnVal = 0;
            };
            struct FflushStruct
            {
                    CFile PTR stream = nullptr;
                    i32 called = 0;
                    i32 returnVal = 0;
            };
            struct Tmpnam_sStruct
            {
                    const CharType * filename = nullptr;
                    Size size = 0;
                    i32 called = 0;
                    i32 returnVal = 0;
            };
            struct RenameStruct
            {
                    const CharType * oldFilename = nullptr;
                    const CharType * newFilename = nullptr;
                    i32 called = 0;
                    i32 returnVal = 0;
            };
            struct RemoveStruct
            {
                    const CharType * filename = nullptr;
                    i32 called = 0;
                    i32 returnVal = 0;
            };

            static FopenStruct FopenTest;
            static FreopenStruct FreopenTest;
            static FcloseStruct FcloseTest;
            static FeofStruct FeofTest;
            static FgetposStruct FgetposTest;
            static FsetposStruct FsetposTest;
            static FtellStruct FtellTest;
            static FseekStruct FseekTest;
            static RewindStruct RewindTest;
            static FreadStruct FreadTest;
            static FwriteStruct FwriteTest;
            static SetvbufStruct SetvbufTest;
            static FflushStruct FflushTest;
            static Tmpnam_sStruct Tmpnam_sTest;
            static RenameStruct RenameTest;
            static RemoveStruct RemoveTest;

        public:
            static auto Fopen(const char8 * filename, const CharType * mode)
            {
                FopenTest.filename = filename;
                FopenTest.mode = mode;
                FopenTest.called ++;

                return FopenTest.returnVal;
            }
            static auto Freopen(const CharType * filename, const CharType * mode)
            {
                FreopenTest.filename = filename;
                FreopenTest.mode = mode;
                FreopenTest.called ++;
                return FopenTest.returnVal;
            }
            static auto Fclose(CFile PTR stream)
            {
                FcloseTest.stream = stream;
                FcloseTest.called ++;
                return FopenTest.returnVal;
            }
            static auto Feof(CFile PTR stream)
            {
                FeofTest.stream = stream;
                FeofTest.called ++;
                return FeofTest.returnVal;
            }
            static auto Fgetpos(CFile PTR stream, Position REF pos)
            {
                FgetposTest.stream = stream;
                FgetposTest.pos = &pos;
                FgetposTest.called ++;
                pos = FgetposTest.setVal;

                return FgetposTest.returnVal;
            }
            static auto Fsetpos(CFile PTR stream, const Position REF pos)
            {
                FsetposTest.stream = stream;
                FsetposTest.pos = pos;
                FsetposTest.called ++;

                return FsetposTest.returnVal;
            }
            static auto Ftell(CFile PTR stream)
            {
                FtellTest.stream = stream;
                FtellTest.called ++;

                return FtellTest.returnVal;
            }
            static auto Fseek(CFile PTR stream, const Offset offset, const Origin origin)
            {
                FseekTest.stream = stream;
                FseekTest.offset = offset;
                FseekTest.origin = origin;
                FseekTest.called ++;

                return FseekTest.returnVal;
            }
            static auto Rewind(CFile PTR stream)
            {
                RewindTest.stream = stream;
                RewindTest.called ++;

                return RewindTest.returnVal;
            }
            static auto Fread(void PTR ptrToData, const Size elementSize, const Size count, CFile PTR stream)
            {
                FreadTest.stream = stream;
                FreadTest.ptrToData = ptrToData;
                FreadTest.elementSize = elementSize;
                FreadTest.count = count;
                FreadTest.called ++;

                return FreadTest.returnVal;
            }
            inline auto Fwrite(const void PTR ptrToData, const Size elementSize, const Size count, CFile PTR stream)
            {
                FwriteTest.stream = stream;
                FwriteTest.ptrToData = ptrToData;
                FwriteTest.elementSize = elementSize;
                FwriteTest.count = count;
                FwriteTest.called ++;

                return FwriteTest.returnVal;
            }
            static auto Setvbuf(CFile PTR stream, char8 PTR bufferPtr, const Size bufferSize, const BufferingMode mode)
            {
                SetvbufTest.stream = stream;
                SetvbufTest.bufferPtr = bufferPtr;
                SetvbufTest.bufferSize = bufferSize;
                SetvbufTest.mode = mode;
                SetvbufTest.called ++;

                return SetvbufTest.returnVal;
            }
            static auto Fflush(CFile PTR stream)
            {
                FflushTest.stream = stream;
                FflushTest.called ++;

                return FflushTest.returnVal;
            }
            static auto Tmpnam_s(CharType filename[], const Size size)
            {
                Tmpnam_sTest.filename = filename;
                Tmpnam_sTest.size = size;
                Tmpnam_sTest.called ++;

                return Tmpnam_sTest.returnVal;
            }
            static auto Rename(const CharType * oldFilename, const CharType * newFilename)
            {
                RenameTest.oldFilename = oldFilename;
                RenameTest.newFilename = newFilename;
                RenameTest.called ++;

                return RenameTest.returnVal;
            }
            static auto Remove(const CharType * filename)
            {
                RemoveTest.filename = filename;
                RemoveTest.called ++;

                return RemoveTest.returnVal;
            }

        public:
            static constexpr i32 ErrorVal = 1;
            static constexpr i32 SuccessVal = 0;
    };


    MockAdapter::FopenStruct    MockAdapter::FopenTest = FopenStruct();
    MockAdapter::FreopenStruct  MockAdapter::FreopenTest = FreopenStruct();
    MockAdapter::FcloseStruct   MockAdapter::FcloseTest = FcloseStruct();
    MockAdapter::FeofStruct     MockAdapter::FeofTest = FeofStruct();
    MockAdapter::FgetposStruct  MockAdapter::FgetposTest = FgetposStruct();
    MockAdapter::FsetposStruct  MockAdapter::FsetposTest = FsetposStruct();
    MockAdapter::FtellStruct    MockAdapter::FtellTest = FtellStruct();
    MockAdapter::FseekStruct    MockAdapter::FseekTest = FseekStruct();
    MockAdapter::RewindStruct   MockAdapter::RewindTest = RewindStruct();
    MockAdapter::FreadStruct    MockAdapter::FreadTest = FreadStruct();
    MockAdapter::FwriteStruct   MockAdapter::FwriteTest = FwriteStruct();
    MockAdapter::SetvbufStruct  MockAdapter::SetvbufTest = SetvbufStruct();
    MockAdapter::FflushStruct   MockAdapter::FflushTest = FflushStruct();
    MockAdapter::Tmpnam_sStruct MockAdapter::Tmpnam_sTest = Tmpnam_sStruct();
    MockAdapter::RenameStruct   MockAdapter::RenameTest = RenameStruct();
    MockAdapter::RemoveStruct   MockAdapter::RemoveTest = RemoveStruct();

    template<typename T>
    void ResetTest(T REF test)
    {
        test = T();
    }
}
#endif // MOCKADAPTOR_H
