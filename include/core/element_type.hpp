#pragma once

#include "range.hpp"
#include "begin.hpp"
#include "meta/expression_of_type.hpp"
#include "meta/type/decay.hpp"

template<typename RangeType>
requires range<RangeType>
using element_type = decltype(*begin(expression_of_type<RangeType>));