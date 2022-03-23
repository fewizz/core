#pragma once

#include "basic.hpp"
#include "value_type.hpp"
#include "../span.hpp"

namespace range {

	template<range::basic Range>
	struct view_copy_on_stack {
		Range& range;
		using value_type = range::value_type<Range>;

		view_copy_on_stack(Range& range) :
			range{ range }
		{}

		template<typename F>
		decltype(auto) operator () (F&& f) {
			nuint size = range.size();
			value_type storage[size];

			nuint index = 0;
			for(auto& v : range) {
				storage[index++] = v;
			}

			return f(span{ storage, size });
		}

	};

} // range