#pragma once
#ifndef ListIterator_$7971E907_CA08_4031_BE80_D21408BC8297
#define ListIterator_$7971E907_CA08_4031_BE80_D21408BC8297 1

#include "KTL.Containers.ListNode.h"

namespace ktl
{
    inline namespace iterators
    {
        /// TEMPLATE CLASS list_iterator

        template<typename _Type>
        struct list_iterator
        {
            using node_type = list_node<_Type>;

            using iterator_category = bidirectional_iterator_tag;
            using difference_type = intptr;
            using value_type = _Type;
            using pointer = value_type *;
            using reference = value_type&;

            _List_node_base *m_node = nullptr;

            list_iterator() NOEXCEPT$TYPE
            { }

            explicit list_iterator(_List_node_base *aNode) NOEXCEPT$TYPE
                : m_node(aNode)
            { }

            list_iterator __const_cast() const NOEXCEPT$TYPE
            {
                return *this;
            }

            reference operator*() const NOEXCEPT$TYPE
            {
                return *static_cast<node_type*>(m_node)->get_pointer();
            }

            pointer operator->() const NOEXCEPT$TYPE
            {
                return static_cast<node_type*>(m_node)->get_pointer();
            }

            list_iterator & operator++() NOEXCEPT$TYPE
            {
                m_node = m_node->m_next;
                return *this;
            }

            list_iterator operator++(int) NOEXCEPT$TYPE
            {
                list_iterator vTemp = *this;
                m_node = m_node->m_next;
                return vTemp;
            }

            list_iterator & operator--() NOEXCEPT$TYPE
            {
                m_node = m_node->m_prev;
                return *this;
            }

            list_iterator operator--(int) NOEXCEPT$TYPE
            {
                list_iterator vTemp = *this;
                m_node = m_node->m_prev;
                return vTemp;
            }

            bool operator==(const list_iterator& aRight) const NOEXCEPT$TYPE
            {
                return (m_node == aRight.m_node);
            }

            bool operator!=(const list_iterator& aRight) const NOEXCEPT$TYPE
            {
                return !(operator==(aRight));
            }
        };

        template<typename _Type>
        struct list_const_iterator
        {
            using node_type = const list_node<_Type>;
            using iterator = list_iterator<_Type>;

            using iterator_category = bidirectional_iterator_tag;
            using difference_type = intptr;
            using value_type = _Type;
            using pointer = const value_type*;
            using reference = const value_type&;

            const _List_node_base* m_node = nullptr;

            list_const_iterator() NOEXCEPT$TYPE
            { }

            explicit list_const_iterator(const _List_node_base* aNode) NOEXCEPT$TYPE
                : m_node(aNode)
            { }

            list_const_iterator(const iterator & aIter) NOEXCEPT$TYPE
                : m_node(aIter.m_node)
            { }

            iterator __const_cast() const NOEXCEPT$TYPE
            {
                return iterator(const_cast<_List_node_base*>(m_node));
            }

            reference operator*() const NOEXCEPT$TYPE
            {
                return *static_cast<node_type*>(m_node)->get_pointer();
            }

            pointer operator->() const NOEXCEPT$TYPE
            {
                return static_cast<node_type*>(m_node)->get_pointer();
            }

            list_const_iterator & operator++() NOEXCEPT$TYPE
            {
                m_node = m_node->m_next;
                return *this;
            }

            list_const_iterator operator++(int) NOEXCEPT$TYPE
            {
                list_const_iterator vTemp = *this;
                m_node = m_node->m_next;
                return vTemp;
            }

            list_const_iterator & operator--() NOEXCEPT$TYPE
            {
                m_node = m_node->m_prev;
                return *this;
            }

            list_const_iterator operator--(int) NOEXCEPT$TYPE
            {
                list_const_iterator vTemp = *this;
                m_node = m_node->m_prev;
                return vTemp;
            }

            bool operator==(const list_const_iterator& aRight) const NOEXCEPT$TYPE
            {
                return (m_node == aRight.m_node);
            }

            bool operator!=(const list_const_iterator& aRight) const NOEXCEPT$TYPE
            {
                return !(operator==(aRight));
            }
        };

        template<typename _Node>
        inline bool operator==(
            const list_iterator<_Node>& aLeft,
            const list_const_iterator<_Node>& aRight) NOEXCEPT$TYPE
        {
            return (aLeft.m_node == aRight.m_node);
        }

        template<typename _Node>
        inline bool operator!=(
            const list_iterator<_Node>& aLeft,
            const list_const_iterator<_Node>& aRight) NOEXCEPT$TYPE
        {
            return !(operator==(aLeft, aRight));
        }

    }

}

#endif
