#pragma once

namespace value {

	template<auto Value>
	struct is_equal_to {

		static constexpr bool is_value_predicate = true;

		template<auto EqualValue>
		static constexpr bool for_value_of = Value == EqualValue;
	};

}