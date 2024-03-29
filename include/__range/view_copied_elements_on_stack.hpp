#pragma once

#include "./sized.hpp"
#include "./size.hpp"
#include "./element_type.hpp"
#include "./iterator.hpp"
#include "./sentinel.hpp"
#include "./copy.hpp"
#include "../view_on_stack.hpp"

namespace __range {

template<sized_range Range, typename Handler>
decltype(auto) view_copied_elements_on_stack(Range&& range, Handler&& handler) {
	nuint size = (nuint) range_size(range);

	using type = decay<range_element_type<Range>>;

	return view_on_stack<type>{ size }(
		[&](span<type> uninitialised_elements_on_stack) -> decltype(auto) {
			__range::copy{ range }.to(uninitialised_elements_on_stack);
			return handler(uninitialised_elements_on_stack);
		}
	);
}

} // __range