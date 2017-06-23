#pragma once
#ifndef Bind_$A11A3B59_AE1C_4A99_88D9_C3AFF2EAD260
#define Bind_$A11A3B59_AE1C_4A99_88D9_C3AFF2EAD260 1

#include "KTL.Containers.Tuple.h"

namespace ktl
{
    inline namespace functional
    {
        /// PLACEHOLDER ARGUMENTS

        template<i32 _Index>
        struct _Ph
        { };

        template<typename _Type>
        struct is_placeholder
            : integral_constant<i32, 0>
        {
            // template to indicate that _Type is not a placeholder
        };

        template<i32 _Index>
        struct is_placeholder<_Ph<_Index>>
            : integral_constant<i32, _Index>
        {
            // template specialization to indicate that _Ph<_Index> is a placeholder
        };

        template<typename _Type>
        struct is_placeholder<const _Type>
            : is_placeholder<_Type>
        {
            // ignore const-qualifiers
        };

        template<typename _Type>
        struct is_placeholder<volatile _Type>
            : is_placeholder<_Type>
        {
            // ignore volatile-qualifiers
        };

        template<typename _Type>
        struct is_placeholder<const volatile _Type>
            : is_placeholder<_Type>
        {
            // ignore cv-qualifiers
        };


        /// TEMPLATE STRUCT is_bind_expression

        // TEMPLATE class _Binder FORWARD DECLARATION
        template<typename _ReturnType, typename _Functor, typename... _ArgTypes>
        class _Binder;

        // TEMPLATE struct is_bind_expression
        template<typename _Type>
        struct is_bind_expression
            : false_type
        {
            // template to indicate that _Type is not a bind expression
        };

        template<typename _ReturnType, typename _Functor, typename... _ArgTypes>
        struct is_bind_expression<_Binder<_ReturnType, _Functor, _ArgTypes...>>
            : true_type
        {
            // template specialization to indicate a bind expression
        };

        template<typename _Type>
        struct is_bind_expression<const _Type>
            : is_bind_expression<_Type>
        {
            // ignore const-qualifiers
        };

        template<typename _Type>
        struct is_bind_expression<volatile _Type>
            : is_bind_expression<_Type>
        {
            // ignore volatile-qualifiers
        };

        template<typename _Type>
        struct is_bind_expression<const volatile _Type>
            : is_bind_expression<_Type>
        {
            // ignore cv-qualifiers
        };


        /// TEMPLATE FUNCTION __fix_args

        template<
            typename _Tuple_bound_args,
            bool = unrefwrap<_Tuple_bound_args>::__is_refwrap,
            bool = is_bind_expression<_Tuple_bound_args>::value,
            int = is_placeholder<_Tuple_bound_args>::value>
            struct _Select_fixer;

        template<typename _Tuple_bound_args>
        struct _Select_fixer<_Tuple_bound_args, true, false, 0>
        {
            // reference_wrapper fixer

            template<typename _Tuple_unbound_args>
            static auto fix(_Tuple_bound_args& aBoundArgs, _Tuple_unbound_args&&)
                -> typename _Tuple_bound_args::type&
            {
                // unwrap a reference_wrapper
                return (aBoundArgs.get());
            }
        };

        template<typename _Tuple_bound_args>
        struct _Select_fixer<_Tuple_bound_args, false, true, 0>
        {
            // nested bind fixer

            template<typename _Tuple_unbound_args, usize... _Index>
            static auto apply(
                _Tuple_bound_args& aBoundArgs,
                _Tuple_unbound_args&& aUnboundArgs,
                integer_sequence<usize, _Index...>)
                -> decltype(aBoundArgs(get<_Index>(forward<_Tuple_unbound_args>(aUnboundArgs))...))
            {
                // call a nested bind expression
                return (aBoundArgs(get<_Index>(forward<_Tuple_unbound_args>(aUnboundArgs))...));
            }

            template<typename _Tuple_unbound_args>
            static auto fix(
                _Tuple_bound_args& aBoundArgs,
                _Tuple_unbound_args&& aUnboundArgs)
                -> decltype(apply(aBoundArgs, forward<_Tuple_unbound_args>(aUnboundArgs),
                    make_integer_sequence<
                    usize,
                    tuple_size<_Tuple_unbound_args>::value>()))
            {
                // call a nested bind expression
                return (apply(aBoundArgs, forward<_Tuple_unbound_args>(aUnboundArgs),
                    make_integer_sequence<
                    usize,
                    tuple_size<_Tuple_unbound_args>::value>()));
            }
        };

        template<typename _Tuple_bound_args>
        struct _Select_fixer<_Tuple_bound_args, false, false, 0>
        {
            // identity fixer

            template<typename _Tuple_unbound_args>
            static _Tuple_bound_args& fix(
                _Tuple_bound_args& aBoundArgs,
                _Tuple_unbound_args&&)
            {
                return (aBoundArgs);
            }
        };

        template<typename _Tuple_bound_args, i32 _PhIndex>
        struct _Select_fixer<_Tuple_bound_args, false, false, _PhIndex>
        {
            // placeholder fixer

            static_assert(_PhIndex > 0, "invalid is_placeholder value");

            template<typename _Tuple_unbound_args>
            static auto fix(_Tuple_bound_args&, _Tuple_unbound_args&& aUnboundArgs)
                -> decltype(get<_PhIndex - 1>(forward<_Tuple_unbound_args>(aUnboundArgs)))
            {
                // choose the _PhIndex unbound argument (1-based indexing)
                return (get<_PhIndex - 1>(forward<_Tuple_unbound_args>(aUnboundArgs)));
            }
        };

        template<typename _Tuple_bound_args, typename _Tuple_unbound_args>
        inline auto __fix_args(
            _Tuple_bound_args& aBoundArgs,
            _Tuple_unbound_args&& aUnboundArgs)
            -> decltype(_Select_fixer<_Tuple_bound_args>::fix(
                aBoundArgs, forward<_Tuple_unbound_args>(aUnboundArgs)))
        {
            // translate an argument for bind
            return (_Select_fixer<_Tuple_bound_args>::fix(
                aBoundArgs, forward<_Tuple_unbound_args>(aUnboundArgs)));
        }


        /// TEMPLATE function _Call_binder

        template<
            typename _ReturnType,
            isize... _Index,
            typename _Functor,
            typename _Tuple_bound_args,
            typename _Tuple_unbound_args>
            inline auto _Call_binder(
                _Forced<_ReturnType> aForcedRetType,
                integer_sequence<isize, _Index...>,
                _Functor& aFunctor,
                _Tuple_bound_args& aBoundArgs,
                _Tuple_unbound_args&& aUnboundArgs)
            -> decltype(_Invoke_ret(
                aForcedRetType,
                aFunctor,
                __fix_args(get<_Index>(aBoundArgs),
                    forward<_Tuple_unbound_args>(aUnboundArgs))...))
        {
            // bind() and bind<R>() invocation

            return (_Invoke_ret(
                aForcedRetType,
                aFunctor,
                __fix_args(get<_Index>(aBoundArgs), forward<_Tuple_unbound_args>(aUnboundArgs))...));
        }


        /// TEMPLATE class _Binder


        template<typename _ReturnType>
        struct _Forced_result_type
        {
            // used by bind<R>()
            using return_type = _ReturnType;
        };

        template<typename _ReturnType, typename _Functor>
        struct _Binder_result_type
        {
            using _Decayed = typename decay<_Functor>::type;
            using _All_weak_types = typename __weak_types<_Decayed>::type;
            using type = typename __if<
                is_same<_ReturnType, _Unforced>::value,
                typename __weak_result_type<_All_weak_types>,
                _Forced_result_type<_ReturnType>>::type;
        };

        template<typename _ReturnType, typename _Functor, typename... _ArgTypes>
        class _Binder
            : public _Binder_result_type<_ReturnType, _Functor>::type
        {
            // wrap bound callable object and arguments

        private:
            using _Sequence =
                make_integer_sequence<isize, sizeof...(_ArgTypes)>;
            using _Functor_decay = typename decay<_Functor>::type;
            using _Bound_arguments = tuple<typename decay<_ArgTypes>::type...>;

            _Functor_decay m_Functor;
            _Bound_arguments m_BoundArgs;

        public:
            explicit _Binder(_Functor&& aFunctor, _ArgTypes&&... aArguments)
                : m_Functor(move(aFunctor))
                , m_BoundArgs(forward<_ArgTypes>(aArguments)...)
            {
                // construct from forwarded callable object and arguments
            }

            template<typename... _UnboundArgTypes>
            auto operator()(_UnboundArgTypes&&... aUnboundArgs)
                -> decltype(_Call_binder(
                    _Forced<_ReturnType>(),
                    _Sequence(),
                    m_Functor,
                    m_BoundArgs,
                    forward_as_tuple(
                        forward<_UnboundArgTypes>(aUnboundArgs)...)))
            {
                /* invoke bound callable object with bound/unbound arguments */
                return (_Call_binder(
                    _Forced<_ReturnType>(),
                    _Sequence(),
                    m_Functor,
                    m_BoundArgs,
                    forward_as_tuple(
                        forward<_UnboundArgTypes>(aUnboundArgs)...)));
            }

            template<typename... _UnboundArgTypes>
            auto operator()(_UnboundArgTypes&&... aUnboundArgs) const
                -> decltype(_Call_binder(
                    _Forced<_ReturnType>(),
                    _Sequence(),
                    m_Functor,
                    m_BoundArgs,
                    forward_as_tuple(
                        forward<_UnboundArgTypes>(aUnboundArgs)...)))
            {
                /* invoke bound callable object with bound/unbound arguments */
                return (_Call_binder(
                    _Forced<_ReturnType>(),
                    _Sequence(),
                    m_Functor,
                    m_BoundArgs,
                    forward_as_tuple(
                        forward<_UnboundArgTypes>(aUnboundArgs)...)));
            }
        };


        /// TEMPLATE FUNCTION bind

        // TEMPLATE function bind (implicit return type)
        template<typename _Functor, typename... _ArgTypes>
        inline _Binder<_Unforced, _Functor, _ArgTypes...>
            bind(_Functor&& aFunctor, _ArgTypes&&... aArguments)
        {
            // bind a callable object with an implicit return type
            return (_Binder<_Unforced, _Functor, _ArgTypes...>(
                forward<_Functor>(aFunctor),
                forward<_ArgTypes>(aArguments)...));
        }

        // TEMPLATE function bind (explicit return type)
        template<typename _ReturnType, typename _Functor, typename... _ArgTypes>
        inline _Binder<_ReturnType, _Functor, _ArgTypes...>
            bind(_Functor&& aFunctor, _ArgTypes&&... aArguments)
        {
            return (_Binder<_ReturnType, _Functor, _ArgTypes...>(
                forward<_Functor>(aFunctor),
                forward<_ArgTypes>(aArguments)...));
        }


        namespace placeholders
        {
            // Define a large number of placeholders. There is no way to
            // simplify this with variadic templates, because we're introducing
            // unique names for each.

            constexpr _Ph<1> _1{};
            constexpr _Ph<2> _2{};
            constexpr _Ph<3> _3{};
            constexpr _Ph<4> _4{};
            constexpr _Ph<5> _5{};
            constexpr _Ph<6> _6{};
            constexpr _Ph<7> _7{};
            constexpr _Ph<8> _8{};
            constexpr _Ph<9> _9{};
            constexpr _Ph<10> _10{};
            constexpr _Ph<11> _11{};
            constexpr _Ph<12> _12{};
            constexpr _Ph<13> _13{};
            constexpr _Ph<14> _14{};
            constexpr _Ph<15> _15{};
            constexpr _Ph<16> _16{};
            constexpr _Ph<17> _17{};
            constexpr _Ph<18> _18{};
            constexpr _Ph<19> _19{};
            constexpr _Ph<20> _20{};
            constexpr _Ph<21> _21{};
            constexpr _Ph<22> _22{};
            constexpr _Ph<23> _23{};
            constexpr _Ph<24> _24{};
            constexpr _Ph<25> _25{};
            constexpr _Ph<26> _26{};
            constexpr _Ph<27> _27{};
            constexpr _Ph<28> _28{};
            constexpr _Ph<29> _29{};
        }
        using namespace ktl::placeholders;
    }

}

#endif