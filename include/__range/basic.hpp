#pragma once

#include "../__type/is_array.hpp"
#include "../__type/remove_const.hpp"
#include "../__type/is_same_as.hpp"
#include "../__type/remove_reference.hpp"
#include "../__iterator/basic.hpp"

template<typename Type>
concept basic_range =
	(
		requires(Type& o) { { o.iterator()  } -> basic_iterator; } &&
		requires(Type& o) { o.sentinel(); }
	) ||
	__type::is_array<remove_reference<Type>>;