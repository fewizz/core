#pragma once

#include "count_of_satisfying_predicate.hpp"
#include "../type/is_same_as.hpp"

namespace types {

	template<typename Type>
	struct count_of_type : types::count_of_satisfying_predicate<type::is_same_as<Type>> {

		struct remove_reference : types::count_of_satisfying_predicate<typename type::is_same_as<Type>::remove_reference> {};

	};

}