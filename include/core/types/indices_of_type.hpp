#pragma once

#include "../type/is_same_as.hpp"
#include "../type/modifier.hpp"
#include "indices_of_satisfying_predicate.hpp"

namespace types {

	template<typename Type>
	struct indices_of_type : types::indices_of_satisfying_predicate<type::is_same_as<Type>> {

		template<type::modifier Modifier>
		using after_applying = indices_of_type<typename Modifier::template for_type_of<Type>>;
	};

}