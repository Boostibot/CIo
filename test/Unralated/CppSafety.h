#ifndef CSAFETY_H
#define CSAFETY_H

#include "Catch2/Catch.hpp"

namespace CSafety
{

#include <cstdio>
#include <cstdlib>

//Typedefs
#include <cstdint>
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef uint8_t byte;

//Toggles:
#define SAFE_ARRAY
#define SAFE_ALLOC
//#define SAFE_TESTING

typedef unsigned int  Size;
typedef double        Decimal;
typedef unsigned char Bool;

#define SIZE_FORMAT "%u"

#define TRUE 1
#define FALSE 0

#ifdef SAFE_ARRAY

#ifdef SAFE_TESTING

    struct ErrorReport
    {
            Size Index;
            Size ElemSize;
            Size Size;
            int Line;
            void * Pointer;
    };

    ErrorReport Report;

    void ResetReport(Size index = 0, Size elemSize = 0, Size size = 0, int line = 0, void * pointer = NULL)
    {
        Report.Index = index;
        Report.ElemSize = elemSize;
        Report.Size = size;
        Report.Line = line;
        Report.Pointer = pointer;
    }

    #define REMOVE_UNUSED(x) (void)(x)

    #define CHECK_INDEX_FULL(index, elemSize, size, indexName, functionName, file, line, function) \
        ResetReport(index, elemSize, size, line); \
        REMOVE_UNUSED(indexName); \
        REMOVE_UNUSED(functionName); \
        REMOVE_UNUSED(file); \
        REMOVE_UNUSED(function); \


#endif //#ifdef SAFE_TESTING

#ifndef SAFE_TESTING
#define CHECK_INDEX_FULL(index, elemSize, size, indexName, functionName, file, line, function) \
    if((index) + (elemSize) > (size)) \
    { \
        printf("[%s]:\tInvalid arguments: acessed " indexName " exceeding size\n", (functionName)); \
        printf("[%s]:\t%s: " SIZE_FORMAT "\n",                (functionName), (indexName), (Size)(index)); \
        printf("[%s]:\tacessed size: " SIZE_FORMAT "\n",      (functionName), (Size)(elemSize)); \
        printf("[%s]:\tsize: " SIZE_FORMAT "\n",              (functionName), (Size)(size)); \
        printf("--> %s (%d) %s(...)\n",         (file), (int)(line), (function)); \
    }
#endif //#ifndef SAFE_TESTING

    #define CHECK_INDEX(index, elemSize, size, indexName, functionName) CHECK_INDEX_FULL(index, elemSize, size, indexName, functionName, __FILE__, __LINE__, __func__)

    Size InternalIdentityAndCheck(Size index, Size elemSize, Size capacity,
                                         const char* functionName, const char * file, int line, const char* function)
    {
        CHECK_INDEX_FULL(index, elemSize, capacity, "index", functionName,
                    file, line, function);
        return index;
    }
    void InternalSetAndCheck(Size * what, Size to, Size elemSize, Size capacity,
                                    const char* functionName, const char * file, int line, const char* function)
    {
        CHECK_INDEX_FULL(*what, elemSize, capacity, "size", functionName,
                    file, line, function);
        *what = to;
    }

    #define IDENTITY_SET_AND_CHECK(what, to, elemSize, capacity, funcName)      InternalSetAndCheck(&(what), to, elemSize, capacity, funcName, __FILE__, __LINE__, __func__)
    #define IDENTITY_AND_CHECK(index, elemSize, capacity, funcName)             InternalIdentityAndCheck(index, elemSize, capacity, funcName, __FILE__, __LINE__, __func__)

    #define At(array, index, size)          ((array)[InternalIdentityAndCheck(index, 1, size, "At", __FILE__, __LINE__, __func__)])

#endif //#ifdef SAFE_ARRAY

#ifndef SAFE_ARRAY

    #define CHECK_INDEX_FULL(index, elemSize, size, indexName, functionName, file, line, function)
    #define CHECK_INDEX(index, elemSize, size, indexName, functionName)
    #define IDENTITY_AND_CHECK(index, elemSize, capacity, funcName) (index)
    #define IDENTITY_SET_AND_CHECK(what, to, elemSize, capacity, funcName) (what) = (to)
    #define At(array, index, size)          ((array)[index])

#endif //#ifndef SAFE_ARRAY

//#define At(array, index, size)          ((array)[IDENTITY_AND_CHECK((index), 1, (size), "At")])
//Use:
//
// Classic:                     New
//
// int myArr[100];          --> int myArr[100];
// myArr[0] = 5;            --> At(myArr, 0, 100) = 5;
// int copy = myArr[0];     --> int copy = At(myArr, 0, 100);


//SAFE ALLOC===================================================================
#ifdef SAFE_ALLOC

    typedef long long MemoryBalance;

