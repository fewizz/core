#pragma once

#include "view_on_stack.hpp"
#include "range.hpp"
#include "value_type.hpp"

#include <core/distance.hpp>

template<range Range>
struct view_copy_on_stack {
	Range& range;
	using value_type = value_type<Range>;

	view_copy_on_stack(Range& range) :
		range{ range }
	{}

	template<typename F>
	decltype(auto) operator () (F&& f) {
		nuint size = distance(range);

		return view_on_stack<value_type>(size)(
			[&](auto s) -> decltype(auto) {
				nuint index = 0;
				for(decltype(auto) v : range) {
					s[index++] = v;
				}
				return f(s);
			}
		);
	}

};