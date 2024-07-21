#pragma once

#include "./predicate_extensions.hpp"

template<typename Type, typename DerivedType>
concept base_of = __is_base_of(Type, DerivedType);

template<typename Type>
struct is_base_of_t :
	type_predicate_extension<is_base_of_t<Type>>
{
	template<typename DerivedType>
	constexpr bool for_type() const {
		return __is_base_of(Type, DerivedType);
	}
};

template<typename DerivedType>
constexpr inline is_base_of_t<DerivedType> is_base_of{};