#pragma once
#ifndef List_$AF845A76_7611_43C3_B4CE_32365806EF32
#define List_$AF845A76_7611_43C3_B4CE_32365806EF32 1

#include "KTL.Memory.Allocator.h"

#include "KTL.Iterators.h"
#include "KTL.Iterators.IteratorTraits.h"
#include "KTL.Iterators.ListIterator.h"

namespace ktl
{
    inline namespace containers
    {
        /// TEMPLATE CLASS list

        template<typename _Type>
        class list
        {
        public:
            using value_type = _Type;

            using node_type         = list_node<value_type>;
            using allocator_type    = typename allocator<value_type>::template rebind<node_type>::other;

            using pointer           = typename allocator<value_type>::pointer;
            using const_pointer     = typename allocator<value_type>::const_pointer;
            using reference         = typename allocator<value_type>::reference;
            using const_reference   = typename allocator<value_type>::const_reference;
            using size_type         = usize;
            using difference_type   = intptr;

            using iterator          = list_iterator<value_type>;
            using const_iterator    = list_const_iterator<value_type>;

            allocator_type   m_alloc;
            list_node<usize> m_head;

        protected:

            usize __get_size() const
            {
                return *(m_head.get_pointer());
            }

            void __set_size(usize aSize)
            {
                *(m_head.get_pointer()) = aSize;
            }

            void __inc_size(usize aSize)
            {
                *(m_head.get_pointer()) += aSize;
            }

            void __dec_size(usize aSize)
            {
                *(m_head.get_pointer()) -= aSize;
            }

            template<typename... _Args>
            node_type* __create_node(_Args... aArgs)
            {
                auto vPtr = m_alloc.allocate(1);
                if (nullptr == vPtr)
                {
                    return nullptr;
                }

                m_alloc.construct(vPtr);
                pointer vElement = vPtr->get_pointer();
                m_alloc.construct(vElement, forward<_Args>(aArgs)...);

                return vPtr;
            }

            void __delete_node(node_type* aNode)
            {
                if (aNode != nullptr)
                {
                    pointer vElement = aNode->get_pointer();
                    m_alloc.destroy(vElement);
                    m_alloc.destroy(aNode);
                    m_alloc.deallocate(aNode, 1);
                }
            }

            void __reset_head() NOEXCEPT$TYPE
            {
                m_head.m_next = &m_head;
                m_head.m_prev = &m_head;
                __set_size(0);
            }

            const_iterator __resize_position(size_type& aSize) const
            {
                const_iterator vIter;
                const size_type vSize = size();

                if (aSize < vSize)
                {
                    if (aSize <= vSize / 2)
                    {
                        vIter = cbegin();
                        advance(vIter, aSize);
                    }
                    else
                    {
                        vIter = cend();
                        intptr vEraseCount = vSize - aSize;
                        advance(vIter, -vEraseCount);
                    }

                    aSize = 0;
                    return vIter;
                }
                else
                {
                    vIter = cend();
                }

                aSize -= vSize;
                return vIter;
            }

            void __transfers(iterator aPosition, iterator aFirst, iterator aLast)
            {
                aPosition.m_node->transfer(aFirst.m_node, aLast.m_node);
            }

            template<typename _StrictWeakOrdering>
            iterator __sort_impl(
                iterator aFirst, iterator aLast, 
                _StrictWeakOrdering aComparator, size_type aSize)
            {
                if (aSize < 2) return aFirst;

                iterator vMiddle = next(aFirst, aSize / 2);
                aFirst = __sort_impl(aFirst, vMiddle, aComparator, aSize / 2);
                vMiddle = __sort_impl(vMiddle, aLast, aComparator, aSize - (aSize / 2));
                iterator vNewFirst = aFirst;

                for (bool vFirstLoop = true; ; vFirstLoop = false)
                {
                    if (aComparator(*vMiddle, *aFirst))
                    {
                        if (vFirstLoop) vNewFirst = vMiddle;
                        splice(aFirst, *this, vMiddle++);
                        if (vMiddle == aLast) return vNewFirst;
                    }
                    else
                    {
                        ++aFirst;
                        if (aFirst == vMiddle) return vNewFirst;
                    }
                }
            }

        public:

            list() = default;
            list(const list&) = delete;
            list& operator=(const list&) = delete;
            ~list() { clear(); }

            bool construction(
                const typename allocator_type::allocator$type& aAlloc,
                const typename allocator_type::deallocator$type& aDealloc) NOEXCEPT$TYPE
            {
                if (empty())
                {
                    return m_alloc.construction(aAlloc, aDealloc);
                }
                return false;
            }

            bool construction(
                const typename allocator_type& aAlloc) NOEXCEPT$TYPE
            {
                if (empty())
                {
                    if (aAlloc.is_default()) return true;
                    return m_alloc.construction(aAlloc);
                }
                return false;
            }

            bool construction(
                typename allocator_type&& aAlloc) NOEXCEPT$TYPE
            {
                if (empty())
                {
                    if (aAlloc.is_default()) return true;
                    return m_alloc.construction(move(aAlloc));
                }
                return false;
            }

            bool construction(const list& aRight) NOEXCEPT$TYPE
            {
                return construction(aRight.begin(), aRight.end(), aRight.m_alloc);
            }

            bool construction(list&& aRight) NOEXCEPT$TYPE
            {
                this->clear();
                if (!construction(move(list.m_alloc))) return false;

                auto *const vRightNode = addressof(aRight.m_head);
                if (false == vRightNode->empty())
                {
                    auto *const vLeftNode = addressof(m_head);
                    vLeftNode->m_next = vRightNode->m_next;
                    vLeftNode->m_prev = vRightNode->m_prev;
                    vLeftNode->m_next->m_prev = vLeftNode->m_prev->m_next = vLeftNode;
                    __set_size(aRight.__get_size());
                    aRight.__reset_head();
                }

                return true;
            }

            bool construction(
                size_type aCount, const value_type& aValue,
                const typename allocator_type& aAlloc = allocator_type()) NOEXCEPT$TYPE
            {
                this->clear();
                if (!construction(aAlloc)) return false;

                bool vIsException = false;
                size_type vCount = 0;
                for (; aCount--; ++vCount)
                {
                    if (end() == emplace_back(aValue))
                    {
                        vIsException = true;
                        break;
                    }
                }

                if (vIsException)
                {
                    for ( ; vCount--; )
                    {
                        pop_back();
                    }
                }

                return (!vIsException);
            }

            template<typename _InputIter, typename = _RequireInputIterator<_InputIter>>
            bool construction(
                _InputIter aFirst, _InputIter aLast, 
                const typename allocator_type& aAlloc = allocator_type()) NOEXCEPT$TYPE
            {
                this->clear();
                if (!construction(aAlloc)) return false;

                bool vIsException = false;
                size_type vCount = 0;
                for (; aFirst != aLast; ++aFirst, ++vCount)
                {
                    if (end() == emplace_back(*aFirst))
                    {
                        vIsException = true;
                        break;
                    }
                }

                if (vIsException)
                {
                    for (; vCount--; )
                    {
                        pop_back();
                    }
                }

                return (!vIsException);
            }

            size_type size() const
            {
                return *(m_head.get_pointer());
            }

            size_type max_size() const NOEXCEPT$TYPE
            {
                return m_alloc.max_size();
            }

            bool empty() const
            {
                return (m_head.empty());
            }

            void clear()
            {
                auto vCursor = m_head.m_next;
                while (vCursor != &m_head)
                {
                    node_type* vTemp = static_cast<node_type*>(vCursor);
                    vCursor = vTemp->m_next;

                    __delete_node(vTemp);
                }

                __reset_head();
            }

            size_type distance(
                const_iterator aFirst,
                const_iterator aLast) const
            {
                size_type vCount = 0;
                while (aFirst != aLast)
                {
                    ++aFirst;
                    ++vCount;
                }

                return vCount;
            }

            iterator begin() NOEXCEPT$TYPE
            {
                return iterator(m_head.m_next);
            }

            const_iterator begin() const NOEXCEPT$TYPE
            {
                return const_iterator(m_head.m_next);
            }

            const_iterator cbegin() const NOEXCEPT$TYPE
            {
                return const_iterator(m_head.m_next);
            }

            iterator end() NOEXCEPT$TYPE
            {
                return iterator(&m_head);
            }

            const_iterator end() const NOEXCEPT$TYPE
            {
                return const_iterator(&m_head);
            }

            const_iterator cend() const NOEXCEPT$TYPE
            {
                return const_iterator(&m_head);
            }

            reference front() NOEXCEPT$TYPE
            {
                return *begin();
            }

            const_reference front() const NOEXCEPT$TYPE
            {
                return *begin();
            }

            reference back() NOEXCEPT$TYPE
            {
                iterator vTemp = end();
                --vTemp;
                return *vTemp;
            }

            const_reference back() const NOEXCEPT$TYPE
            {
                const_iterator vTemp = end();
                --vTemp;
                return *vTemp;
            }

            bool assign(size_type aCount, const value_type &aValue)
            {
                iterator vFirst = begin();
                iterator vLast = end();

                for (; vFirst != vLast && aCount > 0; ++vFirst, --aCount)
                {
                    *vFirst = aValue;
                }

                if (aCount > 0) 
                { 
                    if (end() == insert(vLast, aCount, aValue))
                    {
                        return false;
                    }
                }
                else
                {
                    erase(vFirst, vLast);
                }

                return true;
            }

            template<typename _InputIter, typename = _RequireInputIterator<_InputIter>>
            bool assign(_InputIter aFirst, _InputIter aLast)
            {
                iterator vFirst = begin();
                iterator vLast = end();

                for (; vFirst != vLast && aFirst != aLast; ++vFirst, ++aFirst)
                {
                    *vFirst = *aFirst;
                }

                if (aFirst == aLast)
                {
                    erase(vFirst, vLast);
                }
                else 
                { 
                    if (end() == insert(vLast, aFirst, aLast))
                    {
                        return false;
                    }
                }

                return true;
            }

            bool resize(size_type aSize)
            {
                const_iterator vIter = __resize_position(aSize);
                if (aSize)
                {
                    bool vIsException = false;
                    size_type vCount = 0;
                    for (; vCount < aSize; ++vCount)
                    {
                        if (end() == emplace_back())
                        {
                            vIsException = true;
                            break;
                        }
                    }
                    if (vIsException)
                    {
                        for (; vCount--; )
                        {
                            pop_back();
                        }
                    }
                }
                else 
                {
                    erase(vIter, end());
                    return true;
                }

                return false;
            }

            bool resize(size_type aSize, const value_type& aValue)
            {
                const_iterator vIter = __resize_position(aSize);
                if (aSize)
                {
                    if (end() != insert(end(), aSize, aValue))
                        return true;
                }
                else 
                { 
                    erase(vIter, end());
                    return true;
                }

                return false;
            }

            bool push_front(const value_type& aValue)
            {
                if (end() == emplace(begin(), aValue)) return false;
                return true;
            }

            bool push_front(value_type&& aValue)
            {
                if (end() == emplace(begin(), move(aValue))) return false;
                return true;
            }

            template<typename... _Args>
            iterator emplace_front(_Args... aArgs)
            {
                return emplace(begin(), forward<_Args>(aArgs)...);
            }

            void pop_front() NOEXCEPT$TYPE
            {
                erase(begin());
            }

            bool push_back(const value_type& aValue)
            {
                if (end() == emplace(end(), aValue)) return false;
                return true;
            }

            bool push_back(value_type&& aValue)
            {
                if (end() == emplace(end(), move(aValue))) return false;
                return true;
            }

            template<typename... _Args>
            iterator emplace_back(_Args... aArgs)
            {
                return emplace(end(), forward<_Args>(aArgs)...);
            }

            void pop_back() NOEXCEPT$TYPE
            {
                erase(iterator(m_head.m_prev));
            }

            template<typename... _Args>
            iterator emplace(const_iterator aPosition, _Args&&... aArgs)
            {
                node_type * vNode = __create_node(forward<_Args>(aArgs)...);
                if (nullptr == vNode)
                {
                    return end();
                }

                vNode->hook(aPosition.__const_cast().m_node);
                __inc_size(1);
                return iterator(vNode);
            }

            iterator insert(const_iterator aPosition, const value_type& aValue)
            {
                return emplace(aPosition, aValue);
            }

            iterator insert(const_iterator aPosition, value_type&& aValue)
            {
                return emplace(aPosition, move(aValue));
            }

            iterator insert(const_iterator aPosition, size_type aCount, const value_type& aValue)
            {
                if (aCount)
                {
                    list vTemp;
                    if (vTemp.construction(aCount, aValue, m_alloc))
                    {
                        iterator vIter = vTemp.begin();
                        if (splice(aPosition, vTemp)) return vIter;
                        else return end();
                    }
                }
                return aPosition.__const_cast();
            }

            template<typename _InputIter, typename = _RequireInputIterator<_InputIter>>
            iterator insert(const_iterator aPosition, _InputIter aFirst, _InputIter aLast)
            {
                if (aFirst != aLast)
                {
                    list vTemp;
                    if (vTemp.construction(aFirst, aLast, m_alloc) && !vTemp.empty())
                    {
                        iterator vIter = vTemp.begin();
                        if (splice(aPosition, vTemp)) return vIter;
                        else return end();
                    }
                }

                return aPosition.__const_cast();
            }

            iterator erase(const_iterator aPosition) NOEXCEPT$TYPE
            {
                iterator vNextIter = iterator(aPosition.m_node->m_next);

                iterator vPosition = aPosition.__const_cast();
                __dec_size(1);
                vPosition.m_node->unhook();
                __delete_node(static_cast<node_type*>(vPosition.m_node));

                return vNextIter;
            }

            iterator erase(const_iterator aFirst, const_iterator aLast) NOEXCEPT$TYPE
            {
                while (aFirst != aLast)
                {
                    aFirst = erase(aFirst);
                }
                return aLast.__const_cast();
            }

            void swap(list& aRight) NOEXCEPT$TYPE
            {
                _List_node_base::swap(m_head, aRight.m_head);

                size_type vSize = aRight.__get_size();
                aRight.__set_size(__get_size());
                __set_size(vSize);

                m_alloc.swap(aRight.m_alloc);
            }

            bool splice(const_iterator aPostion, list&& aRight) NOEXCEPT$TYPE
            {
                if (m_alloc != aRight.m_alloc) return false;

                if (!aRight.empty())
                {
                    __transfers(aPostion.__const_cast(), aRight.begin(), aRight.end());
                    __inc_size(aRight.__get_size());
                    aRight.__set_size(0);
                }

                return true;
            }

            bool splice(const_iterator aPosition, list& aRight) NOEXCEPT$TYPE
            {
                return splice(aPosition, move(aRight));
            }

            bool splice(const_iterator aPosition, list&& aRight,
                const_iterator aRightFirst, const_iterator aRightLast) NOEXCEPT$TYPE
            {
                if (aRightFirst != aRightLast
                    && aPosition != aRightFirst)
                {
                    if (this != addressof(aRight)
                        && m_alloc != aRight.m_alloc)
                        return false;

                    size_type vCount = distance(aRightFirst, aRightLast);
                    __inc_size(vCount);
                    aRight.__dec_size(vCount);

                    __transfers(aPosition.__const_cast(), 
                        aRightFirst.__const_cast(), aRightLast.__const_cast());
                }

                return true;
            }

            bool splice(const_iterator aPosition, list& aRight,
                const_iterator aRightFirst, const_iterator aRightLast) NOEXCEPT$TYPE
            {
                return splice(aPosition, move(aRight), aRightFirst, aRightLast);
            }

            bool splice(const_iterator aPosition, list&& aRight, const_iterator aRightPosition)
            {
                iterator vRightPosition = aRightPosition.__const_cast();
                ++vRightPosition;

                if (aPosition == vRightPosition) 
                {
                    return true;
                }

                return splice(aPosition, aRight, aRightPosition.__const_cast(), vRightPosition);
            }

            bool splice(const_iterator aPosition, list& aRight, const_iterator aRightPosition)
            {
                return splice(aPosition, move(aRight), aRightPosition);
            }

            template<typename _Functor>
            void remove_if(_Functor aFunctor)
            {
                if (empty()) return;

                iterator vFirst = begin();
                iterator vLast = end();

                while (vFirst != vLast)
                {
                    iterator vNext = vFirst;
                    ++vNext;
                    if (aFunctor(*vFirst)) erase(vFirst);
                    vFirst = vNext;
                }
            }

            void remove(const value_type& aValue)
            {
                return remove_if([&aValue](const value_type& aValue2) -> bool 
                {
                    if (aValue == aValue2) return true;
                    return false;
                });
            }

            template<typename _Functor>
            void unique(_Functor aFunctor)
            {
                if (empty()) return;

                iterator vFirst = begin();
                iterator vLast = end();
                iterator vNext = vFirst;

                while (++vNext != vLast)
                {
                    if (aFunctor(*vFirst, *vNext)) erase(vNext);
                    else vFirst = vNext;

                    vNext = vFirst;
                }
            }

            void unique()
            {
                return unique(equal_to<>());
            }

            template<typename _StrictWeakOrdering>
            bool merge(list&& aRight, _StrictWeakOrdering aComparator)
            {
                if (this == addressof(aRight)) return true;
                if (m_alloc != aRight.m_alloc) return false;

                iterator vLeftFirst = begin();
                iterator vLeftLast = end();
                iterator vRightFirst = aRight.begin();
                iterator vRightLast = aRight.end();

                const size_type vOriginalSize = aRight.size();

                while (vLeftFirst != vLeftLast && vRightFirst != vRightLast)
                {
                    if (aComparator(*vRightFirst, *vLeftFirst))
                    {
                        iterator vNext = vRightFirst;
                        __transfers(vLeftFirst, vRightFirst, ++vNext);
                        vRightFirst = vNext;
                    }
                    else
                    {
                        ++vLeftFirst;
                    }
                }

                if (vRightFirst != vRightLast)
                {
                    __transfers(vLeftLast, vRightFirst, vRightLast);
                }

                __inc_size(aRight.__get_size());
                aRight.__set_size(0);

                return true;
            }

            template<typename _StrictWeakOrdering>
            bool merge(list& aRight, _StrictWeakOrdering aComparator)
            {
                return merge(move(aRight), aComparator);
            }

            bool merge(list&& aRight)
            {
                return merge(move(aRight), less<>());
            }

            bool merge(list& aRight)
            {
                return merge(move(aRight));
            }

            void reverse() NOEXCEPT$TYPE
            {
                m_head.reverse();
            }

            template<typename _StrictWeakOrdering>
            void sort(_StrictWeakOrdering aComparator)
            {
                __sort_impl(begin(), end(), aComparator, size());
            }

            void sort()
            {
                return sort(less<>());
            }

        };

    }

}

#endif
