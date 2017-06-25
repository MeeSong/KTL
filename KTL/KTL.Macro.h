#pragma once
#ifndef Macro_$9287DB66_8141_4D80_9516_5D537C722795
#define Macro_$9287DB66_8141_4D80_9516_5D537C722795 1

#include "KTL.Type.h"

//////////////////////////////////////////////////////////////////////////

// offset
#ifndef KTL$CompileTime$OffsetOf$Macro
#define KTL$CompileTime$OffsetOf$Macro(s,m) ((ktl::usize)&reinterpret_cast<char const volatile&>((((s*)0)->m)))
#endif

// ARRAYSIZE
#ifndef KTL$CompileTime$ArraySize$Macro
extern "C++" // templates cannot be declared to have 'C' linkage
template <typename T, ktl::usize N>
char(*KTL$CompileTime$NumberOf$Function(__unaligned T(&)[N]))[N];

#define KTL$CompileTime$ArraySize$Macro(a) (sizeof(*KTL$CompileTime$NumberOf$Function(a)))
#endif

// ByteSwap

#ifndef KTL$CompileTime$ByteSwap16$Macro
#define KTL$CompileTime$ByteSwap16$Macro(x) (       \
    ((ktl::u16(x) & ktl::u16(0xFF << 8)) >> 8) |    \
    ((ktl::u16(x) & ktl::u16(0xFF >> 8)) << 8)      \
)
#endif

#ifndef KTL$CompileTime$ByteSwap32$Macro
#define KTL$CompileTime$ByteSwap32$Macro(x) (       \
    ((ktl::u32(x) & ktl::u32(0xFF << 24)) >> 24) |  \
    ((ktl::u32(x) & ktl::u32(0xFF << 16)) >>  8) |  \
    ((ktl::u32(x) & ktl::u32(0xFF <<  8)) <<  8) |  \
    ((ktl::u32(x) & ktl::u32(0xFF <<  0)) << 24)    \
)
#endif

#ifndef KTL$CompileTime$ByteSwap64$Macro
#define KTL$CompileTime$ByteSwap64$Macro(x) (       \
    ((ktl::u64(x) & ktl::u64(0xFF << 56)) >> 56) |  \
    ((ktl::u64(x) & ktl::u64(0xFF << 48)) >> 40) |  \
    ((ktl::u64(x) & ktl::u64(0xFF << 40)) >> 24) |  \
    ((ktl::u64(x) & ktl::u64(0xFF << 32)) >>  8) |  \
    ((ktl::u64(x) & ktl::u64(0xFF << 24)) <<  8) |  \
    ((ktl::u64(x) & ktl::u64(0xFF << 16)) << 24) |  \
    ((ktl::u64(x) & ktl::u64(0xFF <<  8)) << 40) |  \
    ((ktl::u64(x) & ktl::u64(0xFF <<  0)) << 56) |  \
)
#endif

// Make word
#ifndef KTL$CompileTime$MakeWord$Macro
#define KTL$CompileTime$MakeWord$Macro(a, b)    ((ktl::u16)(((ktl::u8)(((ktl::usize)(a)) & 0xff)) | ((ktl::u16)((ktl::u8)(((ktl::usize)(b)) & 0xff))) << 8))
#endif

// Make dword
#ifndef KTL$CompileTime$MakeDword$Macro
#define KTL$CompileTime$MakeDword$Macro(a, b)   ((ktl::u32)(((ktl::u16)(((ktl::usize)(a)) & 0xffff)) | ((ktl::u32)((ktl::u16)(((ktl::usize)(b)) & 0xffff))) << 16))
#endif

// Low word
#ifndef KTL$CompileTime$LowWord$Macro
#define KTL$CompileTime$LowWord$Macro(a)        ((ktl::u16)(((ktl::usize)(a)) & 0xffff))
#endif

// Hight word
#ifndef KTL$CompileTime$HightWord$Macro
#define KTL$CompileTime$HightWord$Macro(a)      ((ktl::u16)((((ktl::usize)(a)) >> 16) & 0xffff))
#endif

// Low byte
#ifndef KTL$CompileTime$LowByte$Macro
#define KTL$CompileTime$LowByte$Macro(a)        ((ktl::u8)(((ktl::usize)(a)) & 0xff))
#endif

// Hight byte
#ifndef KTL$CompileTime$HightByte$Macro
#define KTL$CompileTime$HightByte$Macro(a)      ((ktl::u8)((((ktl::usize)(a)) >> 8) & 0xff))
#endif

// First byte
#ifndef KTL$CompileTime$FirstByte$Macro
#define KTL$CompileTime$FirstByte$Macro(a)      ((ktl::u8)((((ktl::usize)(a)) >> 0) & 0xff))
#endif

// Second byte
#ifndef KTL$CompileTime$SecondByte$Macro
#define KTL$CompileTime$SecondByte$Macro(a)     ((ktl::u8)((((ktl::usize)(a)) >> 8) & 0xff))
#endif

// Third byte
#ifndef KTL$CompileTime$ThirdByte$Macro
#define KTL$CompileTime$ThirdByte$Macro(a)      ((ktl::u8)((((ktl::usize)(a)) >> 16) & 0xff))
#endif

// Fourth byte
#ifndef KTL$CompileTime$FourthByte$Macro
#define KTL$CompileTime$FourthByte$Macro(a)     ((ktl::u8)((((ktl::usize)(a)) >> 24) & 0xff))
#endif

//////////////////////////////////////////////////////////////////////////

#endif
