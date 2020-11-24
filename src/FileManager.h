#ifndef CFILEMANAGER_H
#define CFILEMANAGER_H

#include "CharSupport.h"
#include "UniversalString.h"
#include "OpenMode.h"
#include "CstdioAdapter.h"

namespace cio::Internal
{
    //Class responsible for safe management of the FILE pointer
    template<typename Adaptor>
    class UnadaptedFileManager
    {
        public:
            using OsCharType    = typename Adaptor::CharType;
            using ThisType      = UnadaptedFileManager;

        private:
            using CharSupport   = CharSupport<OsCharType>;

        public:
            using OpenMode      = BasicOpenMode<OsCharType>;
            using OsString      = String<OsCharType>;
            using OsStringView  = StringView<OsCharType>;

            static_assert (CharSupport::IsValid, "Invalid OsCharType; Only char and wchar_t allowed; (No posix function takes other char types)");

        private:
            CFile PTR FilePtr;

        public:
            UnadaptedFileManager() noexcept : FilePtr(nullptr)
            {}

            UnadaptedFileManager(const OsStringView path, const OpenMode REF openMode) noexcept : FilePtr(nullptr)
            {
                OpenNew(path, openMode);
            }

            template<typename ... OpenModeTypes,
                     std::enable_if_t<OpenModeHelpers::AreOpenModeFlags<OpenModeTypes...>(), i32> = 0>
            UnadaptedFileManager(const OsStringView path, OpenModeTypes ... openModes) noexcept : FilePtr(nullptr)
            {
                OpenNew(path, openModes...);
            }

        public:
            UnadaptedFileManager(const ThisType REF) = delete;
            UnadaptedFileManager(ThisType RVALUE_REF other) noexcept
            {
                this->FilePtr = other.FilePtr;
                other.FilePtr = nullptr;
            }

        public:
            ThisType REF operator=(const ThisType REF) = delete;
            ThisType REF operator=(ThisType RVALUE_REF other) noexcept
            {
                ThisType temp(std::move(other));
                this->Swap(temp);

                return PTR_VAL(this);
            }

            explicit inline operator bool() const noexcept
            {
                return this->IsOpen();
            }

        public:
            ~UnadaptedFileManager() noexcept
            {
                Close();
            }

        public:
            inline CFile * REF GetCFile() {return FilePtr;}

        public:
            inline void Swap(ThisType REF other) noexcept
            {
                std::swap(this->FilePtr, other.FilePtr);
            }
            inline friend void Swap(ThisType REF first, ThisType REF second) noexcept
            {
                first.Swap(second);
            }

        public:
            bool OpenNew(const OsStringView path, const OpenMode REF openMode) noexcept
            {
                if(NOT openMode.IsValid())
                    return false;

                //When opening with ReadWriteMustExist the file must be openable
                // else the behaviour is undefined -> explicit check if the file can eb opened
                if(openMode.GetCOpenMode() == COpenMode::ReadWriteMustExist)
                {
                    if(ThisType::IsFileOpenable(path) == false)
                        return false;
                }

                return OpenNewInternal(path, openMode);
            }

            template<typename ... OpenModeTypes,
                     std::enable_if_t<OpenModeHelpers::AreOpenModeFlags<OpenModeTypes...>(), i32> = 0>
            inline bool OpenNew(const OsStringView path, OpenModeTypes ... openModes) noexcept
            {
                return OpenNew(path, OpenMode(openModes...));
            }

        private:
            inline bool OpenNewInternal(const OsStringView path, const OsStringView REF openMode) noexcept
            {
                if(path.empty())
                    return false;

                ThisType::CloseFile(this->FilePtr);

                FilePtr = Adaptor::fopen(path.data(), openMode.data());

                return IsOpen();
            }


        public:
            inline bool IsOpen() const noexcept {return (FilePtr != nullptr);}
            inline bool IsClosed() const noexcept {return (FilePtr == nullptr);}

        private:
            inline static bool CloseFile(CFile PTR filePtr) noexcept
            {
                if(filePtr == nullptr)
                    return true;

                //returns if the closing happened successfully
                // (0 = success)
                return (Adaptor::fclose(filePtr) == 0);
            }
        public:
            static bool IsFileOpenable(const OsStringView REF filename) noexcept
            {
                constexpr OpenMode readMode = COpenMode::ReadMustExist;

                ThisType file;
                //OpenOpenNewInternal is slightly faster since it doenst check the openmode
                return file.OpenNewInternal(filename, readMode);
            }

        public:
            bool Close() noexcept
            {
                bool returnStatus = ThisType::CloseFile(this->FilePtr);

                this->FilePtr = nullptr;

                return returnStatus;
            }

    };
}

namespace cio
{

    template<typename OsCharType>
    using BasicFileManager = Internal::UnadaptedFileManager<CstdioAdapter<OsCharType>>;
}
namespace std
{
    template <typename OsCharT>
    void swap (cio::BasicFileManager<OsCharT> REF file1, cio::BasicFileManager<OsCharT> REF file2) noexcept
    {
        file1.Swap(file2);
    }
}
namespace cio
{
    using FileManager  = BasicFileManager<char8>;
    using WFileManager = BasicFileManager<charW>;
}
#endif // CFILEMANAGER_H
