#pragma once

#include <type_traits>

namespace u {

namespace internal {

template<typename T>
constexpr std::size_t count() {
	return 0;
}

template<typename T, typename T0, typename... Ts>
constexpr std::size_t count() {
	std::size_t v = std::is_same_v<T, T0> ? 1 : 0;

	if constexpr(sizeof...(Ts) != 0) {
		v += count<T, Ts...>();
	}

	return v;
}

} // internal

template<typename T, typename... Ts>
constexpr inline std::size_t count = internal::count<T, Ts...>();

}