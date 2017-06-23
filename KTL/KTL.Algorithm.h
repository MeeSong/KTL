#pragma once
#ifndef Algorithm_$529B7B76_3367_47DE_BDA2_4DC29CE6DA61
#define Algorithm_$529B7B76_3367_47DE_BDA2_4DC29CE6DA61 1

#include "KTL.Functional.h"
#include "KTL.Containers.Tuple.h"

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

namespace ktl
{
    inline namespace algorithm
    {
        template<typename T, typename Compare>
        const T& min(const T& a, const T& b, Compare aCompare)
        {
            return (aCompare(a, b)) ? a : b;
        }

        template<typename T>
        const T& min(const T& a, const T& b)
        {
            return min(a, b, less<>());
        }

        template<typename T, typename Compare>
        const T& max(const T& a, const T& b, Compare aCompare)
        {
            return (aCompare(a, b)) ? a : b;
        }

        template<typename T>
        const T& max(const T& a, const T& b)
        {
            return max(a, b, greater<>());
        }

        template<typename T, typename Compare>
        tuple<const T&, const T&> minmax(const T& a, const T& b, Compare aCompare)
        {
            return aCompare(a, b) 
                ? tuple<const T&, const T&>(a, b)
                : tuple<const T&, const T&>(b, a);
        }

        template<typename T>
        tuple<const T&, const T&> minmax(const T& a, const T& b)
        {
            return minmax(a, b, less<>());
        }

        template<typename ForwardIt, typename Compare>
        tuple<ForwardIt, ForwardIt> minmax_element(ForwardIt aFirst, ForwardIt aLast, Compare aCompare)
        {
            ForwardIt vMin(aFirst);
            ForwardIt vMax(aFirst);

            if ((aFirst == aLast)
                || (++aFirst == aLast))
            {
                return make_tuple(vMin, vMax);
            }

            if (aCompare(*aFirst, *vMin)) {
                vMin = aFirst;
            }
            else {
                vMax = aFirst;
            }
            while (++aFirst != aLast) {
                ForwardIt i = aFirst;
                if (++aFirst == aLast) {
                    if (aCompare(*i, *vMin)) vMin = i;
                    else if (!(aCompare(*i, *vMax))) vMax = i;
                    break;
                }
                else {
                    if (aCompare(*aFirst, *i)) {
                        if (aCompare(*aFirst, *vMin)) vMin = aFirst;
                        if (!(aCompare(*i, *vMax))) vMax = i;
                    }
                    else {
                        if (aCompare(*i, *vMin)) vMin = i;
                        if (!(aCompare(*aFirst, *vMax))) vMax = aFirst;
                    }
                }
            }
            return make_tuple(vMin, vMax);
        }

        template<typename ForwardIt>
        tuple<ForwardIt, ForwardIt> minmax_element(ForwardIt aFirst, ForwardIt aLast)
        {
            return minmax_element(aFirst, aLast, less<>());
        }

    }

}

#endif
