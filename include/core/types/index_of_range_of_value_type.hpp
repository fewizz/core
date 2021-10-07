#pragma once

#include "indices_of_ranges_of_value_type.hpp"
#include "../values/at_index.hpp"
#include "at_index.hpp"

namespace types {

	template<typename ValueType>
	class index_of_range_of_value_type {

		template<typename... Types>
		struct for_types {

			using indices = typename types::indices_of_ranges_of_value_type<ValueType>::template for_types_of<Types...>;

			static_assert(indices::size == 1u);

			static constexpr uint result = indices::template pass_for_value<values::at_index<0u>>;
		};
		
	public:

		template<typename... Types>
		static constexpr uint for_types_of = for_types<Types...>::result;

	};

}