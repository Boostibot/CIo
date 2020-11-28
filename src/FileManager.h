#ifndef NEWFILEMANAGER_H
#define NEWFILEMANAGER_H

#include "OpenMode.h"
#include "CstdioAdapter.h"

namespace cio::Internal
{
    //Class responsible for safe management of the FILE pointer
    template<typename Adapter>
    class UnadaptedFileManager
    {
        public:
            using OsCharType    = typename Adapter::CharType;
            using ThisType      = UnadaptedFileManager;

        private:
            using RequiredActions = typename OpenMode::RequiredActions;

        public:
            using OpenMode      = BasicOpenMode<OsCharType>;
            using OsString      = String<OsCharType>;
            using OsStringView  = StringView<OsCharType>;

        private:
            CFile PTR FilePtr = {nullptr};
            EnabledActions Enabled = {EnabledActions::Closed};

        public:
            UnadaptedFileManager() noexcept = default;

            UnadaptedFileManager(const OsStringView REF path, const OsStringView REF openMode) noexcept
            {
                OpenNew(path, openMode);
            }
            /*
            UnadaptedFileManager(const OsStringView path, const OpenMode REF openMode) noexcept : FilePtr(), Enabled()
            {
                OpenNew(path, openMode);
            }

            template<typename ... OpenModeTypes,
                     std::enable_if_t<OpenModeHelpers::AreOpenModeFlags<OpenModeTypes...>(), i32> = 0>
            UnadaptedFileManager(const OsStringView path, OpenModeTypes ... openModes) noexcept : FilePtr(), Enabled()
            {
                OpenNew(path, openModes...);
            }
            */

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

            explicit inline operator bool() const noexcept {return this->IsOpen();}

        public:
            ~UnadaptedFileManager() noexcept {Close();}

        public:
            inline CFile * REF        GetCFile()   {return FilePtr;}
            inline EnabledActions REF GetEnabled() {return Enabled;}

        public:
            inline void Swap(ThisType REF other) noexcept
            {
                std::swap(this->FilePtr, other.FilePtr);
                std::swap(this->Enabled, other.Enabled);
            }
            inline friend void Swap(ThisType REF first, ThisType REF second) noexcept
            {
                first.Swap(second);
            }

        public:
            inline bool OpenNew(const OsStringView REF path, const OsStringView REF openMode) noexcept
            {
                ThisType::CloseFile(this->FilePtr);

                FilePtr = Adapter::fopen(path.data(), openMode.data());

                return IsOpen();
            }

            /*
            bool OpenNew(const OsStringView path, const OpenMode REF openMode) noexcept
            {
                if(NOT openMode.IsValid())
                    return false;

                if(ExecuteRequiredActions(path, openMode.GetRequiredActions()) == false)
                    return false;

                OpenNewInternal(path, openMode);

                return SetEnabledIfOpened(openMode.GetEnabledActions());
            }

            template<typename ... OpenModeTypes,
                     std::enable_if_t<OpenModeHelpers::AreOpenModeFlags<OpenModeTypes...>(), i32> = 0>
            inline bool OpenNew(const OsStringView path, OpenModeTypes ... openModes) noexcept
            {
                return OpenNew(path, OpenMode(openModes...));
            }

        private:
            inline static bool ExecuteRequiredActions(const OsStringView path, const RequiredActions REF required) noexcept
            {
                if(required.MustExist)
                    if(NOT ThisType::IsFileOpenable(path))
                        return false;

                if(required.MustNotExist)
                    if(ThisType::IsFileOpenable(path))
                        return false;

                if(required.Delete)
                    ThisType::RemoveFile(path);

                if(required.Create)
                    ThisType::CreateFile(path);

                return true;
            }

            inline bool SetEnabledIfOpened(EnabledActions enabled) noexcept
            {
                if(IsOpen())
                {
                    Enabled = enabled;
                    return true;
                }
                else
                {
                    Enabled = EnabledActions::Closed;
                    return false;
                }
            }
            */


        public:
            inline bool IsOpen() const noexcept {return (FilePtr != nullptr);}
            inline bool IsClosed() const noexcept {return (FilePtr == nullptr);}

            /*
        public:
            static bool IsFileOpenable(const OsStringView REF filename) noexcept
            {
                //constexpr OpenMode readMode = COpenMode::Read;
                //ThisType file;
                //return file.OpenNewInternal(filename, readMode);

                ThisType file;
                return file.OpenNew(filename, "r");
            }

            inline static bool CreateFile(const OsStringView filename) noexcept
            {
                //constexpr OpenMode appendMode = COpenMode::Append;
                //ThisType file;
                //return file.OpenNewInternal(filename, appendMode);

                ThisType file;
                return file.OpenNew(filename, "a");
            }

            inline static bool RenameFile(const OsStringView oldFileName, const OsStringView newFileName) noexcept //rename
            {
                return (Adapter::rename(oldFileName.data(), newFileName.data()) == 0);
            }

            inline static bool RemoveFile(const OsStringView fileName) noexcept //remove
            {
                return (Adapter::remove(fileName.data()) == 0);
            }
            */


        private:
            inline static bool CloseFile(CFile PTR filePtr) noexcept
            {
                if(filePtr == nullptr)
                    return true;

                return (Adapter::fclose(filePtr) == 0);
            }

        public:
            bool Close() noexcept
            {
                bool returnStatus = ThisType::CloseFile(this->FilePtr);

                FilePtr = nullptr;
                Enabled = EnabledActions::Closed;

                return returnStatus;
            }

    };
}

namespace cio
{
    using BasicFileManager = Internal::UnadaptedFileManager<CstdioAdapter>;
}
/*
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
*/
#endif // NEWFILEMANAGER_H
