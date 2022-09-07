#pragma once

#include "./predicate.hpp"
#include "./is_array.hpp"

struct is_array : type_predicate_extension<is_array> {

	template<typename Type>
	constexpr bool for_type() const { return __is_array(Type); }

};