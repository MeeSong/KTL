#pragma once
#ifndef Type_$D54FC768_08D1_4C6A_8451_6462DDC767FF
#define Type_$D54FC768_08D1_4C6A_8451_6462DDC767FF 1

// reference rust language

#if __cplusplus < 201103L
#define NOEXCEPT$TYPE throw()
#else
#define NOEXCEPT$TYPE noexcept
#endif

namespace ktl
{
    using nullptr_t = decltype(__nullptr);
    using max_align_t = double;

    // bool
    using boolean = bool;
    // using boolean = unsigned char;

    // char
    using wchar = __wchar_t;

    using sbyte = signed __int8;
    using byte  = unsigned __int8;

    using i8    = signed __int8;
    using u8    = unsigned __int8;

    using i16   = signed __int16; // short
    using u16   = unsigned __int16;

    using i32   = signed __int32; // int
    using u32   = unsigned __int32;

    using i64   = signed __int64;
    using u64   = unsigned __int64;

    // using i128 = signed __int128;
    // using u128 = unsigned __int128;

    using f32 = float;
    using f64 = double;

#ifdef _M_X64
    using isize = i64;
    using usize = u64;
    using intptr = i64;
    using uintptr = u64;
#else
    using isize = i32;
    using usize = u32;
    using intptr = i32;
    using uintptr = u32;
#endif

    enum : byte
    {
        byte_bit = 8
    };

    enum : u8
    {
        max_u8 = u8(~((u8)0))
    };

    enum : i8
    {
        max_i8 = (max_u8 >> 1),
        min_i8 = ~max_i8
    };

    enum : u16
    {
        max_u16 = u16(~((u16)0))
    };

    enum : i16
    {
        max_i16 = (max_u16 >> 1),
        min_i16 = ~max_i16
    };

    enum : u32
    {
        max_u32 = u32(~((u32)0))
    };

    enum : i32
    {
        max_i32 = (max_u32 >> 1),
        min_i32 = ~max_i32
    };

    enum : u64
    {
        max_u64 = u64(~((u64)0))
    };

    enum : i64
    {
        max_i64 = (max_u64 >> 1),
        min_i64 = ~max_i64
    };

    enum : usize
    {
#ifdef _M_X64
        max_usize = max_u64
#else
        max_usize = max_u32
#endif
    };

    enum : isize
    {
#ifdef _M_X64
        max_isize = max_i64,
        min_isize = min_i64
#else
        max_isize = max_i32,
        min_isize = min_i32
#endif
    };

#define max_double      1.7976931348623158e+308
#define min_double      2.2250738585072014e-308
#define min_true_double 4.9406564584124654e-324

#define max_float       3.402823466e+38F
#define min_float       1.175494351e-38F
#define min_true_float  1.401298464e-45F

}

#endif