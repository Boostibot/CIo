#ifndef OPENMODETESTING_H
#define OPENMODETESTING_H

#include "Catch2/Catch.hpp"
#include "src/OpenMode.h"

namespace cio::OpenModeTesting
{
    using namespace OpenModeHelpers;
}

#define OpenMode_TEST_COMPILE_ERROR false
#define OpenModeTestedTypes char8, charW

namespace cio::OpenModeTesting
{
    namespace FlagPresenceTesting
    {
        TEST_CASE("[FlagPresence] : IsFlagPresent should return true if the looking for flag is present in the arguments", "[FlagPresence][IsFlagPresent]")
        {

            WHEN("Given arguments containing the lookignForFlag it should return true")
            {
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Read, OpenModeFlag::Read) == true);
                REQUIRE(FlagPresence::IsFlagPresent(WindowsOpenModeFlag::IfPossibleNoFlushingToDisk, OpenModeFlag::StartAtEnd, WindowsOpenModeFlag::IfPossibleNoFlushingToDisk) == true);
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Write, OpenModeFlag::MustNotExist, OpenModeFlag::Write, OpenModeFlag::MustNotExist) == true);
                REQUIRE(FlagPresence::IsFlagPresent(WindowsOpenModeFlag::RandomAccessOptimized, OpenModeFlag::Read, OpenModeFlag::Write, WindowsOpenModeFlag::RandomAccessOptimized) == true);
            }
            WHEN("Given arguments containing the lookignForFlag multiple times it should return true")
            {
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Read, OpenModeFlag::Read, OpenModeFlag::Read) == true);
                REQUIRE(FlagPresence::IsFlagPresent(WindowsOpenModeFlag::Utf8Encoding, OpenModeFlag::StartAtEnd, WindowsOpenModeFlag::Utf8Encoding, WindowsOpenModeFlag::Utf8Encoding) == true);
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Write, OpenModeFlag::StartAtEnd, WindowsOpenModeFlag::Utf8Encoding, OpenModeFlag::Write, OpenModeFlag::Write) == true);
            }
            WHEN("Given arguments not containing the lookignForFlag it should return false")
            {
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Write) == false);
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Write, WindowsOpenModeFlag::IfPossibleNoFlushingToDisk, OpenModeFlag::StartAtEnd) == false);
                REQUIRE(FlagPresence::IsFlagPresent(WindowsOpenModeFlag::Utf8Encoding, WindowsOpenModeFlag::RandomAccessOptimized, OpenModeFlag::Write) == false);
            }
            WHEN("Given arguments only lookingForType it should return false")
            {
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::Read) == false);
                REQUIRE(FlagPresence::IsFlagPresent(WindowsOpenModeFlag::NotInheritedByChildProcess) == false);
                REQUIRE(FlagPresence::IsFlagPresent(OpenModeFlag::MustNotExist) == false);
                REQUIRE(FlagPresence::IsFlagPresent(WindowsOpenModeFlag::Text) == false);
            }
        }

        TEST_CASE("[FlagPresence] : operator [] should return flag presence at index", "[FlagPresence][operator []]")
        {
            GIVEN("A FlagPresence constructed with certain flags")
            {
                FlagPresence presence(OpenModeFlag::Write, OpenModeFlag::StartAtEnd, OpenModeFlag::MustNotExist, WindowsOpenModeFlag::DeleteAfterClose);
                WHEN("A [] operator is used with the value of constructed flag it should be present")
                {
                    REQUIRE(presence[OpenModeFlag::Write] == true);
                    REQUIRE(presence[OpenModeFlag::StartAtEnd] == true);
                    REQUIRE(presence[OpenModeFlag::MustNotExist] == true);
                    REQUIRE(presence[WindowsOpenModeFlag::DeleteAfterClose] == true);
                }
                WHEN("A [] operator is used with the value of not constructed flag it should not be present")
                {
                    REQUIRE(presence[OpenModeFlag::Read] == false);
                    REQUIRE(presence[OpenModeFlag::Translated] == false);
                    REQUIRE(presence[OpenModeFlag::Override] == false);
                    REQUIRE(presence[WindowsOpenModeFlag::IfPossibleNoFlushingToDisk] == false);
                }
            }
        }

        TEST_CASE("[FlagPresence] : Constructing FlagPresence should fill the presence array accordingly", "[FlagPresence][Constructor]")
        {
            WHEN("Constructed with No flags")
            {
                FlagPresence presence;
                THEN("There should be no presence noted")
                {
                    for(EnumBaseType i = 0; i < OpenModeFlagsCount; i++)
                        CHECK(presence[static_cast<OpenModeFlag>(i)] == false);
                }
            }

            WHEN("Constructed with All flags")
            {
                FlagPresence presence(OpenModeFlag::Read,
                                      OpenModeFlag::Write,
                                      OpenModeFlag::Create,
                                      OpenModeFlag::Override,
                                      OpenModeFlag::MustNotExist,
                                      OpenModeFlag::StartAtEnd,
                                      OpenModeFlag::Translated,
                                      WindowsOpenModeFlag::Text,
                                      WindowsOpenModeFlag::CommitDirectlyToDisk,
                                      WindowsOpenModeFlag::CommitIndirectlyToDisk,
                                      WindowsOpenModeFlag::NotInheritedByChildProcess,
                                      WindowsOpenModeFlag::SequntialAccessOptimized,
                                      WindowsOpenModeFlag::RandomAccessOptimized,
                                      WindowsOpenModeFlag::IfPossibleNoFlushingToDisk,
                                      WindowsOpenModeFlag::DeleteAfterClose,
                                      WindowsOpenModeFlag::UnicodeEncoding,
                                      WindowsOpenModeFlag::Utf8Encoding,
                                      WindowsOpenModeFlag::Utf16Encoding);

                THEN("All flags should be present")
                {
                    for(EnumBaseType i = 0; i < OpenModeFlagsCount; i++)
                        CHECK(presence[static_cast<OpenModeFlag>(i)] == true);
                }
            }

            WHEN("Constructed with a single argument")
            {
                THEN("There should be no presence except the one specified")
                {
                    SECTION("Flag OpenModeFlag::Read")
                    {
                        FlagPresence presence(OpenModeFlag::Read);
                        CHECK(presence[OpenModeFlag::Write] == false);
                        CHECK(presence[OpenModeFlag::Read] == true);
                    }
                    SECTION("Flag OpenModeFlag::Write")
                    {
                        FlagPresence presence(OpenModeFlag::Write);
                        CHECK(presence[OpenModeFlag::Create] == false);
                        CHECK(presence[OpenModeFlag::Write] == true);
                    }
                    SECTION("Flag OpenModeFlag::Create")
                    {
                        FlagPresence presence(OpenModeFlag::Create);
                        CHECK(presence[OpenModeFlag::Override] == false);
                        CHECK(presence[OpenModeFlag::Create] == true);
                    }
                    SECTION("Flag OpenModeFlag::Override")
                    {
                        FlagPresence presence(OpenModeFlag::Override);
                        CHECK(presence[OpenModeFlag::MustNotExist] == false);
                        CHECK(presence[OpenModeFlag::Override] == true);
                    }
                    SECTION("Flag OpenModeFlag::MustNotExist")
                    {
                        FlagPresence presence(OpenModeFlag::MustNotExist);
                        CHECK(presence[OpenModeFlag::StartAtEnd] == false);
                        CHECK(presence[OpenModeFlag::MustNotExist] == true);
                    }
                    SECTION("Flag OpenModeFlag::StartAtEnd")
                    {
                        FlagPresence presence(OpenModeFlag::StartAtEnd);
                        CHECK(presence[OpenModeFlag::Translated] == false);
                        CHECK(presence[OpenModeFlag::StartAtEnd] == true);
                    }
                    SECTION("Flag OpenModeFlag::Translated")
                    {
                        FlagPresence presence(OpenModeFlag::Translated);
                        CHECK(presence[WindowsOpenModeFlag::Text] == false);
                        CHECK(presence[OpenModeFlag::Translated] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::Text")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::Text);
                        CHECK(presence[WindowsOpenModeFlag::CommitDirectlyToDisk] == false);
                        CHECK(presence[WindowsOpenModeFlag::Text] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::CommitDirectlyToDisk")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::CommitDirectlyToDisk);
                        CHECK(presence[WindowsOpenModeFlag::CommitIndirectlyToDisk] == false);
                        CHECK(presence[WindowsOpenModeFlag::CommitDirectlyToDisk] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::CommitIndirectlyToDisk")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::CommitIndirectlyToDisk);
                        CHECK(presence[WindowsOpenModeFlag::NotInheritedByChildProcess] == false);
                        CHECK(presence[WindowsOpenModeFlag::CommitIndirectlyToDisk] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::NotInheritedByChildProcess")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::NotInheritedByChildProcess);
                        CHECK(presence[WindowsOpenModeFlag::SequntialAccessOptimized] == false);
                        CHECK(presence[WindowsOpenModeFlag::NotInheritedByChildProcess] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::SequntialAccessOptimized")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::SequntialAccessOptimized);
                        CHECK(presence[WindowsOpenModeFlag::RandomAccessOptimized] == false);
                        CHECK(presence[WindowsOpenModeFlag::SequntialAccessOptimized] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::RandomAccessOptimized")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::RandomAccessOptimized);
                        CHECK(presence[WindowsOpenModeFlag::IfPossibleNoFlushingToDisk] == false);
                        CHECK(presence[WindowsOpenModeFlag::RandomAccessOptimized] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::IfPossibleNoFlushingToDisk")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::IfPossibleNoFlushingToDisk);
                        CHECK(presence[WindowsOpenModeFlag::DeleteAfterClose] == false);
                        CHECK(presence[WindowsOpenModeFlag::IfPossibleNoFlushingToDisk] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::DeleteAfterClose")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::DeleteAfterClose);
                        CHECK(presence[WindowsOpenModeFlag::UnicodeEncoding] == false);
                        CHECK(presence[WindowsOpenModeFlag::DeleteAfterClose] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::UnicodeEncoding")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::UnicodeEncoding);
                        CHECK(presence[WindowsOpenModeFlag::Utf8Encoding] == false);
                        CHECK(presence[WindowsOpenModeFlag::UnicodeEncoding] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::Utf8Encoding")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::Utf8Encoding);
                        CHECK(presence[WindowsOpenModeFlag::Utf16Encoding] == false);
                        CHECK(presence[WindowsOpenModeFlag::Utf8Encoding] == true);
                    }
                    SECTION("Flag WindowsOpenModeFlag::Utf16Encoding")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::Utf16Encoding);
                        CHECK(presence[OpenModeFlag::Read] == false);
                        CHECK(presence[WindowsOpenModeFlag::Utf16Encoding] == true);
                    }
                }
            }

            WHEN("Constructed with multiple arguments")
            {
                THEN("There should be no presence except the one specified")
                {
                    SECTION("Flags OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::DeleteAfterClose, OpenModeFlag::Utf16Encoding")
                    {
                        FlagPresence presence(OpenModeFlag::Read, OpenModeFlag::Write, WindowsOpenModeFlag::DeleteAfterClose, WindowsOpenModeFlag::Utf16Encoding);
                        CHECK(presence[OpenModeFlag::Read] == true);
                        CHECK(presence[OpenModeFlag::Write] == true);
                        CHECK(presence[WindowsOpenModeFlag::DeleteAfterClose] == true);
                        CHECK(presence[WindowsOpenModeFlag::Utf16Encoding] == true);

                        CHECK(presence[OpenModeFlag::StartAtEnd] == false);
                        CHECK(presence[WindowsOpenModeFlag::Text] == false);
                        CHECK(presence[WindowsOpenModeFlag::RandomAccessOptimized] == false);

                    }
                    SECTION("Flags OpenModeFlag::Binary, OpenModeFlag::Write, OpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::StartAtEnd")
                    {
                        FlagPresence presence(OpenModeFlag::Translated, OpenModeFlag::Write, WindowsOpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::StartAtEnd);
                        CHECK(presence[OpenModeFlag::Translated] == true);
                        CHECK(presence[OpenModeFlag::Write] == true);
                        CHECK(presence[WindowsOpenModeFlag::CommitDirectlyToDisk] == true);
                        CHECK(presence[OpenModeFlag::StartAtEnd] == true);

                        CHECK(presence[OpenModeFlag::Read] == false);
                        CHECK(presence[WindowsOpenModeFlag::Text] == false);
                        CHECK(presence[WindowsOpenModeFlag::SequntialAccessOptimized] == false);

                    }
                    SECTION("Flags OpenModeFlag::NotInheritedByChildProcess, OpenModeFlag::MustExist, OpenModeFlag::Utf8Encoding")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::NotInheritedByChildProcess, OpenModeFlag::MustNotExist, WindowsOpenModeFlag::Utf8Encoding);
                        CHECK(presence[WindowsOpenModeFlag::NotInheritedByChildProcess] == true);
                        CHECK(presence[OpenModeFlag::MustNotExist] == true);
                        CHECK(presence[WindowsOpenModeFlag::Utf8Encoding] == true);

                        CHECK(presence[OpenModeFlag::Write] == false);
                        CHECK(presence[WindowsOpenModeFlag::Text] == false);
                        CHECK(presence[WindowsOpenModeFlag::IfPossibleNoFlushingToDisk] == false);
                        CHECK(presence[WindowsOpenModeFlag::DeleteAfterClose] == false);

                    }
                    SECTION("Flags Read, Write, DeleteAfterClose, Utf16Encoding")
                    {
                        FlagPresence presence(OpenModeFlag::Read, WindowsOpenModeFlag::Text, WindowsOpenModeFlag::UnicodeEncoding);
                        CHECK(presence[OpenModeFlag::Read] == true);
                        CHECK(presence[WindowsOpenModeFlag::Text] == true);
                        CHECK(presence[WindowsOpenModeFlag::UnicodeEncoding] == true);

                        CHECK(presence[OpenModeFlag::Write] == false);
                        CHECK(presence[OpenModeFlag::Translated] == false);
                        CHECK(presence[OpenModeFlag::StartAtEnd] == false);
                        CHECK(presence[WindowsOpenModeFlag::CommitIndirectlyToDisk] == false);

                    }
                }
            }

            WHEN("Constructed with multiples of the same argument")
            {
                THEN("There should be no presence except the one specified")
                {
                    SECTION("Flags OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::DeleteAfterClose, OpenModeFlag::Utf16Encoding")
                    {
                        FlagPresence presence(OpenModeFlag::Read, OpenModeFlag::Read, OpenModeFlag::Write, WindowsOpenModeFlag::DeleteAfterClose, WindowsOpenModeFlag::Utf16Encoding);
                        CHECK(presence[OpenModeFlag::Read] == true);
                        CHECK(presence[OpenModeFlag::Write] == true);
                        CHECK(presence[WindowsOpenModeFlag::DeleteAfterClose] == true);
                        CHECK(presence[WindowsOpenModeFlag::Utf16Encoding] == true);

                        CHECK(presence[OpenModeFlag::StartAtEnd] == false);
                        CHECK(presence[WindowsOpenModeFlag::Text] == false);
                        CHECK(presence[WindowsOpenModeFlag::RandomAccessOptimized] == false);

                    }
                    SECTION("Flags OpenModeFlag::Translated, OpenModeFlag::Write, OpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::StartAtEnd")
                    {
                        FlagPresence presence(OpenModeFlag::Translated, OpenModeFlag::Write, WindowsOpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::Translated, OpenModeFlag::StartAtEnd);
                        CHECK(presence[OpenModeFlag::Translated] == true);
                        CHECK(presence[OpenModeFlag::Write] == true);
                        CHECK(presence[WindowsOpenModeFlag::CommitDirectlyToDisk] == true);
                        CHECK(presence[OpenModeFlag::StartAtEnd] == true);

                        CHECK(presence[OpenModeFlag::Read] == false);
                        CHECK(presence[WindowsOpenModeFlag::Text] == false);
                        CHECK(presence[WindowsOpenModeFlag::SequntialAccessOptimized] == false);

                    }
                    SECTION("Flags OpenModeFlag::NotInheritedByChildProcess, OpenModeFlag::MustExist, OpenModeFlag::Utf8Encoding")
                    {
                        FlagPresence presence(WindowsOpenModeFlag::NotInheritedByChildProcess, OpenModeFlag::MustNotExist, OpenModeFlag::MustNotExist, WindowsOpenModeFlag::Utf8Encoding);
                        CHECK(presence[WindowsOpenModeFlag::NotInheritedByChildProcess] == true);
                        CHECK(presence[OpenModeFlag::MustNotExist] == true);
                        CHECK(presence[WindowsOpenModeFlag::Utf8Encoding] == true);

                        CHECK(presence[OpenModeFlag::Write] == false);
                        CHECK(presence[WindowsOpenModeFlag::Text] == false);
                        CHECK(presence[WindowsOpenModeFlag::IfPossibleNoFlushingToDisk] == false);
                        CHECK(presence[WindowsOpenModeFlag::DeleteAfterClose] == false);

                    }
                    SECTION("Flags Read, Write, DeleteAfterClose, Utf16Encoding")
                    {
                        FlagPresence presence(OpenModeFlag::Read, WindowsOpenModeFlag::Text, WindowsOpenModeFlag::Text, WindowsOpenModeFlag::Text, WindowsOpenModeFlag::UnicodeEncoding);
                        CHECK(presence[OpenModeFlag::Read] == true);
                        CHECK(presence[WindowsOpenModeFlag::Text] == true);
                        CHECK(presence[WindowsOpenModeFlag::UnicodeEncoding] == true);

                        CHECK(presence[OpenModeFlag::Write] == false);
                        CHECK(presence[OpenModeFlag::Translated] == false);
                        CHECK(presence[OpenModeFlag::StartAtEnd] == false);
                        CHECK(presence[WindowsOpenModeFlag::CommitIndirectlyToDisk] == false);

                    }
                }
            }
        }
    }

    namespace OpenModeConversionStateTesting
    {
        TEMPLATE_TEST_CASE("[ConversionState] : Constructor should initialise the FlagPresence according to the given paarmeters", "[ConversionState][Constructor]", OpenModeTestedTypes)
        {
            using ConversionState = ConversionState<TestType>;

            GIVEN("A state constructed with certain flags")
            {
                ConversionState state(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Write, WindowsOpenModeFlag::Text, OpenModeFlag::Translated, WindowsOpenModeFlag::UnicodeEncoding);
                THEN("the specified flags should be present")
                {
                    REQUIRE(state.Presence[OpenModeFlag::Read] == true);
                    REQUIRE(state.Presence[OpenModeFlag::Write] == true);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::Text] == true);
                    REQUIRE(state.Presence[OpenModeFlag::Translated] == true);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::UnicodeEncoding] == true);
                }
                THEN("the not specified flags should not be present")
                {
                    REQUIRE(state.Presence[OpenModeFlag::MustNotExist] == false);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::NotInheritedByChildProcess] == false);
                    REQUIRE(state.Presence[OpenModeFlag::StartAtEnd] == false);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::SequntialAccessOptimized] == false);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::RandomAccessOptimized] == false);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::Utf16Encoding] == false);
                }
            }
            GIVEN("A state constructed with no flags")
            {
                ConversionState state;
                THEN("no flags should be present")
                {
                    REQUIRE(state.Presence[OpenModeFlag::Read] == false);
                    REQUIRE(state.Presence[OpenModeFlag::Write] == false);
                    REQUIRE(state.Presence[OpenModeFlag::StartAtEnd] == false);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::Text] == false);
                    REQUIRE(state.Presence[OpenModeFlag::Translated] == false);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::CommitDirectlyToDisk] == false);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::UnicodeEncoding] == false);
                    REQUIRE(state.Presence[WindowsOpenModeFlag::Utf16Encoding] == false);
                }
            }
        }

        TEMPLATE_TEST_CASE("[ConversionState] : AreFlagsConflicting should return true if the flags are logically conflicting", "[ConversionState][AreFlagsConflicting]", OpenModeTestedTypes)
        {
            using ConversionState = ConversionState<TestType>;

            GIVEN("A states constructed with not conflicting flags")
            {
                ConversionState state1(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Write);
                ConversionState state2(OpenModeFlag::Read, WindowsOpenModeFlag::UnicodeEncoding);
                ConversionState state3;
                THEN("The flags should not conflict")
                {
                    REQUIRE(state1.AreFlagsConflicting() == false);
                    REQUIRE(state2.AreFlagsConflicting() == false);
                    REQUIRE(state3.AreFlagsConflicting() == false);
                }
            }

            GIVEN("states constructed with MustNotExist flag without Create flag")
            {
                ConversionState state1(OpenModeFlag::MustNotExist);
                ConversionState state2(OpenModeFlag::MustNotExist, OpenModeFlag::Write, OpenModeFlag::Read, WindowsOpenModeFlag::UnicodeEncoding);
                THEN("The flags should conflict")
                {
                    REQUIRE(state1.AreFlagsConflicting() == true);
                    REQUIRE(state2.AreFlagsConflicting() == true);
                }
            }
            GIVEN("states constructed with Text and withouth the Translated flag")
            {
                ConversionState state1(WindowsOpenModeFlag::Text);
                ConversionState state2(WindowsOpenModeFlag::Text, OpenModeFlag::Read, WindowsOpenModeFlag::UnicodeEncoding);
                THEN("The flags should conflict")
                {
                    REQUIRE(state1.AreFlagsConflicting() == true);
                    REQUIRE(state2.AreFlagsConflicting() == true);
                }
            }
            GIVEN("states constructed with CommitDirectlyToDisk and CommitIndirectlyToDisk flags")
            {
                ConversionState state1(WindowsOpenModeFlag::CommitDirectlyToDisk, WindowsOpenModeFlag::CommitIndirectlyToDisk);
                THEN("The flags should conflict")
                {
                    REQUIRE(state1.AreFlagsConflicting() == true);
                }
            }
            GIVEN("states constructed with SequntialAccessOptimized and RandomAccessOptimized flags")
            {
                ConversionState state1(WindowsOpenModeFlag::CommitDirectlyToDisk, WindowsOpenModeFlag::CommitIndirectlyToDisk);
                THEN("The flags should conflict")
                {
                    REQUIRE(state1.AreFlagsConflicting() == true);
                }
            }
            GIVEN("states constructed with at leats two of UnicodeEncoding, Utf8Encoding or Utf16Encoding flags")
            {
                ConversionState state1(WindowsOpenModeFlag::UnicodeEncoding, WindowsOpenModeFlag::Utf8Encoding);
                ConversionState state2(WindowsOpenModeFlag::UnicodeEncoding, WindowsOpenModeFlag::Utf16Encoding);
                ConversionState state3(WindowsOpenModeFlag::Utf8Encoding, WindowsOpenModeFlag::Utf16Encoding);
                ConversionState state4(WindowsOpenModeFlag::UnicodeEncoding, WindowsOpenModeFlag::Utf8Encoding, WindowsOpenModeFlag::Utf8Encoding);
                THEN("The flags should conflict")
                {
                    REQUIRE(state1.AreFlagsConflicting() == true);
                    REQUIRE(state2.AreFlagsConflicting() == true);
                    REQUIRE(state3.AreFlagsConflicting() == true);
                    REQUIRE(state4.AreFlagsConflicting() == true);
                }
            }
        }


        TEMPLATE_TEST_CASE("[ConversionState] : SetCOpenModeAndValidty should match the given flags to a COpenMode or set invalid", "[ConversionState][SetCOpenModeAndValidty]", OpenModeTestedTypes)
        {
            using ConversionState = ConversionState<TestType>;
            COpenMode copenMode;
            RequiredActions required;

            WHEN("State is constructed with valid combination of flags the copenMode should be set"
                 " to the appropriate mode and validity should be set to valid")
            {
                SECTION("Flag Read should result in Read and no required actions")
                {
                    ConversionState state(OpenModeFlag::Read);
                    state.GetCopenModeAndRequiredActions(copenMode, required);

                    CHECK(copenMode == COpenMode::Read);
                    CHECK(required.MustExist == false);
                    CHECK(required.MustNotExist == false);
                    CHECK(required.Create == false);
                    CHECK(required.Delete == false);
                    CHECK(required.StartAtEnd == false);
                }
                SECTION("Flags Read, Write, Create should result in ReadExtended and Create actions")
                {
                    ConversionState state(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Create);
                    state.GetCopenModeAndRequiredActions(copenMode, required);

                    CHECK(copenMode == COpenMode::ReadExtended);
                    CHECK(required.MustExist == false);
                    CHECK(required.MustNotExist == false);
                    CHECK(required.Create == true);
                    CHECK(required.Delete == false);
                    CHECK(required.StartAtEnd == false);
                }
                SECTION("Flags Write, Override should result in Write and MustExist actions")
                {
                    ConversionState state(OpenModeFlag::Write, OpenModeFlag::Override);
                    state.GetCopenModeAndRequiredActions(copenMode, required);

                    CHECK(copenMode == COpenMode::Write);
                    CHECK(required.MustExist == true);
                    CHECK(required.MustNotExist == false);
                    CHECK(required.Create == false);
                    CHECK(required.Delete == false);
                    CHECK(required.StartAtEnd == false);
                }
                SECTION("Flags Read, Override, StartAtEnd should result in Read and MustExist, Create, Delete, StartAtEnd actions")
                {
                    ConversionState state(OpenModeFlag::Read, OpenModeFlag::Override, OpenModeFlag::StartAtEnd);
                    state.GetCopenModeAndRequiredActions(copenMode, required);

                    CHECK(copenMode == COpenMode::Read);
                    CHECK(required.MustExist == true);
                    CHECK(required.MustNotExist == false);
                    CHECK(required.Create == true);
                    CHECK(required.Delete == true);
                    CHECK(required.StartAtEnd == true);
                }
                SECTION("Flags Write, Override, StartAtEnd should result in Write and MustNotExist, Create actions")
                {
                    ConversionState state(OpenModeFlag::Write, OpenModeFlag::Create, OpenModeFlag::MustNotExist);
                    state.GetCopenModeAndRequiredActions(copenMode, required);

                    CHECK(copenMode == COpenMode::Write);
                    CHECK(required.MustExist == false);
                    CHECK(required.MustNotExist == true);
                    CHECK(required.Create == true);
                    CHECK(required.Delete == false);
                    CHECK(required.StartAtEnd == false);
                }

            }

            WHEN("State is constructed with additional mode flags should skip them"
                 "and behave like without them")
            {

                SECTION("Flags Read, Text should result in Read and no required actions")
                {
                    ConversionState state(OpenModeFlag::Read, WindowsOpenModeFlag::Text);
                    state.GetCopenModeAndRequiredActions(copenMode, required);

                    CHECK(copenMode == COpenMode::Read);
                    CHECK(required.MustExist == false);
                    CHECK(required.MustNotExist == false);
                    CHECK(required.Create == false);
                    CHECK(required.Delete == false);
                    CHECK(required.StartAtEnd == false);
                }
                SECTION("Flags Read, Write, CommitDirectlyToDisk, Create should result in ReadExtended and Create actions")
                {
                    ConversionState state(OpenModeFlag::Read, OpenModeFlag::Write, WindowsOpenModeFlag::CommitDirectlyToDisk, OpenModeFlag::Create);
                    state.GetCopenModeAndRequiredActions(copenMode, required);

                    CHECK(copenMode == COpenMode::ReadExtended);
                    CHECK(required.MustExist == false);
                    CHECK(required.MustNotExist == false);
                    CHECK(required.Create == true);
                    CHECK(required.Delete == false);
                    CHECK(required.StartAtEnd == false);
                }
                SECTION("Flags Write, Override, DeleteAfterClose, UnicodeEncoding, should result in Write and MustExist actions")
                {
                    ConversionState state(OpenModeFlag::Write, OpenModeFlag::Override, WindowsOpenModeFlag::DeleteAfterClose, WindowsOpenModeFlag::UnicodeEncoding);
                    state.GetCopenModeAndRequiredActions(copenMode, required);

                    CHECK(copenMode == COpenMode::Write);
                    CHECK(required.MustExist == true);
                    CHECK(required.MustNotExist == false);
                    CHECK(required.Create == false);
                    CHECK(required.Delete == false);
                    CHECK(required.StartAtEnd == false);
                }
                SECTION("Flags Read, Override, StartAtEnd, CommitIndirectlyToDisk should result in Read and MustExist, Create, Delete, StartAtEnd actions")
                {
                    ConversionState state(OpenModeFlag::Read, OpenModeFlag::Override, OpenModeFlag::StartAtEnd, WindowsOpenModeFlag::CommitIndirectlyToDisk);
                    state.GetCopenModeAndRequiredActions(copenMode, required);

                    CHECK(copenMode == COpenMode::Read);
                    CHECK(required.MustExist == true);
                    CHECK(required.MustNotExist == false);
                    CHECK(required.Create == true);
                    CHECK(required.Delete == true);
                    CHECK(required.StartAtEnd == true);
                }
            }

            WHEN("State is constructed with invalid combination of flags "
                 "the action should contain both MustExist and MustNotExist (indicates error)")
            {
                SECTION("Flags Read, Write, MustNotExist should result in Read and invaid actions")
                {
                    ConversionState state(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::MustNotExist);
                    state.GetCopenModeAndRequiredActions(copenMode, required);

                    CHECK(copenMode == COpenMode::Read);
                    CHECK(required.MustExist == true);
                    CHECK(required.MustNotExist == true);
                }
                SECTION("Flags Read, MustNotExist, CommitIndirectlyToDisk, Override should result in Read and invaid actions")
                {
                    ConversionState state(OpenModeFlag::Read, OpenModeFlag::MustNotExist, WindowsOpenModeFlag::CommitIndirectlyToDisk, OpenModeFlag::Override);
                    state.GetCopenModeAndRequiredActions(copenMode, required);

                    CHECK(copenMode == COpenMode::Read);
                    CHECK(required.MustExist == true);
                    CHECK(required.MustNotExist == true);
                }
                SECTION("Flags Write, Override, MustNotExist, DeleteAfterClose should result in Read and invaid actions")
                {
                    ConversionState state(OpenModeFlag::Write, OpenModeFlag::Override, OpenModeFlag::MustNotExist, WindowsOpenModeFlag::DeleteAfterClose);
                    state.GetCopenModeAndRequiredActions(copenMode, required);

                    CHECK(copenMode == COpenMode::Read);
                    CHECK(required.MustExist == true);
                    CHECK(required.MustNotExist == true);
                }
            }

        }


        TEMPLATE_TEST_CASE("[ConversionState] : GetCOpenModeStr should return the COpenMode represented in compile time string"
                           "such that if converted to const char * it can be passed into fopen", "[ConversionState][GetCOpenModeStr]", OpenModeTestedTypes)
        {
            using ConversionState = ConversionState<TestType>;
            using OpenModeString = typename ConversionState::OpenModeString;
            using Char8ConstexprStr = typename ConversionState::template ConstexprString<char8>;

            WHEN("Provided with COpenMode::Read the output should be \'r\'")
            {
                const OpenModeString output = ConversionState::GetCOpenModeStr(COpenMode::Read);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("r"));
                REQUIRE(output == expected);
            }
            WHEN("Provided with COpenMode::Write the output should be \'w\'")
            {
                const OpenModeString output = ConversionState::GetCOpenModeStr(COpenMode::Write);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("w"));
                REQUIRE(output == expected);
            }
            WHEN("Provided with COpenMode::Append the output should be \'a\'")
            {
                const OpenModeString output = ConversionState::GetCOpenModeStr(COpenMode::Append);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("a"));
                REQUIRE(output == expected);
            }
            WHEN("Provided with COpenMode::ReadExtended the output should be \'r+\'")
            {
                const OpenModeString output = ConversionState::GetCOpenModeStr(COpenMode::ReadExtended);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("r+"));
                REQUIRE(output == expected);
            }
            WHEN("Provided with COpenMode::WriteExtended the output should be \'w+\'")
            {
                const OpenModeString output = ConversionState::GetCOpenModeStr(COpenMode::WriteExtended);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("w+"));
                REQUIRE(output == expected);
            }
            WHEN("Provided with COpenMode::AppendExtended the output should be \'a+\'")
            {
                const OpenModeString output = ConversionState::GetCOpenModeStr(COpenMode::AppendExtended);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("a+"));
                REQUIRE(output == expected);
            }
        }

        TEMPLATE_TEST_CASE("[ConversionState] : GetAdditionalModeStr should return additional mode string based on the flags passed into constructor", "[ConversionState][GetAdditionalModeStr]", OpenModeTestedTypes)
        {
            using ConversionState = ConversionState<TestType>;
            using OpenModeString = typename ConversionState::OpenModeString;
            using Char8ConstexprStr = typename ConversionState::template ConstexprString<char8>;

            WHEN("Contructed with a single additional flag the additional mode str should contain only"
                 " the according code")
            {
                WHEN("Cosntructed only with OpenModeFlag::Text the output should be \'t\'")
                {
                    ConversionState state(WindowsOpenModeFlag::Text);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("bt"));

                    REQUIRE(output == expected);
                }
                //TODO
                WHEN("Cosntructed only with OpenModeFlag::Translated the output should be empty")
                {
                    ConversionState state(OpenModeFlag::Translated);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr(""));

                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::MustNotExist the output should be \'b\'")
                {
                    ConversionState state(OpenModeFlag::MustNotExist);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("b"));

                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::CommitDirectlyToDisk the output should be \'bc\'")
                {
                    ConversionState state(WindowsOpenModeFlag::CommitDirectlyToDisk);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("bc"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::CommitIndirectlyToDisk the output should be \'bn\'")
                {
                    ConversionState state(WindowsOpenModeFlag::CommitIndirectlyToDisk);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("bn"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::NotInheritedByChildProcess the output should be \'bN\'")
                {
                    ConversionState state(WindowsOpenModeFlag::NotInheritedByChildProcess);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("bN"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::SequntialAccessOptimized the output should be \'bS\'")
                {
                    ConversionState state(WindowsOpenModeFlag::SequntialAccessOptimized);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("bS"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::RandomAccessOptimized the output should be \'bR\'")
                {
                    ConversionState state(WindowsOpenModeFlag::RandomAccessOptimized);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("bR"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::IfPossibleNoFlushingToDisk the output should be \'bT\'")
                {
                    ConversionState state(WindowsOpenModeFlag::IfPossibleNoFlushingToDisk);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("bT"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::DeleteAfterClose the output should be \'bD\'")
                {
                    ConversionState state(WindowsOpenModeFlag::DeleteAfterClose);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("bD"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::UnicodeEncoding the output should be \'b,css=UNICODE\'")
                {
                    ConversionState state(WindowsOpenModeFlag::UnicodeEncoding);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("b,css=UNICODE"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::Utf8Encoding the output should be \'b,css=UTF-8\'")
                {
                    ConversionState state(WindowsOpenModeFlag::Utf8Encoding);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("b,css=UTF-8"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed only with OpenModeFlag::Utf16Encoding the output should be \'b,css=UTF-16LE\'")
                {
                    ConversionState state(WindowsOpenModeFlag::Utf16Encoding);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("b,css=UTF-16LE"));
                    REQUIRE(output == expected);
                }
            }

            WHEN("Contructed with a multiple additional flags the additional mode str should contain"
                 " all of the flag codes concatenated")
            {
                WHEN("Cosntructed with flags Text, RandomAccessOptimized, Translated the output should be \'tR\'")
                {
                    ConversionState state(WindowsOpenModeFlag::Text, WindowsOpenModeFlag::RandomAccessOptimized, OpenModeFlag::Translated);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("tR"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed with flags Translated, Utf8Encoding the output should be \',css=UTF-8\'")
                {
                    ConversionState state(OpenModeFlag::Translated, WindowsOpenModeFlag::Utf8Encoding);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr(",css=UTF-8"));
                    REQUIRE(output == expected);
                }
                //The order shouldnt matter
                WHEN("Cosntructed with flags Translated, Utf8Encoding the output should be \',css=UTF-8\'")
                {
                    ConversionState state(WindowsOpenModeFlag::Utf8Encoding, OpenModeFlag::Translated);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr(",css=UTF-8"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed with flags NotInheritedByChildProcess, RandomAccessOptimized the output should be \'bNR\'")
                {
                    ConversionState state(WindowsOpenModeFlag::NotInheritedByChildProcess, WindowsOpenModeFlag::RandomAccessOptimized);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("bNR"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed with flags Text, CommitDirectlyToDisk, DeleteAfterClose the output should be \'btcD\'")
                {
                    ConversionState state(WindowsOpenModeFlag::Text, WindowsOpenModeFlag::CommitDirectlyToDisk, WindowsOpenModeFlag::DeleteAfterClose);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("btcD"));
                    REQUIRE(output == expected);
                }
            }

            WHEN("Contructed with a additional flags and core flags the additional mode str should contain"
                 " only the additional flag codes cocatenated")
            {
                WHEN("Cosntructed with flags DeleteAfterClose, Read, Write the output should be \'bD\'")
                {
                    ConversionState state(WindowsOpenModeFlag::DeleteAfterClose, OpenModeFlag::Read, OpenModeFlag::Write);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("bD"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed with flags Read, DeleteAfterClose, Append the output should be \'bD\'")
                {
                    ConversionState state(OpenModeFlag::Read, WindowsOpenModeFlag::DeleteAfterClose, OpenModeFlag::StartAtEnd);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("bD"));
                    REQUIRE(output == expected);
                }
                WHEN("Cosntructed with flags Translated, RandomAccessOptimized, Write, DeleteAfterClose, MustExist the output should be \'bD\'")
                {
                    ConversionState state(OpenModeFlag::Translated, WindowsOpenModeFlag::RandomAccessOptimized, OpenModeFlag::Write, WindowsOpenModeFlag::DeleteAfterClose);
                    OpenModeString output = state.GetAdditionalModeStr();
                    OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("RD"));
                    REQUIRE(output == expected);
                }
            }
        }
    }


    namespace OpenModeClassTesting
    {
        TEST_CASE("[OpenMode] : Only char and wchar_t specialisations should compile", "[OpenMode][Compilation]")
        {
            [[maybe_unused]] BasicOpenMode<char8> mode8;
            [[maybe_unused]] BasicOpenMode<charW> modeW;

            #if OpenMode_TEST_COMPILE_ERROR == true
            //Should not compile
            [[maybe_unused]] BasicOpenMode<int> modeInt;
            [[maybe_unused]] BasicOpenMode<double> modeDouble;
            [[maybe_unused]] BasicOpenMode<short> modeShort;
            [[maybe_unused]] BasicOpenMode<long> modeLing;
            [[maybe_unused]] BasicOpenMode<std::string> modeString;
            [[maybe_unused]] BasicOpenMode<std::wstring> modeWString;
            #endif
        }

        TEMPLATE_TEST_CASE("[OpenMode] : Default constructor should create an openmode with invalid Validty", "[OpenMode][Default constructor]", OpenModeTestedTypes)
       {
            const OpenMode mode;
            REQUIRE(mode.IsValid() == false);
        }


        TEMPLATE_TEST_CASE("[OpenMode] : Constructor with COpenMode should crete always valid openmode with appropriate OpenModeString, Enabled and Required Actions", "[OpenMode][Constructor]", OpenModeTestedTypes)
        {
            using OpenMode = BasicOpenMode<TestType>;
            using OpenModeString = typename OpenMode::OpenModeString;
            using Char8ConstexprStr = typename OpenMode::template ConstexprString<char8>;

            WHEN("Provided with COpenMode::Read the OpenModeStr should be \'r\'")
            {
                const OpenMode mode(COpenMode::Read);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("r"));

                EnabledOperations enabled = mode.GetEnabledOperations();
                RequiredActions req = mode.GetRequiredActions();

                CHECK(mode.IsValid() == true);

                CHECK(enabled == EnabledOperations::Read);

                CHECK(req.MustExist == false);
                CHECK(req.MustNotExist == false);
                CHECK(req.Create == false);
                CHECK(req.Delete == false);
                CHECK(req.StartAtEnd == false);

                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::Write the OpenModeStr should be \'w\'")
            {
                const OpenMode mode(COpenMode::Write);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("w"));

                EnabledOperations enabled = mode.GetEnabledOperations();
                RequiredActions req = mode.GetRequiredActions();

                CHECK(mode.IsValid() == true);

                CHECK(enabled == EnabledOperations::Write);

                CHECK(req.MustExist == false);
                CHECK(req.MustNotExist == false);
                CHECK(req.Create == false);
                CHECK(req.Delete == false);
                CHECK(req.StartAtEnd == false);

                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::Append the OpenModeStr should be \'a\'")
            {
                const OpenMode mode(COpenMode::Append);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("a"));

                EnabledOperations enabled = mode.GetEnabledOperations();
                RequiredActions req = mode.GetRequiredActions();

                CHECK(mode.IsValid() == true);

                CHECK(enabled == EnabledOperations::Write);

                CHECK(req.MustExist == false);
                CHECK(req.MustNotExist == false);
                CHECK(req.Create == false);
                CHECK(req.Delete == false);
                CHECK(req.StartAtEnd == false);

                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::ReadExtended the OpenModeStr should be \'r+\'")
            {
                const OpenMode mode(COpenMode::ReadExtended);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("r+"));

                EnabledOperations enabled = mode.GetEnabledOperations();
                RequiredActions req = mode.GetRequiredActions();

                CHECK(mode.IsValid() == true);

                CHECK(enabled == EnabledOperations::ReadWrite);

                CHECK(req.MustExist == true);
                CHECK(req.MustNotExist == false);
                CHECK(req.Create == false);
                CHECK(req.Delete == false);
                CHECK(req.StartAtEnd == false);

                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::WriteExtended the OpenModeStr should be \'w+\'")
            {
                const OpenMode mode(COpenMode::WriteExtended);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("w+"));

                EnabledOperations enabled = mode.GetEnabledOperations();
                RequiredActions req = mode.GetRequiredActions();

                CHECK(mode.IsValid() == true);

                CHECK(enabled == EnabledOperations::ReadWrite);

                CHECK(req.MustExist == false);
                CHECK(req.MustNotExist == false);
                CHECK(req.Create == false);
                CHECK(req.Delete == false);
                CHECK(req.StartAtEnd == false);

                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::AppendExtended the OpenModeStr should be \'a+\'")
            {
                const OpenMode mode(COpenMode::AppendExtended);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("a+"));

                EnabledOperations enabled = mode.GetEnabledOperations();
                RequiredActions req = mode.GetRequiredActions();

                CHECK(mode.IsValid() == true);

                CHECK(enabled == EnabledOperations::ReadWrite);

                CHECK(req.MustExist == false);
                CHECK(req.MustNotExist == false);
                CHECK(req.Create == false);
                CHECK(req.Delete == false);
                CHECK(req.StartAtEnd == false);

                REQUIRE(mode.GetOpenModeString() == expected);
            }
        }

/*
        TEMPLATE_TEST_CASE("[OpenMode] : AssignCOpenMode should change the openmode to always valid openmode with appropriate OpenModeString", "[OpenMode][AssignCOpenMode]", OpenModeTestedTypes)
        {
            using OpenMode = BasicOpenMode<TestType>;
            using OpenModeString = typename OpenMode::OpenModeString;
            using Char8ConstexprStr = typename OpenMode::template ConstexprString<char8>;

            OpenMode mode;

            WHEN("Provided with COpenMode::Read the OpenModeStr should be \'r\'")
            {
                mode.AssignCOpenMode(COpenMode::Read);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("r"));

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::Write the OpenModeStr should be \'w\'")
            {
                mode.AssignCOpenMode(COpenMode::Write);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("w"));

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::Append the OpenModeStr should be \'a\'")
            {
                mode.AssignCOpenMode(COpenMode::Append);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("a"));

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::ReadExtended the OpenModeStr should be \'r+\'")
            {
                mode.AssignCOpenMode(COpenMode::ReadExtended);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("r+"));

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::WriteExtended the OpenModeStr should be \'w+\'")
            {
                mode.AssignCOpenMode(COpenMode::WriteExtended);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("w+"));

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
            WHEN("Provided with COpenMode::AppendExtended the OpenModeStr should be \'a+\'")
            {
                mode.AssignCOpenMode(COpenMode::AppendExtended);
                const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("a+"));

                CHECK(mode.IsValid() == true);
                REQUIRE(mode.GetOpenModeString() == expected);
            }
        }
*/

        TEMPLATE_TEST_CASE("[OpenMode] : Constructor called with individual OpenModeFlags should create a open mode according to the flags passed", "[OpenMode][Constructor]", OpenModeTestedTypes)
        {
            using OpenMode = BasicOpenMode<TestType>;
            using OpenModeString = typename OpenMode::OpenModeString;
            using Char8ConstexprStr = typename OpenMode::template ConstexprString<char8>;
            using OsStringView = typename OpenMode::OsStringView;

            WHEN("Called with valid flags it should construct a valid openmode")
            {
                WHEN("Provided with flags Read MustExist RandomAccessOptimized the OpenModeStr should be \'rbR\'"
                     " and it should be valid")
                {
                    const OpenMode mode(OpenModeFlag::Read, WindowsOpenModeFlag::RandomAccessOptimized);

                    const OpenModeString output = mode.GetOpenModeString();
                    const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("rbR"));

                    const OsStringView outputView = output;
                    const OsStringView expectedView = expected;

                    EnabledOperations enabled = mode.GetEnabledOperations();
                    RequiredActions req = mode.GetRequiredActions();

                    CHECK(mode.IsValid() == true);

                    CHECK(enabled == EnabledOperations::Read);

                    CHECK(req.MustExist == false);
                    CHECK(req.MustNotExist == false);
                    CHECK(req.Create == false);
                    CHECK(req.Delete == false);
                    CHECK(req.StartAtEnd == false);

                    CHECK(outputView == expectedView);

                    CHECK(output == expected);
                }
                WHEN("Provided with flags Read, Write, MustExist, Create, StartAtEnd the OpenModeStr should be \'r+b\'"
                     " and it should be valid")
                {
                    const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Create, OpenModeFlag::StartAtEnd);

                    const OpenModeString output = mode.GetOpenModeString();
                    const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("r+b"));

                    const OsStringView outputView = output;
                    const OsStringView expectedView = expected;

                    EnabledOperations enabled = mode.GetEnabledOperations();
                    RequiredActions req = mode.GetRequiredActions();

                    CHECK(mode.IsValid() == true);

                    CHECK(enabled == EnabledOperations::ReadWrite);

                    CHECK(req.MustExist == false);
                    CHECK(req.MustNotExist == false);
                    CHECK(req.Create == true);
                    CHECK(req.Delete == false);
                    CHECK(req.StartAtEnd == true);

                    CHECK(outputView == expectedView);

                    CHECK(output == expected);
                }
                WHEN("Provided with flags Write, Create, MustNotExist, CommitDirectlyToDisk, SequntialAccessOptimized the OpenModeStr should be \'wbcS\'"
                     " and it should be valid")
                {
                    const OpenMode mode(OpenModeFlag::Write, OpenModeFlag::Create, OpenModeFlag::MustNotExist, WindowsOpenModeFlag::CommitDirectlyToDisk, WindowsOpenModeFlag::SequntialAccessOptimized);

                    const OpenModeString output = mode.GetOpenModeString();
                    const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("wbcS"));

                    const OsStringView outputView = output;
                    const OsStringView expectedView = expected;
                    EnabledOperations enabled = mode.GetEnabledOperations();
                    RequiredActions req = mode.GetRequiredActions();

                    CHECK(mode.IsValid() == true);

                    CHECK(enabled == EnabledOperations::Write);

                    CHECK(req.MustExist == false);
                    CHECK(req.MustNotExist == true);
                    CHECK(req.Create == true);
                    CHECK(req.Delete == false);
                    CHECK(req.StartAtEnd == false);

                    CHECK(outputView == expectedView);

                    CHECK(output == expected);
                }
                WHEN("Provided with flags Read, Translated, NotInheritedByChildProcess, DeleteAfterClose, Utf8Encoding the OpenModeStr should be \'rND,css=UTF-8\'"
                     " and it should be valid")
                {
                    const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Translated, WindowsOpenModeFlag::NotInheritedByChildProcess, WindowsOpenModeFlag::DeleteAfterClose, WindowsOpenModeFlag::Utf8Encoding);

                    const OpenModeString output = mode.GetOpenModeString();
                    const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("rND,css=UTF-8"));

                    const OsStringView outputView = output;
                    const OsStringView expectedView = expected;
                    EnabledOperations enabled = mode.GetEnabledOperations();
                    RequiredActions req = mode.GetRequiredActions();

                    CHECK(mode.IsValid() == true);

                    CHECK(enabled == EnabledOperations::Read);

                    CHECK(req.MustExist == false);
                    CHECK(req.MustNotExist == false);
                    CHECK(req.Create == false);
                    CHECK(req.Delete == false);
                    CHECK(req.StartAtEnd == false);

                    CHECK(outputView == expectedView);

                    CHECK(output == expected);
                }
            }

            WHEN("Called with conflicting flags (according to ConversionState::AreAreFlagsConflicting) it should construct a invalid openmode")
            {
                WHEN("Provided with flags Read, MustNotExist it should be invalid")
                {
                    const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::MustNotExist);
                    CHECK(mode.IsValid() == false);
                }
                WHEN("Provided with flags Read, Write, Text it should be invalid")
                {
                    const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write, WindowsOpenModeFlag::Text);
                    CHECK(mode.IsValid() == false);
                }
                WHEN("Provided with flags Write, UnicodeEncoding, Read, Utf8Encoding it should be invalid")
                {
                    const OpenMode mode(OpenModeFlag::Write, WindowsOpenModeFlag::UnicodeEncoding, OpenModeFlag::Read, WindowsOpenModeFlag::Utf8Encoding);
                    CHECK(mode.IsValid() == false);
                }
                WHEN("Provided with flags Write, Append, CommitDirectlyToDisk, CommitIndirectlyToDisk it should be invalid")
                {
                    const OpenMode mode(OpenModeFlag::Write, OpenModeFlag::StartAtEnd, WindowsOpenModeFlag::CommitDirectlyToDisk, WindowsOpenModeFlag::CommitIndirectlyToDisk);
                    CHECK(mode.IsValid() == false);
                }
                WHEN("Provided with flags Read, SequntialAccessOptimized, RandomAccessOptimized it should be invalid")
                {
                    const OpenMode mode(OpenModeFlag::Read, WindowsOpenModeFlag::SequntialAccessOptimized, WindowsOpenModeFlag::RandomAccessOptimized);
                    CHECK(mode.IsValid() == false);
                }
            }

            WHEN("Called with invalid flags (insufficient information) it should construct a invalid openmode")
            {
                WHEN("Provided with no flags it should be invalid")
                {
                    const OpenMode mode;
                    CHECK(mode.IsValid() == false);
                }
            }
        }


        TEMPLATE_TEST_CASE("[OpenMode] : IsValid shoudl return true if the openmode is valid (fit to open a file)", "[OpenMode][IsValid]", OpenModeTestedTypes)
        {
            using OpenMode = BasicOpenMode<TestType>;
            WHEN("A constructed mode is valid IsValid returns true")
            {
                const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write);
                REQUIRE(mode.IsValid() == true);
            }
            WHEN("A constructed mode is invalid IsValid returns false")
            {
                const OpenMode mode(WindowsOpenModeFlag::Text, OpenModeFlag::MustNotExist);
                REQUIRE(mode.IsValid() == false);
            }
        }


        TEMPLATE_TEST_CASE("[OpenMode] : GetOpenModeString should return a copy of the constexpr openmode string", "[OpenMode][GetOpenModeString]", OpenModeTestedTypes)
        {
            using OpenMode = BasicOpenMode<TestType>;
            using OpenModeString = typename OpenMode::OpenModeString;
            using Char8ConstexprStr = typename OpenMode::template ConstexprString<char8>;

            const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write);
            REQUIRE(mode.IsValid() == true);

            const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("r+b"));
            REQUIRE(mode.GetOpenModeString() == expected);
        }

        TEMPLATE_TEST_CASE("[OpenMode] : GetCOpenMode shoudl return the underlying COpenMode", "[OpenMode][IsValid]", OpenModeTestedTypes)
        {
            using OpenMode = BasicOpenMode<TestType>;
            WHEN("A constructed mode is valid GetCOpenMode returns the used COpenMode")
            {
                {
                    const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Override);
                    REQUIRE(mode.GetCOpenMode() == COpenMode::WriteExtended);
                }
                {
                    const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Create);
                    REQUIRE(mode.GetCOpenMode() == COpenMode::Read);
                }
                {
                    const OpenMode mode(OpenModeFlag::Write, OpenModeFlag::StartAtEnd);
                    REQUIRE(mode.GetCOpenMode() == COpenMode::ReadExtended);
                }

            }
            WHEN("A constructed mode is invalid GetCOpenMode returns ReadExtended (default COpenMode)")
            {
                const OpenMode mode(WindowsOpenModeFlag::Text, OpenModeFlag::MustNotExist);
                REQUIRE(mode.GetCOpenMode() == COpenMode::ReadExtended);
            }
        }

        TEMPLATE_TEST_CASE("[OpenMode] : GetEnabledOperations shoudl return the actions enebled on the file opened with this openmode", "[OpenMode][IsValid]", OpenModeTestedTypes)
        {
            using OpenMode = BasicOpenMode<TestType>;
            WHEN("A constructed mode is valid GetEnabledOperations returns the enebled actions")
            {
                {
                    const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Override);
                    REQUIRE(mode.GetEnabledOperations() == EnabledOperations::ReadWrite);
                }
                {
                    const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Create);
                    REQUIRE(mode.GetEnabledOperations() == EnabledOperations::Read);
                }
                {
                    const OpenMode mode(OpenModeFlag::Write, OpenModeFlag::StartAtEnd);
                    REQUIRE(mode.GetEnabledOperations() == EnabledOperations::Write);
                }
                {
                    const OpenMode mode(OpenModeFlag::StartAtEnd, OpenModeFlag::Create);
                    REQUIRE(mode.GetEnabledOperations() == EnabledOperations::None);
                }

            }
            WHEN("A constructed mode is invalid GetEnabledOperations returns Closed")
            {
                const OpenMode mode(WindowsOpenModeFlag::Text, OpenModeFlag::MustNotExist);
                REQUIRE(mode.GetEnabledOperations() == EnabledOperations::Closed);
            }
        }

        TEMPLATE_TEST_CASE("[OpenMode] : GetRequiredActions shoudl return the actions taht need to be performed when opening a file with this openmode", "[OpenMode][IsValid]", OpenModeTestedTypes)
        {
            using OpenMode = BasicOpenMode<TestType>;
            RequiredActions required;
            WHEN("A constructed mode is valid GetRequiredActions returns the required actions")
            {
                {
                    const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write, OpenModeFlag::Override);
                    required = mode.GetRequiredActions();
                    CHECK(required.MustExist == true);
                    CHECK(required.MustNotExist == false);
                    CHECK(required.Create == false);
                    CHECK(required.Delete == false);
                    CHECK(required.StartAtEnd == false);
                }
                {
                    const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Create);
                    required = mode.GetRequiredActions();
                    CHECK(required.MustExist == false);
                    CHECK(required.MustNotExist == false);
                    CHECK(required.Create == true);
                    CHECK(required.Delete == false);
                    CHECK(required.StartAtEnd == false);
                }
                {
                    const OpenMode mode(OpenModeFlag::Write, OpenModeFlag::StartAtEnd);
                    required = mode.GetRequiredActions();
                    CHECK(required.MustExist == true);
                    CHECK(required.MustNotExist == false);
                    CHECK(required.Create == false);
                    CHECK(required.Delete == false);
                    CHECK(required.StartAtEnd == true);
                }
                {
                    const OpenMode mode(OpenModeFlag::StartAtEnd, OpenModeFlag::Create);
                    required = mode.GetRequiredActions();
                    CHECK(required.MustExist == false);
                    CHECK(required.MustNotExist == false);
                    CHECK(required.Create == true);
                    CHECK(required.Delete == false);
                    CHECK(required.StartAtEnd == true);
                }

            }
            WHEN("A constructed mode is invalid GetRequiredActions returns no actions")
            {
                const OpenMode mode(WindowsOpenModeFlag::Text, OpenModeFlag::MustNotExist);
                required = mode.GetRequiredActions();
                CHECK(required.MustExist == false);
                CHECK(required.MustNotExist == false);
                CHECK(required.Create == false);
                CHECK(required.Delete == false);
                CHECK(required.StartAtEnd == false);
            }
        }

        TEMPLATE_TEST_CASE("[OpenMode] : operator StringView<OsCharType> should return a stringview conatining just the openmode", "[OpenMode][operator StringView<OsCharType>]", OpenModeTestedTypes)
        {
            using OpenMode = BasicOpenMode<TestType>;
            using OpenModeString = typename OpenMode::OpenModeString;
            using OsStringView = typename OpenMode::OsStringView;
            using Char8ConstexprStr = typename OpenMode::template ConstexprString<char8>;

            const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write);
            REQUIRE(mode.IsValid() == true);

            const OsStringView output = mode;
            const OpenModeString expected = PromoteStringCharsTo<TestType>(Char8ConstexprStr("r+b"));
            const OsStringView expectedView = expected;
            REQUIRE(output == expectedView);
        }

        TEMPLATE_TEST_CASE("[OpenMode] : Copy constructor should construct a copy of the provided open mode", "[OpenMode][Copy constructor]", OpenModeTestedTypes)
        {
            using OpenMode = BasicOpenMode<TestType>;
            WHEN("Constructing from an empty invalid openmode the result should be empty and invalid aswell")
            {
                const OpenMode mode;
                const OpenMode constructed(mode);

                REQUIRE(constructed.IsValid() == false);
            }
            WHEN("Constructing from an valid openmode the result should be valid and have the same OpenModeStr")
            {
                const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write);
                const OpenMode constructed(mode);

                REQUIRE(constructed.GetOpenModeString() == mode.GetOpenModeString());
                REQUIRE(constructed.IsValid() == true);
            }
        }

        TEMPLATE_TEST_CASE("[OpenMode] : Move constructor should construct a copy of the provided open mode", "[OpenMode][Move constructor]", OpenModeTestedTypes)
        {
            using OpenMode = BasicOpenMode<TestType>;
            WHEN("Constructing from an empty invalid openmode the result should be empty and invalid aswell")
            {
                OpenMode mode;
                const auto beforeStr = mode.GetOpenModeString();
                const OpenMode constructed(std::move(mode));

                REQUIRE(constructed.IsValid() == false);

                //The result should not chnage
                REQUIRE(mode.GetOpenModeString() == beforeStr);
                REQUIRE(mode.IsValid() == false);
            }
            WHEN("Constructing from an valid openmode the result should be valid and have the same OpenModeStr")
            {
                OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write);
                const auto beforeStr = mode.GetOpenModeString();
                const OpenMode constructed(std::move(mode));

                REQUIRE(constructed.GetOpenModeString() == mode.GetOpenModeString());
                REQUIRE(constructed.IsValid() == true);

                //The result should not chnage
                REQUIRE(mode.GetOpenModeString() == beforeStr);
                REQUIRE(mode.IsValid() == true);
            }
        }

        TEMPLATE_TEST_CASE("[OpenMode] : Copy assigning to OpenMode should copy the provided OpenMode", "[OpenMode][Copy assignment]", OpenModeTestedTypes)
        {
            using OpenMode = BasicOpenMode<TestType>;
            WHEN("Assigning from an empty invalid openmode the result should be empty and invalid aswell")
            {
                const OpenMode mode;
                OpenMode assigned;

                assigned = mode;

                REQUIRE(assigned.IsValid() == false);
            }
            WHEN("Assigning from an valid openmode the result should be valid and have the same OpenModeStr")
            {
                const OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write);
                OpenMode assigned;

                assigned = mode;

                REQUIRE(assigned.GetOpenModeString() == mode.GetOpenModeString());
                REQUIRE(assigned.IsValid() == true);
            }
        }

        TEMPLATE_TEST_CASE("[OpenMode] : Move assigning to OpenMode should copy the provided OpenMode", "[OpenMode][Move assignment]", OpenModeTestedTypes)
        {
            using OpenMode = BasicOpenMode<TestType>;
            WHEN("Assigning from an empty invalid openmode the result should be empty and invalid aswell")
            {
                OpenMode mode;
                OpenMode assigned;
                const auto beforeStr = mode.GetOpenModeString();

                assigned = std::move(mode);

                REQUIRE(assigned.IsValid() == false);

                //The result should not chnage
                REQUIRE(mode.GetOpenModeString() == beforeStr);
                REQUIRE(mode.IsValid() == false);
            }
            WHEN("Assigning from an valid openmode the result should be valid and have the same OpenModeStr")
            {
                OpenMode mode(OpenModeFlag::Read, OpenModeFlag::Write);
                OpenMode assigned;
                const auto beforeStr = mode.GetOpenModeString();

                assigned = std::move(mode);

                REQUIRE(assigned.GetOpenModeString() == mode.GetOpenModeString());
                REQUIRE(assigned.IsValid() == true);

                //The result should not chnage
                REQUIRE(mode.GetOpenModeString() == beforeStr);
                REQUIRE(mode.IsValid() == true);
            }

        }
    }

}

#endif // OPENMODETESTING_H
