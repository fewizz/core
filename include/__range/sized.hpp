#pragma once

#include "./basic.hpp"
#include "../__type/is_array.hpp"

template<typename Range>
concept sized_range =
	basic_range<Range> &&
	(requires(Range& o) { o.size(); } || type_is_array<Range>);