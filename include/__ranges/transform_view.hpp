#pragma once

#include "../__range/borrowed.hpp"
#include "../__range/size.hpp"
#include "../__range/basic.hpp"
#include "../__range/iterator.hpp"
#include "../__range/sentinel.hpp"
#include "../__range/extensions.hpp"
#include "../forward.hpp"

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

	constexpr transform_view_iterator(Iterator iterator, Function& function) :
		iterator_{ iterator }, function_{ function }
	{}

	constexpr Iterator iterator_copy() { return iterator_; }

	constexpr decltype(auto) operator * () const {
		return function_(*iterator_);
	}
	constexpr decltype(auto) operator * ()       {
		return function_(*iterator_);
	}

	constexpr transform_view_iterator& operator ++ () {
		++iterator_; return *this;
	}

	constexpr transform_view_iterator& operator += (nuint n) {
		iterator_ += n;
		return *this;
	}

	constexpr transform_view_iterator operator + (nuint n) {
		return { iterator_ + n, function_ };
	}

	constexpr nuint operator - (transform_view_iterator o) const {
		__builtin_assume(iterator_ > o.iterator_);
		return iterator_ - o.iterator_;
	}

	constexpr bool operator == (
		transform_view_iterator other
	) const { return other.iterator_ == iterator_; }

	template<typename Sentinel>
	friend constexpr bool operator == (
		transform_view_iterator tvi, Sentinel sentinel
	) { return tvi.iterator_copy() == sentinel; }

	template<typename Sentinel>
	friend constexpr bool operator == (
		Sentinel sentinel, transform_view_iterator tvi
	) { return tvi.iterator_copy() == sentinel; }

};

template<typename Function, typename Iterator>
transform_view_iterator(Iterator, Function&)
	-> transform_view_iterator<Function, Iterator>;

template<typename Function, basic_range Range>
class transform_view<Function, Range> :
	public range_extensions<transform_view<Function, Range>>
{
	Range range_;
	Function function_;
public:

	static constexpr bool is_borrowed_range = borrowed_range<
		Range
	>;

	constexpr transform_view(Range&& range, Function&& function) :
		range_{ forward<Range>(range) },
		function_{ forward<Function>(function) }
	{}

	constexpr auto iterator() const {
		return transform_view_iterator{ range_iterator(range_), function_ };
	}
	constexpr auto iterator()       {
		return transform_view_iterator{ range_iterator(range_), function_ };
	}

	constexpr auto sentinel() const { return range_sentinel(range_); }
	constexpr auto sentinel()       { return range_sentinel(range_); }

	constexpr auto size() const requires sized_range<Range> {
		return range_size(range_);
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