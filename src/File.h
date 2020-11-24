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

            static_assert (CharSupport<OsCharType>::IsValid, "Invalid OsCharType; Only char and wchar_t allowed; (No posix function takes other char types)");

        public:
            static constexpr Position ErrorPos          = {-1};

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
            Position GetPosInFile() noexcept
            {
                if(this->IsClosed())
                    return ThisType::ErrorPos;

                return this->GetUnsafe().GetPosInFile();
            }

            bool SetPosInFile(const Position pos, const Origin from = Origin::Beggining) noexcept
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
            [[nodiscard]] inline bool ReadString(String<CharT> REF output) noexcept
            {
                if(this->IsClosed())
                    return false;

                return this->GetUnsafe().ReadString(output);
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
            inline static bool GetFileStatics(Stats REF stats, const ConstFileDescriptor descriptor) noexcept
            {
                if(descriptor == FileDescriptor::ErrorDescriptor)
                    return false;

                return UnsafeFile::GetFileStatics(stats, descriptor);
            }

            inline static bool GetFileStatics(Stats REF stats, const OsStringView filename)
            {
                return UnsafeFile::GetFileStatics(stats, filename);
            }

            static FileSize GetFileSize(const ConstFileDescriptor descriptor) noexcept
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
            ConstFileDescriptor GetFileDescriptor() const noexcept
            {
                if(this->IsClosed())
                    return ConstFileDescriptor();

                return this->GetUnsafe().GetFileDescriptor();
            }
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
}
namespace cio
{
    template<typename CharType>
    using BasicFile     = Internal::UnadaptedFile<CstdioAdapter<CharType>>;
    using File          = BasicFile<char8>;
    using WFile         = BasicFile<charW>;
}
#endif // FILE_H