    typedef struct ALLOC_GUARD_ENTRY
    {
            void* Allocated;
            Size AllocatedSize;
            Size TypeSize;
            Bool Freed;

            const char * File;
            int Line;
            const char * Function;
    } AllocGuardEntry;

    typedef struct ALLOC_GUARD
    {
            AllocGuardEntry * Entries;
            Size EntryCount;
            Size EntriesAllocated;
            MemoryBalance TotalMemoryBalance;
    } AllocGuard;

    #define SET_UP_ALLOC_GUARD {NULL, 0, 0, 0}

    AllocGuard allocGuard = SET_UP_ALLOC_GUARD;

    void InternalTearDownAllocGuard()
    {
        for(Size i = 0; i < allocGuard.EntryCount; i++)
        {
            if(allocGuard.Entries[i].Freed == FALSE)
                free(allocGuard.Entries[i].Allocated);
        }

        if(allocGuard.Entries != NULL)
            free(allocGuard.Entries);


        AllocGuard temp = SET_UP_ALLOC_GUARD;
        allocGuard = temp;
    }

    void InternalPrintMemoryStateWithFiles()
    {
        printf("[PrintMemoryState]:\tPrinting all of %u memory operations\n", allocGuard.EntryCount);
        for(Size i = 0; i < allocGuard.EntryCount; i++)
        {
            AllocGuardEntry * at = &(allocGuard.Entries[i]);
            if(at->Freed)
                printf("[X] [" SIZE_FORMAT "]: (%p) %ux%u\t --> %s (%d) %s(...)\n", i, at->Allocated, at->AllocatedSize, at->TypeSize, at->File, at->Line, at->Function);
            else
                printf("[ ] [" SIZE_FORMAT "]: (%p) %ux%u\t --> %s (%d) %s(...)\n", i, at->Allocated, at->AllocatedSize, at->TypeSize, at->File, at->Line, at->Function);
        }
    }

    void InternalPrintMemoryState()
    {
        printf("[PrintMemoryState]:\tPrinting all of %u memory operations\n", allocGuard.EntryCount);
        for(Size i = 0; i < allocGuard.EntryCount; i++)
        {
            AllocGuardEntry * at = &(allocGuard.Entries[i]);
            if(at->Freed)
                printf("[X] [" SIZE_FORMAT "]: (%p) %ux%u\t --> (%d) %s(...)\n", i, at->Allocated, at->AllocatedSize, at->TypeSize, at->Line, at->Function);
            else
                printf("[ ] [" SIZE_FORMAT "]: (%p) %ux%u\t --> (%d) %s(...)\n", i, at->Allocated, at->AllocatedSize, at->TypeSize, at->Line, at->Function);
        }
    }

    long long InternalGetMemoryBalance()
    {
        return allocGuard.TotalMemoryBalance;
    }

    void InternalPrintMemoryBalance()
    {
        printf("[PrintMemoryBalance]:\tbalance: %lld\n", allocGuard.TotalMemoryBalance);
    }

    void InternalAddEntry(const AllocGuardEntry * entry)
    {
        if(allocGuard.Entries == NULL)
        {
            allocGuard.EntriesAllocated = 5;
            allocGuard.Entries = (AllocGuardEntry *)malloc(allocGuard.EntriesAllocated * sizeof (AllocGuardEntry));
        }
        else if(allocGuard.EntryCount + 1 > allocGuard.EntriesAllocated)
        {
            allocGuard.EntriesAllocated *= 2;
            allocGuard.Entries = (AllocGuardEntry *)realloc(allocGuard.Entries, allocGuard.EntriesAllocated * sizeof (AllocGuardEntry));
        }

        allocGuard.Entries[allocGuard.EntryCount] = *entry;

        allocGuard.Entries[allocGuard.EntryCount].Freed = FALSE;
        allocGuard.TotalMemoryBalance += entry->AllocatedSize * entry->TypeSize;

        allocGuard.EntryCount++;
    }

    void InternalMarkEntryFreed(AllocGuardEntry * entry)
    {
        entry->Freed = TRUE;
        allocGuard.TotalMemoryBalance -= entry->AllocatedSize * entry->TypeSize;
    }

    void InternalAddNewEntry(void* Allocated, Size AllocatedSize, Size TypeSize, const char * File, int Line, const char * Function)
    {
        AllocGuardEntry entry;
        entry.Allocated = Allocated;
        entry.AllocatedSize = AllocatedSize;
        entry.TypeSize = TypeSize;

        entry.File = File;
        entry.Line = Line;
        entry.Function = Function;

        InternalAddEntry(&entry);
    }


#ifndef SAFE_TESTING
    #define MEM_SCREAM(location, calledFrom, file, line, func) \
        printf("[%s]:\tInvallid %s on unallocated block\n", calledFrom, calledFrom); \
        printf("[%s]:\tpointer: %p\n", (calledFrom), location); \
        printf("--> %s (%d) %s(...)\n", file, line, function);
#endif

#ifdef SAFE_TESTING
    #define MEM_SCREAM(location, calledFrom, file, line, func) \
    ResetReport(0, 0, 0, line, location); \
    REMOVE_UNUSED(calledFrom); \
    REMOVE_UNUSED(file); \
    REMOVE_UNUSED(func);
#endif

