#pragma once

#include "../types/are_same.hpp"

namespace type {

	template<typename T>
	concept range = requires(T t) {
		t.begin();
		t.end();
		typename T::value_type;
	};

	template<typename ValueType>
	struct is_range_of_value_type {
		static constexpr bool is_type_predicate = true;

		template<typename RangeType>
		static constexpr bool for_type_of = false;

		template<typename RangeType>
		requires(range<RangeType>)
		static constexpr bool for_type_of<RangeType> = types::are_same::for_types_of<ValueType, typename RangeType::value_type>;
	};

}