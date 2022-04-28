#pragma once

#include "range.hpp"
#include "begin.hpp"
#include "declval.hpp"
#include "meta/type/decay.hpp"

template<typename RangeType>
requires range<RangeType>
using element_type = decltype(*begin(declval<RangeType>()));