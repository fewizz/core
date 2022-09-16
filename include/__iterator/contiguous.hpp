#pragma once

#include "./random_access.hpp"
#include "../__type/is_base.hpp"
#include "../__type/is_pointer.hpp"

struct contiguous_iterator_mark {};

template<typename Type>
concept contiguous_iterator =
	random_access_iterator<Type> &&
	(
		base_of<contiguous_iterator_mark, Type> ||
		type_is_pointer<Type>
	);