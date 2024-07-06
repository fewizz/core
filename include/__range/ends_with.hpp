#pragma once

#include "./basic.hpp"
#include "./size.hpp"
#include "../__array/without_extensions.hpp"
#include "../__ranges/have_equal_size_and_elements.hpp"
#include "../__iterator_and_sentinel/as_range.hpp"


namespace __range {

	template<basic_range Range, basic_range OtherRange>
	constexpr bool ends_with(Range&& range, OtherRange&& other) {
		auto size       = range_size(range);
		auto other_size = range_size(other);
		if (size < other_size) return false;
		return __ranges::have_equal_size_and_elements(
			__iterator_and_sentinel::as_range(
				range.begin() + (size - other_size), range.end()
			),
			other
		);
	}

	template<basic_range Range, typename... Elements>
	requires requires(Elements&&... ts) { array_without_extensions{ ts... }; }
	constexpr bool ends_with(Range&& range, Elements&&... elements) {
		return ends_with(
			forward<Range>(range),
			array_without_extensions{ elements... }
		);
	}

} // __range