    AllocGuardEntry * InternalLocateEntry(void * withAllocated)
    {
        for(Size i = 0; i < allocGuard.EntryCount; i++)
        {
            AllocGuardEntry * at = &(allocGuard.Entries[i]);
            if(at->Allocated == withAllocated && at->Freed == FALSE)
                return at;
        }

        return NULL;
    }

    void * InternalMalloc(Size size, Size elemSize,
                                const char * file, int line, const char* function)
    {
        void * where = malloc(size*elemSize);
        InternalAddNewEntry(where, size, elemSize, file, line, function);
        return where;
    }

    void* InternalRealloc(void* from, Size newSize, Size elemSize,
                                const char* functionName, const char * file, int line, const char* function)
    {
        AllocGuardEntry* located = InternalLocateEntry(from);
        if(located == NULL)
        {
            MEM_SCREAM(from, functionName, file, line, function);
            return NULL;
        }
        void *to = realloc(from, newSize * elemSize);

        InternalMarkEntryFreed(located);
        InternalAddNewEntry(to, newSize, elemSize, file, line, function);
        return to;
    }

    void InternalFree(void* what,
                      const char* functionName, const char * file, int line, const char* function)
    {
        AllocGuardEntry* located = InternalLocateEntry(what);
        if(located == NULL)
        {
            MEM_SCREAM(what, functionName, file, line, function);
            return;
        }

        free(what);
        InternalMarkEntryFreed(located);
    }

#define Malloc(size, type)                  ((type*)InternalMalloc((size), sizeof(type), __FILE__, __LINE__, __func__))
#define Realloc(ptr, newSize, type)         ((type*)InternalRealloc((ptr), (newSize), sizeof(type), "Realloc", __FILE__, __LINE__, __func__))
#define Free(ptr)                           (InternalFree((ptr), "Free", __FILE__, __LINE__, __func__))

#define GetMemoryBalance()                  InternalGetMemoryBalance()
#define PrintMemoryBalance()                InternalPrintMemoryBalance()
#define PrintMemoryState()                  InternalPrintMemoryState()
#define PrintMemoryStateWithFiles()         InternalPrintMemoryStateWithFiles()
#define TearDownAllocGuard()                InternalTearDownAllocGuard()
#endif //#ifdef SAFE_ALLOC

#ifndef SAFE_ALLOC
#define Malloc(size, type)                  ((type*)malloc((size) * sizeof(type)))
#define Realloc(ptr, size, type)            ((type*)realloc((ptr), (size) * sizeof(type)))
#define Free(ptr)                           (free(ptr))

#define GetMemoryBalance()
#define PrintMemoryBalance()
#define PrintMemoryState()
#define PrintMemoryStateWithFiles()
#define TearDownAllocGuard()
#endif //#ifndef SAFE_ALLOC

    //Use:
    //
    // int * myAllocated = (int*)malloc(100 * sizeof(int));             --->   -||- = Malloc(100, int);
    // myAllocated = (int*)realloc(myAllocated, 200 * sizeof(int));     --->   -||- = Realloc(myAllocated, 200, int);
    // free(myAllocated);                                               --->   -||- = Free(myAllocated);

    // Additionally you can use the following meta functions:
    //  when SAFE_ALLOC is not defined they simply expand into nothing.
    //
    // GetMemoryBalance()   - Returns the current balance of memory ie. all allocated - all freed bytes (should be 0 at the end of the program - allocated = freed)
    // PrintMemoryBalance() - Prints into output the above --^
    // PrintMemoryState()   - Prints all allocated blocks with all of the relevant information. For more information see below
    // PrintMemoryStateWithFiles() - Same as above except also prints the file in which the function was called
    // TearDownAllocGuard() - Frees the internal memory used to manage the saved info about allocated blcoks. Also frees all blocks still not freed

    // PrintMemoryState format:
    // [X] [1]: (0x0000142F0) 100x4 - main.cpp (33) main(...)
    //  ^   ^         ^        ^  ^      ^      ^     ^
    //  | block index |     count |     file    |   function
    //  |             |           |             |
    // freed     block adress   sizeof type    line

    //freed:        [X] if the block was freed [ ] if not
    //block index:  the index in the printed list
    //block adress: the adress of the allocated block
    //count:        the count of elements passed into Malloc(count, type)
    //sizeof type:  the size of type passed into Malloc(count, type)
    //file:         the source code file from which the Malloc/Realloc function was called
    //line:         the source code line  --||--
    //function:     the source code function --||--

//GROWING (Requires Safe alloc) ===================================================================================

