#pragma once

#include "integer.hpp"

template<typename Iterator, typename Sentinel>
class range_on_iterator {
	Iterator iterator;
	Sentinel sentinel;
public:

	constexpr range_on_iterator(Iterator begin, Sentinel sentinel) :
		iterator{ begin },
		sentinel{ sentinel }
	{}

	constexpr Iterator begin() const { return iterator; }
	constexpr Sentinel end() const { return sentinel; }

	constexpr decltype(auto) operator [] (nuint index) const {
		return iterator[index];
	}

	constexpr decltype(auto) operator [] (nuint index) {
		return iterator[index];
	}

	// TODO if contiguous
	constexpr nuint size() const { return sentinel - iterator; }
};

#include "default_sentinel.hpp"

template<typename Iterator, typename Sentinel = default_sentinel>
constexpr range_on_iterator<Iterator, Sentinel> to_range(
	Iterator iterator, Sentinel sentinel = {}
) {
	return { iterator, sentinel };
}