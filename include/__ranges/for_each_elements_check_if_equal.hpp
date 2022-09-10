#pragma once

#include "../__range/sized.hpp"
#include "../tuple.hpp"

namespace __ranges {

template<basic_range... Ranges>
constexpr bool for_each_elements_check_if_equal(Ranges&&... ranges) {
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
constexpr bool for_each_elements_check_if_equal(
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

}