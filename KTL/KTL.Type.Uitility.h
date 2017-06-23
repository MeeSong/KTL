#pragma once

#ifndef Uitility_$EF51064C_FCBB_41A1_BE51_EB28830823F8
#define Uitility_$EF51064C_FCBB_41A1_BE51_EB28830823F8 1

#include "KTL.Type.TypeTraits.h"

namespace ktl
{
    inline namespace uitility
    {
        /// TEMPLATE FUNCTION forward

        template<typename _Type>
        inline constexpr _Type&& forward(
            typename remove_reference<_Type>::type& aArguments) NOEXCEPT$TYPE
        {
            return (static_cast<_Type&&>(aArguments));
        }

        template<typename _Type>
        inline constexpr _Type&& forward(
            typename remove_reference<_Type>::type&& aArguments) NOEXCEPT$TYPE
        {
            static_assert(!is_lvalue_reference<_Type>::value,
                "template argument substituting _Tp is an lvalue reference type");
            return (static_cast<_Type&&>(aArguments));
        }

        /// TEMPLATE FUNCTION ktl::move

        template<typename _Type>
        inline constexpr typename remove_reference<_Type>::type&&
            move(_Type&& aArguments) NOEXCEPT$TYPE
        {
            return (static_cast<
                typename remove_reference<_Type>::type&&>(aArguments));
        }

        template<typename _Type>
        inline constexpr typename __if<!is_nothrow_move_constructible<_Type>::value
            && is_copy_constructible<_Type>::value, const _Type&, _Type&&>::type
            move_if_noexcept(_Type& aArguments) NOEXCEPT$TYPE
        {
            return (move(aArguments));
        }

        /// TEMPLATE FUNCTION addressof

        template<typename _Type>
        inline _Type* __addressof_helper(_Type& aValue, true_type) NOEXCEPT$TYPE
        {
            return (aValue);
        }

        template<typename _Type>
        inline _Type* __addressof_helper(_Type& aValue, false_type) NOEXCEPT$TYPE
        {
            return (reinterpret_cast<_Type*>(
                &const_cast<i8&>(
                    reinterpret_cast<const volatile i8&>(aValue))));
        }

        template<typename _Type>
        inline _Type* addressof(_Type& aValue) NOEXCEPT$TYPE
        {
            return (__addressof_helper(aValue, is_function<_Type>()));
        }

        /// FUNCTION TEMPLATE unfancy_pointer

        template<typename _PtrType>
        inline auto unfancy_pointer(_PtrType aPointer)
        {
            // converts from a fancy pointer to a plain pointer
            return (addressof(*aPointer));
        }

        template<typename _Type>
        inline _Type* unfancy_pointer(_Type aPointer)
        {
            // do nothing for plain pointers
            return (aPointer);
        }

        usize aligned_size(usize aSize, usize aAlignment = alignof(max_align_t));

        /// FUNCTION TEMPLATE swap

        template <typename _Tp>
        struct __is_swappable;

        template <typename _Tp>
        struct __is_nothrow_swappable;

        template<
            typename _Type,
            class = typename enable_if<is_move_constructible<_Type>::value
            && is_move_assignable<_Type>::value>::type>
            inline void swap(_Type& aLeft, _Type& aRight);

        template<
            typename _Type,
            usize _Size,
            class = typename enable_if<__is_swappable<_Type>::value>::type>
            inline void swap(_Type(&aLeft)[_Size], _Type(&aRight)[_Size]);

        struct __swappable_details
        {
            struct __do_is_swappable_impl
            {
                template<
                    typename _Type,
                    typename = decltype(swap(declval<_Type&>(), declval<_Type&>()))>
                    static true_type __test(i32);

                template<typename>
                static false_type __test(...);
            };

            struct __do_is_nothrow_swappable_impl
            {
                template<typename _Type>
                static bool_constant<
                    noexcept(swap(declval<_Type&>(), declval<_Type&>()))>
                    __test(i32);

                template<typename>
                static false_type __test(...);
            };
        };

        template<typename _Type>
        struct __is_swappable_impl
            : public __swappable_details::__do_is_swappable_impl
        {
            using type = decltype(__test<_Type>(0));
        };

        template<typename _Type>
        struct __is_swappable
            : public __is_swappable_impl<_Type>::type
        { };

        template<typename _Type, class>
        inline void swap(_Type& aLeft, _Type& aRight)
        {
            _Type vTemp = move(aLeft);
            aLeft = move(aRight);
            aRight = move(vTemp);
        }

        template<typename _ForwardIter1, typename _ForwardIter2>
        inline void iter_swap(_ForwardIter1 aLeft, _ForwardIter2 aRight)
        {
            swap(*aLeft, *aRight);
        }

        template< typename _Type, usize _Size, class>
        inline void swap(_Type(&aLeft)[_Size], _Type(&aRight)[_Size])
        {
            if (&aLeft != &aRight)
            {
                _Type *vLeftFirst = aLeft;
                _Type *vLeftLast = vLeftFirst + _Size;
                _Type *vRightFirst = aRight;

                for (; vLeftFirst != vLeftLast; ++vLeftFirst, ++vRightFirst)
                {
                    iter_swap(vLeftFirst, vRightFirst);
                }
            }
        }

        template<typename _Type>
        inline void swap_adl(_Type& aLeft, _Type& aRight)
        {
            swap(aLeft, aRight);
        }

        /// TEMPLATE STRUCT integer_sequence

        template<typename _Type, _Type... _Value>
        struct integer_sequence
        {
            // sequence of integer parameters

            static_assert(is_integral<_Type>::value,
                "integer_sequence<T, I...> requires T to be an integral type.");

            using type = integer_sequence<_Type, _Value...>;
            using value_type = _Type;

            static constexpr usize size() NOEXCEPT$TYPE
            {
                return (sizeof...(_Value));
            }
        };

        /// ALIAS TEMPLATE make_integer_sequence

        template<
            bool _Negative,
            bool _Zero,
            typename _IntegralCon, // integral_constant
            typename _IntegerSeq>  // integer_sequence
            struct __make_sequence
        {
            // explodes gracefully below 0
            static_assert(!_Negative,
                "make_integer_sequence<T, N> requires N to be non-negative.");
        };

        template<typename _Type, _Type... _Value>
        struct __make_sequence<false, true,
            integral_constant<_Type, 0>,
            integer_sequence<_Type, _Value...>>
            : public integer_sequence<_Type, _Value...>
        {
            // ends recursion at 0
        };

        template<typename _Type, _Type _Index, _Type... _Value>
        struct __make_sequence<false, false,
            integral_constant<_Type, _Index>,
            integer_sequence<_Type, _Value...>>
            : __make_sequence<false, _Index == 1,
            integral_constant<_Type, _Index - 1>,
            integer_sequence<_Type, _Index - 1, _Value...>>
        {
            // counts down to 0
        };

        template<typename _Type, _Type _Size>
        using make_integer_sequence =
            typename __make_sequence< (_Size < 0), (_Size == 0),
            integral_constant<_Type, _Size>, integer_sequence<_Type>>::type;

        template<usize... _Value>
        using index_sequence = integer_sequence<usize, _Value...>;

        template<usize _Size>
        using make_index_sequence = make_integer_sequence<usize, _Size>;

        template<typename... _Types>
        using index_sequence_for = make_index_sequence<sizeof...(_Types)>;
    }

}

#endif