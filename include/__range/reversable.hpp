#pragma once

#include "./sized.hpp"

template<typename Range>
concept reversable_range =
	sized_range<Range> &&
	requires(decltype(range_iterator(expression_of_type<Range>())) it) {
		--it;
	};