#pragma once

#include "./basic.hpp"
#include "./sentinel.hpp"
#include "./element_type.hpp"
#include "./default_sentinel.hpp"
#include "../__iterator/basic.hpp"
#include "../__iterator/element_type.hpp"

namespace __range {

template<basic_iterator Iterator, typename Sentinel>
class flat_view_iterator {
	using internal_range_type = iterator_element_type<Iterator>;
	using internal_iteartor_type = range_iterator_type<internal_range_type>;
	using internal_sentinel_type = range_sentinel_type<internal_range_type>;

	Iterator iterator_;
	Sentinel sentinel_;
	internal_iteartor_type internal_iterator_;
	internal_sentinel_type internal_sentinel_;

	// only when iterator_ != sentinel_
	constexpr void skip_empty() {
		while(internal_iterator_ == internal_sentinel_) {
			++iterator_;
			if(iterator_ == sentinel_) return;

			internal_iterator_ = range_iterator(*iterator_);
			internal_sentinel_ = range_sentinel(*iterator_);
		}
	}
public:

	constexpr flat_view_iterator(
		Iterator iterator, Sentinel sentinel
	) :
		iterator_{ iterator },
		sentinel_{ sentinel }
	{
		if(iterator_ != sentinel) {
			internal_iterator_ = range_iterator(*iterator);
			internal_sentinel_ = range_sentinel(*iterator);
			skip_empty();
		}
	}

	constexpr decltype(auto) operator * () {
		return *internal_iterator_;
	}

	constexpr flat_view_iterator& operator ++ () {
		++internal_iterator_;
		skip_empty();
		return *this;
	}

	constexpr friend bool operator == (
		flat_view_iterator it, default_sentinel
	) { return it.iterator_ == it.sentinel_; }
	constexpr friend bool operator == (
		default_sentinel, flat_view_iterator it
	) { return it.iterator_ == it.sentinel_; }

};

template<basic_range Range>
requires basic_range<range_element_type<Range>>
class flat_view {
	Range range_;
public:

	constexpr flat_view(Range&& range) : range_{ forward<Range>(range) } {}

	constexpr auto iterator() const {
		return flat_view_iterator {
			range_iterator(range_), range_sentinel(range_)
		};
	}
	constexpr auto iterator()       {
		return flat_view_iterator {
			range_iterator(range_), range_sentinel(range_)
		};
	}

	constexpr auto sentinel() const { return default_sentinel{}; }
	constexpr auto sentinel()       { return default_sentinel{}; }
};

template<basic_range Range>
flat_view(Range&&) -> flat_view<Range>;

} // __range