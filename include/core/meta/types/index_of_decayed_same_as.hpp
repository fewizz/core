#pragma once

#include "index_of_satisfying_predicate.hpp"
#include "../type/is_decayed_same_as.hpp"

namespace types {

	template<typename Type>
	struct index_of_decayed_same_as
		: types::index_of_satisfying_predicate<
			type::is_decayed_same_as_type<Type>
		> {};

}