#pragma once

#include "predicate.hpp"

namespace type {

	template<type::predicate Predicate>
	struct negated_predicate {
		static constexpr bool is_type_predicate = true;

		template<typename Type>
		static constexpr bool for_type = ! Predicate::template for_type<Type>;
	};
}