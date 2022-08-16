#pragma once

#include "./sized.hpp"
#include "./size.hpp"
#include "../__iterator/basic.hpp"
#include "../forward.hpp"

namespace __range {

template<basic_iterator Iterator>
struct reverse_view_iterator {
private:
	Iterator iterator_;
public:

	reverse_view_iterator(Iterator iterator) : iterator_{ iterator } {}

	decltype(auto) operator * () {
		Iterator cpy = iterator_;
		--cpy;
		return *cpy;
	}

	reverse_view_iterator& operator ++ () {
		--iterator_;
		return *this;
	}

	bool operator == (reverse_view_iterator other) const {
		return other.iterator_ == iterator_;
	}

	bool operator != (reverse_view_iterator other) const {
		return other.iterator_ != iterator_;
	}

};

template<sized_range Range>
struct reverse_view {
private:
	Range range_;
public:

	reverse_view(Range&& range): range_{ forward<Range>(range) } {}

	auto iterator() {
		auto it   = range_iterator(range_);
		auto size = range_size(range_);
		// avoid applying non-zero offset  to null pointer
		return reverse_view_iterator {
			size == 0 ? it : it + size
		};
	}

	auto sentinel() {
		return reverse_view_iterator {
			range_iterator(range_)
		};
	}

	auto size() {
		return range_.size();
	}

};

template<sized_range Range>
reverse_view(Range&&) -> reverse_view<Range>;

} // __range