#pragma once

#include <type_traits>
#include <tuple>
#include <utility>

namespace u {

namespace internal {

template<template<typename T> typename P, typename... Ts>
struct indices_of_wrapper;

template<template<typename T> typename P>
struct indices_of_wrapper<P> {
	template<std::size_t Index = 0, std::size_t... Indices>
	static constexpr auto create() {
		return std::index_sequence<Indices...>{};
	}
};

template<template<typename T> typename P, typename T0, typename... Ts>
struct indices_of_wrapper<P, T0, Ts...> {

	template<std::size_t Index = 0, std::size_t... Indices>
	static constexpr auto create() {
		constexpr bool p = P<T0>::value;//std::is_same_v<T, T0>;
	
		if constexpr(p) {
			return indices_of_wrapper<P, Ts...>::template create<Index + 1, Indices..., Index>();
		}
		else {
			return indices_of_wrapper<P, Ts...>::template create<Index + 1, Indices...>();
		}
	}
};

} // internal

template<template<typename T> typename P, typename... Ts>
using indices_of = decltype(internal::indices_of_wrapper <P, Ts...>::create());

} // u