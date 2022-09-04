#pragma once

#include "./predicate.hpp"

namespace __type {

	template<typename Type>
	inline constexpr bool is_array = __is_array(Type);

}

struct is_array : type_predicate_extension<is_array> {

	template<typename Type>
	constexpr bool for_type() const { return __is_array(Type); }

};