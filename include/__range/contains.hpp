#pragma once

#include "./basic.hpp"
#include "../forward.hpp"

namespace __range {

template<basic_range Range, typename Element>
constexpr bool contains(Range&& range, Element&& element) {
	for(auto&& v : forward<Range>(range)) {
		if(v == element) return true;
	}
	return false;
}

} // __range