    #include <math.h>
    #define LOG_2 0.69314718055995
    Size CalculateGrowthFactor(Size currentSize, Size minNewSize)
    {
        //Decimal log2 = log_2( toFits / block->ByteCapacity );
        const Decimal log2 = log((Decimal)minNewSize / (Decimal)currentSize) / LOG_2;
        const Decimal ceiled = ceil(log2);
        const Size finalMultiply = (Size)pow(2, ceiled);
        return finalMultiply;
    }

    Bool GrowImpl(void** to, Size* currSize, Size toFitSize, Size elementSize)
    {
        if(*currSize >= toFitSize)
            return FALSE;

        if(*to == NULL)
        {
            *to = malloc(toFitSize * elementSize);
            *currSize = toFitSize;
            return TRUE;
        }

        Size newSize;
        if(*currSize == 0)
            newSize = *currSize;
        else
            newSize = *currSize * CalculateGrowthFactor(*currSize, toFitSize);

        *to = realloc(*to, newSize * elementSize);
        return TRUE;
    }

    Bool ShrinkImpl(void** to, Size* currSize)
    {
        if(*to != NULL)
        {
            free(*to);
            *to = NULL;
            *currSize = 0;
            return TRUE;
        }
        else
        {
            *currSize = 0;
            return FALSE;
        }
    }

#ifdef SAFE_ALLOC
    Bool InternalGrow(void** to, Size* currSize, Size toFitSize, Size elementSize,
                                const char* functionName, const char * file, int line, const char* function)
    {
        if(*currSize >= toFitSize)
            return FALSE;

        if(*to == NULL)
        {
            *to = InternalMalloc(toFitSize, elementSize, file, line, function);
            *currSize = toFitSize;
            return TRUE;
        }

        Size newSize;
        if(*currSize == 0)
            newSize = *currSize;
        else
            newSize = *currSize * CalculateGrowthFactor(*currSize, toFitSize);

        *to = InternalRealloc(*to, newSize, elementSize, functionName, file, line, function);
        return TRUE;
    }
    Bool InternalShrink(void** to, Size* currSize,
                                const char* functionName, const char * file, int line, const char* function)
    {
        if(*to != NULL)
        {
            InternalFree(*to, functionName, file, line, function);
            *to = NULL;
            *currSize = 0;
            return TRUE;
        }
        else
        {
            *currSize = 0;
            return FALSE;
        }
    }

#define Grow(ptr, currSize, newSize, type)  (InternalGrow((void**)&(ptr), &(currSize), newSize, sizeof(type), "Grow", __FILE__, __LINE__, __func__))
#define Shrink(ptr, currSize)               (InternalShrink((void**)&(ptr), &(currSize), "Shrink", __FILE__, __LINE__, __func__))
#endif //#ifdef SAFE_ALLOC

#ifndef SAFE_ALLOC
#define Grow(ptr, currSize, newSize, type)  (GrowImpl((void**)&(ptr), &(currSize), newSize, sizeof(type)))
#define Shrink(ptr, currSize)               (ShrinkImpl((void**)&(ptr), &(currSize)))
#endif

void readAll(FILE * file, Size blockSize, byte** bufferPtr, Size* bufferSize, Size* bufferCapacity)
{
    if(*bufferPtr == NULL)
    {
        *bufferSize = 0;
        *bufferCapacity = blockSize;
        *bufferPtr = (byte*)malloc(*bufferCapacity);
    }

    while(TRUE)
    {
        if(bufferCapacity - bufferSize < blockSize)
        {
            (*bufferCapacity) *= 2;
            *bufferPtr = (byte*)realloc(*bufferPtr, *bufferCapacity);
        }

        size_t read = fread(*bufferPtr + *bufferSize, blockSize, sizeof (byte), file);

        (*bufferSize) += (Size)read;

        if(read != (size_t)blockSize)
            break;
    }
}


int xmain()
{
    int * array = Malloc(400, int);
    At(array, 20, 400) = 32;
    At(array, 20, 19) = 15; //Error
    array = Realloc(array, 800, int);
    array = Realloc(array, 800, int);
    array = Realloc(array, 800, int);
    array = Realloc(array, 800, int); //Not freed

    int * array2 = Malloc(400, int);
    Free(array2);

    PrintMemoryBalance();
    PrintMemoryState();

    TearDownAllocGuard();

    int * grown = NULL;
    Size allocated = 0;

    Grow(grown, allocated, 100, int);
    Grow(grown, allocated, 200, int);
    Grow(grown, allocated, 400, int);

    Shrink(grown, allocated);

    PrintMemoryBalance();
    PrintMemoryState();


    return 0;
}

}

