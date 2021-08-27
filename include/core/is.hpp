#pragma once

#include <type_traits>

namespace is {

template<typename T>
struct type {
	template<typename T0>
	using same_as_predicate = std::is_same<T, T0>;

	template<typename T0>
	static constexpr bool same_as = std::is_same_v<T, T0>;

	template<typename... Args>
	using invocable_with_t = std::is_invocable<T, Args...>;

	template<typename... Args>
	static constexpr bool invocable_with = std::is_invocable_v<T, Args...>;
};

}