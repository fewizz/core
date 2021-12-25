#pragma once

#include "satisfying_predicate.hpp"
#include "../type/is_same_as.hpp"
#include "../type/remove_reference.hpp"
#include "../type/remove_const.hpp"

namespace elements {

	template<typename Type>
	struct of_type : elements::satisfying_predicate<type::is_same_as<Type>> {
	};

}