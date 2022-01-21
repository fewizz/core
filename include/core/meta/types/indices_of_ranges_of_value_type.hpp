#pragma once

#include "indices_of_satisfying_predicate.hpp"
#include "../../range/of_value_type.hpp"

namespace types {

	template<typename ValueType>
	struct indices_of_ranges_of_value_type
		: types::indices_of_satisfying_predicate<
			type::is_range_of_value_type<ValueType>
		> {};

}