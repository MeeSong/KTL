#pragma once
#ifndef PointerTraits_$27ED7FCF_E916_4B9B_B367_E997F9B737D4
#define PointerTraits_$27ED7FCF_E916_4B9B_B367_E997F9B737D4 1

#include "KTL.Type.Uitility.h"

namespace ktl
{
    inline namespace type_traits
    {
        // Given Template<T, ...> return T, otherwise invalid.
        
        template<typename _Type>
        struct __get_first_arg
        {
            using type = _undefined;
        };

        template<template<typename, typename...> class _Template, typename _Type, typename... _Types>
        struct __get_first_arg<_Template<_Type, _Types...>>
        {
            using type = _Type;
        };

        template<typename _Type>
        using __get_first_arg_t = typename __get_first_arg<_Type>::type;

        // Given Template<T, ...> and U return Template<U, ...>, otherwise invalid.
        
        template<typename _Type, typename _UType>
        struct __replace_first_arg
        { };

        template<template<typename, typename...> class _Template, typename _UType, typename _Type, typename... _Types>
        struct __replace_first_arg<_Template<_Type, _Types...>, _UType>
        {
            using type = _Template<_UType, _Types...>;
        };

        template<typename _Type, typename _UType>
        using __replace_first_arg_t = typename __replace_first_arg<_Type, _UType>::type;

        /// pointer_traits

        template<typename _Ptr>
        struct pointer_traits
        {
        private:

            template<typename _Type>
            using __element_type = typename _Type::element_type;

            template<typename _Type>
            using __difference_type = typename _Type::difference_type;
            
            template<typename _Type, typename _UType, typename = void>
            struct __rebind
                : __replace_first_arg<_Type, _UType>
            { };

            template<typename _Type, typename _UType>
            struct __rebind<_Type, _UType, void_t<typename _Type::template rebind<_UType>>>
            { using type = typename _Type::template rebind<_UType>; };

        public:

            using pointer = _Ptr;
            using element_type = __detected_or_t<__get_first_arg_t<_Ptr>, __element_type, _Ptr>;
            using difference_type = __detected_or_t<ktl::intptr, __difference_type, _Ptr>;

            template<typename _UType>
            using rebind = typename __rebind<_Ptr, _UType>::type;

            static _Ptr pointer_to(make_not_void<element_type>& aElement)
            {
                return _Ptr::pointer_to(aElement);
            }

            static_assert(!is_same<element_type, _undefined>::value, 
                "pointer type defines element_type or is like SomePointer<T, Args>");
        };

        template<typename _Type>
        struct pointer_traits<_Type *>
        {
            using pointer = _Type*;
            using element_type = _Type;
            using difference_type = ktl::intptr;

            template<typename _UType>
            using rebind = _UType*;

            static pointer pointer_to(make_not_void<element_type>& aValue) NOEXCEPT$TYPE
            {
                return addressof(aValue);
            }
        };

        template<typename _Ptr, typename _Type>
        using __pointer_rebind = typename pointer_traits<_Ptr>::template rebind<_Type>;

    }

}

#endif // !PointerTraits_$27ED7FCF_E916_4B9B_B367_E997F9B737D4
