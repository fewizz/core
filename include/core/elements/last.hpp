#pragma once

#include "at_index.hpp"

namespace elements {

	struct last {
		template<typename... Types>
		static decltype(auto) for_elements_of(Types&&... elements) {
			return elements::at_index<sizeof...(Types) - 1>
				.for_elements_of(
					forward<Types>(elements)...
				);
		}
	};
}