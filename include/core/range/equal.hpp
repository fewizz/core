#pragma once

#include "correlate.hpp"

namespace range {

	template<range::basic FirstRange, range::basic SecondRange>
	bool equal(FirstRange&& first, SecondRange&& second) {
		bool result = true;

		range::correlate(first, second)([&](auto& a, auto& b) {
			result &= a == b;
			return result;
		});

		return result;
	}

} // range