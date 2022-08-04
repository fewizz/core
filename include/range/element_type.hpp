#pragma once

#include "./basic.hpp"
#include "./begin.hpp"
#include "../expression_of_type.hpp"
#include "../type/of.hpp"

namespace range {

	template<typename Type>
	struct element_type_t : type::of<Type> {};

	template<basic_range RangeType>
	struct element_type_t<RangeType> : type::of<
		decltype(*range::begin(expression_of_type<RangeType>))
	> {};

	template<typename RangeType>
	requires basic_range<RangeType>
	using element_type = typename element_type_t<RangeType>::type;

} // range