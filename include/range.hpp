#pragma once

#include "./forward.hpp"
#include "./__range/accumulate.hpp"
#include "./__range/basic.hpp"
#include "./__range/contains.hpp"
#include "./__range/copy.hpp"
#include "./__range/iterator.hpp"
#include "./__range/view_on_stack_copied_elements.hpp"

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

	constexpr auto accumulate() const {
		return __range::accumulate(range_);
	}

	template<typename Function>
	constexpr auto accumulate(Function&& function) const {
		return __range::accumulate(range_, forward<Function>(function));
	}

	template<typename Element>
	constexpr bool contains(Element&& element) const {
		return __range::contains(range_, forward<Element>(element));
	}

	template<basic_range OtherRange>
	constexpr void copy_to(OtherRange&& other_range) const {
		__range::copy{ range_ }.to( forward<OtherRange>(other_range) );
	}

	template<typename Handler>
	constexpr decltype(auto)
	view_copied_elements_on_stack(Handler&& handler) const {
		return __range::view_on_stack_copied_elements(
			range_, forward<Handler>(handler)
		);
	}

};

template<basic_range Range>
range(Range&&) -> range<Range>;