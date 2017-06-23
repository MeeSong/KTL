#pragma once
#ifndef AlignedBuffer_$9CC6CE6E_EB1C_4D55_8D9D_F34824038C8B
#define AlignedBuffer_$9CC6CE6E_EB1C_4D55_8D9D_F34824038C8B 1

#include "KTL.Type.TypeTraits.h"

namespace ktl
{
    inline namespace uitility
    {
        template<typename _Type>
        struct aligned_membuf
        {
            using value_type = _Type;
            using pointer = value_type *;
            using const_pointer = const value_type *;

            struct _UnusedClass
            {
                value_type m_unused;
            };

            alignas(alignof(_UnusedClass)) byte m_storage[sizeof(value_type)] = { 0 };

            aligned_membuf() = default;
            aligned_membuf(nullptr_t) { }

            void* get_address() NOEXCEPT$TYPE
            {
                return static_cast<void*>(&m_storage);
            }

            const void* get_address() const NOEXCEPT$TYPE
            {
                return static_cast<const void*>(&m_storage);
            }

            pointer get_pointer() NOEXCEPT$TYPE
            {
                return static_cast<pointer>(get_address());
            }

            const_pointer get_pointer() const NOEXCEPT$TYPE
            {
                return static_cast<const_pointer>(get_address());
            }
        };

        template<typename _Type>
        struct aligned_buffer
            : aligned_storage<sizeof(_Type), alignment_of<_Type>::value>
        {
            using value_type = _Type;
            using pointer = value_type *;
            using const_pointer = const value_type *;

            typename aligned_storage<sizeof(_Type), alignment_of<_Type>::value>::type m_storage;
            
            aligned_buffer() = default;
            aligned_buffer(nullptr_t) { }

            void* get_address() NOEXCEPT$TYPE
            {
                return static_cast<void*>(&m_storage);
            }

            const void* get_address() NOEXCEPT$TYPE
            {
                return static_cast<const void*>(&m_storage);
            }

            pointer get_pointer() NOEXCEPT$TYPE
            {
                return static_cast<pointer>(get_address());
            }

            const_pointer get_pointer() NOEXCEPT$TYPE
            {
                return static_cast<const_pointer>(get_address());
            }
        };
    }
}

#endif
