#pragma once

#include <type_traits>

namespace is {

template<typename T>
struct type {
	template<typename T0>
	using same_as_predicate = std::is_same<T, T0>;

	// same_as
	template<typename T0>
	static constexpr bool same_as = std::is_same_v<T, T0>;

	template<typename T0>
	static constexpr bool not_same_as = ! std::is_same_v<T, T0>;

	// invocable_with
	template<typename... Args>
	using invocable_with_args_predicate = std::is_invocable<T, Args...>;
	
	template<typename Arg>
	using invocable_with_arg_predicate = std::is_invocable<T, Arg>;

	template<typename... Args>
	static constexpr bool invocable_with_args = invocable_with_args_predicate<Args...>::value;

	template<typename Arg>
	static constexpr bool invocable_with_arg = invocable_with_arg_predicate<Arg>::value;
	
};

template<auto V>
struct value {
	template<auto V0>
	using same_as_predicate = std::bool_constant<V == V0>;
};

}