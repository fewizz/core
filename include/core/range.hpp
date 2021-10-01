#pragma once

#include "meta/are_types_same.hpp"

template<typename T>
concept range = requires(T t) {
	t.begin();
	t.end();
	typename T::value_type;
};

template<typename ValueType>
struct is_range_value_type {

	template<typename RangeType>
	static constexpr bool for_type_of = are_types_same<ValueType, typename RangeType::value_type>;
};