/*
** TypeTraits.h
** Created by Rayn on 2015/01/30
** 获取传入类型T的类型特性
*/
#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_

#include <type_traits>

namespace rayn {

    /// base classes

    // integral_constant
    template <typename T, T val>
    struct integral_constant {
        static const T                      value = val;
        typedef T                           value_type;
        typedef integral_constant<T, val>   type;

        operator T() { return value; }
    };

    // true type
    typedef integral_constant<bool, true>   true_type;

    // false type
    typedef integral_constant<bool, false>  false_type;

    /// Meta programming helper types.

    // conditional
    template <bool Cond, typename IfTrue, typename IfFalse>
    struct conditional {
        typedef IfTrue  type;
    };

    template <typename IfTrue, typename IfFalse>
    struct conditional<false, IfTrue, IfFalse> {
        typedef IfFalse  type;
    };

    template <typename...>
    struct __or_;

    template <>
    struct __or_<> : public false_type {};

    template <typename B1>
    struct __or_<B1> : public B1 {};

    template <typename B1, typename B2>
    struct __or_<B1, B2>
        : public conditional<B1::value, B1, B2>::type {};

    template <typename B1, typename B2, typename B3, typename... Bn>
    struct __or_<B1, B2, B3, Bn...>
        : public conditional<B1::value, B1, __or_<B2, B3, Bn...>>::type {};

    template <typename...>
    struct __and_;

    template <>
    struct __and_<> : public true_type {};

    template <typename B1>
    struct __and_<B1> : public B1 {};

    template <typename B1, typename B2>
    struct __and_<B1, B2>
        : public conditional<B1::value, B2, B1>::type {};

    template <typename B1, typename B2, typename B3, typename... Bn>
    struct __and_<B1, B2, B3, Bn...>
        : public conditional<B1::value, __and_<B2, B3, Bn...>, B1>::type {};

    template <typename P>
    struct __not_
        : public integral_constant<bool, !P::value> {};


    /// Primary type categories

    template <typename>
    struct remove_cv;

    template <typename>
    struct __is_void_helper : public false_type {};

    template <>
    struct __is_void_helper<void> : public true_type {};

    // is_void
    template <typename T>
    struct is_void
        : public __is_void_helper<typename remove_cv<T>::type>::type {};

    // is_array
    template <typename>
    struct is_array : public false_type {};

    template <typename T, size_t Size>
    struct is_array<T[Size]> : public true_type {};

    template <typename T>
    struct is_array<T[]> : public true_type {};

    // is_class, depend on compiler implementation
    template <typename T>
    struct is_class : public integral_constant <bool, __is_class(T)> {};

    // is_enum, depend on compiler implementation
    template <typename T>
    struct is_enum : public integral_constant <bool, __is_enum(T)> {};

    // is_union, depend on compiler implementation
    template <typename T>
    struct is_union : public integral_constant <bool, __is_union(T)> {};

    // is_floating_point
    template <typename T>
    struct __is_floating_point_helper
        : public false_type {};

    template <>
    struct __is_floating_point_helper<float>
        : public true_type {};

    template <>
    struct __is_floating_point_helper<double>
        : public true_type {};

    template <>
    struct __is_floating_point_helper<long double>
        : public true_type {};

    template <typename T>
    struct is_floating_point
        : public __is_floating_point_helper<typename remove_cv<T>::type>::type {};

    template <typename>
    struct __is_integral_helper : public false_type {};

    template <>
    struct __is_integral_helper<bool> : public true_type {};

    template <>
    struct __is_integral_helper<char> : public true_type {};

    template <>
    struct __is_integral_helper<signed char> : public true_type{};

    template <>
    struct __is_integral_helper<unsigned char> : public true_type{};

    template <>
    struct __is_integral_helper<wchar_t> : public true_type {};

    /*
    template <>
    struct __is_integral_helper<char16_t> : public true_type {};

    template <>
    struct __is_integral_helper<char32_t> : public true_type {};
    */

    template <>
    struct __is_integral_helper<short> : public true_type {};

    template <>
    struct __is_integral_helper<unsigned short> : public true_type {};

    template <>
    struct __is_integral_helper<int> : public true_type {};

