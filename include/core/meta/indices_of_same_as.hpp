#pragma once

#include "indices.hpp"
#include "indices_of_satisfying.hpp"
#include "are_types_same.hpp"
#include "is_same.hpp"

template<typename Type>
struct indices_of_same_as {

	template<typename... Types>
	using of_types = typename indices_of_satisfying<
		is_same<Type>::template as_predicate
	>::template of_types<Types...>;
};