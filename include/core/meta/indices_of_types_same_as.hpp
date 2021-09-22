#pragma once

#include "is_type_same_as.hpp"
#include "indices_of_types_satisfying_predicate.hpp"

template<typename Type>
struct indices_of_types_same_as {

	template<typename... Types>
	using of = typename indices_of_types_satisfying_predicate<
		is_type_same<Type>::template as_predicate
	>::template of_types<Types...>;
};