#pragma once

#include "../type/is_same_as.hpp"
#include "indices_of_satisfying_predicate.hpp"

namespace types {

	template<typename Type>
	struct indices_of_type {

		template<typename... Types>
		using for_types_of = typename types::indices_of_satisfying_predicate<
			type::is_same_as<Type>
		>::template for_types_of<Types...>;

	};

}