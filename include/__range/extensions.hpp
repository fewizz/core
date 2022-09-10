#pragma once

#include "./extensions_declaration.hpp"
#include "./sized.hpp"

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

	auto size() const {
		static_assert(sized_range<Derived>);
		return sentinel() - iterator();
	}

};