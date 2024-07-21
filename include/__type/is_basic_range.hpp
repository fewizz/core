#pragma once

#include "predicate_extensions.hpp"
#include "../__range/basic.hpp"


struct is_basic_range_t :
	type_predicate_extension<is_basic_range_t>
{

	template<typename Type>
	constexpr bool for_type() const {
		return ::basic_range<Type>;
	}

};

constexpr is_basic_range_t is_basic_range{};