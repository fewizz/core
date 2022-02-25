#pragma once

#include "at_index.hpp"
#include "../type/predicate.hpp"
#include "../types/count_of_satisfying_predicate.hpp"
#include "../types/index_of_satisfying_predicate.hpp"

namespace elements {

	template<type::predicate Predicate>
	struct satisfying_predicate_t {

		template<typename... Types>
		requires(types::count_of_satisfying_predicate<Predicate>::template for_types<Types...> == 1)
		constexpr decltype(auto) operator () (Types&&... elements) const {
			constexpr auto index = types::index_of_satisfying_predicate<Predicate>::template for_types<Types...>;
			return elements::at_index<index>(forward<Types>(elements)...);
		}

	};

	template<type::predicate Predicate>
	inline constexpr auto satisfying_predicate = elements::satisfying_predicate_t<Predicate>{};

}