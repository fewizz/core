#pragma once

#include "./pass_satisfying_type_predicate.hpp"
#include "../__type/predicate.hpp"

namespace __elements {

	template<type_predicate auto Predicate>
	inline constexpr auto pass_not_satisfying_type_predicate {
		__elements::pass_satisfying_type_predicate_t<
			!Predicate
		>{}
	};

}