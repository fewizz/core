#pragma once

#include "./predicate.hpp"

template<typename Type>
inline constexpr bool type_is_array = __is_array(Type);

struct is_array : type_predicate_extension<is_array> {

	template<typename Type>
	constexpr bool for_type() const { return __is_array(Type); }

};