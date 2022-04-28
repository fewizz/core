#pragma once

#include "value_type.hpp"

template<typename Handler>
auto accumulate(
	const range auto& range, auto init, Handler&& handler
) {
	for(auto v : range) init = handler(init, v);
	return init;
}

auto accumulate(const range auto& range, auto init) {
	return accumulate(range, init, [](auto x, auto y){ return x + y; });
}

template<range Range>
auto accumulate(const Range& range) {
	return accumulate(
		range, value_type<Range>{}
	);
}