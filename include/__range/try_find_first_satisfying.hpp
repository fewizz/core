#pragma once

#include "./basic.hpp"
#include "./element_type.hpp"
#include "../optional.hpp"

namespace __range {

	template<basic_range Range, typename Predicate>
	constexpr optional<range_element_type<Range>>
	try_find_first_satisfying(
		Range&& range, Predicate&& predicate
	) {
		for(decltype(auto) e : forward<Range>(range)) {
			if(predicate(e)) {
				return { e };
			}
		}
		return {};
	}

} // __range