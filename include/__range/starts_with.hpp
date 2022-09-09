#pragma once

#include "./basic.hpp"
#include "./size.hpp"
#include "../array.hpp"
#include "../__ranges/have_equal_size_and_elements.hpp"
#include "../__iterator_and_sentinel/to_range.hpp"

namespace __range {

	template<basic_range Range, basic_range OtherRange>
	constexpr bool starts_with(Range&& range, OtherRange&& other) {
		auto size       = range_size(range);
		auto other_size = range_size(other);
		if(size < other_size) return false;
		return __ranges::have_equal_size_and_elements(
			__iterator_and_sentinel::to_range(
				range.iterator(), range.iterator() + other_size
			),
			other
		);
	}

	template<basic_range Range, typename... Elements>
	requires requires(Elements&&... ts) { array{ ts... }; }
	constexpr bool starts_with(Range&& range, Elements&&... elements) {
		return starts_with(forward<Range>(range), array{ elements... });
	}

} // __range