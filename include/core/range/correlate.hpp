#pragma once

#include "basic.hpp"

namespace range {

	template<range::basic FirstRange, range::basic SecondRange>
	struct correlate {
		FirstRange& first_range;
		SecondRange& second_range;

		correlate(FirstRange& first_range, SecondRange& second_range) :
			first_range{ first_range },
			second_range{ second_range }
		{}

		template<typename Handler>
		void operator () (Handler&& handler) const {
			auto r1 = first_range.begin();
			auto r2 = second_range.begin();

			while(r1 != first_range.end() && r2 != second_range.end()) {
				if(!handler(*r1++, *r2++)) break;
			}
		}

	};

} // range