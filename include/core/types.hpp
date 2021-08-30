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
std::size_t size

bool        empty

type        type_at_index<Index>
types       types_at_indices<Indices...>
types       types_at<indices::of<Indices...>>

type        front

type        back

types       append_back_types<Types...>

types       append_front_types<Types...>

indices     indices_of_types_that_satisfy<Predicate>
indices     indices_of_types_that_not_satisfy<Predicate>

indices     indices_of_types_same_as<Type>
indices     indices_of_types_not_same_as<Type>

indices     indices_of_types_that_are_args_for_invocable_type<Type>
indices     indices_of_types_that_are_not_args_for_invocable_type<Type>

types       erase_at_indices<Indices...>
types       erase_at<indices::of<Indices...>>

types       erase_types<Types...>

std::size_t count<Type>

std::size_t index_of_first_type<Type>

type        contains_type_predicate<Type>
bool        contains_type<Type>
*/

template<typename... Ts> 
struct of;

template<>
struct of<> {
	static constexpr std::size_t size = 0;
	static constexpr bool empty = true;

	template<std::size_t... Indices>
	requires(sizeof...(Indices) == 0)
	using types_at_indices = of<>;

	template<typename T>
	requires(is::type<T>::template same_as<indices::of<>>)
	using types_at = of<>;

	template<typename... Types>
	using append_back_types = of<Types...>;

	template<typename... Types>
	using append_front_types = of<Types...>;

	template<typename... Types>
	using indices_of_types_that_satisfy = indices::of<>;

	template<typename... Types>
	using indices_of_types_that_not_satisfy = indices::of<>;

	template<typename T>
	using indices_of_types_same_as = indices::of<>;

	template<typename T>
	using indices_of_types_not_same_as = indices::of<>;

	template<typename F>
	using indices_of_types_that_are_args_for_invocable_type = indices::of<>;

	template<typename F>
	using indices_of_types_that_are_not_args_for_invocable_type = indices::of<>;

	template<std::size_t... Indices>
	requires(sizeof...(Indices) == 0)
	using erase_at_indices = of<>;

	template<typename T>
	requires(is::type<T>::template same_as<indices::of<>>)
	using erase_at = of<>;

	template<typename... Types>
	using erase_types = of<>;

	template<typename T>
	static constexpr std::size_t count = 0;

	template<typename T>
	using contains_type_predicate = std::false_type;

	template<typename T>
	static constexpr bool contains_type = false;
};

template<typename... Types>
struct of {
	static constexpr std::size_t size = sizeof...(Types);
	using indieces_type = typename indices::from<0>::to<size>;
	static constexpr bool empty = false;

	template<std::size_t Index>
	using type_at_index = typename ::at<Index>::template of_types<Types...>;

	template<std::size_t... Indices>
	using types_at_indices = of<type_at_index<Indices>...>;

private:

	template<typename T>
	struct types_at_t;

	template<std::size_t... Indices>
	struct types_at_t<indices::of<Indices...>> {
		using type = types_at_indices<Indices...>;
	};
public:
	template<typename T>
	using types_at = typename types_at_t<T>::type;

	using front = type_at_index<0>;
	using back = type_at_index<size - 1>;

	template<typename... Types0>
	using append_back_types = of<Types..., Types0...>;

	template<typename... Types0>
	using append_front_types = of<Types0..., Types...>;

	template<template<typename> typename P>
	using indices_of_types_that_satisfy =
		typename indices::of_types_that_satisfy<P>
		::template of_types<Types...>;

	template<template<typename> typename P>
	using indices_of_types_that_not_satisfy =
		typename indices::of_types_that_not_satisfy<P>
		::template of_types<Types...>;

	template<typename T>
	using indices_of_types_same_as =
		indices_of_types_that_satisfy<
			is::type<T>::template same_as_predicate
		>;
	
	template<typename T>
	using indices_of_types_not_same_as =
		indices_of_types_that_not_satisfy<
			is::type<T>::template same_as_predicate
		>;

	template<typename F>
	using indices_of_types_that_are_args_for_invocable_type =
		indices_of_types_that_satisfy<
			is::type<F>::template invocable_with_arg_predicate
		>;

	template<typename F>
	using indices_of_types_that_are_not_args_for_invocable_type =
		indices_of_types_that_not_satisfy<
			is::type<F>::template invocable_with_arg_predicate
		>;

	template<std::size_t Index>
	using erase_at_index =
		typename erase_at<Index>::
		template of_types<Types...>;


	template<std::size_t... Indices>
	using erase_at_indices =
		types_at<
			typename indieces_type::
			template indices_of_values_that_not_satisfy<
				indices::of<Indices...>::template contains_value_predicate
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
		types_at<
			indices_of_types_that_not_satisfy<types::of<Types0...>::template contains_type_predicate>
		>;

	template<typename Type>
	static constexpr std::size_t index_of_first_type = indices_of_types_same_as<Type>::template value_at<0>;

	template<typename Type>
	static constexpr std::size_t count = indices_of_types_same_as<Type>::size;

	// contains
	template<typename Type>
	using contains_type_predicate = std::bool_constant< (count<Type> > 0) >;

	template<typename Type>
	static constexpr bool contains_type = contains_type_predicate<Type>::value;
};

}