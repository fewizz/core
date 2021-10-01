#pragma once

#include "../range.hpp"
#include "indices_of_types_satisfying_predicate.hpp"

template<typename ValueType>
struct contain_range_of_value_type {

	template<typename... Types>
	static constexpr bool for_types_of =
		!
		indices_of_types_satisfying_predicate<
			is_range_value_type<ValueType>
		>
		::template for_types_of<Types...>
		::empty;
};