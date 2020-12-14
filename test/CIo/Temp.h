#ifndef TEMP_H
#define TEMP_H

#include "Catch2/Catch.hpp"
#include "cplusplus.h"
#include "Blocks.h"



namespace cio::Testing
{
    using namespace Blocks;

    inline Bool IsCharAtSpace(const Block* string, Size index)
    {
        return (isspace(At(string, index, char)) != 0);
    }

    typedef Block BlockBlock ;

    void AppendStringPart(BlockBlock * to, BlockView part)
    {
        GrowBlockToFit(to, to->ByteSize + sizeof (BlockView));
        RawAt(to, to->ByteSize, BlockView) = part;
        SetSize(to, to->ByteSize + sizeof (BlockView), char);
    }


    void SplitIntoWords(Block* string, BlockBlock* words)
    {
        Size lastNonSpace = 0;
        Bool hasNewWord = FALSE;

        Size charSize = GetSize(string, char);
        for(Size i = 0; i < charSize; i++)
        {
            if(hasNewWord)
            {
                if(IsCharAtSpace(string, i))
                {
                    Size len = i - lastNonSpace;
                    BlockView newPart = CreateBlockView(&At(string, lastNonSpace, char), len, len);
                    AppendStringPart(words, newPart);

                    hasNewWord = FALSE;
                }

            }
            else
            {
                if(IsCharAtSpace(string, i) == FALSE)
                {
                    lastNonSpace = i;
                    hasNewWord = TRUE;
                }
            }
        }

        if(hasNewWord)
        {
            Size len = charSize - lastNonSpace;
            BlockView newPart = CreateBlockView(&At(string, lastNonSpace, char), len, len);
            AppendStringPart(words, newPart);
        }
    }

    void GetKeyChars(BlockView* msg, Block* keyChars)
    {
        Size msgLen = GetSize(msg, char);
        //Counts how many of each char there are
        for(Size i = 0; i < msgLen; i++)
        {
            char at = At(msg, i, char);
            At(keyChars, at, int) ++;
        }

        Size keyCharsSize = GetSize(keyChars, int);
        for(Size i = 0; i < keyCharsSize; i++)
        {
            int* at = &At(keyChars, i, int);
            if(*at > 1)
                *at = TRUE;
            else
                *at = FALSE;
        }
    }



    TEST_CASE("Testing of algorhitm", "[Everything]")
    {
        //struct Block msg = CreateEmptyBlock();
        //struct Block dict = CreateEmptyBlock();
        //struct Block output = CreateEmptyBlock();


    }

    void callerfunction()
    {
        int storage[257] = {0};
        Block block = CreateBlock(storage, IntI(256), IntI(256));

        At(&block, 256, int) = 3;
    }

    TEST_CASE("[GetKeyChars] : GetKeyChars should retrieve which chars repeat", "[GetKeyChars]")
    {

        {
            int keyCharsStorage[257] = {0};

            struct BlockView input = ToStringBlockView(" aa xx abcdefghijklmn 13423 bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");

            struct Block keyChars = CreateBlock((void*)keyCharsStorage, IntI(256), IntI(256));

            GetKeyChars(&input, &keyChars);

            puts("");
            At(&keyChars, 256, int) = 3;
            puts("");
            callerfunction();

            CHECK(At(&keyChars, (Size)'a', int) == 1);
            CHECK(At(&keyChars, (Size)'b', int) == 1);
            CHECK(At(&keyChars, (Size)'c', int) == 0);
            CHECK(At(&keyChars, (Size)'d', int) == 0);
            CHECK(At(&keyChars, (Size)'e', int) == 0);
            CHECK(At(&keyChars, (Size)'f', int) == 0);
            CHECK(At(&keyChars, (Size)'g', int) == 0);
            CHECK(At(&keyChars, (Size)'h', int) == 0);
            CHECK(At(&keyChars, (Size)'i', int) == 0);
            CHECK(At(&keyChars, (Size)'j', int) == 0);
            CHECK(At(&keyChars, (Size)'k', int) == 0);
            CHECK(At(&keyChars, (Size)'l', int) == 0);
            CHECK(At(&keyChars, (Size)'m', int) == 0);
            CHECK(At(&keyChars, (Size)'n', int) == 0);
            CHECK(At(&keyChars, (Size)'1', int) == 0);
            CHECK(At(&keyChars, (Size)'2', int) == 0);
            CHECK(At(&keyChars, (Size)'3', int) == 1);
            CHECK(At(&keyChars, (Size)'4', int) == 0);
            CHECK(At(&keyChars, (Size)'x', int) == 1);
            CHECK(At(&keyChars, (Size)' ', int) == 1);
        }
    }

}

#endif // TEMP_H
