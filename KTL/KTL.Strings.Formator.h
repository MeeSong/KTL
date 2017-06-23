#pragma once
#ifndef Formator_$B785EA02_1387_43E7_9AE9_A1D20C2F0BFB
#define Formator_$B785EA02_1387_43E7_9AE9_A1D20C2F0BFB 1

#include "KTL.Macro.h"
#include "KTL.Strings.h"
#include "KTL.Type.FloatPointer.h"

namespace ktl
{
    inline namespace strings
    {
        template<typename _Character>
        class Formator
        {
        public:
            enum
            {
                float_convert_buffer_size = (309 + 40),
            };

            using value_type = typename decay<_Character>::type;
            using pointer = value_type*;
            using const_pointer = const value_type*;

            using size_type = usize;
            using difference_type = isize;

        private:

            u32 m_Radix = 10;
            i32 m_IntegerPrecision = 1;
            i32 m_FloatPrecision = 6;
            bool m_CapitalHexits = false;
            bool m_WideString = (sizeof(value_type) == sizeof(wchar));

            pointer     m_String = nullptr;
            size_type   m_StringMaxCharCount = 0;
            size_type   m_FormatLength = 0;

            char*       m_FloatFormatBuffer = nullptr;
            size_type   m_FloatFormatBufferMaxCharCount = 0;

        protected:

            struct append_integer_tag {};
            struct append_float_pointer_tag {};
            struct append_pointer_tag {};

            struct string_float_pointer
            {
                i32 m_sign = 0;             // Zero if positive otherwise negative
                i32 m_decpt = 0;            // Exponent of floating point number
                char* m_mantissa = nullptr; // Pointer to mantissa in string form
            };

            bool float_pointer_parse_nan_or_infinity_into_buffer( 
                float_pointer_class aClassification, 
                bool aIsNegative)
            {
                using floating_traits = floating_type_traits<double>;
                using components_type = floating_traits::components_type;

                auto vPtr = m_FloatFormatBuffer;
                auto vBufferCount = m_FloatFormatBufferMaxCharCount;

                if (m_FloatFormatBufferMaxCharCount < (KTL$CompileTime$ArraySize$Macro("INF") + aIsNegative))
                {
                    vPtr[0] = '\0';
                    return false;
                }

                if (aIsNegative)
                {
                    *vPtr++ = '-';
                    *vPtr   = '\0';
                    --vBufferCount;
                }

                char const* const vFloatStrings[][4] =
                {
                    { "INF",       "INF", "inf",       "inf" }, // Infinity
                    { "NAN",       "NAN", "nan",       "nan" }, // Quiet NAN
                    { "NAN(SNAN)", "NAN", "nan(snan)", "nan" }, // Signaling NAN
                    { "NAN(IND)",  "NAN", "nan(ind)",  "nan" }, // Indeterminate
                };

                u32 const vRow = static_cast<u32>(aClassification) - 1;
                u32 const vColumn = m_CapitalHexits ? 0 : 2;

                bool const vStringLongWillFit = vBufferCount > string_length(vFloatStrings[vRow][vColumn]);
                string_copy(vPtr, vBufferCount, vFloatStrings[vRow][vColumn + !vStringLongWillFit]);

                return true;
            }

            bool float_pointer_parse_into_buffer(
                double const& aValue,
                float_pointer_class aClassification)
            {
                using floating_traits = floating_type_traits<double>;
                using components_type = floating_traits::components_type;
                components_type& vComponents = reinterpret_cast<components_type&>(aValue);

                string_float_pointer vStringFloat;
                vStringFloat.m_sign = vComponents.m_sign == 1 ? '-' : ' ';
                vStringFloat.m_mantissa = m_FloatFormatBuffer;

                bool const vIsZero = vComponents.m_exponent == 0 && vComponents.m_mantissa == 0;
                if (vIsZero)
                {
                    vStringFloat.m_decpt = 0;
                    string_copy(m_FloatFormatBuffer, m_FloatFormatBufferMaxCharCount, "0", 1);
                    return true;
                }

                if (aClassification != float_pointer_class::finite)
                {
                    vStringFloat.m_decpt = 1;
                }

                switch (aClassification)
                {
                case float_pointer_class::infinity:
                    string_copy(m_FloatFormatBuffer, m_FloatFormatBufferMaxCharCount, "1#INF");
                    break;
                case float_pointer_class::quiet_nan:
                    string_copy(m_FloatFormatBuffer, m_FloatFormatBufferMaxCharCount, "1#QNAN");
                    break;
                case float_pointer_class::signaling_nan:
                    string_copy(m_FloatFormatBuffer, m_FloatFormatBufferMaxCharCount, "1#SNAN");
                    break;
                case float_pointer_class::indeterminate:
                    string_copy(m_FloatFormatBuffer, m_FloatFormatBufferMaxCharCount, "1#IND");
                    break;
                }

                vComponents.m_sign = 0;
                
                bool const vIsDenormal = vComponents.m_exponent == 0;
                u64 const vMantissaAdjustment = vIsDenormal ? 0 : (static_cast<u64>(1) << (floating_traits::mantissa_bits - 1));
                i32 const vExponentAdjustment = vIsDenormal ? 2 : 1;



            }

            template<typename _UInteger>
            void integer_parse_into_buffer(_UInteger aValue, bool aNegative)
            {
                const static size_type vMaxBitsCount = sizeof(u64) * 8;
                value_type vStr[vMaxBitsCount]{};

                pointer const vLastDigit{ &vStr[vMaxBitsCount] - 1 };
                auto vCursor = vLastDigit;
                
                i32 vPrecision = m_IntegerPrecision;
                while (vPrecision > 0 || aValue != 0)
                {
                    value_type vDigit{ static_cast<value_type>(aValue % m_Radix + value_type('0')) };
                    aValue /= m_Radix;

                    if (vDigit > value_type('9'))
                    {
                        int const vBaseLetter{ m_CapitalHexits ? value_type('A') : value_type('a') };
                        int const vOffset{ vBaseLetter - value_type('9') - 1 };

                        vDigit += static_cast<value_type>(vOffset);
                    }

                    *vCursor-- = static_cast<value_type>(vDigit);
                    --vPrecision;
                } 

                if (aNegative && 10 == m_Radix)
                {
                    *vCursor-- = static_cast<value_type>('-');
                }

                size_type vDigitLength = static_cast<size_type>(vLastDigit - vCursor);
                ++vCursor;

                write_string(vCursor, vDigitLength);
            }

            template<typename _Integral, class = typename enable_if<is_integral<_Integral>::value>::type>
            bool append(_Integral&& aValue, append_integer_tag = append_integer_tag{})
            {
                bool vNegative = false;
                u64 vNumber{};
                if ((10 == m_Radix) && (aValue < _Integral(0)))
                {
                    vNegative = true;
                    vNumber = static_cast<u64>((!aValue)+1);
                }
                else
                {
                    vNumber = static_cast<u64>(aValue);
                }

                size_type vIntegerSize = sizeof(aValue);
                if (sizeof(u64) == vIntegerSize)
                {
                    integer_parse_into_buffer<u64>(vNumber, vNegative);
                }
                else
                {
                    integer_parse_into_buffer<u32>(static_cast<u32>(vNumber), vNegative);
                }

                return true;
            }

            template<typename _Pointer, class = typename enable_if<is_pointer<_Pointer>::value>::type>
            bool append(_Pointer&& aValue, append_pointer_tag = append_pointer_tag{})
            {
                return append<difference_type>(move(reinterpret_cast<difference_type>(aValue)));
            }

            template<typename _FloatPointer, class = typename enable_if<is_floating_point<_FloatPointer>::value>::type>
            bool append(_FloatPointer&& aValue, append_float_pointer_tag = append_float_pointer_tag{})
            {
                bool vResult = false;

                i32 vPrecision = m_FloatPrecision;
                if (vPrecision < 0)
                {
                    vPrecision = 6;
                }
                else if (vPrecision == 0)
                {
                    vPrecision = 1;
                }

                for (;;)
                {
                    m_FloatFormatBufferMaxCharCount = float_convert_buffer_size + vPrecision;
                    m_FloatFormatBuffer = new char[m_FloatFormatBufferMaxCharCount];
                    if (nullptr == m_FloatFormatBuffer)
                    {
                        break;
                    }

                    f64 vNumber{ static_cast<f64>(aValue) };
                    float_pointer_class const vClassification = float_pointer_classify(aValue);
                    if (vClassification != float_pointer_class::finite)
                    {
                        vResult = float_pointer_parse_nan_or_infinity_into_buffer(vClassification, float_pointer_is_negative(aValue));
                    }
                    else
                    {
                        vResult = float_pointer_parse_into_buffer(aValue, vClassification);
                    }

                    if (!vResult) break;

                    if (m_WideString)
                    {
                        // vResult = ;
                    }
                    else
                    {
                        vResult = write_string(m_FloatFormatBuffer, string_length(m_FloatFormatBuffer));
                    }
                    
                    if (!vResult) break;

                    vResult = true;
                    break;
                }

                if (m_FloatFormatBuffer)
                {
                    delete[] m_FloatFormatBuffer;
                    m_FloatFormatBuffer = nullptr;
                    m_FloatFormatBufferMaxCharCount = 0;
                }

                return vResult;
            }

            bool append(const char aChar)
            {
                if ((difference_type(m_StringMaxCharCount) - difference_type(m_FormatLength)) > 1)
                {
                    m_String[m_FormatLength] = value_type(aChar);
                }

                ++m_FormatLength;
                return true;
            }

            bool append(const wchar aChar)
            {
                if (m_WideString)
                {
                    if ((difference_type(m_StringMaxCharCount) - difference_type(m_FormatLength)) > 1)
                    {
                        m_String[m_FormatLength] = value_type(aChar);
                    }

                    ++m_FormatLength;
                }
                else if (aChar < wchar(127))
                {
                    if ((difference_type(m_StringMaxCharCount) - difference_type(m_FormatLength)) > 1)
                    {
                        m_String[m_FormatLength] = KTL$CompileTime$FirstByte$Macro(aChar);
                    }

                    ++m_FormatLength;
                }
                else
                {
                    if ((difference_type(m_StringMaxCharCount) - difference_type(m_FormatLength)) > 2)
                    {
                        m_String[m_FormatLength + 0] = KTL$CompileTime$FirstByte$Macro(aChar);
                        m_String[m_FormatLength + 1] = KTL$CompileTime$SecondByte$Macro(aChar);
                    }

                    m_FormatLength += 2;
                }

                return true;
            }

            bool append(const char* aString)
            {
                if (aString == nullptr)
                {
                    aString = "(null)";
                }
                size_type vStringLength = string_length(aString);

                if (m_WideString)
                {
                }
                else
                {
                    write_string(aString, vStringLength);
                }

                return true;
            }

            bool append(const wchar* aString)
            {
                if (aString == nullptr)
                {
                    aString = L"(null)";
                }

                size_type vStringLength = string_length(aString);

                if (m_WideString)
                {
                    write_string(aString, vStringLength);
                }
                else
                {

                }

                return true;
            }

            bool append(const ansi_string* aAnsiString)
            {
                if (nullptr == aAnsiString
                    || nullptr == aAnsiString->Buffer
                    || 0 == aAnsiString->Length
                    || 0 == aAnsiString->MaximumLength)
                {
                    return append("(null)");
                }

                if (m_WideString)
                {

                }
                else
                {
                    write_string(aAnsiString->Buffer, aAnsiString->Length);
                }

                return true;
            }

            bool append(const unicode_string* aUnicodeString)
            {
                if (nullptr == aUnicodeString
                    || nullptr == aUnicodeString->Buffer
                    || 0 == aUnicodeString->Length
                    || 0 == aUnicodeString->MaximumLength)
                {
                    return append(L"(null)");
                }

                if (m_WideString)
                {
                    write_string(aUnicodeString->Buffer, aUnicodeString->Length);
                }
                else
                {

                }

                return true;
            }

            bool append_format()
            {
                return true;
            }

            template<typename _Format, typename... _Args>
            bool append_format(_Format&& aFormat, _Args&&... aArgs) NOEXCEPT$TYPE
            {
                if (!append(move(aFormat))) return false;
                return append_format(forward<_Args>(aArgs)...);
            }

            bool initialize(pointer aString, size_type aStringMaxCharCount)
            {
                m_Radix = 10;
                m_IntegerPrecision = 1;
                m_FloatPrecision = 6;
                m_CapitalHexits = false;
                m_WideString = (sizeof(value_type) == sizeof(wchar));

                m_String = aString;
                m_StringMaxCharCount = aStringMaxCharCount;
                m_FormatLength = 0;

                m_FloatFormatBuffer = nullptr;
                m_FloatFormatBufferMaxCharCount = 0;

                return true;
            }

            bool write_string(const_pointer aString, size_type aStringCharCount)
            {
                if (nullptr == aString
                    || 0 == aStringCharCount)
                {
                    return false;
                }

                if (m_FormatLength < m_StringMaxCharCount)
                {
                    string_append(
                        m_String, m_StringMaxCharCount,
                        aString, aStringCharCount);
                }

                m_FormatLength += aStringCharCount;
                return true;
            }

        public:

            template<typename... _Args>
            difference_type format(
                pointer aString, size_type aStringMaxCharCount,
                _Args... aArgs) NOEXCEPT$TYPE
            {
                if (nullptr == aString
                    || 0 == aStringMaxCharCount)
                {
                    return -1;
                }

                if (!initialize(aString, aStringMaxCharCount)) return -1;

                if (!append_format(forward<_Args>(aArgs)...)) return -1;

                if (aStringMaxCharCount <= m_FormatLength)
                {
                    aString[aStringMaxCharCount - 1] = value_type('\0');
                }

                return m_FormatLength;
            }
        };

    }

}

#endif