#ifdef SAFE_TESTING
namespace CSafety::Test
{
    TEST_CASE("[CSafety::SET_UP_ALLOC_GUARD] : SET_UP_ALLOC_GUARD should initialise alloc guard", "[CSafety::SET_UP_ALLOC_GUARD]")
    {
        TearDownAllocGuard();
        allocGuard = SET_UP_ALLOC_GUARD;

        REQUIRE(allocGuard.Entries == NULL);
        REQUIRE(allocGuard.EntriesAllocated == 0);
        REQUIRE(allocGuard.EntryCount == 0);
        REQUIRE(allocGuard.TotalMemoryBalance == 0);
        TearDownAllocGuard();
    }

    TEST_CASE("[CSafety::InternalGetMemoryBalance] : InternalGetMemoryBalance should return the mem balance of allocGuard", "[CSafety::InternalGetMemoryBalance]")
    {
        TearDownAllocGuard();
        allocGuard.TotalMemoryBalance = 123;
        REQUIRE(InternalGetMemoryBalance() == 123);

        allocGuard.TotalMemoryBalance = -32;
        REQUIRE(InternalGetMemoryBalance() == -32);

        allocGuard.TotalMemoryBalance = 0;
        REQUIRE(InternalGetMemoryBalance() == 0);
        TearDownAllocGuard();
    }

    TEST_CASE("[CSafety::InternalAddEntry] : InternalAddEntry should add a new entry to the allocGuard and change the mem balance", "[CSafety::InternalAddEntry]")
    {
        AllocGuardEntry entry;
        SECTION("Adding entry should add its values and increase size")
        {
            TearDownAllocGuard();

            entry = {NULL, 23, 4, TRUE, "File", 764, "Func"};
            InternalAddEntry(&entry);

            REQUIRE(allocGuard.EntryCount == 1);
            REQUIRE(allocGuard.EntriesAllocated > 0);
            REQUIRE(allocGuard.Entries[0].Allocated == NULL);
            REQUIRE(allocGuard.Entries[0].AllocatedSize == 23);
            REQUIRE(allocGuard.Entries[0].TypeSize == 4);
            REQUIRE(allocGuard.Entries[0].Freed == FALSE);
            REQUIRE(allocGuard.Entries[0].File == std::string_view("File"));
            REQUIRE(allocGuard.Entries[0].Line == 764);
            REQUIRE(allocGuard.Entries[0].Function == std::string_view("Func"));
            REQUIRE(allocGuard.TotalMemoryBalance == 23*4);

            entry = {NULL, 84, 8, TRUE, "File2", 123, "Func2"};
            InternalAddEntry(&entry);

            REQUIRE(allocGuard.EntryCount == 2);
            REQUIRE(allocGuard.EntriesAllocated >= 2);
            REQUIRE(allocGuard.Entries[1].Allocated == NULL);
            REQUIRE(allocGuard.Entries[1].AllocatedSize == 84);
            REQUIRE(allocGuard.Entries[1].TypeSize == 8);
            REQUIRE(allocGuard.Entries[1].Freed == FALSE);
            REQUIRE(allocGuard.Entries[1].File == std::string_view("File2"));
            REQUIRE(allocGuard.Entries[1].Line == 123);
            REQUIRE(allocGuard.Entries[1].Function == std::string_view("Func2"));
            REQUIRE(allocGuard.TotalMemoryBalance == 23*4 + 84*8);

            allocGuard.Entries[0].Freed = TRUE;
            allocGuard.Entries[1].Freed = TRUE;
        }

        SECTION("Adding entry n time should add n entries")
        {
            TearDownAllocGuard();
            entry = {NULL, 23, 4, TRUE, "File", 764, "Func"};

            constexpr Size iters = 10000;

            for(Size i = 0; i < iters; i++)
            {
                InternalAddEntry(&entry);
            }
            REQUIRE(allocGuard.EntryCount == iters);
            REQUIRE(allocGuard.EntriesAllocated >= iters);
            REQUIRE(allocGuard.TotalMemoryBalance == 23*4*iters);

        }
        TearDownAllocGuard();
    }

    TEST_CASE("[CSafety::InternalAddNewEntry] : InternalAddNewEntry should add a new entry with the given values to the allocGuard and change the mem balance", "[CSafety::InternalAddNewEntry]")
    {
        AllocGuardEntry entry;
        SECTION("Adding entry should add its values and increase size")
        {
            TearDownAllocGuard();

            entry = {NULL, 23, 4, TRUE, "File", 764, "Func"};
            InternalAddNewEntry(NULL, 23, 4, "File", 764, "Func");

            REQUIRE(allocGuard.EntryCount == 1);
            REQUIRE(allocGuard.EntriesAllocated > 0);
            REQUIRE(allocGuard.Entries[0].Allocated == NULL);
            REQUIRE(allocGuard.Entries[0].AllocatedSize == 23);
            REQUIRE(allocGuard.Entries[0].TypeSize == 4);
            REQUIRE(allocGuard.Entries[0].Freed == FALSE);
            REQUIRE(allocGuard.Entries[0].File == std::string_view("File"));
            REQUIRE(allocGuard.Entries[0].Line == 764);
            REQUIRE(allocGuard.Entries[0].Function == std::string_view("Func"));
            REQUIRE(allocGuard.TotalMemoryBalance == 23*4);

            allocGuard.Entries[0].Freed = TRUE;
        }
        TearDownAllocGuard();
    }

