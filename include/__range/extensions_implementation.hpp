#pragma once

#include "./extensions.hpp"

#include "./contains.hpp"

template<typename Derived>
template<typename Element>
constexpr bool range_extensions<Derived>::contains(Element&& element) const {
	return __range::contains(range_(), forward<Element>(element));
}

#include "./copy.hpp"

template<typename Derived>
template<basic_range OtherRange>
constexpr void range_extensions<Derived>::copy_to(
	OtherRange&& other_range
) const {
	__range::copy{ range_() }.to( forward<OtherRange>(other_range) );
}

#include "../__ranges/have_equal_size_and_elements.hpp"

template<typename Derived>
template<basic_range OtherRange>
constexpr bool range_extensions<Derived>::have_size_and_elements_equals_to(
	OtherRange&& other_range
) const {
	return __ranges::have_equal_size_and_elements(
		range_(), forward<OtherRange>(other_range)
	);
}