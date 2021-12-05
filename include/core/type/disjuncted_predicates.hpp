#pragma once

#include "predicate.hpp"

namespace type {
	template<type::predicate... Predicates>
	struct disjuncted_predicates {
		static constexpr bool is_type_predicate = true;

		template<typename Type>
		static constexpr bool for_type_of = (Predicates::template for_type_of<Type> || ...);
	};
}