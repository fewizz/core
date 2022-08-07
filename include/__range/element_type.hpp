#pragma once

#include "./basic.hpp"
#include "./iterator.hpp"
#include "../expression_of_type.hpp"
#include "../type/of.hpp"

namespace __range {

	template<typename Type>
	struct element_type_t : type::of<Type> {};

	template<basic_range Range>
	struct element_type_t<Range> : type::of<
		decltype(*range_iterator(expression_of_type<Range>))
	> {};

}

template<typename Range>
requires basic_range<Range>
using range_element_type = typename __range::element_type_t<Range>::type;