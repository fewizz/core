#pragma once

#include "predicate.hpp"

namespace value {

	template<auto Value>
	struct is_equal_to : value::predicate_marker {

		template<auto EqualValue>
		static constexpr bool for_value = Value == EqualValue;

	};

}