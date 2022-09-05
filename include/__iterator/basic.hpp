#pragma once

#include "../__type/remove_reference.hpp"
#include "../__type/remove_const.hpp"

template<typename Type>
concept basic_iterator =
	requires(Type& o) { *o; } &&
	requires(remove_const<remove_reference<Type>>& o) { ++o; };