#pragma once

#include <cstddef>
#include <type_traits>
#include "at.hpp"
#include "is.hpp"
#include "indices_of.hpp"
#include "erase_at.hpp"

namespace values {

/*
size_t     size

bool       empty

value      at_index<Index>
values     at_indices<Indices...>
values     at<indices::of<Indices...>>

value      front

value      back

values     append_back_values<Values...>

values     append_front_values<Values...>

indices    indices_of_satisfying<Predicate>
indices    indices_of_not_satisfying<Predicate>

indices    indices_of_equal_to_value<Value>
indices    indices_of_not_equal_to_value<Value>

values     erase_at_index<Index>
values     erase_at_indices<Indices...>
values     erase_at<indices::of<Indices...>>

values     erase_equal_to_one_of_values<Values...>

size_t     count_of_equal_to_value<Value>

value      contains_equal_to_value_predicate<Predicate>
bool       contains_equal_to_value<Value>
*/

template<auto... Values>
struct of;

}

namespace indices {

template<std::size_t IndexFrom>
class from {
	
	template<std::size_t IndexTo>
	struct to_t {
		static constexpr auto size = IndexTo - IndexFrom;

		template<std::size_t... Indices>
		struct result;

		template<std::size_t... Indices>
		requires(sizeof...(Indices) == size)
		struct result<Indices...> {
			using type = of<Indices...>;
		};

		template<std::size_t... Indices>
		requires(sizeof...(Indices) == 0 && size > 0)
		struct result<Indices...> {
			using type = typename result<IndexFrom>::type;
		};

		template<std::size_t... Indices>
		requires(sizeof...(Indices) > 0 && sizeof...(Indices) != size)
		struct result<Indices...> {
			using type = typename result<Indices..., IndexFrom + sizeof...(Indices)>::type;
		};
	};
public:

	template<std::size_t IndexTo>
	using to = typename to_t<IndexTo>::template result<>::type;
}; // from

} // indices

namespace values {

template<>
struct of<> {
	static constexpr std::size_t size = 0;
	static constexpr bool empty = true;
	using indices_type = indices::from<0>::to<size>;

	template<std::size_t... Indices>
	requires(sizeof...(Indices) == 0)
	using at_indices = of<>;

	template<typename T>
	requires(is::type<T>::template same_as<indices::of<>>)
	using at = of<>;

	template<auto... Values>
	using append_back_values = of<Values...>;

	template<auto... Values>
	using append_front_values = of<Values...>;

	template<typename Predicate>
	using indices_of_values_that_satisfy = of<>;

	template<typename Predicate>
	using indices_of_not_satisfying = of<>;

	template<auto Value>
	using indices_of_equal_to_value = of<>;

	template<auto Value>
	using indices_of_not_equal_to_value = of<>;

	template<std::size_t... Indices>
	requires(sizeof...(Indices) == 0)
	using erase_at_indices = of<>;

	template<typename T>
	requires(is::type<T>::template same_as<indices::of<>>)
	using erase_at = of<>;

	template<auto... Values>
	requires(sizeof...(Values) == 0)
	using erase_equal_to_one_of_values = of<>;

	template<auto Value>
	static constexpr std::size_t count_of_equal_to = 0;

	template<auto Value>
	using contains_equal_to_value_predicate = std::false_type;

	template<auto Value>
	static constexpr bool contains_equal_to_value = false;
};

template<auto... Values>
struct of {
	static constexpr std::size_t size = sizeof...(Values);
	static constexpr bool empty = false;

	using indieces_type = typename indices::from<0>::to<size>;

	template<std::size_t Index>
	static constexpr auto at_index = ::at<Index>::template of_values<Values...>;

	template<std::size_t... Indices>
	using at_indices = of<at_index<Indices>...>;

private:
	template<typename T>
	struct at_t;

	template<std::size_t... Indices>
	struct at_t<of<Indices...>> {
		using type = at_indices<Indices...>;
	};
public:

	template<typename T>
	using at = typename at_t<T>::type;

	template<auto... Values0>
	using append_back_values = of<Values..., Values0...>;

	template<auto... Values0>
	using append_front_values = of<Values0..., Values...>;

	static constexpr auto front = at_index<0>;

	static constexpr auto back = at_index<size - 1>;

	template<template<auto> typename P>
	using indices_of_satisfying = typename
		indices::of_values_that_satisfy<P>
		::template of_values<Values...>;

	template<template<auto> typename P>
	using indices_of_not_satisfying = typename
		indices::of_values_that_not_satisfy<P>
		::template of_values<Values...>;

	template<auto Value>
	using indices_of_equal_to_value =
		indices_of_satisfying<
			is::value<Value>::template same_as_predicate
		>;

	template<auto Value>
	using indices_of_not_equal_to_value =
		indices_of_not_satisfying<
			is::value<Value>::template same_as_predicate
		>;

	template<std::size_t Index>
	using erase_at_index = typename erase_at<Index>::template of_values<Values...>;

	template<std::size_t... Indices>
	using erase_at_indices = at<
			typename indieces_type::
			template indices_of_not_satisfying<
				indices::of<Indices...>::template contains_equal_to_value_predicate
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

	template<auto... Values0>
	using erase_equal_to_one_of_values = at<
		indices_of_not_satisfying<values::of<Values0...>::template contains_equal_to_value_predicate>
	>;
	
	template<auto Value>
	static constexpr std::size_t count_of_equal_to_value = indices_of_equal_to_value<Value>::size;

	template<auto Value>
	using contains_equal_to_value_predicate = std::bool_constant< (count_of_equal_to_value<Value> > 0) >;

	template<auto Value>
	static constexpr bool contains_equal_to_value = contains_equal_to_value_predicate<Value>::value;
};

} // values