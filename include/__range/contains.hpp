#pragma once

#include "./basic.hpp"
#include "./forward.hpp"

namespace __range {

template<basic_range Range, typename Value>
constexpr bool contains(Range&& range, Value&& value) {
	for(auto&& v : forward<Range>(range)) {
		if(v == value) return true;
	}
	return false;
}

} // __range