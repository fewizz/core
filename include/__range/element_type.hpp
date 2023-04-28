#pragma once

#include "./basic.hpp"
#include "./iterator.hpp"
#include "../__iterator/element_type.hpp"
#include "../expression_of_type.hpp"

template<basic_range Range>
using range_element_type = iterator_element_type<
	decltype(range_iterator(expression_of_type<Range>()))
>;