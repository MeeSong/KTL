#pragma once
#ifndef NormalIterator_$E1999050_DDE1_4E39_903B_F6DE247E441B
#define NormalIterator_$E1999050_DDE1_4E39_903B_F6DE247E441B 1

#include "KTL.Iterators.IteratorTraits.h"

namespace ktl
{
    inline namespace iterators
    {
        template<typename _Iterator, typename _Container>
        class normal_iterator
        {
        protected:
            using traits_type = iterator_traits<_Iterator>;

            _Iterator m_iterator;

        public:

            using iterator_type = _Iterator;
            using iterator_category = typename traits_type::iterator_category;
            using value_type = typename traits_type::value_type;
            using difference_type = typename traits_type::difference_type;
            using reference = typename traits_type::reference;
            using pointer = typename traits_type::pointer;

            constexpr normal_iterator() NOEXCEPT$TYPE
                : m_iterator(_Iterator()) 
            { }

            explicit normal_iterator(const _Iterator& aIter) NOEXCEPT$TYPE
                : m_iterator(aIter)
            { }

            template<typename _Iter>
            normal_iterator(
                const normal_iterator<
                _Iter, 
                typename enable_if<
                    is_same<_Iter, typename _Container::pointer>::value, _Container>::type>& aIter) NOEXCEPT$TYPE
                : m_iterator(aIter)
            { }

            // Forward iterator requirements
            
            reference operator*() const NOEXCEPT$TYPE
            {
                return *m_iterator;
            }

            pointer operator->() const NOEXCEPT$TYPE
            {
                return m_iterator;
            }

            normal_iterator& operator++() NOEXCEPT$TYPE
            {
                ++m_iterator;
                return *this;
            }

            normal_iterator operator++(int) NOEXCEPT$TYPE
            {
                return normal_iterator(m_iterator++);
            }

            // Bidirectional iterator requirements

            normal_iterator& operator--() NOEXCEPT$TYPE
            {
                --m_iterator;
                return *this;
            }

            normal_iterator operator--(int) NOEXCEPT$TYPE
            {
                return normal_iterator(m_iterator--);
            }

            // Random access iterator requirements

            reference operator[](difference_type aIndex) const NOEXCEPT$TYPE
            {
                return m_iterator[aIndex];
            }

            normal_iterator& operator+=(difference_type aIndex) NOEXCEPT$TYPE
            {
                m_iterator += aIndex;
                return *this;
            }

            normal_iterator operator+(difference_type aIndex) const NOEXCEPT$TYPE
            {
                return normal_iterator(m_iterator + aIndex);
            }

            normal_iterator& operator-=(difference_type aIndex) NOEXCEPT$TYPE
            {
                m_iterator -= aIndex;
                return *this;
            }

            normal_iterator operator-(difference_type aIndex) const NOEXCEPT$TYPE
            {
                return normal_iterator(m_iterator - aIndex);
            }

            const _Iterator& base() const NOEXCEPT$TYPE
            {
                return m_iterator;
            }
        };

        // Forward iterator requirements
        
        template<typename _LeftIter, typename _RightIter, typename _Container>
        inline bool operator==(
            const normal_iterator<_LeftIter, _Container>& aLeft,
            const normal_iterator<_RightIter, _Container>& aRight) NOEXCEPT$TYPE
        {
            return (aLeft.base() == aRight.base());
        }

        template<typename _Iterator, typename _Container>
        inline bool operator==(
            const normal_iterator<_Iterator, _Container>& aLeft,
            const normal_iterator<_Iterator, _Container>& aRight) NOEXCEPT$TYPE
        {
            return (aLeft.base() == aRight.base());
        }

        template<typename _LeftIter, typename _RightIter, typename _Container>
        inline bool operator!=(
            const normal_iterator<_LeftIter, _Container>& aLeft,
            const normal_iterator<_RightIter, _Container>& aRight) NOEXCEPT$TYPE
        {
            return !(operator==(aLeft, aRight));
        }

        template<typename _Iterator, typename _Container>
        inline bool operator!=(
            const normal_iterator<_Iterator, _Container>& aLeft,
            const normal_iterator<_Iterator, _Container>& aRight) NOEXCEPT$TYPE
        {
            return !(operator==(aLeft, aRight));
        }

        // Random access iterator requirements

        template<typename _LeftIter, typename _RightIter, typename _Container>
        inline bool operator<(
            const normal_iterator<_LeftIter, _Container>& aLeft,
            const normal_iterator<_RightIter, _Container>& aRight) NOEXCEPT$TYPE
        {
            return (aLeft.base() < aRight.base());
        }

        template<typename _Iterator, typename _Container>
        inline bool operator<(
            const normal_iterator<_Iterator, _Container>& aLeft,
            const normal_iterator<_Iterator, _Container>& aRight) NOEXCEPT$TYPE
        {
            return (aLeft.base() < aRight.base());
        }

        template<typename _LeftIter, typename _RightIter, typename _Container>
        inline bool operator>(
            const normal_iterator<_LeftIter, _Container>& aLeft,
            const normal_iterator<_RightIter, _Container>& aRight) NOEXCEPT$TYPE
        {
            return (aLeft.base() > aRight.base());
        }

        template<typename _Iterator, typename _Container>
        inline bool operator>(
            const normal_iterator<_Iterator, _Container>& aLeft,
            const normal_iterator<_Iterator, _Container>& aRight) NOEXCEPT$TYPE
        {
            return (aLeft.base() > aRight.base());
        }

        template<typename _LeftIter, typename _RightIter, typename _Container>
        inline bool operator<=(
            const normal_iterator<_LeftIter, _Container>& aLeft,
            const normal_iterator<_RightIter, _Container>& aRight) NOEXCEPT$TYPE
        {
            return !(operator>(aLeft, aRight));
        }

        template<typename _Iterator, typename _Container>
        inline bool operator<=(
            const normal_iterator<_Iterator, _Container>& aLeft,
            const normal_iterator<_Iterator, _Container>& aRight) NOEXCEPT$TYPE
        {
            return !(operator>(aLeft, aRight));
        }

        template<typename _LeftIter, typename _RightIter, typename _Container>
        inline bool operator>=(
            const normal_iterator<_LeftIter, _Container>& aLeft,
            const normal_iterator<_RightIter, _Container>& aRight) NOEXCEPT$TYPE
        {
            return !(operator<(aLeft, aRight));
        }

        template<typename _Iterator, typename _Container>
        inline bool operator>=(
            const normal_iterator<_Iterator, _Container>& aLeft,
            const normal_iterator<_Iterator, _Container>& aRight) NOEXCEPT$TYPE
        {
            return !(operator<(aLeft, aRight));
        }
    }

}

#endif
