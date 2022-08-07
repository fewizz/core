#pragma once

#include "pass_satisfying_type_predicate.hpp"
#include "../type/predicate_negation.hpp"

namespace elements {

	template<type::predicate Predicate>
	inline constexpr auto pass_not_satisfying_type_predicate {
		elements::pass_satisfying_type_predicate_t<
			type::predicate_negation<Predicate>
		>{}
	};

} // elements