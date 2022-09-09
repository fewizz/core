#pragma once

#include "./basic.hpp"
#include "../integer.hpp"

template<typename Type>
concept random_access_iterator =
	basic_iterator<Type> &&
	requires(Type i) {
		i + nuint{}; nuint{} + i; i += nuint{};
		i - nuint{}; nuint{} - i; i -= nuint{};
		i - i;
	};