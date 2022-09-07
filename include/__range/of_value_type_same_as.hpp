#pragma once

#include "./of_value_type_satisfying_predicate.hpp"
#include "./__type/is_same_as_predicate.hpp"

template<typename Range, typename Value>
concept range_of_value_type_same_as =
	range_of_value_type_satisfying_predicate<
		Range,
		is_same_as<Value>
	>;

// aliases

template<typename Range, typename Value>
concept range_of_value_type = range_of_value_type_same_as<Range, Value>;

template<typename Range, typename Value>
concept range_of = range_of_value_type_same_as<Range, Value>;