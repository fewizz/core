#pragma once

#include "are_types_same.hpp"
#include "value.hpp"

template<typename Type0>
struct is_type_same {
	template<typename Type1>
	static constexpr bool for_type_of = are_types_same<Type0, Type1>;
};