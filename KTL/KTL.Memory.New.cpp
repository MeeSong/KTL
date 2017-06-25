#include "KTL.Memory.New.h"
#include "KTL.Macro.h"


static constexpr ktl::u32 DefaultPoolTag    = KTL$CompileTime$ByteSwap32$Macro('KNew');
static constexpr POOL_TYPE DefaultPoolType  = NonPagedPoolNx;

//////////////////////////////////////////////////////////////////////////

//
// Check buffer 
//

#pragma warning( push )
#pragma warning( disable: 4201)
typedef struct _POOL_HEADER
{
    union
    {
        struct
        {
            struct
            {
                UINT16       PreviousSize : 8;
                UINT16       PoolIndex : 8;
            };
            struct
            {
                UINT16       BlockSize : 8;
                UINT16       PoolType : 8;
            };
        };
        ULONG32      Ulong1;
    };
    ULONG32      PoolTag;
    union
    {
        struct _EPROCESS* ProcessBilled;
        struct
        {
            UINT16       AllocatorBackTraceIndex;
            UINT16       PoolTagHash;
        };
    };
}POOL_HEADER, *PPOOL_HEADER;
#pragma warning( pop )

void CheckPoolTag(void* aPtr, ktl::u32 aTag)
{
    if (PAGE_ALIGN(aPtr) == aPtr)
    {
        //
        // 不支持
        // 因为存储大页池的 nt!PoolBigPageTable 数组,
        // 以及描述数组大小的 nt!PoolBigPageTableSize 都未导出.
        //
        return;
    }

    POOL_HEADER *vPoolHeader = (POOL_HEADER*)(ktl::uintptr(aPtr) - sizeof(POOL_HEADER));
    if (aTag != vPoolHeader->PoolTag)
    {
        // See https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/bug-check-0x19--bad-pool-header
        KeBugCheckEx(
            BAD_POOL_HEADER,
            0x21,
            ktl::uintptr(aPtr),
            ktl::uintptr(vPoolHeader->BlockSize),
            ktl::uintptr(aTag));
    }
}

//////////////////////////////////////////////////////////////////////////
// replaceable usual deallocation functions
//////////////////////////////////////////////////////////////////////////

void * __cdecl operator new(size_t aSize) NOEXCEPT$TYPE
{
    // 根据C++标准，当size为1时，分配1字节

    if (0 == aSize)
    {
        aSize = 1;
    }
    
    return ExAllocatePoolWithTag(DefaultPoolType, aSize, DefaultPoolTag);
}

void __cdecl operator delete(void * aPtr) NOEXCEPT$TYPE
{
    if (nullptr == aPtr)
    {
        return;
    }

    CheckPoolTag(aPtr, DefaultPoolTag);
    return ExFreePoolWithTag(aPtr, DefaultPoolTag);
}

void * __cdecl operator new(size_t aSize, POOL_TYPE aPoolType) NOEXCEPT$TYPE
{
    if (0 == aSize)
    {
        aSize = 1;
    }

    return ExAllocatePoolWithTag(aPoolType, aSize, DefaultPoolTag);
}

void * __cdecl operator new(
    size_t aSize,
    POOL_TYPE aPoolType,
    unsigned long aTag) NOEXCEPT$TYPE
{
    if (0 == aSize)
    {
        aSize = 1;
    }

    return ExAllocatePoolWithTag(aPoolType, aSize, aTag);
}

void __cdecl operator delete(void * aPtr, unsigned long aTag) NOEXCEPT$TYPE
{
    if (nullptr == aPtr)
    {
        return;
    }

    CheckPoolTag(aPtr, aTag);
    return ExFreePoolWithTag(aPtr, aTag);
}

void * __cdecl operator new[](size_t aSize) NOEXCEPT$TYPE
{
    if (0 == aSize)
    {
        aSize = 1;
    }

    return ExAllocatePoolWithTag(DefaultPoolType, aSize, DefaultPoolTag);
}

void __cdecl operator delete[](void * aPtr) NOEXCEPT$TYPE
{
    if (nullptr == aPtr)
    {
        return;
    }

    CheckPoolTag(aPtr, DefaultPoolTag);
    return ExFreePoolWithTag(aPtr, DefaultPoolTag);
}

void * __cdecl operator new[](size_t aSize, POOL_TYPE aPoolType) NOEXCEPT$TYPE
{
    if (0 == aSize)
    {
        aSize = 1;
    }

    return ExAllocatePoolWithTag(aPoolType, aSize, DefaultPoolTag);
}

void * __cdecl operator new[](
    size_t aSize,
    POOL_TYPE aPoolType,
    unsigned long aTag) NOEXCEPT$TYPE
{
    if (0 == aSize)
    {
        aSize = 1;
    }

    return ExAllocatePoolWithTag(aPoolType, aSize, aTag);
}

void __cdecl operator delete[](void * aPtr, unsigned long aTag) NOEXCEPT$TYPE
{
    if (nullptr == aPtr)
    {
        return;
    }

    CheckPoolTag(aPtr, aTag);
    return ExFreePoolWithTag(aPtr, aTag);
}


//////////////////////////////////////////////////////////////////////////
// replaceable placement deallocation functions
//////////////////////////////////////////////////////////////////////////

void * __cdecl operator new(size_t /*aSize*/, void * aPtr) NOEXCEPT$TYPE
{
    return aPtr;
}

void * __cdecl operator new[](size_t /*aSize*/, void * aPtr) NOEXCEPT$TYPE
{
    return aPtr;
}

// void __cdecl operator delete(void * /*aPtr*/, void *) NOEXCEPT$TYPE
// {
// }
// 
// void __cdecl operator delete[](void * /*aPtr*/, void *) NOEXCEPT$TYPE
// {
// }


//////////////////////////////////////////////////////////////////////////
// sized class - specific deallocation functions
//////////////////////////////////////////////////////////////////////////

void __cdecl operator delete(void * aPtr, size_t /*aSize*/) NOEXCEPT$TYPE
{
    if (nullptr == aPtr)
    {
        return;
    }

    CheckPoolTag(aPtr, DefaultPoolTag);
    return ExFreePoolWithTag(aPtr, DefaultPoolTag);
}

void __cdecl operator delete[](void * aPtr, size_t /*aSize*/) NOEXCEPT$TYPE
{
    if (nullptr == aPtr)
    {
        return;
    }

    CheckPoolTag(aPtr, DefaultPoolTag);
    return ExFreePoolWithTag(aPtr, DefaultPoolTag);
}
