#pragma once

#include <type_traits>

namespace u {

template<typename T>
struct is {
	T value;

	is(T v) : value{ v }{}

	template<typename T0>
	using same_as = std::is_same<T, T0>;

	template<typename... Args>
	using invocable_with = std::is_invocable<T, Args...>;

	template<auto... Values>
	constexpr bool contains_in() {
		return ((Values == value) || ... );
	}
};

}