#ifndef CPLUSPLUS_H
#define CPLUSPLUS_H

#include "Catch2/Catch.hpp"

#include <cstdlib>
#include <math.h>

namespace cplusplus
{
#define FALSE 0
#define TRUE 1

    typedef char Bool ;
    typedef unsigned int SizeT ;
    struct RaiiEntry
    {
            void (*Destructor)(void *);
            void* DestructedObj;
            Bool Destroyed;
    };

    inline void InternalCallRaiiEntryDestructor(struct RaiiEntry* raii)
    {
        raii->Destructor(raii->DestructedObj);
        raii->Destroyed = TRUE;
    }

    inline struct RaiiEntry InternalConstructRaiiEntry(void (*destructor)(void *), void* destructedObj, Bool destroyed)
    {
        struct RaiiEntry entry = {destructor, destructedObj, destroyed};
        return entry;
    }

    struct Raii
    {
            RaiiEntry* Entries;
            SizeT Attached;
            SizeT Capacity;
    };

    inline struct Raii InternalConstructRaii(struct RaiiEntry* entries, SizeT attached, SizeT capacity)
    {
        struct Raii raii {entries, attached, capacity};
        return raii;
    }

    inline void Attach(void* destructedObj, void (*destructor)(void *), struct Raii* raii)
    {
        if(raii->Attached >= raii->Capacity)
            return; //SCREAM

        raii->Entries[raii->Attached] = InternalConstructRaiiEntry(destructor, destructedObj, FALSE);
        raii->Attached ++;
    }
    inline Bool Detach(void* destructedObj, struct Raii* raii)
    {
        for(SizeT i = 0; i < raii->Attached; i++)
        {
            if(raii->Entries[i].DestructedObj == destructedObj)
            {
                InternalCallRaiiEntryDestructor(&(raii->Entries[i]));
                return TRUE;
            }
        }

        return FALSE;
    }
    inline void DetachAll(struct Raii* raii)
    {
        if(raii->Attached == 0)
            return;

        for(SizeT i = raii->Attached; i-- > 0;)
            InternalCallRaiiEntryDestructor(&(raii->Entries[i]));
    }

#define CONCAT_HELPER(a, b) a##b
#define CONCAT(a, b) CONCAT_HELPER(a, b)

#define destruct(type)   CONCAT(type, Destructor)
#define def_construct(type)   CONCAT(type, DefConstructor)

#define destructor_decl(type) \
    void destruct(Manager)(void * object)

#define def_construct_decl(type) \
    void def_construct(type)(struct type * manager, struct Raii * raii)

#define raii_begin_counted(count) \
    { \
        struct RaiiEntry _entries[count];\
        struct Raii _raiiObj = InternalConstructRaii(_entries, 0, count); \
        struct Raii * raii = &_raiiObj

#define raii_begin raii_begin_counted(10);

#define raii_end \
    DetachAll(raii); \
    }

#define raii_return  \
    DetachAll(raii); return

#define raii_attach(obj, type)  \
    Attach((void*)&obj, &destruct(type), raii)

#define moved(type) CONCAT(Moved, type)

#define moved_decl(type) \
    struct moved(type) \
    { struct type* const val; }; \
    inline struct moved(type) CONCAT(Move, type)(struct type * obj) \
    { \
        struct moved(type) _moved = {obj}; \
        return _moved; \
    }

#define move(obj) {&obj}
#define inline_move(obj, type) CONCAT(Move, type)(&obj)

#define movex(type) moved()

    struct Manager
    {
            SizeT Allocated;
            char * Data;
    };
    moved_decl(Manager);


    destructor_decl(Manager)
    {
        struct Manager * manager = (struct Manager*)object;
        printf("Manager destructor called with Allocated %u\n", manager->Allocated);

        if(manager->Data != NULL)
            free(manager->Data);
    }
    void DefConstructManager(struct Manager * manager, struct Raii * raii)
    {
        *manager = {0, NULL};
        raii_attach((*manager), Manager);
    }

    void ManagerConstruct(struct Manager * manager, SizeT size, struct Raii * raii)
    {
        *manager = {size, (char*)malloc(size)};
        printf("Manager constructor called with Allocated %u\n", manager->Allocated);
        raii_attach((*manager), Manager);
    }

    void ManagerMoveConstruct(struct Manager * manager, moved(Manager) from, struct Raii * raii)
    {
        manager->Allocated = from.val->Allocated;
        manager->Data = from.val->Data;

        *from.val = {0, NULL};

        printf("Manager move constructor called with Allocated %u\n", manager->Allocated);
        raii_attach((*manager), Manager);
    }

    struct Test
    {
            void * const ptr;
    };

    Bool RaiiTest()
    {
        raii_begin

        struct Manager man1;
        struct Manager man2;
        struct Manager man3;
        ManagerConstruct(&man1, 10, raii);
        ManagerConstruct(&man2, 20, raii);
        ManagerConstruct(&man3, 30, raii);

        man1.Data[9] = 'x';
        man2.Data[19] = 'x';
        man3.Data[29] = 'x';

        printf("man1 size: %u\n", man1.Allocated);
        printf("man2 size: %u\n", man2.Allocated);
        printf("man3 size: %u\n", man3.Allocated);

        struct Manager man4;
        //moved(Manager) moved = move(man1);
        //ManagerMoveConstruct(&man4, moved, raii);
        ManagerMoveConstruct(&man4, inline_move(man1, Manager), raii);

        if(man4.Allocated == 10)
            raii_return TRUE;

        raii_return FALSE;
        raii_end
    }
}

#endif // CPLUSPLUS_H
