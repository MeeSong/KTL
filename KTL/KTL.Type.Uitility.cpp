#include "KTL.Type.Uitility.h"

namespace ktl
{
    inline namespace uitility
    {
        usize aligned_size(usize aSize, usize aAlignment)
        {
            usize vNewAlignment = ((aAlignment != 0) && !(aAlignment & (aAlignment - 1))) ? 
                aAlignment : alignof(max_align_t);
            usize vNewSize = ((aSize - 1) | (vNewAlignment - 1)) + 1;

            return vNewSize;
        }
    }

}
