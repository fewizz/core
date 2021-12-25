#pragma once

#include "at_index.hpp"
#include "../types/indices_of_type.hpp"
#include "../types/count_of_satisfying_predicate.hpp"
#include "../values/at_index.hpp"
#include "../types/index_of_satisfying_predicate.hpp"

namespace elements {

	template<type::predicate Predicate>
	struct satisfying_predicate {
	
		template<typename... Types>
		requires(types::count_of_satisfying_predicate<Predicate>::template for_types_of<Types...> == 1)
		static constexpr decltype(auto) for_elements_of(Types&&... elements) {
			return elements::at_index<
				types::index_of_satisfying_predicate<Predicate>::template for_types_of<Types...>
			>::template for_elements_of<Types...>(forward<Types>(elements)...);
		}

		using ignore_reference = elements::satisfying_predicate<type::modified_predicate<Predicate, type::remove_reference>>;
		using ignore_const = elements::satisfying_predicate<type::modified_predicate<Predicate, type::remove_const>>;
	};
}