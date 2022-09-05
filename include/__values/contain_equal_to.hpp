#pragma once

#include "./indices_of_equal_to.hpp"

namespace __values {

	template<auto Value>
	struct contain_equal_to {

		template<auto... Values>
		static constexpr bool for_values =
			! indices_of_equal_to<Value>::template
			for_values<Values...>::is_empty;

	};

}