#pragma once

#include "./basic.hpp"
#include "./iterator.hpp"
#include "./sentinel.hpp"
#include "../__iterator_and_sentinel/distance.hpp"

namespace __range {

template<basic_range Range>
class sized_view {
	Range range_;
	nuint size_;
public:

	constexpr sized_view(Range&& range) :
		range_{ forward<Range>(range) },
		size_ {
			__iterator_and_sentinel::distance(
				range_iterator(range_),
				range_sentinel(range_)
			)
		}
	{}

	constexpr nuint size() const { return size_; }

	constexpr auto iterator() const { return range_iterator(range_); }
	constexpr auto iterator()       { return range_iterator(range_); }

	constexpr auto sentinel() const { return range_sentinel(range_); }
	constexpr auto sentinel()       { return range_sentinel(range_); }

};

template<basic_range Range>
sized_view(Range&&) -> sized_view<Range>;

} // __range