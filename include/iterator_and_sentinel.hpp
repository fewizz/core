#pragma once

#include "./__iterator/basic.hpp"  // IWYU pragma: export
#include "./__iterator_and_sentinel/get_or_compute_distance.hpp"  // IWYU pragma: export
#include "./__iterator_and_sentinel/as_range.hpp"  // IWYU pragma: export

template<basic_iterator Iterator, typename Sentinel>
struct iterator_and_sentinel {
private:
	Iterator iterator_;
	Sentinel sentinel_;
public:

	constexpr iterator_and_sentinel(Iterator iterator, Sentinel sentinel) :
		iterator_{ iterator }, sentinel_{ sentinel }
	{}

	constexpr nuint get_or_compute_distance() const {
		return __iterator_and_sentinel::get_or_compute_distance(
			iterator_, sentinel_
		);
	}

	constexpr nuint distance() const
	requires requires (Iterator i, Sentinel s) { s - i; } {
		return sentinel_ - iterator_;
	}

	constexpr auto as_range() const {
		return __iterator_and_sentinel::as_range(iterator_, sentinel_);
	}

};