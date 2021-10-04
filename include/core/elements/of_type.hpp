#pragma once

#include "at_index.hpp"
#include "../types/indices_of_type.hpp"
#include "../values/at_index.hpp"

namespace elements {

	template<typename Type>
	struct of_type {
	
		template<typename... Types>
		static constexpr decltype(auto) for_elements_of(Types&&... elements) {
			using indices = typename types::indices_of_type<Type>::template for_types_of<Types...>;
	
			static_assert(indices::size == 1u);
	
			constexpr auto index = indices::template pass_for_value<values::at_index<0u>>;
	
			return elements::at_index<index>::template for_elements_of<Types...>(forward<Types>(elements)...);
		}
	};

}