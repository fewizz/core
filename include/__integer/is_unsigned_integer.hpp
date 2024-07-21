#pragma once

#include "../__type/is_same_as.hpp"

template<typename Type>
concept unsigned_integer = __type::is_same_as<
	uint_of_size_of<Type>, Type
>;


namespace __type {
	
	template<typename Type>
	constexpr inline bool is_unsigned_integer = unsigned_integer<Type>;

}