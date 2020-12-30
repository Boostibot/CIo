#ifndef FILE_H
#define FILE_H

#include "UnsafeFile.h"
namespace cio::Internal
{
    //Final class that should safely encapsulate all necessary c File
    // functionality. All other classes should be provided as member types
    // acessible through BasicFile::ClassName
    template<typename Adapter, typename UnsafeFileArg>
    class UnadaptedFile : public UnsafeFileArg
    {
        private:
            using ThisType          = UnadaptedFile;
            using OsCharType        = typename Adapter::CharType;

        public:
            using UnsafeFile        = UnsafeFileArg;
            using FileManager       = typename UnsafeFile::FileManager;

        public:
            using OpenMode      = BasicOpenMode<OsCharType>;
            using OsString      = String<OsCharType>;
            using OsStringView  = StringView<OsCharType>;

        public:
            static constexpr Offset ErrorPos          = {-1};

        public:
            UnadaptedFile() = default;
            UnadaptedFile(const ThisType REF) = delete;
            UnadaptedFile(ThisType RVALUE_REF) = default;
            ThisType REF operator=(const ThisType REF) = delete;
            ThisType REF operator=(ThisType RVALUE_REF) = default;

        public:
            inline UnadaptedFile(const OsStringView path, const OpenMode REF openMode) : UnsafeFile(path, openMode)
            {}
            template<typename ... OpenModeTypes,
                     std::enable_if_t<OpenModeHelpers::AreOpenModeFlags<OpenModeTypes...>(), i32> = 0>
            inline UnadaptedFile(const OsStringView path, OpenModeTypes ... openModes) noexcept : UnsafeFile(path, openModes...)
            {}

        public:
            [[nodiscard]] bool WasEndOfFileRieched() noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->UnsafeFileArg::WasEndOfFileRieched();
            }

        public:
            inline bool SavePos(Position REF pos) noexcept //fgetpos
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed: return false;
                    default:                     return UnsafeFileArg::SavePos(pos);
                }
            }
            inline bool RestorePos(const Position REF pos) noexcept //fsetpos
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed: return false;
                    default:                     return UnsafeFileArg::RestorePos(pos);
                }
            }

            [[nodiscard]] Offset GetOffset() noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed: return ThisType::ErrorPos;
                    default:                     return UnsafeFileArg::GetOffset();
                }
            }

            bool MoveTo(const Offset offset, const Origin from = Origin::Beggining) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed: return false;
                    default:                     return UnsafeFileArg::MoveTo(offset, from);
                }
            }
            void MoveToBeggining() noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed: return;
                    default:                     return UnsafeFileArg::MoveToBeggining();
                }
            }
            void MoveToEnd() noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed: return;
                    default:                     return UnsafeFileArg::MoveToEnd();
                }
            }
            bool MoveBy(Offset offset) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed: return false;
                    default:                     return UnsafeFileArg::MoveBy(offset);
                }
            }

        public:
            template<typename PointerType>
            bool Read(PointerType PTR ptrToBuffer, Size count) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed:
                    case EnabledOperations::None:
                    case EnabledOperations::Write:  return false;
                    default:                     return UnsafeFileArg::Read(ptrToBuffer, count);
                }
            }

            template<typename PointerType>
            Size ReadAndCount(PointerType PTR ptrToBuffer, Size count) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed:
                    case EnabledOperations::None:
                    case EnabledOperations::Write:  return 0;
                    default:                     return UnsafeFileArg::ReadAndCount(ptrToBuffer, count);
                }
            }

            template<typename ObjectType>
            bool ReadObject(ObjectType RVALUE_REF object) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed:
                    case EnabledOperations::None:
                    case EnabledOperations::Write:  return false;
                    default:                     return UnsafeFileArg::ReadObject(object);
                }
            }

            template<typename CharT>
            bool ReadString(String<CharT> REF output) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed:
                    case EnabledOperations::None:
                    case EnabledOperations::Write:  return false;
                    default:                     return UnsafeFileArg::ReadString(output);
                }
            }

        public:
            template<typename PointerType>
            bool Write(PointerType PTR ptrToData, Size count) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed:
                    case EnabledOperations::None:
                    case EnabledOperations::Read:  return false;
                    default:                    return UnsafeFileArg::Write(ptrToData, count);
                }
            }

            template<typename PointerType>
            Size WriteAndCount(PointerType PTR ptrToData, Size count) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed:
                    case EnabledOperations::None:
                    case EnabledOperations::Read:  return 0;
                    default:                    return UnsafeFileArg::WriteAndCount(ptrToData, count);
                }
            }

            template<typename ObjectType>
            bool WriteObject(ObjectType RVALUE_REF object) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed:
                    case EnabledOperations::None:
                    case EnabledOperations::Read:  return false;
                    default:                    return UnsafeFileArg::WriteObject(object);
                }
            }

            template <typename T,
                      std::enable_if_t<IsAnyString_v<T>, int> = 0>
            bool WriteString(T RVALUE_REF str) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed:
                    case EnabledOperations::None:
                    case EnabledOperations::Read:  return false;
                    default:                    return UnsafeFileArg::WriteString(std::forward(str));
                }
            }

        public:
            bool SetBuffer(void PTR bufferPtr, Size bufferSize, BufferingMode mode) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->UnsafeFileArg::SetBuffer(bufferPtr, bufferSize, mode);
            }
            void Flush() noexcept
            {
                if(this->IsClosed())
                    return;

                return this->UnsafeFileArg::Flush();
            }
            void ClearState() noexcept
            {
                if(this->IsClosed())
                    return;

                return this->UnsafeFileArg::ClearState();
            }

        public:
            [[nodiscard]] FileSize GetFileSize() noexcept
            {
                if(this->IsClosed())
                    return UnsafeFile::ErrorSize;

                return this->UnsafeFileArg::GetFileSize();
            }
    };
}
#endif // FILE_H
