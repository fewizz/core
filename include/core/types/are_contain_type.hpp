#pragma once

#include "indices_of_type.hpp"
#include "at_indices.hpp"

namespace types {

	template<typename Type>
	struct are_contain_type {
		static constexpr bool is_types_predicate = true;

		template<typename... Types>
		using indices_of_affected_types_of = typename indices_of_type<Type>::template for_types_of<Types...>;

		template<typename... Types>
		static constexpr bool for_types_of = ! indices_of_affected_types_of<Types...>::is_empty;
	};

}