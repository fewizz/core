#pragma once

#include "indices_of_value.hpp"

namespace values {

	template<auto Value>
	struct are_contain_value {

		template<auto... Values>
		static constexpr bool for_values = ! indices_of_value<Value>::template for_values<Values...>::is_empty;

	};

}