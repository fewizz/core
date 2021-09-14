#pragma once

#include "are_types_same.hpp"
#include <type_traits>

template<typename Type0>
struct is_same {

	template<typename Type1>
	static constexpr bool as = are_types_same<Type0, Type1>;

	template<typename Type1>
	using as_predicate = std::bool_constant<as<Type1>>;
};