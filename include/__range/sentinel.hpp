#pragma once

#include "../integer.hpp"
#include "./basic.hpp"

namespace __range {

	struct sentinel_t {

		template<typename Type, nuint Size>
		constexpr auto operator () (Type (&o)[Size]) const {
			return o + Size;
		}

		template<basic_range Range>
		constexpr auto operator () (Range&& o) const {
			return o.sentinel();
		}

	};

} // __range

inline constexpr __range::sentinel_t range_sentinel {};

template<basic_range Range>
constexpr auto end(Range&& range) {
	return range_sentinel(range);
}