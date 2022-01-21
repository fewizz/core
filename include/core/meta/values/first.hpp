#pragma once

#include "of.hpp"

namespace values {

	struct first {

		template<auto FirstValue, auto... TailValues>
		static constexpr auto for_values = FirstValue;

	};

}