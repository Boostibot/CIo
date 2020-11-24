#ifndef USEEXAMPLES_H
#define USEEXAMPLES_H

#include "File.h"

namespace cio::UseExamples
{
    //==============================================================================//
    //                                                                              //
    //  These are examples of simple writing, appending and reading functions.      //
    //  For detailed explanation see below.                                         //
    //  For detailed behaviour search for the name of the function in UnitTests     //
    //  in square brackets ie. [OpenNew]                                            //
    //                                                                              //
    //==============================================================================//

    inline bool WriteToFile(cio::File::OsStringView path, const char data[], size_t dataSize) noexcept
    {
        using namespace cio;

        constexpr File::OpenMode openMode(OpenModeFlag::Write, OpenModeFlag::Binary);
        static_assert (openMode.IsValid(), "Invalid arguments");

        File file(path, openMode);

        return file.Write(data, dataSize);
    }

    inline bool AppendToFile(cio::File::OsStringView path, const char data[], size_t dataSize) noexcept
    {
        using namespace cio;

        constexpr File::OpenMode openMode(OpenModeFlag::Write, OpenModeFlag::Append, OpenModeFlag::Binary);
        static_assert (openMode.IsValid(), "Invalid arguments");

        File file(path, openMode);

        return file.Write(data, dataSize);
    }

    template<typename Lambda>
    bool ReadFileByChunks(cio::File::OsStringView path, Lambda processChunk) noexcept
    {
        using namespace cio;
        constexpr size_t chunkSize = 4096;

        constexpr File::OpenMode openMode(OpenModeFlag::Read, OpenModeFlag::MustExist, OpenModeFlag::Binary);
        static_assert (openMode.IsValid(), "Invalid arguments");

        File file(path, openMode);
        char chunk[chunkSize];

        Size readSize;
        while(true)
        {
            readSize = file.ReadAndCount(chunk, chunkSize);

            if(readSize == 0)
                break;

            processChunk(chunk, readSize);
        }

        return file.WasEndOfFileRieched();
    }

    //Detailed explanations:
    template<typename ProcessChunk, typename UpdateProgress>
    bool ReadFileWithProgress(cio::File::OsStringView path, ProcessChunk processChunk, UpdateProgress updateProgress) noexcept
    {
        //Note that this function can be declared noexcept since all
        // functions in the File (BasicFile<>) class are noexcept

        using namespace cio;
        constexpr size_t chunkSize = 4096;

        //Create an open mode with the following flags
        // - it is declcared constexpr and as such the flags are
        //   converted to string at compile time
        constexpr File::OpenMode openMode(OpenModeFlag::Read, OpenModeFlag::Binary, OpenModeFlag::MustExist);
        //We can check at compile time if the arguments are valid
        //(ie. if it does not contain any errors such having both MustExist and MustNotExist flags)
        static_assert (openMode.IsValid(), "Invalid arguments");
        //We can also check if the current combination of arguments are supported
        // (ie. not invalid but unimplmented due to restriction by the cstdio)
        // (this happens when both MustExist and Append flags are present)
        static_assert (openMode.IsSupported(), "Unsupported arguments");

        //Open a file given the path and openMode
        File file(path, openMode);
        char chunk[chunkSize];

        //We check if the file was opened sucessfully
        if(file.IsClosed())
            return false;

        //We get the maximum position by moving to end and getting it
        file.MoveToEnd();
        const Position maxPos = file.GetPosInFile();
        //We restore the pos to the beggining afterwards
        file.MoveToBeggining();

        //Please note that maxPos does neccessarly have to correspond
        // with the file size; This can be the result of opening the file
        // in OpenModeFlag::Text or with OpenModeFlag::Unicode
        // To get the actual file size use file.GetFileSize()

        //We read the file repeatedly by using the ReadAndCount function and saving
        // the read size to readOfChunk;
        Size readSize;
        while(true)
        {
            //We save the size read to be able to check it if the reading
            // succeeded
            readSize = file.ReadAndCount(chunk, chunkSize);

            //Note that we could also use the Read(chunk, chunkSize) function
            // but it wouldnt alow for treading files that are not multiple of
            // 4096 (ie. the last incomplete chunk wouldnt be processed)

            //If no data was read indicates an error -> break from the loop
            if(readSize == 0)
                break;

            //We process the chunk using the passed function
            processChunk(chunk, readSize);

            //And update the progress using the passed function
            updateProgress(file.GetPosInFile(), maxPos);
        }


        //Since the reading stopped there must had been an error
        // We check if the error was end of file
        // -> if it was the file was read successfully
        // -> else there was another error and the reading failed
        if(file.WasEndOfFileRieched())
        {
            updateProgress(maxPos, maxPos); //Sets progress to 100%
            return true;
        }
        else
            return false;
    }

    template<typename Lambda>
    bool ReadFileFaster(cio::File::OsStringView path, Lambda processChunk) noexcept
    {
        using namespace cio;
        constexpr size_t chunkSize = 4096;

        constexpr File::OpenMode openMode(OpenModeFlag::Read, OpenModeFlag::Binary, OpenModeFlag::MustExist);
        static_assert (openMode.IsValid(), "Invalid arguments");

        //We use UnsafeFile instead of File
        // UnsafeFile behaves the same way as File
        // only doesnt check if the C FILE pointer
        // is null
        // (ie. using most of the function on unopen file
        //   will cause a SEGFAULT)

        File::UnsafeFile file;
        char chunk[chunkSize];

        //We need to check if the file was opened
        // since none of the function check in themselfs
        if(file.OpenNew(path, openMode) == false)
            return false;

        //We can also use: file.IsClosed() or file.IsOpen()

        //We set the buffering to None
        // (in this case the first two parameters dont matter)
        file.SetBuffer(nullptr, 0, BufferingMode::None);

        //We read the file normally and process the data
        Size readSize;
        while(true)
        {
            readSize = file.ReadAndCount(chunk, chunkSize);

            if(readSize == 0)
                break;

            processChunk(chunk, readSize);
        }

        //We check the last error and exit normally
        return file.WasEndOfFileRieched();
        //This is the same as the last checking of error
        // only in single line
    }

