#pragma once

#include "of.hpp"

namespace __values {

	template<auto Value>
	struct push_front {

		template<auto... Values>
		using for_values = __values::of<Value, Values...>;

	};

}