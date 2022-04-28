#pragma once

#include "range.hpp"
#include "contains.hpp"
#include "to_range.hpp"
#include "value_type.hpp"
#include "array.hpp"

template<range Range>
class trim {
	Range& range;
public:

	trim(Range& range) : range{ range } {}

	template<::range With>
	auto with(With&& with) const {
		auto begin = range.begin();

		while(
			begin != range.end() &&
			contains(*begin).in(with)
		) {
			++begin;
		}

		auto end = range.end();
		auto last = end - 1;

		while(
			last != begin &&
			contains(*last).in(with)
		) {
			end = last;
			--last;
		}

		return to_range(begin, end);
	}

	template<typename... Args>
	requires types_are_same<value_type<Range>, Args...>
	auto with(Args&&... chars) const {
		return with(array{ chars... });
	}

}; // trim