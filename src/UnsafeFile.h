#ifndef NEWFILE_H
#define NEWFILE_H


#include "UniversalString.h"
#include "FileManager.h"

namespace cio::Internal
{
    //A full wrapper class which should implemnt a wrapper function for all necessary C FILE functions
    //It should also ease the use of C FILE and be the base for more specific classes
    template<typename Adapter, typename FileManagerArg = UnadaptedFileManager<Adapter>>
    class UnadaptedUnsafeFile : public FileManagerArg
    {
        private:
            using ThisType          = UnadaptedUnsafeFile;
            using OsCharType        = typename Adapter::CharType;

        public:
            using FileManager       = FileManagerArg;

        private:
            using RequiredActions = typename OpenMode::RequiredActions;

        public:
            using OpenMode      = BasicOpenMode<OsCharType>;
            using OsString      = String<OsCharType>;
            using OsStringView  = StringView<OsCharType>;

        public:
            static constexpr auto EndOfFile              = EOF;
            static constexpr auto FileNameMaxLenght      = FILENAME_MAX;
            static constexpr auto OpenFilesMax           = FOPEN_MAX;
            static constexpr auto TempFileNameMaxLenght  = L_tmpnam;
            static constexpr auto TempFilesMax           = TMP_MAX;
            static constexpr Size DefaultBufferSize      = BUFSIZ;

        public:
            static constexpr FileSize ErrorSize         = {-1};

        private:
            using FileManager::GetCFile;
            using FileManager::GetEnabled;

        public:
            UnadaptedUnsafeFile() = default;
            UnadaptedUnsafeFile(const ThisType REF) = delete;
            UnadaptedUnsafeFile(ThisType RVALUE_REF) = default;
            ThisType REF operator=(const ThisType REF) = delete;
            ThisType REF operator=(ThisType RVALUE_REF) = default;

        public:
            inline UnadaptedUnsafeFile(const OsStringView path, const OpenMode REF openMode) noexcept
            {
                OpenNew(path, openMode);
            }

            template<typename ... OpenModeTypes,
                     std::enable_if_t<OpenModeHelpers::AreOpenModeFlags<OpenModeTypes...>(), i32> = 0>
            inline UnadaptedUnsafeFile(const OsStringView path, OpenModeTypes ... flags) noexcept
            {
                OpenNew(path, flags...);
            }

        public:
            ~UnadaptedUnsafeFile() = default;

        private:
            inline FileManager REF GetManager() noexcept
            {
                return static_cast<FileManager REF>(PTR_VAL(this));
            }
            inline const FileManager REF GetManager() const noexcept
            {
                return static_cast<const FileManager REF>(PTR_VAL(this));
            }

        public:
            bool OpenNew(const OsStringView path, const OpenMode REF openMode) noexcept
            {
                if(NOT openMode.IsValid())
                    return false;

                if(ExecuteRequiredBeforeActions(path, openMode.GetRequiredActions()) == false)
                    return false;

                GetManager().OpenNew(path, openMode);

                return ExecuteRequiredAfterActions(openMode.GetEnabledOperations(), openMode.GetRequiredActions());
            }

            template<typename ... OpenModeTypes,
                     std::enable_if_t<OpenModeHelpers::AreOpenModeFlags<OpenModeTypes...>(), i32> = 0>
            inline bool OpenNew(const OsStringView path, OpenModeTypes ... openModes) noexcept
            {
                return OpenNew(path, OpenMode(openModes...));
            }

        private:
            inline static bool ExecuteRequiredBeforeActions(const OsStringView path, const RequiredActions REF required) noexcept
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

            inline bool ExecuteRequiredAfterActions(EnabledOperations enabled, const RequiredActions REF required) noexcept
            {
                if(IsOpen())
                {
                    GetEnabled() = enabled;

                    if(required.StartAtEnd)
                        MoveToEnd();

                    return true;
                }
                else
                {
                    GetEnabled() = EnabledOperations::Closed;
                    return false;
                }
            }

        public:
            inline bool WasEndOfFileRieched() const noexcept //feof
            {
                return (feof(GetCFile()) != 0);
            }
            inline EnabledOperations GetEnabledOperations() const noexcept
            {
                return GetManager().GetEnabled();
            }
            inline EnabledOperations IsReadingEnabled() const noexcept
            {
                return GetEnabledOperations() == EnabledOperations::Read ||
                        GetEnabledOperations() == EnabledOperations::ReadWrite;
            }
            inline EnabledOperations IsWritingEnabled() const noexcept
            {
                return GetEnabledOperations() == EnabledOperations::Write ||
                        GetEnabledOperations() == EnabledOperations::ReadWrite;
            }

        public:
            inline Position GetPos() const noexcept //ftell
            {
                Position pos = Position();
                fgetpos(GetCFile(), pos);
                return pos;
            }

            inline bool SavePos(Position REF pos) const noexcept //fgetpos
            {
                return (fgetpos(GetCFile(), pos) == 0);
            }
            inline bool RestorePos(const Position REF pos) const noexcept //fsetpos
            {
                return (fsetpos(GetCFile(), pos) == 0);
            }

