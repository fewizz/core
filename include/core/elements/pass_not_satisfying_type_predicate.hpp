#pragma once

#include "pass_satisfying_type_predicate.hpp"
#include "../type/negated_predicate.hpp"

namespace elements {

	template<type::predicate Predicate>
	inline constexpr auto pass_not_satisfying_type_predicate = elements::pass_satisfying_type_predicate_t<type::negated_predicate<Predicate>>{};

}