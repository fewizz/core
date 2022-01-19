#pragma once

#include "for_each_satisfying_type_predicate.hpp"
#include "../type/is_same_as.hpp"

namespace elements {

	template<typename Type>
	inline constexpr auto for_each_of_type = elements::for_each_satisfying_type_predicate<type::modified_predicate<type::is_same_as<Type>, type::decay>>;

}