    template <>
    struct __is_integral_helper<unsigned int> : public true_type {};

    template <>
    struct __is_integral_helper<long> : public true_type {};

    template <>
    struct __is_integral_helper<unsigned long> : public true_type {};

    template <>
    struct __is_integral_helper<long long> : public true_type {};

    template <>
    struct __is_integral_helper<unsigned long long> : public true_type {};

    // is_integral
    template <typename T>
    struct is_integral
        : public __is_integral_helper<typename remove_cv<T>::type>::type {};

    template <typename>
    struct __is_pointer_helper : public false_type {};

    template <typename T>
    struct __is_pointer_helper<T*> : public true_type {};

    // is_pointer
    template <typename T>
    struct is_pointer
        : public __is_pointer_helper<typename remove_cv<T>::type>::type {};


    // is_lvalue_reference
    template <typename>
    struct is_lvalue_reference : public false_type {};

    template <typename T>
    struct is_lvalue_reference<T&> : public true_type {};

    // is_rvalue_reference
    template <typename>
    struct is_rvalue_reference : public false_type {};

    template <typename T>
    struct is_rvalue_reference<T&&> : public true_type {};


    // is_function
    template <typename>
    struct is_function : public false_type {};

    template <typename Res, typename... ArgsTypes>
    struct is_function<Res(ArgsTypes...)> : public true_type {};

    template <typename Res, typename... ArgsTypes>
    struct is_function<Res(ArgsTypes..., ...)> : public true_type {};



    // is_member_object_pointer
    template <typename>
    struct __is_member_object_pointer_helper : public false_type {};

    template <typename T, typename C>
    struct __is_member_object_pointer_helper<T C::*>
        : public integral_constant<bool, !is_function<T>::value>{};

    template <typename T>
    struct is_member_object_pointer
        : public __is_member_object_pointer_helper<typename remove_cv<T>::type>::type {};

    // is_member_function_pointer
    template <typename>
    struct __is_member_function_pointer_helper : false_type {};

    template <typename Res, typename C, typename... ArgsTypes>
    struct __is_member_function_pointer_helper<Res (C::*)(ArgsTypes...)>
        : true_type {};

    template <typename Res, typename C, typename... ArgsTypes>
    struct __is_member_function_pointer_helper<Res (C::*)(ArgsTypes..., ...)>
        : true_type {};

    template <typename T>
    struct is_member_function_pointer
        : public __is_member_function_pointer_helper<typename remove_cv<T>::type>::type {};

    /// Composite type categories

    // is_null_pointer
    template <typename>
    struct __is_null_pointer_helper : public false_type {};

    template <>
    struct __is_null_pointer_helper<std::nullptr_t> : public true_type {};

    template <typename T>
    struct is_null_pointer
        : public __is_null_pointer_helper<typename remove_cv<T>::type>::type {};

    // is_arithmetic
    template <typename T>
    struct is_arithmetic
        : public __or_<is_integral<T>, is_floating_point<T>>::type {};

    // is_fundamental
    template<typename T>
    struct is_fundamental
        : public __or_<is_arithmetic<T>, is_void<T>, is_null_pointer<T>>::type {};

    // is_compound
    template <typename T>
    struct is_compound : public __not_<is_fundamental<T>>::type {};

    // is_reference
    template <typename T>
    struct is_reference
        : public __or_<is_lvalue_reference<T>, is_rvalue_reference<T>>::type {};

    // is_object
    template <typename T>
    struct is_object
        : public __not_<__or_<is_function<T>, is_reference<T>, is_void<T>>>::type {};

    // is_member_pointer
    template <typename T>
    struct __is_member_pointer_helper : public false_type {};

    template <typename T, typename C>
    struct __is_member_pointer_helper<T C::*>
        : public true_type {};

    template <typename T>
    struct is_member_pointer
        : public __is_member_pointer_helper<typename remove_cv<T>::type>::type {};

    // is_scalar
    template<typename T>
    struct is_scalar
        : public __or_<is_arithmetic<T>, is_enum<T>, is_pointer<T>,
                       is_member_pointer<T>, is_null_pointer<T>>::type
    {};

    // Utility to detect referenceable types ([defns.referenceable]).

    template <typename T>
    struct __is_referenceable : public __or_<is_object<T>, is_reference<T>>::type {};

