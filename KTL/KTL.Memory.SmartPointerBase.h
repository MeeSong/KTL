#pragma once
#ifndef SmartPointerBase_$EA30D895_2FDC_464F_A947_9BD1F70811CB
#define SmartPointerBase_$EA30D895_2FDC_464F_A947_9BD1F70811CB 1

#include <intrin.h>

#include "KTL.Functional.Function.h"

namespace ktl
{
    inline namespace memory
    {
        /// TEMPLATE CLASS __smart_ptr_counted_base

        class __smart_ptr_counted_base
        {
        public:
            __smart_ptr_counted_base()
            { }

            virtual ~__smart_ptr_counted_base()
            { }

            virtual void dispose() = 0;

            virtual void destroy()
            {
                delete this;
            }

            void reference_use()
            {
                // increment use count
                _InterlockedIncrement(
                    reinterpret_cast<volatile long *>(&m_UsedCount));
            }

            bool reference_use_lock()
            {
                if (0 == _InterlockedExchangeAdd(
                    reinterpret_cast<volatile long *>(&m_UsedCount), 1))
                {
                    m_UsedCount = 0;
                    return false;
                }
                return true;
            }

            void release_use()
            {
                // decrement use count
                if (0 == static_cast<u32>(_InterlockedDecrement(
                    reinterpret_cast<volatile long *>(&m_UsedCount))))
                {
                    dispose();
                    release_weak();
                }
            }

            void reference_weak()
            {
                // increment use count
                _InterlockedIncrement(
                    reinterpret_cast<volatile long *>(&m_WeakCount));
            }

            void release_weak()
            {
                // decrement use count
                if (0 == static_cast<u32>(_InterlockedDecrement(
                    reinterpret_cast<volatile long *>(&m_WeakCount))))
                {
                    destroy();
                }
            }

            u32 use_count() const
            {
                return const_cast<const volatile u32&>(m_UsedCount);
            }

        private:
            __smart_ptr_counted_base(const __smart_ptr_counted_base&) = delete;
            __smart_ptr_counted_base& operator=(const __smart_ptr_counted_base&) = delete;

            u32 m_UsedCount = 1;  // #shared
            u32 m_WeakCount = 1;  // #weak + (#shared != 0)
        };

        /// TEMPLATE CLASS __smart_ptr_base

        template<typename _Ptr>
        class __smart_ptr_base
            : public __smart_ptr_counted_base
        {
        public:
            using _Deleter = function<void(_Ptr)>;

            __smart_ptr_base()
            { }

            inline bool is_valid() const
            {
                return static_cast<bool>(m_Pointer && m_Deleter);
            }

            template<typename _Del>
            bool attach(_Ptr aPointer, _Del aDeleter)
            {
                if (nullptr == aPointer)
                {
                    return false;
                }

                if (!m_Deleter.attach(aDeleter))
                {
                    return false;
                }

                m_Pointer = aPointer;
                return true;
            }

            _Ptr get_pointer() const
            {
                return m_Pointer;
            }

            virtual void dispose()
            {
                if (is_valid())
                {
                    m_Deleter(m_Pointer);
                    m_Pointer = nullptr;
                }
            }

        private:
            __smart_ptr_base(const __smart_ptr_base&) = delete;
            __smart_ptr_base& operator=(const __smart_ptr_base&) = delete;

            _Ptr m_Pointer = nullptr;
            _Deleter m_Deleter = nullptr;
        };

    }

}

#endif
