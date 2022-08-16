#pragma once

#include "../integer.hpp"
#include "../__iterator/basic.hpp"
#include "../__range/iterator.hpp"
#include "../__range/sentinel.hpp"
#include "../__range/default_sentinel.hpp"

namespace __iterator_and_sentinel {

template<basic_iterator Iterator, typename Sentinel>
class range_on_iterator_and_sentinel {
	Iterator iterator_;
	Sentinel sentinel_;
public:

	constexpr range_on_iterator_and_sentinel(
		Iterator iterator, Sentinel sentinel
	) :
		iterator_{ iterator },
		sentinel_{ sentinel }
	{}

	constexpr Iterator iterator() const { return iterator_; }
	constexpr Sentinel sentinel() const { return sentinel_; }

	constexpr decltype(auto) operator [] (nuint index) const {
		return iterator_[index];
	}

	constexpr decltype(auto) operator [] (nuint index) {
		return iterator_[index];
	}

	// TODO if contiguous
	constexpr nuint size() const { return sentinel_ - iterator_; }
};

template<basic_iterator Iterator, typename Sentinel = default_sentinel>
constexpr range_on_iterator_and_sentinel<Iterator, Sentinel> to_range(
	Iterator iterator, Sentinel sentinel = {}
) {
	return { iterator, sentinel };
}

} // __iteartor_and_sentinel