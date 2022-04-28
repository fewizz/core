#pragma once

#include "view_on_stack.hpp"
#include "range.hpp"
#include "value_type.hpp"


template<range Range>
struct view_copy_on_stack {
	Range& range;
	using value_type = value_type<Range>;

	view_copy_on_stack(Range& range) :
		range{ range }
	{}

	template<typename F>
	decltype(auto) operator () (F&& f) {
		nuint size = range.size();

		return view_on_stack<value_type>(size)(
			[&](auto s) {
				nuint index = 0;
				for(auto& v : range) {
					s[index++] = v;
				}
				return f(s);
			}
		);
	}

};