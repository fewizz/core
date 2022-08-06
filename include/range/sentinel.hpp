#pragma once

#include "integer.hpp"

struct range_sentinel_t {

	template<typename Type, nuint Size>
	constexpr auto operator () (Type (&o)[Size]) const {
		return o + Size;
	}

	template<typename Type>
	requires requires (Type& o) { o.sentinel(); }
	constexpr auto operator () (Type& o) const {
		return o.sentinel();
	}

};

inline constexpr range_sentinel_t range_sentinel {};

template<typename Type>
constexpr auto end(Type& possible_range) {
	return range_sentinel(possible_range);
}