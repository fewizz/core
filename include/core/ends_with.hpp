#pragma once

#include "range.hpp"
#include "to_range.hpp"
#include "equals.hpp"
#include "array.hpp"
#include "value_type.hpp"
#include "meta/types/are_same.hpp"
#include "meta/types/first.hpp"

template<range Range>
struct ends {
	Range range_;

	constexpr ends(Range&& range) : range_{ forward<Range>(range) } {}

	template<range OtherRange>
	constexpr bool with(OtherRange&& other) const {
		auto size = range_.size();
		auto other_size = other.size();

		if(size < other_size) return false;

		return equals(
			to_range(
				range_.begin() + (size - other_size), range_.end()
			),
			other
		);
	}

	template<typename... Types>
	requires(
		(sizeof...(Types) == 1 || types_are_same<decay<Types>...>) &&
		types_are_same<decay<element_type<Range>>, decay<first_type<Types...>>>
	)
	constexpr bool with(Types&&... values) const {
		return with(array{ values... });
	}

};

template<range Range>
ends(Range&&) -> ends<Range>;