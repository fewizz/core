#pragma once

#include <type_traits>
#include <tuple>
#include <utility>

namespace u {

namespace internal {

template<typename T, typename... Ts>
struct index_of_wrapper;

template<typename T>
struct index_of_wrapper<T> {
	template<std::size_t Index = 0, std::size_t... Indices>
	static constexpr auto create() {
		return std::index_sequence<Indices...>{};
	}
};

template<typename T, typename T0, typename... Ts>
struct index_of_wrapper<T, T0, Ts...> {
	template<std::size_t Index = 0, std::size_t... Indices>
	static constexpr auto create() {
		constexpr bool same = std::is_same_v<T, T0>;
	
		if constexpr(same) {
			return index_of_wrapper<T, Ts...>::template create<Index + 1, Indices..., Index>();
		}
		else {
			return index_of_wrapper<T, Ts...>::template create<Index + 1, Indices...>();
		}
	}
};

} // internal

template<typename T, typename... Ts>
using indexes_of = decltype(internal::template index_of_wrapper<T, Ts...>::create());

} // u