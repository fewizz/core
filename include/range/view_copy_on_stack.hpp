#pragma once

#include "./basic.hpp"
#include "./value_type.hpp"
#include "../view_on_stack.hpp"

namespace range {

	template<basic_range Range>
	struct view_copy_on_stack {
		Range range;
		using value_type = value_type<Range>;

		view_copy_on_stack(Range&& range) :
			range{ forward<Range>(range) }
		{}

		template<typename F>
		decltype(auto) operator () (F&& f) {
			nuint size = range.size();

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

	template<basic_range Range>
	view_copy_on_stack(Range&& range) -> view_copy_on_stack<Range>;

}