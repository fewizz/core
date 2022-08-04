#pragma once

#include "./element_type.hpp"

namespace range {

	template<typename Type>
	using value_type = decay<range::element_type<Type>>;

} // range