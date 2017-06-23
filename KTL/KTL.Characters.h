#pragma once
#ifndef Characters_$CEBFBD62_DE6B_4263_98F1_60E07677ADE1
#define Characters_$CEBFBD62_DE6B_4263_98F1_60E07677ADE1 1

namespace ktl
{
    inline namespace characters
    {

        template<typename _C>
        _C characters_toupper(_C aChar)
        {
            enum
            {
                LatterCaseDifference = _C('a') - _C('A'),
            };

            if ('a' <= aChar && 'z' >= aChar)
            {
                aChar -= LatterCaseDifference;
            }

            return aChar;
        }

        template<typename _C>
        _C characters_tolower(_C aChar)
        {
            enum
            {
                LatterCaseDifference = _C('a') - _C('A'),
            };

            if ('A' <= aChar && 'Z' >= aChar)
            {
                aChar += LatterCaseDifference;
            }

            return aChar;
        }
    }

}


#endif
