#pragma once

#include "../__type/is_array.hpp"
#include "../__type/remove_const.hpp"
#include "../__type/is_same_as.hpp"
#include "../__type/remove_reference.hpp"
#include "../__iterator/basic.hpp"

template<typename RangeType>
concept basic_range =
	requires(RangeType r) {
		{ r.iterator() } -> basic_iterator;
		r.sentinel();
		r.iterator() == r.sentinel();
		r.iterator() != r.sentinel();
	} ||
	type_is_array<remove_reference<RangeType>>;