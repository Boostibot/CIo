#ifndef NEWOPENMODE_H
#define NEWOPENMODE_H

#include "AuxiliaryTypes.h"
#include "ConstexprString.h"

namespace cio
{
    namespace Meta
    {
        enum class Indetifier : u8 {Indentify};

        template<Indetifier identifier>
        constexpr static bool AndInternal()
        {
            return true;
        }
        template<Indetifier identifier, typename FirstType, typename ... OtherArgumentTypes>
        constexpr static bool AndInternal(FirstType firstValue, OtherArgumentTypes... values)
        {
            return firstValue && AndInternal<identifier>(values...);
        }
        template<typename ... ArgumentTypes>
        constexpr bool And(ArgumentTypes... values)
        {
            return AndInternal<Indetifier::Indentify, ArgumentTypes...>(values...);
        }
    }

    namespace  OpenModeHelpers
    {
        using OpenModeEnumsBaseType = u8;
    }

    //These are all of the open mode flags necessary to represent all
    // c standard compilant open mode strings
    enum class OpenModeFlag : OpenModeHelpers::OpenModeEnumsBaseType
    {
        Read,
        Write,
        Append,
        MustExist,

        Binary,
        MustNotExist


        //***************************************//
        //    OpenModeFlagsCount needs to be     //
        //  updated to match the actual number   //
        //             of options                //
        //***************************************//
        //This is to not bother the user with the count option
    };
    //These are all of the open mode flags necessary to represent all
    // visual c++ additional open mode symbols
    enum class WindowsOpenModeFlag : OpenModeHelpers::OpenModeEnumsBaseType
    {
        Text,

        CommitDirectlyToDisk,
        CommitIndirectlyToDisk,

        NotInheritedByChildProcess,

        SequntialAccessOptimized,
        RandomAccessOptimized,

        IfPossibleNoFlushingToDisk,

        DeleteAfterClose,

        UnicodeEncoding,
        Utf8Encoding,
        Utf16Encoding

        //***************************************//
        // WindowsOpenModeFlagsCount needs to be //
        //  updated to match the actual number   //
        //             of options                //
        //***************************************//
        //This is to not bother the user with the count option
    };
    //These are all of the basic modes supported by fopen
    enum class COpenMode : OpenModeHelpers::OpenModeEnumsBaseType
    {
        ReadMustExist,      //Read && MustExist
        Write,              //Write
        WriteAppend,        //Write && Append
        ReadWriteMustExist, //Read && Write && MustExist
        ReadWrite,          //Read && Write
        ReadWriteAppend     //Read && Write && Append
    };

    namespace  OpenModeHelpers
    {
            static constexpr OpenModeEnumsBaseType OpenModeFlagsCount = 6;
            static constexpr OpenModeEnumsBaseType WindowsOpenModeFlagsCount = 11;

            enum class OpenModeValidity : OpenModeEnumsBaseType
            {
                Valid,              //Arguemnts are valid
                Invalid,            //Arguments are invalid. ie. both Read and Write are missing
                Unsupported,        //Arguments that cannot be parsed due to fopen argument limitations
                                    // ie. all combinations containing Append && MustExist
            };

            template <typename FlagType>
            constexpr bool IsTypeOpenModeFlag()
            {
                return std::is_same_v<FlagType, OpenModeFlag> || std::is_same_v<FlagType, WindowsOpenModeFlag>;
            }

            template <typename ... FlagTypes>
            constexpr bool AreOpenModeFlags()
            {
                return Meta::And(IsTypeOpenModeFlag<FlagTypes>()...);
            }

            class FlagPresence
            {
                public:
                    using ThisType = FlagPresence;
                    using Underlying = OpenModeEnumsBaseType;

                public:
                    static constexpr Underlying WindowsFlagsOffset = OpenModeFlagsCount;
                    static constexpr Underlying TotalFlagsCount = OpenModeFlagsCount + WindowsOpenModeFlagsCount;

                private:
                    bool Presence[TotalFlagsCount] = {};

                public:
                    template<typename ... FlagTypes,
                             std::enable_if_t<OpenModeHelpers::AreOpenModeFlags<FlagTypes...>(), u32> = 0>
                    constexpr FlagPresence(const FlagTypes ... flags) : Presence()
                    {
                        FillFlagPresence<FlagTypes...>(flags...);
                    }

                public:
                    constexpr bool operator[](OpenModeFlag flag) const
                    {
                        return Presence[ThisType::ToIndex(flag)];
                    }
                    constexpr bool operator[](WindowsOpenModeFlag flag) const
                    {
                        return Presence[ThisType::ToIndex(flag)];
                    }

