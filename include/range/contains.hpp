#pragma once

#include "./basic.hpp"
#include "./forward.hpp"

template<basic_range Range, typename Value>
constexpr bool range_contains(Range&& range, Value&& value) {
	for(auto&& v : forward<Range>(range)) {
		if(v == value) return true;
	}
	return false;
}