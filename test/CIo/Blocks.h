#ifndef BLOCKS_H
#define BLOCKS_H

#include "Catch2/Catch.hpp"

#include <cstdlib>
#include <math.h>


namespace Blocks
{

#define SAFE_BLOCKS //Setts if blocks should use safe mode - when commented out there should be 0 runtime overhead
//#define BLOCK_TEST

#define INCLUDE_BLOCKS          //Base functionality - Functions for working with blocks with added safety as defined by SAFE_BLOCKS
#define INCLUDE_BLOCKS_ALLOC    //Allocating of blocks - malloc, realloc, free for blocks
#define INCLUDE_BLOCKS_COPY     //Copying functions - Copies certain ammount of data from one block to another
#define INCLUDE_BLOCKS_STRING   //String functions - Null terminated blocks, concatenating blocks, and string literal blocks
#define INCLUDE_BLOCKS_TYPE_SUPPORT //Support for many other types in the form of I and IFrom specialistaions

//Definitions
#if defined (INCLUDE_BLOCKS)

#define TRUE 1
#define FALSE 0

#ifdef BLOCK_TEST
static char Error = false;
#define BLOCK_TEST_REF_VAR Bool* Error
#define BLOCK_TEST_INIT_REF_VAR(block) block->Error = NULL
#define CHECK_INDEX(index, capacity, indexName, functionName, file, line, function) \
    if((index) >= (capacity)) \
        Error = TRUE; \
    else \
        Error = FALSE;

#else
#define BLOCK_TEST_REF_VAR

#if defined(SAFE_BLOCKS) && ! defined (BLOCK_TEST)

#define CHECK_INDEX(index, elemSize, capacity, indexName, functionName, file, line, function) \
    if((index) + (elemSize) > (capacity)) \
    { \
        puts("[Block::" functionName "]: Invalid arguments: acessed " indexName " exceeding capacity"); \
        printf("[Block::" functionName "]: " indexName ": %u\n", (Size)(index)); \
        printf("[Block::" functionName "]: sizeof type: %u\n", (Size)(elemSize)); \
        printf("[Block::" functionName "]: capacity: %u\n", (Size)(capacity)); \
        printf("--> %s (%d) %s(...)\n", file, line, function); \
    }

#else
#define CHECK_INDEX(index, capacity, indexName, functionName, file, line, function)
#endif
#endif

#define CHECK_INDEX_NO_INFO(index, elemSize, capacity, indexName, functionName) \
    CHECK_INDEX(index, elemSize, capacity, indexName, functionName, "???", 0, "???")

    //Types
    typedef unsigned int Size;
    typedef double       Decimal;
    typedef char         Bool;
    typedef unsigned char byte;

    //Base
    struct Block
    {
            byte * Data;
            Size ByteSize;
            Size ByteCapacity;
    };
    struct BlockView
    {
            const byte * Data;
            Size ByteSize;
            Size ByteCapacity;
    };




#define I(index, type) ((index) * sizeof (type))
#define IFrom(byteIndex, type) ((byteIndex) / sizeof (type))

    //Peromance saving
    #ifdef SAFE_BLOCKS
    inline Size InternalIdentityAndCheck(Size index, Size elemSize, Size capacity,
                                         const char * file, int line, const char* function)
    {
        CHECK_INDEX(index, elemSize, capacity, "index", "At",
                    file, line, function);
        return index;
    }
    inline void InternalSetAndCheck(Size * what, Size to, Size elemSize, Size capacity,
                                    const char * file, int line, const char* function)
    {
        CHECK_INDEX(*what, elemSize, capacity, "size", "SetSize",
                    file, line, function);
        *what = to;
    }
    #define IDENTITY_AND_CHECK(index, elemSize, capacity)             InternalIdentityAndCheck(index, elemSize, capacity, __FILE__, __LINE__, __func__)
    #define IDENTITY_SET_AND_CHECK(what, to, elemSize, capacity)  InternalSetAndCheck(&(what), to, elemSize, capacity, __FILE__, __LINE__, __func__)
    #else
    #define IDENTITY_AND_CHECK(index, elemSize, capacity) (index)
    #define IDENTITY_SET_AND_CHECK(what, to, elemSize, capacity) (what) = (to)
    #endif

    #define CAST_PTR(ptr, type) (type*)(void*)(ptr)
    #define RawAt(anyBlock, index, type)    (*CAST_PTR( (anyBlock)->Data + IDENTITY_AND_CHECK((index), sizeof (type), (anyBlock)->ByteCapacity), type ))
    #define At(anyBlock, index, type)       RawAt(anyBlock, (index) * sizeof (type), type)
    #define GetSize(anyBlock, type)         IFrom((anyBlock)->ByteSize, type)
    #define SetSize(anyBlock, to, type)     IDENTITY_SET_AND_CHECK((anyBlock)->ByteSize, I(to, type), 0, (anyBlock)->ByteCapacity)
    #define GetCapacity(anyBlock, type) I   From((anyBlock)->ByteSize, type)
    #define SetCapacity(anyBlock, to, type) (anyBlock)->ByteCapacity = I(to, type)

    struct Block CreateBlock(void * data, Size byteSize, Size byteCapacity)
    {
        struct Block block;
        block.Data = (byte*)data;
        block.ByteSize = byteSize;
        block.ByteCapacity = byteCapacity;
        CHECK_INDEX_NO_INFO(byteSize, 0, block.ByteCapacity, "byteSize", "CreateBlock");

        return block;
    } 
    struct BlockView CreateBlockView(const void * data, Size byteSize, Size byteCapacity)
    {
        struct BlockView block;
        block.Data = (byte*)data;
        block.ByteSize = byteSize;
        block.ByteCapacity = byteCapacity;
        CHECK_INDEX_NO_INFO(byteSize, 0, block.ByteCapacity, "byteSize", "CreateBlockView");

        return block;
    }

    struct Block CreateEmptyBlock()
    {
        return CreateBlock(NULL, 0, 0);
    }
    struct BlockView CreateEmptyBlockView()
    {
        return CreateBlockView(NULL, 0, 0);
    }

    inline struct BlockView ToBlockView(const struct Block * block)
    {
        return CreateBlockView(block->Data, block->ByteSize, block->ByteCapacity);
    }   

#if defined (INCLUDE_BLOCKS_ALLOC)
    inline struct Block AllocateBlock(Size byteCapacity)
    {
        byte * allocated = (byte*)malloc(byteCapacity*sizeof(byte));
        return CreateBlock(allocated, 0, byteCapacity);
    }
        
    inline void ReallocateBlock(struct Block* block, Size newCapacity)
    {
        block->Data = (byte*)realloc(block->Data, newCapacity);
        block->ByteCapacity = newCapacity;
    }    

    #define LOG_2 0.69314718055995
    Size CalculateGrowthFactor(Size currentSize, Size minNewSize)
    {
        //Decimal log2 = log_2( toFits / block->ByteCapacity );
        const Decimal log2 = log((Decimal)minNewSize / (Decimal)currentSize) / LOG_2;
        const Decimal ceiled = ceil(log2);
        const Size finalMultiply = (Size)pow(2, ceiled);
        return finalMultiply;
    }

    Bool GrowBlockToFit(struct Block* block, Size toFits)
    {
        if(block->ByteCapacity >= toFits)
            return FALSE;
        
        if(block->ByteCapacity == 0)
        {
            ReallocateBlock(block, toFits);
            return TRUE;
        }

        ReallocateBlock(block, block->ByteCapacity*CalculateGrowthFactor(block->ByteCapacity, toFits));
        return TRUE;
    }

