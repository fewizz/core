#pragma once

#include "satisfying_predicate.hpp"
#include "../type/is_same_as.hpp"

namespace elements {

	template<typename Type>
	struct of_type : elements::satisfying_predicate<type::is_same_as<Type>> {

		using ignore_reference = elements::satisfying_predicate<typename type::is_same_as<Type>::ignore_reference>;
	};

}