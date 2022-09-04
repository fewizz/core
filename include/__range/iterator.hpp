#pragma once

#include "./basic.hpp"
#include "../integer.hpp"
#include "../__type/remove_const.hpp"
#include "../__type/remove_reference.hpp"

namespace __range {

	struct iterator_t {

		template<basic_range Range>
		requires __type::is_array<Range>
		constexpr auto operator () (Range&& o) const {
			return o;
		}

		template<basic_range Range>
		constexpr auto operator () (Range&& o) const {
			return o.iterator();
		}

	};

}

inline constexpr __range::iterator_t range_iterator {};

#include "../expression_of_type.hpp"

template<basic_range Range>
using range_iterator_type = decltype(range_iterator(expression_of_type<Range>));

template<basic_range Range>
constexpr auto begin(Range&& range) {
	return range_iterator(range);
}