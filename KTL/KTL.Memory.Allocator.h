#pragma once
#ifndef Allocator_$A8F38098_1E72_4B5E_9D8D_0D1FCFF0DC93
#define Allocator_$A8F38098_1E72_4B5E_9D8D_0D1FCFF0DC93 1

#include "KTL.Memory.h"
#include "KTL.Functional.Function.h"

namespace ktl
{
    inline namespace memory
    {
        template<typename _Type>
        class allocator;

        template<>
        class allocator<void>
        {
        public:
            using allocator$type = function<void*(usize aSize)>;
            using deallocator$type = function<void(void* aPtr)>;

        private:
            allocator$type      m_alloc = nullptr;
            deallocator$type    m_dealloc = nullptr;

        public:

            using size_type = usize;
            using difference_type = intptr;
            using value_type = void;
            using pointer = value_type *;
            using const_pointer = const value_type *;

            template<typename _Other>
            struct rebind
            {
                using other = allocator<_Other>;
            };

            using propagate_on_container_move_assignment = true_type;
            using is_always_equal = true_type;

            template<typename _Type, typename... _Args>
            void construct(_Type *aObject, _Args&&... aArgs)
            {
                new (void*)(aObject) _Type(forward<_Args>(aArgs)...);
            }

            template<typename _Type>
            void destroy(_Type *aObject)
            {
                aObject->~_Type();
            }
        };

        template<typename _Type>
        class allocator
        {
        public:
            using allocator$type    = function<void*(usize aSize)>;
            using deallocator$type  = function<void(void* aPtr)>;

            allocator$type      m_alloc     = nullptr;
            deallocator$type    m_dealloc   = nullptr;

        public:

            using size_type         = usize;
            using difference_type   = intptr;
            using value_type        = _Type;
            using pointer           = value_type *;
            using const_pointer     = const value_type *;
            using reference         = value_type&;
            using const_reference   = const value_type&;

            template<typename _Other>
            struct rebind
            {
                using other = allocator<_Other>;
            };

            using propagate_on_container_copy_assignment = true_type;
            using propagate_on_container_move_assignment = true_type;
            using propagate_on_container_swap = true_type;
            using is_always_equal = false_type;

            allocator() NOEXCEPT$TYPE
            { }

            ~allocator() NOEXCEPT$TYPE
            { }

            allocator(const allocator& aRight) = delete;
            allocator& operator=(const allocator& aRight) = delete;

            bool is_default() const
            {
                if (!bool(m_alloc) && !bool(m_dealloc))
                {
                    return true;
                }
                return false;
            }

            bool is_equal(const allocator& aRight) const
            {
                if (this->is_default() && aRight.is_default())
                {
                    return true;
                }

                return false;
            }

            template<typename _Type2>
            bool is_equal(const allocator<_Type2>& aRight) const
            {
                if (this->is_default() && aRight.is_default())
                {
                    return true;
                }

                return false;
            }

            bool set_allocator_deallocator(const allocator$type &aAlloc, const deallocator$type &aDealloc)
            {
                bool vResult = false;

                for (;;)
                {
                    if (aAlloc && !m_alloc.attach(aAlloc))
                    {
                        break;
                    }

                    if (aDealloc && !m_dealloc.attach(aDealloc))
                    {
                        m_alloc.reset();
                        break;
                    }

                    vResult = true;
                    break;
                }

                return vResult;
            }

            void reset_allocator_deallocator()
            {
                m_alloc.reset();
                m_dealloc.reset();
            }

            bool construction(const allocator$type& aAlloc, const deallocator$type &aDealloc) NOEXCEPT$TYPE
            {
                return set_allocator_deallocator(aAlloc, aDealloc);
            }

            bool construction(const allocator& aRight) NOEXCEPT$TYPE
            {
                bool vResult = false;

                for (;;)
                {
                    if (aRight.is_default())
                    {
                        reset_allocator_deallocator();
                        vResult = true;
                        break;
                    }

                    if (!set_allocator_deallocator(aRight.m_alloc, aRight.m_dealloc))
                    {
                        break;
                    }

                    vResult = true;
                    break;
                }

                return vResult;
            }

            bool construction(allocator&& aRight) NOEXCEPT$TYPE
            {
                bool vResult = false;

                for (;;)
                {
                    if (aRight.is_default())
                    {
                        reset_allocator_deallocator();
                        vResult = true;
                        break;
                    }

                    if (aAlloc && !m_alloc.attach(move(aAlloc)))
                    {
                        break
                    }

                    if (aDealloc && !m_dealloc.attach(move(aDealloc)))
                    {
                        m_alloc.reset();
                        break;
                    }

                    vResult = true;
                    break;
                }

                return vResult;
            }

            template<typename _Type2>
            bool construction(const allocator<_Type2>& aRight) NOEXCEPT$TYPE
            {
                bool vResult = false;

                for (;;)
                {
                    if (aRight.is_default())
                    {
                        reset_allocator_deallocator();
                        vResult = true;
                        break;
                    }

                    if (!set_allocator_deallocator(aRight.m_alloc, aRight.m_dealloc))
                    {
                        break;
                    }

                    vResult = true;
                    break;
                }

                return vResult;
            }

            pointer address(reference a) const NOEXCEPT$TYPE
            {
                return addressof(a);
            }

            const_pointer address(const_reference a) const NOEXCEPT$TYPE
            {
                return addressof(a);
            }

            pointer allocate(usize aCount)
            {
                if (aCount > max_size())
                {
                    return nullptr;
                }

                if (m_alloc)
                {
                    return pointer(m_alloc(aCount * sizeof(_Type)));
                }

                return pointer(default_allocate(aCount * sizeof(_Type)));
            }

            void deallocate(pointer aPtr, usize /*aCount*/)
            {
                if (m_dealloc)
                {
                    return m_dealloc(aPtr);
                }

                return default_deallocate(aPtr);
            }

            void swap(allocator& aRight)
            {
                if (is_default() && aRight.is_default()) return;

                ktl::swap(m_alloc, aRight.m_alloc);
                ktl::swap(m_dealloc, aRight.m_dealloc);
            }

            size_type max_size() const NOEXCEPT$TYPE
            {
                return (size_type(~((size_type)0)) / sizeof(_Type));
            }

            template<typename _Type2, typename... _Args>
            void construct(_Type2 * aObject, _Args &&... aArgs)
            {
                new ((void*)(aObject)) _Type2(forward<_Args>(aArgs)...);
            }

            template<typename _Type2>
            void destroy(_Type2 *aObject)
            {
                aObject;
                (aObject)->~_Type2();
            }
        };

        template<typename _Type>
        inline bool operator==(const allocator<_Type>& aLeft, const allocator<_Type>& aRight)
        {
            return aLeft.is_equal(aRight);
        }

        template<typename _Type>
        inline bool operator!=(const allocator<_Type>& aLeft, const allocator<_Type>& aRight)
        {
            return !(operator==(aLeft, aRight));
        }

        template<typename _Type1, typename _Type2>
        inline bool operator==(const allocator<_Type1>& aLeft, const allocator<_Type2>& aRight)
        {
            return aLeft.is_equal(aRight);
        }

        template<typename _Type1, typename _Type2>
        inline bool operator!=(const allocator<_Type1>& aLeft, const allocator<_Type2>& aRight)
        {
            return !(operator==(aLeft, aRight));
        }
    }

}

#endif
