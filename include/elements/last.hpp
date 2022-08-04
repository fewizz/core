#pragma once

#include "at_index.hpp"

namespace elements {

	struct last_t {

		template<typename... Types>
		requires(sizeof...(Types) > 0)
		decltype(auto) operator () (Types&&... elements) const {
			return elements::at_index<sizeof...(Types) - 1>(
				forward<Types>(elements)...
			);
		}

	};

	inline constexpr auto last = last_t{};
}