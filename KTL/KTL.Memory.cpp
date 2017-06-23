#include "KTL.Memory.h"
#include "KTL.Type.Uitility.h"
#include "KTL.Algorithm.h"

#include <intrin.h>

namespace ktl
{
    inline namespace memory
    {
        /// FUNCTION default_allocate & default_deallocate

        void *default_allocate(usize aSize, const usize aAlignment)
        {
            usize vNewSize = aligned_size(aSize, aAlignment);
            return (void*)(new byte[vNewSize]);
        }

        void default_deallocate(void *aPtr)
        {
            delete[](byte*)(aPtr);
        }

        void * memory_set(void * aDestBuffer, usize aDestBufferBytesCount, byte aValue)
        {
            if (nullptr == aDestBuffer
                || 0 == aDestBufferBytesCount)
            {
                return aDestBuffer;
            }

            __stosb((ktl::byte*)aDestBuffer, aValue, aDestBufferBytesCount);
            return aDestBuffer;
        }

        void * memory_zero(void * aBuffer, usize aBufferBytesCont)
        {
            return memory_set(aBuffer, aBufferBytesCont, 0);
        }

        usize memory_copy(
            void * aDestBuffer, usize aDestBufferBytesCount, 
            void * aSrcBuffer, usize aSrcBufferBytesCount)
        {
            if (nullptr == aDestBuffer
                || nullptr == aSrcBuffer
                || 0 == aDestBufferBytesCount
                || 0 == aSrcBufferBytesCount
                || aDestBuffer == aSrcBuffer)
            {
                return 0;
            }

            usize vCopyBytesCount = min(aDestBufferBytesCount, aSrcBufferBytesCount);
            
            __movsb((ktl::byte*)aDestBuffer, (const ktl::byte*)aSrcBuffer, vCopyBytesCount);
            return vCopyBytesCount;
        }
    }

}
