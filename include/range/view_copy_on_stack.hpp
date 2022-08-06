#pragma once

#include "./sized.hpp"
#include "element_type.hpp"
#include "../view_on_stack.hpp"

template<sized_range Range, typename F>
decltype(auto) view_on_stack_copied_range_elements(Range&& range, F&& f) {
	nuint size = range.size();

	return view_on_stack<range_element_type<Range>>(size)(
		[&](auto s) -> decltype(auto) {
			nuint index = 0;
			for(decltype(auto) v : range) {
				s[index++] = v;
			}
			return f(s);
		}
	);
}