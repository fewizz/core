#pragma once

#include "./of.hpp"

namespace __values {

	struct first {
		template<auto FirstValue, auto... TailValues>
		static constexpr auto for_values = FirstValue;
	};

}

template<auto FirstValue, auto... TailValues>
static constexpr auto first_of_values = FirstValue;