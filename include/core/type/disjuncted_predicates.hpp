#pragma once

#include "predicate.hpp"

namespace type {

	template<type::predicate... Predicates>
	struct disjuncted_predicates : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = (Predicates::template for_type<Type> || ...);

	};

}