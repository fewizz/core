#pragma once

#include "./predicate_extensions.hpp"
#include "./is_array.hpp"

struct is_array : type_predicate_extensions<is_array> {

	template<typename Type>
	constexpr bool for_type() const { return __is_array(Type); }

};