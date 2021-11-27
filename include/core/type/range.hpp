#pragma once

#include "is_same_as.hpp"
#include "remove_reference.hpp"

namespace type {

	template<typename T>
	concept range = requires(T t) {
		t.begin();
		t.end();
		typename remove_reference::for_type_of<T>::value_type;
	};

	template<typename RangeType, typename ValueType>
	concept range_of_value_type =
		range<RangeType> && type::is_same_as<typename remove_reference::for_type_of<RangeType>::value_type>::template for_type_of<ValueType>;

	template<typename ValueType>
	struct is_range_of_value_type {
		static constexpr bool is_type_predicate = true;

		template<typename RangeType>
		static constexpr bool for_type_of = range_of_value_type<RangeType, ValueType>;
	};

}