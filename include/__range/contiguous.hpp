#pragma once

#include "./basic.hpp"
#include "./element_type.hpp"
#include "../__iterator/contiguous.hpp"

template<typename Range>
concept contiguous_range =
	basic_range<Range> &&
	requires(Range range) {
		{ range_iterator(range) } -> contiguous_iterator;
	};