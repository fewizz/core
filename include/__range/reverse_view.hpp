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

	decltype(auto) operator * () { return *iterator_; }

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
		return reverse_view_iterator {
			(range_iterator(range_) + range_size(range_)) - 1
		};
	}

	auto sentinel() {
		return reverse_view_iterator {
			range_iterator(range_) - 1
		};
	}

};

template<sized_range Range>
reverse_view(Range&&) -> reverse_view<Range>;

} // __range