    inline void FreeBlock(struct Block* block)
    {
        free(block->Data);
    }
#endif

#endif
#if defined (INCLUDE_BLOCKS_COPY)
    //Copy block and return if all data was copied
    Bool CopyTo(Block * to, const void* from, Size fromSize, Size byteIndex)
    {
        if(byteIndex >= to->ByteCapacity)
            return FALSE;

        const Size copyToSize = to->ByteCapacity - byteIndex;
        const Size copyFromSize = fromSize;

        Size copySize;
        Bool returnStatus;

        if(copyToSize < copyFromSize)
        {
            //Is not able to copy the whole data - is limited
            // by the to->ByteCapacity
            copySize = copyToSize;
            returnStatus = FALSE;
        }
        else
        {
            copySize = copyFromSize;
            returnStatus = TRUE;
        }

        memcpy(to->Data + byteIndex, from, copySize);

        //If the position at which the copying finished is outside the
        // the size associated with this block
        // -> increases it to the position;
        Size finishedAt = copySize + byteIndex;
        if(finishedAt > to->ByteSize)
            to->ByteSize = finishedAt;

        return returnStatus;
    }
    inline Bool Append(struct Block* to, void * what, Size whatSize)
    {
        return CopyTo(to, what, whatSize, to->ByteSize);
    }

    inline Bool CopyBlockTo(struct Block* to, const struct Block* from, Size byteIndex)
    {
        CHECK_INDEX_NO_INFO(byteIndex, 1, to->ByteCapacity, "byteIndex", "CopyBlockTo");
        return CopyTo(to, from->Data, from->ByteSize, byteIndex);
    }
    inline Bool CopyBlockViewTo(struct Block* to, const struct BlockView* from, Size byteIndex)
    {
        CHECK_INDEX_NO_INFO(byteIndex, 1, to->ByteCapacity, "byteIndex", "CopyBlockViewTo");
        return CopyTo(to, from->Data, from->ByteSize, byteIndex);
    }

    inline Bool CopyBlock(struct Block* to, const struct Block* from)
    {
        return CopyBlockTo(to, from, 0);
    }
    inline Bool CopyBlockView(struct Block* to, const struct BlockView* from)
    {
        return CopyBlockViewTo(to, from, 0);
    }
    
    inline Bool AppendBlock(struct Block* to, const struct Block* from)
    {
        return CopyBlockTo(to, from, to->ByteSize);
    }
    inline Bool AppendBlockView(struct Block* to, const struct BlockView* from)
    {
        return CopyBlockViewTo(to, from, to->ByteSize);
    }

#endif

#if defined (INCLUDE_BLOCKS_STRING)
    #define STRING_TERMINATOR '\0'
    inline void TerminateStringBlock(const Block* block)
    {
        CHECK_INDEX_NO_INFO(block->ByteSize, 0, block->ByteCapacity + sizeof (char), "block->ByteSize", "TerminateStringBlock");
        block->Data[block->ByteSize] = STRING_TERMINATOR;
    }
    
    inline void ReduceToStringBlock(struct Block* block)
    {
        CHECK_INDEX_NO_INFO(block->ByteSize, sizeof (char), block->ByteCapacity, "block->ByteSize", "ReduceToStringBlock");
        block->ByteCapacity --;
        block->Data[block->ByteCapacity] = STRING_TERMINATOR;
    }

    struct Block CreateStringBlock(char * data, Size size, Size capacity)
    {
        struct Block block = CreateBlock((void*)data, size, capacity - 1);
        CHECK_INDEX_NO_INFO(size, sizeof (char), capacity, "byteSize", "CreateStringBlock");

        block.Data[block.ByteCapacity] = STRING_TERMINATOR;
        block.Data[block.ByteSize] = STRING_TERMINATOR;

        return block;
    }

    inline struct BlockView CreateStringBlockView(char * data, Size size, Size capacity)
    {
        return CreateBlockView((void*)data, size, capacity);
    }
    
    inline struct BlockView ToStringBlockView(const char * str)
    {
        const Size len = (Size)strlen(str);
        return CreateBlockView((void*)str, len, len);
    }
    
    #if defined (INCLUDE_BLOCKS_ALLOC)
    struct Block AllocateStringBlock(Size byteCapacity)
    {
        struct Block block = AllocateBlock(byteCapacity + 1);
        ReduceToStringBlock(&block);
        return block;
    }
    void ReallocateStringBlock(struct Block* block, Size newCapacity)
    {
        ReallocateBlock(block, newCapacity + 1);
        ReduceToStringBlock(block);
    }
    Bool GrowStringBlockToFit(struct Block* block, Size toFitString)
    {
        const Bool state = GrowBlockToFit(block, toFitString + 1);
        if(state)
            ReduceToStringBlock(block);
        return state;
    }
    #endif

    #if defined (INCLUDE_BLOCKS_COPY)
    Bool CopyStringBlockTo(struct Block* to, const struct Block* from, Size intIndex)
    {
        Bool state = CopyBlockTo(to, from, intIndex*sizeof(int));
        TerminateStringBlock(to);
        return state;
    }
    Bool CopyStringBlockViewTo(struct Block* to, const struct BlockView* from, Size intIndex)
    {
        Bool state = CopyBlockViewTo(to, from, intIndex*sizeof(int));
        TerminateStringBlock(to);
        return state;
    }
    
    inline Bool CopyStringBlock(struct Block* to, const struct Block* from)
    {
        return CopyStringBlockTo(to, from, 0);
    }
    inline Bool CopyStringBlockView(struct Block* to, const struct BlockView* from)
    {
        return CopyStringBlockViewTo(to, from, 0);
    }
    
    inline Bool AppendStringBlock(struct Block* to, const struct Block* from)
    {
        return CopyStringBlockTo(to, from, to->ByteSize);
    }
    inline Bool AppendStringBlockView(struct Block* to, const struct BlockView* from)
    {
        return CopyStringBlockViewTo(to, from, to->ByteSize);
    }
    #endif

    inline Bool AppendToStringBlock(struct Block* to, char appended)
    {
        if(to->ByteSize >= to->ByteCapacity)
            return FALSE;

        At(to, to->ByteSize, char) = appended;
        SetSize(to, to->ByteSize + 1, char);
        TerminateStringBlock(to);
        return TRUE;
    }

#ifdef INCLUDE_BLOCKS_TYPE_SUPPORT
    Size CharI(Size i)          {return I(i, char);}
    Size CharIFrom(Size bytei)  {return IFrom(bytei, char);}
    Size ShortI(Size i)         {return I(i, short);}
    Size ShortIFrom(Size bytei) {return IFrom(bytei, short);}
    Size IntI(Size i)           {return I(i, int);}
    Size IntIFrom(Size bytei)   {return IFrom(bytei, int);}
    Size LongI(Size i)          {return I(i, long);}
    Size LongIFrom(Size bytei)  {return IFrom(bytei, long);}
    Size LLI(Size i)            {return I(i, long long);}
    Size LLIFrom(Size bytei)    {return IFrom(bytei, long long);}

    Size UCharI(Size i)         {return I(i, unsigned char);}
    Size UCharIFrom(Size bytei) {return IFrom(bytei, unsigned char);}
    Size UShortI(Size i)        {return I(i, unsigned short);}
    Size UShortIFrom(Size bytei){return IFrom(bytei, unsigned short);}
    Size UIntI(Size i)          {return I(i, unsigned int);}
    Size UIntIFrom(Size bytei)  {return IFrom(bytei, unsigned int);}
    Size ULongI(Size i)         {return I(i, unsigned long);}
    Size ULongIFrom(Size bytei) {return IFrom(bytei, unsigned long);}
    Size ULLI(Size i)           {return I(i, unsigned long long);}
    Size ULLIFrom(Size bytei)   {return IFrom(bytei, unsigned long long);}