                private:
                    static constexpr Underlying ToIndex(OpenModeFlag flag) noexcept
                    {
                        return static_cast<Underlying>(flag);
                    }
                    static constexpr Underlying ToIndex(WindowsOpenModeFlag flag) noexcept
                    {
                        return static_cast<Underlying>(flag) + WindowsFlagsOffset;
                    }

                private:
                    template<typename ... FlagTypes>
                    constexpr void FillFlagPresence(const FlagTypes ... flags) noexcept
                    {
                        for(Underlying i = 0; i < TotalFlagsCount; i++)
                            Presence[i] = ThisType::IsFlagPresentInternal<Meta::Indetifier::Indentify, FlagTypes...>(i, flags...);
                    }

                public:
                    template<typename ... FlagTypes>
                    static constexpr bool IsFlagPresent(const OpenModeFlag lookingForFlag, const FlagTypes ... flags) noexcept
                    {
                        return ThisType::IsFlagPresentInternal<Meta::Indetifier::Indentify, FlagTypes...>(ThisType::ToIndex(lookingForFlag), flags...);
                    }
                    template<typename ... FlagTypes>
                    static constexpr bool IsFlagPresent(const WindowsOpenModeFlag lookingForFlag, const FlagTypes ... flags) noexcept
                    {
                        return ThisType::IsFlagPresentInternal<Meta::Indetifier::Indentify, FlagTypes...>(ThisType::ToIndex(lookingForFlag), flags...);
                    }

                private:
                    template<Meta::Indetifier identifier>
                    static constexpr bool IsFlagPresentInternal(const Underlying) noexcept {return false;}

                    template<Meta::Indetifier identifier, typename FirstOpneModeType, typename ... FlagTypes>
                    static constexpr bool IsFlagPresentInternal(const Underlying lookingForFlag, const FirstOpneModeType firstOpenMode, const FlagTypes ... flags) noexcept
                    {
                        const Underlying firstModeIndex = ThisType::ToIndex(firstOpenMode);

                        if(lookingForFlag == firstModeIndex)
                            return true;
                        else
                            return ThisType::IsFlagPresentInternal<identifier, FlagTypes...>(lookingForFlag, flags...);
                    }
            };

            template<typename OsCharT>
            class OpenModeConversionState
            {
                public:
                    using ThisType = OpenModeConversionState;

                public:
                    static constexpr u32 OpenModeMaxChars = 26;
                    using OsCharType = OsCharT;

                    template<typename CharT>
                    using ConstexprString = ConstexprString<CharT, OpenModeMaxChars>;
                    using OpenModeString = ConstexprString<OsCharType>;


                public:
                    const FlagPresence Presence;

                public:
                    template<typename ... FlagTypes>
                    constexpr OpenModeConversionState(const FlagTypes ... flags) : Presence(flags...)
                    {}

                public:
                    constexpr bool AreFlagsConflicting() const noexcept
                    {
                        if(Presence[OpenModeFlag::MustExist] && Presence[OpenModeFlag::MustNotExist])
                            return true;

                        if(Presence[WindowsOpenModeFlag::Text] && Presence[OpenModeFlag::Binary])
                            return true;

                        if(Presence[WindowsOpenModeFlag::CommitDirectlyToDisk] && Presence[WindowsOpenModeFlag::CommitIndirectlyToDisk])
                            return true;

                        if(Presence[WindowsOpenModeFlag::SequntialAccessOptimized] && Presence[WindowsOpenModeFlag::RandomAccessOptimized])
                            return true;

                        //Encodings
                        if(Presence[WindowsOpenModeFlag::UnicodeEncoding] && Presence[WindowsOpenModeFlag::Utf8Encoding])
                            return true;

                        if(Presence[WindowsOpenModeFlag::Utf16Encoding] && Presence[WindowsOpenModeFlag::Utf8Encoding])
                            return true;

                        if(Presence[WindowsOpenModeFlag::UnicodeEncoding] && Presence[WindowsOpenModeFlag::Utf16Encoding])
                            return true;

                        return false;
                    }