    template <typename Res, typename... Args>
    struct __is_referenceable<Res(Args...)> : public true_type {};

    template <typename Res, typename... Args>
    struct __is_referenceable<Res(Args..., ...)> : public true_type {};


    /// Type properties.

    // is_const
    template <typename>
    struct is_const : public false_type {};

    template <typename T>
    struct is_const<T const> : public true_type {};

    // is_volatile
    template <typename>
    struct is_volatile : public false_type {};

    template <typename T>
    struct is_volatile<T volatile> : public true_type {};

    // is_signed
    template <typename T, bool = is_arithmetic<T>::value>
    struct __is_signed_helper : public false_type {};

    template <typename T>
    struct __is_signed_helper<T, true>
        : public integral_constant<bool, T(-1) < T(0)> {};

    template <typename T>
    struct is_signed : public __is_signed_helper<T>::type {};

    // is_unsigned
    template <typename T>
    struct is_unsigned : public __and_ < is_arithmetic<T>, __not_<is_signed<T>> > {};

    // is_abstract
    template <typename T>
    struct is_abstract : public std::is_abstract<T> {};

    /// Type relationships

    // is_same
    template <typename, typename>
    struct is_same : public false_type {};

    template <typename T>
    struct is_same<T, T> : public true_type{};

    // is_convertible
    template <typename From, typename To>
    struct is_convertible : public std::is_convertible<From, To> {};

    // is_base_of
    template <class Base, class Derived>
    struct is_base_of : public std::is_base_of<Base, Derived> {};

    /// Type Property queries

    // alignment_of
    template <typename T>
    struct alignment_of : public std::alignment_of<T> {};

    // rank
    template <typename>
    struct rank : public integral_constant<size_t, 0> {};

    template <typename T, size_t Size>
    struct rank<T[Size]>
        : public integral_constant<size_t, 1 + rank<T>::value> {};

    template <typename T>
    struct rank<T[]>
        : public integral_constant<size_t, 1 + rank<T>::value> {};

    // extent
    template <typename T, unsigned N = 0>
    struct extent 
        : public integral_constant<size_t, 0> {};

    template <typename T>
    struct extent<T[], 0>
        : public integral_constant<size_t, 0>{};

    template <typename T, unsigned N>
    struct extent<T[], N>
        : public integral_constant<size_t, extent<T, N - 1>::value>{};

    template <typename T, size_t Size>
    struct extent<T[Size], 0>
        : public integral_constant<size_t, Size> {};

    template <typename T, unsigned N, size_t Size>
    struct extent<T[Size], N>
        : public integral_constant<size_t, extent<T, N - 1>::value> {};

    //// Type transformations

    /// Const-volatile qualifications

    // remove_const
    template <typename T>
    struct remove_const {
        typedef T   type;
    };

    template <typename T>
    struct remove_const<T const> {
        typedef T   type;
    };

    // remove_volatile
    template <typename T>
    struct remove_volatile {
        typedef T   type;
    };

    template <typename T>
    struct remove_volatile<T volatile> {
        typedef T   type;
    };

    // remove_cv
    template <typename T>
    struct remove_cv {
        typedef typename
            remove_const<typename remove_volatile<T>::type>::type   type;
    };

    // add_const
    template <typename T>
    struct add_const {
        typedef T const     type;
    };

    // add_volatile
    template <typename T>
    struct add_volatile {
        typedef T volatile  type;
    };

    // add_cv
    template <typename T>
    struct add_cv {
        typedef typename
            add_const<typename add_volatile<T>::type>::type     type;
    };

    /// Compound type alterations

    // remove_reference
    template <typename T>
    struct remove_reference {
        typedef T   type;
    };

    template <typename T>
    struct remove_reference<T&> {
        typedef T   type;
    };

    template <typename T>
    struct remove_reference<T&&> {
        typedef T   type;
    };

    /// add_lvalue_reference
    template <typename T, bool = __is_referenceable<T>::value>
    struct __add_lvalue_reference_helper {
        typedef T       type;
    };

    template <typename T>
    struct __add_lvalue_reference_helper<T, true> {
        typedef T&      type;
    };

