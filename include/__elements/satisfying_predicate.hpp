#pragma once

#include "./at_index.hpp"
#include "../__type/predicate.hpp"
#include "../__types/count_of_satisfying_predicate.hpp"
#include "../__types/index_of_satisfying_predicate.hpp"

namespace __elements {

	template<type_predicate auto Predicate>
	struct satisfying_predicate_t {

		template<typename... Types>
		requires(
			count_of_satisfying_predicate<Predicate>.template
			for_types<Types...>() == 1
		)
		constexpr decltype(auto) operator () (Types&&... elements) const {
			constexpr auto index =
				__types::index_of_satisfying_predicate<Predicate>::
				template for_types<Types...>;

			return __elements::at_index<index>(forward<Types>(elements)...);
		}

	};

	template<type_predicate auto Predicate>
	inline constexpr auto satisfying_predicate =
		__elements::satisfying_predicate_t<Predicate>
	{};

}