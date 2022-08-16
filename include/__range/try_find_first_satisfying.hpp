#pragma once

#include "./basic.hpp"
#include "./element_type.hpp"
#include "../optional.hpp"

namespace __range {

	template<basic_range Range, typename Handler>
	constexpr optional<range_element_type<Range>> try_find_first_satisfying(
		Range&& range, Handler&& handler
	) {
		for(decltype(auto) e : forward<Range>(range)) {
			if(handler(e)) {
				return { e };
			}
		}
		return {};
	}

} // __range