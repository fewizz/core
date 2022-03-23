#pragma once

#include "value_type.hpp"

namespace range {

	auto accumulate(const range::basic auto& range, auto init, auto f) {
		for(auto v : range) init = f(init, v);
		return init;
	}

	auto accumulate(const range::basic auto& range, auto init) {
		return accumulate(range, init, [](auto x, auto y){ return x + y; });
	}

	template<range::basic Range>
	auto accumulate(const Range& range) {
		return accumulate(
			range,
			range::value_type<Range>{},
			[](auto x, auto y){ return x + y; }
		);
	}

} // range