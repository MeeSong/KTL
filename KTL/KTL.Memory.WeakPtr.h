#pragma once
#ifndef WeakPtr_$6B9E0CFA_6746_4015_B8C2_E6BFE5A2CF35
#define WeakPtr_$6B9E0CFA_6746_4015_B8C2_E6BFE5A2CF35 1

#include "KTL.Memory.h"

namespace ktl
{
    inline namespace memory
    {
        template<typename _Type>
        class shared_ptr;

        /// TEMPLATE CLASS weak_ptr

        template<typename _Type>
        class weak_ptr
        {
        public:
            using element_type = typename remove_extent<_Type>::type;
            using element_ptr = element_type *;
            using _Base = __smart_ptr_base<element_ptr>;

        private:
            template<typename... _Args>
            using _Constructible = typename enable_if<is_constructible<element_ptr, _Args...>::value>::type;

        public:
            constexpr weak_ptr()
            { }

            weak_ptr(const weak_ptr& aRight)
                : m_Pointer(aRight.m_Pointer)
            {
                reference();
            }

            /*template<typename _UType, typename = _Constructible<const weak_ptr<_UType>::element_ptr &>>
            weak_ptr(const weak_ptr<_UType>& aRight)
                : m_Pointer(aRight.m_Pointer)
            {
                reference();
            }*/

            template<typename _UType, typename = _Constructible<const weak_ptr<_UType>::element_ptr &>>
            weak_ptr(const shared_ptr<_UType>& aRight)
                : m_Pointer(aRight.m_Pointer)
            {
                reference();
            }

            weak_ptr(weak_ptr&& aRight)
                : m_Pointer(move(aRight.m_Pointer))
            {
                aRight.m_Pointer = nullptr;
            }

            /*template<typename _UType, typename = _Constructible<const weak_ptr<_UType>::element_ptr &&>>
            weak_ptr(weak_ptr<_UType>&& aRight)
                : m_Pointer(move(aRight.m_Pointer))
            {
                aRight.m_Pointer = nullptr;
            }*/

            ~weak_ptr()
            {
                release();
            }

            weak_ptr& operator=(const weak_ptr& aRight)
            {
                weak_ptr(aRight).swap(*this);
                return *this;
            }

            /*template<typename _UType, typename = _Constructible<const weak_ptr<_UType>::element_ptr &>>
            weak_ptr& operator=(const weak_ptr<_UType>& aRight)
            {
                weak_ptr(aRight).swap(*this);
                return *this;
            }*/

            template<typename _UType, typename = _Constructible<const weak_ptr<_UType>::element_ptr>&>
            weak_ptr& operator=(const shared_ptr<_UType>& aRight)
            {
                weak_ptr(aRight).swap(*this);
                return *this;
            }

            weak_ptr& operator=(weak_ptr&& aRight)
            {
                weak_ptr(move(aRight)).swap(*this);
                return *this;
            }

            /*template<typename _UType, typename = _Constructible<const weak_ptr<_UType>::element_ptr &&>>
            weak_ptr& operator=(weak_ptr<_UType>&& aRight)
            {
                weak_ptr(move(aRight)).swap(*this);
                return *this;
            }*/

            inline bool is_attach() const
            {
                return static_cast<bool>(m_Pointer != nullptr);
            }

            void swap(weak_ptr& aRight)
            {
                swap_adl(m_Pointer, aRight.m_Pointer);
            }

            void reset()
            {
                weak_ptr().swap(*this);
            }

            bool expired() const
            {
                if (is_attach())
                {
                    return static_cast<bool>(m_Pointer->use_count() == 0);
                }

                return false;
            }

            bool owner_before(const weak_ptr& aRight) const
            {
                return static_cast<bool>(m_Pointer < aRight.m_Pointer);
            }

            inline u32 use_count() const
            {
                return is_attach() ? m_Pointer->use_count() : 0;
            }

            shared_ptr<_Type> lock() const
            {
                return expired() ? shared_ptr<_Type>() : shared_ptr<_Type>(*this);
            }

        private:
            inline void reference()
            {
                if (is_attach())
                {
                    m_Pointer->reference_weak();
                }
            }

            inline void release()
            {
                if (is_attach())
                {
                    m_Pointer->release_weak();
                }
            }

            friend class shared_ptr<_Type>;

        private:
            _Base *m_Pointer = nullptr;
        };

    }

}

#endif