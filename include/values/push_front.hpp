#pragma once

#include "of.hpp"

namespace values {

	template<auto Value>
	struct push_front {

		template<auto... Values>
		using for_values = values::of<Value, Values...>;

	};

} // values