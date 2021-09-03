#pragma once

#include "erase_at.hpp"
#include "is.hpp"
#include "are.hpp"
#include "at.hpp"
#include "indices_of.hpp"
#include "values.hpp"
#include <type_traits>

namespace types {

/*
size_t     size

bool       empty

type       at_index<Index>
types      at_indices<Indices...>
types      at<indices::of<Indices...>>

type       front

type       back

types      append_back_types<Types...>

types      append_front_types<Types...>

indices    indices_of_satisfying<Predicate>
indices    indices_of_not_satisfying<Predicate>

indices    indices_of_same_as<Type>
indices    indices_of_not_same_as<Type>

indices    indices_of_args_for_invocable_type<Type>
indices    indices_of_not_args_for_invocable_type<Type>

types      erase_at_index<Index>
types      erase_at_indices<Indices...>
types      erase_at<indices::of<Indices...>>

types      erase_types<Types...>

size_t     count_of_same_as<Type>

type       contains_same_as_predicate<Type>
bool       contains_same_as<Type>
*/

template<typename... Ts> 
struct of;

template<>
struct of<> {
	static constexpr std::size_t size = 0;
	static constexpr bool empty = true;

	template<std::size_t... Indices>
	requires(sizeof...(Indices) == 0)
	using at_indices = of<>;

	template<typename T>
	requires(is::type<T>::template same_as<indices::of<>>)
	using at = of<>;

	template<typename... Types>
	using append_back_types = of<Types...>;

	template<typename... Types>
	using append_front_types = of<Types...>;

	template<typename... Types>
	using indices_of_satisfying = indices::of<>;

	template<typename... Types>
	using indices_of_not_satisfying = indices::of<>;

	template<typename T>
	using indices_of_same_as = indices::of<>;

	template<typename T>
	using indices_of_not_same_as = indices::of<>;

	template<typename F>
	using indices_of_args_for_invocable_type = indices::of<>;

	template<typename F>
	using indices_of_not_args_for_invocable_type = indices::of<>;

	template<std::size_t... Indices>
	requires(sizeof...(Indices) == 0)
	using erase_at_indices = of<>;

	template<typename T>
	requires(is::type<T>::template same_as<indices::of<>>)
	using erase_at = of<>;

	template<typename... Types>
	using erase_types = of<>;

	template<typename T>
	static constexpr std::size_t count_of_same_as = 0;

	template<typename T>
	using contains_same_as_predicate = std::false_type;

	template<typename T>
	static constexpr bool contains_same_as = false;
};

template<typename... Types>
struct of {
	static constexpr std::size_t size = sizeof...(Types);
	using indieces_type = typename indices::from<0>::to<size>;
	static constexpr bool empty = false;

	template<std::size_t Index>
	using at_index = typename ::at<Index>::template of_types<Types...>;

	template<std::size_t... Indices>
	using at_indices = of<at_index<Indices>...>;

private:

	template<typename T>
	struct at_t;

	template<std::size_t... Indices>
	struct at_t<indices::of<Indices...>> {
		using type = at_indices<Indices...>;
	};
public:
	template<typename T>
	using at = typename at_t<T>::type;

	using front = at_index<0>;
	using back = at_index<size - 1>;

	template<typename... Types0>
	using append_back_types = of<Types..., Types0...>;

	template<typename... Types0>
	using append_front_types = of<Types0..., Types...>;

	template<template<typename> typename P>
	using indices_of_satisfying =
		typename indices::of_types_that_satisfy<P>
		::template of_types<Types...>;

	template<template<typename> typename P>
	using indices_of_not_satisfying =
		typename indices::of_types_that_not_satisfy<P>
		::template of_types<Types...>;

	template<typename T>
	using indices_of_same_as =
		indices_of_satisfying<
			is::type<T>::template same_as_predicate
		>;
	
	template<typename T>
	using indices_of_not_same_as =
		indices_of_not_satisfying<
			is::type<T>::template same_as_predicate
		>;

	template<typename F>
	using indices_of_args_for_invocable_type =
		indices_of_satisfying<
			is::type<F>::template invocable_with_arg_predicate
		>;

	template<typename F>
	using indices_of_are_not_args_for_invocable_type =
		indices_of_not_satisfying<
			is::type<F>::template invocable_with_arg_predicate
		>;

	template<std::size_t Index>
	using erase_at_index =
		typename erase_at<Index>::
		template of_types<Types...>;


	template<std::size_t... Indices>
	using erase_at_indices =
		at<
			typename indieces_type::
			template indices_of_not_satisfying<
				indices::of<Indices...>::template contains_equal_to_predicate
			>
		>;

private:
	template<typename T>
	struct erase_at_t;

	template<std::size_t... Indices>
	struct erase_at_t<indices::of<Indices...>> {
		using type = erase_at_indices<Indices...>;
	};
public:

	template<typename T>
	using erase_at = typename erase_at_t<T>::type;

	template<typename... Types0>
	using erase_types =
		at<
			indices_of_not_satisfying<types::of<Types0...>::template contains_type_predicate>
		>;

	template<typename Type>
	static constexpr std::size_t count_of_same_as = indices_of_same_as<Type>::size;

	// contains
	template<typename Type>
	using contains_same_as_predicate = std::bool_constant< (count_of_same_as<Type> > 0) >;

	template<typename Type>
	static constexpr bool contains_same_as = contains_same_as_predicate<Type>::value;
};

}