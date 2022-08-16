#pragma once

#include "./basic.hpp"
#include "./element_type.hpp"
#include "../optional.hpp"

namespace __range {

	template<basic_range Range, typename Handler>
	constexpr optional<nuint> try_find_index_of_first_satisfying(
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

} // __range