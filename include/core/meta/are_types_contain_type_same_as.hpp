#pragma once

#include "indices_of_types_same_as.hpp"

template<typename Type>
struct are_types_contain_type_same_as {

	template<typename... Types>
	using of = typename indices_of_types_same_as<Type>::template of<Types...>;
};