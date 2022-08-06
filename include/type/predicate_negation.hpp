#pragma once

#include "predicate.hpp"

namespace type {

	template<type::predicate Predicate>
	struct predicate_negation : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = ! Predicate::template for_type<Type>;

	};

} // type