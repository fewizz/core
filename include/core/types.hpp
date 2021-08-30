#pragma once

#include "is.hpp"
#include "are.hpp"
#include "at.hpp"
#include "indices_of.hpp"
#include "values.hpp"

namespace types {

template<typename... Ts> 
struct of;

template<>
struct of<> {
	static constexpr std::size_t size = 0;
	static constexpr bool empty = true;

	template<class T>
	static constexpr bool contains = false;

	template<class T>
	using append_back = of<T>;

	template<class T>
	using append_front = of<T>;

	template<typename T>
	static constexpr std::size_t count = 0;

	template<typename T>
	using indices_of_same_as = indices::of<>;

	template<typename T>
	using indices_of_not_same_as = indices::of<>;

	template<typename... Args>
	using indices_of_invocable_with = indices::of<>;

	template<typename... Args>
	using indices_of_not_invocable_with = indices::of<>;
};

template<typename... Ts>
struct of {
	static constexpr std::size_t size = sizeof...(Ts);
	using indieces_type = typename indices::from<0>::to<size>;
	static constexpr bool empty = false;

	template<std::size_t Index>
	using type_at = typename ::at<Index>::template of_types<Ts...>;

	template<std::size_t... Indices>
	using types_at = of<type_at<Indices>...>;

private:

	template<typename T>
	struct types_at_indices_t;

	template<std::size_t... Indices>
	struct types_at_indices_t<indices::of<Indices...>> {
		using type = types_at<Indices...>;
	};
public:
	template<typename T>
	using types_at_indices = types_at_indices_t<T>;

	using front = type_at<0>;
	using back = type_at<size - 1>;

	template<typename... Ts0>
	using append_back = of<Ts..., Ts0...>;

	template<typename... Ts0>
	using append_front = of<Ts0..., Ts...>;

	template<template<typename> typename P>
	using indices_of_types_that_satisfy =
		typename indices::of_types_that_satisfy<P>
		::template of_types<Ts...>;

	template<template<typename> typename P>
	using indices_of_types_that_not_satisfy =
		typename indices::of_types_that_not_satisfy<P>
		::template of_types<Ts...>;

	template<typename T>
	using indices_of_same_as =
		indices_of_types_that_satisfy<
			is::type<T>::template same_as_predicate
		>;
	
	template<typename T>
	using indices_of_not_same_as =
		indices_of_types_that_not_satisfy<
			is::type<T>::template same_as_predicate
		>;

	template<typename... Args>
	using indices_of_invocable_with =
		indices_of_types_that_satisfy<
			are::types<Args...>::template args_for_predicate
		>;

	template<typename... Args>
	using indices_of_not_invocable_with =
		indices_of_types_that_not_satisfy<
			are::types<Args...>::template args_for_predicate
		>;

	private:
	template<typename T>
	struct erase_at_indices_t;

	template<std::size_t... Indices>
	struct erase_at_indices_t<indices::of<Indices...>> {
		using type =
			types_at_indices<
				typename indieces_type::
				template indices_of_values_that_not_satisfy<
					indices::of<Indices...>::template contains_predicate
				>
			>;
	};
public:

	template<typename T>
	using erase_at_indices = typename erase_at_indices_t<T>::type;

	template<typename T>
	static constexpr std::size_t index_of_first_type = indices_of_same_as<T>::template value_at<0>;


	template<typename Type>
	static constexpr auto count = indices_of_same_as<Type>::size;

	// contains
	template<typename Type>
	using contains_predicate = std::bool_constant< (count<Type> > 0) >;

	template<typename Type>
	static constexpr bool contains = contains_predicate<Type>::value;
};

}