#pragma once

#include "./sized.hpp"
#include "./size.hpp"
#include "../expression_of_type.hpp"

template<sized_range Range>
using range_size_type = decltype(range_size(expression_of_type<Range>));