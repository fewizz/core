#pragma once

#include "correlate.hpp"

template<range FirstRange, range SecondRange>
constexpr bool equals(FirstRange&& first, SecondRange&& second) {
	bool result = true;

	auto end = correlate(first, second)([&](auto& a, auto& b) {
		result &= a == b;
		return result;
	});

	return result && end == ending::both;
}