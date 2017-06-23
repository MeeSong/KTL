#pragma once
#ifndef String_$4E7E4251_86BF_4E5E_B21C_670686B9683E
#define String_$4E7E4251_86BF_4E5E_B21C_670686B9683E 1

#include "KTL.Strings.h"
#include "KTL.Memory.Allocator.h"
#include "KTL.Iterators.NormalIterator.h"

namespace ktl
{
    inline namespace strings
    {
        template<typename _CharT>
        class basic_string
        {
        public:
            using value_type = typename decay<_CharT>::type;

            using allocator_type = allocator<value_type>;

            using pointer = value_type*;
            using const_pointer = const value_type*;
            using reference = value_type&;
            using const_reference = const value_type&;
            using size_type = usize;
            using difference_type = intptr;

            using iterator = normal_iterator<pointer, basic_string>;
            using const_iterator = normal_iterator<const_pointer, basic_string>;

            static const size_type npos = static_cast<size_type>(-1);

            allocator_type  m_alloc;
            pointer         m_data = nullptr;
            size_type       m_size = 0;
            size_type       m_capacity = 16;

        protected:

            pointer __allocate_buffer(size_type aCapacity)
            {
                pointer vPtr = m_alloc.allocate(aCapacity);
                if (nullptr == vPtr)
                {
                    return vPtr;
                }
                RtlSecureZeroMemory(vPtr, aCapacity);

                return vPtr;
            }

            pointer __allocate_buffer(size_type aCapacity, pointer aStr)
            {
                pointer vPtr = __allocate_buffer(aCapacity);
                if (nullptr == vPtr)
                {
                    return vPtr;
                }

                return vPtr;
            }

            pointer __allocate_buffer(size_type aCapacity, pointer aStr, size_type aStrCharCount)
            {

            }

        public:

            basic_string() = default;
            basic_string(const basic_string&) = delete;
            basic_string& operator=(const basic_string&) = delete;

            basic_string(nullptr_t) NOEXCEPT$TYPE
            { }

            ~basic_string() NOEXCEPT$TYPE
            { clear(); }

            bool construction(
                const typename allocator_type::allocator$type& aAlloc,
                const typename allocator_type::deallocator$type& aDealloc) NOEXCEPT$TYPE
            {
                if (empty())
                {
                    return m_alloc.construction(aAlloc, aDealloc);
                }
                return false;
            }

            bool construction(
                const typename allocator_type& aAlloc) NOEXCEPT$TYPE
            {
                if (empty())
                {
                    if (aAlloc.is_default()) return true;
                    return m_alloc.construction(aAlloc);
                }
                return false;
            }

            bool construction(
                typename allocator_type&& aAlloc) NOEXCEPT$TYPE
            {
                if (empty())
                {
                    if (aAlloc.is_default()) return true;
                    return m_alloc.construction(move(aAlloc));
                }
                return false;
            }

            bool construction(const basic_string& aRight) NOEXCEPT$TYPE
            {
                this->clear();
                if (!construction(aRight.m_alloc)) return false;


            }

            bool construction(basic_string&& aRight) NOEXCEPT$TYPE
            {
                this->clear();
                if (!construction(move(aRight.m_alloc))) return false;


            }

            size_type size() const NOEXCEPT$TYPE
            {
                return m_size;
            }

            void clear()
            {

            }
        };

    }

}

#endif
