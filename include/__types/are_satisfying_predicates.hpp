#pragma once

#include "./predicate.hpp"

namespace __types {
	
	template<types_predicate auto... Predicates>
	struct are_satisfying_predicates {

		template<typename... Types>
		static constexpr bool for_types =
			(Predicates.template for_types<Types...>() && ...);

	};

}