#pragma once

#include "indices_of_type.hpp"

template<typename Type>
struct contain_type {

	template<typename... Types>
	static constexpr bool for_types_of = ! indices_of_type<Type>::template for_types_of<Types...>::empty;
};