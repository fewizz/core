#pragma once

#include "integer.hpp"
#include "range.hpp"

template<typename Iterator, typename Sentinel>
constexpr nuint distance(Iterator iterator, Sentinel sentinel) {
	nuint d = 0;

	while(iterator != sentinel) {
		++d;
		++iterator;
	}

	return d;
}

template<range Range>
constexpr nuint distance(Range&& range) {
	return distance(begin(range), end(range));
}