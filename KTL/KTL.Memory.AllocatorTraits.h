#pragma once
#ifndef AllocatorTraits_$8823FC95_0535_4A5E_9BCF_A04ACBEECCA8
#define AllocatorTraits_$8823FC95_0535_4A5E_9BCF_A04ACBEECCA8 1

#include "KTL.Type.Uitility.h"
#include "KTL.Type.PointerTraits.h"

namespace ktl
{
    inline namespace memory
    {
        struct __allocator_traits_base
        {
            template <typename _Type, typename _Type2, typename = void>
            struct __rebind : __replace_first_arg<_Type, _Type2>
            { };

            template <typename _Type, typename _Type2>
            struct __rebind<_Type, _Type2, void_t<typename _Type::template rebind<_Type2>::other>>
            {
                using type = typename _Type::template rebind<_Type2>::other;
            };

        protected:

            template<typename _Type>
            using pointer_base = typename _Type::pointer;

            template<typename _Type>
            using const_pointer_base = typename _Type::const_pointer;

            template<typename _Type>
            using void_pointer_base = typename _Type::void_pointer;

            template<typename _Type>
            using const_void_pointer_base = typename _Type::const_void_pointer;

            template<typename _Type>
            using propagate_on_container_copy_assignment_base = typename _Type::propagate_on_container_copy_assignment;

            template<typename _Type>
            using propagate_on_container_move_assignment_base = typename _Type::propagate_on_container_move_assignment;

            template<typename _Type>
            using propagate_on_container_swap_base = typename _Type::propagate_on_container_swap;

            template<typename _Type>
            using is_always_equal_base = typename _Type::is_always_equal;
        };

        template<typename _Alloc, typename _Alloc2>
        using __allocator_traits_base_rebind = typename __allocator_traits_base::template __rebind<_Alloc, _Alloc2>::type;

        template<typename _Alloc>
        struct allocator_traits : __allocator_traits_base
        {
            using allocator_type = _Alloc;
            using value_type = typename _Alloc::value_type;
            using pointer = __detected_or_t<value_type*, pointer_base, _Alloc>;

        private:
            // Select _Func<_Alloc> or pointer_traits<pointer>::rebind<_Type>
            template<template<typename> class _Func, typename _Type, typename = void>
            struct _Ptr
            {
                using type = typename pointer_traits<pointer>::template rebind<_Type>;
            };

            template <template <typename> class _Func, typename _Type>
            struct _Ptr<_Func, _Type, void_t<_Func<_Alloc>>>
            {
                using type = _Func<_Alloc>;
            };

            // Select _Alloc2::difference_type or pointer_traits<_Ptr>::difference_type
            template <typename _Alloc2, typename _PtrT, typename = void>
            struct _Diff
            {
                using type = typename pointer_traits<_PtrT>::difference_type;
            };

            template <typename _Alloc2, typename _PtrT>
            struct _Diff<_Alloc2, _PtrT, void_t<typename _Alloc2::difference_type>>
            {
                using type = typename _Alloc2::difference_type;
            };

            // Select _Alloc2::size_type or make_unsigned<_DiffT>::type
            template <typename _Alloc2, typename _DiffT, typename = void>
            struct _Size : make_unsigned<_DiffT>
            { };

            template <typename _Alloc2, typename _DiffT>
            struct _Size<_Alloc2, _DiffT, void_t<typename _Alloc2::size_type>>
            {
                using type = typename _Alloc2::size_type;
            };

        public:

            using const_pointer = typename _Ptr<const_pointer_base, const value_type>::type;

            using void_pointer = typename _Ptr<void_pointer_base, void>::type;

            using const_void_pointer = typename _Ptr<const_void_pointer_base, const void>::type;

            using difference_type = typename _Diff<_Alloc, pointer>::type;

            using size_type = typename _Size<_Alloc, difference_type>::type;

            using propagate_on_container_copy_assignment = __detected_or_t<false_type, propagate_on_container_copy_assignment_base, _Alloc>;

            using propagate_on_container_move_assignment = __detected_or_t<false_type, propagate_on_container_move_assignment_base, _Alloc>;

            using propagate_on_container_swap = __detected_or_t<false_type, propagate_on_container_swap_base, _Alloc>;

            using is_always_equal = __detected_or_t<typename is_empty<_Alloc>::type, is_always_equal_base, _Alloc>;

            template <typename _Type>
            using rebind_alloc = __allocator_traits_base_rebind<_Alloc, _Type>;
            template <typename _Type>
            using rebind_traits = allocator_traits<rebind_alloc<_Type>>;

        private:

            template <typename _Alloc2>
            static auto
                __allocate_impl(_Alloc2 &aAlloc, size_type aSize, const_void_pointer aHint, int)
                -> decltype(aAlloc.allocate(aSize, aHint))
            {
                return aAlloc.allocate(aSize, aHint);
            }

            template <typename _Alloc2>
            static pointer
                __allocate_impl(_Alloc2 &aAlloc, size_type aSize, const_void_pointer, ...)
            {
                return aAlloc.allocate(aSize);
            }

            template <typename _Type, typename... _Args>
            struct __construct_helper
            {
                template <typename _Alloc2,
                    typename = decltype(declval<_Alloc2 *>()->construct(
                        declval<_Type *>(), declval<_Args>()...))>
                    static true_type __test(int);

                template <typename>
                static false_type __test(...);

                using type = decltype(__test<_Alloc>(0));
            };

            template <typename _Type, typename... _Args>
            using __has_construct = typename __construct_helper<_Type, _Args...>::type;

            template <typename _Type, typename... _Args>
            static __require<__has_construct<_Type, _Args...>> 
                __construct_impl(_Alloc &aAlloc, _Type *aObject, _Args &&... aArgs)
            {
                aAlloc.construct(aObject, forward<_Args>(aArgs)...);
            }

            template <typename _Type, typename... _Args>
            static __require<conjunction<negation<__has_construct<_Type, _Args...>>, is_constructible<_Type, _Args...>>>
                __construct_impl(_Alloc &, _Type *aObject, _Args &&... aArgs)
            {
                ::new ((void *)(aObject)) _Type(forward<_Args>(aArgs)...);
            }

            template <typename _Alloc2, typename _Type>
            static auto __destroy_impl(_Alloc2 &aAlloc, _Type *aObject, int)
                -> decltype(aAlloc.destroy(aObject))
            {
                aAlloc.destroy(aObject);
            }

            template <typename _Alloc2, typename _Type>
            static void __destroy_impl(_Alloc2 &, _Type *aObject, ...)
            {
                aObject->~_Type();
            }

            template <typename _Alloc2>
            static auto __max_size_impl(_Alloc2 &aAlloc, int)
                -> decltype(aAlloc.max_size())
            {
                return aAlloc.max_size();
            }

            template <typename _Alloc2>
            static size_type __max_size_impl(_Alloc2 &, ...)
            {
                return size_type(~(size_type(0))) / sizeof(value_type);
            }

            template <typename _Alloc2>
            static auto __select_impl(_Alloc2 &aNewAlloc, _Alloc2 &aSourceAlloc, int)
                -> decltype(aAlloc.select_on_container_copy_construction(aNewAlloc))
            {
                return aSourceAlloc.select_on_container_copy_construction(aNewAlloc);
            }

            template <typename _Alloc2>
            static bool __select_impl(_Alloc2 &aNewAlloc, _Alloc2 &aSourceAlloc, ...)
            {
                return aSourceAlloc.construction(aNewAlloc);
            }

        public:

            static bool is_default(_Alloc& aAlloc)
            {
                return aAlloc.is_default();
            }

            static bool construction(_Alloc& aLeft, const _Alloc& aRight)
            {
                return aLeft.construction(aRight);
            }

            static bool construction(_Alloc& aLeft, _Alloc&& aRight)
            {
                return aLeft.construction(move(aRight));
            }

            static pointer allocate(_Alloc& aAlloc, size_type aSize)
            {
                return aAlloc(aSize);
            }

            static pointer allocate(_Alloc& aAlloc, size_type aSize, const_void_pointer aHint)
            {
                return __allocate_impl(aAlloc, aSize, aHint, 0);
            }

            static void deallocate(_Alloc& aAlloc, pointer aObject, size_type aSize)
            {
                aAlloc.deallocate(aObject, aSize);
            }

            template<typename _Type, typename... _Args>
            static auto construct(_Alloc& aAlloc, _Type* aObject, _Args &&... aArgs)
                -> decltype(__construct_impl(aAlloc, aObject, forward<_Args>(aArgs)...))
            {
                return (__construct_impl(aAlloc, aObject, forward<_Args>(aArgs)...));
            }

            template <typename _Type>
            static void destroy(_Alloc& aAlloc, _Type *aObject)
            {
                __destroy_impl(aAlloc, aObject, 0);
            }

            static size_type max_size(const _Alloc& aAlloc) NOEXCEPT$TYPE
            {
                return __max_size_impl(aAlloc, 0);
            }

            static bool select_on_container_copy_construction(const _Alloc& aRight)
            {
                return __select_impl(aRight, 0);
            }
        };
    }

}

#endif
