#pragma once

#include "./predicate_extensions.hpp"

template<typename Type, typename BaseType>
concept derived_from = __is_base_of(BaseType, Type);

template<typename BaseType>
struct is_derived_from_t :
	type_predicate_extensions<is_derived_from_t<BaseType>>
{
	template<typename Type>
	constexpr bool for_type() const {
		return __is_base_of(BaseType, Type);
	}
};

template<typename BaseType>
constexpr inline is_derived_from_t<BaseType> is_derived_from{};