#pragma once

#include "count_of_type.hpp"

namespace types {

	template<typename Type>
	struct are_contain_one_type : types::count_of_type<Type>::template equals<1> {};

}