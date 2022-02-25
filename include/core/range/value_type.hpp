#pragma once

#include "../meta/type/of.hpp"
#include "basic.hpp"

namespace range {

	template<typename Type>
	requires range::basic<remove_reference<remove_const<Type>>>
	using value_type = typename remove_reference<remove_const<Type>>::value_type;

}