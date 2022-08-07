#pragma once

#include "../__range/basic.hpp"
#include "../__range/sized.hpp"
#include "./elements/of.hpp"
#include "./array.hpp"

template<basic_range... Ranges>
constexpr bool ranges_are_equal_unknown_size(Ranges&&... ranges) {
	auto iterators = elements::of {
		elements::of {
			ranges.iterator(),
			ranges.sentinel()
		}...
	};

	while(true) {
		array<bool, sizeof...(Ranges)> ends = iterators.pass([](auto... its) {
			return array{ (its.template at<0>() == its.template at<1>()) ...  };
		});

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

		iterators.pass([](auto&... its) { (++its.template at<0>(), ...);});
	}
}

template<basic_range Range0, basic_range Range1>
constexpr bool ranges_are_equal_unknown_size(
	Range0&& range0, Range1&& range1
) {
	auto i0 = range0.iterator();
	auto s0 = range0.sentinel();
	auto i1 = range1.iterator();
	auto s1 = range1.sentinel();

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
constexpr bool ranges_are_equal_known_size(Ranges&&... ranges) {
	auto iterators = elements::of { ranges.iterator()... };
	auto sizes = elements::of{ ranges.size()... };

	if(!sizes.pass([](auto first, auto... others) {
		return ((first == others) && ...);
	})) { return false; }

	nuint size = sizes.template at<0>();

	while(size > 0) {
		--size;

		bool elements_equal_to_first = iterators.pass(
			[](auto first, auto... its) {
				decltype(auto) first_element = *first;
				return ((first_element == *its) && ...);
			}
		);
		if(!elements_equal_to_first) { return false; }

		iterators.pass([](auto&... its) { (++its, ...);});
	}

	return true;
}

template<sized_range Range0, sized_range Range1>
constexpr bool ranges_are_equal_known_size(
	Range0&& range0, Range1&& range1
) {
	auto s0 = size(range0);
	auto s1 = size(range1);

	if(s0 != s1) { return false; }

	auto i0 = range0.iterator();
	auto i1 = range1.iterator();

	while(s0 > 0) {
		--s0;
		if(*i0 != *i1) {
			return false;
		}
		++i0;
		++i1;
	}

	return true;
}

template<basic_range... Ranges>
constexpr bool ranges_are_equal(Ranges&&... ranges) {
	if constexpr((sized_range<Ranges> && ...)) {
		return ranges_are_equal_known_size(forward<Ranges>(ranges)...);
	}
	else {
		return ranges_are_equal_unknown_size(forward<Ranges>(ranges)...);
	}
}