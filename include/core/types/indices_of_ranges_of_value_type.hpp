#pragma once

#include "indices_of_satisfying_predicate.hpp"
#include "../range/of_value_type.hpp"

namespace types {

	template<typename ValueType>
	struct indices_of_ranges_of_value_type {

		template<typename... Types>
		using for_types_of = typename types::indices_of_satisfying_predicate<
				type::is_range_of_value_type<ValueType>
			>::template for_types_of<Types...>;
	};

}