    TEST_CASE("[CSafety::InternalMarkEntryFreed] : InternalMarkEntryFreed should mark an entry freed and change the mem balance", "[CSafety::InternalMarkEntryFreed]")
    {
        TearDownAllocGuard();
        allocGuard.TotalMemoryBalance = 34;

        AllocGuardEntry entry;
        entry = {NULL, 64, 4, FALSE, "File", 23, "Func"};

        InternalMarkEntryFreed(&entry);
        REQUIRE(entry.Allocated == NULL);
        REQUIRE(entry.AllocatedSize == 64);
        REQUIRE(entry.TypeSize == 4);
        REQUIRE(entry.Freed == TRUE);
        REQUIRE(entry.File == std::string_view("File"));
        REQUIRE(entry.Line == 23);
        REQUIRE(entry.Function == std::string_view("Func"));
        REQUIRE(allocGuard.TotalMemoryBalance == 34 - 64*4);

        TearDownAllocGuard();
    }

    TEST_CASE("[CSafety::InternalLocateEntry] : InternalLocateEntry should locate an entry given a valid pointer to memory", "[CSafety::InternalLocateEntry]")
    {
        TearDownAllocGuard();
        constexpr Size memSize = 10000;
        constexpr Size iters = memSize - 20;

        byte mem[memSize];
        AllocGuardEntry entry = {NULL, 0, 4, TRUE, "File", 764, "Func"};

        for(Size i = 0; i < iters; i++)
        {
            entry.Allocated = &(mem[i]);
            entry.AllocatedSize = i;
            InternalAddEntry(&entry);
        }


        AllocGuardEntry * located;
        located = InternalLocateEntry((void*)&(mem[34]));
        REQUIRE(located != NULL);
        REQUIRE(located->AllocatedSize == 34);

        located = InternalLocateEntry((void*)&(mem[54]));
        REQUIRE(located != NULL);
        REQUIRE(located->AllocatedSize == 54);

        located = InternalLocateEntry((void*)&(mem[84]));
        REQUIRE(located != NULL);
        REQUIRE(located->AllocatedSize == 84);

        located = InternalLocateEntry((void*)&(mem[iters]));
        REQUIRE(located == NULL);

        located = InternalLocateEntry(NULL);
        REQUIRE(located == NULL);

        //Since entries are not really allocated
        // we need to set them freed in order to not crash here
        for(Size i = 0; i < iters; i++)
            allocGuard.Entries[i].Freed = TRUE;

        TearDownAllocGuard();
    }


    TEST_CASE("[CSafety::InternalTearDownAllocGuard] : InternalTearDownAllocGuard should free the allocGuard storage and free all not yet freed entries", "[CSafety::InternalTearDownAllocGuard]")
    {
        TearDownAllocGuard();
        constexpr Size memSize = 100;
        constexpr Size iters = memSize - 20;

        byte mem[memSize];
        AllocGuardEntry entry = {NULL, 0, 4, TRUE, "File", 764, "Func"};

        for(Size i = 0; i < iters; i++)
        {
            entry.Allocated = &(mem[i]);
            entry.AllocatedSize = i;
            InternalAddEntry(&entry);
        }

        //Since entries are not really allocated
        // we need to set them freed in order to not crash here
        for(Size i = 0; i < iters; i++)
            allocGuard.Entries[i].Freed = TRUE;

        InternalTearDownAllocGuard();

        REQUIRE(allocGuard.Entries == NULL);
        REQUIRE(allocGuard.EntriesAllocated == 0);
        REQUIRE(allocGuard.EntryCount == 0);
        REQUIRE(allocGuard.TotalMemoryBalance == 0);
        TearDownAllocGuard();
    }