    Size FloatI(Size i)         {return I(i, float);}
    Size FloatIFrom(Size bytei) {return IFrom(bytei, float);}
    Size DoubleI(Size i)        {return I(i, double);}
    Size DoubleIFrom(Size bytei){return IFrom(bytei, double);}
    Size LDoubleI(Size i)       {return I(i, long double);}
    Size LDoubleIFrom(Size bytei){return IFrom(bytei, long double);}
#endif
#endif

}


namespace Blocks::Testing
{

    TEST_CASE("Testing of everything", "[Everything]")
    {
        //Creating and allocating
        Block string = AllocateStringBlock(100);
        BlockView strLiteral = ToStringBlockView("A message");

        //Growing
        GrowStringBlockToFit(&string, strLiteral.ByteSize);
        CopyStringBlockView(&string, &strLiteral);

        //Reallocating
        ReallocateStringBlock(&string, 200);

        //Appending
        strLiteral = ToStringBlockView("A Another msg");

        if(AppendStringBlockView(&string, &strLiteral) == TRUE)
            printf("Whole string appended\n");

        //Char manipulation
        char firstChar = At(&string, 0, char);
        At(&string, 1, char) = firstChar;
        At(&string, 2, char) = firstChar;
        At(&string, 3, char) = firstChar;

        //Shrinking
        SetSize(&string, 4, char);
        TerminateStringBlock(&string);

        //Freeing
        FreeBlock(&string);
    }

