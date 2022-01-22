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
	struct is_range_of_value_type : type::predicate_marker {

		template<typename RangeType>
		static constexpr bool for_type = range::of_value_type<RangeType, ValueType>;

	};
}

#include "../meta/types/count_of_satisfying_predicate.hpp"
#include "../meta/types/are_contain_satisfying_predicate.hpp"

namespace types {

	template<typename ValueType>
	struct count_of_ranges_of_value_type : types::count_of_satisfying_predicate<type::is_range_of_value_type<ValueType>> {};

	template<typename ValueType>
	struct are_contain_range_of_value_type : types::are_contain_satisfying_predicate<type::is_range_of_value_type<ValueType>>{};

	template<typename ValueType>
	struct are_may_contain_range_of_value_type : types::are_may_contain_satisfying_predicate<type::is_range_of_value_type<ValueType>>{};

	template<typename ValueType>
	struct are_contain_one_range_of_value_type : types::are_contain_one_satisfying_predicate<type::is_range_of_value_type<ValueType>> {};

	template<typename ValueType>
	struct are_may_contain_one_range_of_value_type : types::are_may_contain_one_satisfying_predicate<type::is_range_of_value_type<ValueType>> {};
}

#include "../meta/elements/satisfying_predicate.hpp"

namespace elements {

	template<typename ValueType>
	inline constexpr auto range_of_value_type = elements::satisfying_predicate<type::is_range_of_value_type<ValueType>>;
}