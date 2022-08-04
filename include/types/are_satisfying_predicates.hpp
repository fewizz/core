#pragma once

#include "predicate.hpp"

namespace types {
	
	template<types::predicate... Predicates>
	struct are_satisfying_predicates {

		template<typename... Types>
		static constexpr bool for_types =
			(Predicates::template for_types<Types...> && ...);

	};

} // types