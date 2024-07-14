#pragma once

#include "./basic.hpp"
#include "./contains.hpp"
#include "./of_element_type_same_as.hpp"
#include "./default_sentinel.hpp"
#include "../__iterator_and_sentinel/as_range.hpp"
#include "../__iterator/sentinel.hpp"
#include "../__array/without_extensions.hpp"

namespace __range {

template<
	basic_iterator Iterator,
	sentinel_for<Iterator> Sentinel,
	basic_range SplittersRange
>
struct split_view_iterator {
private:
	Iterator iterator_;
	Iterator iterator_end_;
	Sentinel sentinel_;
	SplittersRange& splitters_range_;

	constexpr bool is_splitter(basic_iterator auto it) const {
		return __range::contains(splitters_range_, *it);
	}

	constexpr bool is_not_splitter_or_end(basic_iterator auto it) const {
		return iterator_end_ != sentinel_ && !is_splitter(it);
	}

	constexpr void skip() {
		// skip splitters, search for beginning
		while (is_splitter(iterator_end_)) {
			++iterator_end_;
		}
		iterator_ = iterator_end_;
		++iterator_end_;
		// search for ending
		while (is_not_splitter_or_end(iterator_end_)) {
			++iterator_end_;
		}
	}
public:

	constexpr split_view_iterator(
		Iterator iterator,
		Sentinel sentinel,
		SplittersRange& splitters_range
	) :
		iterator_       { iterator       },
		iterator_end_   { iterator       },
		sentinel_       { sentinel       },
		splitters_range_{ splitters_range }
	{
		skip();
	}

	constexpr auto operator * () const {
		return __iterator_and_sentinel::as_range(iterator_, iterator_end_);
	}

	constexpr auto& operator ++ () {
		++iterator_end_;
		skip();
		return *this;
	}

	constexpr auto& operator += (nuint n) {
		while (n > 0) {
			--n;
			++(*this);
		}
		return *this;
	}

	constexpr auto operator + (nuint n) const {
		return split_view_iterator{ *this } += n;
	}

	friend constexpr inline bool operator == (
		split_view_iterator<Iterator, Sentinel, SplittersRange> it,
		default_sentinel
	) {
		return it.iterator_ == it.sentinel_;
	}

	friend constexpr inline bool operator == (
		default_sentinel,
		split_view_iterator<Iterator, Sentinel, SplittersRange> it
	) {
		return it.iterator_ == it.sentinel_;
	}

};

template<
	basic_range Range,
	range_of_decayed<decay<range_element_type<Range>>> SplittersRange
>
struct split_view : range_extensions<split_view<Range, SplittersRange>>{
private:
	Range range_;
	SplittersRange splitters_range_;
public:

	constexpr split_view(Range&& range, SplittersRange&& splitters_range) :
		range_{ forward<Range>(range) },
		splitters_range_{ forward<SplittersRange>(splitters_range) }
	{}

	template<typename... Splitters>
	constexpr split_view(Range&& range, Splitters&&... splitters) :
		range_{ forward<Range>(range) },
		splitters_range_{
			array_without_extensions{ forward<Splitters>(splitters)... }
		}
	{}

	constexpr auto iterator() {
		return split_view_iterator {
			range_.iterator(),
			range_.sentinel(),
			splitters_range_
		};
	}

	constexpr auto sentinel() { return default_sentinel{}; }

	constexpr decltype(auto) operator [] (nuint index) const {
		return *(iterator() + index);
	}

	constexpr decltype(auto) operator [] (nuint index) {
		return *(iterator() + index);
	}

};

template<
	basic_range Range,
	range_of<decay<range_element_type<Range>>> SplittersRange
>
split_view(Range&&, SplittersRange&&) -> split_view<Range, SplittersRange>;

template<
	basic_range Range,
	typename... Splitters
>
split_view(Range&&, Splitters&&... splitters)
	-> split_view<
		Range,
		decltype(array_without_extensions{ forward<Splitters>(splitters)... })
	>;

} // __range