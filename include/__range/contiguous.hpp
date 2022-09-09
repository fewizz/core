#pragma once

#include "./basic.hpp"
#include "./element_type.hpp"

template<typename Range>
concept continguous_range =
	basic_range<Range> &&
	requires(Range range) {
		range.elements_ptr()
			-> same_as<remove_reference<range_element_type<Range>>>;
	};