#pragma once

#include "./basic.hpp"
#include "./iterator.hpp"
#include "./sentinel.hpp"
#include "../__type/is_array.hpp"
#include "../__type/is_same_as.hpp"

template<typename Range>
concept sized_range =
	basic_range<Range> &&
	requires(Range r) {
		{ range_sentinel(r) - range_iterator(r) } -> not_same_as<void>;
		{ range_iterator(r) - range_iterator(r) } -> not_same_as<void>;
	};