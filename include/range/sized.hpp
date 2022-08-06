#pragma once

#include "size.hpp"

template<typename Range>
concept sized_range =
	basic_range<Range> &&
	requires(Range& r){ size(r); };