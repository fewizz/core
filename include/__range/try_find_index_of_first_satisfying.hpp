#pragma once

#include "./basic.hpp"
#include "./element_type.hpp"
#include "./element_index_type.hpp"
#include "../optional.hpp"

namespace __range {

	template<basic_range Range, typename Handler>
	constexpr optional<nuint>
	try_find_index_of_first_satisfying(
		Range&& range, Handler&& handler
	) {
		nuint index = 0;
		for(decltype(auto) e : forward<Range>(range)) {
			if(handler(e)) {
				return { index };
			}
			++index;
		}
		return {};
	}

	template<sized_range Range, typename Handler>
	constexpr optional<range_element_index_type<Range>>
	try_find_index_of_first_satisfying(
		Range&& range, Handler&& handler
	) {
		range_element_index_type<Range> index = 0;
		for(decltype(auto) e : forward<Range>(range)) {
			if(handler(e)) {
				return { index };
			}
			++index;
		}
		return {};
	}

} // __range