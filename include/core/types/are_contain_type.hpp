#pragma once

#include "indices_of_type.hpp"

namespace types {

	template<typename Type>
	struct are_contain_type {
		static constexpr bool is_types_predicate = true;

		template<typename... Types>
		static constexpr bool for_types_of = ! indices_of_type<Type>::template for_types_of<Types...>::empty;
	};

}