    template <typename T>
    struct add_lvalue_reference
        : public __add_lvalue_reference_helper<T> {};

    // add_rvalue_reference
    template <typename T, bool = __is_referenceable<T>::value>
    struct __add_rvalue_reference_helper {
        typedef T       type;
    };

    template <typename T>
    struct __add_rvalue_reference_helper<T, true> {
        typedef T&&     type;
    };

    template <typename T>
    struct add_rvalue_reference
        : public __add_rvalue_reference_helper<T> {};

    // remove_pointer
    template <typename T, typename>
    struct __remove_pointer_helper {
        typedef T   type;
    };

    template <typename T, typename U>
    struct __remove_pointer_helper<T, U*> {
        typedef U   type;
    };

    template <typename T>
    struct remove_pointer
        : public __remove_pointer_helper<T, typename remove_cv<T>::type> {};

    // add_pointer
    template <typename T, bool = __or_<__is_referenceable<T>, is_void<T>>::value>
    struct __add_pointer_helper {
        typedef T   type;
    };

    template <typename T>
    struct __add_pointer_helper<T, true> {
        typedef typename remove_reference<T>::type*     type;
    };

    template <typename T>
    struct add_pointer : public __add_pointer_helper<T> {};

    // remove_extent
    template <typename T>
    struct remove_extent {
        typedef T   type;
    };

    template <typename T, size_t Size>
    struct remove_extent<T[Size]> {
        typedef T   type;
    };

    template <typename T>
    struct remove_extent<T[]> {
        typedef T   type;
    };

    // remove_all_extents
    template <typename T>
    struct remove_all_extents {
        typedef T   type;
    };

    template <typename T, size_t Size>
    struct remove_all_extents<T[Size]> {
        typedef typename remove_all_extents<T>::type    type;
    };

    template <typename T>
    struct remove_all_extents<T[]> {
        typedef typename remove_all_extents<T>::type    type;
    };

    // decay
    template <typename T,
              bool IsArray = is_array<T>::value,
              bool IsFunction = is_function<T>::value>
    struct __decay_selector;

    template <typename T>
    struct __decay_selector<T, false, false> {
        typedef typename remove_cv<T>::type   __type;
    };

    template <typename T>
    struct __decay_selector<T, true, false> {
        typedef typename remove_extent<T>::type*   __type;
    };

    template <typename T>
    struct __decay_selector<T, false, true> {
        typedef typename add_pointer<T>::type   __type;
    };

    template <typename T>
    class decay {
        typedef typename remove_reference<T>::type  __remove_type;

    public:
        typedef typename __decay_selector<__remove_type>::__type    type;
    };

    // declval
    template <typename T>
    inline typename add_rvalue_reference<T>::type
        declval();


    /// Other type generators
    template <bool, typename T = void>
    struct enable_if {};

    template <typename T>
    struct enable_if<true, T> {
        typedef T   type;
    };


    ////////////////////////////////
    /// 以下已废弃
    ////////////////////////////////

    /*
    ** primary template
    */

    struct _true_type {};
    struct _false_type {};

    template<class T>
    struct _type_traits {
        typedef _true_type      this_dummy_member_must_be_first;
        typedef _false_type     has_trivial_default_constructor;
        typedef _false_type     has_trivial_copy_constructor;
        typedef _false_type     has_trivial_assignment_operator;
        typedef _false_type     has_trivial_destructor;
        typedef _false_type     is_POD_type;
    };

    /*
    ** 为built_in类型做特化
    */
    template<>
    struct _type_traits < bool > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < char > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < unsigned char > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };

    template<>
    struct _type_traits < signed char > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };

    template<>
    struct _type_traits < wchar_t > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };

    template<>
    struct _type_traits < short > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < unsigned short > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < int > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < unsigned int > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < long > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < unsigned long > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < long long > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < unsigned long long > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < float > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < double > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < long double > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };

    /*
    ** T ptr & const T ptr特化
    */
    template<class T>
    struct _type_traits < T* > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<class T>
    struct _type_traits < const T* > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };

    /*
    * C-Style String <XXX char*> 特化
    */
    template<>
    struct _type_traits < char* > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < unsigned char* > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < signed char* > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < const char* > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < const unsigned char* > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < const signed char* > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
}

#endif