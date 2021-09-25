#pragma once

#include "is_type_same_as.hpp"
#include "indices_of_types_satisfying_predicate.hpp"

template<typename Type>
struct indices_of_type {

	template<typename... Types>
	using for_types_of = typename indices_of_types_satisfying_predicate<
		is_type_same<Type>
	>::template for_types_of<Types...>;
};