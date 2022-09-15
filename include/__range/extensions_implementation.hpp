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

#include "../__ranges/have_equal_elements.hpp"

template<typename Derived>
template<basic_range OtherRange>
constexpr bool range_extensions<Derived>::have_elements_equal_to(
	OtherRange&& other_range
) const {
	return __ranges::have_equal_elements(
		range_(), forward<OtherRange>(other_range)
	);
}

#include "./flat_view.hpp"

template<typename Derived>
constexpr auto range_extensions<Derived>::flat_view() const &  {
	return __range::flat_view{ range_() };
}
template<typename Derived>
constexpr auto range_extensions<Derived>::flat_view()       &  {
	return __range::flat_view{ range_() };
}
template<typename Derived>
constexpr auto range_extensions<Derived>::flat_view() const && {
	return __range::flat_view{ move(range_()) };
}
template<typename Derived>
constexpr auto range_extensions<Derived>::flat_view()       && {
	return __range::flat_view{ move(range_()) };
}

#include "./view_copied_elements_on_stack.hpp"

template<typename Derived>
template<typename Handler>
decltype(auto)
range_extensions<Derived>::view_copied_elements_on_stack(
	Handler&& handler
) const {
	return __range::view_copied_elements_on_stack(
		range_(),
		forward<Handler>(handler)
	);
}
template<typename Derived>
template<typename Handler>
decltype(auto)
range_extensions<Derived>::view_copied_elements_on_stack(
	Handler&& handler
) {
	return __range::view_copied_elements_on_stack(
		range_(),
		forward<Handler>(handler)
	);
}

#include "./try_find_first_satisfying.hpp"

template<typename Derived>
template<typename Predicate>
auto range_extensions<Derived>::try_find_first_satisfying(
	Predicate&& predicate
) const {
	return __range::try_find_first_satisfying(
		*this,
		forward<Predicate>(predicate)
	);
}