#pragma once

#include "./sized.hpp"
#include "./element_type.hpp"
#include "./iterator.hpp"
#include "./sentinel.hpp"
#include "../view_on_stack.hpp"

namespace __range {

template<sized_range Range, typename Handler>
decltype(auto) view_copied_elements_on_stack(Range&& range, Handler&& handler) {
	nuint size = range.size();

	return view_on_stack<decay<range_element_type<Range>>>(size)(
		[&](auto s) -> decltype(auto) {
			nuint index = 0;
			for(decltype(auto) v : range) {
				s[index++] = v;
			}
			return handler(s);
		}
	);
}

} // __range