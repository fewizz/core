#pragma once

#include "indices_of_type.hpp"
#include "at_indices.hpp"
#include "../type/remove_reference.hpp"

namespace types {

	template<typename Type>
	struct are_contain_type {
		static constexpr bool is_types_predicate = true;

		template<typename... Types>
		using indices_of_affected_types_of = typename indices_of_type<Type>::template for_types_of<Types...>;

		template<typename... Types>
		static constexpr bool for_types_of = ! indices_of_affected_types_of<Types...>::is_empty;

		using remove_reference = are_contain_type<type::remove_reference::for_type_of<Type>>;
	};

}