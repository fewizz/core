#pragma once

#include "./basic.hpp"
#include "./element_type.hpp"
#include "../__ranges/are_equal.hpp"
#include "../__iterator_and_sentinel/to_range.hpp"
#include "../array.hpp"
#include "../types/are_same.hpp"
#include "../types/first.hpp"

template<basic_range Range, basic_range OtherRange>
constexpr bool range_ends_with(Range&& range, OtherRange&& other) {
	auto size       = range.size();
	auto other_size = other.size();
	if(size < other_size) return false;
	return __ranges::are_equal(
		__iterator_and_sentinel::to_range(
			range.begin() + (size - other_size), range.end()
		),
		other
	);
}

template<basic_range Range, typename... Elements>
requires requires(Elements&&... ts) { array{ ts... }; }
constexpr bool range_ends_with(Range&& range, Elements&&... elements) {
	return with(forward<Range>(range), array{ elements... });
}