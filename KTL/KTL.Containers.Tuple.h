#pragma once
#ifndef Tuple_$73B86AF7_AE69_47BF_9CDE_E6365B79363E
#define Tuple_$73B86AF7_AE69_47BF_9CDE_E6365B79363E 1

#include "KTL.Functional.h"

namespace ktl
{
    inline namespace containers
    {
        /// TEMPLATE CLASS _Tuple_implement

        template<usize _Index, typename... _Elements>
        struct _Tuple_implement;

        // Zero-element tuple implementation.This is the basis case for the 
        // inheritance recursion.
        template<usize _Index>
        struct _Tuple_implement<_Index>
        {
            using _This_type = _Tuple_implement<_Index>;

            constexpr bool equals(const _This_type&) const
            {
                return true;
            }

            constexpr bool less(const _This_type&) const
            {
                return false;
            }

            void swap(_This_type&) NOEXCEPT$TYPE
            {
            }
        };

        // Recursive tuple implementation.
        template<usize _Index, typename _Head, typename... _Tail>
        struct _Tuple_implement<_Index, _Head, _Tail...>
            : public _Tuple_implement<_Index + 1, _Tail...>
        {
            using _Inherited = _Tuple_implement<_Index + 1, _Tail...>;
            static constexpr usize ItemCount = 1 + sizeof...(_Tail);
            _Head m_Value;

            _Inherited& tail()
            {
                return *this;
            }

            const _Inherited& tail() const
            {
                return *this;
            }

            template<typename... _UElements>
            constexpr bool equals(
                const _Tuple_implement<_Index, _UElements...>& aRight) const
            {
                // test if *this == aRight
                static_assert(ItemCount == sizeof...(_UElements),
                    "comparing tuple to object with different size");

                return (m_Value == aRight.m_Value &&
                    tail().equals(aRight.tail()));
            }

            template<typename... _UElements>
            constexpr bool less(
                const _Tuple_implement<_Index, _UElements...>& aRight) const
            {
                static_assert(ItemCount == sizeof...(_UElements),
                    "comparing tuple to object with different size");

                return (m_Value < aRight.m_Value ||
                    (!(aRight.m_Value < m_Value) &&
                        tail().less(aRight.tail())));
            }

            void swap(_Tuple_implement& aRight)
            {
                swap_adl(m_Value, aRight.m_Value);
                tail().swap(aRight.tail());
            }

            _Tuple_implement()
                : _Inherited()
                , m_Value()
            {
            }

            explicit constexpr _Tuple_implement(
                typename add_lvalue_reference<typename remove_reference<const _Head>::type>::type aHead,
                typename add_lvalue_reference<typename remove_reference<const _Tail>::type>::type... aTail)
                : _Inherited(aTail...)
                , m_Value(aHead)
            { }

            explicit constexpr _Tuple_implement(
                typename add_rvalue_reference<typename remove_reference<const _Head>::type>::type aHead,
                typename add_rvalue_reference<typename remove_reference<const _Tail>::type>::type... aTail)
                : _Inherited(forward<
                    typename add_rvalue_reference<typename remove_reference<const _Tail>::type>::type>(aTail)...)
                , m_Value(forward<
                    typename add_rvalue_reference<typename remove_reference<const _Head>::type>::type>(aHead))
            { }

            template<typename... _UElements>
            constexpr _Tuple_implement(const _Tuple_implement<_Index, _UElements...>& aSource)
                : _Inherited(aSource.tail())
                , m_Value(aSource.m_Value)
            { }

            template<
                typename _UHead, typename... _UElements, 
                typename = typename enable_if<sizeof...(_Tail) == sizeof...(_UElements)>::type>
            constexpr _Tuple_implement(_Tuple_implement<_Index, _UHead, _UElements...>&& aSource)
                : _Inherited(forward<
                    _Tuple_implement<_Index + 1, _UElements...>>(aSource.tail()))
                , m_Value(forward<_UHead>(aSource.m_Value))
            { }

            constexpr _Tuple_implement(const _Tuple_implement& aSource)
                : _Inherited(aSource.tail())
                , m_Value(aSource.m_Value)
            { }

            constexpr _Tuple_implement(_Tuple_implement&& aSource)
                : _Inherited(forward<_Inherited>(aSource.tail()))
                , m_Value(forward<_Head>(aSource.m_Value))
            { }

            template<typename... _UElements>
            _Tuple_implement& operator=(
                const _Tuple_implement<_Index, _UElements...>& aSource)
            {
                m_Value = aSource.m_Value;
                tail() = aSource.tail();
                return *this;
            }

            _Tuple_implement& operator=(const _Tuple_implement& aSource)
            {
                m_Value = aSource.m_Value;
                tail() = aSource.tail();
                return *this;
            }

            template<
                typename _UHead, typename... _UElements,
                typename = typename enable_if<sizeof...(_Tail) == sizeof...(_UElements)>::type>
            _Tuple_implement& operator=(
                _Tuple_implement<_Index, _UHead, _UElements...>&& aSource)
            {
                using _UInherited = _Tuple_implement<_Index + 1, _UElements...>;
                m_Value = forward<_UHead>(aSource.m_Value);
                tail() = forward<_UInherited>(aSource.tail());
                return *this;
            }

            _Tuple_implement& operator=(_Tuple_implement&& aSource)
            {
                m_Value = forward<_Head>(aSource.m_Value);
                tail() = forward<_Inherited>(aSource.tail());
                return *this;
            }

            template<typename... _UElements>
            inline constexpr bool operator==(
                const _Tuple_implement<_Index, _UElements...>& aRight) const
            {
                return equals(aRight);
            }

            template<typename... _UElements>
            inline constexpr bool operator!=(
                const _Tuple_implement<_Index, _UElements...>& aRight) const
            {
                return (!(*this == aRight));
            }

            template<typename... _UElements>
            inline constexpr bool operator<(
                const _Tuple_implement<_Index, _UElements...>& aRight) const
            {
                return less(aRight);
            }

            template<typename... _UElements>
            inline constexpr bool operator>=(
                const _Tuple_implement<_Index, _UElements...>& aRight) const
            {
                return (!(*this < aRight));
            }

            template<typename... _UElements>
            inline constexpr bool operator>(
                const _Tuple_implement<_Index, _UElements...>& aRight) const
            {
                return (aRight < *this);
            }

            template<typename... _UElements>
            inline constexpr bool operator<=(
                const _Tuple_implement<_Index, _UElements...>& aRight) const
            {
                return (!(aRight < *this));
            }
        };


#define KTL$DefineTupleCompareOperator$Macro(aOperator) \
    template<typename... _UElements> \
    inline constexpr bool operator##aOperator##( \
        const tuple<_UElements...>& aRight) const \
    { \
        return _Inherited::operator##aOperator##(aRight); \
    }


