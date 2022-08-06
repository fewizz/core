#pragma once

#include "./basic.hpp"
#include "./element_type.hpp"
#include "../ranges/equal.hpp"
#include "../array.hpp"
#include "../types/are_same.hpp"
#include "../types/first.hpp"

template<basic_range Range, basic_range OtherRange>
constexpr bool range_ends_with(Range&& range, OtherRange&& other) {
	auto size       = range.size();
	auto other_size = other.size();

	if(size < other_size) return false;

	return ranges::equal(
		to_range(
			range.begin() + (size - other_size), range.end()
		),
		other
	);
}

template<basic_range Range, typename... Types>
requires(
	(sizeof...(Types) == 1 || types_are_same<decay<Types>...>) &&
	types_are_same<
		decay<range_element_type<Range>>,
		decay<first_type<Types...>>
	>
)
constexpr bool range_ends_with(Range&& range, Types&&... values) {
	return with(forward<Range>(range), array{ values... });
}