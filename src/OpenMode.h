#ifndef NEWOPENMODE_H
#define NEWOPENMODE_H

#include "AuxiliaryTypes.h"
#include "ConstexprString.h"

namespace cio
{
    namespace  OpenModeHelpers
    {
        using EnumBaseType = u8;

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
    }

    //These are all of the open mode flags necessary to represent all
    // c standard compilant open mode strings
    enum class OpenModeFlag : OpenModeHelpers::EnumBaseType
    {
        Read,
        Write,
        Create,
        Override,
        MustNotExist,
        StartAtEnd,

        Translated,

        //***************************************//
        //    OpenModeFlagsCount needs to be     //
        //  updated to match the actual number   //
        //             of options                //
        //***************************************//
        //This is to not bother the user with the count option
    };
    //These are all of the open mode flags necessary to represent all
    // visual c++ additional open mode symbols
    enum class WindowsOpenModeFlag : OpenModeHelpers::EnumBaseType
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
    enum class COpenMode : OpenModeHelpers::EnumBaseType
    {
        Read,           //Read && MustExist
        Write,          //Write
        Append,         //Write && Append
        ReadExtended,   //Read && Write && MustExist
        WriteExtended,  //Read && Write
        AppendExtended  //Read && Write && Append
    };

    enum class EnabledOperations: OpenModeHelpers::EnumBaseType
    {
        None,
        Read,
        Write,
        ReadWrite,
        Closed
    };

    namespace  OpenModeHelpers
    {
            static constexpr EnumBaseType OpenModeFlagsCount = 7;
            static constexpr EnumBaseType WindowsOpenModeFlagsCount = 11;

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
                    using Underlying = EnumBaseType;

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

            struct RequiredActions
            {
                    bool MustExist    = false;
                    bool MustNotExist = false;
                    bool Create       = false;
                    bool Delete       = false;
                    bool StartAtEnd   = false;
            };

            template<typename OsCharT>
            class ConversionState
            {
                public:
                    using ThisType = ConversionState;

                public:
                    using EnumBase = EnumBaseType;
                    static constexpr u32 OpenModeMaxChars = 26;
                    using OsCharType = OsCharT;

                    template<typename CharT>
                    using ConstexprString = ConstexprString<CharT, OpenModeMaxChars>;
                    using OpenModeString = ConstexprString<OsCharType>;


                public:
                    const FlagPresence Presence;

                public:
                    template<typename ... FlagTypes>
                    constexpr ConversionState(const FlagTypes ... flags) noexcept : Presence(flags...)
                    {}

