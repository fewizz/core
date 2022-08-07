#pragma once

#include "./basic.hpp"
#include "../type/is_array.hpp"

template<typename Range>
concept sized_range =
	basic_range<Range> &&
	(requires(Range& o) { o.size(); } || is_array<Range>);