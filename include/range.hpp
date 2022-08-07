#pragma once

#include "./forward.hpp"
#include "./__range/accumulate.hpp"
#include "./__range/basic.hpp"
#include "./__range/contains.hpp"
#include "./__range/copy.hpp"
#include "./__range/iterator.hpp"
#include "./__range/split_view.hpp"
#include "./__range/view_copied_elements_on_stack.hpp"
#include "./__ranges/transform_view.hpp"

template<basic_range Range>
struct range {
private:
	Range range_;
public:

	using element_type = range_element_type<Range>;

	constexpr range(Range&& range) : range_{ forward<Range>(range) } {}

	constexpr basic_range auto iterator() const { return range_.iterator(); }
	constexpr basic_range auto iterator()       { return range_.iterator(); }

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

	template<basic_range OtherRange>
	constexpr void copy_to(OtherRange&& other_range)       {
		__range::copy{ range_ }.to( forward<OtherRange>(other_range) );
	}

	template<basic_range SplittersRange>
	constexpr auto split_view(SplittersRange&& splitters_range) const {
		__range::split_view{ range_, forward<SplittersRange>(splitters_range) };
	}
	template<basic_range SplittersRange>
	constexpr auto split_view(SplittersRange&& splitters_range) {
		__range::split_view{ range_, forward<SplittersRange>(splitters_range) };
	}

	template<typename... Splitters>
	constexpr auto split_view(Splitters&&... splitters) const {
		__range::split_view{ range_, forward<Splitters>(splitters)... };
	}
	template<typename... Splitters>
	constexpr auto split_view(Splitters&&... splitters)       {
		__range::split_view{ range_, forward<Splitters>(splitters)... };
	}

	template<typename Handler>
	constexpr decltype(auto)
	view_copied_elements_on_stack(Handler&& handler) const {
		return __range::view_copied_elements_on_stack(
			range_, forward<Handler>(handler)
		);
	}
	template<typename Handler>
	constexpr decltype(auto)
	view_copied_elements_on_stack(Handler&& handler)       {
		return __range::view_copied_elements_on_stack(
			range_, forward<Handler>(handler)
		);
	}

	template<typename Function>
	constexpr auto transform_view(Function&& function) const {
		return __ranges::transform_view{ forward<Function>(function), range_ };
	}
	template<typename Function>
	constexpr auto transform_view(Function&& function)       {
		return __ranges::transform_view{ forward<Function>(function), range_ };
	}

};

template<basic_range Range>
range(Range&&) -> range<Range>;