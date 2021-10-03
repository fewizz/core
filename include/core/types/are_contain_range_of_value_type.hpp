#pragma once

#include "../type/range.hpp"
#include "indices_of_satisfying_predicate.hpp"

namespace types {

	template<typename ValueType>
	struct are_contain_range_of_value_type {
		static constexpr bool is_types_predicate = true;
		
		template<typename... Types>
		static constexpr bool for_types_of =
			!
			types::indices_of_satisfying_predicate<
				type::is_range_of_value_type<ValueType>
			>
			::template for_types_of<Types...>
			::empty;
	};

}