#pragma once
#ifndef FloatPointer_$EE14AEA2_7DF8_4A48_B76C_803F06F98146
#define FloatPointer_$EE14AEA2_7DF8_4A48_B76C_803F06F98146 1

#include "KTL.Type.h"
#include "KTL.Macro.Float.h"

namespace ktl
{
    inline namespace type_traits
    {
        template <typename FloatingType>
        struct floating_type_traits;

        template <>
        struct floating_type_traits<float>
        {
            enum : i32
            {
                mantissa_bits = KTL$Constants$FloatMantissaDigits,
                exponent_bits = sizeof(float) * byte_bit - mantissa_bits,

                maximum_binary_exponent = KTL$Constants$FloatMaxBinaryExponent$Macro - 1,
                minimum_binary_exponent = KTL$Constants$FloatMinBinaryExponent$Macro - 1,

                exponent_bias = 127
            };

            enum : u32
            {
                exponent_mask = (1u << (exponent_bits)) - 1,
                normal_mantissa_mask = (1u << (mantissa_bits)) - 1,
                denormal_mantissa_mask = (1u << (mantissa_bits - 1)) - 1,

                special_nan_mantissa_mask = (1u << (mantissa_bits - 2))
            };

            struct components_type
            {
                u32 m_mantissa : mantissa_bits - 1;
                u32 m_exponent : exponent_bits;
                u32 m_sign : 1;
            };

            static_assert(sizeof(components_type) == sizeof(float), "unexpected components size");
        };

        template <>
        struct floating_type_traits<double>
        {
            enum : i32
            {
                mantissa_bits = KTL$Constants$DoubleMantissaDigits,
                exponent_bits = sizeof(double) * byte_bit - mantissa_bits,

                maximum_binary_exponent = KTL$Constants$DoubleMaxBinaryExponent$Macro - 1,
                minimum_binary_exponent = KTL$Constants$DoubleMinBinaryExponent$Macro - 1,

                exponent_bias = 1023
            };

            enum : u64
            {
                exponent_mask = (1ui64 << (exponent_bits)) - 1,
                normal_mantissa_mask = (1ui64 << (mantissa_bits)) - 1,
                denormal_mantissa_mask = (1ui64 << (mantissa_bits - 1)) - 1,

                special_nan_mantissa_mask = (1ui64 << (mantissa_bits - 2))
            };

            struct components_type
            {
                u64 m_mantissa : mantissa_bits - 1;
                u64 m_exponent : exponent_bits;
                u64 m_sign : 1;
            };

            static_assert(sizeof(components_type) == sizeof(double), "unexpected components size");
        };

        enum class float_pointer_class : u32
        {
            finite,
            infinity,
            quiet_nan,
            signaling_nan,
            indeterminate,
        };

        inline float_pointer_class float_pointer_classify(double const& aValue) NOEXCEPT$TYPE
        {
            using floating_traits = floating_type_traits<double>;
            using components_type = floating_traits::components_type;

            components_type const& vComponents = reinterpret_cast<components_type const&>(aValue);
            bool const vValueIsNanOrInfinity = vComponents.m_exponent == ((1u << floating_traits::exponent_bias) - 1);
            if (!vValueIsNanOrInfinity)
            {
                return float_pointer_class::finite;
            }
            else if (vComponents.m_mantissa == 0)
            {
                return float_pointer_class::infinity;
            }
            else if (vComponents.m_sign == 1 
                && vComponents.m_mantissa == floating_traits::special_nan_mantissa_mask)
            {
                return float_pointer_class::indeterminate;
            }
            else if (vComponents.m_mantissa & floating_traits::special_nan_mantissa_mask) // Quiet NAN
            {
                return float_pointer_class::quiet_nan;
            }
            else // Signaling NAN
            {
                return float_pointer_class::signaling_nan;
            }
        }

        inline bool float_pointer_is_negative(double const& aValue) NOEXCEPT$TYPE
        {
            using floating_traits = floating_type_traits<double>;
            using components_type = floating_traits::components_type;

            components_type const& vComponents = reinterpret_cast<components_type const&>(aValue);
            return (vComponents.m_sign == 1);
        }

    }
}

#endif
