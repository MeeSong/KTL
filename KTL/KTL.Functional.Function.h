#pragma once
#ifndef Function_$196042EF_9259_402B_9606_932D5BA69C2B
#define Function_$196042EF_9259_402B_9606_932D5BA69C2B 1

#include "KTL.Functional.h"

namespace ktl
{
    inline namespace functional
    {
        /// TEMPLATE CLASS function

        template<typename _ReturnType, typename... _ArgTypes>
        struct _Arguments_types
        { };

        template<typename _ReturnType, typename _ArgTypes>
        struct _Arguments_types<_ReturnType, _ArgTypes>
            : unary_function<_ArgTypes, _ReturnType>
        { };

        template<typename _ReturnType, typename _ArgTypes1, typename _ArgTypes2>
        struct _Arguments_types<_ReturnType, _ArgTypes1, _ArgTypes2>
            : binary_function<_ArgTypes1, _ArgTypes2, _ReturnType>
        { };

        // Simple type wrapper that helps avoid annoying const problems
        // when casting between void pointers and pointers-to-pointers.
        template<typename _Type>
        struct _Simple_type_wrapper
        {
            _Simple_type_wrapper(_Type aValue)
                : value(aValue)
            { }

            _Type value;
        };

        template<typename _Type>
        struct __is_location_invariant
            : is_trivially_copyable<_Type>::type
        { };

        template<typename _Type>
        struct __is_location_invariant<_Simple_type_wrapper<_Type>>
            : __is_location_invariant<_Type>
        { };

#ifdef _M_X64
#pragma pack(push, 8)
#else
#pragma pack(push, 4)
#endif
        union _Nocopy_types
        {
            void *object;
            const void *const_object;
            void(*function_pointer)();
            void(_undefined::*member_pointer)();
        };
#pragma pack(pop)

        union _Any_data
        {
            void*       access() { return &m_PodData[0]; }
            const void* access() const { return &m_PodData[0]; }

            template<typename _Functor>
            _Functor& access()
            {
                return *static_cast<_Functor*>(access());
            }

            template<typename _Functor>
            const _Functor& access() const
            {
                return *static_cast<const _Functor*>(access());
            }

            _Nocopy_types m_Unused;
            char m_PodData[sizeof(_Nocopy_types)] = { 0 };
        };


        enum class _Manager_operation
        {
            get_functor_ptr,
            clone_functor,
            destroy_functor
        };


        // Converts a reference to a function object into a callable
        // function object.
        template<typename _Functor>
        inline _Functor&
            _Convert_function_reference_to_callable(_Functor& aFunctor)
        {
            return aFunctor;
        }

        template<typename _Member, typename _Class>
        inline __mem_fn<_Member _Class::*>
            _Convert_function_reference_to_callable(
                _Member _Class::* &aMemberPointer)
        {
            return mem_fn(aMemberPointer);
        }

        template<typename _Member, typename _Class>
        inline __mem_fn<_Member _Class::*>
            _Convert_function_reference_to_callable(
                _Member _Class::* const &aMemberPointer)
        {
            return mem_fn(aMemberPointer);
        }

        template<typename _Member, typename _Class>
        inline __mem_fn<_Member _Class::*>
            _Convert_function_reference_to_callable(
                _Member _Class::* volatile &aMemberPointer)
        {
            return mem_fn(aMemberPointer);
        }

        template<typename _Member, typename _Class>
        inline __mem_fn<_Member _Class::*>
            _Convert_function_reference_to_callable(
                _Member _Class::* const volatile &aMemberPointer)
        {
            return mem_fn(aMemberPointer);
        }

        // TEMPLATE class function_base

        template<typename _FuncType>
        class function;

        class _Function_base
        {
        public:
            static constexpr usize m_MaxSize = sizeof(_Nocopy_types);
            static constexpr usize m_MaxAlign = alignof(_Nocopy_types);

            template<typename _Functor>
            class base_manager
            {
            protected:

                static constexpr bool m_stored_locally =
                    (__is_location_invariant<_Functor>::value
                    && sizeof(_Functor) <= m_MaxSize
                    && alignof(_Functor) <= m_MaxAlign
                    && (m_MaxAlign % alignof(_Functor) == 0));

                using _Local_storage = bool_constant<m_stored_locally>;

                // Retrieve a pointer to the function object
                static _Functor* get_pointer(const _Any_data& aSource)
                {
                    const _Functor *vPtr = m_stored_locally ?
                        addressof(aSource.access<_Functor>())
                        : aSource.access<_Functor*>(); /* have stored a pointer */

                    return const_cast<_Functor*>(vPtr);
                }

                // Clone a location-invariant function object that fits within
                // an _Any_data structure.
                static bool clone(_Any_data& aDest, const _Any_data& aSource, true_type)
                {
                    new (aDest.access()) _Functor(aSource.access<_Functor>());
                    return true;
                }

                // Clone a function object that is not location-invariant or
                // that cannot fit into an _Any_data structure.
                static bool clone(_Any_data& aDest, const _Any_data& aSource, false_type)
                {
                    aDest.access<_Functor*>() = new _Functor(*aSource.access<_Functor*>());
                    if (nullptr == aDest.access<_Functor*>())
                    {
                        return false;
                    }

                    return true;
                }

                // Destroying a location-invariant object may still require
                // destruction.
                static void destroy(_Any_data& aObject, true_type)
                {
                    aObject.access<_Functor>().~_Functor();
                    aObject.m_Unused.function_pointer = nullptr;
                }

                // Destroying an object located on the heap.
                static void destroy(_Any_data& aObject, false_type)
                {
                    delete aObject.access<_Functor*>();
                    aObject.m_Unused.function_pointer = nullptr;
                }

            public:
                static bool manager(_Any_data& aDest, const _Any_data& aSource,
                    _Manager_operation aOperation)
                {
                    bool vSuccess = false;

                    switch (aOperation)
                    {
                    case _Manager_operation::get_functor_ptr:
                        aDest.access<_Functor*>() = get_pointer(aSource);
                        vSuccess = true;
                        break;
                    case _Manager_operation::clone_functor:
                        vSuccess = clone(aDest, aSource, _Local_storage());
                        break;
                    case _Manager_operation::destroy_functor:
                        destroy(aDest, _Local_storage());
                        vSuccess = true;
                        break;
                    }

                    return vSuccess;
                }

                static bool init_functor(_Any_data& aFunctor, _Functor&& aPrimitiveFunctor)
                {
                    return init_functor(aFunctor, move(aPrimitiveFunctor), _Local_storage());
                }

                template<typename _FuncType>
                static bool not_empty_function(const function<_FuncType>& aFunctor)
                {
                    return static_cast<bool>(aFunctor);
                }

                template<typename _FuncType>
                static bool not_empty_function(_FuncType *aFunctionPointer)
                {
                    return aFunctionPointer != nullptr;
                }

                template<typename _Class, typename _FuncType>
                static bool not_empty_function(_FuncType _Class::* aMemberPointer)
                {
                    return aMemberPointer != nullptr;
                }

                template<typename _FuncType>
                static bool not_empty_function(const _FuncType&)
                {
                    return true;
                }

            private:
                static bool init_functor(_Any_data& aFunctor, _Functor&& aPrimitiveFunctor, true_type)
                {
                    new (aFunctor.access()) _Functor(move(aPrimitiveFunctor));
                    return true;
                }

                static bool init_functor(_Any_data& aFunctor, _Functor&& aPrimitiveFunctor, false_type)
                {
                    aFunctor.access<_Functor*>() = new _Functor(move(aPrimitiveFunctor));
                    if (nullptr == aFunctor.access<_Functor*>())
                    {
                        return false;
                    }

                    return true;
                }
            };

            template<typename _Functor>
            class _Reference_manager
                : public base_manager<_Functor*>
            {
                using _Base = _Function_base::base_manager<_Functor*>;

            public:
                static bool manager(_Any_data& aDest, const _Any_data& aSource, _Manager_operation aOperation)
                {
                    bool vSuccess = false;

                    switch (aOperation)
                    {
                    case _Manager_operation::get_functor_ptr:
                        aDest.access<_Functor*>() = *_Base::get_pointer(aSource);
                        return is_const<_Functor>::value;

                    default:
                        vSuccess = _Base::manager(aDest, aSource, aOperation);
                    }

                    return vSuccess;
                }

                static bool init_functor(_Any_data& aFunctor, reference_wrapper<_Functor> aPrimitiveFunctor)
                {
                    return _Base::init_functor(aFunctor, addressof(aPrimitiveFunctor.get()));
                }
            };

            _Function_base()
                : m_Manager(nullptr)
            {
            }

            ~_Function_base()
            {
                if (m_Manager)
                {
                    m_Manager(m_Functor, m_Functor, _Manager_operation::destroy_functor);
                }
            }

            bool is_empty() const
            {
                return !m_Manager;
            }

            using _ManagerType = bool(*)(_Any_data&, const _Any_data&, _Manager_operation);

            _Any_data    m_Functor;
            _ManagerType m_Manager = nullptr;
        };

        // TEMPLATE class function_handler

        template<typename _FunctorType, typename _Functor>
        class _Function_handler;

        template<typename _ReturnType, typename _Functor, typename... _ArgTypes>
        class _Function_handler<_ReturnType(_ArgTypes...), _Functor>
            : public _Function_base::base_manager<_Functor>
        {
            using _Base = _Function_base::base_manager<_Functor>;

        public:
            static _ReturnType invoke(const _Any_data& aFunctor, _ArgTypes&&... aArguments)
            {
                return (*_Base::get_pointer(aFunctor))(forward<_ArgTypes>(aArguments)...);
            }
        };

        template<typename _Functor, typename... _ArgTypes>
        class _Function_handler<void(_ArgTypes...), _Functor>
            : public _Function_base::base_manager<_Functor>
        {
            using _Base = _Function_base::base_manager<_Functor>;

        public:
            static void invoke(const _Any_data& aFunctor, _ArgTypes&&... aArguments)
            {
                (*_Base::get_pointer(aFunctor))(forward<_ArgTypes>(aArguments)...);
            }
        };

        template<typename _ReturnType, typename _Functor, typename... _ArgTypes>
        class _Function_handler<_ReturnType(_ArgTypes...), reference_wrapper<_Functor>>
            : public _Function_base::_Reference_manager<_Functor>
        {
            using _Base = _Function_base::_Reference_manager<_Functor>;

        public:
            static _ReturnType invoke(const _Any_data& aFunctor, _ArgTypes&&... aArguments)
            {
                return _Convert_function_reference_to_callable(
                    **_Base::get_pointer(aFunctor))(
                        forward<_ArgTypes>(aArguments)...);
            }
        };

        template<typename _Functor, typename... _ArgTypes>
        class _Function_handler<void(_ArgTypes...), reference_wrapper<_Functor>>
            : public _Function_base::base_manager<_Functor>
        {
            using _Base = _Function_base::_Reference_manager<_Functor>;

        public:
            static void invoke(const _Any_data& aFunctor, _ArgTypes&&... aArguments)
            {
                _Convert_function_reference_to_callable(
                    **_Base::get_pointer(aFunctor))(
                        forward<_ArgTypes>(aArguments)...);
            }
        };

        template<typename _Class, typename _Member, typename... _ArgTypes>
        class _Function_handler<void(_ArgTypes...), _Member _Class::*>
            : public _Function_base::base_manager<typename _Simple_type_wrapper<_Member _Class::*>>
        {
            using _Functor = _Member _Class::*;
            using _Wrapper = _Simple_type_wrapper<_Functor>;
            using _Base = _Function_base::base_manager<_Wrapper>;

        public:
            static bool mamager(_Any_data& aDest, const _Any_data& aSource, _Manager_operation aOperation)
            {
                bool vSuccess = false;

                switch (aOperation)
                {
                case _Manager_operation::get_functor_ptr:
                    aDest.access<_Functor*>() = &_Base::get_pointer(aSource)->value;
                    vSuccess = true;
                    break;
                default:
                    vSuccess = _Base::manager(aDest, aSource, aOperation);
                    break;
                }

                return vSuccess;
            }

            static void invoke(const _Any_data& aFunctor, _ArgTypes&&... aArguments)
            {
                mem_fn(_Base::get_pointer(aFunctor)->value)(forward<_ArgTypes>(aArguments)...);
            }
        };

        template<typename _Class, typename _Member, typename _ReturnType, typename... _ArgTypes>
        class _Function_handler<_ReturnType(_ArgTypes...), _Member _Class::*>
            : public _Function_handler<void(_ArgTypes...), _Member _Class::*>
        {
            using _Base = _Function_handler<void(_ArgTypes...), _Member _Class::*>;

        public:
            static _ReturnType invoke(const _Any_data& aFunctor, _ArgTypes&&... aArguments)
            {
                return mem_fn(_Base::get_pointer(aFunctor)->value)(forward<_ArgTypes>(aArguments)...);
            }
        };


        template<typename _From, typename _To>
        using __check_func_return_type = disjunction< // or
            is_void<_To>,
            is_same<_From, _To>,
            is_convertible<_From, _To>>;

        // TEMPLATE class function 

        template<typename _ReturnType, typename... _ArgTypes>
        class function<_ReturnType(_ArgTypes...)>
            : public _Arguments_types<_ReturnType, _ArgTypes...>,
            private _Function_base
        {
            using _FunctorType = _ReturnType(_ArgTypes...);

            template<
                typename _Functor,
                typename _ReturnType2 = typename result_of<_Functor&(_ArgTypes...)>::type>
            struct _Callable : __check_func_return_type<_ReturnType2, _ReturnType> {};

            // Used so the return type convertibility checks aren't done when
            // performing overload resolution for copy construction/assignment.
            template<typename _Type>
            struct _Callable<function, _Type> : false_type
            { };

            template<typename _Cond, typename _Type>
            using _Requires = typename enable_if<_Cond::value, _Type>::type;

        public:
            using return_type = _ReturnType;

            function() NOEXCEPT$TYPE
                : _Function_base()
            { }

            function(nullptr_t) NOEXCEPT$TYPE
                : _Function_base()
            { }

            function(const function&) = delete;
            function& operator= (const function&) = delete;

            /*function(const function& aSource)
            {
                if (static_cast<bool>(aSource))
                {
                    if (aSource.m_Manager(m_Functor, aSource.m_Functor,
                        _Manager_operation::clone_functor))
                    {
                        m_Invoker = aSource.m_Invoker;
                        m_Manager = aSource.m_Manager;
                    }
                }
            }

            function(function&& aSource)
            {
                aSource.swap(*this);
            }

            template<typename _Functor,
                typename = _Requires<negation<is_same<_Functor, function>>, void>,
                typename = _Requires<_Callable<_Functor>, void>
            >
            function(_Functor aFunctor)
            {
                using _Handler = _Function_handler<_FunctorType, _Functor>;

                if (_Handler::not_empty_function(aFunctor))
                {
                    if (_Handler::init_functor(m_Functor, move(aFunctor)))
                    {
                        m_Invoker = &_Handler::invoke;
                        m_Manager = &_Handler::manager;
                    }
                }
            }

            function& operator=(const function& aSource)
            {
                function(aSource).swap(*this);
                return *this;
            }

            function& operator=(function&& aSource)
            {
                function(move(aSource)).swap(*this);
                return *this;
            }

            function& operator=(nullptr_t) NOEXCEPT$TYPE
            {
                reset();
                return *this;
            }

            template<typename _Functor>
            _Requires<_Callable<typename decay<_Functor>::type>, function&>
                operator=(_Functor&& aSource)
            {
                function(forward<_Functor>(aSource)).swap(*this);
                return *this;
            }

            template<typename _Functor>
            function& operator=(reference_wrapper<_Functor> aSource) NOEXCEPT$TYPE
            {
                function(aSource.get()).swap(*this);
                return *this;
            }*/

            bool attach(const function& aSource)
            {
                function vTemp;

                if (static_cast<bool>(aSource))
                {
                    if (aSource.m_Manager(vTemp.m_Functor, aSource.m_Functor,
                        _Manager_operation::clone_functor))
                    {
                        vTemp.m_Invoker = aSource.m_Invoker;
                        vTemp.m_Manager = aSource.m_Manager;

                        vTemp.swap(*this);
                        return true;
                    }
                }

                return false;
            }

            bool attach(function&& aSource)
            {
                reset();
                aSource.swap(*this);
                return true;
            }

            template<typename _Functor,
                typename = _Requires<negation<is_same<_Functor, function>>, void>,
                typename = _Requires<_Callable<_Functor>, void>
            >
            bool attach(_Functor aFunctor) NOEXCEPT$TYPE
            {
                function vTemp;
                using _Handler = _Function_handler<_FunctorType, _Functor>;

                if (_Handler::not_empty_function(aFunctor))
                {
                    if (_Handler::init_functor(vTemp.m_Functor, move(aFunctor)))
                    {
                        vTemp.m_Invoker = &_Handler::invoke;
                        vTemp.m_Manager = &_Handler::manager;

                        vTemp.swap(*this);
                        return true;
                    }
                }

                return false;
            }

            template<typename _Functor>
            bool attach(reference_wrapper<_Functor> aSource) NOEXCEPT$TYPE
            {
                return attach(aSource.get());
            }

            void reset()
            {
                if (!is_empty())
                {
                    m_Manager(m_Functor, m_Functor, _Manager_operation::destroy_functor);
                    m_Manager = nullptr;
                    m_Invoker = nullptr;
                }
            }

            void swap(function& aFunctor)
            {
                swap_adl(m_Functor, aFunctor.m_Functor);
                swap_adl(m_Manager, aFunctor.m_Manager);
                swap_adl(m_Invoker, aFunctor.m_Invoker);
            }

            bool is_empty() const
            {
                return (!m_Manager || !m_Invoker);
            }

            explicit operator bool() const NOEXCEPT$TYPE
            {
                return !is_empty();
            }

            inline bool operator==(nullptr_t) const NOEXCEPT$TYPE
            {
                return !(static_cast<bool>(*this));
            }

            inline bool operator!=(nullptr_t) const NOEXCEPT$TYPE
            {
                return (static_cast<bool>(*this));
            }
            
            _ReturnType operator()(_ArgTypes... aArguments) const
            {
                if (!is_empty())
                {
                    return m_Invoker(m_Functor, forward<_ArgTypes>(aArguments)...);
                }

                return static_cast<_ReturnType>(0);
            }

            template<typename _Functor>
            _Functor *target() NOEXCEPT$TYPE
            {
                if (m_Manager)
                {
                    _Any_data vPtr;
                    m_Manager(vPtr, m_Functor, _Manager_operation::get_functor_ptr);
                    return vPtr.access<_Functor*>();
                }

                return nullptr;
            }

            template<typename _Functor>
            const _Functor *target() const NOEXCEPT$TYPE
            {
                if (m_Manager)
                {
                    _Any_data vPtr;
                    m_Manager(vPtr, m_Functor, _Manager_operation::get_functor_ptr);
                    return vPtr.access<_Functor*>();
                }

                return nullptr;
            }

        private:
            using _InvokerType = _ReturnType(*)(const _Any_data& aFunctor, _ArgTypes&&...);
            _InvokerType m_Invoker = nullptr;
        };

        /// function function's swap

        template<typename _ReturnType, typename... _ArgTypes>
        inline void swap(function<_ReturnType(_ArgTypes...)>& aLeft,
            function<_ReturnType(_ArgTypes...)>& aRight)
        {
            aLeft.swap(aRight);
        }

    }

}

#endif
