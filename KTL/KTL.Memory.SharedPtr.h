#pragma once
#ifndef SharedPtr_$72F84D72_64F8_4FF7_83FA_B94A71598F2F
#define SharedPtr_$72F84D72_64F8_4FF7_83FA_B94A71598F2F 1

#include "KTL.Memory.SmartPointerBase.h"

namespace ktl
{
    inline namespace memory
    {
        template<typename _Type>
        class weak_ptr;

        /// TEMPLATE CLASS shared_ptr

        struct __smart_ptr_make_shared_tag { };

        template<typename _Type>
        class shared_ptr
        {
        public:
            using element_type = typename remove_extent<_Type>::type;
            using element_ptr = element_type *;
            using _Base = __smart_ptr_base<element_ptr>;

        private:
            template<typename... _Args>
            using _Constructible = typename enable_if<is_constructible<element_ptr, _Args...>::value>::type;

        public:
            constexpr shared_ptr()
            { }

            explicit constexpr shared_ptr(nullptr_t) NOEXCEPT$TYPE
            { }
            
            ~shared_ptr()
            {
                release();
            }

            shared_ptr(const shared_ptr& aRight)
                : m_Pointer(aRight.m_Pointer)
            {
                reference();
            }

            shared_ptr(shared_ptr&& aRight)
                : m_Pointer(move(aRight.m_Pointer))
            {
                aRight.m_Pointer = nullptr;
            }

            template<typename _UType, typename = _Constructible<const weak_ptr<_UType>::element_ptr &>>
            explicit shared_ptr(const weak_ptr<_UType> &aRight)
            {
                if (!aRight.expired())
                {
                    m_Pointer = aRight.m_Pointer;
                    reference();
                }
            }

            shared_ptr& operator=(const shared_ptr& aRight)
            {
                if (m_Pointer != aRight.m_Pointer)
                {
                    shared_ptr(aRight).swap(*this);
                }

                return *this;
            }

            shared_ptr& operator=(shared_ptr&& aRight)
            {
                if (m_Pointer != aRight.m_Pointer)
                {
                    shared_ptr(move(aRight)).swap(*this);
                }

                return *this;
            }

            inline bool is_attach() const
            {
                return static_cast<bool>(m_Pointer != nullptr);
            }
            
            bool attach(element_ptr aPointer)
            {
                return attach(aPointer, [](element_ptr aPtr) -> void { delete aPtr; });
            }

            template<typename _Deleter>
            bool attach(element_ptr aPointer, _Deleter aDeleter)
            {
                auto vPointer = new _Base;
                if (nullptr == vPointer)
                {
                    return false;
                }

                if (false == vPointer->attach(aPointer, aDeleter))
                {
                    delete vPointer;
                    aDeleter(aPointer);

                    return false;
                }

                release();
                m_Pointer = vPointer;
                return true;
            }

            void swap(shared_ptr& aRight)
            {
                swap_adl(m_Pointer, aRight.m_Pointer);
            }

            bool reset()
            {
                shared_ptr().swap(*this);
                return true;
            }

            bool reset(element_ptr aPointer)
            {
                return attach(aPointer);
            }

            template<typename _Deleter>
            bool reset(element_ptr aPointer, _Deleter aDeleter)
            {
                return attach(aPointer, aDeleter);
            }

            element_ptr get() const
            {
                if (operator bool())
                {
                    return m_Pointer->get_pointer();
                }

                return nullptr;
            }

            bool owner_before(const shared_ptr& aRight) const
            {
                return static_cast<bool>(m_Pointer < aRight.m_Pointer);
            }

            inline u32 use_count() const
            {
                return is_attach() ? m_Pointer->use_count() : 0;
            }

            inline bool unique() const
            {
                return is_attach() ? static_cast<bool>(1 == m_Pointer->use_count()) : false;
            }

            typename add_lvalue_reference<element_type>::type
                operator*() const
            {
                return (*this->get());
            }

            element_ptr operator->() const
            {
                return (this->get());
            }

            explicit operator bool() const
            {
                return static_cast<bool>(is_attach() && m_Pointer->use_count() > 0);
            }

            friend inline bool operator==(const shared_ptr& aLeft,
                const shared_ptr& aRight)
            {
                return static_cast<bool>(aLeft.get() == aRight.get());
            }

            friend inline bool operator<(const shared_ptr& aLeft,
                const shared_ptr& aRight)
            {
                return static_cast<bool>(aLeft.get() < aRight.get());
            }

        private:
            inline void reference()
            {
                if (is_attach())
                {
                    m_Pointer->reference_use();
                }
            }

            inline void release()
            {
                if (is_attach())
                {
                    m_Pointer->release_use();
                }
            }

            template<typename... _Args>
            bool attach(__smart_ptr_make_shared_tag, _Args&&... aArgs)
            {
                auto vObject = new element_type{ forward<_Args>(aArgs)... };
                if (vObject == nullptr)
                {
                    return false;
                }

                return attach(vObject);
            }

            template<typename _UType, typename... _Args>
            friend bool make_shared(shared_ptr<_UType> &aPtr, _Args&&... aArgs);

            friend class weak_ptr<_Type>;

        private:
            _Base *m_Pointer = nullptr;
        };

        /// TEMPLATE FUNCTION make_shared
        template<typename _Type, typename... _Args>
        inline bool make_shared(shared_ptr<_Type> &aPtr, _Args&&... aArgs)
        {
            return aPtr.attach(__smart_ptr_make_shared_tag(), forward<_Args>(aArgs)...);
        }
    }

}

#endif
