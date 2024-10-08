#pragma once

#include "./basic.hpp"
#include "./default_sentinel.hpp"
#include "./iterator.hpp"
#include "./sentinel.hpp"
#include "./extensions.hpp"
#include "../__iterator/basic.hpp"
#include "../__iterator/element_type.hpp"

namespace __range {

template<basic_iterator Iterator, typename Sentinel, typename Predicate>
class filter_view_iterator {
	Iterator iterator_;
	Sentinel sentinel_;
	Predicate& predicate;
	using element_type = iterator_element_type<Iterator>;

	constexpr void skip() {
		while (iterator_ != sentinel_ && !predicate(*iterator_)) {
			++iterator_;
		}
	}

public:

	constexpr filter_view_iterator(
		Iterator iterator, Sentinel sentinel, Predicate& predicate
	) :
		iterator_{ iterator }, sentinel_{ sentinel }, predicate{ predicate }
	{
		skip();
	}

	constexpr decltype(auto) operator * (this auto&& self) {
		return *(self.iterator_);
	}

	constexpr filter_view_iterator& operator ++ () {
		++iterator_;
		skip();
		return *this;
	}

	constexpr friend bool operator == (
		filter_view_iterator it, default_sentinel
	) { return it.iterator_ == it.sentinel_; }
	constexpr friend bool operator == (
		default_sentinel, filter_view_iterator it
	) { return it.iterator_ == it.sentinel_; }

};

template<basic_range Range, typename Predicate>
class filter_view :
	public range_extensions<filter_view<Range, Predicate>>,
	public range_element_index_type_mark<range_element_index_type<Range>>
{
	Range range_;
	Predicate predicate_;
public:

	constexpr filter_view(Range&& range, Predicate&& predicate) :
		range_{ forward<Range>(range) },
		predicate_{ forward<Predicate>(predicate) }
	{}

	constexpr auto iterator(this auto&& self) {
		return filter_view_iterator {
			range_iterator(self.range_),
			range_sentinel(self.range_),
			self.predicate_
		};
	}

	constexpr default_sentinel sentinel() const { return {}; }

};

template<basic_range Range, typename Predicate>
filter_view(Range&&, Predicate&&) -> filter_view<Range, Predicate>;

} // __range