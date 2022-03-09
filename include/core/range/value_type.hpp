#pragma once

#include "core/meta/type/decay.hpp"
#include "basic.hpp"

namespace range {

	template<typename Type>
	requires range::basic<decay<Type>>
	using value_type = typename decay<Type>::value_type;

}