        template<typename... _Elements>
        class tuple :
            public _Tuple_implement<0, _Elements...>
        {
            using _Inherited = _Tuple_implement<0, _Elements...>;

        public:

            void swap(tuple& aRight)
            {
                _Inherited::swap(aRight);
            }

            constexpr tuple()
                : _Inherited()
            { }

            explicit constexpr tuple(
                typename add_lvalue_reference<typename remove_reference<const _Elements>::type>::type... aElements)
                : _Inherited(aElements...)
            { }

            explicit constexpr tuple(
                typename add_rvalue_reference<typename remove_reference<const _Elements>::type>::type... aElements)
                : _Inherited(forward<
                    typename add_rvalue_reference<typename remove_reference<const _Elements>::type>::type>(aElements)...)
            { }

            template<typename... _UElements>
            constexpr tuple(const tuple<_UElements...>& aSource)
                : _Inherited(static_cast<const _Tuple_implement<0, _UElements...>&>(aSource))
            { }

            template<typename... _UElements>
            constexpr tuple(tuple<_UElements...>&& aSource)
                : _Inherited(forward<_Tuple_implement<0, _UElements...>>(aSource))
            { }

            constexpr tuple(const tuple& aSource)
                : _Inherited(aSource)
            { }

            constexpr tuple(tuple&& aSource)
                : _Inherited(move(aSource))
            { }

            template<typename... _UElements>
            tuple& operator=(const tuple<_UElements...>& aSource)
            {
                static_cast<_Inherited&>(*this) = aSource;
                return *this;
            }

            tuple& operator=(const tuple& aSource)
            {
                static_cast<_Inherited&>(*this) = aSource;
                return *this;
            }

            template<typename... _UElements>
            tuple& operator=(tuple<_UElements...>&& aSource)
            {
                static_cast<_Inherited&&>(*this) = move(aSource);

                return *this;
            }

            tuple& operator=(tuple&& aSource)
            {
                static_cast<_Inherited&&>(*this) = move(aSource);
                return *this;
            }

            KTL$DefineTupleCompareOperator$Macro(== )
            KTL$DefineTupleCompareOperator$Macro(!= )
            KTL$DefineTupleCompareOperator$Macro(< )
            KTL$DefineTupleCompareOperator$Macro(>= )
            KTL$DefineTupleCompareOperator$Macro(> )
            KTL$DefineTupleCompareOperator$Macro(<= )
        };


