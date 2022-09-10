#pragma once

#include "./random_access.hpp"
#include "../__type/is_base.hpp"
#include "../__type/is_pointer.hpp"

struct contiguos_iterator_mark {};

template<typename Type>
concept contiguous_iterator =
	(
		random_access_iterator<Type> &&
		base_of<Type, contiguos_iterator_mark>
	) ||
	type_is_pointer<Type>;