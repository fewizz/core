#pragma once

#include "range.hpp"
#include "to_range.hpp"
#include "equals.hpp"

template<range Range>
struct starts {
	Range range_;

	constexpr starts(Range&& range) : range_{ forward<Range>(range) } {}

	template<range OtherRange>
	constexpr bool with(OtherRange&& other) const {
		auto size = range_.size();
		auto other_size = other.size();

		if(size < other_size) return false;

		return equals(
			to_range(
				range_.begin(), range_.begin() + other_size
			),
			other
		);
	}

};

template<range Range>
starts(Range&&) -> starts<Range>;