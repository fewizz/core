#pragma once

#include "./element_type.hpp"
#include "../__type/decay.hpp"

namespace __range {

template<typename Function>
constexpr auto accumulate(
	const basic_range auto& range, auto init, Function&& function
) {
	for(auto v : range) init = function(init, v);
	return init;
}

constexpr auto accumulate(const basic_range auto& range, auto init) {
	return accumulate(range, init, [](auto x, auto y){ return x + y; });
}

template<basic_range Range>
constexpr auto accumulate(const Range& range) {
	return accumulate(range, decay<range_element_type<Range>>{});
}

} // __range