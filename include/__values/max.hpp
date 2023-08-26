#pragma once

#include "../integer.hpp"

namespace __values {

	template<typename Type>
	struct max_instead_of_ge {
		Type value;

		constexpr auto operator > (auto other) {
			return value > other ? value : other;
		}
	};
	template<typename Type>
	max_instead_of_ge(Type) -> max_instead_of_ge<Type>;

	template<auto V0, auto... Vs>
	static constexpr auto max = (max_instead_of_ge{Vs} > ... > V0);

}