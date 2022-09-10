#pragma once

#include "./basic.hpp"
#include "../integer.hpp"
#include "../__type/is_same_as.hpp"

template<typename Type>
concept random_access_iterator =
	basic_iterator<Type> &&
	requires(Type i) {
		i + nuint{}; nuint{} + i; i += nuint{};
		i - nuint{}; i -= nuint{};
		{ i - i } -> not_same_as<void>;
	};