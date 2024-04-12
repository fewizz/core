#pragma once

#include "./borrowed.hpp"
#include "./basic.hpp"
#include "./iterator.hpp"
#include "./sentinel.hpp"
#include "./extensions.hpp"

namespace __range {

template<basic_range Range>
class sized_view : public range_extensions<sized_view<Range>> {
	Range range_;
	nuint size_;
public:

	static constexpr bool is_borrowed_range = borrowed_range<Range>;

	constexpr sized_view(Range&& range, nuint size) :
		range_{ forward<Range>(range) },
		size_ { size }
	{}

	constexpr nuint size() const { return size_; }

	constexpr auto iterator() const { return range_iterator(range_); }
	constexpr auto iterator()       { return range_iterator(range_); }

	constexpr auto sentinel() const { return range_sentinel(range_); }
	constexpr auto sentinel()       { return range_sentinel(range_); }
};

template<basic_range Range>
sized_view(Range&&, nuint size) -> sized_view<Range>;

} // __range