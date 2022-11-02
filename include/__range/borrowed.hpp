#pragma once

#include "./basic.hpp"
#include "../__type/is_base.hpp"
#include "../__type/is_reference.hpp"

template<typename Type>
concept borrowed_range =
	basic_range<Type> &&
	(
		(
			requires(Type type) {
				{ Type::is_borrowed_range } -> same_as<const bool&>;
			} &&
			Type::is_borrowed_range
		) ||
		type_is_reference<Type>
	);