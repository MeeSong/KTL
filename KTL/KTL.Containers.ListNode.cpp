#include "KTL.Containers.ListNode.h"
#include "KTL.Type.Uitility.h"

namespace ktl
{
    inline namespace containers
    {
        /// STRUCT _List_node_base

        bool _List_node_base::empty() const NOEXCEPT$TYPE
        {
            return static_cast<bool>((m_next == this));
        }

        void _List_node_base::swap(
            _List_node_base& aLeft, 
            _List_node_base& aRight) NOEXCEPT$TYPE
        {
            if (!aLeft.empty())
            {
                if (!aRight.empty())
                {
                    // Both aLeft and aRight are not empty.

                    swap_adl(aLeft.m_next, aRight.m_next);
                    swap_adl(aLeft.m_prev, aRight.m_prev);
                    aLeft.m_next->m_prev = aLeft.m_prev->m_next = &aLeft;
                    aRight.m_next->m_prev = aRight.m_prev->m_next = &aRight;
                }
                else
                {
                    // aLeft is not empty, aRight is empty.

                    aRight.m_next = aLeft.m_next;
                    aRight.m_prev = aLeft.m_prev;
                    aRight.m_next->m_prev = aRight.m_prev->m_next = &aRight;

                    aLeft.m_next = aLeft.m_prev = &aLeft;
                }
            }
            else if (!aRight.empty())
            {
                // aLeft is empty, aRight is not empty.

                aLeft.m_next = aRight.m_next;
                aLeft.m_prev = aRight.m_prev;
                aLeft.m_next->m_prev = aLeft.m_prev->m_next = &aLeft;

                aRight.m_next = aRight.m_prev = &aRight;
            }

            // Both aLeft and aRight are empty.
        }

        void _List_node_base::transfer(
            _List_node_base *const aFirst, 
            _List_node_base *const aLast) NOEXCEPT$TYPE
        {
            if (aFirst 
                && aLast
                && this != aLast 
                && aFirst != aLast)
            {
                // Remove [first, last) from its old position.

                aLast->m_prev->m_next = this;
                aFirst->m_prev->m_next = aLast;
                this->m_prev->m_next = aFirst;

                // Splice [first, last) into its new position.

                _List_node_base *const vTemp = this->m_prev;
                this->m_prev = aLast->m_prev;
                aLast->m_prev = aFirst->m_prev;
                aFirst->m_prev = vTemp;
            }
        }

        void _List_node_base::reverse() NOEXCEPT$TYPE
        {
            _List_node_base * vTemp = this;
            do 
            {
                swap_adl(vTemp->m_next, vTemp->m_prev);

                // Old next node is now prev
                vTemp = vTemp->m_prev;
            } while (vTemp != this);
        }

        void _List_node_base::hook(_List_node_base *const aPosition) NOEXCEPT$TYPE
        {
            this->m_next = aPosition;
            this->m_prev = aPosition->m_prev;
            aPosition->m_prev->m_next = this;
            aPosition->m_prev = this;
        }

        void _List_node_base::unhook() NOEXCEPT$TYPE
        {
            _List_node_base *const vNext = this->m_next;
            _List_node_base *const vPrev = this->m_prev;
            vPrev->m_next = vNext;
            vNext->m_prev = vPrev;
        }

    }

}
