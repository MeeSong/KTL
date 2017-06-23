#include "KTL.Memory.New.h"

static constexpr ktl::u32 DefaultPoolTag = 'KNew';
static constexpr POOL_TYPE DefaultPoolType = NonPagedPoolNx;


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
    else if (size_t(-1) == aSize)
    {
        return nullptr;
    }

    return ExAllocatePoolWithTag(DefaultPoolType, aSize, DefaultPoolTag);
}

void __cdecl operator delete(void * aPtr) NOEXCEPT$TYPE
{
    if (nullptr == aPtr)
    {
        return;
    }

    return ExFreePoolWithTag(aPtr, DefaultPoolTag);
}

void * __cdecl operator new(size_t aSize, POOL_TYPE aPoolType) NOEXCEPT$TYPE
{
    if (0 == aSize)
    {
        aSize = 1;
    }
    else if (size_t(-1) == aSize)
    {
        return nullptr;
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
    else if (size_t(-1) == aSize)
    {
        return nullptr;
    }

    return ExAllocatePoolWithTag(aPoolType, aSize, aTag);
}

void __cdecl operator delete(void * aPtr, unsigned long aTag) NOEXCEPT$TYPE
{
    if (nullptr == aPtr)
    {
        return;
    }

    return ExFreePoolWithTag(aPtr, aTag);
}

void * __cdecl operator new[](size_t aSize) NOEXCEPT$TYPE
{
    if (0 == aSize)
    {
        aSize = 1;
    }
    else if (size_t(-1) == aSize)
    {
        return nullptr;
    }

    return ExAllocatePoolWithTag(DefaultPoolType, aSize, DefaultPoolTag);
}

void __cdecl operator delete[](void * aPtr) NOEXCEPT$TYPE
{
    if (nullptr == aPtr)
    {
        return;
    }

    return ExFreePoolWithTag(aPtr, DefaultPoolTag);
}

void * __cdecl operator new[](size_t aSize, POOL_TYPE aPoolType) NOEXCEPT$TYPE
{
    if (0 == aSize)
    {
        aSize = 1;
    }
    else if (size_t(-1) == aSize)
    {
        return nullptr;
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
    else if (size_t(-1) == aSize)
    {
        return nullptr;
    }

    return ExAllocatePoolWithTag(aPoolType, aSize, aTag);
}

void __cdecl operator delete[](void * aPtr, unsigned long aTag) NOEXCEPT$TYPE
{
    if (nullptr == aPtr)
    {
        return;
    }

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

    return ExFreePoolWithTag(aPtr, DefaultPoolTag);
}

void __cdecl operator delete[](void * aPtr, size_t /*aSize*/) NOEXCEPT$TYPE
{
    if (nullptr == aPtr)
    {
        return;
    }

    return ExFreePoolWithTag(aPtr, DefaultPoolTag);
}
