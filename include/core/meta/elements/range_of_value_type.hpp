#pragma once

#include "satisfying_predicate.hpp"
#include "../types/index_of_range_of_value_type.hpp"

namespace elements {

	template<typename ValueType>
	inline constexpr auto range_of_value_type = elements::satisfying_predicate<type::is_range_of_value_type<ValueType>>;

}