#pragma once

#include "indices_of_value.hpp"

namespace values {

	template<auto Value>
	struct are_contain_value {
		static constexpr bool is_value_predicate = true;

		template<auto... Values>
		using indices_of_same_types = typename indices_of_value<Value>::template for_values_of<Values...>;

		template<auto... Values>
		static constexpr bool for_values_of = ! indices_of_same_types<Values...>::is_empty;
	};
}