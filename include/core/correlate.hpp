#pragma once

#include "range.hpp"

template<range FirstRange, range SecondRange>
struct correlate {
	FirstRange& first_range;
	SecondRange& second_range;

	constexpr correlate(FirstRange& first_range, SecondRange& second_range) :
		first_range{ first_range },
		second_range{ second_range }
	{}

	template<typename Handler>
	constexpr void operator () (Handler&& handler) const {
		auto r1 = begin(first_range);
		auto r2 = end(second_range);

		while(r1 != end(first_range) && r2 != end(second_range)) {
			if(!handler(*r1++, *r2++)) break;
		}
	}

};