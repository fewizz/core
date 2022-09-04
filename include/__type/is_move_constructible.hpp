#pragma once

#include "./is_constructible_from.hpp"

template<typename Type>
concept move_constructible = constructible_from<Type, Type&&>;

namespace __type {

	template<typename Type>
	constexpr inline bool is_move_constructible = move_constructible<Type>;

}