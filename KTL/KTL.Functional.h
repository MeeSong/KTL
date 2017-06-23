#pragma once
#ifndef Functional_$B29751E0_A639_4327_A0D5_6EEAA6DB1E31
#define Functional_$B29751E0_A639_4327_A0D5_6EEAA6DB1E31 1

#include "KTL.Type.Uitility.h"

namespace ktl
{
    inline namespace functional
    {
        /// TEMPLATE FUNCTION invoke

        // TEMPLATE struct Tags
        template<
            typename _ReturnType, 
            bool = is_void<_ReturnType>::value>
        struct _Forced
        {
            // tag to give INVOKE an explicit return type
        };

        struct _Unforced
        {
            // tag to distinguish bind() from bind<R>()
        };

        struct _Invoker_member_function_pointer_object
        {
            // INVOKE a pointer to member function on an object
            template<
                typename _Functor_decay,
                typename _Class,
                typename... _ArgTypes>
                static auto call(
                    _Functor_decay aFunctor,
                    _Class&& aObject,
                    _ArgTypes&&... aArguments)
                -> decltype((forward<_Class>(aObject).*aFunctor)(
                    forward<_ArgTypes>(aArguments)...))
            {
                return ((forward<_Class>(aObject).*aFunctor)(
                    forward<_ArgTypes>(aArguments)...));
            }
        };

        struct _Invoker_member_function_pointer_object_ptr
        {
            // INVOKE a pointer to member function on a [smart] pointer
            template<
                typename _Functor_decay,
                typename _Class,
                typename... _ArgTypes>
                static auto call(
                    _Functor_decay aFunctor,
                    _Class&& aObject,
                    _ArgTypes&&... aArguments)
                -> decltype(((*forward<_Class>(aObject)).*aFunctor)(
                    forward<_ArgTypes>(aArguments)...))
            {
                return (((*forward<_Class>(aObject)).*aFunctor)(
                    forward<_ArgTypes>(aArguments)...));
            }
        };

        struct _Invoker_member_data_pointer_object
        {
            // INVOKE a pointer to member data on an object
            template<typename _Data_decay, typename _Class>
            static auto call(_Data_decay aData, _Class&& aObject)
                -> decltype(forward<_Class>(aObject).*aData)
            {
                return (forward<_Class>(aObject).*aData);
            }
        };

        struct _Invoker_member_data_pointer_object_ptr
        {
            // INVOKE a pointer to member data on a [smart] pointer
            template<typename _Data_decay, typename _Class>
            static auto call(_Data_decay aData, _Class&& aObject)
                -> decltype((*forward<_Class>(aObject)).*aData)
            {
                return ((*forward<_Class>(aObject)).*aData);
            }
        };

        struct _Invoker_functor
        {
            template<typename _Callable, typename... _ArgTypes>
            static auto call(_Callable&& aCallable, _ArgTypes&&... aArguments)
                -> decltype(forward<_Callable>(aCallable)(
                    forward<_ArgTypes>(aArguments)...))
            {
                return (forward<_Callable>(aCallable)(
                    forward<_ArgTypes>(aArguments)...));
            }
        };

        // TEMPLATE STRUCT Invoker
        template<
            typename _Callable,
            typename _ArgTypes,
            typename _Decayed = typename decay<_Callable>::type,
            bool _Is_memfunc_ptr = is_member_function_pointer<_Decayed>::value,
            bool _Is_memdata_ptr = is_member_object_pointer<_Decayed>::value>
            struct _Invoker_impl;

        template<
            typename _Callable,
            typename _ArgTypes,
            typename _Decayed>
            struct _Invoker_impl<_Callable, _ArgTypes, _Decayed, true, false>
            : __if<is_base_of<
            typename __is_member_function_pointer_impl<_Decayed>::__class_type,
            typename decay<_ArgTypes>::type>::value,
            _Invoker_member_function_pointer_object,
            _Invoker_member_function_pointer_object_ptr>::type
        {   // pointer to member function
        };

        template<
            typename _Callable,
            typename _ArgTypes,
            typename _Decayed>
            struct _Invoker_impl<_Callable, _ArgTypes, _Decayed, false, true>
            : __if<is_base_of<
            typename __is_member_object_pointer_impl<_Decayed>::__class_type,
            typename decay<_ArgTypes>::type>::value,
            _Invoker_member_data_pointer_object,
            _Invoker_member_data_pointer_object_ptr>::type
        {   // pointer to member data
        };

        template<
            typename _Callable,
            typename _ArgTypes,
            typename _Decayed>
            struct _Invoker_impl<_Callable, _ArgTypes, _Decayed, false, false>
            : _Invoker_functor
        {   // function object
        };

        template<typename _Callable, typename... _ArgTypes>
        struct _Invoker;

        template<typename _Callable>
        struct _Invoker<_Callable>
            : _Invoker_functor
        {   // zero arguments
        };

        template<typename _Callable, typename _ArgType, typename... _ArgTypes>
        struct _Invoker<_Callable, _ArgType, _ArgTypes...>
            : _Invoker_impl<_Callable, _ArgType>
        {   // one or more arguments
        };

        // TEMPLATE FUNCTION invoke
        template<typename _Callable, typename... _ArgTypes>
        inline auto invoke(_Callable&& aCallable, _ArgTypes&&... aArguments)
            -> decltype(_Invoker<_Callable, _ArgTypes...>::call(
                forward<_Callable>(aCallable),
                forward<_ArgTypes>(aArguments)...))
        {
            // INVOKE a callable object
            return (_Invoker<_Callable, _ArgTypes...>::call(
                forward<_Callable>(aCallable),
                forward<_ArgTypes>(aArguments)...));
        }

        template<typename _Const_volatile_void, typename... _ArgTypes>
        inline void _Invoke_ret(
            _Forced<_Const_volatile_void, true>,
            _ArgTypes&&... aArguments)
        {
            // INVOKE, "implicitly" converted to void
            invoke(forward<_ArgTypes>(aArguments)...);
        }

        template<typename _ReturnType, typename... _ArgTypes>
        inline _ReturnType _Invoke_ret(
            _Forced<_ReturnType, false>,
            _ArgTypes&&... aArguments)
        {
            // INVOKE, implicitly converted to _Rx
            return (invoke(forward<_ArgTypes>(aArguments)...));
        }

        template<typename... _ArgTypes>
        inline auto _Invoke_ret(
            _Forced<_Unforced, false>,
            _ArgTypes&&... aArguments)
            -> decltype(invoke(forward<_ArgTypes>(aArguments)...))
        {
            // INVOKE, unchanged
            return (invoke(forward<_ArgTypes>(aArguments)...));
        }

        /// TEMPLATE STRUCT __weak_type

        template<typename _Type, typename = void>
        struct __weak_result_type
        { };

        template<typename _Type>
        struct __weak_result_type<_Type,
            void_t<typename _Type::return_type>> // test if parameters are valid
        {
            // defined if _Ty::return_type exists
            using return_type = typename _Type::return_type;
        };

        template<typename _Type, typename = void>
        struct __weak_argument_type
            : __weak_result_type<_Type>
        { };

        template<typename _Type>
        struct __weak_argument_type<_Type,
            void_t<typename _Type::argument_type>>
            : __weak_result_type<_Type>
        {
            using argument_type = typename _Type::argument_type;
        };

        template<typename _Type, typename = void>
        struct __weak_binary_arguments
            : __weak_argument_type<_Type>
        { };

        template<typename _Type>
        struct __weak_binary_arguments<_Type, void_t<
            typename _Type::first_argument_type,
            typename _Type::second_argument_type>>
            : __weak_argument_type<_Type>
        {
            using first_argument_type = typename _Type::first_argument_type;
            using second_argument_type = typename _Type::second_argument_type;
        };

        template<typename _Type>
        struct __weak_types
        {
            using __is_f_or_pf = is_function<
                typename remove_pointer<_Type>::type>;

            using __is_pmf = is_member_function_pointer<
                typename remove_cv<_Type>::type>;

            using type = typename __if<__is_f_or_pf::value, __is_f_or_pf,
                typename __if<__is_pmf::value, __is_pmf,
                __weak_binary_arguments<_Type>>::type>::type;
        };

        /// TEMPLATE CLASS reference_wrapper

        template<typename _Type>
        class reference_wrapper
            : public __weak_types<_Type>::type
        {
        public:
            static_assert(is_object<_Type>::value
                || is_function<_Type>::value,
                "reference_wrapper<T> requires T to be an object type "
                "or a function type.");

            using type = _Type;

            reference_wrapper(_Type& aValue) NOEXCEPT$TYPE
                : m_Pointer(addressof(aValue))
            { }

            operator _Type&() const NOEXCEPT$TYPE
            {
                return (*m_Pointer);
            }

            _Type& get() const NOEXCEPT$TYPE
            {
                return (*m_Pointer);
            }

            template<typename... _Types>
            auto operator()(_Types&&... aArguments) const
                -> decltype(invoke(get(), forward<_Types>(aArguments)...))
            {
                // invoke object/function
                return (invoke(get(), forward<_Types>(aArguments)...));
            }

            reference_wrapper(_Type&&) = delete;

        private:
            _Type *m_Pointer;
        };

        /// TEMPLATE FUNCTIONS ref AND cref

        template<typename _Type>
        inline reference_wrapper<_Type> ref(_Type& aValue) NOEXCEPT$TYPE
        {
            return (reference_wrapper<_Type>(aValue));
        }

        template<typename _Type>
        void ref(const _Type&&) = delete;

        template<typename _Type>
        inline reference_wrapper<_Type> ref(reference_wrapper<_Type> aValue) NOEXCEPT$TYPE
        {
            return (ref(aValue.get()));
        }

        template<typename _Type>
        inline reference_wrapper<const _Type> cref(const _Type& aValue) NOEXCEPT$TYPE
        {
            return (reference_wrapper<const _Type>(aValue));
        }

        template<typename _Type>
        void cref(const _Type&&) = delete;

        template<typename _Type>
        inline reference_wrapper<const _Type> cref(reference_wrapper<_Type> aValue) NOEXCEPT$TYPE
        {
            return (cref(aValue.get()));
        }

        /// TEMPLATE CLASS unrefwrap

        template<typename _Type>
        struct __unrefwrap_helper
        {
            using type = _Type;
            static constexpr bool __is_refwrap = false;
        };

        template<typename _Type>
        struct __unrefwrap_helper<reference_wrapper<_Type>>
        {
            using type = _Type&;
            static constexpr bool __is_refwrap = true;
        };

        template<typename _Type>
        struct unrefwrap
            : public __unrefwrap_helper<typename decay<_Type>::type>
        { };


        /// TEMPLATE FUNCTION mem_fn
        template<typename _MemPtr>
        class __mem_fn
            : public __weak_types<_MemPtr>::type
        {
            // wrap a pointer to member function/data

        private:
            _MemPtr m_MemberPtr;

        public:
            explicit __mem_fn(_MemPtr aValue) NOEXCEPT$TYPE
                : m_MemberPtr(aValue)
            { }

            template<typename... _Types>
            auto operator()(_Types&&... aArguments) const
                -> decltype(invoke(m_MemberPtr, forward<_Types>(aArguments)...))
            {
                return (invoke(m_MemberPtr, forward<_Types>(aArguments)...));
            }
        };

        template<typename _Member, typename _Class>
        inline __mem_fn<_Member _Class::*> mem_fn(
            _Member _Class::* aMemberPtr) NOEXCEPT$TYPE
        {
            return (__mem_fn<_Member _Class::*>);
        }


        /// TEMPLATE STRUCT unary_function & binary_function

        template<typename _ArgType, typename _ReturnType>
        struct unary_function
        {	// base class for unary functions
            using argument_type = _ArgType;
            using return_type = _ReturnType;
        };

        // TEMPLATE STRUCT binary_function
        template<typename _ArgType1, typename _ArgType2, typename _ReturnType>
        struct binary_function
        {	// base class for binary functions
            using first_argument_type = _ArgType1;
            using second_argument_type = _ArgType2;
            using return_type = _ReturnType;
        };


        /// Arithmetic operations

        struct __is_transparent; // undefined

        template<typename _Type = void>
        struct plus : public binary_function<_Type, _Type, _Type>
        {
            constexpr _Type operator()(const _Type& aLeft,
                const _Type& aRight) const
            {
                return (aLeft + aRight);
            }
        };

        template<typename _Type = void>
        struct minus : public binary_function<_Type, _Type, _Type>
        {
            constexpr _Type operator()(const _Type& aLeft,
                const _Type& aRight) const
            {
                return (aLeft - aRight);
            }
        };

        template<typename _Type = void>
        struct multiplies : public binary_function<_Type, _Type, _Type>
        {
            constexpr _Type operator()(const _Type& aLeft,
                const _Type& aRight) const
            {
                return (aLeft * aRight);
            }
        };

        template<typename _Type = void>
        struct divides : public binary_function<_Type, _Type, _Type>
        {
            constexpr _Type operator()(const _Type& aLeft,
                const _Type& aRight) const
            {
                return (aLeft / aRight);
            }
        };

        template<typename _Type = void>
        struct modulus : public binary_function<_Type, _Type, _Type>
        {
            constexpr _Type operator()(const _Type& aLeft,
                const _Type& aRight) const
            {
                return (aLeft % aRight);
            }
        };

        template<typename _Type = void>
        struct negate : public unary_function<_Type, _Type>
        {
            constexpr _Type operator()(const _Type& aValue) const
            {
                return (-aValue);
            }
        };

        template<>
        struct plus<void>
        {
            using is_transparent = __is_transparent;

            template<typename _Type1, typename _Type2>
            constexpr auto operator()(_Type1&& aLeft, _Type2&& aRight) const
                -> decltype(forward<_Type1>(aLeft) +
                    forward<_Type2>(aRight))
            {
                return (forward<_Type1>(aLeft) +
                    forward<_Type2>(aRight));
            }
        };

        template<>
        struct minus<void>
        {
            using is_transparent = __is_transparent;

            template<typename _Type1, typename _Type2>
            constexpr auto operator()(_Type1&& aLeft, _Type2&& aRight) const
                -> decltype(forward<_Type1>(aLeft) -
                    forward<_Type2>(aRight))
            {
                return (forward<_Type1>(aLeft) -
                    forward<_Type2>(aRight));
            }
        };

        template<>
        struct multiplies<void>
        {
            using is_transparent = __is_transparent;

            template<typename _Type1, typename _Type2>
            constexpr auto operator()(_Type1&& aLeft, _Type2&& aRight) const
                -> decltype(forward<_Type1>(aLeft) *
                    forward<_Type2>(aRight))
            {
                return (forward<_Type1>(aLeft) *
                    forward<_Type2>(aRight));
            }
        };

        template<>
        struct divides<void>
        {
            using is_transparent = __is_transparent;

            template<typename _Type1, typename _Type2>
            constexpr auto operator()(_Type1&& aLeft, _Type2&& aRight) const
                -> decltype(forward<_Type1>(aLeft) /
                    forward<_Type2>(aRight))
            {
                return (forward<_Type1>(aLeft) /
                    forward<_Type2>(aRight));
            }
        };

        template<>
        struct modulus<void>
        {
            using is_transparent = __is_transparent;

            template<typename _Type1, typename _Type2>
            constexpr auto operator()(_Type1&& aLeft, _Type2&& aRight) const
                -> decltype(forward<_Type1>(aLeft) %
                    forward<_Type2>(aRight))
            {
                return (forward<_Type1>(aLeft) %
                    forward<_Type2>(aRight));
            }
        };

        template<>
        struct negate<void>
        {
            using is_transparent = __is_transparent;

            template<typename _Type>
            constexpr auto operator()(_Type&& aValue) const
                -> decltype(-forward<_Type>(aValue))
            {
                return (-forward<_Type>(aValue));
            }
        };


        /// Comparisons

        template<typename _Type = void>
        struct equal_to : public binary_function<_Type, _Type, bool>
        {
            constexpr bool operator()(const _Type& aLeft,
                const _Type& aRight) const
            {
                return (aLeft == aRight);
            }
        };

        template<typename _Type = void>
        struct not_equal_to : public binary_function<_Type, _Type, bool>
        {
            constexpr bool operator()(const _Type& aLeft,
                const _Type& aRight) const
            {
                return (aLeft != aRight);
            }
        };

        template<typename _Type = void>
        struct greater : public binary_function<_Type, _Type, bool>
        {
            constexpr bool operator()(const _Type& aLeft,
                const _Type& aRight) const
            {
                return (aLeft > aRight);
            }
        };

        template<typename _Type = void>
        struct less : public binary_function<_Type, _Type, bool>
        {
            constexpr bool operator()(const _Type& aLeft,
                const _Type& aRight) const
            {
                return (aLeft < aRight);
            }
        };

        template<typename _Type = void>
        struct greater_equal : public binary_function<_Type, _Type, bool>
        {
            constexpr bool operator()(const _Type& aLeft,
                const _Type& aRight) const
            {
                return (aLeft >= aRight);
            }
        };

        template<typename _Type = void>
        struct less_equal : public binary_function<_Type, _Type, bool>
        {
            constexpr bool operator()(const _Type& aLeft,
                const _Type& aRight) const
            {
                return (aLeft <= aRight);
            }
        };

        template<>
        struct equal_to<void>
        {
            using is_transparent = __is_transparent;

            template<typename _Type1, typename _Type2>
            constexpr auto operator()(_Type1&& aLeft, _Type2&& aRight) const
                -> decltype(forward<_Type1>(aLeft) == 
                    forward<_Type2>(aRight))
            {
                return (forward<_Type1>(aLeft) ==
                    forward<_Type2>(aRight));
            }
        };

        template<>
        struct not_equal_to<void>
        {
            using is_transparent = __is_transparent;

            template<typename _Type1, typename _Type2>
            constexpr auto operator()(_Type1&& aLeft, _Type2&& aRight) const
                -> decltype(forward<_Type1>(aLeft) !=
                    forward<_Type2>(aRight))
            {
                return (forward<_Type1>(aLeft) !=
                    forward<_Type2>(aRight));
            }
        };

        template<>
        struct greater<void>
        {
            using is_transparent = __is_transparent;

            template<typename _Type1, typename _Type2>
            constexpr auto operator()(_Type1&& aLeft, _Type2&& aRight) const
                -> decltype(forward<_Type1>(aLeft) >
                    forward<_Type2>(aRight))
            {
                return (forward<_Type1>(aLeft) >
                    forward<_Type2>(aRight));
            }
        };

        template<>
        struct less<void>
        {
            using is_transparent = __is_transparent;

            template<typename _Type1, typename _Type2>
            constexpr auto operator()(_Type1&& aLeft, _Type2&& aRight) const
                -> decltype(forward<_Type1>(aLeft) <
                    forward<_Type2>(aRight))
            {
                return (forward<_Type1>(aLeft) <
                    forward<_Type2>(aRight));
            }
        };

        template<>
        struct greater_equal<void>
        {
            using is_transparent = __is_transparent;

            template<typename _Type1, typename _Type2>
            constexpr auto operator()(_Type1&& aLeft, _Type2&& aRight) const
                -> decltype(forward<_Type1>(aLeft) >=
                    forward<_Type2>(aRight))
            {
                return (forward<_Type1>(aLeft) >=
                    forward<_Type2>(aRight));
            }
        };

        template<>
        struct less_equal<void>
        {
            using is_transparent = __is_transparent;

            template<typename _Type1, typename _Type2>
            constexpr auto operator()(_Type1&& aLeft, _Type2&& aRight) const
                -> decltype(forward<_Type1>(aLeft) <=
                    forward<_Type2>(aRight))
            {
                return (forward<_Type1>(aLeft) <=
                    forward<_Type2>(aRight));
            }
        };


        /// logical operations

        template<typename _Type = void>
        struct logical_and : public binary_function<_Type, _Type, bool>
        {
            constexpr bool operator()(const _Type& aLeft,
                const _Type& aRight) const
            {
                return (aLeft && aRight);
            }
        };

        template<typename _Type = void>
        struct logical_or : public binary_function<_Type, _Type, bool>
        {
            constexpr bool operator()(const _Type& aLeft,
                const _Type& aRight) const
            {
                return (aLeft || aRight);
            }
        };

        template<typename _Type = void>
        struct logical_not : public unary_function<_Type, bool>
        {
            constexpr bool operator()(const _Type& aValue) const
            {
                return (!aValue);
            }
        };

        template<>
        struct logical_and<void>
        {
            using is_transparent = __is_transparent;

            template<typename _Type1, typename _Type2>
            constexpr auto operator()(_Type1&& aLeft, _Type2&& aRight) const
                -> decltype(forward<_Type1>(aLeft) &&
                    forward<_Type2>(aRight))
            {
                return (forward<_Type1>(aLeft) &&
                    forward<_Type2>(aRight));
            }
        };

        template<>
        struct logical_or<void>
        {
            using is_transparent = __is_transparent;

            template<typename _Type1, typename _Type2>
            constexpr auto operator()(_Type1&& aLeft, _Type2&& aRight) const
                -> decltype(forward<_Type1>(aLeft) ||
                    forward<_Type2>(aRight))
            {
                return (forward<_Type1>(aLeft) ||
                    forward<_Type2>(aRight));
            }
        };

        template<>
        struct logical_not<void>
        {
            using is_transparent = __is_transparent;

            template<typename _Type>
            constexpr auto operator()(_Type&& aValue) const
                -> decltype(!forward<_Type>(aValue))
            {
                return (!forward<_Type>(aValue));
            }
        };


        /// Bitwise operations

        template<typename _Type = void>
        struct bit_and : public binary_function<_Type, _Type, _Type>
        {
            constexpr _Type operator()(const _Type& aLeft,
                const _Type& aRight) const
            {
                return (aLeft & aRight);
            }
        };

        template<typename _Type = void>
        struct bit_or : public binary_function<_Type, _Type, _Type>
        {
            constexpr _Type operator()(const _Type& aLeft,
                const _Type& aRight) const
            {
                return (aLeft | aRight);
            }
        };

        template<typename _Type = void>
        struct bit_xor : public binary_function<_Type, _Type, _Type>
        {
            constexpr _Type operator()(const _Type& aLeft,
                const _Type& aRight) const
            {
                return (aLeft ^ aRight);
            }
        };

        template<typename _Type = void>
        struct bit_not : public unary_function<_Type, _Type>
        {
            constexpr _Type operator()(const _Type& aValue) const
            {
                return (~aValue);
            }
        };

        template<>
        struct bit_and<void>
        {
            using is_transparent = __is_transparent;

            template<typename _Type1, typename _Type2>
            constexpr auto operator()(_Type1&& aLeft, _Type2&& aRight) const
                -> decltype(forward<_Type1>(aLeft) & 
                    forward<_Type2>(aRight))
            {
                return (forward<_Type1>(aLeft) &
                    forward<_Type2>(aRight));
            }
        };

        template<>
        struct bit_or<void>
        {
            using is_transparent = __is_transparent;

            template<typename _Type1, typename _Type2>
            constexpr auto operator()(_Type1&& aLeft, _Type2&& aRight) const
                -> decltype(forward<_Type1>(aLeft) |
                    forward<_Type2>(aRight))
            {
                return (forward<_Type1>(aLeft) |
                    forward<_Type2>(aRight));
            }
        };

        template<>
        struct bit_xor<void>
        {
            using is_transparent = __is_transparent;

            template<typename _Type1, typename _Type2>
            constexpr auto operator()(_Type1&& aLeft, _Type2&& aRight) const
                -> decltype(forward<_Type1>(aLeft) ^
                    forward<_Type2>(aRight))
            {
                return (forward<_Type1>(aLeft) ^
                    forward<_Type2>(aRight));
            }
        };

        template<>
        struct bit_not<void>
        {
            using is_transparent = __is_transparent;

            template<typename _Type>
            constexpr auto operator()(_Type&& aValue) const
                -> decltype(~forward<_Type>(aValue))
            {
                return (~forward<_Type>(aValue));
            }
        };
    }

}

#endif