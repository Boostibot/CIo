#ifndef NEWFILEMANAGER_H
#define NEWFILEMANAGER_H

#include "OpenMode.h"
#include "AuxiliaryTypes.h"

namespace cio::Internal
{
    //Class responsible for safe management of the FILE pointer
    template<typename Adapter>
    class UnadaptedFileManager
    {
        public:
            using ThisType      = UnadaptedFileManager;

        private:
            using RequiredActions = typename OpenMode::RequiredActions;

        public:
            using OsCharType    = typename Adapter::CharType;
            using OpenMode      = BasicOpenMode<OsCharType>;
            using OsString      = String<OsCharType>;
            using OsStringView  = StringView<OsCharType>;

        private:
            CFile PTR FilePtr = {nullptr};
            EnabledOperations Enabled = {EnabledOperations::Closed};

        public:
            UnadaptedFileManager() noexcept = default;

            UnadaptedFileManager(const OsStringView REF path, const OsStringView REF openMode) noexcept
            {
                OpenNew(path, openMode);
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

            explicit inline operator bool() const noexcept {return this->IsOpen();}

        public:
            ~UnadaptedFileManager() noexcept {Close();}

        public:
            [[nodiscard]] inline CFile * REF           GetCFile() noexcept {return FilePtr;}
            [[nodiscard]] inline const CFile *         GetCFile() const noexcept {return FilePtr;}
            [[nodiscard]] inline EnabledOperations REF GetEnabled() noexcept {return Enabled;}
            [[nodiscard]] inline EnabledOperations     GetEnabled() const noexcept {return Enabled;}

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

                FilePtr = Adapter::Fopen(path.data(), openMode.data());

                return IsOpen();
            }

        public:
            [[nodiscard]] inline bool IsOpen() const noexcept {return (FilePtr != nullptr);}
            [[nodiscard]] inline bool IsClosed() const noexcept {return (FilePtr == nullptr);}

        private:
            inline static bool CloseFile(CFile PTR filePtr) noexcept
            {
                if(filePtr == nullptr)
                    return true;

                return (Adapter::Fclose(filePtr) == 0);
            }

        public:
            bool Close() noexcept
            {
                bool returnStatus = ThisType::CloseFile(this->FilePtr);

                FilePtr = nullptr;
                Enabled = EnabledOperations::Closed;

                return returnStatus;
            }

    };
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
