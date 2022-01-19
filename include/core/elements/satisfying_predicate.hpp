#pragma once

#include "at_index.hpp"
#include "../types/indices_of_type.hpp"
#include "../types/count_of_satisfying_predicate.hpp"
#include "../values/at_index.hpp"
#include "../types/index_of_satisfying_predicate.hpp"

namespace elements {

	template<type::predicate Predicate>
	struct satisfying_predicate_t {
	
		template<typename... Types>
		requires(types::count_of_satisfying_predicate<Predicate>::template for_types_of<Types...> == 1)
		constexpr auto& for_elements_of(const Types&... elements) const {
			return elements::at_index<
				types::index_of_satisfying_predicate<Predicate>::template for_types_of<Types...>
			>.template for_elements_of<Types...>(elements...);
		}

		template<typename... Types>
		constexpr auto& operator () (const Types&... elements) const {
			return for_elements_of<Types...>(elements...);
		}
	};

	template<type::predicate Predicate>
	inline constexpr auto satisfying_predicate = elements::satisfying_predicate_t<Predicate>{};
}