                    constexpr void SetCOpenModeAndValidty(COpenMode REF mode, OpenModeValidity REF validity) const noexcept
                    {
                        const bool read         = Presence[OpenModeFlag::Read];
                        const bool write        = Presence[OpenModeFlag::Write];
                        const bool append       = Presence[OpenModeFlag::Append];
                        const bool mustExist    = Presence[OpenModeFlag::MustExist];

                        auto setValidity = [REF mode, REF validity](OpenModeValidity _validity)
                        {
                            validity = _validity;
                            mode = COpenMode::ReadWrite;
                        };
                        auto setMode = [REF mode, REF validity](COpenMode _mode)
                        {
                            validity = OpenModeValidity::Valid;
                            mode = _mode;
                        };

                        struct Helper
                        {
                                static constexpr u32 Binary(u8 a, u8 b, u8 c, u8 d) {return 8*a + 4*b + 2*c + 1*d;}
                        };

                        switch (Helper::Binary(read, write, append, mustExist))
                        {
                            //                  R  W  A  M
                            case Helper::Binary(0, 0, 0, 0): setValidity(OpenModeValidity::Invalid); break;
                            case Helper::Binary(0, 0, 0, 1): setValidity(OpenModeValidity::Invalid); break;
                            case Helper::Binary(0, 0, 1, 0): setValidity(OpenModeValidity::Invalid); break;
                            case Helper::Binary(0, 0, 1, 1): setValidity(OpenModeValidity::Invalid); break;
                            case Helper::Binary(0, 1, 0, 0): setMode(COpenMode::Write); break;
                            case Helper::Binary(0, 1, 0, 1): setMode(COpenMode::ReadWriteMustExist); break;
                            case Helper::Binary(0, 1, 1, 0): setMode(COpenMode::WriteAppend); break;
                            case Helper::Binary(0, 1, 1, 1): setValidity(OpenModeValidity::Unsupported); break;
                            case Helper::Binary(1, 0, 0, 0): setMode(COpenMode::ReadWrite); break;
                            case Helper::Binary(1, 0, 0, 1): setMode(COpenMode::ReadMustExist); break;
                            case Helper::Binary(1, 0, 1, 0): setMode(COpenMode::ReadWriteAppend); break;
                            case Helper::Binary(1, 0, 1, 1): setValidity(OpenModeValidity::Unsupported); break;
                            case Helper::Binary(1, 1, 0, 0): setMode(COpenMode::ReadWrite); break;
                            case Helper::Binary(1, 1, 0, 1): setMode(COpenMode::ReadWriteMustExist); break;
                            case Helper::Binary(1, 1, 1, 0): setMode(COpenMode::ReadWriteAppend); break;
                            case Helper::Binary(1, 1, 1, 1): setValidity(OpenModeValidity::Unsupported); break;

                            default: setValidity(OpenModeValidity::Invalid); break;
                        }

                    }

                    constexpr static OpenModeString GetCOpenModeStr(const COpenMode mode) noexcept
                    {
                        switch(mode)
                        {
                            case COpenMode::ReadMustExist:      return ToModeString("r");
                            case COpenMode::Write:              return ToModeString("w");
                            case COpenMode::WriteAppend:        return ToModeString("a");
                            case COpenMode::ReadWriteMustExist: return ToModeString("r+");
                            case COpenMode::ReadWrite:          return ToModeString("w+");
                            case COpenMode::ReadWriteAppend:    return ToModeString("a+");
                            default:                            return ToModeString("i!");
                        }
                    }

                    constexpr OpenModeString GetAdditionalModeStr() const noexcept
                    {
                        OpenModeString additionalModeStr;

                        if(Presence[OpenModeFlag::Binary])                            additionalModeStr += ToModeString("b");
                        if(Presence[WindowsOpenModeFlag::Text])                       additionalModeStr += ToModeString("t"); //Text needs to be in place of binary
                        if(Presence[OpenModeFlag::MustNotExist])                      additionalModeStr += ToModeString("x");
                        if(Presence[WindowsOpenModeFlag::CommitDirectlyToDisk])       additionalModeStr += ToModeString("c");
                        if(Presence[WindowsOpenModeFlag::CommitIndirectlyToDisk])     additionalModeStr += ToModeString("n");
                        if(Presence[WindowsOpenModeFlag::NotInheritedByChildProcess]) additionalModeStr += ToModeString("N");
                        if(Presence[WindowsOpenModeFlag::SequntialAccessOptimized])   additionalModeStr += ToModeString("S");
                        if(Presence[WindowsOpenModeFlag::RandomAccessOptimized])      additionalModeStr += ToModeString("R");
                        if(Presence[WindowsOpenModeFlag::IfPossibleNoFlushingToDisk]) additionalModeStr += ToModeString("T");
                        if(Presence[WindowsOpenModeFlag::DeleteAfterClose])           additionalModeStr += ToModeString("D");
                        if(Presence[WindowsOpenModeFlag::UnicodeEncoding])            additionalModeStr += ToModeString(",css=UNICODE");
                        if(Presence[WindowsOpenModeFlag::Utf8Encoding])               additionalModeStr += ToModeString(",css=UTF-8");
                        if(Presence[WindowsOpenModeFlag::Utf16Encoding])              additionalModeStr += ToModeString(",css=UTF-16LE");

                        return additionalModeStr;
                    }

                private:
                    constexpr static OpenModeString ToModeString(const char8 * text)
                    {
                        return PromoteStringCharsTo<OsCharType>(ConstexprString<char8>(text));
                    }
            };
    };

