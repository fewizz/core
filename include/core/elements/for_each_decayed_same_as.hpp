#pragma once

#include "for_each_satisfying_type_predicate.hpp"
#include "../type/is_decayed_same_as.hpp"

namespace elements {

	template<typename Type>
	inline constexpr auto for_each_decayed_same_as = elements::for_each_satisfying_type_predicate<type::is_decayed_same_as_type<Type>>;

}