#pragma once

#include "range.hpp"
#include "to_range.hpp"
#include "equals.hpp"

template<range Range>
struct ends {
	Range&& range_;

	constexpr ends(Range&& range) : range_{ range } {}

	template<range OtherRange>
	constexpr bool with(OtherRange&& other) const {
		auto size = range_.size();
		auto other_size = other.size();

		if(size < other_size) return false;

		return equals(
			to_range(
				range_.begin() + (range_.size() - other.size()), range_.end()
			),
			other
		);
	}

};

template<range Range>
ends(Range&&) -> ends<Range>;