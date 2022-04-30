#pragma once

#include "transform.hpp"
#include "value_type.hpp"

template<typename Function>
constexpr auto accumulate(
	const range auto& range, auto init, Function&& function
) {
	for(auto v : range) init = function(init, v);
	return init;
}

constexpr auto accumulate(const range auto& range, auto init) {
	return accumulate(range, init, [](auto x, auto y){ return x + y; });
}

template<range Range>
constexpr auto accumulate(const Range& range) {
	return accumulate(range, value_type<Range>{});
}