    TEST_CASE("[CSafety::InternalMalloc] : InternalMalloc should allocate memory and add a new entry", "[CSafety::InternalMalloc]")
    {
        TearDownAllocGuard();
        constexpr Size elemSize = sizeof (int);
        constexpr Size allocSize = 100;
        void * allocated = InternalMalloc(allocSize, elemSize, "File", 23, "Func");

        REQUIRE(allocGuard.Entries != NULL);
        REQUIRE(allocGuard.EntriesAllocated >= 1);
        REQUIRE(allocGuard.EntryCount == 1);
        REQUIRE(allocGuard.TotalMemoryBalance == allocSize * elemSize);

        REQUIRE(allocGuard.Entries[0].Allocated == allocated);
        REQUIRE(allocGuard.Entries[0].AllocatedSize == allocSize);
        REQUIRE(allocGuard.Entries[0].TypeSize == elemSize);
        REQUIRE(allocGuard.Entries[0].Freed == FALSE);
        REQUIRE(allocGuard.Entries[0].File == std::string_view("File"));
        REQUIRE(allocGuard.Entries[0].Line == 23);
        REQUIRE(allocGuard.Entries[0].Function == std::string_view("Func"));

        TearDownAllocGuard();
    }

    TEST_CASE("[CSafety::InternalRealloc] : InternalRealloc should reallocate memory, mark reallocated block freed and add a new entry", "[CSafety::InternalMalloc]")
    {
        constexpr Size elemSize = sizeof (int);
        constexpr Size allocSize = 100;
        constexpr Size reallocSize = 200;

        WHEN("Provided with previously allocated realloc adress it should succed")
        {
            TearDownAllocGuard();

            void * allocated = InternalMalloc(allocSize, elemSize, "File", 23, "Func");
            void * reallocated = InternalRealloc(allocated, reallocSize, elemSize, "calledFrom", "File2", 58, "Func2");
            REMOVE_UNUSED(reallocated);

            REQUIRE(allocGuard.Entries != NULL);
            REQUIRE(allocGuard.EntriesAllocated >= 2);
            REQUIRE(allocGuard.EntryCount == 2);

            REQUIRE(allocGuard.Entries[0].Freed == TRUE);
            REQUIRE(allocGuard.Entries[1].Allocated == reallocated);
            REQUIRE(allocGuard.Entries[1].AllocatedSize == reallocSize);
            REQUIRE(allocGuard.Entries[1].TypeSize == elemSize);
            REQUIRE(allocGuard.Entries[1].Freed == FALSE);
            REQUIRE(allocGuard.Entries[1].File == std::string_view("File2"));
            REQUIRE(allocGuard.Entries[1].Line == 58);
            REQUIRE(allocGuard.Entries[1].Function == std::string_view("Func2"));
        }

        WHEN("Provided with realloc adress previously not allocated it should fail")
        {
            TearDownAllocGuard();
            {
                ResetReport();
                void * reallocated = InternalRealloc(NULL, reallocSize, elemSize, "calledFrom", "File2", 58, "Func2");
                REMOVE_UNUSED(reallocated);
                REQUIRE(Report.Line == 58);
                REQUIRE(Report.Pointer == NULL);
            }

            {
                ResetReport();
                int tempInt = 0;
                void * reallocated = InternalRealloc((void*)&tempInt, reallocSize, elemSize, "calledFrom", "File2", 32, "Func2");

                REMOVE_UNUSED(reallocated);

                REQUIRE(Report.Line == 32);
                REQUIRE(Report.Pointer == (void*)&tempInt);
            }
        }

        TearDownAllocGuard();
    }

    TEST_CASE("[CSafety::InternalFree] : InternalFree should free memory and mark reallocated block freed", "[CSafety::InternalFree]")
    {
        constexpr Size elemSize = sizeof (int);
        constexpr Size allocSize = 100;

        WHEN("Provided with previously allocated free adress it should succed")
        {
            TearDownAllocGuard();

            void * allocated = InternalMalloc(allocSize, elemSize, "File", 23, "Func");
            InternalFree(allocated, "funcName", "file", 32, "func");

            REQUIRE(allocGuard.Entries != NULL);
            REQUIRE(allocGuard.EntriesAllocated >= 1);
            REQUIRE(allocGuard.EntryCount == 1);

            REQUIRE(allocGuard.Entries[0].Freed == TRUE);
        }

        WHEN("Provided with realloc adress previously not allocated it should fail")
        {
            TearDownAllocGuard();
            {
                ResetReport();
                InternalFree(NULL, "calledFrom", "File2", 58, "Func2");
                REQUIRE(Report.Line == 58);
                REQUIRE(Report.Pointer == NULL);
            }

            {
                ResetReport();
                int tempInt = 0;
                InternalFree((void*)&tempInt, "calledFrom", "File2", 32, "Func2");
                REQUIRE(Report.Line == 32);
                REQUIRE(Report.Pointer == (void*)&tempInt);
            }

        }

        TearDownAllocGuard();
    }


}
#endif



