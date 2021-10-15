#pragma once

#include "satisfying_predicate.hpp"
#include "../type/is_same_as.hpp"

namespace elements {

	template<typename Type>
	struct of_type : elements::satisfying_predicate<type::is_same_as<Type>> {

		using remove_reference = elements::satisfying_predicate<typename type::is_same_as<Type>::remove_reference>;
	};

}