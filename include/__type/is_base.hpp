#pragma once

#include "./predicate.hpp"

template<typename Type, typename BaseType>
concept base_of = __is_base_of(BaseType, Type);

template<typename BaseType>
struct is_base_of_t :
	type_predicate_extension<is_base_of_t<BaseType>>
{
	template<typename Type>
	constexpr bool for_type() const {
		return __is_base_of(BaseType, Type);
	}
};

template<typename BaseType>
constexpr inline is_base_of_t<BaseType> is_base_of{};