    /*
    TEST_CASE("[Blocks::CreateEmptyBlock] : CreateEmptyBlock should create an empty block", "[Blocks][CreateEmptyBlock]")
    {
        Block block = CreateEmptyBlock();
        REQUIRE(block.ByteCapacity == 0);
        REQUIRE(block.ByteSize == 0);
        REQUIRE(block.Data == NULL);
    }

    TEST_CASE("[Blocks::CreateEmptyBlockView] : CreateEmptyBlockView should create an empty block", "[Blocks][CreateEmptyBlockView]")
    {
        BlockView block = CreateEmptyBlockView();
        REQUIRE(block.ByteCapacity == 0);
        REQUIRE(block.ByteSize == 0);
        REQUIRE(block.Data == NULL);
    }

    TEST_CASE("[Blocks::CreateCharBlock] : CreateCharBlock should create a block of the provided size", "[Blocks][CreateCharBlock]")
    {
        char c[10];
        Bool error;
        WHEN("Provided with valid size it should be set")
        {
            Block block = CreateCharBlock(c, 10, 10);
            block.Error = &error;

            REQUIRE(block.ByteCapacity == 10);
            REQUIRE(block.ByteSize == 10);
            REQUIRE(block.Data == c);
            REQUIRE(Error == FALSE);
        }
        WHEN("Provided with invalid size it should be set and scream error")
        {
            Block block = CreateCharBlock(c, 10, 9);
            block.Error = &error;

            REQUIRE(block.ByteCapacity == 9);
            REQUIRE(block.ByteSize == 10);
            REQUIRE(block.Data == c);
            REQUIRE(Error == TRUE);
        }
    }

    TEST_CASE("[Blocks::CreateCharBlockView] : CreateCharBlockView should create a block of the provided size", "[Blocks][CreateCharBlockView]")
    {
        char c[10];
        Bool error;
        WHEN("Provided with valid size it should be set")
        {
            BlockView block = CreateCharBlockView(c, 10, 10);
            block.Error = &error;

            REQUIRE(block.ByteCapacity == 10);
            REQUIRE(block.ByteSize == 10);
            REQUIRE(block.Data == c);
            REQUIRE(Error == FALSE);
        }
        WHEN("Provided with invalid size it should be set and scream error")
        {
            BlockView block = CreateCharBlockView(c, 10, 9);
            block.Error = &error;

            REQUIRE(block.ByteCapacity == 9);
            REQUIRE(block.ByteSize == 10);
            REQUIRE(block.Data == c);
            REQUIRE(Error == TRUE);
        }
    }

    TEST_CASE("[Blocks::SetCharSize] : SetCharSize should set the block size", "[Blocks][SetCharSize]")
    {
        Block block = CreateEmptyBlock();
        Bool error;
        block.Error = &error;

        WHEN("Provided with valid size it should be set")
        {
            block.ByteCapacity = 10;
            SetCharSize(&block, 10);
            REQUIRE(block.ByteSize == 10);
            REQUIRE(error == FALSE);
        }
        WHEN("Provided with invalid size it should be set and scream error")
        {
            SetCharSize(&block, 10);
            REQUIRE(block.ByteSize == 10);
            REQUIRE(error == TRUE);
        }
    }

    TEST_CASE("[Blocks::IsCharIndexValid] : IsCharIndexValid should return if the provided index is valid", "[Blocks][IsCharIndexValid]")
    {
        Block block = CreateCharBlock(NULL, 0, 10);

        WHEN("Provided with valid index it should return TRUE")
        {
            REQUIRE(IsCharIndexValid(&block, 5) == TRUE);
            REQUIRE(IsCharIndexValid(&block, 0) == TRUE);
            REQUIRE(IsCharIndexValid(&block, 9) == TRUE);
        }
        WHEN("Provided with invalid index it should return FALSE")
        {
            REQUIRE(IsCharIndexValid(&block, 10) == FALSE);
            REQUIRE(IsCharIndexValid(&block, 9852859) == FALSE);
            REQUIRE(IsCharIndexValid(&block, -1) == FALSE);
        }
    }

    TEST_CASE("[Blocks::IsCharIndexValidView] : IsCharIndexValidView should return if the provided index is valid", "[Blocks][IsCharIndexValidView]")
    {
        BlockView block = CreateCharBlockView(NULL, 0, 10);

        WHEN("Provided with valid index it should return TRUE")
        {
            REQUIRE(IsCharIndexValidView(&block, 5) == TRUE);
            REQUIRE(IsCharIndexValidView(&block, 0) == TRUE);
            REQUIRE(IsCharIndexValidView(&block, 9) == TRUE);
        }
        WHEN("Provided with invalid index it should return FALSE")
        {
            REQUIRE(IsCharIndexValidView(&block, 10) == FALSE);
            REQUIRE(IsCharIndexValidView(&block, 9852859) == FALSE);
            REQUIRE(IsCharIndexValidView(&block, -1) == FALSE);
        }
    }

    TEST_CASE("[Blocks::ToBlockView] : ToBlockView should return a copy of the provided block as const block", "[Blocks][ToBlockView]")
    {
        char c[10];
        Block block = CreateCharBlock(c, 7, 10);
        BlockView constBlock = ToBlockView(&block);

        REQUIRE(constBlock.ByteCapacity == 10);
        REQUIRE(constBlock.ByteSize == 7);
        REQUIRE(constBlock.Data == c);
    }

    TEST_CASE("[Blocks::CharAt] : CharAt should return a char pointer to the specified index", "[Blocks][CharAt]")
    {
        char c[1000] = {42, 52, 64};
        Block block = CreateCharBlock(c, 3, 3);
        Bool error;
        block.Error = &error;

        WHEN("Provided with valid index it should return teh pointer")
        {
            REQUIRE(CharAt(&block, 0) == c + 0);
            REQUIRE(error == FALSE);
            REQUIRE(CharAt(&block, 1) == c + 1);
            REQUIRE(error == FALSE);
            REQUIRE(CharAt(&block, 2) == c + 2);
            REQUIRE(error == FALSE);
        }
        WHEN("Provided with valid index it should return teh pointer and scream error")
        {
            REQUIRE(CharAt(&block, 3) == c + 3);
            REQUIRE(error == TRUE);
            REQUIRE(CharAt(&block, 999) == c + 999);
            REQUIRE(error == TRUE);
        }
    }

    TEST_CASE("[Blocks::CharAtView] : CharAtView should return a char pointer to the specified index", "[Blocks][CharAtView]")
    {
        char c[1000] = {42, 52, 64};
        BlockView block = CreateCharBlockView(c, 3, 3);
        Bool error;
        block.Error = &error;

        WHEN("Provided with valid index it should return teh pointer")
        {
            REQUIRE(CharAtView(&block, 0) == c + 0);
            REQUIRE(error == FALSE);
            REQUIRE(CharAtView(&block, 1) == c + 1);
            REQUIRE(error == FALSE);
            REQUIRE(CharAtView(&block, 2) == c + 2);
            REQUIRE(error == FALSE);
        }
        WHEN("Provided with valid index it should return teh pointer and scream error")
        {
            REQUIRE(CharAtView(&block, 3) == c + 3);
            REQUIRE(error == TRUE);
            REQUIRE(CharAtView(&block, 999) == c + 999);
            REQUIRE(error == TRUE);
        }
    }

    TEST_CASE("[Blocks::AllocateCharBlock] : AllocateCharBlock should allocate a new blcok and return it", "[Blocks][AllocateCharBlock]")
    {
        {
            Block block = AllocateCharBlock(10);
            REQUIRE(block.ByteCapacity == 10);
            REQUIRE(block.ByteSize == 0);
            REQUIRE(block.Data != NULL);
            FreeBlock(&block);
        }
        {
            Block block = AllocateCharBlock(0);
            REQUIRE(block.ByteCapacity == 0);
            REQUIRE(block.ByteSize == 0);
            REQUIRE(block.Data != NULL);
            FreeBlock(&block);
        }
        {
            Block block = AllocateCharBlock(43);
            REQUIRE(block.ByteCapacity == 43);
            REQUIRE(block.ByteSize == 0);
            REQUIRE(block.Data != NULL);
            FreeBlock(&block);
        }
    }

    TEST_CASE("[Blocks::ReallocateCharBlock] : ReallocateCharBlock should reallocate block into the provided size and opy the data", "[Blocks][ReallocateCharBlock]")
    {
        Block block = AllocateCharBlock(10);
        SetCharSize(&block, 10);
        *CharAt(&block, 0) = 'a';
        *CharAt(&block, 4) = 'b';
        *CharAt(&block, 7) = 'c';

        WHEN("Reallocating to larger size the capacity should change and the data should be copied")
        {
            ReallocateCharBlock(&block, 12);
            REQUIRE(block.ByteCapacity == 12);
            REQUIRE(*CharAt(&block, 0) == 'a');
            REQUIRE(*CharAt(&block, 4) == 'b');
            REQUIRE(*CharAt(&block, 7) == 'c');
        }
        WHEN("Reallocating to smaller size the capacity should change and the data should be copied")
        {
            ReallocateCharBlock(&block, 5);
            REQUIRE(block.ByteCapacity == 5);
            REQUIRE(*CharAt(&block, 0) == 'a');
            REQUIRE(*CharAt(&block, 4) == 'b');
        }

        FreeBlock(&block);
    }

    TEST_CASE("[Blocks::CalculateGrowthFactor] : CalculateGrowthFactor shoudl return the number of times the current size needs to be multiplied in powers of two", "[Blocks][CalculateGrowthFactor]")
    {
        REQUIRE(CalculateGrowthFactor(10, 14) == 2);
        REQUIRE(CalculateGrowthFactor(10, 24) == 4);
        REQUIRE(CalculateGrowthFactor(10, 25) == 4);
        REQUIRE(CalculateGrowthFactor(10, 800) == 128);
        REQUIRE(CalculateGrowthFactor(7, 14) == 2);
        REQUIRE(CalculateGrowthFactor(76, 9187) == 128);
        REQUIRE(CalculateGrowthFactor(9815, 9815) == 1);
        REQUIRE(CalculateGrowthFactor(9815, 2918501) == 512);
    }

    TEST_CASE("[Blocks::GrowCharBlockToFit] : GrowCharBlockToFit shoudl reallocate the block by a power of two to fit at leats the number of data specified", "[Blocks][GrowCharBlockToFit]")
    {
        Block block = AllocateCharBlock(10);
        SetCharSize(&block, 10);
        *CharAt(&block, 0) = 'a';
        *CharAt(&block, 4) = 'b';
        *CharAt(&block, 7) = 'c';


        WHEN("Growing to larger size the capacity should change by a power of two, the data should be copied, and the function should return true")
        {
            SECTION("To fit 16")
            {
                REQUIRE(GrowCharBlockToFit(&block, 16) == TRUE);
                REQUIRE(block.ByteCapacity == 20);
                REQUIRE(*CharAt(&block, 0) == 'a');
                REQUIRE(*CharAt(&block, 4) == 'b');
                REQUIRE(*CharAt(&block, 7) == 'c');
            }
            SECTION("To fit 24")
            {
                REQUIRE(GrowCharBlockToFit(&block, 24) == TRUE);
                REQUIRE(block.ByteCapacity == 40);
                REQUIRE(*CharAt(&block, 0) == 'a');
                REQUIRE(*CharAt(&block, 4) == 'b');
                REQUIRE(*CharAt(&block, 7) == 'c');
            }
            SECTION("To fit 25")
            {
                REQUIRE(GrowCharBlockToFit(&block, 25) == TRUE);
                REQUIRE(block.ByteCapacity == 40);
                REQUIRE(*CharAt(&block, 0) == 'a');
                REQUIRE(*CharAt(&block, 4) == 'b');
                REQUIRE(*CharAt(&block, 7) == 'c');
            }
        }

        WHEN("Growing to smaller size the nothing should happedn and the function should return false")
        {
            SECTION("To fit 6")
            {
                REQUIRE(GrowCharBlockToFit(&block, 6) == FALSE);
                REQUIRE(block.ByteCapacity == 10);
                REQUIRE(*CharAt(&block, 0) == 'a');
                REQUIRE(*CharAt(&block, 4) == 'b');
                REQUIRE(*CharAt(&block, 7) == 'c');
            }
            SECTION("To fit 10")
            {
                REQUIRE(GrowCharBlockToFit(&block, 10) == FALSE);
                REQUIRE(block.ByteCapacity == 10);
                REQUIRE(*CharAt(&block, 0) == 'a');
                REQUIRE(*CharAt(&block, 4) == 'b');
                REQUIRE(*CharAt(&block, 7) == 'c');
            }
        }

        WHEN("Growing from a allocated size of 0 size the resulting size should be exactly the one specified")
        {
            Block block2 = AllocateCharBlock(0);
            REQUIRE(GrowCharBlockToFit(&block2, 25) == TRUE);
            REQUIRE(block2.ByteCapacity == 25);
            FreeBlock(&block2);
        }

        FreeBlock(&block);
    }

    TEST_CASE("[Blocks::FreeBlock] : FreeBlock should free a block", "[Blocks][FreeBlock]")
    {
        Block block = AllocateCharBlock(10);
        REQUIRE(block.ByteCapacity == 10);
        REQUIRE(block.ByteSize == 0);
        REQUIRE(block.Data != NULL);
        FreeBlock(&block);
    }

    TEST_CASE("[Blocks::CopyCharBlockTo] : CopyCharBlockTo should copy all charcters that can fit to the block from the specified bloack at the specified location", "[Blocks][CopyCharBlockTo]")
    {
        char buffer1[200] = {1, 2, 3, 4, 5, 6};
        char buffer2[50] = {11, 12, 13, 14, 15, 16};
        buffer1[99] = 100;
        buffer1[98] = 99;
        buffer1[97] = 98;
        buffer1[96] = 97;

        Block to = CreateCharBlock(buffer1, 6, 100);
        Block from = CreateCharBlock(buffer2, 6, 50);

        WHEN("Copying to a valid position the data shoudl be copied")
        {
            WHEN("Copying to a location that enables to save all data all data should be saved and the function should return true")
            {
                SECTION("To index 0")
                {
                    REQUIRE(CopyCharBlockTo(&to, &from, 0) == TRUE);
                    REQUIRE(*CharAt(&to, 0) == 11);
                    REQUIRE(*CharAt(&to, 1) == 12);
                    REQUIRE(*CharAt(&to, 5) == 16);
                    REQUIRE(to.ByteSize == 6);
                }
                SECTION("To index 3")
                {
                    REQUIRE(CopyCharBlockTo(&to, &from, 3) == TRUE);
                    REQUIRE(*CharAt(&to, 0) == 1);
                    REQUIRE(*CharAt(&to, 1) == 2);
                    REQUIRE(*CharAt(&to, 2) == 3);
                    REQUIRE(*CharAt(&to, 3) == 11);
                    REQUIRE(*CharAt(&to, 5) == 13);
                    REQUIRE(to.ByteSize == 9);
                }
            }
            WHEN("Copying to a location that does not enables to save all data only data up until capacity saved and the function should return false")
            {
                SECTION("To index 99")
                {
                    REQUIRE(CopyCharBlockTo(&to, &from, 99) == FALSE);
                    REQUIRE(*CharAt(&to, 97) == 98);
                    REQUIRE(*CharAt(&to, 98) == 99);
                    REQUIRE(*CharAt(&to, 99) == 11);
                    REQUIRE(to.ByteSize == 100);
                }
                SECTION("To index 97")
                {
                    REQUIRE(CopyCharBlockTo(&to, &from, 97) == FALSE);
                    REQUIRE(*CharAt(&to, 96) == 97);
                    REQUIRE(*CharAt(&to, 97) == 11);
                    REQUIRE(*CharAt(&to, 98) == 12);
                    REQUIRE(*CharAt(&to, 99) == 13);
                    REQUIRE(to.ByteSize == 100);
                }
            }
        }
        WHEN("Copying to a invalid position the data shoudl not be copied and the function shoudl return false")
        {
            REQUIRE(CopyCharBlockTo(&to, &from, 150) == FALSE);
            REQUIRE(CopyCharBlockTo(&to, &from, 100) == FALSE);
            REQUIRE(CopyCharBlockTo(&to, &from, 1090591) == FALSE);
        }
    }

    TEST_CASE("[Blocks::CopyCharBlockViewTo] : CopyCharBlockViewTo should copy all charcters that can fit to the block from the specified bloack at the specified location", "[Blocks][CopyCharBlockViewTo]")
    {
        char buffer1[200] = {1, 2, 3, 4, 5, 6};
        char buffer2[50] = {11, 12, 13, 14, 15, 16};
        buffer1[99] = 100;
        buffer1[98] = 99;
        buffer1[97] = 98;
        buffer1[96] = 97;

        Block to = CreateCharBlock(buffer1, 6, 100);
        BlockView from = CreateCharBlockView(buffer2, 6, 50);

        WHEN("Copying to a valid position the data shoudl be copied")
        {
            WHEN("Copying to a location that enables to save all data all data should be saved and the function should return true")
            {
                SECTION("To index 0")
                {
                    REQUIRE(CopyCharBlockViewTo(&to, &from, 0) == TRUE);
                    REQUIRE(*CharAt(&to, 0) == 11);
                    REQUIRE(*CharAt(&to, 1) == 12);
                    REQUIRE(*CharAt(&to, 5) == 16);
                    REQUIRE(to.ByteSize == 6);
                }
                SECTION("To index 3")
                {
                    REQUIRE(CopyCharBlockViewTo(&to, &from, 3) == TRUE);
                    REQUIRE(*CharAt(&to, 0) == 1);
                    REQUIRE(*CharAt(&to, 1) == 2);
                    REQUIRE(*CharAt(&to, 2) == 3);
                    REQUIRE(*CharAt(&to, 3) == 11);
                    REQUIRE(*CharAt(&to, 5) == 13);
                    REQUIRE(to.ByteSize == 9);
                }
            }
            WHEN("Copying to a location that does not enables to save all data only data up until capacity saved and the function should return false")
            {
                SECTION("To index 99")
                {
                    REQUIRE(CopyCharBlockViewTo(&to, &from, 99) == FALSE);
                    REQUIRE(*CharAt(&to, 97) == 98);
                    REQUIRE(*CharAt(&to, 98) == 99);
                    REQUIRE(*CharAt(&to, 99) == 11);
                    REQUIRE(to.ByteSize == 100);
                }
                SECTION("To index 97")
                {
                    REQUIRE(CopyCharBlockViewTo(&to, &from, 97) == FALSE);
                    REQUIRE(*CharAt(&to, 96) == 97);
                    REQUIRE(*CharAt(&to, 97) == 11);
                    REQUIRE(*CharAt(&to, 98) == 12);
                    REQUIRE(*CharAt(&to, 99) == 13);
                    REQUIRE(to.ByteSize == 100);
                }
            }
        }
        WHEN("Copying to a invalid position the data shoudl not be copied and the function shoudl return false")
        {
            REQUIRE(CopyCharBlockViewTo(&to, &from, 150) == FALSE);
            REQUIRE(CopyCharBlockViewTo(&to, &from, 100) == FALSE);
            REQUIRE(CopyCharBlockViewTo(&to, &from, 1090591) == FALSE);
        }
    }

    TEST_CASE("[Blocks::CopyBlock] : CopyBlock should copy all data from the specified block to the specified block and return if all data was copied", "[Blocks][CopyBlock]")
    {
        char buffer1[200] = {1, 2, 3, 4, 5, 6};
        char buffer2[200] = {11, 12, 13, 14, 15, 16};
        buffer1[99] = 100;
        buffer1[98] = 99;
        buffer1[97] = 98;
        buffer1[96] = 97;
        Block to = CreateCharBlock(buffer1, 6, 100);


        WHEN("Copying from a block of lesser size than the to block capacity the function should return true and copy all data")
        {
            SECTION("ByteSize 0")
            {
                Block from = CreateCharBlock(buffer2, 0, 100);
                REQUIRE(CopyBlock(&to, &from) == TRUE);
                REQUIRE(*CharAt(&to, 0) == 1);
                REQUIRE(*CharAt(&to, 1) == 2);
                REQUIRE(*CharAt(&to, 5) == 6);
                REQUIRE(to.ByteSize == 6);
            }
            SECTION("ByteSize 50")
            {
                Block from = CreateCharBlock(buffer2, 50, 100);
                REQUIRE(CopyBlock(&to, &from) == TRUE);
                REQUIRE(*CharAt(&to, 0) == 11);
                REQUIRE(*CharAt(&to, 1) == 12);
                REQUIRE(*CharAt(&to, 5) == 16);
                REQUIRE(to.ByteSize == 50);
            }
            SECTION("ByteSize 74")
            {
                Block from = CreateCharBlock(buffer2, 74, 100);
                REQUIRE(CopyBlock(&to, &from) == TRUE);
                REQUIRE(*CharAt(&to, 0) == 11);
                REQUIRE(*CharAt(&to, 1) == 12);
                REQUIRE(*CharAt(&to, 5) == 16);
                REQUIRE(to.ByteSize == 74);
            }
            SECTION("ByteSize 100")
            {
                Block from = CreateCharBlock(buffer2, 100, 100);
                REQUIRE(CopyBlock(&to, &from) == TRUE);
                REQUIRE(*CharAt(&to, 0) == 11);
                REQUIRE(*CharAt(&to, 1) == 12);
                REQUIRE(*CharAt(&to, 5) == 16);
                REQUIRE(to.ByteSize == 100);
            }
        }
        WHEN("Copying from a block of greater size than the to block capacity the function should return false and copy all data taht fits")
        {
            SECTION("ByteSize 101")
            {
                Block from = CreateCharBlock(buffer2, 101, 100);
                REQUIRE(CopyBlock(&to, &from) == FALSE);
                REQUIRE(*CharAt(&to, 0) == 11);
                REQUIRE(*CharAt(&to, 1) == 12);
                REQUIRE(*CharAt(&to, 5) == 16);
                REQUIRE(to.ByteSize == 100);
            }
            SECTION("ByteSize 158")
            {
                Block from = CreateCharBlock(buffer2, 158, 100);
                REQUIRE(CopyBlock(&to, &from) == FALSE);
                REQUIRE(*CharAt(&to, 0) == 11);
                REQUIRE(*CharAt(&to, 1) == 12);
                REQUIRE(*CharAt(&to, 5) == 16);
                REQUIRE(to.ByteSize == 100);
            }
            SECTION("ByteSize 200")
            {
                Block from = CreateCharBlock(buffer2, 200, 100);
                REQUIRE(CopyBlock(&to, &from) == FALSE);
                REQUIRE(*CharAt(&to, 0) == 11);
                REQUIRE(*CharAt(&to, 1) == 12);
                REQUIRE(*CharAt(&to, 5) == 16);
                REQUIRE(to.ByteSize == 100);
            }
        }
    }

    TEST_CASE("[Blocks::CopyBlockView] : CopyBlockView should copy all data from the specified block to the specified block and return if all data was copied", "[Blocks][CopyBlockView]")
    {
        char buffer1[200] = {1, 2, 3, 4, 5, 6};
        char buffer2[200] = {11, 12, 13, 14, 15, 16};
        buffer1[99] = 100;
        buffer1[98] = 99;
        buffer1[97] = 98;
        buffer1[96] = 97;
        Block to = CreateCharBlock(buffer1, 6, 100);


        WHEN("Copying from a block of lesser size than the to block capacity the function should return true and copy all data")
        {
            SECTION("ByteSize 0")
            {
                BlockView from = CreateCharBlockView(buffer2, 0, 100);
                REQUIRE(CopyBlockView(&to, &from) == TRUE);
                REQUIRE(*CharAt(&to, 0) == 1);
                REQUIRE(*CharAt(&to, 1) == 2);
                REQUIRE(*CharAt(&to, 5) == 6);
                REQUIRE(to.ByteSize == 6);
            }
            SECTION("ByteSize 50")
            {
                BlockView from = CreateCharBlockView(buffer2, 50, 100);
                REQUIRE(CopyBlockView(&to, &from) == TRUE);
                REQUIRE(*CharAt(&to, 0) == 11);
                REQUIRE(*CharAt(&to, 1) == 12);
                REQUIRE(*CharAt(&to, 5) == 16);
                REQUIRE(to.ByteSize == 50);
            }
            SECTION("ByteSize 74")
            {
                BlockView from = CreateCharBlockView(buffer2, 74, 100);
                REQUIRE(CopyBlockView(&to, &from) == TRUE);
                REQUIRE(*CharAt(&to, 0) == 11);
                REQUIRE(*CharAt(&to, 1) == 12);
                REQUIRE(*CharAt(&to, 5) == 16);
                REQUIRE(to.ByteSize == 74);
            }
            SECTION("ByteSize 100")
            {
                BlockView from = CreateCharBlockView(buffer2, 100, 100);
                REQUIRE(CopyBlockView(&to, &from) == TRUE);
                REQUIRE(*CharAt(&to, 0) == 11);
                REQUIRE(*CharAt(&to, 1) == 12);
                REQUIRE(*CharAt(&to, 5) == 16);
                REQUIRE(to.ByteSize == 100);
            }
        }
        WHEN("Copying from a block of greater size than the to block capacity the function should return false and copy all data taht fits")
        {
            SECTION("ByteSize 101")
            {
                BlockView from = CreateCharBlockView(buffer2, 101, 100);
                REQUIRE(CopyBlockView(&to, &from) == FALSE);
                REQUIRE(*CharAt(&to, 0) == 11);
                REQUIRE(*CharAt(&to, 1) == 12);
                REQUIRE(*CharAt(&to, 5) == 16);
                REQUIRE(to.ByteSize == 100);
            }
            SECTION("ByteSize 158")
            {
                BlockView from = CreateCharBlockView(buffer2, 158, 100);
                REQUIRE(CopyBlockView(&to, &from) == FALSE);
                REQUIRE(*CharAt(&to, 0) == 11);
                REQUIRE(*CharAt(&to, 1) == 12);
                REQUIRE(*CharAt(&to, 5) == 16);
                REQUIRE(to.ByteSize == 100);
            }
            SECTION("ByteSize 200")
            {
                BlockView from = CreateCharBlockView(buffer2, 200, 100);
                REQUIRE(CopyBlockView(&to, &from) == FALSE);
                REQUIRE(*CharAt(&to, 0) == 11);
                REQUIRE(*CharAt(&to, 1) == 12);
                REQUIRE(*CharAt(&to, 5) == 16);
                REQUIRE(to.ByteSize == 100);
            }
        }
    }

    TEST_CASE("[Blocks::AppendBlock] : AppendBlock should copy the block data to the end of the block specified and return if all data was copied", "[Blocks][AppendBlock]")
    {
        char buffer1[200] = {1, 2, 3, 4, 5, 6};
        char buffer2[200] = {11, 12, 13, 14, 15, 16};

        WHEN("Appending to a position that is big enough for all the data it should copy all data and return true")
        {
            SECTION("ByteSize 0")
            {
                Block to = CreateCharBlock(buffer1, 23, 100);
                Block from = CreateCharBlock(buffer2, 0, 100);
                REQUIRE(AppendBlock(&to, &from) == TRUE);
                REQUIRE(*CharAt(&to, 0) == 1);
                REQUIRE(*CharAt(&to, 1) == 2);
                REQUIRE(*CharAt(&to, 5) == 6);
                REQUIRE(to.ByteSize == 23);
            }
            SECTION("ByteSize 50")
            {
                Block to = CreateCharBlock(buffer1, 50, 100);
                Block from = CreateCharBlock(buffer2, 50, 100);
                REQUIRE(AppendBlock(&to, &from) == TRUE);
                CHECK(*CharAt(&to, 0) == 1);
                CHECK(*CharAt(&to, 1) == 2);
                CHECK(*CharAt(&to, 5) == 6);
                CHECK(*CharAt(&to, 50) == 11);
                CHECK(*CharAt(&to, 51) == 12);
                CHECK(*CharAt(&to, 55) == 16);
                REQUIRE(to.ByteSize == 100);
            }
            SECTION("ByteSize 74")
            {
                Block to = CreateCharBlock(buffer1, 0, 100);
                Block from = CreateCharBlock(buffer2, 74, 100);
                REQUIRE(AppendBlock(&to, &from) == TRUE);
                REQUIRE(*CharAt(&to, 0) == 11);
                REQUIRE(*CharAt(&to, 1) == 12);
                REQUIRE(*CharAt(&to, 5) == 16);
                REQUIRE(to.ByteSize == 74);
            }
            SECTION("ByteSize 100")
            {
                Block to = CreateCharBlock(buffer1, 37, 150);
                Block from = CreateCharBlock(buffer2, 100, 100);
                REQUIRE(AppendBlock(&to, &from) == TRUE);
                CHECK(*CharAt(&to, 0) == 1);
                CHECK(*CharAt(&to, 1) == 2);
                CHECK(*CharAt(&to, 5) == 6);
                CHECK(*CharAt(&to, 37) == 11);
                CHECK(*CharAt(&to, 38) == 12);
                CHECK(*CharAt(&to, 39) == 13);
                REQUIRE(to.ByteSize == 137);
            }
        }
        WHEN("Appending to a position that is not big enough for all the data it should copy all data that fits and return false")
        {
            SECTION("ByteSize 1")
            {
                Block to = CreateCharBlock(buffer1, 100, 100);
                Block from = CreateCharBlock(buffer2, 1, 100);
                REQUIRE(AppendBlock(&to, &from) == FALSE);
                REQUIRE(*CharAt(&to, 0) == 1);
                REQUIRE(*CharAt(&to, 1) == 2);
                REQUIRE(*CharAt(&to, 5) == 6);
                REQUIRE(to.ByteSize == 100);
            }
            SECTION("ByteSize 50")
            {
                Block to = CreateCharBlock(buffer1, 51, 100);
                Block from = CreateCharBlock(buffer2, 50, 100);
                REQUIRE(AppendBlock(&to, &from) == FALSE);
                REQUIRE(*CharAt(&to, 0) == 1);
                REQUIRE(*CharAt(&to, 1) == 2);
                REQUIRE(*CharAt(&to, 5) == 6);
                REQUIRE(*CharAt(&to, 51) == 11);
                REQUIRE(*CharAt(&to, 52) == 12);
                REQUIRE(*CharAt(&to, 56) == 16);
                REQUIRE(to.ByteSize == 100);
            }
            SECTION("ByteSize 101")
            {
                Block to = CreateCharBlock(buffer1, 0, 100);
                Block from = CreateCharBlock(buffer2, 101, 100);
                REQUIRE(AppendBlock(&to, &from) == FALSE);
                REQUIRE(*CharAt(&to, 0) == 11);
                REQUIRE(*CharAt(&to, 1) == 12);
                REQUIRE(*CharAt(&to, 5) == 16);
                REQUIRE(to.ByteSize == 100);
            }
            SECTION("ByteSize 100")
            {
                Block to = CreateCharBlock(buffer1, 37, 150);
                Block from = CreateCharBlock(buffer2, 167, 200);
                REQUIRE(AppendBlock(&to, &from) == FALSE);
                REQUIRE(*CharAt(&to, 0) == 1);
                REQUIRE(*CharAt(&to, 1) == 2);
                REQUIRE(*CharAt(&to, 5) == 6);
                CHECK(*CharAt(&to, 37) == 11);
                CHECK(*CharAt(&to, 38) == 12);
                CHECK(*CharAt(&to, 39) == 13);
                REQUIRE(to.ByteSize == 150);
            }
        }
    }

    TEST_CASE("[Blocks::AppendBlockView] : AppendBlockView should copy the block data to the end of the block specified and return if all data was copied", "[Blocks][AppendBlockView]")
    {
        char buffer1[200] = {1, 2, 3, 4, 5, 6};
        char buffer2[200] = {11, 12, 13, 14, 15, 16};

        WHEN("Appending to a position that is big enough for all the data it should copy all data and return true")
        {
            SECTION("ByteSize 0")
            {
                Block to = CreateCharBlock(buffer1, 23, 100);
                BlockView from = CreateCharBlockView(buffer2, 0, 100);
                REQUIRE(AppendBlockView(&to, &from) == TRUE);
                REQUIRE(*CharAt(&to, 0) == 1);
                REQUIRE(*CharAt(&to, 1) == 2);
                REQUIRE(*CharAt(&to, 5) == 6);
                REQUIRE(to.ByteSize == 23);
            }
            SECTION("ByteSize 50")
            {
                Block to = CreateCharBlock(buffer1, 50, 100);
                BlockView from = CreateCharBlockView(buffer2, 50, 100);
                REQUIRE(AppendBlockView(&to, &from) == TRUE);
                CHECK(*CharAt(&to, 0) == 1);
                CHECK(*CharAt(&to, 1) == 2);
                CHECK(*CharAt(&to, 5) == 6);
                CHECK(*CharAt(&to, 50) == 11);
                CHECK(*CharAt(&to, 51) == 12);
                CHECK(*CharAt(&to, 55) == 16);
                REQUIRE(to.ByteSize == 100);
            }
            SECTION("ByteSize 74")
            {
                Block to = CreateCharBlock(buffer1, 0, 100);
                BlockView from = CreateCharBlockView(buffer2, 74, 100);
                REQUIRE(AppendBlockView(&to, &from) == TRUE);
                REQUIRE(*CharAt(&to, 0) == 11);
                REQUIRE(*CharAt(&to, 1) == 12);
                REQUIRE(*CharAt(&to, 5) == 16);
                REQUIRE(to.ByteSize == 74);
            }
            SECTION("ByteSize 100")
            {
                Block to = CreateCharBlock(buffer1, 37, 150);
                BlockView from = CreateCharBlockView(buffer2, 100, 100);
                REQUIRE(AppendBlockView(&to, &from) == TRUE);
                CHECK(*CharAt(&to, 0) == 1);
                CHECK(*CharAt(&to, 1) == 2);
                CHECK(*CharAt(&to, 5) == 6);
                CHECK(*CharAt(&to, 37) == 11);
                CHECK(*CharAt(&to, 38) == 12);
                CHECK(*CharAt(&to, 39) == 13);
                REQUIRE(to.ByteSize == 137);
            }
        }
        WHEN("Appending to a position that is not big enough for all the data it should copy all data that fits and return false")
        {
            SECTION("ByteSize 1")
            {
                Block to = CreateCharBlock(buffer1, 100, 100);
                BlockView from = CreateCharBlockView(buffer2, 1, 100);
                REQUIRE(AppendBlockView(&to, &from) == FALSE);
                REQUIRE(*CharAt(&to, 0) == 1);
                REQUIRE(*CharAt(&to, 1) == 2);
                REQUIRE(*CharAt(&to, 5) == 6);
                REQUIRE(to.ByteSize == 100);
            }
            SECTION("ByteSize 50")
            {
                Block to = CreateCharBlock(buffer1, 51, 100);
                BlockView from = CreateCharBlockView(buffer2, 50, 100);
                REQUIRE(AppendBlockView(&to, &from) == FALSE);
                REQUIRE(*CharAt(&to, 0) == 1);
                REQUIRE(*CharAt(&to, 1) == 2);
                REQUIRE(*CharAt(&to, 5) == 6);
                REQUIRE(*CharAt(&to, 51) == 11);
                REQUIRE(*CharAt(&to, 52) == 12);
                REQUIRE(*CharAt(&to, 56) == 16);
                REQUIRE(to.ByteSize == 100);
            }
            SECTION("ByteSize 101")
            {
                Block to = CreateCharBlock(buffer1, 0, 100);
                BlockView from = CreateCharBlockView(buffer2, 101, 100);
                REQUIRE(AppendBlockView(&to, &from) == FALSE);
                REQUIRE(*CharAt(&to, 0) == 11);
                REQUIRE(*CharAt(&to, 1) == 12);
                REQUIRE(*CharAt(&to, 5) == 16);
                REQUIRE(to.ByteSize == 100);
            }
            SECTION("ByteSize 100")
            {
                Block to = CreateCharBlock(buffer1, 37, 150);
                BlockView from = CreateCharBlockView(buffer2, 167, 200);
                REQUIRE(AppendBlockView(&to, &from) == FALSE);
                REQUIRE(*CharAt(&to, 0) == 1);
                REQUIRE(*CharAt(&to, 1) == 2);
                REQUIRE(*CharAt(&to, 5) == 6);
                CHECK(*CharAt(&to, 37) == 11);
                CHECK(*CharAt(&to, 38) == 12);
                CHECK(*CharAt(&to, 39) == 13);
                REQUIRE(to.ByteSize == 150);
            }
        }
    }


    TEST_CASE("[Blocks::CreateStringBlock] : CreateStringBlock should create a string block", "[Blocks][CreateStringBlock]")
    {
        char data[100];

        WHEN("Provided with a size at least one smaller than capacity a string block with capacity of the provided capacty-1 should be created")
        {
            {
                const Block stringBlock = CreateStringBlock(NULL, 0, 1);
                REQUIRE(stringBlock.Data == NULL);
                REQUIRE(stringBlock.ByteSize == 0);
                REQUIRE(stringBlock.ByteCapacity == 0);
                REQUIRE(Error == FALSE);
            }
            {
                const Block stringBlock = CreateStringBlock(data, 99, 100);
                REQUIRE(stringBlock.Data == data);
                REQUIRE(stringBlock.ByteSize == 99);
                REQUIRE(stringBlock.ByteCapacity == 99);
                REQUIRE(Error == FALSE);
            }
            {
                const Block stringBlock = CreateStringBlock(data + 4, 65, 73);
                REQUIRE(stringBlock.Data == data + 4);
                REQUIRE(stringBlock.ByteSize == 65);
                REQUIRE(stringBlock.ByteCapacity == 72);
                REQUIRE(Error == FALSE);
            }

        }
        WHEN("Provided with invalid size and capcity the string should be created and error messege should be broadcasted")
        {
            {
                const Block stringBlock = CreateStringBlock(data, 100, 100);
                REQUIRE(stringBlock.Data == data);
                REQUIRE(stringBlock.ByteSize == 50);
                REQUIRE(stringBlock.ByteCapacity == 99);
                REQUIRE(Error == TRUE);
            }
            {
                const Block stringBlock = CreateStringBlock(NULL, 0, 0);
                REQUIRE(stringBlock.Data == NULL);
                REQUIRE(stringBlock.ByteSize == 0);
                REQUIRE(stringBlock.ByteCapacity == (Size)-1);
                REQUIRE(Error == TRUE);
            }
            {
                const Block stringBlock = CreateStringBlock(data, 64, 28);
                REQUIRE(stringBlock.Data == data);
                REQUIRE(stringBlock.ByteSize == 99);
                REQUIRE(stringBlock.ByteCapacity == 99);
                REQUIRE(Error == TRUE);
            }
        }
    }

    TEST_CASE("[Blocks::CreateStringBlockView] : CreateStringBlockView should create a string block from the provided c string", "[Blocks][CreateStringBlockView]")
    {
        {
            const char data[100] = "Hello";
            const BlockView stringBlock = CreateStringBlockView(data);

            REQUIRE(stringBlock.Data == data);
            REQUIRE(stringBlock.ByteSize == strlen(data));
            REQUIRE(stringBlock.ByteCapacity == strlen(data));
        }
        {
            const BlockView stringBlock = CreateStringBlockView("msg");

            REQUIRE(stringBlock.Data != NULL);
            REQUIRE(stringBlock.ByteSize == 3);
            REQUIRE(stringBlock.ByteCapacity == 3);
        }
        {
            const BlockView stringBlock = CreateStringBlockView("");

            REQUIRE(stringBlock.Data != NULL);
            REQUIRE(stringBlock.ByteSize == 0);
            REQUIRE(stringBlock.ByteCapacity == 0);
        }
    }

    TEST_CASE("[Blocks::TerminateStringBlock] : TerminateStringBlock should put a string terminator past the last used index", "[Blocks][TerminateStringBlock]")
    {
        char data[10] = {'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a'};
        Bool error = false;

        WHEN("Provided with a a block of size at least one smaller than capacity the function should set the termination charcter")
        {
            SECTION("size = 9")
            {
                Block block = CreateCharBlock(data, 9, 10);
                block.Error = &error;
                TerminateStringBlock(&block);
                REQUIRE(data[9] == STRING_TERMINATOR);
                REQUIRE(error == FALSE);
            }
            SECTION("size = 0")
            {
                Block block = CreateCharBlock(data, 0, 10);
                block.Error = &error;
                TerminateStringBlock(&block);
                REQUIRE(data[0] == STRING_TERMINATOR);
                REQUIRE(error == FALSE);
            }
            SECTION("size = 4")
            {
                Block block = CreateCharBlock(data, 4, 10);
                block.Error = &error;
                TerminateStringBlock(&block);
                REQUIRE(data[4] == STRING_TERMINATOR);
                REQUIRE(error == FALSE);
            }
        }
        WHEN("Provided with a block of invalid size the function should set the termination charcter and report error")
        {
            SECTION("size = 8")
            {
                Block block = CreateCharBlock(data, 8, 8);
                block.Error = &error;
                TerminateStringBlock(&block);
                REQUIRE(data[8] == STRING_TERMINATOR);
                REQUIRE(error == TRUE);
            }
            SECTION("size = 0")
            {
                Block block = CreateCharBlock(data, 0, 0);
                block.Error = &error;
                TerminateStringBlock(&block);
                REQUIRE(data[0] == STRING_TERMINATOR);
                REQUIRE(error == TRUE);
            }
            SECTION("size = 9")
            {
                Block block = CreateCharBlock(data, 9, 6);
                block.Error = &error;
                TerminateStringBlock(&block);
                REQUIRE(data[9] == STRING_TERMINATOR);
                REQUIRE(error == TRUE);
            }
        }
    }

    TEST_CASE("[Blocks::ReduceToStringBlock] : ReduceToStringBlock should reserve one capacity and set the null termination charcter at that size", "[Blocks][ReduceToStringBlock]")
    {
        char data[10] = {'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a'};
        Bool error = false;

        WHEN("Provided with a a block of size at least one smaller than capacity the function should set the termination charcter")
        {
            SECTION("size = 9")
            {
                Block block = CreateCharBlock(data, 9, 10);
                block.Error = &error;
                ReduceToStringBlock(&block);
                TerminateStringBlock(&block);
                REQUIRE(data[9] == STRING_TERMINATOR);
                REQUIRE(error == FALSE);
            }
            SECTION("size = 0")
            {
                Block block = CreateCharBlock(data, 0, 10);
                block.Error = &error;
                TerminateStringBlock(&block);
                REQUIRE(data[0] == STRING_TERMINATOR);
                REQUIRE(error == FALSE);
            }
            SECTION("size = 4")
            {
                Block block = CreateCharBlock(data, 4, 10);
                block.Error = &error;
                TerminateStringBlock(&block);
                REQUIRE(data[4] == STRING_TERMINATOR);
                REQUIRE(error == FALSE);
            }
        }
        WHEN("Provided with a block of invalid size the function should set the termination charcter and report error")
        {
            SECTION("size = 8")
            {
                Block block = CreateCharBlock(data, 8, 8);
                block.Error = &error;
                TerminateStringBlock(&block);
                REQUIRE(data[8] == STRING_TERMINATOR);
                REQUIRE(error == TRUE);
            }
            SECTION("size = 0")
            {
                Block block = CreateCharBlock(data, 0, 0);
                block.Error = &error;
                TerminateStringBlock(&block);
                REQUIRE(data[0] == STRING_TERMINATOR);
                REQUIRE(error == TRUE);
            }
            SECTION("size = 9")
            {
                Block block = CreateCharBlock(data, 9, 6);
                block.Error = &error;
                TerminateStringBlock(&block);
                REQUIRE(data[9] == STRING_TERMINATOR);
                REQUIRE(error == TRUE);
            }
        }
    }

    TEST_CASE("[Blocks::CreateCharBlock] : CreateCharBlock", "[Blocks][CreateCharBlock]")
    {

    }

    TEST_CASE("[Blocks::CreateCharBlock] : CreateCharBlock", "[Blocks][CreateCharBlock]")
    {

    }

    */
}
#endif // BLOCKS_H
