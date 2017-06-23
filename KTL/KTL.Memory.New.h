#pragma once

#ifndef New_$910C2682_3929_4387_9BE4_E556DE90DABA
#define New_$910C2682_3929_4387_9BE4_E556DE90DABA 1

#include "KTL.Type.h"
#include <ntifs.h>

using size_t = ktl::usize;

// replaceable usual deallocation functions
void* __cdecl operator new(size_t aSize) NOEXCEPT$TYPE;
void __cdecl operator delete (void *aPtr) NOEXCEPT$TYPE;

void* __cdecl operator new (size_t aSize, POOL_TYPE aPoolType) NOEXCEPT$TYPE;
void* __cdecl operator new (size_t aSize, POOL_TYPE aPoolType, unsigned long aTag) NOEXCEPT$TYPE;
void __cdecl operator delete (void *aPtr, unsigned long aTag) NOEXCEPT$TYPE;

void* __cdecl operator new[](size_t aSize) NOEXCEPT$TYPE;
void  __cdecl operator delete[](void* aPtr) NOEXCEPT$TYPE;

void* __cdecl operator new[](size_t aSize, POOL_TYPE aPoolType) NOEXCEPT$TYPE;
void* __cdecl operator new[](size_t aSize, POOL_TYPE aPoolType, unsigned long aTag) NOEXCEPT$TYPE;
void __cdecl operator delete[](void *aPtr, unsigned long aTag) NOEXCEPT$TYPE;

// replaceable placement deallocation functions
void* __cdecl operator new  (size_t aSize, void* aPtr) NOEXCEPT$TYPE;
void* __cdecl operator new[](size_t aSize, void* aPtr) NOEXCEPT$TYPE;

// T::~T()
// void __cdecl operator delete (void* aPtr, void*) NOEXCEPT$TYPE;
// void __cdecl operator delete[](void* aPtr, void*) NOEXCEPT$TYPE;

// sized class - specific deallocation functions
void __cdecl operator delete (void* aPtr, size_t aSize) NOEXCEPT$TYPE;
void __cdecl operator delete[](void* aPtr, size_t aSize) NOEXCEPT$TYPE;


#endif