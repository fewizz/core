#pragma once

#include <type_traits>
#include "are_types_same.hpp"

namespace are {

template<typename... Ts>
struct types {
	template<typename T>
	using args_for_predicate = std::is_invocable<T, Ts...>;

	template<typename T>
	static constexpr bool args_for = std::is_invocable_v<T, Ts...>;
	
	static constexpr bool same = are_types_same<Ts...>;
};

template<auto... Values>
struct values {
	static constexpr bool equal = (Values == ...);
};

}