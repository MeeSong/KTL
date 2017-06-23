#pragma once
#ifndef ListNode_$89124ED5_6C94_4326_A78C_B42E9952D04F
#define ListNode_$89124ED5_6C94_4326_A78C_B42E9952D04F 1

#include "KTL.Type.h"
#include "KTL.Type.AlignedBuffer.h"

namespace ktl
{
    inline namespace containers
    {
        /// STRUCT _List_node_base

        struct _List_node_base
        {
            _List_node_base* m_prev = this;
            _List_node_base* m_next = this;

            static void swap(_List_node_base& aLeft, _List_node_base& aRight) NOEXCEPT$TYPE;
            bool empty() const NOEXCEPT$TYPE;
            void transfer(_List_node_base *const aFirst, _List_node_base *const aLast) NOEXCEPT$TYPE;
            void reverse() NOEXCEPT$TYPE;
            void hook(_List_node_base *const aPosition) NOEXCEPT$TYPE;
            void unhook() NOEXCEPT$TYPE;
        };

        template<typename _Type>
        struct list_node : _List_node_base
        {
            aligned_membuf<_Type> m_storage;
            _Type* get_pointer()
            {
                return m_storage.get_pointer();
            }

            const _Type * get_pointer() const
            {
                return m_storage.get_pointer();
            }
        };

    }

}

#endif