            inline Position GetOffset() const noexcept //ftell
            {
                return ftell(GetCFile());
            }
            inline bool MoveTo(const Offset offset, const Origin from = Origin::Beggining) noexcept //fseek
            {
                return (fseek(GetCFile(), offset, from) == 0);
            }
            inline void MoveToBeggining() noexcept //frewind
            {
                rewind(GetCFile());
            }
            inline void MoveToEnd() noexcept //fseek end
            {
                //No return statement - moving to the end should never fail
                // The fseek function only checks if the final position would be
                // negative
                MoveTo(0, Origin::End);
            }
            inline bool MoveBy(Offset offset) noexcept //fseek curr
            {
                return MoveTo(offset, Origin::CurrentPosition);
            }

        public:
            template<typename PointerType>
            [[nodiscard]] inline bool Read(PointerType PTR ptrToBuffer, const Size count) noexcept
            {
                return (ReadAndCount(ptrToBuffer, count) == count);
            }


            template<typename PointerType>
            [[nodiscard]] inline Size ReadAndCount(PointerType PTR ptrToBuffer, const Size count) noexcept //fread
            {
                return fread(ptrToBuffer, sizeof (PointerType), count, GetCFile());
            }


            template<typename ObjectType>
            [[nodiscard]] inline bool ReadObject(ObjectType REF object) noexcept
            {
                return Read(ADDRESS(object), 1);
            }

            template<typename CharT>
            [[nodiscard]] inline bool ReadString(String<CharT> REF output) noexcept
            {
                return Read(output.data(), output.size());
            }

        public:
            template<typename PointerType>
            [[nodiscard]] inline bool Write(const PointerType PTR const ptrToData, const Size count) noexcept
            {
                return (WriteAndCount(ptrToData, count) == count);
            }

            template<typename PointerType>
            [[nodiscard]] inline Size WriteAndCount(const PointerType PTR const ptrToData, const Size count) noexcept //fwrite
            {
                return fwrite(ptrToData, sizeof (PointerType), count, GetCFile());
            }

            template<typename ObjectType>
            [[nodiscard]] inline bool WriteObject(ObjectType RVALUE_REF object) noexcept
            {
                return Write(ADDRESS(object), 1);
            }

        private:
            template <typename CharT>
            [[nodiscard]] inline bool WriteStringImpl(const std::basic_string_view<CharT> str) noexcept
            {
                return Write(str.data(), str.size());
            }

        public:
            template <typename T,
                      std::enable_if_t<IsAnyString_v<T>, i32> = 0>
            [[nodiscard]] inline bool WriteString(T RVALUE_REF str)
            {
                using CharT = GetAnyStringType_t<T>;

                return WriteStringImpl<CharT>(str);
            }


        public:
            inline bool SetBuffer(void PTR bufferPtr, const Size bufferSize, const BufferingMode mode) noexcept //setvbuf
            {
                return (setvbuf(GetCFile(), static_cast<char PTR>(bufferPtr), static_cast<i32>(mode), bufferSize) == 0); //fflush
            }
            inline void Flush() noexcept //fflush
            {
                fflush(GetCFile());
            }
            inline void SwitchBetweenReadAndWrite() noexcept
            {
                MoveBy(0);
            }

        public:
            static bool GetUniqueTempFileName(OsCharType filename[], const Size filenameSize) noexcept //tmpnam_s
            {
                if(filenameSize < ThisType::TempFileNameMaxLenght)
                    return false;

                return (tmpnam_s(filename, filenameSize) == 0);
            }
            inline static bool GetUniqueTempFileName(OsString REF filename) noexcept //tmpnam_s
            {
                return GetUniqueTempFileName(filename.data(), filename.size());
            }

            static bool IsFileOpenable(const OsStringView REF filename) noexcept
            {
                FileManager file;
                return file.OpenNew(filename, "r");
            }

            static bool CreateFile(const OsStringView filename) noexcept
            {
                FileManager file;
                return file.OpenNew(filename, "a");
            }

            inline static bool RenameFile(const OsStringView oldFileName, const OsStringView newFileName) noexcept //rename
            {
                return (rename(oldFileName.data(), newFileName.data()) == 0);
            }

            inline static bool RemoveFile(const OsStringView fileName) noexcept //remove
            {
                return (remove(fileName.data()) == 0);
            }

            static FileSize GetFileSize(const OsStringView filename) noexcept
            {
                constexpr OpenMode mode(OpenModeFlag::Read);
                ThisType file(filename, mode);

                if(file.IsClosed())
                    return ThisType::ErrorSize;

                file.MoveToEnd();
                return static_cast<FileSize>(file.GetOffset());
            }

        public:
            inline FileSize GetFileSize() const noexcept
            {
                Position beforePos;
                SavePos(beforePos);

                MoveToEnd();

                const FileSize size = static_cast<FileSize>(GetOffset());
                RestorePos(beforePos);

                return size;
            }

    };
}
#endif // NEWFILE_H
