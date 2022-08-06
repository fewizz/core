#pragma once

#include "basic.hpp"
#include "integer.hpp"

struct size_t {

	template<typename Type, nuint Size>
	constexpr auto operator () (Type (&)[Size] ) const {
		return Size;
	}

	template<basic_range Range>
	requires requires(Range& r) { r.size(); }
	constexpr auto operator () (Range&& range) const {
		return range.size();
	}

};

inline constexpr size_t size {};