    //Class responsible for transforming Open mode flags into
    // a valid fopen openmode string at compile time
    template <typename OsCharT>
    class BasicOpenMode
    {
        public:
            using ThisType = BasicOpenMode;

        private:
            using OpenModeConversionState = OpenModeHelpers::OpenModeConversionState<OsCharT>;
            using OpenModeValidity        = OpenModeHelpers::OpenModeValidity;

        public:
            using OpenModeEnumsBaseType   = OpenModeHelpers::OpenModeEnumsBaseType;
            using OpenModeFlag            = OpenModeFlag;
            using WindowsOpenModeFlag     = WindowsOpenModeFlag;
            using COpenMode               = COpenMode;

        public:
            using OsCharType              = typename OpenModeConversionState::OsCharType;
            template<typename CharT>
            using ConstexprString         = typename OpenModeConversionState::template ConstexprString<CharT>;
            using OpenModeString          = typename OpenModeConversionState::OpenModeString;
            using OsStringView            = StringView<OsCharType>;

            static_assert (std::is_same_v<OsCharType, char8> || std::is_same_v<OsCharType, charW>,
                           "Invalid OsCharType; Only char and wchar_t allowed; (No posix function takes other char types)");

        private:
            static constexpr COpenMode DefaultCOpenMode = COpenMode::ReadWrite;
            static constexpr OpenModeValidity DefaultOpenModeValidity = OpenModeValidity::Invalid;

        public:
            OpenModeString   OpenModeStr;
            COpenMode        CMode;
            OpenModeValidity Validity;

        public:
            constexpr BasicOpenMode() noexcept
                : OpenModeStr(), CMode(DefaultCOpenMode), Validity(DefaultOpenModeValidity)
            {}

            constexpr BasicOpenMode(COpenMode openMode) noexcept
                : OpenModeStr(), CMode(DefaultCOpenMode), Validity(DefaultOpenModeValidity)
            {
                AssignCOpenMode(openMode);
            }

        public:
            constexpr BasicOpenMode(const ThisType REF other) = default;
            constexpr BasicOpenMode(ThisType RVALUE_REF other) = default;

            constexpr ThisType REF operator=(const ThisType REF) = default;
            constexpr ThisType REF operator=(ThisType RVALUE_REF) = default;

            template<typename ... FlagTypes,
                     std::enable_if_t<OpenModeHelpers::AreOpenModeFlags<FlagTypes...>(), u32> = 0>
            constexpr BasicOpenMode(FlagTypes ... flags) noexcept
                : OpenModeStr(), CMode(), Validity()
            {
                this->operator =(CreateOpenMode<FlagTypes ...>(flags...));
            }

            constexpr void AssignCOpenMode(COpenMode cOpenMode) noexcept
            {
                this->CMode = cOpenMode;
                this->Validity = OpenModeValidity::Valid;
                this->OpenModeStr = OpenModeConversionState::GetCOpenModeStr(cOpenMode);
            }


            inline constexpr bool IsValid()     const noexcept {return IsValid(this->Validity);}
            inline constexpr bool IsSupported() const noexcept {return IsSupported(this->Validity);}

            inline constexpr auto GetOpenModeString() const noexcept
            {
                if(IsValid())
                    return OpenModeStr;
                else
                    return OpenModeString();
            }

            inline constexpr auto GetCOpenMode() const noexcept
            {
                return this->CMode;
            }

            inline constexpr operator OsStringView() const noexcept
            {
                return OpenModeStr.operator std::basic_string_view<OsCharType>();
            }

        private:
            static constexpr bool IsValid(OpenModeValidity validity)        noexcept {return validity == OpenModeValidity::Valid;}
            static constexpr bool IsSupported(OpenModeValidity validity)    noexcept {return NOT(validity == OpenModeValidity::Unsupported);}

            template<typename ... FlagTypes>
            static constexpr ThisType CreateOpenMode(FlagTypes ... flags) noexcept
            {
                static_assert (OpenModeHelpers::AreOpenModeFlags<FlagTypes...>(),
                        "BasicOpenMode::CreateOpenMode : All provided flags must be of type OpenModeFlag");

                OpenModeConversionState state(flags...);
                ThisType openMode;

                if(state.AreFlagsConflicting())
                    return openMode;

                state.SetCOpenModeAndValidty(openMode.CMode, openMode.Validity);

                if(NOT openMode.IsValid())
                    return openMode;

                openMode.OpenModeStr += state.GetCOpenModeStr(openMode.CMode);
                openMode.OpenModeStr += state.GetAdditionalModeStr();
                return openMode;
            }
    };

    using OpenMode = BasicOpenMode<char8>;
    using WOpenMode = BasicOpenMode<charW>;
}
#endif // NEWOPENMODE_H