    template<typename Lambda>
    inline bool ReadDoublesBackwards(cio::File::OsStringView path, Lambda processUnit) noexcept
    {
        using namespace cio;

        constexpr File::OpenMode openMode(OpenModeFlag::Read, OpenModeFlag::Binary, OpenModeFlag::MustExist);
        static_assert (openMode.IsValid(), "Invalid arguments");

        File::UnsafeFile file;
        double unit;

        //We open the file normally
        if(file.OpenNew(path, openMode) == false)
            return false;

        //We move to the end of the file
        file.MoveToEnd();

        //We move negative (backwards) sizeof double to not
        // read past the file end
        file.MoveBy(- static_cast<Position>(sizeof (unit)));

        //Both of the above functons can be fused into the following one:
        //file.SetPosInFile(- static_cast<File::SizeType>(chunkSize), File::OriginPosition::End);
        //This is faster because the file is moved only once instead of twice

        //We read the file backwards by reading and moving
        // backwards repeatedly
        while(true)
        {
            if(file.ReadObject(unit) == false)
                return false;

            processUnit(unit);

            //We move backwards the sizeof double TWO TIMES
            // this is because the File::Read function automatically
            // moves after the read data
            // (this is cstdio restriction and can not be changed)
            if(file.MoveBy(- static_cast<Position>(sizeof (unit)) * 2) == false)
            {
                //This function fails only when moving to position which would be
                // negative (ie. before the start of the file)
                // As such if it returns false we can declare that the whole file
                // was read and break from the reading cycle
                break;
            }
        }

        return true;
    }

    inline bool WriteFileUtfStrings(cio::WFile::OsStringView path)
    {
        using namespace cio;


        //When working with files in UTF there are few important things to note:
        //
        // 1. ALWAYS OPEN IN BINARY MODE (see below for more explanation)
        // 2. Only char and wchar_t can be used to comunicate with the OS (opening, renaming, deleting files) (see below)
        // 3. When on windows open files with UnicodeEncoding flag (this skips the unicode BOM)
        //

        //IMPORTANT: even though these functions operate with strings they do not use
        // the cstdio string functions (fputs, fgets) as they cannot be used with other string types than
        // char and wchar_t string. fwrite and fread could specially handle '\n' and '\x1A' and thus change
        // the data read/written.
        // As such they might behave incorectly when the file not opened with the OpenModeFlag::Binary

        // OS interaction and utf
        //========================
        //
        // The cstdio library only uses utf8 (char) to interact with the OS but there are also
        // expansions for wchar_t support.
        //
        // Only one type can be used at a time as specified with BasicFile<char / wchar_t>.
        // The File class is just a template specialisation of BasicFile with the OsCharType
        // This type can be changed with the following defines that must be defined before including "File.h":
        //
        // If possible use the utf8 version. The utfWide (note that not necesarly the same as utf16) is present just for
        // Windows support which doesnt use utf8 for file system.

        // File function interacting with the OS are:
        // - File::File(...)
        // - File::OpenNew(...)
        // - File::GetUniqueTempFileName(...)
        // - File::CreateFile(...)
        // - File::RenameFile(...)
        // - File::RemoveFile(...)
        // - File::GetFileStatics(...) //Static overload version
        // - File::GetFileSize(...) //Static overload version

        using namespace cio;
        constexpr WFile::OpenMode openMode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Binary);

        //If we are on windows we can also use UnicodeEncoding flag which makes the file automatically skip the BOM mark
        //WFile::WindowsOpenModeFlag::UnicodeEncoding

        const std::basic_string<char> utf8Str      = "My msg";
        const std::basic_string<wchar_t> utfWStr   = L"My msg";
        const std::basic_string<char16_t> utf16Str = u"My msg";
        const std::basic_string<char32_t> utf32Str = U"My msg";

        const std::basic_string_view<char> utf8StrView = "My msg";
        const wchar_t * utfWCStr                      = L"My msg";

        WFile::UnsafeFile file;

        //We open the file normally
        if(file.OpenNew(path, openMode) == false)
            return false;

        //(Is used for silencing the [[nodiscard]] attribute)
        bool status;

        //We can use the WriteString function to write the string
        // the argument can be any string
        // (std::string, std::string_view, c style string, string literal)
        // All charcters of the string are written except the nulltermination character
        status = file.WriteString(utf8Str);
        status = file.WriteString(utfWStr);

        status = file.WriteString(utf16Str);
        status = file.WriteString(utf32Str);

        status = file.WriteString(utf8StrView);

        status = file.WriteString(utfWCStr);

        status = file.WriteString(U"My msg");


        //Note that these function simply wrap around the File::Write funcion
        // as such the following two lines are equivalent:
        status = file.WriteString(utf8Str);
        status = file.Write(utf8Str.data(), utf8Str.size());


        return status;
    }

}

//==============================================================================//
//                                                                              //
//      For further detailed behaviour search for the name of the               //
//      function in UnitTests in square brackets ie. [OpenNew]                  //
//                                                                              //
//==============================================================================//

#endif // USEEXAMPLES_H
