#ifndef TEMP_H
#define TEMP_H

#include "Catch2/Catch.hpp"

#include <cstdlib>
#include <math.h>

#define SAFE_BLOCKS

namespace Blocks
{

#define TRUE 1
#define FALSE 0
#define BOOL char

#ifdef SAFE_BLOCKS
#define CHECK_INDEX(block, index, indexName, functionName) \
    if(newCharSize > block.Capacity) \
    { \
        printf("[Block::" functionName "]: Invalid arguments; " indexName " exceeding block Capacity\n"); \
        printf("[Block::" functionName "]: " indexName ": %u\n", index); \
        printf("[Block::" functionName "]: block Capacity: %u\n", block.Capacity); \
        printf("[Block::" functionName "]: Further execution undefined\n"); \
    }
#else
#define CHECK_INDEX(block, index, indexName, functionName)
#endif


    //Base
    struct Block
    {
            char * Data;
            unsigned int Size;
            unsigned int Capacity;
    };
    void * GetData(Block* block)            {return block.Data;}
    unsigned int GetSize(Block* block)      {return block.Size;}
    unsigned int GetCapacity(Block* block)  {return block.Capacity;}

    Block CreateEmptyBlock()
    {
        Block block;
        block.Data = NULL;
        block.Capacity = 0;
        block.Size = 0;

        return block;
    }

    BOOL IsCharIndexValid(Block* block, unsigned int charSize)
    {
        return charSize <= block.Capacity;
    }

    BOOL IsIntIndexValid(Block* block, unsigned int intSize)
    {
        return IsCharIndexValid(block, intSize*sizeof(int));
    }

    void SetCharSize(Block* block, unsigned int newCharSize)
    {
        CHECK_INDEX(block, newCharSize, "newCharSize", "SetCharSize");
        block.Size = newCharSize;
    }

    void SetIntSize(Block* block, unsigned int newIntSize)
    {
        SetCharSize(block, newIntSize*sizeof(int));
    }

    Block CreateCharBlock(char * data, unsigned int charSize, unsigned int charCapacity)
    {
        Block block;
        block.Data = data;
        block.Capacity = charCapacity;
        block.Size = charSize;
        CHECK_INDEX(block, newCharSize, "charSize", "CreateCharBlock");

        return block;
    }

    Block CreateIntBlock(int * data, unsigned int intSize, unsigned int intCapacity)
    {
        return CreateCharBlock(data, intSize*sizeof(int), intCapacity*sizeof(int));
    }

    char* CharAt(Block* block, unsigned int charindex)
    {
        CHECK_INDEX(block, charindex, "charindex", "CharAt");
        return block->Data[charindex];
    }

    int* IntAt(Block* block, unsigned int intIndex)
    {
        return CharAt(block, intIndex*sizeof(int));
    }


    //Allocating
    Block AllocateCharBlock(unsigned int charCapacity)
    {
        return CreateCharBlock((char*)malloc(charCapacity*sizeof(char)), 0, charCapacity);
    }

    Block AllocateIntBlock(unsigned int intCapacity)
    {
        return AllocateCharBlock(charCapacity*sizeof(int));
    }

}

namespace cio::Testing
{

    TEST_CASE("Testing of everything", "[Everything]")
    {
    }
}

#endif // TEMP_H
