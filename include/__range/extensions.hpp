#pragma once

#include "./extensions_declaration.hpp"

template<typename Derived>
struct range_extensions {

private:
	const Derived& range_() const { return (const Derived&) *this; }
	      Derived& range_()       { return (      Derived&) *this; }
public:

	constexpr basic_iterator auto iterator() const {
		return range_iterator(range_());
	}
	constexpr basic_iterator auto iterator()       {
		return range_iterator(range_());
	}

	constexpr auto sentinel() const { return range_sentinel(range_()); }
	constexpr auto sentinel()       { return range_sentinel(range_()); }

	template<typename Element>
	constexpr bool contains(Element&& element) const;

	template<basic_range OtherRange>
	constexpr void copy_to(OtherRange&& other_range) const;

	template<basic_range OtherRange>
	constexpr bool have_size_and_elements_equals_to(
		OtherRange&& other_range
	) const;

};

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