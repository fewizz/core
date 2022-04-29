#pragma once

#include "range.hpp"

template<range Range>
class copy {
	Range from;
public:

	template<range Range0>
	constexpr copy(Range0&& range) : from{ range } {}

	template<range To>
	constexpr decltype(auto) to(To&& to) {
		auto from_begin = begin(from);
		auto from_end = end(from);
		auto to_begin = begin(to);

		while(from_begin != from_end) {
			*to_begin = *from_begin;
			++from_begin;
			++to_begin;
		}

		return forward<To>(to);
	}

};

template<range Range>
copy(Range&&) -> copy<Range>;