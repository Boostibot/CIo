#include <stdio.h>
#include <stdlib.h>

//Typedefs
#include <stdint.h>
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

typedef unsigned int  Size;
typedef unsigned char bool;

#define SIZE_FORMAT "%u"

#define true 1
#define false 0

#ifdef SAFE_ARRAY

    Size InternalIdentityAndCheck(Size index, Size elemSize, Size capacity,
                                  const char * file, int line, const char* function)
    {
        if(index + elemSize > size)
        {
            printf("[At]:\tInvalid arguments: acessed index exceeding size\n");
            printf("[At]:\tindex: " SIZE_FORMAT "\n",           index);
            printf("[At]:\tacessed size: " SIZE_FORMAT "\n",    elemSize);
            printf("[At]:\tsize: " SIZE_FORMAT "\n",            capacity);
            printf("--> %s (%d) %s(...)\n",                     file, line, function));
        }

        return index;
    }
    #define At(array, index, size)          ((array)[InternalIdentityAndCheck(index, 1, size, __FILE__, __LINE__, __func__)])

#endif //#ifdef SAFE_ARRAY

#ifndef SAFE_ARRAY
    #define At(array, index, size)          ((array)[index])
#endif //#ifndef SAFE_ARRAY

//Use:
//
// Classic:                     New
//
// int myArr[100];          --> int myArr[100];
// myArr[0] = 5;            --> At(myArr, 0, 100) = 5;
// int copy = myArr[0];     --> int copy = At(myArr, 0, 100);

//SAFE ALLOC===================================================================
#ifdef SAFE_ALLOC

    typedef struct ALLOC_GUARD_ENTRY
    {
            void* Allocated;
            Size AllocatedSize;
            Size TypeSize;
            bool Freed;

            const char * File;
            int Line;
            const char * Function;
    } AllocGuardEntry;

    typedef struct ALLOC_GUARD
    {
            AllocGuardEntry * Entries;
            Size EntryCount;
            Size EntriesAllocated;
            int TotalMemoryBalance;
    } AllocGuard;

    #define SET_UP_ALLOC_GUARD {NULL, 0, 0, 0}

    AllocGuard allocGuard = SET_UP_ALLOC_GUARD;

    void InternalTearDownAllocGuard()
    {
        for(Size i = 0; i < allocGuard.EntryCount; i++)
        {
            if(allocGuard.Entries[i].Freed == false)
                free(allocGuard.Entries[i].Allocated);
        }

        if(allocGuard.Entries != NULL)
            free(allocGuard.Entries);


        AllocGuard temp = SET_UP_ALLOC_GUARD;
        allocGuard = temp;
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

    int InternalGetMemoryBalance()
    {
        return allocGuard.TotalMemoryBalance;
    }

    void InternalPrintMemoryBalance()
    {
        printf("[PrintMemoryBalance]:\tbalance: %d\n", allocGuard.TotalMemoryBalance);
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

        allocGuard.Entries[allocGuard.EntryCount].Freed = false;
        allocGuard.TotalMemoryBalance += entry->AllocatedSize * entry->TypeSize;

        allocGuard.EntryCount++;
    }

    void InternalMarkEntryFreed(AllocGuardEntry * entry)
    {
        entry->Freed = true;
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

    #define MEM_SCREAM(location, calledFrom, file, line, func) \
        printf("[%s]:\tInvallid %s on unallocated block\n", calledFrom, calledFrom); \
        printf("[%s]:\tpointer: %p\n", (calledFrom), location); \
        printf("--> %s (%d) %s(...)\n", file, line, function);

    AllocGuardEntry * InternalLocateEntry(void * withAllocated)
    {
        for(Size i = 0; i < allocGuard.EntryCount; i++)
        {
            AllocGuardEntry * at = &(allocGuard.Entries[i]);
            if(at->Allocated == withAllocated && at->Freed == false)
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
#define TearDownAllocGuard()                InternalTearDownAllocGuard()
#endif //#ifdef SAFE_ALLOC

#ifndef SAFE_ALLOC
#define Malloc(size, type)                  ((type*)malloc((size) * sizeof(type)))
#define Realloc(ptr, size, type)            ((type*)realloc((ptr), (size) * sizeof(type)))
#define Free(ptr)                           (free(ptr))

#define GetMemoryBalance()
#define PrintMemoryBalance()
#define PrintMemoryState()
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
        //double log2 = log_2( toFits / block->ByteCapacity );
        const double log2 = log((double)minNewSize / (double)currentSize) / LOG_2;
        const double ceiled = ceil(log2);
        const Size finalMultiply = (Size)pow(2, ceiled);
        return finalMultiply;
    }

    bool GrowImpl(void** to, Size* currSize, Size toFitSize, Size elementSize)
    {
        if(*currSize >= toFitSize)
            return false;

        if(*to == NULL)
        {
            *to = malloc(toFitSize * elementSize);
            *currSize = toFitSize;
            return true;
        }

        Size newSize;
        if(*currSize == 0)
            newSize = *currSize;
        else
            newSize = *currSize * CalculateGrowthFactor(*currSize, toFitSize);

        *to = realloc(*to, newSize * elementSize);
        return true;
    }

    bool ShrinkImpl(void** to, Size* currSize)
    {
        if(*to != NULL)
        {
            free(*to);
            *to = NULL;
            *currSize = 0;
            return true;
        }
        else
        {
            *currSize = 0;
            return false;
        }
    }

#ifdef SAFE_ALLOC
    bool InternalGrow(void** to, Size* currSize, Size toFitSize, Size elementSize,
                                const char* functionName, const char * file, int line, const char* function)
    {
        if(*currSize >= toFitSize)
            return false;

        if(*to == NULL)
        {
            *to = InternalMalloc(toFitSize, elementSize, file, line, function);
            *currSize = toFitSize;
            return true;
        }

        Size newSize;
        if(*currSize == 0)
            newSize = *currSize;
        else
            newSize = *currSize * CalculateGrowthFactor(*currSize, toFitSize);

        *to = InternalRealloc(*to, newSize, elementSize, functionName, file, line, function);
        return true;
    }
    bool InternalShrink(void** to, Size* currSize,
                                const char* functionName, const char * file, int line, const char* function)
    {
        if(*to != NULL)
        {
            InternalFree(*to, functionName, file, line, function);
            *to = NULL;
            *currSize = 0;
            return true;
        }
        else
        {
            *currSize = 0;
            return false;
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

    while(true)
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

int main()
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

    TearDownAllocGuard();
    return 0;
}
