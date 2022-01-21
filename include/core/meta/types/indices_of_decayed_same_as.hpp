#pragma once

#include "indices_of_satisfying_predicate.hpp"
#include "../type/is_decayed_same_as.hpp"

namespace types {

	template<typename Type>
	struct indices_of_type : types::indices_of_satisfying_predicate<
		type::is_decayed_same_as_type<Type>
	> {};

}