#pragma once

#include "./forward.hpp"
#include "./__range/iterator.hpp"
#include "./__range/sentinel.hpp"
#include "./__range/accumulate.hpp"
#include "./__range/basic.hpp"
#include "./__range/contains.hpp"
#include "./__range/copy.hpp"
#include "./__range/iterator.hpp"
#include "./__range/reverse_view.hpp"
#include "./__range/split_view.hpp"
#include "./__range/starts_with.hpp"
#include "./__range/try_find_first_satisfying.hpp"
#include "./__range/try_find_index_of_first_satisfying.hpp"
#include "./__range/view_copied_elements_on_stack.hpp"
#include "./__ranges/transform_view.hpp"
#include "./__ranges/are_equal.hpp"

template<basic_range Range>
struct range {
private:
	Range range_;
public:

	using element_type = range_element_type<Range>;

	constexpr range(Range&& range) : range_{ forward<Range>(range) } {}

	constexpr basic_iterator auto iterator() const {
		return range_iterator(range_);
	}
	constexpr basic_iterator auto iterator()       {
		return range_iterator(range_);
	}

	constexpr auto sentinel() const { return range_sentinel(range_); }
	constexpr auto sentinel()       { return range_sentinel(range_); }

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

	template<basic_range OtherRange>
	constexpr bool equals_to(OtherRange&& other_range) const {
		return __ranges::are_equal(range_, forward<OtherRange>(other_range));
	}

	auto reverse_view() const { return __range::reverse_view{ range_ }; }
	auto reverse_view()       { return __range::reverse_view{ range_ }; }

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

	template<typename... Types>
	constexpr bool starts_with(Types&&... range_or_elements) const {
		return __range::starts_with(
			range_, forward<Types>(range_or_elements)...
		);
	}

	template<typename Handler>
	constexpr auto try_find_first_satisfying(Handler&& handler) const {
		return __range::try_find_first_satisfying(range_, handler);
	}
	template<typename Handler>
	constexpr auto try_find_first_satisfying(Handler&& handler)       {
		return __range::try_find_first_satisfying(range_, handler);
	}

	template<typename Handler>
	constexpr auto try_find_last_satisfying(Handler&& handler) const {
		return __range::try_find_first_satisfying(reverse_view(), handler);
	}
	template<typename Handler>
	constexpr auto try_find_last_satisfying(Handler&& handler)       {
		return __range::try_find_first_satisfying(reverse_view(), handler);
	}

	template<typename Handler>
	constexpr auto try_find_index_of_first_satisfying(Handler&& handler) const {
		return __range::try_find_index_of_first_satisfying(range_, handler);
	}
	template<typename Handler>
	constexpr auto try_find_index_of_first_satisfying(Handler&& handler)       {
		return __range::try_find_index_of_first_satisfying(range_, handler);
	}

	template<typename Handler>
	constexpr auto try_find_index_of_last_satisfying(Handler&& handler) const {
		return __range::try_find_index_of_first_satisfying(
			__range::reverse_view{ range_ }, handler
		);
	}
	template<typename Handler>
	constexpr auto try_find_index_of_last_satisfying(Handler&& handler)       {
		return __range::try_find_index_of_first_satisfying(
			__range::reverse_view{ range_ }, handler
		);
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
		return __ranges::transform_view{ range_, forward<Function>(function) };
	}
	template<typename Function>
	constexpr auto transform_view(Function&& function)       {
		return __ranges::transform_view{ range_, forward<Function>(function) };
	}

};

template<basic_range Range>
range(Range&&) -> range<Range>;