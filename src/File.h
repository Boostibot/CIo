#ifndef FILE_H
#define FILE_H

#include "UnsafeFile.h"
namespace cio::Internal
{
    //Final class that should safely encapsulate all necessary c File
    // functionality. All other classes should be provided as member types
    // acessible through BasicFile::ClassName
    template<typename Adapter, typename UnsafeFileArg = UnadaptedUnsafeFile<Adapter>>
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

        private:
            inline UnsafeFile REF GetUnsafe() noexcept
            {
                return static_cast<UnsafeFile REF>(PTR_VAL(this));
            }
            inline const UnsafeFile REF GetUnsafe() const noexcept
            {
                return static_cast<const UnsafeFile REF>(PTR_VAL(this));
            }

        public:
            bool WasEndOfFileRieched() const noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().WasEndOfFileRieched();
            }

        public:
            inline Position GetPos() const noexcept //ftell
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed: return Position();
                    default:                     return GetUnsafe().GetPos();
                }
            }

            inline bool SavePos(Position REF pos) const noexcept //fgetpos
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed: return false;
                    default:                     return GetUnsafe().SavePos(pos);
                }
            }
            inline bool RestorePos(const Position REF pos) const noexcept //fsetpos
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed: return false;
                    default:                     return GetUnsafe().RestorePos(pos);
                }
            }

            Offset GetOffset() noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed: return ThisType::ErrorPos;
                    default:                     return GetUnsafe().GetOffset();
                }
            }

            bool MoveTo(const Offset offset, const Origin from = Origin::Beggining) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed: return false;
                    default:                     return GetUnsafe().MoveTo(offset, from);
                }
            }
            void MoveToBeggining() noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed: return;
                    default:                     return GetUnsafe().MoveToBeggining();
                }
            }
            void MoveToEnd() noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed: return;
                    default:                     return GetUnsafe().MoveToEnd();
                }
            }
            bool MoveBy(Offset offset) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed: return false;
                    default:                     return GetUnsafe().MoveBy(offset);
                }
            }

        public:
            template<typename PointerType>
            [[nodiscard]] bool Read(PointerType PTR ptrToBuffer, Size count) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed:
                    case EnabledOperations::None:
                    case EnabledOperations::Write:  return false;
                    default:                     return GetUnsafe().Read(ptrToBuffer, count);
                }
            }

            template<typename PointerType>
            [[nodiscard]] Size ReadAndCount(PointerType PTR ptrToBuffer, Size count) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed:
                    case EnabledOperations::None:
                    case EnabledOperations::Write:  return 0;
                    default:                     return GetUnsafe().ReadAndCount(ptrToBuffer, count);
                }
            }

            template<typename ObjectType>
            [[nodiscard]] bool ReadObject(ObjectType RVALUE_REF object) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed:
                    case EnabledOperations::None:
                    case EnabledOperations::Write:  return false;
                    default:                     return GetUnsafe().ReadObject(object);
                }
            }

            template<typename CharT>
            [[nodiscard]] inline bool ReadString(String<CharT> REF output) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed:
                    case EnabledOperations::None:
                    case EnabledOperations::Write:  return false;
                    default:                     return GetUnsafe().ReadString(output);
                }
            }


        public:
            template<typename PointerType>
            [[nodiscard]] bool Write(PointerType PTR ptrToData, Size count) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed:
                    case EnabledOperations::None:
                    case EnabledOperations::Read:  return false;
                    default:                    return GetUnsafe().Write(ptrToData, count);
                }
            }
            template<typename PointerType>
            [[nodiscard]] Size WriteAndCount(PointerType PTR ptrToData, Size count) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed:
                    case EnabledOperations::None:
                    case EnabledOperations::Read:  return 0;
                    default:                    return GetUnsafe().WriteAndCount(ptrToData, count);
                }
            }
            template<typename ObjectType>
            [[nodiscard]] bool WriteObject(ObjectType RVALUE_REF object) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed:
                    case EnabledOperations::None:
                    case EnabledOperations::Read:  return false;
                    default:                    return GetUnsafe().WriteObject(object);
                }
            }

            template <typename T,
                      std::enable_if_t<IsAnyString_v<T>, int> = 0>
            [[nodiscard]] bool WriteString(T RVALUE_REF str) noexcept
            {
                switch (GetEnabled())
                {
                    case EnabledOperations::Closed:
                    case EnabledOperations::None:
                    case EnabledOperations::Read:  return false;
                    default:                    return GetUnsafe().WriteString(std::forward(str));
                }
            }


        public:
            bool SetBuffer(void PTR bufferPtr, Size bufferSize, BufferingMode mode) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().SetBuffer(bufferPtr, bufferSize, mode);
            }
            void Flush() noexcept
            {
                if(this->IsClosed())
                    return;

                return this->GetUnsafe().Flush();
            }
            void SwitchBetweenReadAndWrite() noexcept
            {
                if(this->IsClosed())
                    return;

                return this->GetUnsafe().SwitchBetweenReadAndWrite();
            }

        public:
            FileSize GetFileSize() const noexcept
            {
                if(this->IsClosed())
                    return UnsafeFile::ErrorSize;

                return this->GetUnsafe().GetFileSize();
            }
    };
}
#endif // FILE_H
