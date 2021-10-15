#pragma once

#include "../type/is_same_as.hpp"
#include "indices_of_satisfying_predicate.hpp"

namespace types {

	template<typename Type>
	struct indices_of_type : types::indices_of_satisfying_predicate<type::is_same_as<Type>> {

		using remove_reference = types::indices_of_satisfying_predicate<typename type::is_same_as<Type>::remove_reference>;

	};

}