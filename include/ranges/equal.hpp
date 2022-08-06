#pragma once

#include "./correlate.hpp"

namespace ranges {

	template<basic_range FirstRange, basic_range SecondRange>
	constexpr bool equal(FirstRange&& first, SecondRange&& second) {
		bool result = true;

		auto correlation_result {
			correlate(first, second)([&](auto&& a, auto&& b) {
				result &= a == b;
				if(!result) { return loop_action::stop; }
				return loop_action::next;
			})
		};

		return
			result &&
			correlation_result == ranges_correlation_end_cause::both_ended;
	}

} // ranges