#ifndef CSTDIOADAPTER_H
#define CSTDIOADAPTER_H

#include "CharSupport.h"
#include "AuxiliaryTypes.h"

namespace cio
{
    template<typename CharT>
    struct CstdioAdapter
    {
            using CharType = CharT;

        private:
            using CharSupport = CharSupport<CharType>;

            static_assert (CharSupport::IsValid, "Invalid OsCharType; Only char and wchar_t allowed; (No posix function takes other char types)");

        public:
            static inline auto fopen(const CharType * filename, const CharType * mode)
            {
                return CharSupport::fopen(filename, mode);
            }
            static inline auto fclose(CFile PTR stream)
            {
                return std::fclose(stream);
            }
            static inline auto feof(CFile PTR stream)
            {
                return std::feof(stream);
            }
            static inline auto ftell(CFile PTR stream)
            {
                return std::ftell(stream);
            }
            static inline auto fseek(CFile PTR stream, const Position offset, const Origin origin)
            {
                return std::fseek(stream, offset, static_cast<COrigin>(origin));
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
            static inline auto setvbuf(CFile PTR stream, char PTR bufferPtr, const Size bufferSize, const BufferingMode mode)
            {
                return std::setvbuf(stream, bufferPtr, bufferSize, static_cast<CBufferingMode>(mode));
            }
            static inline auto fflush(CFile PTR stream)
            {
                return std::fflush(stream);
            }
            static inline auto tmpnam_s(const CharType filename[], const Size size)
            {
                return CharSupport::tmpnam_s(filename, size);
            }
            static inline auto rename(const CharType * oldFilename, const CharType * newFilename)
            {
                return CharSupport::rename(oldFilename, newFilename);
            }
            static inline auto remove(const CharType * filename)
            {
                return CharSupport::remove(filename);
            }
            static inline auto _fstat64(const FileDescriptor descriptor, Stats REF stats)
            {
                return ::_fstat64(descriptor, ADDRESS(stats));
            }
            static inline auto _stat64(const CharType * filename, Stats REF stats)
            {
                return CharSupport::_stat64(filename, stats);
            }
            static inline auto _filelength(const FileDescriptor descriptor)
            {
                return ::_filelength(descriptor);
            }
            static inline auto _fileno(CFile PTR stream)
            {
                return ::_fileno(stream);
            }

    };
}

#endif // CSTDIOADAPTER_H
