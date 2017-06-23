#pragma once
#ifndef IteratorTraits_$877CCAB1_C495_473B_B000_2A727C4E6B9E
#define IteratorTraits_$877CCAB1_C495_473B_B000_2A727C4E6B9E 1

#include "KTL.Type.TypeTraits.h"

namespace ktl
{
    inline namespace iterators
    {
        /// STRUCT Iterator tags 

        struct input_iterator_tag { };
        struct output_iterator_tag { };
        struct forward_iterator_tag : input_iterator_tag { };
        struct bidirectional_iterator_tag : forward_iterator_tag { };
        struct random_access_iterator_tag : bidirectional_iterator_tag { };
        
        template <
            typename _Category,
            typename _Type,
            typename _Distance = intptr,
            typename _Pointer = _Type *,
            typename _Reference = _Type &
        >
            struct iterator
        {
            /// One of the @link iterator_tags tag types@endlink.
            using iterator_category = _Category;
            /// The type "pointed to" by the iterator.
            using value_type = _Type;
            /// Distance between iterators is represented as this type.
            using difference_type = _Distance;
            /// This type represents a pointer-to-value_type.
            using pointer = _Pointer;
            /// This type represents a reference-to-value_type.
            using reference = _Reference;
        };

        /// TEMPLATE CLASS iterator_traits

        template<typename _Iterator, typename = void_t<>>
        struct __iterator_traits_impl
        { };

        template<typename _Iterator>
        struct __iterator_traits_impl<
            _Iterator,
            void_t<typename _Iterator::iterator_category,
                   typename _Iterator::value_type,
                   typename _Iterator::difference_type,
                   typename _Iterator::pointer,
                   typename _Iterator::reference>
        >
        {
            using iterator_category = typename _Iterator::iterator_category;
            using value_type        = typename _Iterator::value_type;
            using difference_type   = typename _Iterator::difference_type;
            using pointer           = typename _Iterator::pointer;
            using reference         = typename _Iterator::reference;
        };

        template<typename _Iterator>
        struct iterator_traits : __iterator_traits_impl<_Iterator>
        { };

        /// Partial specialization for pointer types.

        template <typename _Type>
        struct iterator_traits<_Type *>
        {
            using iterator_category = random_access_iterator_tag;
            using value_type        = _Type;
            using difference_type   = intptr;
            using pointer           = value_type *;
            using reference         = value_type&;
        };

        /// Partial specialization for const pointer types.

        template <typename _Type>
        struct iterator_traits<const _Type *>
        {
            using iterator_category = random_access_iterator_tag;
            using value_type = _Type;
            using difference_type = intptr;
            using pointer = const value_type *;
            using reference = const value_type&;
        };

        template<typename _Iterator>
        inline typename iterator_traits<_Iterator>::iterator_category
            __get_iterator_category(const _Iterator &)
        {
            return typename iterator_traits<_Iterator>::iterator_category();
        }

        template<typename _Iterator>
        using _RequireInputIterator = typename enable_if<is_convertible<typename iterator_traits<_Iterator>::iterator_category, input_iterator_tag>::value>::type;

    }

}

#endif
