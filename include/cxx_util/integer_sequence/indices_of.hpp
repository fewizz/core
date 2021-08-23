#pragma once

#include <utility>

namespace u {

namespace integer_sequence_internal {

template<
	template<auto> typename P, // predicate
	auto... Value // sequence values
>
struct indices_of_wrapper;

template<template<auto> typename P>
struct indices_of_wrapper<P> {
	template<std::size_t Index = 0, std::size_t... Indices>
	static constexpr auto create() {
		return std::index_sequence<Indices...>{};
	}
};

template<template<auto> typename P, auto Value, auto... Values>
struct indices_of_wrapper<P, Value, Values...> {

	template<std::size_t Index = 0, std::size_t... Indices>
	static constexpr auto create() {
		constexpr bool p = P<Value>::value;//std::is_same_v<T, T0>;
	
		if constexpr(p) {
			return indices_of_wrapper<P, Values...>::template create<Index + 1, Indices..., Index>();
		}
		else {
			return indices_of_wrapper<P, Values...>::template create<Index + 1, Indices...>();
		}
	}
};

} // internal

template<template<auto> typename P, auto... Values>
using indices_of = decltype(integer_sequence_internal::indices_of_wrapper<P, Values...>::create());

}