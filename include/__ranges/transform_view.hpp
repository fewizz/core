#pragma once

#include "../__range/sized.hpp"
#include "../__range/basic.hpp"
#include "../forward.hpp"
#include "../type/decay.hpp"

namespace __ranges {

template<typename Function, typename... Iterators>
class transform_view_iterator;

template<typename Function, basic_range... Ranges>
class transform_view;

// specialisation for single range
template<typename Function, typename Iterator>
class transform_view_iterator<Function, Iterator> {
	Iterator iterator_;
	Function& function_;
public:

	transform_view_iterator(Iterator iterator, Function& function) :
		iterator_{ iterator }, function_{ function }
	{}

	Iterator iterator_copy() { return iterator_; }

	decltype(auto) operator * ()       { return function_(*iterator_); }
	decltype(auto) operator * () const { return function_(*iterator_); }

	transform_view_iterator& operator ++ () { ++iterator_; return *this; }

};

template<typename Function, typename Iterator, typename Sentinel>
constexpr bool operator == (
	transform_view_iterator<Iterator, Function> tvi,
	Sentinel sentinel
) {
	return tvi.iterator_copy() == sentinel;
}

template<typename Function, typename Iterator, typename Sentinel>
constexpr bool operator == (
	Sentinel sentinel,
	transform_view_iterator<Iterator, Function> tvi
) {
	return tvi.iterator_copy() == sentinel;
}

template<typename Function, basic_range Range>
class transform_view<Function, Range> {
	Range range_;
	Function function_;
public:

	constexpr transform_view(Range&& range, Function&& function) :
		range_{ forward<Range>(range) },
		function_{ forward<Function>(function) }
	{}

	constexpr auto iterator() const {
		transform_view_iterator{ iterator(range_), function_ };
	}
	constexpr auto iterator()       {
		transform_view_iterator{ iterator(range_), function_ };
	}

	constexpr auto sentinel() const { return sentinel(range_); }
	constexpr auto sentinel()       { return sentinel(range_); }

	constexpr auto size() const requires sized_range<Range> {
		return size(range_);
	}

	constexpr decltype(auto) operator [] (nuint index) const {
		return function_(range_[index]);
	}
	constexpr decltype(auto) operator [] (nuint index)       {
		return function_(range_[index]);
	}

};

template<typename Function, basic_range Range>
transform_view(Range&&, Function&&) -> transform_view<Function, Range>;

} // __ranges