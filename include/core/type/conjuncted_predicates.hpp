#pragma once

#include "predicate.hpp"

namespace type {
	template<type::predicate... Predicates>
	struct conjuncted_predicates {
		static constexpr bool is_type_predicate = true;

		template<typename Type>
		static constexpr bool for_type = (Predicates::template for_type<Type> && ...);
	};
}