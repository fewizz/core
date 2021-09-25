#pragma once

#include "indices_of_types_satisfying_predicate.hpp"
#include "is_type_same_as.hpp"
#include "types_contain_type_same_as.hpp"

template<typename... TypesToErase>
struct erase_types_same_as_one_of {

	template<typename... Types>
	using for_types_of = ;
};