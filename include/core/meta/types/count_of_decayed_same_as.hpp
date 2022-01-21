#pragma once

#include "count_of_satisfying_predicate.hpp"
#include "../type/is_decayed_same_as.hpp"

namespace types {

	template<typename Type>
	struct count_of_decayed_same_as
		: types::count_of_satisfying_predicate<type::is_decayed_same_as_type<Type>> {};

}