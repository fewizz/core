#pragma once

#include "./size.hpp"
#include "./element_index_type.hpp"
#include "../optional.hpp"

namespace __range {

	template<sized_range Range, typename Handler>
	constexpr optional<range_element_index_type<Range>>
	try_find_index_of_last_satisfying(
		Range&& range, Handler&& handler
	) {
		range_element_index_type<Range> remaining = range_size(range);

		auto beginning = range_iterator(range);
		auto it = beginning + remaining;

		while (it != beginning) {
			--it;

			if (handler(*it)) {
				return { (range_element_index_type<Range>) (it - beginning) };
			}
		}
		return {};
	}

} // __range