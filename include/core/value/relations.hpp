#pragma once

namespace value {

	template<auto Value>
	struct equals_to {
		static constexpr bool is_value_predicate = true;

		template<auto OtherValue>
		static constexpr bool for_value_of = Value == OtherValue;
	};
}