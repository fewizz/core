#pragma once

#include "basic.hpp"
#include "sub.hpp"
#include "contains.hpp"
#include "../array.hpp"

namespace range {

	template<range::basic Range>
	struct trim {
		Range& range;

		trim(Range& range) : range{ range } {}

		template<range::basic Characters>
		auto with(Characters&& characters) const {
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

		template<typename... Args>
		requires types_are_same<range::value_type<Range>, Args...>
		auto with(Args&&... chars) const {
			return with(array{ chars... });
		}

	}; // trim

} // range