#pragma once

#include <type_traits>
#include "are_types_same.hpp"

namespace is {

template<typename T>
struct type {

	// same_as
	template<typename T0>
	using same_as_predicate = are_types_same_predicate<T, T0>;

	template<typename T0>
	static constexpr bool same_as = are_types_same<T, T0>;

	template<typename T0>
	static constexpr bool not_same_as = ! same_as<T>;

	template<typename T0>
	using same_as_remove_cv_ref_predicate = std::is_same<T, std::remove_cvref_t<T0>>;

	template<typename T0>
	static constexpr bool same_as_remove_cv_ref = same_as_remove_cv_ref_predicate<T0>::value;

	// enum // I don't like this _type postfix
	using enum_type_predicate = std::is_enum<T>;

	// same_as
	static constexpr bool enum_type = enum_type_predicate::value;

	template<typename T0>
	static constexpr bool not_enum_type = ! enum_type;

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