        template<>
        class tuple<> {};


#undef KTL$DefineTupleCompareOperator$Macro


        /// Gives the type of the ith element of a given tuple type.
        template<usize _Index, typename _Type>
        struct tuple_element;

        // Basis case for tuple_element: The first element is the one we're seeking.
        template<typename _Head, typename... _Tail>
        struct tuple_element<0, tuple<_Head, _Tail...>>
        {
            using type = _Head;
        };

        // Recursive case for tuple_element: strip off the first element in
        // the tuple and retrieve the (i-1)th element of the remaining tuple.
        template<usize _Index, typename _Head, typename... _Tail>
        struct tuple_element<_Index, tuple<_Head, _Tail...>>
            : tuple_element<_Index - 1, tuple<_Tail...>>
        {
        };


        /// Finds the size of a given tuple type.

        template<typename _Type>
        struct tuple_size;

        template<typename... _Elements>
        struct tuple_size<tuple<_Elements...>>
        {
            static constexpr usize value = sizeof...(_Elements);
        };

        template<typename... _Elements>
        const usize tuple_size<tuple<_Elements...>>::value;


        /// function tuple's get

        template<usize _Index, typename _Head, typename... _Tail>
        inline typename add_lvalue_reference<_Head>::type
            __tuple_get_helper(_Tuple_implement<_Index, _Head, _Tail...>& aTuple)
        {
            return aTuple.m_Value;
        }

        template<usize _Index, typename _Head, typename... _Tail>
        inline typename add_lvalue_reference<const _Head>::type
            __tuple_get_helper(const _Tuple_implement<_Index, _Head, _Tail...>& aTuple)
        {
            return aTuple.m_Value;
        }

        // Return a reference (const reference) to the ith element of a tuple.
        // Any const or non-const ref elements are returned with their original type.
        template<usize _Index, typename... _Elements>
        inline typename add_lvalue_reference<
            typename tuple_element<_Index, tuple<_Elements...>>::type>::type
            get(tuple<_Elements...>& aTuple)
        {
            return __tuple_get_helper<_Index>(aTuple);
        }

        template<usize _Index, typename... _Elements>
        inline constexpr typename add_lvalue_reference<
            const typename tuple_element<_Index, tuple<_Elements...>>::type>::type
            get(const tuple<_Elements...>& aTuple)
        {
            return __tuple_get_helper<_Index>(aTuple);
        }


        /// function tuple's make_tuple & tie & forward_as_tuple

        template<typename... _Elements>
        inline constexpr tuple<typename unrefwrap<_Elements>::type...>
            make_tuple(_Elements&&... aArguments)
        {
            return tuple<typename unrefwrap<_Elements>::type...>(
                forward<_Elements>(aArguments)...);
        }

        template<typename... _Elements>
        inline constexpr tuple<_Elements&...>
            tie(_Elements&... aArguments) NOEXCEPT$TYPE
        {
            return (tuple<_Elements&...>(aArguments...));
        }

        template<typename... _Elements>
        inline constexpr tuple<_Elements&&...>
            forward_as_tuple(_Elements&&... aArguments) NOEXCEPT$TYPE
        {
            return (tuple<_Elements&&...>(forward<_Elements>(aArguments)...));
        }


        /// function tuple's swap

        template<typename... _Elements>
        inline void swap(tuple<_Elements...>& aLeft, tuple<_Elements...>& aRight)
        {
            aLeft.swap(aRight);
        }
    }

}

#endif