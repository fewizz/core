#pragma once

#include "same.hpp"
#include "satisfying.hpp"

template<typename Type>
struct indices_of_same_as {

	template<typename... Types>
	using of_types = typename indices_of_satisfying<
		is_same<Type>::template as_predicate
	>::template of_types<Types...>;
};