#pragma once

#include "../__range/basic.hpp"
#include "../__range/size.hpp"
#include "../__range/iterator.hpp"
#include "../__range/sentinel.hpp"
#include "../__iterator/basic.hpp"
#include "../tuple.hpp"

namespace __ranges {

template<basic_range... Ranges>
constexpr bool have_equal_elements_and_size(Ranges&&... ranges) {
	auto iterators = tuple {
		tuple {
			ranges.iterator(),
			ranges.sentinel()
		}...
	};

	return [&]<nuint... Indices>(
		indices::of<Indices...> = indices::from<0>::to<sizeof...(Ranges)>{}
	) {
		while(true) {
			bool ends[sizeof...(Ranges)] {
				iterators.template at<Indices>.template at<0>() ==
				iterators.template at<Indices>.template at<1>() ...
			};

			// all ended
			if(ends.pass([](auto... ends){ return (ends && ...); })) {
				return true;
			}

			// some of them ended, not all
			if(ends.pass([](auto... ends){ return (ends || ...); })) {
				return false;
			}

			bool elements_equal_to_first = iterators.pass(
				[](auto first, auto... its) {
					decltype(auto) first_element = *first.template at<0>();
					return ((first_element == *its.template at<0>()) && ...);
				}
			);
			if(!elements_equal_to_first) {
				return false;
			}

			(++iterators.template at<Indices>.template at<0>() , ...);
		}
	}();
}

template<basic_range Range0, basic_range Range1>
constexpr bool have_equal_elements_and_size(
	Range0&& range0, Range1&& range1
) {
	basic_iterator auto i0 = range_iterator(range0);
	               auto s0 = range_sentinel(range0);
	basic_iterator auto i1 = range_iterator(range1);
	               auto s1 = range_sentinel(range1);

	while(true) {
		bool e0 = i0 == s0;
		bool e1 = i1 == s1;
		if(e0 && e1) {
			return true;
		}
		if(e0 || e1) {
			return false;
		}
		if(*i0 != *i1) {
			return false;
		}
		++i0;
		++i1;
	}
}

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
constexpr bool have_equal_elements_and_size(
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

template<basic_range... Ranges>
constexpr bool are_equal(Ranges&&... ranges) {
	if constexpr((sized_range<Ranges> && ...)) {
		return have_equal_size_and_elements(forward<Ranges>(ranges)...);
	}
	else {
		return have_equal_elements_and_size(forward<Ranges>(ranges)...);
	}
}

} // __ranges