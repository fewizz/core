#pragma once

#include "indices_of_satisfying_predicate.hpp"
#include "../value/is_equal_to.hpp"

namespace values {
	
	template<auto Value>
	struct indices_of_value {

		template<auto... Values>
		using for_values = typename values::indices_of_satisfying_predicate<
			value::is_equal_to<Value>
		>::template for_values<Values...>;

	};

}