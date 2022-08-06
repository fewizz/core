#pragma once

#include "integer.hpp"

template<typename Iterator, typename Sentinel>
constexpr nuint iterators_distance(Iterator iterator, Sentinel sentinel) {
	nuint distance = 0;
	while(iterator != sentinel) {
		++distance;
		++iterator;
	}
	return distance;
}

template<typename Iterator, typename Sentinel>
requires requires(Iterator i, Sentinel s) { s - i; }
constexpr auto iterators_distance(Iterator iterator, Sentinel sentinel) {
	return sentinel - iterator;
}