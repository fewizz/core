#pragma once

#include "indices_of_value.hpp"

namespace values {

	template<auto Value>
	struct are_contain_value {

		template<auto... Values>
		using indices_of_same_types = typename indices_of_value<Value>::template for_values<Values...>;

		template<auto... Values>
		static constexpr bool for_values = ! indices_of_same_types<Values...>::is_empty;

	};

}