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

	if (!sizes.pass([](integer auto first, integer auto... others) {
		return ((first == others) && ...);
	})) { return false; }

	nuint size = sizes.template at<0>();

	while (size > 0) {
		--size;

		bool elements_equal_to_first = iterators.pass(
			[](basic_iterator auto first, basic_iterator auto... its) {
				decltype(auto) first_element = *first;
				return ((first_element == *its) && ...);
			}
		);
		if (!elements_equal_to_first) { return false; }

		iterators.pass([](basic_iterator auto&... its) { (++its, ...);});
	}

	return true;
}

template<sized_range Range0, sized_range Range1>
constexpr bool have_equal_size_and_elements(
	Range0&& range_0, Range1&& range_1
) {
	integer auto size_0 = range_size(range_0);
	integer auto size_1 = range_size(range_1);

	if (size_0 != size_1) { return false; }

	basic_iterator auto iterator_0 = range_iterator(range_0);
	basic_iterator auto iterator_1 = range_iterator(range_1);

	while (size_0 > 0) {
		if (*iterator_0 != *iterator_1) {
			return false;
		}
		++iterator_0;
		++iterator_1;
		--size_0;
	}

	return true;
}

// non-sized ones
template<basic_range... Ranges>
constexpr bool have_equal_size_and_elements(Ranges&&... ranges) {
	auto iterators = tuple {
		tuple {
			range_iterator(ranges),
			range_sentinel(ranges)
		}...
	};

	return [&]<nuint... Indices>(
		indices::of<Indices...> = indices::from<0>::to<sizeof...(Ranges)>{}
	) {
		while (true) {
			bool ends[sizeof...(Ranges)] {
				iterators.template at<Indices>.template at<0>() ==
				iterators.template at<Indices>.template at<1>() ...
			};

			// all ended
			if (ends.pass([](auto... ends){ return (ends && ...); })) {
				return true;
			}

			// some of them ended, not all
			if (ends.pass([](auto... ends){ return (ends || ...); })) {
				return false;
			}

			bool elements_equal_to_first = iterators.pass(
				[](auto first, auto... its) {
					decltype(auto) first_element = *first.template at<0>();
					return ((first_element == *its.template at<0>()) && ...);
				}
			);
			if (!elements_equal_to_first) {
				return false;
			}

			(++iterators.template at<Indices>.template at<0>() , ...);
		}
	}();
}

template<basic_range Range0, basic_range Range1>
constexpr bool have_equal_size_and_elements(
	Range0&& range_0, Range1&& range_1
) {
	basic_iterator auto iterator_0 = range_iterator(range_0);
	               auto sentinel_0 = range_sentinel(range_0);
	basic_iterator auto iterator_1 = range_iterator(range_1);
	               auto sentinel_1 = range_sentinel(range_1);

	while (true) {
		bool end_0 = iterator_0 == sentinel_0;
		bool end_1 = iterator_1 == sentinel_1;
		if (end_0 && end_1) {
			return true;
		}
		if (end_0 || end_1) {
			return false;
		}
		if (*iterator_0 != *iterator_1) {
			return false;
		}
		++iterator_0;
		++iterator_1;
	}
}

} // __ranges