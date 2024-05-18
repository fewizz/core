#pragma once

#include "./element_type.hpp"
#include "./iterator.hpp"
#include "./sentinel.hpp"
#include "./copy.hpp"
#include "../view_on_stack.hpp"
#include "../__iterator_and_sentinel/get_or_compute_distance.hpp"

namespace __range {

template<basic_range Range, typename Handler>
decltype(auto) view_copied_elements_on_stack(Range&& range, Handler&& handler) {
	nuint size = __iterator_and_sentinel::get_or_compute_distance(
		range_iterator(range),
		range_sentinel(range)
	);

	using type = decay<range_element_type<Range>>;

	return view_on_stack<type>{ size }(
		[&](span<type> uninitialised_elements_on_stack) -> decltype(auto) {
			__range::copy{ range }.to(uninitialised_elements_on_stack);
			return handler(uninitialised_elements_on_stack);
		}
	);
}

} // __range