#pragma once

#include "./is_signed_integer.hpp"
#include "./is_unsigned_integer.hpp"

template<typename Type>
concept integer = signed_integer<Type> || unsigned_integer<Type>;

namespace __type {

	template<typename Type>
	constexpr inline bool is_integer = integer<Type>;

}