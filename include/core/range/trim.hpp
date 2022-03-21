#pragma once

#include "basic.hpp"
#include "sub.hpp"
#include "contains.hpp"

namespace range {

	template<range::basic Range>
	struct trim {
		Range& range;

		trim(Range& range) : range{ range } {}

		template<range::basic Characters>
		auto with (Characters&& characters) const {
			auto begin = range.begin();

			while(
				begin != range.end() &&
				range::contains(*begin).in(characters)
			) {
				++begin;
			}

			auto end = range.end();
			auto last = end - 1;

			while(
				last != begin &&
				range::contains(*last).in(characters)
			) {
				end = last;
				--last;
			}

			return range::sub{ begin, end };
		}
	};

} // range