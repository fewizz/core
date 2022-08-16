#pragma once

#include "./__iterator/basic.hpp"
#include "./__iterator_and_sentinel/distance.hpp"
#include "./__iterator_and_sentinel/to_range.hpp"
#include "./__range/iterator.hpp"
#include "./__range/sentinel.hpp"

namespace __iterator_and_sentinel {
	using ::begin;
	using ::end;
}

template<basic_iterator Iterator, typename Sentinel>
struct iterator_and_sentinel {
private:
	Iterator iterator_;
	Sentinel sentinel_;
public:

	constexpr iterator_and_sentinel(Iterator iterator, Sentinel sentinel) :
		iterator_{ iterator }, sentinel_{ sentinel }
	{}

	constexpr nuint distance() const {
		return __iterator_and_sentinel::distance(iterator_, sentinel_);
	}

	constexpr auto to_range() const {
		return __iterator_and_sentinel::to_range(iterator_, sentinel_);
	}

};