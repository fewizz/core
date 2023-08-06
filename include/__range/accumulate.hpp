#pragma once

#include "./element_type.hpp"
#include "../__type/decay.hpp"

namespace __range {

template<typename Function, basic_range Range>
constexpr auto accumulate(
	Range&& range, auto initial_value, Function&& function
) {
	for(auto v : range) {
		initial_value = function(initial_value, v);
	}
	return initial_value;
}

template<basic_range Range>
constexpr auto accumulate(Range&& range, auto initial_value) {
	return accumulate(
		forward<Range>(range),
		initial_value,
		[](auto&& x, auto&& y) { return x + y; }
	);
}

template<basic_range Range>
constexpr auto accumulate(Range&& range) {
	return accumulate(
		forward<Range>(range),
		decay<range_element_type<Range>>{}
	);
}

} // __range