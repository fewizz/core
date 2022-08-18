#pragma once

#include "./size.hpp"
#include "./element_type.hpp"
#include "./size_type.hpp"
#include "./reverse_view.hpp"
#include "../optional.hpp"

namespace __range {

	template<sized_range Range, typename Handler>
	constexpr optional<range_size_type<Range>>
	try_find_index_of_last_satisfying(
		Range&& range, Handler&& handler
	) {
		range_size_type<Range> remaining = range_size(range);

		auto beginning = range_iterator(range);
		auto it = beginning + remaining;

		while(it != beginning) {
			--it;

			if(handler(*it)) {
				return { (range_size_type<Range>) (it - beginning) };
			}
		}
		return {};
	}

} // __range