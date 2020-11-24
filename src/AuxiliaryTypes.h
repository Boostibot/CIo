#ifndef AUXILIARYTYPES_H
#define AUXILIARYTYPES_H

#include "Common.h"
#include "CompilerSupport.h"

namespace cio
{
    using CStats          = CompilerSupport::StatType;
    using CFile           = std::FILE;
    using CFileDescriptor = i32;
    using COrigin         = i32;
    using CBufferingMode  = i32;

    using Position  = CompilerSupport::OffsetType;
    using Size      = size_t;
    using FileSize  = decltype (CStats::st_size);

    template<typename CharType>
    using String        = std::basic_string<CharType>;
    template<typename CharType>
    using StringView    = std::basic_string_view<CharType>;

    struct FileDescriptor
    {
            static constexpr CFileDescriptor ErrorDescriptor = {-1};

        private:
            #if defined (_MSC_VER)
            [[maybe_unused]]
            #endif
            const CFileDescriptor Descriptor = {ErrorDescriptor};

        public:
            constexpr FileDescriptor() noexcept = default;
            constexpr FileDescriptor(CFileDescriptor val) noexcept : Descriptor(val)
            {}
            constexpr inline operator CFileDescriptor() const noexcept
            {
                return Descriptor;
            }
            constexpr inline bool IsValid() const noexcept
            {
                return Descriptor > ErrorDescriptor;
            }
    };

    struct ConstFileDescriptor : public FileDescriptor
    {};

    struct Stats : public CStats
    {
            inline auto GroupIdOwningFile()         const noexcept {return st_gid;}
            inline auto UserIdOwningFile()          const noexcept {return st_uid;}
            inline auto TimeOfCreation()            const noexcept {return st_ctime;}
            inline auto TimeOfLastAccess()          const noexcept {return st_atime;}
            inline auto TimeOfLastModification()    const noexcept {return st_mtime;}
            inline auto DriveNumberOfDiskWithFile() const noexcept {return st_dev;}
            inline auto NumberOfInformationNodes()  const noexcept {return st_ino;}
            inline auto FileModeBitMask()           const noexcept {return st_mode;}
            inline auto NumberOfHardLinks()         const noexcept {return st_nlink;}
            inline auto Size()                      const noexcept {return st_size;}
    };

    enum class Origin : COrigin
    {
        Beggining = SEEK_SET,
        CurrentPosition = SEEK_CUR,
        End = SEEK_END
    };

    enum class BufferingMode : CBufferingMode
    {
        Full = _IOFBF,
        Line = _IOLBF,
        None = _IONBF
    };

}

#endif // AUXILIARYTYPES_H
