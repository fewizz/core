#pragma once

#include "integer.hpp"
#include "range/basic.hpp"

namespace iterators {

	// TODO optimized versions

	template<typename Iterator, typename Sentinel>
	constexpr nuint distance(Iterator iterator, Sentinel sentinel) {
		nuint d = 0; // TODO size_type

		while(iterator != sentinel) {
			++d;
			++iterator;
		}

		return d;
	}

} // iterators