#pragma once
#ifndef Memory_$3C7D1B09_9AF6_4390_B588_420D096AE364
#define Memory_$3C7D1B09_9AF6_4390_B588_420D096AE364 1

#include "KTL.Type.h"

namespace ktl
{
    inline namespace memory
    {
        /// FUNCTION default_allocate & default_deallocate

        void *default_allocate(usize aSize, const usize aAlignment = alignof(usize));
        void default_deallocate(void *aPtr);

        /// FUNCTION Uitility

        void* memory_set(void* aDestBuffer, usize aDestBufferBytesCount, byte aValue);
        void * memory_zero(void * aBuffer, usize aBufferBytesCont);

        usize memory_copy(
            void* aDestBuffer, usize aDestBufferBytesCount,
            void* aSrcBuffer, usize aSrcBufferBytesCount);
    }

}

#endif
