#pragma once

#include "../../range/of_value_type.hpp"
#include "count_of_ranges_of_value_type.hpp"

namespace types {

	template<typename ValueType>
	struct are_contain_range_of_value_type : types::count_of_ranges_of_value_type<ValueType>::template greater_or_equals<1> {};

	template<typename ValueType>
	struct are_contain_one_range_of_value_type : types::count_of_ranges_of_value_type<ValueType>::template equals<1> {};

	template<typename ValueType>
	struct are_may_contain_one_range_of_value_type : types::count_of_ranges_of_value_type<ValueType>::template less_or_equals<1> {};

}