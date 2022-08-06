#pragma once

#include "./forward.hpp"
#include "./range/basic.hpp"
#include "./range/contains.hpp"
#include "./range/copy.hpp"
#include "./range/iterator.hpp"
#include "./range/element_type.hpp"

template<basic_range Range>
struct range {
private:
	Range range_;
public:

	using element_type = range_element_type<Range>;

	constexpr range(Range&& range) : range_{ forward<Range>(range) } {}

	constexpr auto iterator() const { return range_.iterator(); }
	constexpr auto iterator()       { return range_.iterator(); }

	constexpr auto sentinel() const { return range_.sentinel(); }
	constexpr auto sentinel()       { return range_.sentinel(); }

	template<typename Element>
	constexpr bool contains(Element&& element) const {
		return range_contains(range_, forward<Element>(element));
	}

	template<basic_range OtherRange>
	constexpr void copy_to(OtherRange&& other_range) const {
		range_copy{ range_ }.to( forward<OtherRange>(other_range) );
	}

};

template<basic_range Range>
range(Range&&) -> range<Range>;