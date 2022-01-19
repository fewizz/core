#pragma once

#include "at_index.hpp"
#include "../types/index_of_range_of_value_type.hpp"
#include "satisfying_predicate.hpp"

namespace elements {

	template<typename ValueType>
	inline constexpr auto range_of_value_type = elements::satisfying_predicate<type::is_range_of_value_type<ValueType>>;

}