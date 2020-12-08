#ifndef CSTDIOADAPTER_H
#define CSTDIOADAPTER_H

#include "OsSupport.h"
#include "File.h"

namespace cio
{
    struct CstdioAdapter
    {
            using CharType = char8;

        protected:
            static inline auto fopen(const CharType * filename, const CharType * mode)
            {
                return OsSupport::fopen(filename, mode);
            }
            static inline auto freopen(const CharType * filename, const CharType * mode)
            {
                return OsSupport::fopen(filename, mode);
            }
            static inline auto fclose(CFile PTR stream)
            {
                return std::fclose(stream);
            }
            static inline auto feof(CFile PTR stream)
            {
                return std::feof(stream);
            }
            static inline auto fgetpos(CFile PTR stream, Position REF pos)
            {
                return std::fgetpos(stream, &pos);
            }
            static inline auto fsetpos(CFile PTR stream, const Position REF pos)
            {
                return std::fsetpos(stream, &pos);
            }
            static inline auto ftell(CFile PTR stream)
            {
                return OsSupport::ftell(stream);
            }
            static inline auto fseek(CFile PTR stream, const Offset offset, const Origin origin)
            {
                return OsSupport::fseek(stream, offset, static_cast<COrigin>(origin));
            }
            static inline auto rewind(CFile PTR stream)
            {
                return std::rewind(stream);
            }
            static inline auto fread(void PTR ptrToData, const Size elementSize, const Size count, CFile PTR stream)
            {
                return std::fread(ptrToData, elementSize, count, stream);
            }
            static inline auto fwrite(const void PTR ptrToData, const Size elementSize, const Size count, CFile PTR stream)
            {
                return std::fwrite(ptrToData, elementSize, count, stream);
            }
            static inline auto setvbuf(CFile PTR stream, char8 PTR bufferPtr, const Size bufferSize, const BufferingMode mode)
            {
                return std::setvbuf(stream, bufferPtr, static_cast<i32>(bufferSize), static_cast<CBufferingMode>(mode));
            }
            static inline auto fflush(CFile PTR stream)
            {
                return std::fflush(stream);
            }
            static inline auto tmpnam_s(CharType filename[], const Size size)
            {
                return ::tmpnam_s(filename, size);
            }
            static inline auto rename(const CharType * oldFilename, const CharType * newFilename)
            {
                return std::rename(oldFilename, newFilename);
            }
            static inline auto remove(const CharType * filename)
            {
                return std::remove(filename);
            }
    };

    using FileManager = Internal::UnadaptedFileManager<CstdioAdapter>;
    using UnsafeFile = Internal::UnadaptedUnsafeFile<CstdioAdapter>;
    using File = Internal::UnadaptedFile<CstdioAdapter>;
}

#endif // CSTDIOADAPTER_H
