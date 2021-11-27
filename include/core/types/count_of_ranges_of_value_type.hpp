#pragma once

#include "indices_of_ranges_of_value_type.hpp"
#include "count_of_satisfying_predicate.hpp"
#include "count_of_satisfying_predicate.hpp"
#include "../type/range.hpp"

namespace types {

	template<typename ValueType>
	struct count_of_ranges_of_value_type : types::count_of_satisfying_predicate<type::is_range_of_value_type<ValueType>> {
		//using ignore_reference = types::count_of_satisfying_predicate<type::ignore_reference<type::is_range_of_value_type<ValueType>>>;
	};

}