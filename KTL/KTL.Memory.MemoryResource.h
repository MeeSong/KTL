#pragma once
#ifndef MemoryResource_$C507C683_02A6_45DD_9542_257520B06073
#define MemoryResource_$C507C683_02A6_45DD_9542_257520B06073 1

#include "KTL.Memory.AllocatorTraits.h"

namespace ktl
{
    inline namespace memory
    {
        /// TEMPLATE CLASS memory_resource

        class memory_resource
        {
        protected:
            static constexpr usize m_MaxAlign = alignof(max_align_t);

            virtual void *do_allocate(usize aSize, const usize aAlignment);
            virtual void do_deallocate(void *aPtr, usize /*aSize*/, usize /*aAlignment*/);
            virtual bool do_is_equal(const memory_resource& aRight) const NOEXCEPT$TYPE;

        public:

            void *allocate(usize aSize, usize aAlignment = m_MaxAlign);
            void deallocate(void *aPtr, usize aSize, usize aAlignment = m_MaxAlign);
            bool is_equal(const memory_resource& aRight) const NOEXCEPT$TYPE;
            inline bool operator==(const memory_resource& aRight) NOEXCEPT$TYPE;
            inline bool operator!=(const memory_resource& aRight) NOEXCEPT$TYPE;
        };

        /// TEMPLATE CLASS polymorphic_allocator

        // ...

    }

}

#endif
