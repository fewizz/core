#pragma once

#include "../range/of_value_type.hpp"
#include "index_of_satisfying_predicate.hpp"

namespace types {

	template<typename ValueType>
	struct index_of_range_of_value_type
		: types::index_of_satisfying_predicate<type::is_range_of_value_type<ValueType>> {};

}