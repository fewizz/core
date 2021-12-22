#pragma once

#include "at_index.hpp"
#include "../types/indices_of_type.hpp"
#include "../values/at_index.hpp"
#include "../types/index_of_satisfying_predicate.hpp"

namespace elements {

	template<type::predicate Predicate>
	struct satisfying_predicate {
	
		template<typename... Types>
		static constexpr decltype(auto) for_elements_of(Types&&... elements) {
			return elements::at_index<
				types::index_of_satisfying_predicate<Predicate>::template for_types_of<Types...>
			>::template for_elements_of<Types...>(forward<Types>(elements)...);
		}
	};
}