#pragma once
#ifndef Iterator_$A4A200D0_2218_4FF2_BE2C_C3FABA88DAAD
#define Iterator_$A4A200D0_2218_4FF2_BE2C_C3FABA88DAAD 1

#include "KTL.Iterators.IteratorTraits.h"

namespace ktl
{
    inline namespace iterators
    {
        /// TEMPLATE FUNCTION distance

        template<typename _InputIter>
        inline typename iterator_traits<_InputIter>::difference_type
            __distance_impl(_InputIter aFirst, _InputIter aLast, input_iterator_tag)
        {
            typename iterator_traits<_InputIter>::difference_type vCount = 0;
            while (aFirst != aLast)
            {
                ++aFirst;
                ++vCount;
            }
            return vCount;
        }

        template<typename _RandomAccessIter>
        inline typename iterator_traits<_RandomAccessIter>::difference_type
            __distance_impl(_RandomAccessIter aFirst, _RandomAccessIter aLast, random_access_iterator_tag)
        {
            return (aLast - aFirst);
        }

        template<typename>
        struct list_iterator;

        template<typename>
        struct list_const_iterator;

        template<typename _Type>
        inline intptr __distance_impl(list_iterator<_Type> aFirst, list_iterator<_Type> aLast, input_iterator_tag aTag)
        {
            using const_iterator = typename list_const_iterator<_Type>;
            return __distance_impl(const_iterator(aFirst), const_iterator(aLast), aTag);
        }

        template<typename _Type>
        inline intptr __distance_impl(list_const_iterator<_Type> aFirst, list_const_iterator<_Type> aLast, input_iterator_tag aTag)
        {
            using head_node_type = list_node<usize>;
            list_const_iterator<_Type> vBeyond = aLast;
            ++vBeyond;

            if (aFirst == vBeyond)
            {
                return *(static_cast<const head_node_type*>(aLast.m_node)->get_pointer());
            }

            intptr vCount = 0;
            while (aFirst != aLast)
            {
                ++aFirst;
                ++vCount;
            }
            return vCount;
        }

        template<typename _InputIter>
        inline typename iterator_traits<_InputIter>::difference_type
            distance(_InputIter aFirst, _InputIter aLast)
        {
            return __distance_impl(aFirst, aLast, __get_iterator_category(aFirst));
        }

        /// TEMPLATE FUNCTION advance

        template<typename _InputIter, typename _Distance>
        inline void __advance_impl(_InputIter& aIter, _Distance aDistance, input_iterator_tag)
        {
            if (aDistance >= 0)
            {
                while (aDistance--)
                {
                    ++aIter;
                }
            }
        }

        template<typename _BidirectionalIter, typename _Distance>
        inline void __advance_impl(_BidirectionalIter& aIter, _Distance aDistance, bidirectional_iterator_tag)
        {
            if (aDistance > 0) while (aDistance--) ++aIter;
            else while (aDistance++) --aIter;
        }

        template<typename _RandomAccessIter, typename _Distance>
        inline void __advance_impl(_RandomAccessIter& aIter, _Distance aDistance, random_access_iterator_tag)
        {
            aIter += aDistance;
        }

        template<typename _InputIter, typename _Distance>
        inline void advance(_InputIter& aIter, _Distance aDistance)
        {
            typename iterator_traits<_InputIter>::difference_type vDistance = aDistance;
            __advance_impl(aIter, vDistance, __get_iterator_category(aIter));
        }

        /// TEMPLATE FUNCTION next & prev

        template<typename _ForwardIter>
        inline _ForwardIter next(
            _ForwardIter aIter, 
            typename iterator_traits<_ForwardIter>::difference_type aDistance = 1)
        {
            advance(aIter, aDistance);
            return aIter;
        }

        template<typename _BidrectionalIter>
        inline _BidrectionalIter prev(
            _BidrectionalIter aIter,
            typename iterator_traits<_BidrectionalIter>::difference_type aDistance = 1)
        {
            advance(aIter, -aDistance);
            return aIter;
        }

        /// TEMPLATE FUNCTIONS begin & end

        template<typename _Container>
        auto inline begin(_Container& aContainer) -> decltype(aContainer.begin())
        {
            return (aContainer.begin());
        }

        template<typename _Container>
        auto inline begin(const _Container& aContainer) -> decltype(aContainer.begin())
        {
            return (aContainer.begin());
        }

        template<typename _Container>
        auto inline end(_Container& aContainer) -> decltype(aContainer.end())
        {
            return (aContainer.end());
        }

        template<typename _Container>
        auto inline end(const _Container& aContainer) -> decltype(aContainer.end())
        {
            return (aContainer.end());
        }

        template<typename _Type, usize _Size>
        inline constexpr _Type* begin(_Type (&aArray)[_Size]) NOEXCEPT$TYPE
        {
            return (aArray);
        }

        template<typename _Type, usize _Size>
        inline constexpr _Type* end(_Type(&aArray)[_Size]) NOEXCEPT$TYPE
        {
            return (aArray + _Size);
        }

        /// TEMPLATE FUNCTIONS cbegin & cend

        template<typename _Container>
        constexpr auto inline cbegin(const _Container& aContainer) -> decltype(begin(aContainer))
        {
            return begin(aContainer);
        }

        template<typename _Container>
        constexpr auto inline cend(const _Container& aContainer)-> decltype(end(aContainer))
        {
            return end(aContainer);
        }

    }

}

#endif
