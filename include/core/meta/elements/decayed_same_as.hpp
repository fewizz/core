#pragma once

#include "satisfying_predicate.hpp"
#include "../type/is_decayed_same_as.hpp"

namespace elements {

	template<typename Type>
	inline constexpr auto decayed_same_as = elements::satisfying_predicate<
		type::is_decayed_same_as_type<Type>
	>;

} // elements