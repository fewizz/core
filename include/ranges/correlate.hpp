#pragma once

#include "../range/basic.hpp"

namespace ranges {

	enum class ending {
		first, second, both, interrupted
	};

	template<basic_range FirstRange, basic_range SecondRange>
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
			auto a = range::begin(first_range);
			auto b = range::begin(second_range);

			while(true) {
				bool first  = a == range::end(first_range);
				bool second = b == range::end(second_range);

				if( first &&  second) return ending::both;
				if(!first &&  second) return ending::second;
				if( first && !second) return ending::first;

				if(!handler(*a++, *b++)) return ending::interrupted;
			}
		}

	};

} // ranges