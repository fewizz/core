#pragma once

#include "range.hpp"

enum class ending {
	first, second, both, interrupted
};

template<range FirstRange, range SecondRange>
struct correlate {
	FirstRange& first_range;
	SecondRange& second_range;

	constexpr correlate(FirstRange& first_range, SecondRange& second_range) :
		first_range{ first_range },
		second_range{ second_range }
	{}

	template<typename Handler>
	[[ nodiscard ]]
	constexpr ending operator () (Handler&& handler) const {
		auto a = begin(first_range);
		auto b = begin(second_range);

		while(true) {
			bool first  = a == end(first_range);
			bool second = b == end(second_range);

			if( first &&  second) return ending::both;
			if(!first &&  second) return ending::second;
			if( first && !second) return ending::first;

			if(!handler(*a++, *b++)) return ending::interrupted;
		}
	}

};