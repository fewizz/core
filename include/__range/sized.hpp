#pragma once

#include "./basic.hpp"
#include "./has_size_member_function.hpp"
#include "../type/is_array.hpp"

template<typename Range>
concept sized_range =
	basic_range<Range> &&
	(__range::has_size_member_function<Range> || is_array<Range>);