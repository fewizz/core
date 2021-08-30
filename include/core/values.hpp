#pragma once

#include <cstddef>
#include <type_traits>
#include "at.hpp"
#include "is.hpp"
#include "indices_of.hpp"
#include "erase_at.hpp"

namespace values {

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
	using indices = indices::from<0>::to<size>;

	// append back
	template<auto... Values>
	using append_back_values = of<Values...>;

	// append front
	template<auto... Values>
	using append_front_values = of<Values...>;

	template<auto Value>
	static constexpr std::size_t count = 0;

	// contains
	template<auto Value>
	using contains_value_predicate = std::false_type;

	template<auto Value>
	static constexpr bool contains_value = false;
};

template<auto... Values>
struct of {
	static constexpr std::size_t size = sizeof...(Values);
	using indieces_type = typename indices::from<0>::to<size>;

	// append back
	template<auto... Values0>
	using append_back_values = of<Values..., Values0...>;

	// append front
	template<auto... Values0>
	using append_front_values = of<Values0..., Values...>;

	// at
	template<std::size_t Index>
	static constexpr auto value_at_index = ::at<Index>::template of_values<Values...>;

	template<std::size_t... Indices>
	using values_at_indices = of<value_at_index<Indices>...>;

private:
	template<typename T>
	struct value_at_t;

	template<std::size_t... Indices>
	struct value_at_t<of<Indices...>> {
		using type = values_at_indices<Indices...>;
	};
public:

	template<typename T>
	using value_at = typename value_at_t<T>::type;

	// front
	static constexpr auto front_value = value_at_index<0>;

	// back
	static constexpr auto back_value = value_at_index<size - 1>;

	// indices_of_values_that_satisfy
	template<template<auto> typename P>
	using indices_of_values_that_satisfy = typename
		indices::of_values_that_satisfy<P>
		::template of_values<Values...>;

	// indices_of_values_that_not_satisfy
	template<template<auto> typename P>
	using indices_of_values_that_not_satisfy = typename
		indices::of_values_that_not_satisfy<P>
		::template of_values<Values...>;

	// indices_of_same_as
	template<auto Value>
	using indices_of_values_same_as =
		indices_of_values_that_satisfy<
			is::value<Value>::template same_as_predicate
		>;

	// indices_of_not_same_as
	template<auto Value>
	using indices_of_values_not_same_as =
		indices_of_values_that_not_satisfy<
			is::value<Value>::template same_as_predicate
		>;

	// count
	template<auto Value>
	static constexpr std::size_t values_count = indices_of_values_same_as<Value>::size;

	// contains
	template<auto Value>
	using contains_predicate = std::bool_constant< (values_count<Value> > 0) >;

	template<auto Value>
	static constexpr bool contains_value = contains_predicate<Value>::value;

	// erase_at
	template<std::size_t Index>
	using erase_value_at_index = typename erase_at<Index>::template of_values<Values...>;

	template<std::size_t... Indices>
	using erase_values_at_indices = value_at<
			typename indieces_type::
			template indices_of_values_that_not_satisfy<
				indices::of<Indices...>::template contains_predicate
			>
		>;

private:
	template<typename T>
	struct erase_values_at_t;

	template<std::size_t... Indices>
	struct erase_values_at_t<indices::of<Indices...>> {
		using type = erase_values_at_indices<Indices...>;
	};
public:

	template<typename T>
	using erase_values_at = typename erase_values_at_t<T>::type;
};

} // values