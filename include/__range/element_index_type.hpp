#pragma once

#include "./sized.hpp"
#include "./size.hpp"
#include "../expression_of_type.hpp"
#include "../__type/decay.hpp"

template<sized_range Range>
using range_element_index_type
	= decay<decltype(range_size(expression_of_type<Range>()))>;