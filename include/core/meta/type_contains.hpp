#pragma once

#include "indices_of_types_same_as.hpp"

template<typename Type>
struct type_same_as {

	template<typename... Types>
	using contains_in = typename indices_of_types_same_as<Type>::template in<Types...>;
};