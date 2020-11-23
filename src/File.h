#ifndef FILE_H
#define FILE_H

#include "UnsafeFile.h"
namespace CIo
{
    //Final class that should safely encapsulate all necessary c File
    // functionality. All other classes should be provided as member types
    // acessible through BasicFile::ClassName
    template<typename OsCharTypeArg>
    class BasicFile : public BasicUnsafeFile<OsCharTypeArg>
    {
        private:
            using ThisType          = BasicFile;

        public:
            using UnsafeFile        = BasicUnsafeFile<OsCharTypeArg>;
            using FileManager       = typename UnsafeFile::FileManager;

        public:
            using Size              = typename UnsafeFile::Size;
            using FileSize          = typename UnsafeFile::FileSize;
            using Position          = typename UnsafeFile::Position;
            using OriginPosition    = typename UnsafeFile::OriginPosition;
            using BufferingCode     = typename UnsafeFile::BufferingCode;
            using FileDescriptor    = typename UnsafeFile::FileDescriptor;
            using Stats             = typename UnsafeFile::Stats;

            using OpenMode            = typename UnsafeFile::OpenMode;
            using OpenModeFlag        = typename OpenMode::OpenModeFlag;
            using WindowsOpenModeFlag = typename OpenMode::WindowsOpenModeFlag;
            using COpenMode           = typename OpenMode::COpenMode;

            template<typename CharType>
            using String        = typename UnsafeFile:: template String<CharType>;
            template<typename CharType>
            using StringView    = typename UnsafeFile:: template StringView<CharType>;
            template<typename CharType>
            using CStringRef    = typename UnsafeFile:: template CStringRef<CharType>;

            using OsCharType    = typename UnsafeFile::OsCharType;
            using OsString      = typename UnsafeFile::OsString;
            using OsStringView  = typename UnsafeFile::OsStringView;
            using OsCStringRef  = typename UnsafeFile::OsCStringRef;

            static_assert (CIo::CharSupport<OsCharType>::IsValid, "Invalid OsCharType; Only char and wchar_t allowed; (No posix function takes other char types)");

        public:
            static constexpr Position ErrorPos = {-1};

        public:
            BasicFile() = default;
            BasicFile(const ThisType REF) = delete;
            BasicFile(ThisType RVALUE_REF) = default;
            ThisType REF operator=(const ThisType REF) = delete;
            ThisType REF operator=(ThisType RVALUE_REF) = default;

        public:
            inline BasicFile(const OsStringView path, const OpenMode REF openMode) : UnsafeFile(path, openMode)
            {}
            template<typename ... OpenModeTypes,
                     std::enable_if_t<OpenModeHelpers::AreOpenModeFlags<OpenModeTypes...>(), i32> = 0>
            inline BasicFile(const OsStringView path, OpenModeTypes ... openModes) noexcept : UnsafeFile(path, openModes...)
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
            Position GetPosInFile() noexcept
            {
                if(this->IsClosed())
                    return ThisType::ErrorPos;

                return this->GetUnsafe().GetPosInFile();
            }

            bool SetPosInFile(Position pos, OriginPosition from = OriginPosition::Beggining) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().SetPosInFile(pos, from);
            }
            void MoveToBeggining() noexcept
            {
                if(this->IsClosed())
                    return;

                this->GetUnsafe().MoveToBeggining();
            }
            void MoveToEnd() noexcept
            {
                if(this->IsClosed())
                    return;

                this->GetUnsafe().MoveToEnd();
            }
            bool MoveBy(Position by) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().MoveBy(by);
            }

        public:
            template<typename PointerType>
            [[nodiscard]] bool Read(PointerType PTR ptrToBuffer, Size count) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().Read(ptrToBuffer, count);
            }

            template<typename PointerType>
            [[nodiscard]] Size ReadAndCount(PointerType PTR ptrToBuffer, Size count) noexcept
            {
                if(this->IsClosed())
                    return 0;

                return this->GetUnsafe().ReadAndCount(ptrToBuffer, count);
            }

            template<typename ObjectType>
            [[nodiscard]] bool ReadObject(ObjectType RVALUE_REF object) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().ReadObject(object);
            }

            template<typename CharT>
            [[nodiscard]] bool ReadString(CStringRef<CharT> to) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().ReadString(to);
            }


        public:
            template<typename PointerType>
            [[nodiscard]] bool Write(PointerType PTR ptrToData, Size count) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().Write(ptrToData, count);
            }
            template<typename PointerType>
            [[nodiscard]] Size WriteAndCount(PointerType PTR ptrToData, Size count) noexcept
            {
                if(this->IsClosed())
                    return 0;

                return this->GetUnsafe().WriteAndCount(ptrToData, count);
            }
            template<typename ObjectType>
            [[nodiscard]] bool WriteObject(ObjectType RVALUE_REF object) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().WriteObject(object);
            }

            template <typename T,
                      std::enable_if_t<IsAnyString_v<T>, int> = 0>
            [[nodiscard]] bool WriteString(T RVALUE_REF str) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().WriteString(std::forward<T>(str));
            }


        public:
            bool SetBuffer(void PTR bufferPtr, Size bufferSize, BufferingCode mode) noexcept
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
            inline static bool GetFileStatics(Stats REF stats, const FileDescriptor descriptor) noexcept
            {
                if(descriptor == FileDescriptor::ErrorDescriptor)
                    return false;

                return UnsafeFile::GetFileStatics(stats, descriptor);
            }

            inline static bool GetFileStatics(Stats REF stats, const OsStringView filename)
            {
                return UnsafeFile::GetFileStatics(stats, filename);
            }

            static FileSize GetFileSize(const FileDescriptor descriptor) noexcept
            {
                if(descriptor == FileDescriptor::ErrorDescriptor)
                    return UnsafeFile::ErrorSize;

                return UnsafeFile::GetFileSize(descriptor);
            }

            static FileSize GetFileSize(const OsStringView filename) noexcept
            {
                return UnsafeFile::GetFileSize(filename);
            }


        public:
            FileDescriptor GetFileDescriptor() noexcept
            {
                if(this->IsClosed())
                    return FileDescriptor();

                return this->GetUnsafe().GetFileDescriptor();
            }

            bool GetFileStatics(Stats REF stats) const noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().GetFileStatics(stats);
            }

            FileSize GetFileSize() const noexcept
            {
                if(this->IsClosed())
                    return UnsafeFile::ErrorSize;

                return this->GetUnsafe().GetFileSize();
            }
    };

    using File          = BasicFile<char8>;
    using WFile         = BasicFile<charW>;
}
#endif // FILE_H
