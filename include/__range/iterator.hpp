#pragma once

#include "integer.hpp"
#include "./basic.hpp"
#include "type/remove_const.hpp"
#include "type/remove_reference.hpp"

namespace __range {

	struct iterator_t {

		template<basic_range Range>
		requires is_array<Range>
		constexpr auto operator () (Range&& o) const {
			return o;
		}

		template<basic_range Type>
		constexpr auto operator () (Type&& o) const {
			return o.iterator();
		}

	};

}

inline constexpr __range::iterator_t range_iterator {};

template<basic_range Range>
constexpr auto begin(Range&& range) {
	return range_iterator(range);
}