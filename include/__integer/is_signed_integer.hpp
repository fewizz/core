#pragma once

#include "../__type/is_same_as.hpp"

template<typename Type>
concept signed_integer = __type::is_same_as<
	int_of_size_of<Type>, Type
>;

namespace __type {

	template<typename Type>
	constexpr inline bool is_signed_integer = signed_integer<Type>;

}