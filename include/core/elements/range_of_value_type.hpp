#pragma once

#include "at_index.hpp"
#include "../types/index_of_range_of_value_type.hpp"

namespace elements {


	template<typename ValueType>
	struct range_of_value_type {

		template<typename... Types>
		static constexpr auto& for_elements_of(Types&... elements) {
			constexpr nuint index = types::index_of_range_of_value_type<ValueType>::template for_types_of<Types...>;
			return elements::at_index<index>::for_elements_of(elements...);
		}
	};

}