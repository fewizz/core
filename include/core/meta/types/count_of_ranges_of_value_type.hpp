#pragma once

#include "count_of_satisfying_predicate.hpp"
#include "../../range/of_value_type.hpp"

namespace types {

	template<typename ValueType>
	struct count_of_ranges_of_value_type : types::count_of_satisfying_predicate<type::is_range_of_value_type<ValueType>> {};

}	