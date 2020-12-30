#ifndef CSTDIOADAPTER_H
#define CSTDIOADAPTER_H

#include "OsSupport.h"
#include "File.h"

namespace cio
{
    struct CstdioAdapter
    {
            using CharType = char8;

        public:
            static inline auto Fopen(const CharType * filename, const CharType * mode)
            {
                return OsSupport::fopen(filename, mode);
            }
            static inline auto Freopen(const CharType * filename, const CharType * mode)
            {
                return OsSupport::fopen(filename, mode);
            }
            static inline auto Fclose(CFile PTR stream)
            {
                return std::fclose(stream);
            }
            static inline auto Feof(CFile PTR stream)
            {
                return std::feof(stream);
            }
            static inline auto Fgetpos(CFile PTR stream, Position REF pos)
            {
                return std::fgetpos(stream, &pos);
            }
            static inline auto Fsetpos(CFile PTR stream, const Position REF pos)
            {
                return std::fsetpos(stream, &pos);
            }
            static inline auto Ftell(CFile PTR stream)
            {
                return OsSupport::ftell(stream);
            }
            static inline auto Fseek(CFile PTR stream, const Offset offset, const Origin origin)
            {
                return OsSupport::fseek(stream, offset, static_cast<COrigin>(origin));
            }
            static inline auto Fewind(CFile PTR stream)
            {
                return std::rewind(stream);
            }
            static inline auto Fread(void PTR ptrToData, const Size elementSize, const Size count, CFile PTR stream)
            {
                return std::fread(ptrToData, elementSize, count, stream);
            }
            static inline auto Fwrite(const void PTR ptrToData, const Size elementSize, const Size count, CFile PTR stream)
            {
                return std::fwrite(ptrToData, elementSize, count, stream);
            }
            static inline auto Setvbuf(CFile PTR stream, char8 PTR bufferPtr, const Size bufferSize, const BufferingMode mode)
            {
                return std::setvbuf(stream, bufferPtr, static_cast<i32>(bufferSize), static_cast<CBufferingMode>(mode));
            }
            static inline auto Fflush(CFile PTR stream)
            {
                return std::fflush(stream);
            }
            static inline auto Tmpnam_s(CharType filename[], const Size size)
            {
                return ::tmpnam_s(filename, size);
            }
            static inline auto Rename(const CharType * oldFilename, const CharType * newFilename)
            {
                return std::rename(oldFilename, newFilename);
            }
            static inline auto Remove(const CharType * filename)
            {
                return std::remove(filename);
            }
    };

    using FileManager = Internal::UnadaptedFileManager<CstdioAdapter>;
    using UnsafeFile = Internal::UnadaptedUnsafeFile<CstdioAdapter, FileManager>;
    using File = Internal::UnadaptedFile<CstdioAdapter, UnsafeFile>;
}

#endif // CSTDIOADAPTER_H
