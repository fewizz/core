#pragma once

#include "../__range/sized.hpp"
#include "./for_each_elements_check_if_equal.hpp"
#include "./have_equal_size_and_elements.hpp"

namespace __ranges {

template<basic_range... Ranges>
constexpr bool have_equal_elements(Ranges&&... ranges) {
	if constexpr((sized_range<Ranges> && ...)) {
		return __ranges::have_equal_size_and_elements(
			forward<Ranges>(ranges)...
		);
	}
	else {
		return __ranges::for_each_elements_check_if_equal(
			forward<Ranges>(ranges)...
		);
	}
}

}