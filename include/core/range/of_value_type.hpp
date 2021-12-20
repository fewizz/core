#pragma once

#include "basic.hpp"

namespace range {
	template<typename RangeType, typename ValueType>
	concept of_value_type =
		range::basic<RangeType> &&
		type::is_same_as<typename type::remove_reference::for_type<RangeType>::value_type>::template for_type<ValueType>;

}

namespace type {

	template<typename ValueType>
	struct is_range_of_value_type {
		static constexpr bool is_type_predicate = true;

		template<typename RangeType>
		static constexpr bool for_type = range::of_value_type<RangeType, ValueType>;
	};
}