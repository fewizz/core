#pragma once

#include "./element_type.hpp"
#include "../ranges/transform_view.hpp"

namespace range {

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
		return accumulate(range, value_type<Range>{});
	}

} // range