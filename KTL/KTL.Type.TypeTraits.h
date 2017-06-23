#pragma once
#ifndef TypeTraits_$F89CCC60_F845_4658_B183_44366E7F0302
#define TypeTraits_$F89CCC60_F845_4658_B183_44366E7F0302 1

#include "KTL.Type.h"

namespace ktl
{
    inline namespace type_traits
    {
        /// STRUCT _undefined

        struct _undefined;

        /// STRUCT _nil

        struct _nil
        {	// empty struct, for unused argument types
        };

        /// TEMPLATE CLASS integral_constant

        template<class _Type, _Type _Value>
        struct integral_constant
        {	// convenient template for integral constant types
            static constexpr _Type value = _Value;

            using value_type = _Type;
            using type = integral_constant<_Type, _Value>;

            constexpr operator value_type() const NOEXCEPT$TYPE
            {	// return stored value
                return (value);
            }

            constexpr value_type operator()() const NOEXCEPT$TYPE
            {	// return stored value
                return (value);
            }
        };

        using true_type = integral_constant<bool, true>;
        using false_type = integral_constant<bool, false>;

        /// ALIAS TEMPLATE bool_constant

        template<bool _Value>
        using bool_constant = integral_constant<bool, _Value>;

        /// TEMPLATE CLASS conditional

        template<bool _Test, typename _Iftrue, typename _Iffalse>
        struct conditional
        {	// type is _Iffalse for assumed !_Test
            using type = _Iffalse;
        };

        template<typename _Iftrue, typename _Iffalse>
        struct conditional<true, _Iftrue, _Iffalse>
        {	// type is _Iftrue for _Test
            using type = _Iftrue;
        };

        /// Alias template for conditional
        template<bool _Test, typename _Iftrue, typename _Iffalse>
        struct __if
            : public conditional<_Test, _Iftrue, _Iffalse>
        { };

        /// always_false

        template<typename _Type>
        struct always_false
        {
            static constexpr bool value = false;
        };

        /// variadic logical operation

        // Meta programming helper types.

        template<typename...>
        struct __or_;

        template<>
        struct __or_<>
            : public false_type
        { };

        template<typename _B1>
        struct __or_<_B1>
            : public _B1
        { };

        template<typename _B1, typename _B2>
        struct __or_<_B1, _B2>
            : public conditional<_B1::value, _B1, _B2>::type
        { };

        template<typename _B1, typename _B2, typename _B3, typename... _Bn>
        struct __or_<_B1, _B2, _B3, _Bn...>
            : public conditional<_B1::value, _B1, __or_<_B2, _B3, _Bn...>>::type
        { };

        template<typename...>
        struct __and_;

        template<>
        struct __and_<>
            : public true_type
        { };

        template<typename _B1>
        struct __and_<_B1>
            : public _B1
        { };

        template<typename _B1, typename _B2>
        struct __and_<_B1, _B2>
            : public conditional<_B1::value, _B2, _B1>::type
        { };

        template<typename _B1, typename _B2, typename _B3, typename... _Bn>
        struct __and_<_B1, _B2, _B3, _Bn...>
            : public conditional<_B1::value, __and_<_B2, _B3, _Bn...>, _B1>::type
        { };

        template<typename _Pp>
        struct __not_
            : public bool_constant<!_Pp::value>
        { };

        // and
        template<typename... _Bn>
        struct conjunction
            : __and_<_Bn...>
        { };

        // or
        template<typename... _Bn>
        struct disjunction
            : __or_<_Bn...>
        { };

        // not
        template<typename _Pp>
        struct negation
            : __not_<_Pp>
        { };

        /// TEMPLATE CLASS enable_if

        template<bool _Test, typename _Type = void>
        struct enable_if
        {	// type is undefined for assumed !_Test
        };

        template<typename _Type>
        struct enable_if<true, _Type>
        {	// type is _Type for _Test
            using type = _Type;
        };

        template <typename... _Cond>
        using __require = typename enable_if<conjunction<_Cond...>::value>::type;

        /// TEMPLATE CLASS is_same

        template<typename _Type1, typename _Type2>
        struct is_same
            : false_type
        {	// determine whether _Ty1 and _Ty2 are the same type
        };

        template<typename _Type1>
        struct is_same<_Type1, _Type1>
            : true_type
        {	// determine whether _Ty1 and _Ty2 are the same type
        };

        template<typename _Type, typename _Type2>
        constexpr bool is_same_v = is_same<_Type, _Type2>::value;

        /// TEMPLATE CLASS remove const & volatile

        // TEMPLATE CLASS remove_const
        template<typename _Type>
        struct remove_const
        {	// remove top level const qualifier
            using type = _Type;
        };

        template<typename _Type>
        struct remove_const<const _Type>
        {	// remove top level const qualifier
            using type = _Type;
        };

        // TEMPLATE CLASS remove_volatile
        template<typename _Type>
        struct remove_volatile
        {	// remove top level volatile qualifier
            using type = _Type;
        };

        template<typename _Type>
        struct remove_volatile<volatile _Type>
        {	// remove top level volatile qualifier
            using type = _Type;
        };

        // TEMPLATE CLASS remove_cv
        template<typename _Type>
        struct remove_cv
        {	// remove top level const and volatile qualifiers
            using type = typename remove_const<
                typename remove_volatile<_Type>::type>::type;
        };

        /// TEMPLATE CLASS add const & volatile

        template<typename _Type>
        struct add_const
        {
            using type = const _Type;
        };

        template<typename _Type>
        struct add_volatile
        {
            using type = volatile _Type;
        };

        template<typename _Type>
        struct add_cv
        {
            using type = const volatile _Type;
        };

        /// TEMPLATE CLASS remove_reference

        template<class _Type>
        struct remove_reference
        {	// remove reference
            using type = _Type;
        };

        template<class _Type>
        struct remove_reference<_Type&>
        {	// remove reference
            using type = _Type;
        };

        template<class _Type>
        struct remove_reference<_Type&&>
        {	// remove rvalue reference
            using type = _Type;
        };

        /// TEMPLATE CLASS add_reference

        template<typename _Type>
        struct is_referenceable;

        template<typename _Type, bool = is_referenceable<_Type>::value>
        struct __add_reference_helper
        {
            using lvalue = _Type;
            using rvalue = _Type;
        };

        template<typename _Type>
        struct __add_reference_helper<_Type, true>
        {
            using lvalue = _Type&;
            using rvalue = _Type&&;
        };

        template<typename _Type>
        struct add_lvalue_reference
        {
            using type = typename __add_reference_helper<_Type>::lvalue;
        };

        template<typename _Type>
        struct add_rvalue_reference
        {
            using type = typename __add_reference_helper<_Type>::rvalue;
        };

        /// TEMPLATE FUNCTION declval
        template<typename _Type>
        typename add_rvalue_reference<_Type>::type declval() NOEXCEPT$TYPE;

        /// TEMPLATE CLASS is_void

        template<typename _Type>
        struct __is_void_helper
            : false_type
        { };

        template<>
        struct __is_void_helper<void>
            : public true_type
        { };

        template<typename _Type>
        struct is_void
            : __is_void_helper<typename remove_cv<_Type>::type>
        { };

        /// TEMPLATE CLASS is_integral

        // TEMPLATE CLASS __is_integral_helper
        template<typename>
        struct __is_integral_helper
            : public false_type
        { };

        template<>
        struct __is_integral_helper<bool>
            : public true_type
        { };

        template<>
        struct __is_integral_helper<char>
            : public true_type
        { };

        template<>
        struct __is_integral_helper<sbyte>
            : public true_type
        { };

        template<>
        struct __is_integral_helper<byte>
            : public true_type
        { };

        template<>
        struct __is_integral_helper<wchar>
            : public true_type
        { };

        template<>
        struct __is_integral_helper<char16_t>
            : public true_type
        { };

        template<>
        struct __is_integral_helper<char32_t>
            : public true_type
        { };

        template<>
        struct __is_integral_helper<i16>
            : public true_type
        { };

        template<>
        struct __is_integral_helper<u16>
            : public true_type
        { };

        template<>
        struct __is_integral_helper<i32>
            : public true_type
        { };

        template<>
        struct __is_integral_helper<u32>
            : public true_type
        { };

        template<>
        struct __is_integral_helper<long>
            : public true_type
        { };

        template<>
        struct __is_integral_helper<unsigned long>
            : public true_type
        { };

        template<>
        struct __is_integral_helper<long long>
            : public true_type
        { };

        template<>
        struct __is_integral_helper<unsigned long long>
            : public true_type
        { };

        // TEMPLATE CLASS is_integral
        template<typename _Type>
        struct is_integral
            : public __is_integral_helper<typename remove_cv<_Type>::type>::type
        { };

        template<class _Type>
        constexpr bool is_integral_v = is_integral<_Type>::value;

        /// TEMPLATE CLASS is_floating_point

        // TEMPLATE CLASS __is_floating_point_helper
        template<typename>
        struct __is_floating_point_helper
            : public false_type
        { };

        template<>
        struct __is_floating_point_helper<f32>
            : public true_type
        { };

        template<>
        struct __is_floating_point_helper<f64>
            : public true_type
        { };

        template<>
        struct __is_floating_point_helper<long double>
            : public true_type
        { };

        // TEMPLATE CLASS is_floating_point
        template<typename _Type>
        struct is_floating_point
            : public __is_floating_point_helper<typename remove_cv<_Type>::type>::type
        { };

        template<class _Type>
        constexpr bool is_floating_point_v = is_floating_point<_Type>::value;

        /// TEMPLATE CLASS is_arithmetic

        template<class _Type>
        struct is_arithmetic
            : public bool_constant<is_integral<_Type>::value
            || is_floating_point<_Type>::value>
        {	// determine whether _Type is an arithmetic type
        };

        template<class _Type>
        constexpr bool is_arithmetic_v = is_arithmetic<_Type>::value;

        /// TEMPLATE CLASS is_array

        template<typename>
        struct is_array
            : public false_type
        { };

        template<typename _Type, usize _Size>
        struct is_array<_Type[_Size]>
            : public true_type
        { };

        template<typename _Type>
        struct is_array<_Type[]>
            : public true_type
        { };

        /// TEMPLATE CLASS is_enum

        template<typename _Type>
        struct is_enum
            : public bool_constant<__is_enum(_Type)>
        { };

        /// TEMPLATE CLASS is_union

        template<typename _Type>
        struct is_union
            : public bool_constant< __is_union(_Type)>
        { };

        /// TEMPLATE CLASS is_class

        template<typename _Type>
        struct is_class
            : public bool_constant< __is_class(_Type)>
        { };

        /// TEMPLATE CLASS is_function

        template<typename>
        struct is_function
            : public false_type { };

        template<typename _ReturnType, typename... _ArgTypes>
        struct is_function<_ReturnType(_ArgTypes...)>
            : public true_type { };

        template<typename _ReturnType, typename... _ArgTypes>
        struct is_function<_ReturnType(_ArgTypes...) &>
            : public true_type { };

        template<typename _ReturnType, typename... _ArgTypes>
        struct is_function<_ReturnType(_ArgTypes...) && >
            : public true_type { };

        template<typename _ReturnType, typename... _ArgTypes>
        struct is_function<_ReturnType(_ArgTypes...) const>
            : public true_type { };

        template<typename _ReturnType, typename... _ArgTypes>
        struct is_function<_ReturnType(_ArgTypes...) const &>
            : public true_type { };

        template<typename _ReturnType, typename... _ArgTypes>
        struct is_function<_ReturnType(_ArgTypes...) const &&>
            : public true_type { };

        template<typename _ReturnType, typename... _ArgTypes>
        struct is_function<_ReturnType(_ArgTypes...) volatile>
            : public true_type { };

        template<typename _ReturnType, typename... _ArgTypes>
        struct is_function<_ReturnType(_ArgTypes...) volatile &>
            : public true_type { };

        template<typename _ReturnType, typename... _ArgTypes>
        struct is_function<_ReturnType(_ArgTypes...) volatile &&>
            : public true_type { };

        template<typename _ReturnType, typename... _ArgTypes>
        struct is_function<_ReturnType(_ArgTypes...) const volatile>
            : public true_type { };

        template<typename _ReturnType, typename... _ArgTypes>
        struct is_function<_ReturnType(_ArgTypes...) const volatile &>
            : public true_type { };

        template<typename _ReturnType, typename... _ArgTypes>
        struct is_function<_ReturnType(_ArgTypes...) const volatile &&>
            : public true_type { };

        /// TEMPLATE CLASS is_member_object_pointer

        template<typename _Type, bool = is_function<_Type>::value>
        struct __is_member_object_pointer_impl
            : public false_type
        { };

        template<typename _Type, typename _Class>
        struct __is_member_object_pointer_impl<_Type _Class::*, false>
            : public true_type
        {
            using __class_type = _Class;
        };

        template<typename _Type>
        struct is_member_object_pointer
            : public __is_member_object_pointer_impl<
            typename remove_cv<_Type>::type>::type
        { };

        /// TEMPLATE CLASS is_member_function_pointer

        template<typename _Type, bool = is_function<_Type>::value>
        struct __is_member_function_pointer_impl
            : public false_type
        { };

        template<typename _Type, typename _Class>
        struct __is_member_function_pointer_impl<_Type _Class::*, true>
            : public true_type
        {
            using __class_type = _Class;
        };

        template<typename _Type>
        struct is_member_function_pointer
            : public __is_member_function_pointer_impl<
            typename remove_cv<_Type>::type>::type
        { };

        /// TEMPLATE CLASS is_pointer

        template<typename>
        struct __is_pointer_helper
            : public false_type
        { };

        template<typename _Type>
        struct __is_pointer_helper<_Type*>
            : public bool_constant<!is_member_object_pointer<_Type *>::value
            && !is_member_function_pointer<_Type *>::value>
        { };

        template<typename _Type>
        struct is_pointer
            : public __is_pointer_helper<typename remove_cv<_Type>::type>::type
        { };

        /// TEMPLATE CLASS is_null_pointer

        template<typename _Type>
        struct is_null_pointer
            : public bool_constant<
            is_same<typename remove_cv<_Type>::type, nullptr_t>::value>::type
        { };

        /// TEMPLATE CLASS is_lvalue_reference

        template<typename>
        struct is_lvalue_reference
            : public false_type 
        { };

        template<typename _Type>
        struct is_lvalue_reference<_Type&>
            : public true_type 
        { };

        /// TEMPLATE CLASS is_rvalue_reference

        template<typename>
        struct is_rvalue_reference
            : public false_type 
        { };

        template<typename _Type>
        struct is_rvalue_reference<_Type&&>
            : public true_type 
        { };

        /// TEMPLATE CLASS is_reference

        template<typename _Type>
        struct is_reference
            : public bool_constant<is_lvalue_reference<_Type>::value
            || is_rvalue_reference<_Type>::value>
        { };

        /// TEMPLATE CLASS is_fundamental

        template<typename _Type>
        struct is_fundamental
            : public bool_constant <is_arithmetic<_Type>::value
            || is_void<_Type>::value
            || is_null_pointer<_Type>::value>
        { };

        /// TEMPLATE CLASS is_object

        template<typename _Type>
        struct is_object
            : public bool_constant<
            !bool_constant<is_function<_Type>::value
            || is_reference<_Type>::value
            || is_void<_Type>::value>::value>
        { };

        /// TEMPLATE CLASS is_member_pointer

        template<typename _Type>
        struct is_member_pointer
            : public bool_constant<is_member_object_pointer<_Type>::value
            || is_member_function_pointer<_Type>::value>
        { };

        /// TEMPLATE CLASS is_scalar

        template<typename _Type>
        struct is_scalar
            : public bool_constant<is_arithmetic<_Type>::value
            || is_enum<_Type>::value
            || is_pointer<_Type>::value
            || is_member_pointer<_Type>::value
            || is_null_pointer<_Type>::value>
        { };

        /// TEMPLATE CLASS is_compound 

        template<typename _Type>
        struct is_compound
            : public bool_constant<!is_fundamental<_Type>::value>
        { };

        /// TEMPLATE CLASS is_referenceable
        // Utility to detect referenceable types ([defns.referenceable]).

        template<typename _Type>
        struct is_referenceable
            : public disjunction<is_object<_Type>, is_reference<_Type>>
        { };

        template<typename _ReturnType, typename... _Args>
        struct is_referenceable<_ReturnType(_Args...)>
            : public true_type
        { };

        /// TEMPLATE CLASS is_const

        template<typename>
        struct is_const
            : public false_type
        { };

        template<typename _Type>
        struct is_const<const _Type>
            : public true_type
        { };

        /// TEMPLATE CLASS is_volatile

        template<typename _Type>
        struct is_volatile
            : false_type
        { };

        template<typename _Type>
        struct is_volatile<volatile _Type>
            : true_type
        { };

        /// TEMPLATE CLASS is_trivial

        template <typename _Type>
        struct is_trivial
            : public bool_constant<__is_trivial(_Type)>
        { };

        /// TEMPLATE CLASS is_trivially_copyable

        template <typename _Type>
        struct is_trivially_copyable
            : public bool_constant<__is_trivially_copyable(_Type)>
        { };

        /// TEMPLATE CLASS is_standard_layout

        template <typename _Type>
        struct is_standard_layout
            : public bool_constant<__is_standard_layout(_Type)>
        { };

        /// TEMPLATE CLASS is_pod

        template <typename _Type>
        struct is_pod
            : public bool_constant<__is_pod(_Type)>
        { };

        /// TEMPLATE CLASS is_literal_type

        template <typename _Type>
        struct is_literal_type
            : public bool_constant<__is_literal_type(_Type)>
        { };

        /// TEMPLATE CLASS is_empty

        template <typename _Type>
        struct is_empty
            : public bool_constant<__is_empty(_Type)>
        { };

        /// TEMPLATE CLASS is_polymorphic

        template <typename _Type>
        struct is_polymorphic
            : public bool_constant<__is_polymorphic(_Type)>
        { };

        /// TEMPLATE CLASS is_final

        template <typename _Type>
        struct is_final
            : public bool_constant<__is_final(_Type)>
        { };

        /// TEMPLATE CLASS is_abstract

        template <typename _Type>
        struct is_abstract
            : public bool_constant<__is_abstract(_Type)>
        { };

        /// TEMPLATE CLASS is_signed

        template<typename _Type, bool = is_arithmetic<_Type>::value>
        struct __is_signed_helper
            : public false_type
        { };

        template<typename _Type>
        struct __is_signed_helper<_Type, true>
            : public bool_constant < _Type(-1) < _Type(0) >
        { };

        template<typename _Type>
        struct is_signed
            : public __is_signed_helper<_Type>::type
        { };

        /// TEMPLATE CLASS is_unsigned

        template<typename _Type>
        struct is_unsigned
            : public bool_constant<is_arithmetic<_Type>::value
            && (!is_signed<_Type>::value)>
        { };

        /// TEMPLATE CLASS is_destructible

        template<typename _Type>
        struct is_destructible
            : public bool_constant<__is_destructible(_Type)>
        { };

        /// TEMPLATE CLASS is_nothrow_destructible

        template<typename _Type>
        struct is_nothrow_destructible
            : public bool_constant<__is_nothrow_destructible(_Type)>
        { };

        /// TEMPLATE CLASS is_constructible

        template<typename _Type, typename... _ArgTypes>
        struct is_constructible
            : public bool_constant<__is_constructible(_Type, _ArgTypes...)>
        { };

        /// TEMPLATE CLASS is_default_constructible

        template<typename _Type>
        struct is_default_constructible
            : is_constructible<_Type>::type
        { };

        /// TEMPLATE CLASS is_copy_constructible

        template<typename _Type>
        struct is_copy_constructible
            : is_constructible<
            _Type,
            typename add_lvalue_reference<
            typename add_const<_Type>::type
            >::type
            >::type
        { };

        /// TEMPLATE CLASS is_move_constructible

        template<class _Type>
        struct is_move_constructible
            : is_constructible<
            _Type,
            typename add_rvalue_reference<_Type>::type
            >::type
        { };

        /// TEMPLATE CLASS is_nothrow_constructible

        template<typename _Type, typename... _ArgTypes>
        struct is_nothrow_constructible
            : public bool_constant<__is_nothrow_constructible(_Type, _ArgTypes...)>
        { };

        /// TEMPLATE CLASS is_nothrow_default_constructible

        template<typename _Type>
        struct is_nothrow_default_constructible
            : is_nothrow_constructible<_Type>::type
        { };

        /// TEMPLATE CLASS is_nothrow_copy_constructible

        template<typename _Type>
        struct is_nothrow_copy_constructible
            : is_nothrow_constructible<
            _Type,
            typename add_lvalue_reference<
            typename add_const<_Type>::type
            >::type
            >::type
        { };

        /// TEMPLATE CLASS is_nothrow_move_constructible

        template<typename _Type>
        struct is_nothrow_move_constructible
            : is_nothrow_constructible<
            _Type,
            typename add_rvalue_reference<_Type>::type
            >::type
        { };

        /// TEMPLATE CLASS is_assignable

        template<typename _To, typename _From>
        struct is_assignable
            : public bool_constant<__is_assignable(_To, _From)>
        { };

        /// TEMPLATE CLASS is_copy_assignable

        template<typename _Type>
        struct is_copy_assignable
            : is_assignable<
            typename add_lvalue_reference<_Type>::type,
            typename add_lvalue_reference<
            typename add_const<_Type>::type>::type
            >::type
        { };

        /// TEMPLATE CLASS is_move_assignable

        template<typename _Type>
        struct is_move_assignable
            : is_assignable<
            typename add_lvalue_reference<_Type>::type,
            typename add_rvalue_reference<_Type>::type
            >::type
        { };

        /// TEMPLATE CLASS is_nothrow_assignable

        template<typename _To, typename _From>
        struct is_nothrow_assignable
            : public bool_constant<__is_nothrow_assignable(_To, _From)>
        { };

        /// TEMPLATE CLASS is_nothrow_copy_assignable

        template<typename _Type>
        struct is_nothrow_copy_assignable
            : is_nothrow_assignable<
            typename add_lvalue_reference<_Type>::type,
            typename add_lvalue_reference<
            typename add_const<_Type>::type
            >::type
            >::type
        { };

        /// TEMPLATE CLASS is_nothrow_move_assignable

        template<typename _Type>
        struct is_nothrow_move_assignable
            : is_nothrow_assignable<
            typename add_lvalue_reference<_Type>::type,
            typename add_rvalue_reference<_Type>::type
            >::type
        { };

        /// TEMPLATE CLASS is_trivially_constructible

        template<typename _Type, typename... _ArgTypes>
        struct is_trivially_constructible
            : public bool_constant<__is_trivially_constructible(_Type, _ArgTypes...)>
        { };

        /// TEMPLATE CLASS is_trivially_default_constructible

        template<typename _Type>
        struct is_trivially_default_constructible
            : is_trivially_constructible<_Type>::type
        { };

        /// TEMPLATE CLASS is_trivially_copy_constructible

        template<typename _Type>
        struct is_trivially_copy_constructible
            : is_trivially_constructible<
            _Type,
            typename add_lvalue_reference<
            typename add_const<_Type>::type
            >::type
            >::type
        { };

        /// TEMPLATE CLASS is_trivially_move_constructible

        template<typename _Type>
        struct is_trivially_move_constructible
            : is_trivially_constructible<
            _Type,
            typename add_rvalue_reference<_Type>::type
            >::type
        { };

        /// TEMPLATE CLASS is_trivially_assignable

        template<typename _To, typename _From>
        struct is_trivially_assignable
            : public bool_constant<__is_trivially_assignable(_To, _From)>
        { };

        /// TEMPLATE CLASS is_trivially_copy_assignable

        template<typename _Type>
        struct is_trivially_copy_assignable
            : is_trivially_assignable<
            typename add_lvalue_reference<_Type>::type,
            typename add_lvalue_reference<
            typename add_const<_Type>::type
            >::type
            >::type
        { };

        /// TEMPLATE CLASS is_trivially_move_assignable

        template<typename _Type>
        struct is_trivially_move_assignable
            : is_trivially_assignable<
            typename add_lvalue_reference<_Type>::type,
            typename add_rvalue_reference<_Type>::type
            >::type
        { };

        /// TEMPLATE CLASS is_trivially_destructible

        template<typename _Type>
        struct is_trivially_destructible
            : public bool_constant<__has_trivial_destructor(_Type)>
        { };


        /// TEMPLATE CLASS has_trivial_default_constructor (temporary legacy)
        template<typename _Type>
        struct has_trivial_default_constructor
            : public bool_constant<__has_trivial_constructor(_Type)>
        { };

        /// TEMPLATE CLASS has_trivial_copy_constructor (temporary legacy)
        template<typename _Type>
        struct has_trivial_copy_constructor
            : public bool_constant<__has_trivial_copy(_Type)>
        { };

        /// TEMPLATE CLASS has_trivial_copy_assign (temporary legacy)
        template<typename _Type>
        struct has_trivial_copy_assign
            : public bool_constant<__has_trivial_assign(_Type)>
        { };

        /// TEMPLATE CLASS has_virtual_destructor
        template<typename _Type>
        struct has_virtual_destructor
            : public bool_constant<__has_virtual_destructor(_Type)>
        { };

        /// TEMPLATE CLASS alignment_of

        template<typename _Type>
        struct alignment_of
            : public integral_constant<usize, alignof(_Type)>
        { };

        /// TEMPLATE CLASS rank

        template<typename _Type>
        struct rank
            : public integral_constant<usize, 0>
        { };

        template<typename _Type, usize _Size>
        struct rank<_Type[_Size]>
            : public integral_constant<usize, rank<_Type>::value + 1>
        { };

        template<typename _Type>
        struct rank<_Type[]>
            : public integral_constant<usize, rank<_Type>::value + 1>
        { };

        /// TEMPLATE CLASS extent
        template<typename, u32 _Nx = 0>
        struct extent
            : public integral_constant<usize, 0>
        { };

        template<typename _Type, u32 _Nx, usize _Size>
        struct extent<_Type[_Size], _Nx>
            : public integral_constant<usize,
            _Nx == 0 ? _Size : extent<_Type,
            _Nx - 1>::value>
        { };

        template<typename _Type, u32 _Nx>
        struct extent<_Type[], _Nx>
            : public integral_constant<usize,
            _Nx == 0 ? 0 : extent<_Type,
            _Nx - 1>::value>
        { };

        /// TEMPLATE CLASS is_base_of

        template<typename _Base, typename _Derived>
        struct is_base_of
            : public bool_constant<__is_base_of(_Base, _Derived)>
        {	// determine whether _Base is a base of or the same as _Der
        };

        /// TEMPLATE CLASS is_convertible

        template<typename _From, typename _To>
        struct is_convertible
            : public bool_constant<__is_convertible_to(_From, _To)>
        {	// determine whether _From is convertible to _To
        };

        /// TEMPLATE CLASS make_unsigned & make_signed

        template<typename _Type>
        struct __change_sign
        {
            // signed/unsigned partners to _Type

            static_assert(
                ((is_integral<_Type>::value || is_enum<_Type>::value)
                    && !is_same<_Type, bool>::value),
                "make_signed<T>/make_unsigned<T> require that T shall be a (possibly"
                " cv-qualified) integral type or enumeration but not a bool type.");

            using __signed =
                typename __if<is_same<_Type, signed char>::value
                || is_same<_Type, unsigned char     >::value, signed char,
                typename __if<is_same<_Type, short      >::value
                || is_same<_Type, unsigned short    >::value, short,
                typename __if<is_same<_Type, int        >::value
                || is_same<_Type, unsigned int      >::value, int,
                typename __if<is_same<_Type, long       >::value
                || is_same<_Type, unsigned long     >::value, long,
                typename __if<is_same<_Type, long long  >::value
                || is_same<_Type, unsigned long long>::value, long long,
                typename __if<sizeof(_Type) == sizeof(signed char), signed char,
                typename __if<sizeof(_Type) == sizeof(short), short,
                typename __if<sizeof(_Type) == sizeof(int), int,
                typename __if<sizeof(_Type) == sizeof(long), long,
                long long
                >::type>::type>::type>::type>::type>::type>::type>::type>::type;

            using __unsigned =
                typename __if<is_same<__signed, signed char >::value, unsigned char,
                typename __if<is_same<__signed, short       >::value, unsigned short,
                typename __if<is_same<__signed, int         >::value, unsigned int,
                typename __if<is_same<__signed, long        >::value, unsigned long,
                unsigned long long
                >::type>::type>::type>::type;
        };

        template<typename _Type>
        struct __change_sign<const _Type>
        {
            using __signed = const typename __change_sign<_Type>::__signed;
            using __unsigned = const typename __change_sign<_Type>::__unsigned;
        };

        template<typename _Type>
        struct __change_sign<volatile _Type>
        {
            using __signed = volatile typename __change_sign<_Type>::__signed;
            using __unsigned = volatile typename __change_sign<_Type>::__unsigned;
        };

        template<typename _Type>
        struct __change_sign<const volatile _Type>
        {
            using __signed = const volatile typename __change_sign<_Type>::__signed;
            using __unsigned = const volatile typename __change_sign<_Type>::__unsigned;
        };

        template<typename _Type>
        struct make_signed
        {
            using type = typename __change_sign<_Type>::__signed;
        };

        template<typename _Type>
        struct make_unsigned
        {
            using type = typename __change_sign<_Type>::__unsigned;
        };

        /// TEMPLATE CLASS remove_extent

        template<typename _Type>
        struct remove_extent
        {	// remove array extent
            using type = _Type;
        };

        template<typename _Type, usize _Size>
        struct remove_extent<_Type[_Size]>
        {	// remove array extent
            using type = _Type;
        };

        template<typename _Type>
        struct remove_extent<_Type[]>
        {	// remove array extent
            using type = _Type;
        };

        // TEMPLATE CLASS remove_all_extents
        template<typename _Type>
        struct remove_all_extents
        {	// remove all array extents
            using type = _Type;
        };

        template<typename _Type, usize _Size>
        struct remove_all_extents<_Type[_Size]>
        {	// remove all array extents
            using type = typename remove_all_extents<_Type>::type;
        };

        template<class _Type>
        struct remove_all_extents<_Type[]>
        {	// remove all array extents
            using type = typename remove_all_extents<_Type>::type;
        };

        /// TEMPLATE CLASS remove_pointer

        template<typename _Type, typename>
        struct __remove_pointer_helper
        {
            using type = _Type;
        };

        template<typename _Type, typename _UType>
        struct __remove_pointer_helper<_Type, _UType*>
        {
            using type = _UType;
        };

        template<typename _Type>
        struct remove_pointer
            : public __remove_pointer_helper<_Type, typename remove_cv<_Type>::type>
        { };

        /// TEMPLATE CLASS add_pointer

        template<typename _Type, bool = bool_constant<is_referenceable<_Type>::value
            || is_void<_Type>::value>::value>
            struct __add_pointer_helper
        {
            using type = _Type;
        };

        template<typename _Type>
        struct __add_pointer_helper<_Type, true>
        {
            using type = typename remove_reference<_Type>::type*;
        };

        template<typename _Type>
        struct add_pointer
            : public __add_pointer_helper<_Type>
        { };

        /// TEMPLATE CLASS aligned_storage

        template<typename _Type, usize _Len>
        union __align_type
        {
            _Type _Val;
            char _Pad[_Len];
        };

        template<usize _Len, usize _Align, typename _Type, bool>
        struct __aligned_storage_helper;

        template<usize _Len, usize _Align, typename _Type>
        struct __aligned_storage_helper<_Len, _Align, _Type, true>
        {
            using type = __align_type<_Type, _Len>;
        };

        template<usize _Len, usize _Align>
        struct __aligned_storage_helper<_Len, _Align, f64, false>
        {
            using type = __align_type<max_align_t, _Len>;
        };

        template<usize _Len, usize _Align>
        struct __aligned_storage_helper<_Len, _Align, i32, false>
        {
            using type = typename __aligned_storage_helper<
                _Len, _Align, f64, (_Align <= alignment_of<f64>::value)>::type;
        };

        template<usize _Len, usize _Align>
        struct __aligned_storage_helper<_Len, _Align, i16, false>
        {
            using type = typename __aligned_storage_helper<
                _Len, _Align, i32, (_Align <= alignment_of<i32>::value)>::type;
        };

        template<usize _Len, usize _Align>
        struct __aligned_storage_helper<_Len, _Align, i8, false>
        {
            using type = typename __aligned_storage_helper<
                _Len, _Align, i16, (_Align <= alignment_of<i16>::value)>::type;
        };

        template<usize _Len, usize _Align = alignment_of<max_align_t>::value>
        struct aligned_storage
        {
            using type = typename __aligned_storage_helper<
                _Len, _Align, i8, (_Align <= alignment_of<i8>::value)>::type;
        };

        /// TEMPLATE CLASS aligned_union

        template <typename... _Types>
        struct __strictest_alignment
        {
            static const usize __alignment = 0;
            static const usize __size = 0;
        };

        template <typename _Type, typename... _Types>
        struct __strictest_alignment<_Type, _Types...>
        {
            static const usize __alignment =
                alignof(_Type) > __strictest_alignment<_Types...>::__alignment
                ? alignof(_Type) : __strictest_alignment<_Types...>::__alignment;

            static const usize __size =
                sizeof(_Type) > __strictest_alignment<_Types...>::__size
                ? sizeof(_Type) : __strictest_alignment<_Types...>::__size;
        };

        template <usize _Len, typename... _Types>
        struct aligned_union
        {
        private:
            static_assert(sizeof...(_Types) != 0, "At least one type is required");

            using __strictest = __strictest_alignment<_Types...>;
            static const usize __len = _Len > __strictest::__size
                ? _Len : __strictest::__size;
        public:
            /// The value of the strictest alignment of _Types.
            static const usize alignment_value = __strictest::__alignment;
            /// The storage.
            using type = typename aligned_storage<__len, alignment_value>::type;
        };

        template <usize _Len, typename... _Types>
        const usize aligned_union<_Len, _Types...>::alignment_value;

        /// TEMPLATE CLASS decay

        template<typename _Type>
        class decay
        {
            using __remove_type = typename remove_reference<_Type>::type;

        public:
            using type = typename __if<is_array<__remove_type>::value,
                typename remove_extent<__remove_type>::type *,
                typename __if<is_function<__remove_type>::value,
                typename add_pointer<__remove_type>::type,
                typename remove_cv<__remove_type>::type
                >::type
            >::type;
        };

        /// TEMPLATE CLASS common_type

        template<typename... _Type>
        struct common_type;

        template<typename _Type>
        struct common_type<_Type>
        {	// type is typename decay<_Ty>::type for one argument
            using type = typename decay<_Type>::type;
        };

        template<typename _Type1, typename _Type2>
        struct common_type<_Type1, _Type2>
        {	// type is common type of _Ty0 and _Ty1 for two arguments
            using type = typename decay<
                decltype(always_false<_Type1>::value ?
                    declval<_Type1>() : declval<_Type2>())
            >::type;
        };

        template<typename _Type1, typename _Type2, typename... _Types>
        struct common_type<_Type1, _Type2, _Types...>
        {	// type is defined recursively for more than two arguments
            using type = typename common_type<
                typename common_type<_Type1, _Type2>::type, _Types...
            >::type;
        };

        /// TEMPLATE CLASS underlying_type
        template<typename _Type>
        struct underlying_type
        {	// determine type underlying type for enum
            using type = __underlying_type(_Type);
        };

        template<typename _Type>
        struct __declval_protector
        {
            static constexpr bool __stop = false;
            static typename add_rvalue_reference<_Type>::type __delegate();
        };

        template<typename _Type>
        inline typename add_rvalue_reference<_Type>::type
            declval() NOEXCEPT$TYPE
        {
            static_assert(__declval_protector<_Type>::__stop,
                "declval() must not be used!");

            return __declval_protector<_Type>::__delegate();
        }

        /// TEMPLATE CLASS result_of

        template<class>
        struct result_of;

        template<class F, class... ArgTypes>
        struct result_of<F(ArgTypes...)>
        {
            using type = decltype(declval<F>()(declval<ArgTypes>()...));
        };

        template<class F>
        struct result_of
        {	// explain usage
            static_assert(always_false<F>::value,
                "result_of<CallableType> is invalid; use "
                "result_of<CallableType(zero or more argument types)> instead.");
        };

        /// TEMPLATE STRUCT make_void

        template<typename... Types>
        struct make_void
        {
            using type = void;
        };

        template<typename _Type>
        using make_not_void = typename conditional<is_void<_Type>::value, _undefined, _Type>::type;

        template<typename... Types>
        using void_t = typename make_void<Types...>::type;

        /// __detected_or_t_

        /// Implementation of the detection idiom (negative case).
        template<typename _Default, typename _AlwaysVoid,
            template<typename...> class _Op, typename... _ArgTypes>
        struct __detector
        {
            using value_t = false_type;
            using type = _Default;
        };

        /// Implementation of the detection idiom (positive case).
        template<typename _Default, template<typename...> class _Op,
            typename... _ArgTypes>
            struct __detector<_Default, void_t<_Op<_ArgTypes...>>, _Op, _ArgTypes...>
        {
            using value_t = true_type;
            using type = _Op<_ArgTypes...>;
        };

        // Detect whether _Op<_Args...> is a valid type, use _Default if not.
        template<typename _Default, template<typename...> class _Op,
            typename... _ArgTypes>
            using __detected_or = __detector<_Default, void, _Op, _ArgTypes...>;

        // _Op<_Args...> if that is a valid type, otherwise _Default.
        template<typename _Default, template<typename...> class _Op,
            typename... _ArgTypes>
            using __detected_or_t = typename __detected_or<_Default, _Op, _ArgTypes...>::type;

        // _Op<_Args...> if that is a valid type, otherwise _Default<_Args...>.
        template<template<typename...> class _Default,
            template<typename...> class _Op, typename... _ArgTypes>
        using __detected_or_t_ =
            typename __detected_or_t<_Default<_ArgTypes...>, _Op, _ArgTypes...>;
    }

}

#endif