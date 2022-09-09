#pragma once

#include "../__range/basic.hpp"
#include "../__range/size.hpp"
#include "../__range/iterator.hpp"
#include "../__range/sentinel.hpp"
#include "../__iterator/basic.hpp"
#include "../tuple.hpp"

namespace __ranges {

template<sized_range... Ranges>
constexpr bool have_equal_size_and_elements(Ranges&&... ranges) {
	auto iterators = tuple { ranges.iterator()... };
	auto sizes = tuple { ranges.size()... };

	if(!sizes.pass([](integer auto first, integer auto... others) {
		return ((first == others) && ...);
	})) { return false; }

	nuint size = sizes.template at<0>();

	while(size > 0) {
		--size;

		bool elements_equal_to_first = iterators.pass(
			[](basic_iterator auto first, basic_iterator auto... its) {
				decltype(auto) first_element = *first;
				return ((first_element == *its) && ...);
			}
		);
		if(!elements_equal_to_first) { return false; }

		iterators.pass([](basic_iterator auto&... its) { (++its, ...);});
	}

	return true;
}

template<sized_range Range0, sized_range Range1>
constexpr bool have_equal_size_and_elements(
	Range0&& range0, Range1&& range1
) {
	integer auto s0 = range_size(range0);
	integer auto s1 = range_size(range1);

	if(s0 != s1) { return false; }

	basic_iterator auto i0 = range0.iterator();
	basic_iterator auto i1 = range1.iterator();

	while(s0 > 0) {
		if(*i0 != *i1) {
			return false;
		}
		++i0;
		++i1;
		--s0;
	}

	return true;
}

} // __ranges