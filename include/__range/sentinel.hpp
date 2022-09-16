#pragma once

#include "./basic.hpp"
#include "../integer.hpp"

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

#include "../expression_of_type.hpp"

template<basic_range Range>
using range_sentinel_type = decltype(range_sentinel(expression_of_type<Range>));

template<basic_range Range>
constexpr auto end(Range&& range) {
	return range_sentinel(range);
}

template<typename Sentinel, typename Iterator>
concept basic_sentinel_of =
	basic_iterator<Iterator> &&
	requires(Sentinel s, Iterator i) {
		s == i; s != i;
	};