                public:
                    constexpr bool AreFlagsConflicting() const noexcept
                    {

                        if(NOT Presence[OpenModeFlag::Create] && Presence[OpenModeFlag::MustNotExist])
                            return true;

                        if(Presence[WindowsOpenModeFlag::Text] && NOT Presence[OpenModeFlag::Translated])
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

                    constexpr void GetCopenModeAndRequiredActions(COpenMode REF cmode, RequiredActions REF required) const noexcept
                    {
                        const bool Read         = Presence[OpenModeFlag::Read];
                        const bool Write        = Presence[OpenModeFlag::Write];
                        const bool Create       = Presence[OpenModeFlag::Create];
                        const bool Override     = Presence[OpenModeFlag::Override];
                        const bool MustNotExist = Presence[OpenModeFlag::MustNotExist];
                        const bool StartAtEnd   = Presence[OpenModeFlag::StartAtEnd];

                        struct Helper
                        {
                                static constexpr u32 Binary(EnumBase a, EnumBase b, EnumBase c) {return 4*a + 2*b + 1*c;}
                        };

                        auto set = [&](COpenMode to, bool must = false, bool mustNot = false, bool create = false, bool del = false)
                        {
                            cmode = to;
                            required.MustExist = must;
                            required.MustNotExist = mustNot;
                            required.Create = create;
                            required.Delete = del;
                        };

#ifdef SIMPLE_OPENMODE_CREATION
                        switch (Helper::Binary(Create, Override, MustNotExist))
                        {
                            //                  C  O  N                                M  N  C  D
                            case Helper::Binary(0, 0, 0): set(COpenMode::ReadExtended, 1, 0, 0, 0);
                            case Helper::Binary(0, 0, 1): set(COpenMode::ReadExtended, 1, 1, 0, 0); //Error
                            case Helper::Binary(0, 1, 0): set(COpenMode::ReadExtended, 1, 0, 1, 1);
                            case Helper::Binary(0, 1, 1): set(COpenMode::ReadExtended, 1, 1, 0, 0); //Error
                            case Helper::Binary(1, 0, 0): set(COpenMode::ReadExtended, 0, 0, 1, 0);
                            case Helper::Binary(1, 0, 1): set(COpenMode::ReadExtended, 0, 1, 1, 0);
                            case Helper::Binary(1, 1, 0): set(COpenMode::ReadExtended, 0, 0, 1, 0);
                            case Helper::Binary(1, 1, 1): set(COpenMode::ReadExtended, 0, 1, 1, 1);
                        }

#else
                        //Best matching permissions - Only two cases where permissions require exeptions
                        if(Write && Read)
                        {
                            switch (Helper::Binary(Create, Override, MustNotExist))
                            {
                                //                  C  O  N                                 M  N  C  D
                                case Helper::Binary(0, 0, 0): set(COpenMode::ReadExtended,  1, 0, 0, 0); break;
                                case Helper::Binary(0, 0, 1): set(COpenMode::Read,          1, 1, 0, 0); break; //Error
                                case Helper::Binary(0, 1, 0): set(COpenMode::WriteExtended, 1, 0, 0, 0); break;
                                case Helper::Binary(0, 1, 1): set(COpenMode::Read,          1, 1, 0, 0); break; //Error
                                case Helper::Binary(1, 0, 0): set(COpenMode::ReadExtended,  0, 0, 1, 0); break;
                                case Helper::Binary(1, 0, 1): set(COpenMode::WriteExtended, 0, 1, 1, 0); break;
                                case Helper::Binary(1, 1, 0): set(COpenMode::WriteExtended, 0, 0, 1, 1); break;
                                case Helper::Binary(1, 1, 1): set(COpenMode::WriteExtended, 0, 1, 1, 1); break;
                            }
                        }
                        else if(Write)
                        {
                            switch (Helper::Binary(Create, Override, MustNotExist))
                            {
                                //                  C  O  N                                 M  N  C  D
                                case Helper::Binary(0, 0, 0): set(COpenMode::ReadExtended,  1, 0, 0, 0); break; //UNMATCHING PERMISSIONS
                                case Helper::Binary(0, 0, 1): set(COpenMode::Read,          1, 1, 0, 0); break; //Error
                                case Helper::Binary(0, 1, 0): set(COpenMode::Write,         1, 0, 0, 0); break;
                                case Helper::Binary(0, 1, 1): set(COpenMode::Read,          1, 1, 0, 0); break; //Error
                                case Helper::Binary(1, 0, 0): set(COpenMode::ReadExtended,  0, 0, 1, 0); break; //UNMATCHING PERMISSIONS
                                case Helper::Binary(1, 0, 1): set(COpenMode::Write,         0, 1, 1, 0); break;
                                case Helper::Binary(1, 1, 0): set(COpenMode::Write,         0, 0, 1, 0); break;
                                case Helper::Binary(1, 1, 1): set(COpenMode::Write,         0, 1, 1, 1); break;
                            }
                        }
                        else
                        {
                            switch (Helper::Binary(Create, Override, MustNotExist))
                            {
                                //                  C  O  N                        M  N  C  D
                                case Helper::Binary(0, 0, 0): set(COpenMode::Read, 0, 0, 0, 0); break;
                                case Helper::Binary(0, 0, 1): set(COpenMode::Read, 1, 1, 0, 0); break; //Error
                                case Helper::Binary(0, 1, 0): set(COpenMode::Read, 1, 0, 1, 1); break;
                                case Helper::Binary(0, 1, 1): set(COpenMode::Read, 1, 1, 0, 0); break; //Error
                                case Helper::Binary(1, 0, 0): set(COpenMode::Read, 0, 0, 1, 0); break;
                                case Helper::Binary(1, 0, 1): set(COpenMode::Read, 0, 1, 1, 0); break;
                                case Helper::Binary(1, 1, 0): set(COpenMode::Read, 0, 0, 1, 0); break;
                                case Helper::Binary(1, 1, 1): set(COpenMode::Read, 0, 1, 1, 1); break;
                            }
                        }
#endif

                        if(StartAtEnd)
                            required.StartAtEnd = true;
                    }

                    constexpr EnabledOperations GetEnabledOperations() const noexcept
                    {
                        const bool read         = Presence[OpenModeFlag::Read];
                        const bool write        = Presence[OpenModeFlag::Write];

                        if(read && write)
                            return EnabledOperations::ReadWrite;
                        if(read)
                            return EnabledOperations::Read;
                        if(write)
                            return EnabledOperations::Write;

                        return EnabledOperations::None;
                    }

                    constexpr static RequiredActions GetCOpenRequiredActions(const COpenMode mode) noexcept
                    {
                        if(mode == COpenMode::ReadExtended)
                        {
                            RequiredActions required;
                            required.MustExist = true;
                            return required;
                        }
                        else
                            return RequiredActions();
                    }

                    constexpr static EnabledOperations GetCOpenEnabledOperations(const COpenMode mode) noexcept
                    {
                        switch(mode)
                        {
                            case COpenMode::Read:           return EnabledOperations::Read;
                            case COpenMode::Write:          return EnabledOperations::Write;
                            case COpenMode::Append:         return EnabledOperations::Write;
                            case COpenMode::ReadExtended:   return EnabledOperations::ReadWrite;
                            case COpenMode::WriteExtended:  return EnabledOperations::ReadWrite;
                            case COpenMode::AppendExtended: return EnabledOperations::ReadWrite;
                            default:                        return EnabledOperations::None;
                        }
                    }

                    constexpr static OpenModeString GetCOpenModeStr(const COpenMode mode) noexcept
                    {
                        switch(mode)
                        {
                            case COpenMode::Read:           return ToModeString("r");
                            case COpenMode::Write:          return ToModeString("w");
                            case COpenMode::Append:         return ToModeString("a");
                            case COpenMode::ReadExtended:   return ToModeString("r+");
                            case COpenMode::WriteExtended:  return ToModeString("w+");
                            case COpenMode::AppendExtended: return ToModeString("a+");
                            default:                        return ToModeString("i!");
                        }
                    }

                    constexpr OpenModeString GetAdditionalModeStr() const noexcept
                    {
                        OpenModeString additionalModeStr;

                        if(NOT Presence[OpenModeFlag::Translated])                    additionalModeStr += ToModeString("b");
                        if(Presence[WindowsOpenModeFlag::Text])                       additionalModeStr += ToModeString("t"); //Text needs to be in place of binary
                        //if(Presence[OpenModeFlag::MustNotExist])                      additionalModeStr += ToModeString("x"); //Is checked through required actions
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
                    constexpr static OpenModeString ToModeString(const char8 * text) noexcept
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
            using ConversionState         = OpenModeHelpers::ConversionState<OsCharT>;

        public:
            using EnabledOperations       = EnabledOperations;
            using RequiredActions         = OpenModeHelpers::RequiredActions;
            using EnumBaseType            = OpenModeHelpers::EnumBaseType;
            using OpenModeFlag            = OpenModeFlag;
            using WindowsOpenModeFlag     = WindowsOpenModeFlag;
            using COpenMode               = COpenMode;

        public:
            using OsCharType              = typename ConversionState::OsCharType;
            template<typename CharT>
            using ConstexprString         = typename ConversionState::template ConstexprString<CharT>;
            using OpenModeString          = typename ConversionState::OpenModeString;
            using OsStringView            = StringView<OsCharType>;

        private:
            bool                Valid       = {false};
            COpenMode           CMode       = {COpenMode::ReadExtended};
            EnabledOperations   Enabled     = {EnabledOperations::Closed};
            RequiredActions     Required    = {RequiredActions()};
            OpenModeString      OpenModeStr = {OpenModeString()};

        public:
            constexpr BasicOpenMode() noexcept = default;
            constexpr BasicOpenMode(const ThisType REF other) = default;
            constexpr BasicOpenMode(ThisType RVALUE_REF other) = default;

            constexpr ThisType REF operator=(const ThisType REF) = default;
            constexpr ThisType REF operator=(ThisType RVALUE_REF) = default;

        public:
            constexpr BasicOpenMode(COpenMode cOpenMode) noexcept
                //: OpenModeStr(), Valid(), Enabled(), Required()
            {
                AssignCOpenMode(cOpenMode);
            }

            template<typename ... FlagTypes,
                     std::enable_if_t<OpenModeHelpers::AreOpenModeFlags<FlagTypes...>(), u32> = 0>
            constexpr BasicOpenMode(FlagTypes ... flags) noexcept
                //: OpenModeStr(), Valid(), Enabled(), Required()
            {
                AssignOpenMode(flags...);
            }

            inline constexpr bool IsValid()             const noexcept {return Valid;}

            inline constexpr auto GetOpenModeString()   const noexcept {return OpenModeStr;}
            inline constexpr auto GetCOpenMode()        const noexcept {return CMode;}
            inline constexpr auto GetEnabledOperations()   const noexcept {return Enabled;}
            inline constexpr auto GetRequiredActions()  const noexcept {return Required;}

            inline constexpr operator OsStringView() const noexcept
            {
                return OpenModeStr.operator std::basic_string_view<OsCharType>();
            }


        private:
            constexpr void AssignCOpenMode(COpenMode cOpenMode) noexcept
            {
                CMode = cOpenMode;
                Valid = true;
                Required = ConversionState::GetCOpenRequiredActions(cOpenMode);
                Enabled = ConversionState::GetCOpenEnabledOperations(cOpenMode);
                OpenModeStr = ConversionState::GetCOpenModeStr(cOpenMode);
            }
            template<typename ... FlagTypes>
            constexpr void AssignOpenMode(FlagTypes ... flags) noexcept
            {
                const ConversionState state(flags...);

                Valid = NOT state.AreFlagsConflicting();

                if(IsValid() == false)
                    return;

                Enabled = state.GetEnabledOperations();
                state.GetCopenModeAndRequiredActions(CMode, Required);

                OpenModeStr += state.GetCOpenModeStr(CMode);
                OpenModeStr += state.GetAdditionalModeStr();
                return;
            }

    };

    using OpenMode = BasicOpenMode<char8>;
    using WOpenMode = BasicOpenMode<charW>;
}
#endif // NEWOPENMODE_H
