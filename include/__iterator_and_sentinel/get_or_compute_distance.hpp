#pragma once

#include "../integer.hpp"
#include "../__iterator/basic.hpp"
#include "../__iterator/sentinel.hpp"

namespace __iterator_and_sentinel {

template<
	basic_iterator Iterator,
	sentinel_for<Iterator> Sentinel
>
constexpr nuint get_or_compute_distance(Iterator iterator, Sentinel sentinel) {
	nuint distance = 0;
	while(iterator != sentinel) {
		++distance;
		++iterator;
	}
	return distance;
}

template<basic_iterator Iterator, sentinel_for<Iterator> Sentinel>
requires requires(Iterator i, Sentinel s) { s - i; }
constexpr nuint get_or_compute_distance(Iterator iterator, Sentinel sentinel) {
	return sentinel - iterator;
}

} // __iterator_and_sentinel