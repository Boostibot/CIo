#ifndef AUXILIARYTYPES_H
#define AUXILIARYTYPES_H

#include <string>
#include <string_view>

#include "Common.h"
#include "OsSupport.h"

namespace cio
{
    using CFile           = std::FILE;
    using CFileDescriptor = i32;
    using COrigin         = i32;
    using CBufferingMode  = i32;

    using Offset    = CompilerSupport::OffsetType;
    using Position  = std::fpos_t;
    using Size      = std::size_t;
    using FileSize  = Offset;

    template<typename CharType>
    using String        = std::basic_string<CharType>;
    template<typename CharType>
    using StringView    = std::basic_string_view<CharType>;

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
