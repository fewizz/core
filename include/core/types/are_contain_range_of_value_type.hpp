#pragma once

#include "predicate.hpp"
#include "../range/of_value_type.hpp"
#include "indices_of_satisfying_predicate.hpp"

namespace types {

	template<typename ValueType>
	struct are_contain_range_of_value_type : types::predicate_marker {
		
		template<typename... Types>
		static constexpr bool for_types_of =
			!
			types::indices_of_satisfying_predicate<
				type::is_range_of_value_type<ValueType>
			>
			::template for_types_of<Types...>
			::is_empty;
	};

}