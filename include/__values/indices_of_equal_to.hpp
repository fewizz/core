#pragma once

#include "indices_of_satisfying_predicate.hpp"
#include "../__value/equals_to.hpp"

namespace __values {
	
	template<auto Value>
	struct indices_of_equal_to {

		template<auto... Values>
		using for_values = typename __values::indices_of_satisfying_predicate<
			equals_to<Value>
		>::template for_values<Values...>;

	};

}