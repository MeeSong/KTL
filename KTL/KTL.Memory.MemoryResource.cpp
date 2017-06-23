#include "KTL.Memory.MemoryResource.h"
#include "KTL.Memory.h"

namespace ktl
{
    inline namespace memory
    {
        /// TEMPLATE CLASS memory_resource

        void * memory_resource::do_allocate(usize aSize, const usize aAlignment)
        {
            return default_allocate(aSize, aAlignment);
        }

        void memory_resource::do_deallocate(void *aPtr, usize /*aSize*/, usize /*aAlignment*/)
        {
            return default_deallocate(aPtr);
        }

        bool memory_resource::do_is_equal(const memory_resource& /*aRight*/) const NOEXCEPT$TYPE
        {
            return true;
        }

        void * memory_resource::allocate(usize aSize, usize aAlignment)
        {
            return do_allocate(aSize, aAlignment);
        }

        void memory_resource::deallocate(void *aPtr, usize aSize, usize aAlignment)
        {
            return do_deallocate(aPtr, aSize, aAlignment);
        }

        bool memory_resource::is_equal(const memory_resource& aRight) const NOEXCEPT$TYPE
        {
            return do_is_equal(aRight);
        }

        inline bool memory_resource::operator==(const memory_resource& aRight) NOEXCEPT$TYPE
        {
            return ((this == &aRight) || (is_equal(aRight)));
        }

        inline bool memory_resource::operator!=(const memory_resource& aRight) NOEXCEPT$TYPE
        {
            return !(operator==(aRight));
        }

    }

}