/*
void testTestFunc(int goTo1, int goTo2)
{
    if(goTo1 == 0)
    {
        testTestFunc(1, 0);
    }
    else if(goTo1 == 1)
    {
        //execute
        puts("[1]");

        if(goTo2 == 0)
        {
            testTestFunc(1, 1);
        }
        else if(goTo2 == 1)
        {
            puts("[1] [1]");
            //execute
        }

        if(goTo2 == 0)
        {
            testTestFunc(1, 2);
        }
        else if(goTo2 == 2)
        {
            puts("[1] [2]");
            //execute
        }
    }

    if(goTo1 == 0)
    {
        testTestFunc(2, 0);
    }
    else if(goTo1 == 2)
    {
        puts("[2]");
        //execute
    }

    if(goTo1 == 0)
    {
        testTestFunc(3, 0);
    }
    else if(goTo1 == 3)
    {
        puts("[3]");
        //execute

        if(goTo2 == 0)
        {
            testTestFunc(3, 1);
        }
        else if(goTo2 == 1)
        {
            puts("[3] [1]");
            //execute
        }
    }
}

#define MAX_DEPTH 10
#define ANY_SECTION 0

void testTestFunc2(int goTos[], int currentChecks[], int currentDepth, int targetDepth, bool targetDepthReached)
{
    puts("[before]");

    if(goTos[currentDepth] == ANY_SECTION)
    {
        if(currentDepth + 1 > MAX_DEPTH)
        {
            puts("EXCEEDING DEPTH");
            return;
        }

        int newGoTos[MAX_DEPTH]; //= old;
        int newCurrentChecks[MAX_DEPTH]; //= old;

        memcpy(newGoTos, goTos, MAX_DEPTH * sizeof (int));

        newGoTos[currentDepth] = 1;
        newGoTos[currentDepth + 1] = ANY_SECTION;
        testTestFunc2(newGoTos, newCurrentChecks, 0, currentDepth + 1, false);
    }
    else if(goTos[currentDepth] == 1 && targetDepthReached == false)
    {
        currentDepth++;
        if(currentDepth >= targetDepth)
            targetDepthReached = true;

        //execute
        printf("[1][?] depth: %d target %d\n", currentDepth, targetDepth);

        if(goTos[currentDepth] == ANY_SECTION)
        {
            if(currentDepth + 1 > MAX_DEPTH)
            {
                puts("EXCEEDING DEPTH");
                return;
            }

            int newGoTos[MAX_DEPTH]; //= old;
            int newCurrentChecks[MAX_DEPTH]; //= old;

            memcpy(newGoTos, goTos, MAX_DEPTH * sizeof (int));

            newGoTos[currentDepth] = 1;
            newGoTos[currentDepth + 1] = ANY_SECTION;
            testTestFunc2(newGoTos, newCurrentChecks, 0, currentDepth + 1, false);
        }
        else if(goTos[currentDepth] == 1 && targetDepthReached == false)
        {
            currentDepth++;
            if(currentDepth >= targetDepth)
                targetDepthReached = true;
            //execute
            printf("[1][1] depth: %d target %d\n", currentDepth, targetDepth);

        }

        if(goTos[currentDepth] == ANY_SECTION)
        {
            puts("Exploring of second");
            if(currentDepth + 1 > MAX_DEPTH)
            {
                puts("EXCEEDING DEPTH");
                return;
            }

            int newGoTos[MAX_DEPTH]; //= old;
            int newCurrentChecks[MAX_DEPTH]; //= old;

            memcpy(newGoTos, goTos, MAX_DEPTH * sizeof (int));

            newGoTos[currentDepth] = 2;
            newGoTos[currentDepth + 1] = ANY_SECTION;
            testTestFunc2(newGoTos, newCurrentChecks, 0, currentDepth + 1, false);
        }
        else if(goTos[currentDepth] == 2 && targetDepthReached == false)
        {
            currentDepth++;
            if(currentDepth >= targetDepth)
                targetDepthReached = true;
            //execute
            printf("[2][?] depth: %d target %d\n", currentDepth, targetDepth);

        }
    }


    if(goTos[currentDepth] == ANY_SECTION)
    {
        puts("Exploring of second");
        if(currentDepth + 1 > MAX_DEPTH)
        {
            puts("EXCEEDING DEPTH");
            return;
        }

        int newGoTos[MAX_DEPTH]; //= old;
        int newCurrentChecks[MAX_DEPTH]; //= old;

        memcpy(newGoTos, goTos, MAX_DEPTH * sizeof (int));

        newGoTos[currentDepth] = 2;
        newGoTos[currentDepth + 1] = ANY_SECTION;
        testTestFunc2(newGoTos, newCurrentChecks, 0, currentDepth + 1, false);
    }
    else if(goTos[currentDepth] == 2 && targetDepthReached == false)
    {
        currentDepth++;
        if(currentDepth >= targetDepth)
            targetDepthReached = true;
        //execute
        printf("[2][?] depth: %d target %d\n", currentDepth, targetDepth);

    }


    puts("[end]");
}
*/


#endif // CSAFETY_H
