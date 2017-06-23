#pragma once
#ifndef Strings_$0C5B6170_91D8_43BB_AD6D_8BD814C3866A
#define Strings_$0C5B6170_91D8_43BB_AD6D_8BD814C3866A 1

#include "KTL.Memory.h"
#include "KTL.Characters.h"
#include "KTL.Algorithm.h"

struct ansi_string {
    ktl::u16 Length = 0;
    ktl::u16 MaximumLength = 0;
    char* Buffer = nullptr;
};

struct unicode_string {
    ktl::u16 Length = 0;
    ktl::u16 MaximumLength = 0;
    ktl::wchar* Buffer = nullptr;
};

namespace ktl
{
    inline namespace strings
    {
        template<typename _C>
        usize string_length(const _C* aString)
        {
            usize vLength = 0;
            for (; aString[vLength]; ++vLength);
            return (vLength);
        }

        template<typename _C>
        usize string_copy(
            _C* aDestString, usize aDestStringMaxCharCount,
            const _C* aSrcString, usize aSrcStringCharCount = usize(-1))
        {
            if (nullptr == aDestString
                || nullptr == aSrcString
                || 0 == aDestStringMaxCharCount
                || 0 == aSrcStringCharCount)
            {
                return 0;
            }

            if (aSrcStringCharCount == usize(-1))
            {
                aSrcStringCharCount = string_length<_C>(aSrcString);
            }

            usize vCopyCount = 0;
            vCopyCount = memory_copy(
                (ktl::byte*)(aDestString), (aDestStringMaxCharCount - 1) * sizeof(_C), 
                (ktl::byte*)(aSrcString), aSrcStringCharCount * sizeof(_C));
            if (isize(vCopyCount) == 0)
            {
                return 0;
            }
            vCopyCount /= sizeof(_C);

            aDestString[vCopyCount] = _C(0);
            return vCopyCount;
        }

        template<typename _C>
        usize string_append(
            _C* aDestString, usize aDestStringMaxCharCount,
            const _C* aSrcString, usize aSrcStringCharCount = usize(-1))
        {
            if (nullptr == aDestString
                || nullptr == aSrcString
                || 0 == aDestStringMaxCharCount
                || 0 == aSrcStringCharCount)
            {
                return 0;
            }

            if (aSrcStringCharCount == usize(-1))
            {
                aSrcStringCharCount = string_length<_C>(aSrcString);
            }

            usize vDestStrlen = string_length(aDestString);
            if (vDestStrlen >= aDestStringMaxCharCount)
            {
                return 0;
            }

            usize vCopyCount = 0;
            vCopyCount = memory_copy(
                (ktl::byte*)(aDestString + vDestStrlen), (aDestStringMaxCharCount - vDestStrlen - 1) * sizeof(_C),
                (ktl::byte*)(aSrcString), aSrcStringCharCount * sizeof(_C));
            if (isize(vCopyCount) == 0)
            {
                return 0;
            }
            vCopyCount /= sizeof(_C);

            aDestString[vDestStrlen + vCopyCount] = _C(0);
            return vCopyCount;
        }
        
        template<typename _C>
        isize string_compare(
            const _C* aString1, usize aStringCharCount1,
            const _C* aString2, usize aStringCharCount2)
        {
            if ((0 == aStringCharCount1)
                && (aStringCharCount1 == aStringCharCount2))
            {
                return 0;
            }

            auto vMinCount = algorithm::min(aStringCharCount1, aStringCharCount2);
            for (usize i = 0
                ; i < vMinCount
                ; ++i)
            {
                if (aString1[i] < aString2[i]) return -1;
                if (aString1[i] > aString2[i]) return 1;
            }

            if (aStringCharCount1 == aStringCharCount2) return 0;
            if (vMinCount == aStringCharCount1) return -1;
            
            return 1;
        }

        template<typename _C>
        isize string_compare_case_insensitive(
            const _C* aString1, usize aStringCharCount1,
            const _C* aString2, usize aStringCharCount2)
        {
            if ((0 == aStringCharCount1)
                && (aStringCharCount1 == aStringCharCount2))
            {
                return 0;
            }

            auto vMinCount = algorithm::min(aStringCharCount1, aStringCharCount2);
            for (usize i = 0
                ; i < vMinCount
                ; ++i)
            {
                if (characters_tolower(aString1[i]) < characters_tolower(aString2[i])) return -1;
                if (characters_tolower(aString1[i]) > characters_tolower(aString2[i])) return 1;
            }

            if (aStringCharCount1 == aStringCharCount2) return 0;
            if (vMinCount == aStringCharCount1) return -1;

            return 1;
        }

        template<typename _C>
        bool string_same(
            const _C* aString1, usize aStringCharCount1,
            const _C* aString2, usize aStringCharCount2)
        {
            if (nullptr == aString1
                || nullptr == aString2
                || 0 == aStringCharCount1
                || 0 == aStringCharCount2
                || aStringCharCount1 != aStringCharCount2)
            {
                return false;
            }

            return (0 == string_compare(aString1, aStringCharCount1, aString2, aStringCharCount2));
        }

        template<typename _C>
        bool string_same_case_insensitive(
            const _C* aString1, usize aStringCharCount1,
            const _C* aString2, usize aStringCharCount2)
        {
            if (nullptr == aString1
                || nullptr == aString2
                || 0 == aStringCharCount1
                || 0 == aStringCharCount2
                || aStringCharCount1 != aStringCharCount2)
            {
                return false;
            }

            return (0 == string_compare_case_insensitive(aString1, aStringCharCount1, aString2, aStringCharCount2));
        }

        template<typename _C>
        const _C* string_find(
            const _C* aString, usize aStringCharCount,
            const _C* aSubString, usize aSubStringCharCount)
        {
            if (nullptr == aString
                || nullptr == aSubString
                || 0 == aStringCharCount
                || 0 == aSubStringCharCount
                || aSubStringCharCount > aStringCharCount)
            {
                return nullptr;
            }

            usize vIndex = 0;
            for (;vIndex <= aStringCharCount - aSubStringCharCount; ++vIndex)
            {
                if (string_same(
                    aString + vIndex, aSubStringCharCount,
                    aSubString, aSubStringCharCount))
                {
                    return (aString + vIndex);
                }
            }

            return nullptr;
        }

        template<typename _C>
        const _C* string_find_case_insensitive(
            const _C* aString, usize aStringCharCount,
            const _C* aSubString, usize aSubStringCharCount)
        {
            if (nullptr == aString
                || nullptr == aSubString
                || 0 == aStringCharCount
                || 0 == aSubStringCharCount
                || aSubStringCharCount > aStringCharCount)
            {
                return nullptr;
            }

            usize vIndex = 0;
            usize vMaxIndex = aStringCharCount - aSubStringCharCount;
            for (; vIndex <= vMaxIndex; ++vIndex)
            {
                if (string_same_case_insensitive(
                    aString + vIndex, aSubStringCharCount,
                    aSubString, aSubStringCharCount))
                {
                    return (aString + vIndex);
                }
            }

            return nullptr;
        }

        template<typename _C>
        const _C* string_reverse_find(
            const _C* aString, usize aStringCharCount,
            const _C* aSubString, usize aSubStringCharCount)
        {
            if (nullptr == aString
                || nullptr == aSubString
                || 0 == aStringCharCount
                || 0 == aSubStringCharCount
                || aSubStringCharCount > aStringCharCount)
            {
                return nullptr;
            }

            usize vIndex = (aStringCharCount - aSubStringCharCount);
            for (; vIndex >=0; --vIndex)
            {
                if (string_same(
                    aString + vIndex, aSubStringCharCount,
                    aSubString, aSubStringCharCount))
                {
                    return (aString + vIndex);
                }
            }

            return nullptr;
        }

        template<typename _C>
        const _C* string_reverse_find_case_insensitive(
            const _C* aString, usize aStringCharCount,
            const _C* aSubString, usize aSubStringCharCount)
        {
            if (nullptr == aString
                || nullptr == aSubString
                || 0 == aStringCharCount
                || 0 == aSubStringCharCount
                || aSubStringCharCount > aStringCharCount)
            {
                return nullptr;
            }

            usize vIndex = (aStringCharCount - aSubStringCharCount);
            for (; vIndex >= 0; --vIndex)
            {
                if (string_same_case_insensitive(
                    aString + vIndex, aSubStringCharCount,
                    aSubString, aSubStringCharCount))
                {
                    return (aString + vIndex);
                }
            }

            return nullptr;
        }

        template<typename _C>
        const _C* char_find(const _C* aString, usize aStringCharCount, _C aChar)
        {
            _C vChar[] = { aChar };
            return string_find<_C>(aString, aStringCharCount, vChar, 1);
        }

        template<typename _C>
        const _C* char_find_case_insensitive(const _C* aString, usize aStringCharCount, _C aChar)
        {
            _C vChar[] = { aChar };
            return string_find_case_insensitive<_C>(aString, aStringCharCount, vChar, 1);
        }

        template<typename _C>
        const _C* char_reverse_find(const _C* aString, usize aStringCharCount, _C aChar)
        {
            _C vChar[] = { aChar };
            return string_reverse_find<_C>(aString, aStringCharCount, vChar, 1);
        }

        template<typename _C>
        const _C* char_reverse_find_case_insensitive(const _C* aString, usize aStringCharCount, _C aChar)
        {
            _C vChar[] = { aChar };
            return string_reverse_find_case_insensitive<_C>(aString, aStringCharCount, vChar, 1);
        }
    }

}

#endif
